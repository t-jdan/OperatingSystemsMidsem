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

int workloadOne(int rows, int cols){
    // define matrix 
    int **matrix;
    // int matrix[rows][cols];
    matrix = (int **)malloc(rows * sizeof(int *));

    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for matrix rows.\n");
        return -1;  // Return an error code
    }

        // Dynamically allocate memory for each row of the matrix
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for matrix columns.\n");

            // Free previously allocated memory to avoid memory leaks
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);

            return -1;  // Return an error code
        }
    }
    
    // Initialize the matrix with some random values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int random = rand()  % 11;
            matrix[i][j] = i * random + j ;
        }
    }

    // Print the matrix
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    int rand_sum = 0;

    // calculate and sum row/2 of elements in the matrix
    for(int i=0; i < rows/2 ; i++){
        int row_rand =  rand() % rows;
        int col_rand  = rand() % cols;
        rand_sum += matrix[row_rand][col_rand];
    }

    printf("The sum of %d random numbers in the array is %d \n", rows/2, rand_sum);

    // Free dynamically allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}


int main(){
    // right now  i want to test that for a process i am able to divide it into pages and allocate 
    // also track statistics 

    pid_t pid = fork();

    if (pid == -1) {
        // Handle fork error
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // This is the child process
        int rows = 5;  // Set the desired number of rows
        int cols = 5;  // Set the desired number of columns
        int result = workloadOne(rows, cols);

        // Exit the child process
        exit(result);
    } else {
        // This is the parent process
        int childStatus;
        waitpid(pid, &childStatus, 0);

        if (WIFEXITED(childStatus)) {
            // The child process exited normally
            int exitCode = WEXITSTATUS(childStatus);
            printf("Child process exited with code: %d\n", exitCode);
        } else {
            // The child process did not exit normally
            printf("Child process did not exit normally.\n");
        }
    }

    return 0;
}

// array representation of first level entry
// FirstLevelEntry* first_level_page_table = (FirstLevelEntry*)malloc(PAGE_NUM * sizeof(FirstLevelEntry));
// using static function until i hve to initialize in main. main initialization is above
// FirstLevelEntry first_level_page_table[PAGE_NUM] = {0};