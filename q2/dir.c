#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_PATH_LENGTH 4096 // Maximum path length

void printVerboseMessage(const char *message) {
    fprintf(stdout, "Info: %s\n", message);
}

int createDirectory(const char *dirName, int removeExistingFlag, int verboseFlag) {
    // Check if the directory already exists
    struct stat st;
    if (stat(dirName, &st) == 0) {
        if (removeExistingFlag) {
            if (S_ISDIR(st.st_mode)) {
                if (rmdir(dirName) != 0) {
                    perror("Error");
                    return 1;
                }
                if (verboseFlag) {
                    printVerboseMessage("Directory removed successfully.");
                }
            } else {
                fprintf(stderr, "Error: '%s' exists but is not a directory\n", dirName);
                return 1;
            }
        } else {
            fprintf(stderr, "Error: Directory '%s' already exists. Use -r to remove it.\n", dirName);
            return 1;
        }
    }

    // Create the directory
    if (mkdir(dirName, 0777) == 0) {
        if (verboseFlag) {
            printVerboseMessage("Directory created successfully.");
        }

        // Change the current working directory to the newly created directory
        if (chdir(dirName) != 0) {
            perror("Error");
            return 1;
        }
    } else {
        perror("Error");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int removeExistingFlag = 0;
    int verboseFlag = 0;
    char *dirName = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            removeExistingFlag = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            verboseFlag = 1;
        } else {
            if (!dirName) {
                dirName = argv[i];
            } else {
                fprintf(stderr, "Incorrect format, correct: %s [-r] [-v] [dir_name]\n", argv[0]);
                return 1;
            }
        }
    }

    if (!dirName) {
        fprintf(stderr, "Error: Missing directory name argument\n");
        return 1;
    }

    // Fork a child process
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // This is the child process
        int result = createDirectory(dirName, removeExistingFlag, verboseFlag);
        exit(result); // Exit the child process with the result of createDirectory
    } else {
        // This is the parent process
        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            // Child process terminated normally
            return WEXITSTATUS(status);
        } else {
            // Child process didn't terminate normally
            fprintf(stderr, "Error: Child process did not terminate normally\n");
            return 1;
        }
    }
}
