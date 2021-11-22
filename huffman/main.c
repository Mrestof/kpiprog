#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKSIZE 512 // size of the buffer block
typedef struct _node {
  struct _node *left, *right;
  unsigned long freq;
  unsigned char c;
} node;
unsigned long freq[256]; // arr of frequencies, index corresponds to char code
node *tree;

int calc_freq(const char *input);
int build_tree();
int compress(const char *input, const char *output);

int main(int argc, char **argv) {
  int res;
  if (argc < 3) {
    printf("usage:  %s  <input_file>  <output_file>\n", argv[0]);
    return 1;
  }
  res = calc_freq(argv[1]);
  if (res != 0) {
    printf("Error  in  calc_freq!  %s\n", strerror(errno));
    return 1;
  }
  for (int i = 0; i < 256; ++i) { // print out the entire `freq` array
    printf("%d:  %lu\n", i, freq[i]);
  }
  return 0;
}

int calc_freq(const char *input) {
  /* Calculate frequency of each character in char array and write it to the
   * `freq` array.
   */
  FILE *inp;
  int n;
  unsigned char buff[BLOCKSIZE];

  // by default pointer is pointing to NULL
  if ((inp = fopen(input, "rb")) == NULL) { // exit if file can't be opened
    printf("%p\n", (void *)inp);
    printf("%p\n", (void *)&inp);
    return 1;
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
  unsigned char indices[256]; // array of corresponding indices for `freq` array
  int i, k = 1, r = 255, l = 0;
  node *leaves[1024], *tmp;

  for (i = 0; i < 256; i++) {
    indices[i] = i;
    if(freq[i]==0)
      l++;
  }

  while (k > 0) { // sort `indices` arr
    k = 0;
    for (i = 0; i < 255; i++) {
      if (freq[indices[i]] > freq[indices[i + 1]]) {
        indices[i] = indices[i] ^ indices[i + 1];
        indices[i + 1] = indices[i] ^ indices[i + 1];
        indices[i] = indices[i] ^ indices[i + 1];
        k++;
      }
    }
  }

  for (i = 0; i < 256; i++) {
    if (freq[indices[i]] == 0)
      continue;
    r++;
    leaves[indices[i]] = (node *)malloc(sizeof(node));
    leaves[indices[i]]->c = i;
    leaves[indices[i]]->freq = freq[indices[i]];
  }
  
  
  
    while(r!=l){  //Создание дерева
        zz.freq = leaves[l]->freq + leaves[l+1]->freq;
        zz.right=leaves[l+1];
        zz.left=leaves[l];
        l+=2;
        j=r;
        while ((j>l)&&(zz.freq<leaves[j]->freq)){
            leaves[j+1]=leaves[j];
            j--;
        }
        leaves[j]->freq=zz.freq;
        leaves[j]->left=zz.left;
        leaves[j]->right=zz.right;
        r++;
    }

  return 0;
}
