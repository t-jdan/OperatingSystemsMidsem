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

// properly understand hpw to use the offsets

void initializeFrameStatus(){
    for(int i=0; i < FRAME_NUM; i++){
        frame_status[i] = false;
    }
}

// allocate frames when we need them based on translated address 
int allocateFrame(int virtual_address){
    // translate the virtual address to the physical address 
    // translateVirtualToPhysical();
    // get frame number 
    // change status of frame number 
    // track all the other things 
}

int deallocateFrame(int physical_address){
    // calculate frame number 
    // change frame  status
    // change the physical memory used 
    // change number of free and allocated 
}