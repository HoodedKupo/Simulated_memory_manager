/* ------------------------------------------------------
# Student's Name: Nathan Koop
# Assignment 2 Milestone 2
# Lab Section: X03L, Winter 2021
# CMPUT 201, Winter 2021
# Lab Instructor's Name: Antal Buss
*------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "memManager.h"

/*
  CheckPtr(RELP region)
  a sub function of the myFree function. Is used to check if the region points to a used block.
  PARAMS: region - a pointer to an allocated memory region 
  RETURN: 0 if the region does not point to a used block
          1 if the region does point to a used block.
*/
int CheckPtr(RELP region){
    /* check if region lies within the scope of allocated memory*/
    if(region >= MAX_TOTAL_MEMORY || region < 6){
        printf("Invalid Pointer\n");
        return 0;
    }

    /*check if region points to the first byte of user data in a used chunk*/
    RELP* probe;
    RELP address = 4, blockSize, previousadress = 4;
    /* loop checks if address is less than max memory and if the previous adress is higher than current
     * since the address will overflow once it reaches max memory*/ 
    while(address >= previousadress && address < MAX_TOTAL_MEMORY){
        probe = (RELP*)(myMemory + address);
        blockSize = *probe;
        if(blockSize%2 == 0 && address + 2 == region){
            /*Given region points to a free'd region*/
            return 0;
        }
        if(blockSize%2 == 1){
            blockSize = blockSize-1;
        }
        if(address + 2 == region){
            /* region references this used chunk*/
            return 1;
        }else{
            /*move address to the start of the next chunk*/
            previousadress = address;
            address = address + blockSize;
        }
    }

    /* The region did not point to the start of any used regions */
    return 0;
}

/*
  GetLength(int number)
  calculated the length, or the number of digits, in number. This is a sub function to the outputMemory function
  PARAMS: number - the number that the user wants to get the length of
  RETURN: number of digits, or length, of number.
*/
int GetLength(int number){
    int Length = 0;
    if(number == 0){
        return 1;
    } else if(number < 0){
        number = number*-1;
    }
    while (number!=0){
        number = (number - (number%10))/10;
        Length++;
    }
    return Length;
}

/*
  printSpaces(int MaxSpaces, int strLength)
  prints enough spaces to orient the curser such that printing a string of length strLength will 
  orient it to the left side of the line.
  PARAMS: MaxSpaces - how many spaces are in a line
          strLength - the length of the string you want to orient to the left
  RETURN: void
  PRE: MaxSpaces >= strLength
*/
void printSpaces(int MaxSpaces, int strLength){
    int i = 0;
    for(i = 0; i<MaxSpaces-strLength; i++){
        printf(" ");
    }
}

/*
  initalizeMemory(void)
  creates a simulated memory of 65532 bits. sets up the initial FW and BW links in the header.
  then sets up Fsize and Esize at the begining and end of the memory. a pointer to the start of the simulated memory is stored in myMemory
  PARAMS: region - a pointer to an allocated memory region 
  RETURN: void

*/
void initializeMemory(void){
    RELP* relp;
    myMemory = (char*)malloc(MAX_TOTAL_MEMORY);
    if(myMemory == NULL){return;}
    relp = (RELP*)myMemory;
    *relp = 4;
    *(relp+1) = 4;
    *(relp+2) = MAX_TOTAL_MEMORY-(2*ADSZ);
    *(relp+(MAX_TOTAL_MEMORY/ADSZ) -1) = MAX_TOTAL_MEMORY-(2*ADSZ);
}

/*
  ClearData(RELP relPtr, RELP size)
  clears all data in chunk starting at relPtr and of size size.
  PARAMS: relPtr - the relative location of the start of the chunk to be cleared
          size   - size of the chunk to be cleared.
  RETURN: void
  PRE: relPtr is within bounds of allocated memory.
       relPtr + size does not exceed the bounds of allocated memory.
  POST: all data in the specified chunk is reverted to 0's, except for FW, BW, Fsize, and Esize
*/
void ClearData(RELP relPtr, RELP size){
    int i = 0;
    for(i = 2; i<size; i+=2){
        *((RELP*)(myMemory + relPtr + i)) = 0;
    }
    return;
}

/*
  InsertRegion(RELP FsizePosition, RELP size)
  inserts the chunk starting at FsizePosition into the linked list of free chunks.
  PARAMS: FsizePosition - the start position of the chunk to insert.
          size          - the size of the chunk to insert.
  RETURN: void
  PRE: FsizePosition is within the bounds of allocated memory.
       FsizePosition + size does not exceed the bounds of allocated memory.
  POST: free chunk is placed into the linked list at the appropriate place based on size.
*/
void InsertRegion(RELP FsizePosition, RELP size){
    if(*((RELP*)myMemory) == 0 && *((RELP*)(myMemory +2)) == 0){
        *((RELP*)(myMemory)) = FsizePosition; /*adjust FW link in the header*/
        *((RELP*)(myMemory+FsizePosition)+1) = 0; /*make the FW link in the new free chunk*/
        *((RELP*)(myMemory)+1) = FsizePosition;/*adjust the BW link in the header*/
        *((RELP*)(myMemory+FsizePosition)+2) = 0;/*make the BW link in the new free chunk*/
        return;
    }
    RELP* probe, *previousBlock, defaultValue = 1;//since adresses cannot be in header or odd
    RELP relAdress = *((RELP*)myMemory);
    probe = (RELP*)(myMemory + relAdress);
    previousBlock = &defaultValue;
    while(1==1){
        if(*(probe)<=size){/*get the next address*/
            *previousBlock = relAdress;
            relAdress = *((RELP*)(myMemory+relAdress)+1);
            probe = (RELP*)(myMemory+relAdress);
        }else{
            break;
        }
        if(relAdress == 0){
            /*this is the largest free chunk*/
            *((RELP*)(myMemory + *previousBlock) + 1) = FsizePosition; /*adjust FW link of previous free chunk*/
            *((RELP*)(myMemory+FsizePosition)+1) = 0; /*make the FW link in the new free chunk*/
            *((RELP*)(myMemory)+1) = FsizePosition;/*adjust the BW link in the header*/
            *((RELP*)(myMemory+FsizePosition)+2) = *previousBlock;/*make the BW link in the new free chunk*/
            return;
        }
    }
    if(*previousBlock == 1){
        /*this is the smallest free chunk*/
        *((RELP*)myMemory) = FsizePosition; /*adjust FW link of the header*/
        *((RELP*)(myMemory+FsizePosition)+1) = (RELP)((char*)probe - myMemory); /*make the FW link in the new free chunk*/
        *(probe+2) = FsizePosition;/*adjust the BW link of the next free chunk*/
        *((RELP*)(myMemory+FsizePosition)+2) = 0;/*make the BW link in the new free chunk*/
    }else{
        /*Adjust the FW and BW links*/
        *((RELP*)(myMemory + *previousBlock) + 1) = FsizePosition; /*adjust FW link of previous free chunk*/
        *((RELP*)(myMemory+FsizePosition)+1) = (RELP)((char*)probe - myMemory); /*make the FW link in the new free chunk*/
        *(probe+2) = FsizePosition;/*adjust the BW link of the next free chunk*/
        *((RELP*)(myMemory+FsizePosition)+2) = *previousBlock;/*make the BW link in the new free chunk*/
    }
}

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
RELP myMalloc(size_t size){
    RELP relAdress;
    int LoopFlag = 0; //used to break out of loop after at least one iteration.
    if(size < MNBK){return 0;}
    /* Adjust size to nearest multiple of 4*/
    if(size%MNBK != 0){
        size = size+(MNBK-(size%MNBK));
    }
    RELP* probe;
    probe = (RELP*)myMemory;
    relAdress = *probe;
    probe = (RELP*)(myMemory + relAdress);
    while(1==1){
        /*The value at probe is now the block size*/
        if(*probe >= size + MNBK){
            if(*probe > size + MNCK){ /*split chunk into two chunks.*/
                RELP ChunkSize = *probe;
                *probe = size + 5;// Fsize of allocated chunk.
                *(RELP*)(myMemory + relAdress + size+2) = size + 5; // Esize of allocated chunk.
                *(RELP*)(myMemory + relAdress + *probe-1) = ChunkSize - (size+4); // Fsize of new free chunk.
                *(RELP*)(myMemory + relAdress + *probe-1 + 2) = *(RELP*)(myMemory + relAdress + 2); // FW link of new free chunk.
                *(RELP*)(myMemory + relAdress + *probe-1 + 4) = *(RELP*)(myMemory + relAdress + 4); // BW link of new free chunk.
                *((RELP*)(myMemory + relAdress + ChunkSize)-1) = ChunkSize - (size + 4); // Esize of new free chunk.
                /*Adjust the old free blocks's FW and BW links*/
                RELP FWlink = *(RELP*)(myMemory + relAdress + 2);
                RELP BWLink = *(RELP*)(myMemory + relAdress + 4);
                if(FWlink == 0){/*points to the header*/
                    *((RELP*)(myMemory+2)) = BWLink;
                }else{
                    *((RELP*)(myMemory + FWlink + 4)) = BWLink;
                }
                if(BWLink == 0){
                    *((RELP*)(myMemory)) = FWlink;
                }else{
                    *((RELP*)(myMemory + BWLink + 2)) = FWlink;
                }
                InsertRegion(relAdress + *probe -1,ChunkSize-(size+4));
                /*clear FW and BW in allocated chunk*/
                ClearData(relAdress, size);
                *(RELP*)(myMemory + relAdress + 2) = 0;
                *(RELP*)(myMemory + relAdress + 4) = 0;
                return relAdress+2;
            }else{
                /*allocate all to user*/
                *((RELP*)(myMemory + relAdress + *probe)-1) = *probe + 1; // Esize of allocated chunk.
                *probe = *probe +1;
                /* check if the FW or BW link in the memory header is pointing to this block*/
                if(*(RELP*)myMemory == relAdress){ /* FW points to this block*/
                    *((RELP*)myMemory) = *((RELP*)(myMemory + relAdress) + 1);
                    //LoopFlag = 1;
                }else{
                    *((RELP*)(myMemory + *(RELP*)(myMemory + relAdress + 4))+1) = *(RELP*)(myMemory + relAdress + 2); // Adjust FW link
                }
                if(*((RELP*)myMemory + 1) == relAdress){/* BW points to this block*/
                    *((RELP*)myMemory + 1) = *((RELP*)(myMemory + relAdress) + 2);
                    //LoopFlag = 1;
                }else{
                    *((RELP*)(myMemory + *(RELP*)(myMemory + relAdress + 2))+2) = *(RELP*)(myMemory + relAdress + 4); // Ajust BW link
                }
                if(LoopFlag == 1){
                    /*clear FW and BW*/
                    *((RELP*)(myMemory + relAdress + 4) + 1) = 0;
                    *((RELP*)(myMemory + relAdress + 2) + 2) = 0;
                    return relAdress+2;
                }
                  
                
                ClearData(relAdress, size);
                return relAdress+2;
            }
        }else{
            printf("Moving to next chunk: %d\n", *(probe + 1));
            relAdress = *(probe+1);
            probe = (RELP*)(myMemory+relAdress);
            
            /*Check if probe is back at the FW link, meaning we looped*/
            if(probe == (RELP*)myMemory && LoopFlag != 0){
                /*No block able to be allocated*/
                printf("Could not allocate memory\n");
                return 0;
            }
            LoopFlag = 1;
        }
    }
}

/*
  myFree(RELP region)
  Frees the memory region at the given pointer, region, that was previously allocated using myMalloc. 
  It gives the chunk of memory containing region back to the allocator, and allows for it to be reused in the future.
  PARAMS: region - a pointer to an allocated memory region 
  RETURN: void
  PRE: 78 <=region < MAX_TOTAL_MEMORY-6
  POST: the chunk containing region is added to the available memory
*/
void myFree(RELP region){
    int ValidPtr = 0;
    ValidPtr = CheckPtr(region);
    if(ValidPtr == 1){
        RELP size = *((RELP*)(myMemory + region)-1)-1, i = 0;
        RELP EsizePosition = region+size-4;
        RELP FsizePosition = region - 2;

        *(myMemory+EsizePosition) = size;
        *(myMemory+FsizePosition) = size;
        /*check if the block to the left is free*/
        RELP blockSize = 0;
        if(region-2 >=6){/*bytes to the left are not the header*/
            RELP blockSize = *((RELP*)(myMemory+region)-2);
            if(blockSize%2 == 0){/*The block is free*/
                FsizePosition = (RELP)((region-blockSize)-2);
                *((RELP*)(myMemory+FsizePosition)) = blockSize+size;
                *((RELP*)(myMemory+EsizePosition)) = blockSize+size;
                size = blockSize+size;

                RELP FWlink = *((RELP*)(myMemory+FsizePosition+2));
                RELP BWlink = *((RELP*)(myMemory+FsizePosition+4));

                /*adjust FW link of chunk that has this chunk as it's FW link*/
                if(*((RELP*)myMemory) == FsizePosition){
                    *((RELP*)myMemory) = *((RELP*)(myMemory+FsizePosition+2));
                }else{
                    *((RELP*)(myMemory+BWlink+2)) = FWlink; 
                }

                /*ajust BW link of chunk that has this chunk as it's BW link*/
                if(*((RELP*)(myMemory+2)) == FsizePosition){
                    *((RELP*)(myMemory+2)) = *(myMemory+FsizePosition+4);
                }else{
                    *((RELP*)(myMemory+FWlink+4)) = BWlink;
                }
            
                /*check if the new free chunk is the only chunk in the linked list*/
                if(*((RELP*)(myMemory+FsizePosition+2)) == 0 && *((RELP*)(myMemory+FsizePosition+4)) == 0){
                    /*set Header and footer to this chunk then exit the function*/
                    *((RELP*)myMemory) = FsizePosition;
                    *((RELP*)(myMemory+2)) = FsizePosition;
                    for(i = 2; i<size-2; i++){
                        *(myMemory+FsizePosition+i) = 0;
                    }
                    return;
                }
            }
        }
        /*check if the block to the right is free*/
        if(EsizePosition+2<=MAX_TOTAL_MEMORY){
            blockSize = *((RELP*)(myMemory+EsizePosition)+1);
            if(blockSize%2 == 0){/*block is free*/
                EsizePosition = (RELP)((EsizePosition)+blockSize);
                *((RELP*)(myMemory+FsizePosition)) = blockSize+size;
                *((RELP*)(myMemory+EsizePosition)) = blockSize+size;
                RELP FWlink = *((RELP*)(myMemory+FsizePosition+size+2));
                RELP BWlink = *((RELP*)(myMemory+FsizePosition+size+4));
                /*adjust FW link of chunk that has this chunk as it's FW link*/
                if(*((RELP*)myMemory) == FsizePosition+size){
                    *((RELP*)myMemory) = *((RELP*)(myMemory+FsizePosition+size+2));
                }else{
                    *((RELP*)(myMemory+BWlink+2)) = FWlink; 
                }
                /*ajust BW link of chunk that has this chunk as it's BW link*/
                if(*((RELP*)(myMemory+2)) == FsizePosition+size){
                    *((RELP*)(myMemory+2)) = *(myMemory+EsizePosition+4);
                }else{
                    *((RELP*)(myMemory+FWlink+4)) = BWlink;
                }
                /*check if the new free chunk is the only chunk in the linked list*/
                if(*((RELP*)(myMemory+FsizePosition+size+2)) == 0 && *((RELP*)(myMemory+FsizePosition+size+4)) == 0){
                    *((RELP*)myMemory) = FsizePosition;
                    *((RELP*)(myMemory+2)) = FsizePosition;
                    size = blockSize+size;
                    for(i = 2; i<size-2; i++){
                        *(myMemory+FsizePosition+i) = 0;
                    }
                    return;
                }else{
                    size = blockSize+size;
                }
            }
        }

        /*clear all data in the allocated block*/
        for(i = 2; i<size-2; i++){
            *(myMemory+FsizePosition+i) = 0;
        }
        /* Find where to insert the newly created free region*/
        InsertRegion(FsizePosition, size);
    }else{
        printf("Pointer does not point to a used region\n");
    }
}

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
void outputMemory(void){
    RELP* probe;
    RELP address = 4, blockSize, totalBlockSize, previousadress = 3, startAddress;
    int numSpaces=0; /*used to bind text to the right*/
    printf("+--------+-------+------------+-------+\n");
    printf("| Status | Start | Block Size | End   |\n");
    /* loop checks if address is less than max memory and if the previous adress is higher than current
     * since the address will overflow once it reaches max memory*/ 
    while(address > previousadress && address < MAX_TOTAL_MEMORY){
        probe = (RELP*)(myMemory+address);
        blockSize = *probe;
        if(blockSize%2 == 0){
            totalBlockSize = blockSize;
            startAddress = address;
            printf("| Free   |");
        }else{
            printf("| Used   |");
            startAddress = address;
            blockSize = blockSize - 1;
            totalBlockSize = blockSize;
            /*Scan until the next free block is found*/
            previousadress = address;
            address = address + blockSize;
            while(address >= previousadress && address < MAX_TOTAL_MEMORY){
                probe = (RELP*)(myMemory + address);
                blockSize = *probe;
                if(blockSize%2 == 1){
                    blockSize -= 1;
                    totalBlockSize = totalBlockSize + blockSize;
                    previousadress = address;
                    address = address + blockSize;
                }else{
                    address = previousadress;
                    probe = (RELP*)(myMemory + address);
                    blockSize = *probe -1;
                    break;
                }
            }
        }

        /*display the start position*/
        numSpaces = GetLength(startAddress);
        printSpaces(6, numSpaces);
        printf("%d |",startAddress);

        /*display the size*/
        numSpaces = GetLength(totalBlockSize);
        printSpaces(11, numSpaces);
        printf("%d |",totalBlockSize);

        /*display the end position*/
        numSpaces = GetLength(startAddress + totalBlockSize -1);
        printSpaces(6, numSpaces);
        printf("%d |\n", startAddress + totalBlockSize -1);

        previousadress = address;
        address += blockSize;
    }
}
