# Simulated_memory_manager
An implementation that represents a simple memory mannager, includes a malloc and free function.\
the simple implementation does not attempt to fix fragmentation of memory.\
## Description
creates a simulated memory bank of size MAX_TOTAL_MEMORY defined in memManager.h, this is currently set to 65536 bytes.\
blocks of unused memory are linked together in a double linked list fashion. The first few bytes of the simulated memory contain the address of the first and last block in the memory\
stored as a hex number. The first few bytes of each unused block contains the address of the next and previous used block as well.\
When memory is requested the system finds the first free block that is of sufficent size then marks that chunk as allocated.\
if the free block is sufficently larger than the requested block size then the user gets the requested memory size and the remaining block is marked as free.\
When a block if freed all data is cleared then, if there is an adjacent free block, the two blocks are merged into one.\
## Usage
After compiling memManager.c with memManager.h you must first call initializeMemory() to initialize the simulated memory.\
after this step call:\
Relp myMalloc(size) to get a region of memory with usuable memory equal to or greater than size. The size will be larger if the available free chunk is not large enough to form another free chunk after allocation.\
myFree(Relp) to free a reqgion of memory you aquired from myMalloc()\
outputMemory() to display the status of the simulated memory.\

