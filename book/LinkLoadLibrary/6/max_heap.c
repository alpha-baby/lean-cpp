#include <stdio.h>
#include <stdlib.h>

unsigned long long maxnum = 0;

int main() {
  unsigned int blocksize[4] = {1024 * 1024 * 1024, 1024 * 1024, 1024, 1};
  int i,count;
  for (i = 0; i < 4; i++) {
    for (count = 1;; count++) {
        void* pmalloc = malloc(maxnum + blocksize[i]*count);
        if (pmalloc) {
            maxnum = maxnum + blocksize[i] * count;
            printf("maxnum = %lu \n", maxnum);
            free(pmalloc);
        }else {
            break;
        }
    }
  }
  printf("maxnum mallc size is %lu bytes\n", maxnum);
}