#include <stdio.h>
#include <stdlib.h>
#include "memManager.h"

/*
  main(void)
  runs a given list of tests to test the correctness of the memManager allocater.
  POST: three test files are created to the directory of this executable
*/
int main(void){
    initializeMemory();
    outputMemory();
    RELP r1, r2, r3, r4, r5, r6;
    r1 = myMalloc(20);
    r2 = myMalloc(20);
    r3 = myMalloc(20);
    r4 = myMalloc(20);
    r5 = myMalloc(20);
    r6 = myMalloc(20);
    FILE *TestOutput;
    TestOutput = fopen("testFile1.txt","w");
    fwrite(myMemory, 1, MAX_TOTAL_MEMORY, TestOutput);
    fclose(TestOutput);
    outputMemory();
    
    myFree(r3);
    myFree(r3);
    myFree(r2);
    myFree(r5);
    r2 = myMalloc(21);
    r3 = myMalloc(20);
    myFree(r1);
    TestOutput = fopen("testFile2.txt","w");
    fwrite(myMemory, 1, MAX_TOTAL_MEMORY, TestOutput);
    fclose(TestOutput);
    outputMemory();

    myFree(r4);
    myFree(69);
    myFree(r6);
    myFree(r2);
    myFree(r3);
    TestOutput = fopen("testFile3.txt","w");
    fwrite(myMemory, 1, MAX_TOTAL_MEMORY, TestOutput);
    fclose(TestOutput);

    outputMemory();
    return 0;
}