#include "bf.h"
#include "bv.h"
#include "city.h"
#include "ht.h"
#include "ll.h"
#include "messages.h"
#include "node.h"
#include "parser.h"

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc
#include <unistd.h> // for getopt

#define OPTIONS "ht:f:ms"

int main(int argc, char **argv) {
  bool mtf = false;
  bool stats = false;
  uint32_t hash_size = 10000;
  uint32_t bloom_size = 1 << 19; // 2^19
  bool thoughtcrime = false, rightspeak = false;

  int opt = 0;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      fprintf(stderr,
              "Usage: ./banhammer [options]\n"
              "  ./banhammer will read in words from stdin, identify any "
              "badspeak or old speak and output an\n"
              "  appropriate punishment message. The badspeak and oldspeak "
              "(with the newspeak translation)\n"
              "  that caused the punishment will be printed after the message. "
              "If statistics are enabled\n"
              "  punishment messages are suppressed and only statistics will "
              "be printed.\n"
              "    -t <ht_size>: Hash table size set to <ht_size>. (default: "
              "10000)\n"
              "    -f <bf_size>: Bloom filter size set to <bf_size>. (default "
              "2^19)\n"
              "    -s          : Enables the printing of statistics.\n"
              "    -m          : Enables move-to-front rule.\n"
              "    -h          : Display program synopsis and usage.\n");
      return 0;

    case 't': // specifies the hash table entries
      hash_size = strtoul(optarg, NULL, 10);
      break;

    case 'f': // specifies the bloom filter sizes
      bloom_size = strtoul(optarg, NULL, 10);
      break;

    case 'm': // enabling the move-to-front rule
      mtf = true;
      break;

    case 's': // enabling the printing of statistics to stdout
      stats = true;
      break;

    default:
      fprintf(stderr,
              "Usage: ./banhammer [options]\n"
              "  ./banhammer will read in words from stdin, identify any "
              "badspeak or old speak and output an\n"
              "  appropriate punishment message. The badspeak and oldspeak "
              "(with the newspeak translation)\n"
              "  that caused the punishment will be printed after the message. "
              "If statistics are enabled\n"
              "  punishment messages are suppressed and only statistics will "
              "be printed.\n"
              "    -t <ht_size>: Hash table size set to <ht_size>. (default: "
              "10000)\n"
              "    -f <bf_size>: Bloom filter size set to <bf_size>. (default "
              "2^19)\n"
              "    -s          : Enables the printing of statistics.\n"
              "    -m          : Enables move-to-front rule.\n"
              "    -h          : Display program synopsis and usage.\n");

      return -1;
    }
  }

  BloomFilter *bf = bf_create(bloom_size);
  HashTable *ht = ht_create(hash_size, mtf);
  // read in a list of badspeak words with fgets()

  FILE *badspeak_file = fopen("badspeak.txt", "r");
  char badspeak_words[1024] = {0};
  Parser *p = parser_create(badspeak_file);

  // each badspeak word should be added to the bloom filter and the hash table

  while (next_word(p, badspeak_words)) {
    bf_insert(bf, badspeak_words);
    ht_insert(ht, badspeak_words, NULL);
  }

  // read in a list of oldspeak and newspeak
  FILE *newspeak_file = fopen("newspeak.txt", "r");
  Parser *pp = parser_create(newspeak_file);
  char oldspeak_words[1024];
  char newspeak_words[1024];
  while (next_word(pp, oldspeak_words)) {
    bf_insert(bf, oldspeak_words);
    next_word(pp, newspeak_words);
    ht_insert(ht, oldspeak_words, newspeak_words);
  }

  // read words in from stdin using your parsing module
  char words[1024]; // using 1024
  LinkedList *badspeak = ll_create(mtf);
  LinkedList *oldspeak_newspeak = ll_create(mtf);

  BloomFilter *bf_badspeak = bf_create(1024);
  BloomFilter *bf_oldspeak_newspeak = bf_create(1024);
  Parser *input = parser_create(stdin);
  while (next_word(input, words)) {
    // check to see if it has been added to the Bloom Filter
    if (bf_probe(bf, words)) {
      Node *hash = ht_lookup(ht, words);
      if (hash != NULL) {
        if (hash->newspeak == NULL) { // does not have newspeak translation
          thoughtcrime = true;
          bf_insert(bf_badspeak, words);
          if (ll_lookup(badspeak, words) == NULL) { // prevent repetition
            ll_insert(badspeak, words, NULL);
          }
        }
        if (hash->newspeak != NULL) { // has newspeak translation
          rightspeak = true;
          bf_insert(bf_badspeak, words);
          if (ll_lookup(oldspeak_newspeak, words) ==
              NULL) { // prevent repetition
            ll_insert(oldspeak_newspeak, words, hash->newspeak);
          }
        }
      }
    }
  }

  if (stats) {

    uint32_t ht_nk = 0, ht_nh = 0, ht_nm = 0, ht_ne = 0;
    ht_stats(ht, &ht_nk, &ht_nh, &ht_nm, &ht_ne);
    fprintf(stdout, "ht keys: %u\n", ht_nk);
    fprintf(stdout, "ht hits: %u\n", ht_nh);
    fprintf(stdout, "ht misses: %u\n", ht_nm);
    fprintf(stdout, "ht probes: %u\n", ht_ne);

    uint32_t bf_nk = 0, bf_nh = 0, bf_nm = 0, bf_ne = 0;
    bf_stats(bf, &bf_nk, &bf_nh, &bf_nm, &bf_ne);
    fprintf(stdout, "bf keys: %u\n", bf_nk);
    fprintf(stdout, "bf hits: %u\n", bf_nh);
    fprintf(stdout, "bf misses: %u\n", bf_nm);
    fprintf(stdout, "bf bits examined: %u\n", bf_ne);

    double num = (bf_ne - (5 * bf_nh)); // bit masking
    double per_miss = (bf_nm == 0) ? (0) : (num / bf_nm);
    fprintf(stdout, "Bits examined per miss: %1.6lf\n", per_miss);

    double numerator = ht_nm; // bit masking
    double false_pos = (bf_nh == 0) ? (0) : (numerator / bf_nh);
    fprintf(stdout, "False positives: %1.6lf\n", false_pos);

    double denom = ht_nh + ht_nm; // bit masking
    double seek_length = ((ht_nh + ht_nm) == 0) ? (0) : ((ht_ne) / denom);
    fprintf(stdout, "Average seek length: %1.6lf\n", seek_length);

    double counter = bf_count(bf); // bit masking
    double filter_load = counter / bf_size(bf);
    fprintf(stdout, "Bloom filter load: %1.6lf\n", filter_load);

  } else {

    // if thoughtcrime + rightspeak, they are given mixspeak messages
    if (thoughtcrime && rightspeak) {
      fprintf(stdout, "%s", mixspeak_message);
      ll_print(badspeak);
      ll_print(oldspeak_newspeak);
    }

    // if only thoughtcrime badspeak messages
    if (rightspeak == false && thoughtcrime == true) {
      fprintf(stdout, "%s", badspeak_message);
      ll_print(badspeak);
    }

    // if only rightspeak, then goodspeak messages
    if (rightspeak == true && thoughtcrime == false) {
      fprintf(stdout, "%s", goodspeak_message);
      ll_print(oldspeak_newspeak);
    }
  }

  // cleaning up
  bf_delete(&bf);
  bf_delete(&bf_badspeak);
  bf_delete(&bf_oldspeak_newspeak);
  ht_delete(&ht);
  ll_delete(&badspeak);
  ll_delete(&oldspeak_newspeak);
  parser_delete(&input);
  parser_delete(&p);
  parser_delete(&pp);
  fclose(badspeak_file); // closing all files
  fclose(newspeak_file);
  return 0;
}
