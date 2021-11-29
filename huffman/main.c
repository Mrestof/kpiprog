#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKSIZE 512 // size of the buffer block
typedef struct _node {
  struct _node *left, *right;
  unsigned long freq;
  unsigned char c;
  char *code;
} node;
unsigned long freq[256]; // arr of frequencies, index corresponds to char code
node *tree;
int l,r;
char *codes[256];
int calc_freq(const char *input);
node *build_tree();
int compress(node *tmp);

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

  compress(build_tree());
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
    return 1;
  }

  do {
    n = fread(buff, sizeof (char), 1, inp);
    for (int i = 0; i < n; ++i) {
      freq[buff[i]]++;
    }
  } while (n);

  return 0;
}

node *build_tree() { //Готове дерево
  unsigned char indices[256]; // array of corresponding indices for `freq` array
  int i, k = 1, summ=0;
  l=0;
  r=255;
  node *leaves[256*2-1], *tmp;

  for (i = 0; i < 256; i++) {
    indices[i] = i;
    summ+=freq[i];
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

  for (i = 0; i < 511; i++) {
    leaves[i] = (node *) malloc(sizeof(node));
  }
  for (i = 0; i < 256; i++)
  {
      leaves[i]->c = indices[i];
      leaves[i]->freq = freq[indices[i]];
  }

    tmp = (node *)malloc(sizeof(node));
    while(l<r)
    {

      tmp->freq = leaves[l]->freq + leaves[l+1]->freq;
      tmp->right = leaves[l+1];
      tmp->left = leaves[l];
      l+=2;
      i = r;
      while ((tmp->freq < leaves[i]->freq) && (i > l)) //Сдвиг по фазе
      {
        leaves[i+1]->freq = leaves[i]->freq;
        leaves[i+1]->left = leaves[i]->left;
        leaves[i+1]->right = leaves[i]->right;
        i--;
      }

      i++;
      leaves[i]->freq = tmp->freq;
      leaves[i]->left = tmp->left;
      leaves[i]->right = tmp->right;

      r++;
    }
  return tmp;
}

int compress(node *tmp){
  if(tmp->c>0) {
    codes[tmp->c]=malloc(sizeof(tmp->code));
    codes[tmp->c]=tmp->code;
    return 0;
  }
  tmp->right->code = malloc(sizeof(tmp->code)+1);
  tmp->left->code = malloc(sizeof(tmp->code)+1);
  tmp->right->code = tmp->code;
  strncat(tmp->right->code,"0",1);
  tmp->left->code = tmp->code;
  strncat(tmp->left->code,"1",1);
  compress(tmp->right);
  compress(tmp->left);

  return 0;
}