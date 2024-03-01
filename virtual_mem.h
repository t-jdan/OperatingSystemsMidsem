#ifndef VIRTUAL_MEM_H  // Include guard to prevent multiple inclusion
#define VIRTUAL_MEM_H

// declare functions 
int allocatePage();
int deallocatePage(Process process);
void handlePageFault(Process process);
void initializePageStatus();
void fillMemoryAddressesArr();
int randomMemoryAddress();
void initProcesses();

#endif