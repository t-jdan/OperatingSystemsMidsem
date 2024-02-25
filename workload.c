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
    // workloadOne(8, 2);
    workloadThree();
    // workloadFive(21);
    return 0;
}


//initializes a matrix and gets the sum of row/2 random values in the matrix
int workloadOne(int rows, int cols){
    // define matrix 
    int matrix[rows][cols];
    
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

    return 0;
}



//reads input from a file and conerts to uppercase before writing to output file
int workloadTwo(){
    int max_line_length = 100;
    FILE *inputFile, *outputFile;
    char inputFileName[] = "input.txt";   
    char outputFileName[] = "output.txt"; 
    char buffer[max_line_length];

    // Open the input file for reading
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    // Read from the input file, convert to uppercase, and write to the output file
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = toupper(buffer[i]);
        }
        fprintf(outputFile, "%s", buffer);
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File conversion completed.\n");

    return 0;
}


// some sort of process synchronization where we perform mathematical operationns on numbers in the file but only one process at a time
// workload 3
// int workloadThree(){ //not valid 
//     int pipefds1[2], pipefds2[2];
//     int returnstatus1, returnstatus2;
//     int pid;

//    returnstatus1 = pipe(pipefds1);
   
//    if (returnstatus1 == -1) {
//       printf("Unable to create pipe 1 \n");
//       return 1;
//    }
//    returnstatus2 = pipe(pipefds2);
   
//    if (returnstatus2 == -1) {
//       printf("Unable to create pipe 2 \n");
//       return 1;
//    }
//    pid = fork();
   
//    if (pid != 0){ // Parent process {
//        int pipe1writemessage[100];

//        for(int i = 0; i < 100; i++){
//             pipe1writemessage[i] = i+1;
//        }

//        int pipe2readmessage;

//       close(pipefds1[0]); // Close the unwanted pipe1 read side
//       close(pipefds2[1]); // Close the unwanted pipe2 write side
//       printf("In Parent: Writing to pipe 1 – Message is %d\n", pipe1writemessage[0]);
//       write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
//       read(pipefds2[0], pipe2readmessage, sizeof(pipe2readmessage));
//       printf("In Parent: Reading from pipe 2 – Message is %d\n", pipe2readmessage);
//    } 
//     else { //child process
//             int pipe2writemessage;
//             int pipe1readmessage[100];

//         close(pipefds1[1]); // Close the unwanted pipe1 write side
//         close(pipefds2[0]); // Close the unwanted pipe2 read side
//         read(pipefds1[0], pipe1readmessage, sizeof(readmessage));

//         int total;
//         for(int i= 0; i < 100; i++){
//             total+= pipe1readmessage[i];
//         }
//     //   pipe2writemessage = total;
//     //   printf("In Child: Reading from pipe 1 – Message is %s\n", pipe1readmessage[0]);
//     //   printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
//       write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
//     }
//     return 0;
// }

// workload 4 
// a workload that does multithreading

//sums the elements in an array
int workloadFive(int num){
    int arr[num];
    int sum = 0;

    // initialize the array with numbers 
    for(int i=0; i < num; i++){
        int random = rand() % num;
        arr[i] = random;
    }

    // sum all the elements in the array 
    for(int i=0; i<num; i++){
        sum += arr[i];
    }

    printf("The sum of the array is %d\n", sum);
}

// workload 6
// a process  that forks  recursively or sth