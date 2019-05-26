#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("%s\n", "Parameters to read: filename (read from it) and opeartion r; Parameters to write: filename (write to it), operation w and filename (read from it)");
        return 1;
    }

    FILE *file;
    FILE *lockFile;
    FILE *fileToWrite;

    char buffer[1024];
    size_t bytesRead = 0;

    char* operation = argv[2];
    char* fileName = argv[1];
    int fileNameSize = strlen(fileName);

    // Allocate the memory for buffer with lock file name (file name + extention(.lck)).
    char* nameBufferPointer = malloc(fileNameSize + 5);

    //Create the lock file name with extention.
    sprintf(nameBufferPointer, "%s.lck", fileName);

    lockFile = fopen(nameBufferPointer, "r");

    // Whiel the lockfile created, do nothing. Sleep for decrease the requests for OS.
    while (lockFile != NULL) {
        lockFile = fopen(nameBufferPointer, "r");
        sleep(1);
    }

    lockFile = fopen(nameBufferPointer, "w");
    fprintf(lockFile, "%i %s", getpid(), operation);
    fclose(lockFile);

    file = fopen(fileName, operation);
    // Write data from one file to another by chunk.
    if (strcmp(operation, "w") == 0) {
        fileToWrite = fopen(argv[3], "r");
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), fileToWrite)) > 0) {
            fwrite(buffer , 1 , bytesRead, file);
        }
        fclose(fileToWrite);
    } else {
        // Read data to stdout from file.
        if (strcmp(operation, "r") == 0) {
            fileToWrite = fdopen(1, "w");
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                fwrite(buffer, 1, bytesRead, fileToWrite);
            }
            fclose(fileToWrite);
        } else {
            printf("%s\n", "Operation should be r or w.");
        }
    }
    fclose(file);

    remove(nameBufferPointer);

    return 0;
}