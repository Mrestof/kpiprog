#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKSIZE 512
typedef struct _node {
  struct _node *left, *right;
  unsigned long freq;
  unsigned char c;
} node;
unsigned long freq[256];
node *tree;

int calc_freq(const char *input);
int build_tree();
int compress(const char *input, const char *output);

int main(int argc, char **argv) {
  int res;
  if (argc < 3) {
    printf("usage:  %s  <input_file>  <output_file>\n", argv[0]);
    return -1;
  }
  res = calc_freq(argv[1]);
  if (res != 0) {
    printf("Error  in  calc_freq!  %s\n", strerror(errno));
    return -1;
  }
  for (int i = 0; i < 256; ++i) {
    printf("%d:  %u\n", i, freq[i]);
  }
  return 0;
}

int calc_freq(const char *input) {
  FILE *inp;
  int n;
  unsigned char buff[BLOCKSIZE];
  if ((inp = fopen(input, "rb")) == NULL) {
    return -1;
  }
  do {
    n = fread(buff, BLOCKSIZE, 1, inp);
    for (int i = 0; i < n; ++i) {
      freq[buff[i]]++;
    }
  } while (n);
  return 0;
}

int build_tree() {
  unsigned char indecies[256];
  int i;
  int k = 1;
  node *leaves[256], *tmp;

  for (i = 0; i < 256; i++)
    indecies[i] = i;

  while (k > 0) {
    k = 0;
    for (i = 0; i < 255; i++) {
      if (freq[indecies[i]] > freq[indecies[i + 1]]) {
        indecies[i] = indecies[i] ^ indecies[i + 1];
        indecies[i + 1] = indecies[i] ^ indecies[i + 1];
        indecies[i] = indecies[i] ^ indecies[i + 1];
        k++;
      }
    }
  }

  for (i = 0; i < 256; i++) {
    leaves[indecies[i]] = (node *)malloc(sizeof(node));
    leaves[indecies[i]]->c = i;
    leaves[indecies[i]]->freq = freq[indecies[i]];
  }

  return 0;
}
