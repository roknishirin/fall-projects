// parser.c

#include "parser.h"
#include <ctype.h>  // for insalum
#include <stdint.h> // for uint32_t
#include <stdlib.h> // for malloc

#define MAX_PARSER_LINE_LENGTH 1000

struct Parser {
  FILE *f;
  char current_line[MAX_PARSER_LINE_LENGTH + 1];
  uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
  Parser *p = (Parser *)calloc(1, sizeof(Parser));
  if (p) {
    p->line_offset = 0;
    p->f = f;
    // fgets(p->current_line, p->line_offset, f);
    // fgets(p->current_line, MAX_PARSER_LINE_LENGTH, f);
  }
  return p;
}

void parser_delete(Parser **p) {
  free(*p);
  *p = NULL;
}

// mimic strlen function
// from lecture 5 slides provided by professor
uint64_t my_strlen(char *s) {
  size_t val = 0;

  do {
    val++;
  } while (s[val] != 0);

  return val;
}

// mimic strncpy()
// from lecture 5 slides provided by professor
void my_strncpy(char *word, char *s, size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    word[i] = tolower(s[i]);
  }
  word[i] = '\0';
}

bool next_word(Parser *p, char *word) {
  // it will find the next valid word in the buffer within the parser and store
  // it in the buffer passed in if the next word was succesfully parsed, then
  // the funtion returns true

  int end = 0;
  int start = 0;

  for (char i = p->current_line[p->line_offset]; (i != EOF);) {

    if (p->line_offset == 0) {
      if ((fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) &&
          (my_strlen(&i) == 1)) {
        return false;
      }
      i = p->current_line[p->line_offset];
    }
    while (i == ('\n')) {
      // goes until  next good line
      if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL)
        return false;
      p->line_offset = 0;
      i = p->current_line[p->line_offset];
    }

    if (i == (' ') || (!(isalnum(i)) && i != ('-') &&
                       i != ('\''))) { // if invalid character + space

      char j = i;

      do { // keep on going through line until valid again
        p->line_offset++;
        j = p->current_line[p->line_offset];
      } while (j != ('\n') && !(isalnum(j)) && j != ('-') && j != ('\'') &&
               j == (' '));

      i = j;
    }

    if (i != (' ') && (isalnum(i) || i == ('-') || i == ('\''))) { // valid word
      start = p->line_offset;
      char j = i;
      while (j != ('\n') &&
             (j != (' ')) && // keep on going through line until invalid again
             ((isalnum(j)) || j == ('-') || j == ('\'') || j == (' '))) {
        j = p->current_line[p->line_offset++];
      }
      i = j;
      end = p->line_offset - 1;

      if (i == ('\n')) {
        p->line_offset = 0;
      }

      break;
    }
  }

  my_strncpy(word, p->current_line + start, end - start);

  if ((end - start) > 0) {
    return true;
  } else {
    return false;
  }
}

// test?
