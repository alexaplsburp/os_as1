#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_BUFFER_SIZE 1024

// Function to count words in a file
int countWordsInFile(const char *filename, int ignoreNewline) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to locate the file '%s'\n", filename);
        return -1;
    }

    int wordCount = 0;
    int insideWord = 0;
    char currentChar;

    while ((currentChar = fgetc(file)) != EOF) {
        if (ignoreNewline && currentChar == '\n') {
            continue;
        }

        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n') {
            if (insideWord) {
                insideWord = 0;
            }
        } else {
            if (!insideWord) {
                wordCount++;
                insideWord = 1;
            }
        }
    }

    fclose(file);
    return wordCount;
}

int main(int argc, char *argv[]) {
    int ignoreNewlineFlag = 0;
    int calculateDifferenceFlag = 0;
    char *file1Path = NULL;
    char *file2Path = NULL;
    
    int wordCount1; // Store the word count of the first file

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-n] [-d] file1 [file2]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            ignoreNewlineFlag = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            calculateDifferenceFlag = 1;
        } else {
            if (!file1Path) {
                file1Path = argv[i];
            } else if (!file2Path) {
                file2Path = argv[i];
            } else {
                fprintf(stderr, "Usage: %s [-n] [-d] file1 [file2]\n", argv[0]);
                return 1;
            }
        }
    }

    if (!file1Path) {
        fprintf(stderr, "Error: Missing file1 argument\n");
        return 1;
    }

    // Create a pipe to communicate the word count from child to parent
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Use fork to create a child process
    pid_t childProcessID = fork();

    if (childProcessID < 0) {
        fprintf(stderr, "Forking failed\n");
        return 1;
    }

    if (childProcessID == 0) {
        // Child process
        close(pipefd[0]); // Close the read end of the pipe in the child

        wordCount1 = countWordsInFile(file1Path, ignoreNewlineFlag);
        if (wordCount1 >= 0) {
            // Send the word count to the parent through the pipe
            write(pipefd[1], &wordCount1, sizeof(wordCount1));
        }

        close(pipefd[1]); // Close the write end of the pipe in the child
        exit(0);
    } else {
        // Parent process
        close(pipefd[1]); // Close the write end of the pipe in the parent

        int status;
        waitpid(childProcessID, &status, 0);

        // Read the word count from the child through the pipe
        read(pipefd[0], &wordCount1, sizeof(wordCount1));
        close(pipefd[0]); // Close the read end of the pipe in the parent

        if (wordCount1 >= 0) {
            printf("The file '%s' contains %d words.\n", file1Path, wordCount1);
        }

        if (file2Path && calculateDifferenceFlag) {
            int wordCount2 = countWordsInFile(file2Path, ignoreNewlineFlag);
            if (wordCount2 >= 0) {
                int wordCountDifference = abs(wordCount1 - wordCount2);
                printf("Word count difference between '%s' and '%s': %d\n", file1Path, file2Path, wordCountDifference);
            }
        }
    }

    return 0;
}
