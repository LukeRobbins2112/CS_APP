
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memlib.h"
#include "mm.h"


// *************************************
// Constants & macros
// *************************************

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)    // 4096 bytes

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)bp - WSIZE)
#define FTRP(bp) ((char *)bp + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)bp + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)bp - GET_SIZE(((char *)(bp) - DSIZE)))



// *************************************
// Global variables
// *************************************

static char * heap_listp = 0;

// ***************************
// Helper function prototypes
// ***************************
static void * extend_heap(size_t words);
static void * coalesce(void * bp);
static void place(void * bp, size_t size);
static void * find_fit(size_t size);


// *********************************
// Main Functionality
// *********************************

int mm_init(){

  // Initialize heap itself
  mem_init();

  // Create initial empty heap
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1){
      return -1;
    }

    PUT(heap_listp, 0);
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3*WSIZE), PACK(0, 1));

    // Point heap_listp to the "usable memory" section of the prologue
    // Normally this would be the beginning of the block, but
    // there is no data in this block
    heap_listp += (2*WSIZE);

    // Extend the empty heap
    /* if (extend_heap(CHUNKSIZE/WSIZE) == NULL){ */
    /*   return -1; */
    /* } */

    return 0;
}


void mm_free(void *bp){
  size_t size = GET_SIZE(HDRP(bp));

  PUT(FTRP(bp), PACK(size, 0));
  PUT(HDRP(bp), PACK(size, 0));

  coalesce(bp);
}

void* mm_alloc(size_t size){

  size_t adjustedSize;
  size_t extendSize;
  char *bp;

  if (size == 0){
    return NULL;
  }

  // Adjust block size to include overhead & alignment
  if (size <= DSIZE){
    adjustedSize = 2*DSIZE;
  } else {
    adjustedSize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);
  }

  // Search free list for a fit
  if ((bp = find_fit(adjustedSize)) != NULL){
    place(bp, adjustedSize);
    return bp;
  }

  // NO fit found, request memory
  extendSize = MAX(adjustedSize, CHUNKSIZE);
  if ((bp = extend_heap(extendSize/WSIZE)) == NULL){
    return NULL;
  }

  place(bp, adjustedSize);
  return bp;
}
  

// *****************************
// Helper functions
// *****************************

static void * extend_heap(size_t words){

    char* bp;
    size_t size;

    // Allocate even number of words
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1){
      return NULL;
    }

    // Initialize free block header/footer
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    // Coalesce if possible
    return coalesce(bp);
}



static void * coalesce (void * bp){

  size_t prevAlloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t nextAlloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prevAlloc && nextAlloc){
    return bp;
  }
  else if (prevAlloc && !nextAlloc){
    size_t nextSize = GET_SIZE(HDRP(NEXT_BLKP(bp)));
    size += nextSize;

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  } else if (!prevAlloc && nextAlloc){
    size_t prevSize = GET_SIZE(FTRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  } else {
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    size += GET_SIZE(FTRP(PREV_BLKP(bp)));

    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }
  
  return bp;
}


static void place(void * bp, size_t size){

  size_t prevSize = GET_SIZE(HDRP(bp));

  if (prevSize < size + (2*DSIZE)){
    PUT(HDRP(bp), PACK(prevSize, 1));
    PUT(FTRP(bp), PACK(prevSize, 1));
  } else {

    // Set header with new info
    PUT(HDRP(bp), PACK(size, 1));

    // Create new footer
    PUT(FTRP(bp), PACK(size, 1));

    // Create new header for remaining memory
    PUT(HDRP(NEXT_BLKP(bp)), PACK(prevSize - size, 0));

    // Set old footer with new info
    PUT(FTRP(NEXT_BLKP(bp)), PACK(prevSize - size, 0));
  }

}

static void * find_fit(size_t size){

  void * fitPtr;
    
  for (fitPtr = heap_listp; GET_SIZE(HDRP(fitPtr)) != 0; fitPtr = NEXT_BLKP(fitPtr)){
    if (!GET_ALLOC(HDRP(fitPtr)) && (size <= GET_SIZE(HDRP(fitPtr)))){
	return fitPtr;
      }
  }
    return NULL;
}
