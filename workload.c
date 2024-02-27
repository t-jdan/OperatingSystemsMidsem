#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

int workloadOne(int rows, int cols);
int workloadTwo();
int workloadThree();
int workloadFive();

int  main(){
    workloadOne(8, 2);
    // workloadThree();
    // workloadFive(21);
    return 0;
}


//initializes a matrix and gets the sum of row/2 random values in the matrix
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



//reads input from a file and conerts to uppercase before writing to output file
// int workloadTwo(){
//     int max_line_length = 100;
//     FILE *inputFile, *outputFile;
//     char inputFileName[] = "input.txt";   
//     char outputFileName[] = "output.txt"; 
//     char buffer[max_line_length];

//     // Open the input file for reading
//     inputFile = fopen(inputFileName, "r");
//     if (inputFile == NULL) {
//         perror("Error opening input file");
//         return 1;
//     }

//     // Open the output file for writing
//     outputFile = fopen(outputFileName, "w");
//     if (outputFile == NULL) {
//         perror("Error opening output file");
//         fclose(inputFile);
//         return 1;
//     }

//     // Read from the input file, convert to uppercase, and write to the output file
//     while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
//         for (int i = 0; buffer[i] != '\0'; i++) {
//             buffer[i] = toupper(buffer[i]);
//         }
//         fprintf(outputFile, "%s", buffer);
//     }

//     // Close the files
//     fclose(inputFile);
//     fclose(outputFile);

//     printf("File conversion completed.\n");

//     return 0;
// }

// create a two-way pipe
// some sort of process synchronization where we perform mathematical operationns on numbers in the file but only one process at a time
// int workloadThree(){ 
   
// }

// workload 6
// a process  that forks  recursively or sth