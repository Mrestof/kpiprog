#include <stdio.h>

typedef struct node_t {
  struct node_t *left, *right;
  int freq;
  char c;
} * node;

int freq[256] = {0};

void make_table(char *s) {
  for (; *s; s++) {
    // todo: fucking complete the frequency checker
    printf("%c ", *s);
  }
}

int main() {
  char input[] = "abcaaaab";
  int input_len = 0;
  while (input[input_len]) {
    printf("%c\n", input[input_len]);
    input_len++;
  }

  make_table(input);

  return 0;
}
