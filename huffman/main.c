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
int l,r, buff_size=0, buff_pos=0;
unsigned char codes[256]={0}, masks[256]={0};
unsigned char *buffer;

int calc_freq(const char *input);
node *build_tree();
void compress(node *tmp, unsigned char code, unsigned char len);
void insert(unsigned char c);

int main(int argc, char **argv) {
  int res;
  FILE *inp, *out;
  char c;
  if (argc < 3) {
    printf("usage:  %s  <input_file>  <output_file>\n", argv[0]);
    return 1;
  }
  res = calc_freq(argv[1]);
  if (res < 0) {
    printf("Error  in  calc_freq!  %s\n", strerror(errno));
    return 1;
  }
  buffer = (unsigned char *)malloc( res );
  compress(build_tree(), 0, 0);

  if ((inp = fopen(argv[1], "rb")) == NULL) { // exit if file can't be opened
    return -1;
  }
  while((c=fgetc(inp)) != EOF)
    insert(c);

  if ((out = fopen(argv[2], "wb")) == NULL) { // exit if file can't be opened
    return -1;
  }
  fwrite(buffer, buff_size+1, 1, out);
  return 0;
}

int calc_freq(const char *input) {
  /* Calculate frequency of each character in char array and write it to the
   * `freq` array.
   */
  FILE *inp;
  int n, size=0;
  unsigned char buff[BLOCKSIZE];

  // by default pointer is pointing to NULL
  if ((inp = fopen(input, "rb")) == NULL) { // exit if file can't be opened
    return -1;
  }

  do {
    n = fread(buff, sizeof (char), 1, inp);
    size+=n;
    for (int i = 0; i < n; ++i) {
      freq[buff[i]]++;
    }
  } while (n);
  fclose(inp);
  return size;
}

node *build_tree() { //Готове дерево
  unsigned char indices[256]; // array of corresponding indices for `freq` array
  int i, k = 1;
  unsigned long summ=0;
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
    leaves[i]->c=0;
    leaves[i]->freq=0;
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
        leaves[i+1]->c=leaves[i]->c;
        i--;
      }

      i++;
      leaves[i]->freq = tmp->freq;
      leaves[i]->left = tmp->left;
      leaves[i]->right = tmp->right;
      leaves[i]->c = tmp->c;

      r++;
    }

  return tmp;
}

void compress(node *tmp, unsigned char code, unsigned char len){
  if(tmp->left==NULL) {
    codes[tmp->c]=code;
    masks[tmp->c]=len;
    return;
  }

  compress(tmp->right, code<<1, len+1);
  compress(tmp->left, (code<<1)+1, len+1);
}

void insert(unsigned char c){
  //buff_pos buff_size buffer[]
  //codes[c], masks[c]
  unsigned char mask=masks[c], code=codes[c], k, x;
  while(mask>0) {
    if (mask < 8 - buff_pos) {
      buffer[buff_size] |= code<<buff_pos;
      buff_pos+=mask;
      mask=0;
    } else {
      k = 8-buff_pos;
      x = ~(0xff<<k);
      x = code & x;
      code >>= k;
      buffer[buff_size] |= x << buff_pos;
      buff_pos = 0;
      buff_size ++;
      mask -= k;
    }
  }
}