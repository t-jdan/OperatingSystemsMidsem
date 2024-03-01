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

// create physical memory
PhysicalMemoryFrame physical_memory[FRAME_NUM];

// create array to track free and allocated frames 
bool frame_status[FRAME_NUM];

// function definitions
void handlePageFault(FirstLevelEntry* pageTable, int firstLevelIndex);
int translateVirtualToPhysical(int virtual_address, FirstLevelEntry* first_level_page_table);
void initializePageTables(FirstLevelEntry* first_level_page_table);
void initializeFrameStatus();



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
        handlePageFault(pageTable, firstLevelIndex);
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










int main(){
    //printf("%d", (int)sizeof(workloadOne));

    FirstLevelEntry* pageTable = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
    int virtualAddress = 0xABCDE000; 
    int physicalAddress = translateVirtualToPhysical(virtualAddress, pageTable);

    if (physicalAddress != -1) {
        printf("Virtual Address: 0x%08X\n", virtualAddress);
        printf("Physical Address: 0x%08X\n", physicalAddress);
    }

    initializePageTables(pageTable);
     free(pageTable);


    return 0;
}

void handlePageFault(FirstLevelEntry* pageTable, int firstLevelIndex) {
    printf("Page Fault: Handling page fault...\n");

    // Simulate loading the page into physical memory
    for (int i = 0; i < FRAME_NUM; ++i) {
        if (!frame_status[i]) {
            frame_status[i] = true;
            pageTable[firstLevelIndex].second_level_page_table[0].frameNumber = i;
            break;
        }
    }
}