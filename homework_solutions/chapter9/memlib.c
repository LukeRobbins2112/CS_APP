#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "memlib.h"

// Constants
#define MAX_HEAP (20*(1<<20))  /* 20 MB */
#define PAGE_SIZE 1024 * 4


// Global
static char * mem_heap;
static char * mem_brk;
static char * mem_max_addr;


void mem_init() {

  mem_heap = (char *)malloc(MAX_HEAP);
  mem_brk = (char *)mem_heap;
  mem_max_addr = (char *)(mem_heap + MAX_HEAP);

}

void * mem_sbrk(int heapInc){

  char * old_brk = mem_brk;

  if ((heapInc < 0) || ((mem_brk + heapInc) > mem_max_addr)){
    errno = ENOMEM;
    fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory\n");
    return (void *)-1;
  }

  mem_brk += heapInc;
  return (void *)old_brk;

}
