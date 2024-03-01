#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>


#define PHYSICAL_SIZE 32768 
#define VIRTUAL_SIZE 49152 
#define PAGE_SIZE 4096 //4kb
#define PAGE_NUM (VIRTUAL_SIZE/PAGE_SIZE) 
#define FRAME_NUM (PHYSICAL_SIZE/PAGE_SIZE) 

// structs
// Process
typedef struct {
    int pid;
    int memory_address;
    int size; // can choose to randomly generate or not
} Process;

// creating second-level page entry
typedef struct {
    int frameNumber; // physical memory address
    bool valid; // stores valid and invalid bits
} SecondLevelEntry;

// creating first-level page entry
typedef struct {
    SecondLevelEntry second_level_page_table[PAGE_NUM];; // a pointer to the second level page entry
} FirstLevelEntry;

typedef struct {
    char data[PAGE_SIZE];  // array of size PAGE_SIZE to represent each frame 
} PhysicalMemoryFrame;

// initialize functions 
int allocatePage();
int deallocatePage(Process process);
void handlePageFault(Process process);
void initializePageStatus();
void fillMemoryAddressesArr();
int randomMemoryAddress();
void initProcesses();
int translateVirtualToPhysical(int virtual_address, FirstLevelEntry* first_level_page_table);
void initializePageTables(FirstLevelEntry* first_level_page_table);
void initializeFrameStatus();

// initialize constants 
int total_virtual_memory_used = 0;
int num_allocated = 0;
int num_free = PAGE_NUM;
int memory_addresses[PAGE_NUM];
int next_available_address = 0;
bool page_status[PAGE_NUM];
int num_allocated_frame = 0;
int num_free_frame = FRAME_NUM;
int total_physical_memory_used = 0;
bool frame_status[FRAME_NUM];


// initializes page status 
void initializePageStatus(){
    for(int i=0; i < PAGE_NUM; i++){
        page_status[i] = false;
    }
}

// initializes array with memory addresses
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

    printf("%d", randomIndex);
    // Take the random value from the array
    int randomMemory = memory_addresses[randomIndex];

    return randomMemory;
}

// indicates page fault to user
void handlePageFault(Process process){
    printf("Page fault occurred for process with PID %d.\n", process.pid);
}

// allocates pages and updates statistics
int allocatePage(Process process) { // is run on process pages to allocate page
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages needed 

    if(num_of_pages > num_free){ // no adequate pages available
        printf("Memory allocation for process with PID %d failed.\n", process.pid);
        handlePageFault(process); 
        return -1;
    }

    process.memory_address = next_available_address;

    // check if memory address is out of range
    if(process.memory_address >= VIRTUAL_SIZE){
        handlePageFault(process);
        return -1;
    }

    int expected_page = process.memory_address/PAGE_SIZE; //expected page location

    // assign the memory addresses and track the base memory address and offset 
    for(int i=0; i<num_of_pages; i++){
        page_status[expected_page+i] = true;
        // update statistics 
        total_virtual_memory_used += PAGE_SIZE; 
        next_available_address += PAGE_SIZE;
        num_free -= 1;
        num_allocated += 1;
    }

    printf("Total virtual memory used %d \n", total_virtual_memory_used);
    printf("Next available address %d \n", next_available_address);
    printf("Number of free pages: %d \n", num_free);

    return 0;
}

// deallocates pages and updates statistics
int deallocatePage(Process process) {
    int num_of_pages = ceil(process.size/PAGE_SIZE); // number of pages to deallocate
    int expected_page = process.memory_address/PAGE_SIZE; //starting deallocation page

    for(int i=0; i< num_of_pages; i++){
        page_status[expected_page+i] = false;
        // update statistics 
        total_virtual_memory_used -= PAGE_SIZE;
        next_available_address -= PAGE_SIZE; 
        num_free += 1;
        num_allocated -= 1;
    }

    return 0;
}

// initializes processes
void initProcesses(){
    // Initialize processes with different workloads 
    Process p1;
    p1.size = 8194;
    p1.pid = 8484;
    allocatePage(p1);
    printf("Number of free pages after allocating p1: %d\n", num_free);

    Process p2;
    p2.size = 32776;
    p2.pid = 2832;
    allocatePage(p2);
    printf("Number of free pages after allocating p2: %d\n", num_free);

    Process p3;
    p3.size = 4096;
    p3.pid = 1392;
    allocatePage(p3);
    printf("Number of free pages after allocating p3: %d\n", num_free);

    Process p4;
    p4.size = 16388;
    p4.pid = 2489;
    allocatePage(p4);
    printf("Number of free pages after allocating p4: %d\n", num_free);

    Process p5;
    p5.size = 49152;
    p5.pid = 9802;
    allocatePage(p5);
    printf("Number of free pages after allocating p5: %d\n", num_free);
}

// initialize page table
void initializePageTables(FirstLevelEntry* first_level_page_table) {
    for (int i = 0; i < PAGE_NUM; ++i) {
        // No need to allocate memory, it's already part of the structure

        // Initializes each entry in the second_level_page_table
        for (int j = 0; j < PAGE_NUM; ++j) {
            first_level_page_table[i].second_level_page_table[j].frameNumber = -1;  // Set a default invalid value
            first_level_page_table[i].second_level_page_table[j].valid = false;    // Set validity to false
        }
    }
}

// translates vitual address to physical and places in page table 
int translateVirtualToPhysical(int virtualAddress, FirstLevelEntry* pageTable) {
    int firstLevelIndex = (virtualAddress >> 22) & 0x3FF;
    int secondLevelIndex = (virtualAddress >> 12) & 0x3FF;
    int offset = virtualAddress & 0xFFF;
    if (pageTable[firstLevelIndex].second_level_page_table == NULL) {
        printf("Page Fault: Second-level page table not initialized.\n");
        return -1;
    }
    int frameNumber = pageTable[firstLevelIndex].second_level_page_table[secondLevelIndex].frameNumber;

    // Check if the frame number is valid
    if (frameNumber == -1) {
        printf("Page Fault: Page not in physical memory.\n");
        return -1;
    }
    int physicalAddress = (frameNumber << 12) | offset;

    return physicalAddress;
}

// initializes frame status
void initializeFrameStatus(){
    for(int i=0; i < FRAME_NUM; i++){
        frame_status[i] = false;
    }
}

// allocate frames when we need them based on translated address 
int allocateFrame(int virtual_address, FirstLevelEntry* pageTable){
    int physical_address = translateVirtualToPhysical(virtual_address, pageTable);

    int frame_number = (physical_address >> 12) & 0x3FF;
    // Check if the frame is already allocated
    if (frame_status[frame_number]) {
        printf("Frame Allocation Error: Frame already allocated.\n");
        return -1;
    }

    // Allocate the frame
    frame_status[frame_number] = true;
    total_physical_memory_used += PAGE_SIZE;
    num_allocated_frame++;
    num_free_frame--;

    printf("Frame Allocated: Frame %d allocated. Total Physical Memory Used: %d\n", frame_number, total_physical_memory_used);

    return frame_number;
}

// deallocated frames 
int deallocateFrame(int frame_number){
    if (!frame_status[frame_number]) {
        printf("Frame Deallocation Error: Frame already deallocated.\n");
        return 0;
    }
    // Deallocate the frame
    frame_status[frame_number] = false;
    total_physical_memory_used -= PAGE_SIZE;
    num_allocated_frame--;
    num_free_frame++;

    printf("Frame Deallocated: Frame %d deallocated. Total Physical Memory Used: %d\n", frame_number, total_physical_memory_used);

    return 0;

}

int main(){
    // Initialize 3 processes with different workloads 
    Process p1;
    p1.size = 8194;
    p1.pid = 8484;

    Process p2;
    p2.size = 32776;
    p2.pid = 2832;

    Process p3;
    p3.size = 16388;
    p3.pid = 2489;

    // allocating virtual memory to processes 
    allocatePage(p1);
    printf("Process 1 with PID %d page allocated\n", p1.pid);

    allocatePage(p2);
    printf("Process 2 with PID %d page allocated\n", p2.pid);
    
    allocatePage(p3);
    printf("Process 3 with PID %d page allocated\n", p3.pid);

}