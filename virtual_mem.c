#include virtual_mem.h
int allocatePage();
int deallocatePage(Process process);
void handlePageFault(Process process);
void initializePageStatus();
void fillMemoryAddressesArr();
int randomMemoryAddress();
void initProcesses();

int total_virtual_memory_used = 0;
int num_allocated = 0;
int num_free = PAGE_NUM;
int memory_addresses[PAGE_NUM];
int next_available_address = 0;

bool page_status[PAGE_NUM];

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
    // printf("%d", randomIndex);

    // make sure we return an unoccupied page 
    // while(page_status[randomIndex]){
    //     randomIndex = rand() % PAGE_NUM;
    // }

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

int main(){
    fillMemoryAddressesArr();

    initProcesses();

    for(int i=0; i<PAGE_NUM; i++){
        printf("%d", page_status[i]);
    }
    // printf("\n");
    // Process p1;
    // p1.pid = 3456;
    // p1.memory_address = randomMemoryAddress();
    // printf("%d old \n", p1.memory_address);
    // p1.size = 8194;
    // allocatePage(p1);
    // printf("%d new \n", p1.memory_address);
    // int randomAd = randomMemoryAddress();
    // int random = randomMemoryAddress();
    // printf("%d \n", randomAd);
    // printf("%d \n", randomAd/PAGE_SIZE);
    // printf("%d \n", total_virtual_memory_used);
    // int allo = allocatePage();
    // printf("%d \n", next_available_address);
    

    // deallocatePage(p1);


    // printf("%d \n", total_virtual_memory_used);
    // printf("%d \n", allo);
    // deallocatePage(allo);
    // printf("%d \n", next_available_address);
    // printf("%d \n", total_virtual_memory_used);
    return 0;
}


// inserts data into FIFO
// void insertFIFO(Process process) {
//     // printf("rear %d", rear);
//     // fifo_queue[rear] = process;
//     // rear += 1;
// }

// void removeFIFO(Process process) {
//     for(int i = 0; i < rear-1; i++){
//         fifo_queue[i]=fifo_queue[i+1];
//     }
//     // fifo_queue[rear] = NULL;
//     rear -= 1; // cause im overriding 
// }
