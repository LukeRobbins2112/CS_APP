
#include "memlib.h"
#include "mm.h"
#include <stdlib.h>

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

#define HDRP(bp) ((char *)bp - DSIZE)
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


// *********************************
// Main Functionality
// *********************************

int mm_init(){

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


// @TODO implement this
static void * coalesce (void * bp){
  return NULL;
}
