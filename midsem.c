#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

// function declarations
int createPage();
int createPhysicalMemory();



// in bytes
#define PHYSICAL_SIZE 16672 
#define VIRTUAL_SIZE 25008
#define PAGE_SIZE 2084 //itll make translation easier
#define PAGE_NUM 12
#define FRAME_NUM 8

// represents each entry in the page table 
typedef struct{
    int address; //address for physical memory
    int offset; //offset for physical memory
} PageTableItem;

// GLOBAL
PageTableItem* page_table;
char** physical_memory;


int main(){
    createPage();
    createPhysicalMemory();


    // printf("%zu", total_size);
    return 0;
}

int createPage(){
    // Initialize page table arry of type PageTableItem
    page_table = (PageTableItem*)malloc(PAGE_NUM * sizeof(PageTableItem));

    if (page_table == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Memory allocation for page table failed.\n");
        return 1;  // Return an error code
    }

    printf("Success.\n");
    
    return 0;
}

int createPhysicalMemory(){
    // Initialize physical memory array of type char to allow flexibility
    physical_memory = (char**)malloc(PHYSICAL_SIZE);

    // // Initialize each physical memory frame with size of PAGE_SIZE
    for (int i = 0; i < FRAME_NUM; ++i) {
        physical_memory[i] = (char*)malloc(PAGE_SIZE);
    }

    if (physical_memory == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Memory allocation for physical memory failed.\n");
        return 1;  // Return an error code
    }

    printf("Success.\n");

    return 0;
}
