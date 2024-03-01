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
int deallocatePage(Process process);
void handlePageFault(Process process);

// int next_available_address = 0;
int total_virtual_memory_used = 0;
int num_allocated = 0;
int num_free = PAGE_NUM;
int memory_addresses[PAGE_NUM];
int next_available_address = 0;
Process fifo_queue[PAGE_NUM] = {0}; // malloc this
// Process* fifo_queue = (Process*)malloc(PAGE_NUM * sizeof(Process));
int rear = 0;

bool page_status[PAGE_NUM];

void initializeFrameStatus(){
for(int i=0; i < PAGE_NUM; i++){
    page_status[i] = false;
}
}

void fillMemoryAddressesArr(){
    int ini = 0;

    for(int i =0; i < PAGE_NUM; i++){
        memory_addresses[i] = ini;
        ini += PAGE_SIZE;
    }
}

// returns appropriate memory address 
int randomMemoryAddress(){
    // Generate a random index within the array size
    int randomIndex;
    // printf("%d", randomIndex);

    // make sure we return an unoccupied page 
    // while(page_status[randomIndex]){
    //     randomIndex = rand() % PAGE_NUM;
    // }

    printf("%d", randomIndex);
    // Take the random value from the array
    int randomMemory = memory_addresses[randomIndex];

    return randomMemory;
}

// inserts data into FIFO
// void insertFIFO(Process process) {
//     // printf("rear %d", rear);
//     // fifo_queue[rear] = process;
//     // rear += 1;
// }

// void removeFIFO(Process process) {
//     for(int i = 0; i < rear-1; i++){
//         fifo_queue[i]=fifo_queue[i+1];
//     }
//     // fifo_queue[rear] = NULL;
//     rear -= 1; // cause im overriding 
// }

void handlePageFault(Process process){
    printf("Page fault occurred for process with PID %d.\n", process.pid);
    // printf("Handling page fault using FIFO page replacement.\n");

    // Process dealloc = fifo_queue[0];
    // we have to do this until the number of free pages is what we need 
    // Process deallocate = fifo_queue[0];
    // int success = deallocatePage(deallocate); 
    // if(success){
    //     removeFIFO(deallocate);
    // }
    // printf("in fault");
}

// only considers contiguous page allocation, try to implement non-contiguous if we can 
int allocatePage(Process process) { // is run on process pages to allocate page
    // FIGURE THE ROUNDING UP OUT 

    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages needed 
    // YEAA WE HAVE TO FIND A WAY OF CHANGING 

    if(num_of_pages > num_free){ // no free pages available
        printf("Memory allocation for process with PID %d failed.\n", process.pid);
        handlePageFault(process); //find another way to  do this 
    //     // itd handles the page fault and everything is okay now 
        return -1;
    }

    process.memory_address = next_available_address;
    printf("%d", process.memory_address);

    if(process.memory_address >= VIRTUAL_SIZE){
        handlePageFault(process);
        return -1;
    }

    int expected_page = process.memory_address/PAGE_SIZE;

    // assign the memory addresses and track the base memory address and offset 
    for(int i=0; i<num_of_pages; i++){
        page_status[expected_page+i] = true;
        total_virtual_memory_used += PAGE_SIZE; // update total memory used 
        next_available_address += PAGE_SIZE;
        num_free -= 1;
        num_allocated += 1;
    }

    // insertFIFO(process);

    return 0;
}

// considers only contiguous deallocation
int deallocatePage(Process process) {
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages to deallocate
    int expected_page = process.memory_address/PAGE_SIZE; //starting deallocation page

    for(int i=0; i< num_of_pages; i++){
        page_status[expected_page+i] = false;
        total_virtual_memory_used -= PAGE_SIZE; // update total memory used
        next_available_address -= PAGE_SIZE;
        num_free += 1;
        num_allocated -= 1;
    }

    return 0;
}

void initProcesses(){
    // Test allocation and deallocation
    Process p1;
    p1.size = 8194;
    allocatePage(p1);
    printf("Number of free pages after allocating p1: %d\n", num_free);

    Process p2;
    p2.size = 8194;
    allocatePage(p2);
    printf("Number of free pages after allocating p2: %d\n", num_free);

    Process p3;
    p3.size = 8194;
    allocatePage(p3);
    printf("Number of free pages after allocating p3: %d\n", num_free);

    Process p4;
    p4.size = 8194;
    allocatePage(p4);
    printf("Number of free pages after allocating p4: %d\n", num_free);

    Process p5;
    p5.size = 8194;
    allocatePage(p5);
    printf("Number of free pages after allocating p5: %d\n", num_free);

    Process p6;
    p6.size = 8194;
    allocatePage(p6);
    printf("Number of free pages after allocating p6: %d\n", num_free);

    Process p7;
    p7.size = 8194;
    allocatePage(p7);
    printf("Number of free pages after allocating p7: %d\n", num_free);
}

int main(){
    fillMemoryAddressesArr();

    initProcesses();

    for(int i=0; i<PAGE_NUM; i++){
        printf("%d", page_status[i]);
    }
    // printf("\n");
    // Process p1;
    // p1.pid = 3456;
    // p1.memory_address = randomMemoryAddress();
    // printf("%d old \n", p1.memory_address);
    // p1.size = 8194;
    // allocatePage(p1);
    // printf("%d new \n", p1.memory_address);
    // int randomAd = randomMemoryAddress();
    // int random = randomMemoryAddress();
    // printf("%d \n", randomAd);
    // printf("%d \n", randomAd/PAGE_SIZE);
    // printf("%d \n", total_virtual_memory_used);
    // int allo = allocatePage();
    // printf("%d \n", next_available_address);
    

    // deallocatePage(p1);


    // printf("%d \n", total_virtual_memory_used);
    // printf("%d \n", allo);
    // deallocatePage(allo);
    // printf("%d \n", next_available_address);
    // printf("%d \n", total_virtual_memory_used);
    return 0;
}