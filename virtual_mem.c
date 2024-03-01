#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define VIRTUAL_SIZE 49152 //used to track the allocation of processes to virtual memory
#define PAGE_SIZE 4096 //4kb
#define PAGE_NUM (VIRTUAL_SIZE/PAGE_SIZE) //change

typedef struct {
    int pid;
    int memory_address;
    int size; // can choose to randomly generate or not
} Process;

int allocatedPage();
void deallocatePage(Process process);

// int next_available_address = 0;
int total_virtual_memory_used = 0;
int num_allocated = 0;
int num_free = PAGE_NUM;
int memory_addresses[PAGE_NUM];

bool page_status[PAGE_NUM];

void initializeFrameStatus(){
    for(int i=0; i < PAGE_NUM; i++){
        page_status[i] = false;
    }
}

void fillMemoryAddressesArr(){
    int memory_addresses[PAGE_NUM];
    int ini = 0;

    for(int i =0; i < PAGE_NUM; i++){
        memory_addresses[i] = ini;
        ini += PAGE_SIZE;
    }
}

// returns appropriate memory address 
int randomMemoryAddress(){
    // Generate a random index within the array size
    int randomIndex = rand() % PAGE_NUM;

    // make sure we return an unoccupied page 
    while(page_status[randomIndex]){
        randomIndex = rand() % PAGE_NUM;
    }

    // Take the random value from the array
    int randomMemory = memory_addresses[randomIndex];

    return randomMemory;
}

// only considers contiguous page allocation, try to implement non-contiguous if we can 
int allocatePage(Process process) { // is run on process pages to allocate page
    double size = process.size;
    // int num_of_p = ceil(size/PAGE_SIZE);
    // printf("%d", num_of_p); FIGURE THE ROUNDING UP OUT 
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages needed 
    

    if(num_of_pages > num_free){ // no free pages available
        printf("Memory allocation for process with PID %d failed.\n", process.pid);
        printf("Deallocate memory and try again.\n");
        exit(1); //find another way to  do this 
    }

    int expected_page = process.memory_address/PAGE_SIZE;

    // assign the memory addresses and track the base memory address and offset 
    for(int i=0; i<num_of_pages; i++){
        page_status[expected_page+i] = true;

        // if (next_available_address < VIRTUAL_SIZE) { //check that it is within the virtual memory
            // int allocated_page = next_available_address / PAGE_SIZE; // get the page number that has been allocated
            // next_available_address += PAGE_SIZE;  // calculate the next available virtual memory address
            total_virtual_memory_used += PAGE_SIZE; // update total memory used 
            num_free -= 1;
            num_allocated += 1;
        // }
    }

    return 0;
}

// considers only contiguous deallocation
void deallocatePage(Process process) {
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages to deallocate
    int expected_page = process.memory_address/PAGE_SIZE; //starting deallocation page

    for(int i=0; i< num_of_pages; i++){
        // if (page_number >= 0 && page_number < PAGE_NUM) { //  check that it is between virtual memory range
        page_status[expected_page+i] = false;
        // next_available_address = (expected_page+i) * PAGE_SIZE; // open it up for use  
        total_virtual_memory_used -= PAGE_SIZE; // update total memory used
        num_free += 1;
        num_allocated -= 1;
    }
    
}


int main(){
    Process p1;
    p1.pid = 3456;
    p1.memory_address = randomMemoryAddress();
    p1.size = 10198;
    allocatePage(p1);
    // printf("%d \n", p1.memory_address);
    int randomAd = randomMemoryAddress();
    // int random = randomMemoryAddress();
    // printf("%d \n", randomAd);
    // printf("%d \n", randomAd/PAGE_SIZE);
    // printf("%d \n", total_virtual_memory_used);
    // int allo = allocatePage();
    // printf("%d \n", next_available_address);
    // for(int i=0; i<PAGE_NUM; i++){
    //     printf("%d", page_status[i]);
    // }

    deallocatePage(p1);
    // for(int i=0; i<PAGE_NUM; i++){
    //     printf("%d", page_status[i]);
    // }

    // printf("%d \n", total_virtual_memory_used);
    // printf("%d \n", allo);
    // deallocatePage(allo);
    // printf("%d \n", next_available_address);
    // printf("%d \n", total_virtual_memory_used);
    return 0;
}