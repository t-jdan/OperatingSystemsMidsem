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



// idk i think we need to change this to only a calculation 
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








int main(){
    //printf("%d", (int)sizeof(workloadOne));


    FirstLevelEntry* pageTable = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
    FirstLevelEntry* pageTable = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
    int virtualAddress = 0x12345678;
    int physicalAddress = translateVirtualToPhysical(virtualAddress, pageTable);

    if (physicalAddress != -1) {
        printf("Virtual Address: 0x%08X\n", virtualAddress);
        printf("Physical Address: 0x%08X\n", physicalAddress);
    }

    initializePageTables(pageTable);
     free(pageTable);


    // right now  i want to test that for a process i am able to divide it into pages and allocate 
    // also track statistics 


    // FirstLevelEntry first_level_page_table[PAGE_NUM];
    // initializePageTables(first_level_page_table);
    // initializeFrameStatus();
    // // pid_t pid = fork();

    // if (pid == -1) {
    //     // Handle fork error
    //     perror("fork");
    //     exit(EXIT_FAILURE);
    // }

    // if (pid == 0) {
    //     // This is the child process
    //     int rows = 5;  // Set the desired number of rows
    //     int cols = 5;  // Set the desired number of columns
    //     int result = workloadOne(rows, cols, pid, first_level_page_table);

    //     // Exit the child process
    //     exit(result);
    // } else {
    //     // This is the parent process
    //     int childStatus;
    //     waitpid(pid, &childStatus, 0);

    //     if (WIFEXITED(childStatus)) {
    //         // The child process exited normally
    //         int exitCode = WEXITSTATUS(childStatus);
    //         printf("Child process exited with code: %d\n", exitCode);
    //     } else {
    //         // The child process did not exit normally
    //         printf("Child process did not exit normally.\n");
    //     }
    // }

    return 0;
}

// array representation of first level entry
// FirstLevelEntry* first_level_page_table = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
// using static function until i hve to initialize in main. main initialization is above
// FirstLevelEntry first_level_page_table[PAGE_NUM] = {0};
