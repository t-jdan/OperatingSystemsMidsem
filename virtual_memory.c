#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #define PHYSICAL_SIZE 32768 //change
#define VIRTUAL_SIZE 49152 //used to track the allocation of processes to virtual memory
#define PAGE_SIZE 4096 //4kb
#define PAGE_NUM (VIRTUAL_SIZE/PAGE_SIZE) //change
// #define FRAME_NUM (PHYSICAL_SIZE/PAGE_SIZE) //change
// #define NUM_PROCESSES 3

int next_available_address = 0;
int total_virtual_memory_used = 0;

int allocatePage() { // is run on process pages to allocate page
    if (next_available_address < VIRTUAL_SIZE) { //check that it is within the virtual memory
        int allocated_page = next_available_address / PAGE_SIZE; // get the page number that has been allocated
        next_available_address += PAGE_SIZE;  // calculate the next available virtual memory address
        total_virtual_memory_used += PAGE_SIZE; // update total memory used 
        return allocated_page;
    }

    return -1;  // No free pages available
}

void deallocatePage(int page_number) {
    if (page_number >= 0 && page_number < PAGE_NUM) { //  check that it is between virtual memory range
        next_available_address = page_number * PAGE_SIZE; // open it up for use  
        total_virtual_memory_used -= PAGE_SIZE; // update total memory used
    }
}

int randomMemoryAddress(){
    // Generate a random integer within the virtual memory range
    return rand() % VIRTUAL_MEMORY_SIZE;
}