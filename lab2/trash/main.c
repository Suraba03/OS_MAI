#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main()
{
    pid_t child_pid;
    int pipe1[2];
    int returnstatus_pipe;
    char file_name[20];
    char read_file_name[20];
    char sequence_of_numbers[20];
    char read_sequence_of_numbers[20];

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
        read(pipe1[0], read_file_name, sizeof(read_file_name)); // read 1

        float x, y, z;
		read(pipe1[0], &x, sizeof(float)); // read 2
		read(pipe1[0], &y, sizeof(float)); // read 3
        read(pipe1[0], &z, sizeof(float)); // read 4    
        
        printf("[Child Process,  id=%d]: Reading from the pipe - Message is file name: \"%s\"\n", getpid(), read_file_name);
        fflush(stdout);  
        
        printf("[Child Process,  id=%d]: Reading from the pipe – Message is sequence of numbers: \"%f, %f, %f\"\n", getpid(), x, y, z);
        fflush(stdout);

        float res = x + y + z;
        printf("[Child Process,  id=%d]: result of sum is %f\n", getpid(), res);
        fflush(stdout);

        close(pipe1[0]);
        close(pipe1[1]);
    }
    // Parent Process
    else {
        printf("[Parent Process, id=%d]: Enter name of the file where result will store: ", getpid());
        fflush(stdout);
        scanf("%s", file_name);
        fflush(stdin);

        printf("[Parent Process, id=%d]: Writing to the pipe - Message is file name: \"%s\"\n", getpid(), file_name);
        fflush(stdout);
        write(pipe1[1], file_name, sizeof(file_name)); // write 1
        
        float x, y, z;
        printf("[Parent Process, id=%d]: Enter the sequence of real type numbers: ", getpid());
        fflush(stdout);        
        scanf("%f%f%f", &x, &y, &z);
        fflush(stdin);

        printf("[Parent Process, id=%d]: Writing to the pipe - Message is sequence of numbers: \"%f, %f, %f\"\n", getpid(), x, y, z);
        fflush(stdout);


		write(pipe1[1], &x, sizeof(float)); // write 2
		write(pipe1[1], &y, sizeof(float)); // write 3
        write(pipe1[1], &z, sizeof(float)); // write 4         

        close(pipe1[0]);
        close(pipe1[1]);
    }
    return 0;
}

