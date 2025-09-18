#include <iostream>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int a[5] = {1, 2, 3, 4, 5}; // Sample data to be sent through the pipe
    int pipefd[2];              // pipefd[0] for reading, pipefd[1] for writing

    // Create a pipe
    if (pipe(pipefd) == -1)
    {
        std::cerr << "Pipe creation failed. Exiting..." << std::endl;
        return 1;
    }

    // fork
    pid_t pid;
    if ((pid = fork()) > 0)
    {                     // Parent process
        close(pipefd[0]); // Close read end for parent process
        for (int i = 0; i < 5; ++i)
        {
            std::cout << "Producer: " << a[i] << std::endl;
            write(pipefd[1], &a[i], sizeof(a[i])); // Write to pipe
            sleep(1);                              // used for simple synchronization
        }
        close(pipefd[1]); // Close write end
        wait(NULL);       // Wait for child process to finish
    }
    else if (pid == 0)
    {                     // Child process
        close(pipefd[1]); // Close write end for child process
        for (int i = 0; i < 5; ++i)
        {
            int value;
            read(pipefd[0], &value, sizeof(value)); // Read from pipe
            std::cout << "Consumer: " << value << std::endl;
        }
        close(pipefd[0]); // Close read end
        exit(0);          // Exit child process
    }
    else
    {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    }

    return 0;
}