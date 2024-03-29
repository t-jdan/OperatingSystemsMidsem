#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

#define PHYSICAL_SIZE 16672 
#define VIRTUAL_SIZE 25008
#define FIFO_QUEUE_SIZE 100
#define TLB_SIZE 16
#define PAGE_SIZE 2084 
#define PAGE_NUM 12
#define FRAME_NUM 8
#define FRAME_SIZE 2084



// TLB Entry Structure
typedef struct {
    int virtualPageNumber;
    int frameNumber;
    bool valid;
} TLBEntry;

// TLB Structure
typedef struct {
    TLBEntry entries[TLB_SIZE];
} TLB;

// FIFO Queue Structure
typedef struct {
    int queue[FIFO_QUEUE_SIZE];
    int front, rear;
} FIFOQueue;



// int createPage();
// int createPhysicalMemory();

int isValidPageNumber(int pageNumber);
int translateAddress(PageTable* masterPageTable, TLB* tlb, int virtualAddress, int* tlbHits, int* tlbMisses);
void handlePageFault(PageTable* masterPageTable, TLB* tlb, int virtualAddress, FIFOQueue* fifoQueue);



int main(){
    // createPage();
    // createPhysicalMemory();
    PageTable masterPageTable;
    TLB tlb;
    FIFOQueue fifoQueue;
    fifoQueue.front = fifoQueue.rear = -1;

    // Initialize TLB
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb.entries[i].valid = false;
    }

    int tlbHits = 0, tlbMisses = 0;

    // why 1000?
    for (int i = 0; i < 1000; i++) {
        // take a random virtual address
        int virtualAddress = rand() % VIRTUAL_SIZE;

        // Check if the virtual address is valid
        if (isValidPageNumber(virtualAddress / PAGE_SIZE)) {
            int physicalAddress = translateAddress(&masterPageTable, &tlb, virtualAddress, &tlbHits, &tlbMisses);

            if (physicalAddress == -1) {
                handlePageFault(&masterPageTable, &tlb, virtualAddress, &fifoQueue);
            } else {
                 printf("Page hit at virtual address %d\n", virtualAddress);

            }
        } else {
            printf("Invalid virtual address: %d\n", virtualAddress);
        }
    }

    // Display TLB hit and miss rates
    float tlbHitRate = (float)tlbHits / (tlbHits + tlbMisses) * 100;
    float tlbMissRate = (float)tlbMisses / (tlbHits + tlbMisses) * 100;

    printf("TLB Hit Rate: %.2f%%\n", tlbHitRate);
    printf("TLB Miss Rate: %.2f%%\n", tlbMissRate);
    return 0;
}



// translats virtual address to physical address and calculates hits and misses
int translateAddress(PageTable* masterPageTable, TLB* tlb, int virtualAddress, int* tlbHits, int* tlbMisses) {

    int masterIndex = virtualAddress / (PAGE_NUM * PAGE_SIZE);
    int pageIndex = (virtualAddress / PAGE_SIZE) % PAGE_NUM;
    int offset = virtualAddress % PAGE_SIZE;

    // Check TLB first
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->entries[i].valid && tlb->entries[i].virtualPageNumber == pageIndex) {
            // TLB hit
            (*tlbHits)++;
            return tlb->entries[i].frameNumber * FRAME_NUM + offset;
        }
    }

    (*tlbMisses)++;

    if (isValidPageNumber(masterIndex) && masterPageTable->entries[masterIndex].valid) {
        // Get the frame number from the intermediate page table
        int frameNumber = masterPageTable->entries[masterIndex].frameNumber;

        if (isValidPageNumber(pageIndex)) {
            int physicalAddress = frameNumber * FRAME_SIZE + offset;

            for (int i = 0; i < TLB_SIZE; i++) {
                if (!tlb->entries[i].valid) {
                    tlb->entries[i].valid = true;
                    tlb->entries[i].virtualPageNumber = pageIndex;
                    tlb->entries[i].frameNumber = frameNumber;
                    break;
                }
            }

            return physicalAddress;
        }
    }

    return -1; 
}




int isValidPageNumber(int pageNumber) {
    return pageNumber >= 0 && pageNumber < PAGE_NUM;
}