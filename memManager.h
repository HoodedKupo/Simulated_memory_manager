#include <stdio.h>
#include <stdlib.h>




#define ADSZ 2  //address size in bytes
#define MAX_TOTAL_MEMORY 65536  //size of memory 
/*-------------------------------------------------*/

#define MNBK (2*ADSZ) //minimum size of block, also gives allignment
#define MNCK (2*MNBK) //minimum size of chunck

#define HDSZ  (2*ADSZ)  // Header size
 

/*-------------------------------------------------*/

typedef unsigned short RELP; //Relative pointer actual type depends on ADSZ

char * myMemory; //A pointer to a memory section of size MAX_TOTAL_MEMORY
                 //You are to get a valid value for this pointer
                 //You can assign this pointer either statically, dynamically, or using system calls (sbrk)


void initializeMemory(void);

/*
  myMalloc(size_t size)
  Allocates the requested bytes, size, and returns a Pointer to the beginning
  of the allocated memory. If the space cannot be allocated, then 0 is returned.
  PARAMS: size - number of bytes to allocate
  RETURN: a pointer to a newly allocated memory region of the desired size
          or NULL if allocation fails
  PRE: 0 < size < MAX_TOTAL_MEMORY - 76
  POST: region is allocated, and valid pointer is returned 
        OR region is not allocated, and 0 is returned
*/
RELP myMalloc(size_t size);

/*
  myFree(RELP region)
  Frees the memory region at the given pointer, region, that was previously allocated using myMalloc. 
  It gives the chunk of memory containing region back to the allocator, and allows for it to be reused in the future.
  PARAMS: region - a pointer to an allocated memory region 
  RETURN: void
  PRE: 78 <=region < MAX_TOTAL_MEMORY-6
  POST: the chunk containing region is added to the available memory

*/
void myFree(RELP region);


/*
  void outputMemory();
  Output all blocks of Memory and their status 
  PARAMS: none
  RETURN: void
  PRE: myMemory is initialized
  POST: prints on stdout information about all blocks of Memory in the format:
        +---------+-------+------------+-------+
        |  Status | Start | Block Size | End   |
        |         |       |            |       |
        where: 
           Status-is the status of the chunk containing the block
           Start-is the beginning of the block (the beginning of the user data space)
           Block Size-is the size of the block
           End-is the end of the block (the end of the user data space)
*/

void outputMemory(void);
