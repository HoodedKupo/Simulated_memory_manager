# Simulated_memory_manager
An implementation that represents a simple memory manager, includes a malloc and free function.\
the simple implementation does not attempt to fix fragmentation of memory.
## Description
creates a simulated memory bank of size MAX_TOTAL_MEMORY defined in memManager.h, this is currently set to 65536 bytes.\
blocks of unused memory are linked together in a double linked list fashion. The first few bytes of the simulated memory contain the address of the first and last block in the memory\
stored as a hex number. The first few bytes of each unused block contains the address of the next and previous used block as well.\
When memory is requested the system finds the first free block that is of sufficient size then marks that chunk as allocated.\
if the free block is sufficiently larger than the requested block size then the user gets the requested memory size and the remaining block is marked as free.\
When a block if freed all data is cleared then, if there is an adjacent free block, the two blocks are merged into one.
## Usage
compile the program by running the make command.
After compiling memManager.c with memManager.h you must first call initializeMemory() to initialize the simulated memory.\
after this step call:\
Relp myMalloc(size) to get a region of memory with useable memory equal to or greater than size. The size will be larger if the available free chunk is not large enough to form another free chunk after allocation.\
myFree(Relp) to free a region of memory you acquired from myMalloc()\
outputMemory() to display the status of the simulated memory.
## Important Skills
- Low level address manipulation in C.
- Worked with Double Linked List data structure.
- Developed skills with C.

