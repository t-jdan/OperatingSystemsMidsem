#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// #define PHYSICAL_SIZE 32768 //change
#define VIRTUAL_SIZE 49152 //used to track the allocation of processes to virtual memory
#define PAGE_SIZE 4096 //4kb
#define PAGE_NUM (VIRTUAL_SIZE/PAGE_SIZE) //change
// #define FRAME_NUM (PHYSICAL_SIZE/PAGE_SIZE) //change
// #define NUM_PROCESSES 3

typedef struct {
    int pid;
    int memory_address;
    int size; // can choose to randomly generate or not
} Process;

int allocatedPage();
void deallocatePage(Process process);

int next_available_address = 0;
int total_virtual_memory_used = 0;
int num_allocated = 0;
int num_free =  PAGE_NUM;



bool page_status[PAGE_NUM];

void initializeFrameStatus(){
    for(int i=0; i < PAGE_NUM; i++){
        page_status[i] = false;
    }
}

int allocatePage(Process process) { // is run on process pages to allocate page
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages needed   

    if(num_of_pages > num_free){ // no free pages available
        printf("Memory allocation for process with PID %d failed.\n", process.pid);
        printf("Deallocate memory and try again.\n");
        exit(1); //find another way to  do this 
    }

    int expected_page = process.memory_address/PAGE_SIZE; //page number flaws with this approach cause youll never get 0. correct it 
    // also causes probelm with next available  memory  address
    if(page_status[expected_page]){//check  if page is already occupied 
        printf("Memory address occupied. Assigning new memory address"); // give message  
        process.memory_address  = next_available_address; //assign unoccupied expected address
        expected_page = process.memory_address/PAGE_SIZE; //reassign expected address
        next_available_address += PAGE_SIZE;  // calculate the next available virtual memory address
    }

    // assign the memory addresses and track the base memory address and offset 
    for(int i=0; i<num_of_pages; i++){
        page_status[num_of_pages+i] = true;

        if (next_available_address < VIRTUAL_SIZE) { //check that it is within the virtual memory
            int allocated_page = next_available_address / PAGE_SIZE; // get the page number that has been allocated
            next_available_address += PAGE_SIZE;  // calculate the next available virtual memory address
            total_virtual_memory_used += PAGE_SIZE; // update total memory used 
            num_free -= 1;
            num_allocated += 1;
        }
    }

    // how does the base and offset thing work? technically i can calculate it 

    // if (next_available_address < VIRTUAL_SIZE) { //check that it is within the virtual memory
    //     int allocated_page = next_available_address / PAGE_SIZE; // get the page number that has been allocated
    //     next_available_address += PAGE_SIZE;  // calculate the next available virtual memory address
    //     total_virtual_memory_used += PAGE_SIZE; // update total memory used 
    //     return allocated_page;
    // }

    return 0;
}

// void deallocatePage(int page_number) {
//     if (page_number >= 0 && page_number < PAGE_NUM) { //  check that it is between virtual memory range
//         next_available_address = page_number * PAGE_SIZE; // open it up for use  
//         total_virtual_memory_used -= PAGE_SIZE; // update total memory used
//     }
// }

void deallocatePage(Process process) {
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages to deallocate
    int expected_page = process.memory_address/PAGE_SIZE; //starting deallocation page

    for(int i=0; i< num_of_pages; i++){
        // if (page_number >= 0 && page_number < PAGE_NUM) { //  check that it is between virtual memory range
        page_status[expected_page+i] = false;
        next_available_address = (expected_page+i) * PAGE_SIZE; // open it up for use  
        total_virtual_memory_used -= PAGE_SIZE; // update total memory used
        num_free += 1;
        num_allocated -= 1;
        // }
    }
    
}

int randomMemoryAddress(){
    // Generate a random integer within the virtual memory range
    return rand() % VIRTUAL_SIZE;
    // make sure they are a good multiple so they divie evenly  
}

int main(){
    Process p1;
    p1.pid = 3456;
    p1.memory_address = 8192;
    p1.size = 8198;
    allocatePage(p1);
    // int random = randomMemoryAddress();
    // printf("%d \n", p1.pid);
    // printf("%d \n", total_virtual_memory_used);
    // int allo = allocatePage();
    printf("%d \n", next_available_address);
    for(int i=0; i<PAGE_NUM; i++){
        printf("%d", page_status[i]);
    }

    deallocatePage(p1);
    for(int i=0; i<PAGE_NUM; i++){
        printf("%d", page_status[i]);
    }
    // printf("%d \n", total_virtual_memory_used);
    // printf("%d \n", allo);
    // deallocatePage(allo);
    // printf("%d \n", next_available_address);
    // printf("%d \n", total_virtual_memory_used);
    return 0;
}