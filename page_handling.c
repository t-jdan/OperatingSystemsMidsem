#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define VIRTUAL_SIZE 49152 // Total virtual memory size
#define PAGE_SIZE 4096     // Page size in bytes
#define PAGE_NUM (VIRTUAL_SIZE / PAGE_SIZE) // Total number of pages

typedef struct {
    int pid;
    int memory_address;
    int size;
} Process;


// FIFO takes struct process 
// when a page is coming to run and the virtual memory is full
// handle page fault 
// else continue as usual 
// remember to add Process to fifo queue 

bool page_status[PAGE_NUM];
// int next_available_address = 0;
int num_free = PAGE_NUM;
int fifo_queue[PAGE_NUM];
int front = 0;
int rear = -1;
int itemCount = 0;

int allocate_page(Process process);
void deallocate_page(Process process);
int handle_page_fault(Process process);

void initializePageStatus() {
    for (int i = 0; i < PAGE_NUM; i++) {
        page_status[i] = false;
    }
}

// inserts data into FIFO
void insert(int data) {
    if (itemCount < PAGE_NUM) {
        if (rear == PAGE_NUM - 1) {
            rear = -1;
        }
        fifo_queue[++rear] = data;
        itemCount++;
    }
}

// removed data from FIFO
int removeData() {
    int data = fifo_queue[front++];
    if (front == PAGE_NUM) {
        front = 0;
    }
    itemCount--;
    return data;
}

int handle_page_fault(Process process) {
    printf("Page fault occurred for process with PID %d.\n", process.pid);
    printf("Handling page fault using FIFO page replacement.\n");

    // Check if the FIFO queue is full
    if (itemCount == PAGE_NUM) {
        // Deallocate the oldest process's pages using FIFO page replacement
        int oldest_pid = removeData();
        Process oldest_process = {oldest_pid, 0, PAGE_SIZE}; // Create a dummy process with size of one page
        deallocate_page(oldest_process);
    }

    // Allocate pages for the new process
    allocate_page(process);

    return 0;
}

void deallocate_page(Process process) {
    // Deallocate pages for the process
    int num_of_pages = ceil(process.size / (double)PAGE_SIZE);
    for (int i = 0; i < num_of_pages; i++) {
        int page_index = process.memory_address / PAGE_SIZE + i;
        if (page_status[page_index]) { // Check if the page is allocated
            page_status[page_index] = false;
            num_free++; // Increment num_free only if the page was allocated
        }
    }
}

int allocate_page(Process process) {
    int num_of_pages = ceil(process.size / (double)PAGE_SIZE);

    if (num_of_pages > num_free) {
        // printf("Not enough free pages to allocate for process with PID %d.\n", process.pid);
        return handle_page_fault(process); // dont return
    }

    int expected_page = process.memory_address/PAGE_SIZE;

    // Allocate pages for the new process
    for (int i = 0; i < num_of_pages; i++) {
        page_status[expected_page+i] = true;
        total_virtual_memory_used += PAGE_SIZE; // update total memory used 
        num_free -= 1;
        num_allocated += 1;
    }

    // Enqueue the PID of the new process into the FIFO queue
    insert(process.pid); // whether we are handling oage faults or not has to go
    // 

    return 0;
}

int main() {
    initialize_memory();

    // Test allocation and deallocation
    Process p1 = {3456, 0, 8198};
    allocate_page(p1);
    printf("Number of free pages after allocating p1: %d\n", num_free);

    Process p2 = {3457, 0, 8198};
    allocate_page(p2);
    printf("Number of free pages after allocating p2: %d\n", num_free);

    Process p3 = {3458, 0, 8198};
    allocate_page(p3);
    printf("Number of free pages after allocating p3: %d\n", num_free);

    Process p4 = {3459, 0, 8198};
    allocate_page(p4);
    printf("Number of free pages after allocating p4: %d\n", num_free);

    Process p5 = {3460, 0, 8198};
    allocate_page(p5);
    printf("Number of free pages after allocating p5: %d\n", num_free);

    return 0;
}
