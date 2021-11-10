#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functions.h"

int main()
{
    pid_t child_pid;
    int pipe1[2];
    int returnstatus_pipe;
    char file_name[20];
    char read_file_name[20];
    char sequence_of_numbers[100];
    char read_sequence_of_numbers[100];

    returnstatus_pipe = pipe(pipe1);

    // Error: can not create pipe
    if (returnstatus_pipe == -1) {
        printf("Unable to create pipe\n");
        return 1;
    }

    child_pid = fork();

    // Error: can not create child process
    if (child_pid < 0) {
        perror("fork error");
        return -1;
    }
    // Child Process
    else if (child_pid == 0) {
        int read_cnt;
        read(pipe1[0], read_file_name, sizeof(read_file_name)); // read 1
		read(pipe1[0], read_sequence_of_numbers, sizeof(read_sequence_of_numbers)); // read 2
        read(pipe1[0], &read_cnt, sizeof(int)); // read 3

        printf("[Child Process,  id=%d]: Reading from the pipe. Message is file name: %s\n", getpid(), read_file_name);
        fflush(stdout);
        
        printf("[Child Process,  id=%d]: Reading from the pipe. Message is sequence of numbers: %s\n", getpid(), read_sequence_of_numbers);
        fflush(stdout);

        remove(read_file_name);
        FILE *write_seq = fopen(read_file_name, "w");
        fputs(read_sequence_of_numbers, write_seq);
        fclose(write_seq);

        FILE *read_seq = fopen(read_file_name, "r");
        float res = sum(read_seq, read_cnt);
        fclose(read_seq);

        FILE *write_res = fopen(read_file_name, "w");
        write_res = fopen(read_file_name, "w");
        fprintf(write_res, "%f", res);
        fclose(write_res);
     /*   printf("[Child Process,  id=%d]: result of sum is %f\n", getpid(), res);
        fflush(stdout);

        printf("[Child Process,  id=%d]: writing result to file %s\n", getpid(), read_file_name);
        fflush(stdout);*/

        close(pipe1[0]);
        close(pipe1[1]);
    }
    // Parent Process
    else {
        printf("[Parent Process, id=%d]: Enter name of the file where result will store: ", getpid());
        fflush(stdout);
        fgets(file_name, 20, stdin);
        if (file_name[strlen(file_name) - 1] == '\n')
            file_name[strlen(file_name) - 1] = '\0';
        fflush(stdin);

        printf("[Parent Process, id=%d]: Enter the sequence of real type numbers: ", getpid());
        fflush(stdout);   
        fgets(sequence_of_numbers, 100, stdin);
        if (sequence_of_numbers[strlen(sequence_of_numbers) - 1] == '\n')
            sequence_of_numbers[strlen(sequence_of_numbers) - 1] = '\0';
        fflush(stdin);

        printf("[Parent Process, id=%d]: Writing to the pipe. Message is file name: %s\n\n", getpid(), file_name);
        fflush(stdout);

        int cnt = count(sequence_of_numbers);

        write(pipe1[1], file_name, sizeof(file_name)); // write 1
        write(pipe1[1], sequence_of_numbers, sizeof(sequence_of_numbers)); // write 2 
        write(pipe1[1], &cnt, sizeof(int)); // write 3

        /* printf("[Parent Process, id=%d]: Writing to the pipe - Message is sequence of numbers: \"%f, %f, %f\"\n", getpid(), x, y, z);
        fflush(stdout); */  

        close(pipe1[0]);
        close(pipe1[1]);
    }
    return 0;
}
