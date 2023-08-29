#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-d STRING] [-R] file_name\n", argv[0]);
        return 1;
    }

    char *file_name = NULL;
    char *string_arg = NULL;
    int useRFC5322Format = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 < argc) {
                string_arg = argv[i + 1];
                i++; // Skip the next argument
            } else {
                fprintf(stderr, "Error: -d option requires a STRING argument.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-R") == 0) {
            useRFC5322Format = 1;
        } else {
            if (file_name == NULL) {
                file_name = argv[i];
            } else {
                fprintf(stderr, "Error: Only one file_name is allowed.\n");
                return 1;
            }
        }
    }

    if (file_name == NULL) {
        fprintf(stderr, "Error: Please provide a file_name.\n");
        return 1;
    }

    struct stat file_info;
    if (stat(file_name, &file_info) != 0) {
        perror("Error");
        return 1;
    }

    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Child process
        if (string_arg != NULL) {
            FILE *file = fopen(file_name, "r");
            if (file == NULL) {
                perror("Error");
                exit(1);
            }

            char line[512];
            int found = 0;
            time_t last_modification_time = file_info.st_mtime;

            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, string_arg)) {
                    found = 1;
                    last_modification_time = file_info.st_mtime;
                    break;
                }
            }

            fclose(file);

            if (found) {
                struct tm *time_info;
                time_info = localtime(&last_modification_time);

                char time_buffer[100];
                strftime(time_buffer, sizeof(time_buffer), "%a %b %d %H:%M:%S %Z %Y", time_info);
                printf("%s\n", time_buffer);
            } else {
                printf("String not found in the file.\n");
            }
        } else {
            if (useRFC5322Format) {
                char time_buffer[100];
                strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S %z", localtime(&file_info.st_mtime));
                printf("%s\n", time_buffer);
            } else {
                // Display the last modification time in a custom format when no options are provided.
                char time_buffer[100];
                strftime(time_buffer, sizeof(time_buffer), "%a %b %d %H:%M:%S %Z %Y", localtime(&file_info.st_mtime));
                printf("%s\n", time_buffer);
            }
        }

        exit(0);
    } else {
        // Parent process
        wait(&status);
    }

    return 0;
}
