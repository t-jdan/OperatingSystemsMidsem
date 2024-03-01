#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

#define PHYSICAL_SIZE 32768 //change
#define VIRTUAL_SIZE 49152 //used to track the allocation of processes to virtual memory
#define PAGE_SIZE 4096 //4kb
#define PAGE_NUM (VIRTUAL_SIZE/PAGE_SIZE) //change
#define FRAME_NUM (PHYSICAL_SIZE/PAGE_SIZE) //change

int total_physical_memory_used = 0;
int num_allocated = 0;
int num_free = PAGE_NUM;

bool frame_status[FRAME_NUM];

// create representation of physical memory frame 
typedef struct {
    char data[PAGE_SIZE];  // array of size PAGE_SIZE to represent each frame 
} PhysicalMemoryFrame;

// create physical memory
PhysicalMemoryFrame physical_memory[FRAME_NUM];

// creating second-level page entry
typedef struct {
    int frameNumber; // physical memory address
    bool valid; // stores valid and invalid bits
} SecondLevelEntry;

typedef struct {
    SecondLevelEntry second_level_page_table[PAGE_NUM]; // a pointer to the second level page entry
} FirstLevelEntry;

FirstLevelEntry pageTable[PAGE_NUM];
// properly understand hpw to use the offsets



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

void initializeFrameStatus(){
    for(int i=0; i < FRAME_NUM; i++){
        frame_status[i] = false;
    }
}


int translateVirtualToPhysical(int virtualAddress, FirstLevelEntry* pageTable) {
    // Extract indices from the virtual address
    int firstLevelIndex = (virtualAddress >> 22) & 0x3FF;
    int secondLevelIndex = (virtualAddress >> 12) & 0x3FF;
    int offset = virtualAddress & 0xFFF;
    if (pageTable[firstLevelIndex].second_level_page_table == NULL) {
        printf("Page Fault: Second-level page table not initialized.\n");
        return -1;
    }
    int frameNumber = pageTable[firstLevelIndex].second_level_page_table[secondLevelIndex].frameNumber;
    if (frameNumber == -1) {
        printf("Page Fault: Page not in physical memory.\n");
        return -1;
    }
    int physicalAddress = (frameNumber << 12) | offset;

    return physicalAddress;
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
    num_allocated++;
    num_free--;

    printf("Frame Allocated: Frame %d allocated. Total Physical Memory Used: %d\n", frame_number, total_physical_memory_used);

    return frame_number;
}

int deallocateFrame(int frame_number){
    if (!frame_status[frame_number]) {
        printf("Frame Deallocation Error: Frame already deallocated.\n");
        return 0;
    }
    // Deallocate the frame
    frame_status[frame_number] = false;
    total_physical_memory_used -= PAGE_SIZE;
    num_allocated--;
    num_free++;

    printf("Frame Deallocated: Frame %d deallocated. Total Physical Memory Used: %d\n", frame_number, total_physical_memory_used);

    return 0;

}


int main(){
    initializeFrameStatus();
    FirstLevelEntry* pageTable = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
    initializePageTables(pageTable);

    int virtualAddress = 0x12345678;
    
    int allocatedFrame = allocateFrame(virtualAddress, pageTable);

    if (allocatedFrame != -1) {
        printf("Frame Allocation Successful: Frame %d allocated.\n", allocatedFrame);

        printf("Frame Status after Allocation:\n");
        for (int i = 0; i < FRAME_NUM; ++i) {
            printf("Frame %d: %s\n", i, frame_status[i] ? "Allocated" : "Free");
        }

        deallocateFrame(allocatedFrame);

        printf("Frame Status after Deallocation:\n");
        for (int i = 0; i < FRAME_NUM; ++i) {
            printf("Frame %d: %s\n", i, frame_status[i] ? "Allocated" : "Free");
        }
    }
    
     free(pageTable);

    return 0;
}