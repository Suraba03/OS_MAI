#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Main program started\n");
    char* argv[] = { NULL };
    char* envp[] = { NULL };
    if (execve("./sub", argv, envp) == -1)
        perror("Could not execve");
    return 1;
}