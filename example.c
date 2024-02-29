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
#define NUM_PROCESSES 3

// creating first-level page entry
typedef struct {
    int* second_level_page_table; // a pointer to the second level page entry
} FirstLevelEntry;

// creating second-level page entry
typedef struct {
    int frame_number; // physical memory address
    bool valid; // stores valid and invalid bits
} SecondLevelEntry;

// create representation of physical memory frame 
typedef struct {
    char data[PAGE_SIZE];  // array of size PAGE_SIZE to represent each frame 
} PhysicalMemoryFrame;

// create physical memory
PhysicalMemoryFrame physical_memory[FRAME_NUM];

// create array to track free and allocated frames 
bool frame_status[FRAME_NUM];

// function definitions
void initializePageTables(FirstLevelEntry* first_level_page_table);
void initializeFrameStatus();

// initialize entries in the first level page table 
void initializePageTables(FirstLevelEntry* first_level_page_table) { //takes a pointer to the first level entry array
    for (int i = 0; i < PAGE_NUM; ++i) {
        first_level_page_table[i].second_level_page_table = NULL; // initializes each pointer to NULL since no page table has been created 
    }
}

void initializeFrameStatus(){
    for(int i=0; i < FRAME_NUM; i++){
        frame_status[i] = false;
    }
}


int main(){
    // FirstLevelEntry* first_level_page_table = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
    // initializePageTables(first_level_page_table);

    // for(int i=0; i < PAGE_NUM; i++){
    //     printf("%p",(void*)first_level_page_table[i].second_level_page_table);
    // }    


    // initializeFrameStatus();
    // for(int i =0; i < FRAME_NUM;i++){
    //     printf("%d", frame_status[i]);
    // }

    return 0;
}

// array representation of first level entry
// FirstLevelEntry* first_level_page_table = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
// using static function until i hve to initialize in main. main initialization is above
// FirstLevelEntry first_level_page_table[PAGE_NUM] = {0};