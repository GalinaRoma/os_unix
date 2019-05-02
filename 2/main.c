#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    FILE *inputFile;
    FILE *outputFile;
    // Buffer where we will read from input file.
    char buffer[1024];
    // Buffer for not 0 symbols to write in output (between fseek calls).
    char bufferToWrite[1024];
    int counterZeros = 0;
    int counterToWrite = 0;
    size_t bytesRead = 0;
    // Flag to do the fseek (If we saved zero bytes before).
    int fSeek = 0;

    // Check the arg with output file name.
    if (argc < 2) {
        printf("Name of output file is required!\n");
        return 1;
    }

    inputFile = fdopen(0, "rb");
    outputFile = fopen(argv[1], "wb");

    // Check the files are opened without errors.
    if (inputFile == NULL || outputFile == NULL)
    {
        printf("Some problems with input or output file!\n");
        return 1;
    }
    else
    {
        // Read input file by chunks in 1024 bytes.
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
        {
            for (int i=0; i < sizeof(buffer); i++) {
                // Count the zero bytes repeated.
                if (buffer[i] == 0x00) {
                    counterZeros++;
                    fSeek = 1;
                } else {
                    // Do fSeek if needed.
                    if (fSeek) {
                        fwrite(bufferToWrite , 1 , counterToWrite, outputFile );
                        fseek(outputFile, counterZeros, SEEK_CUR);
                        counterZeros = 0;
                        fSeek = 0;
                        counterToWrite = 0;
                    }
                    bufferToWrite[counterToWrite] = buffer[i];
                    counterToWrite++;
                }
            }
            // Write the rest of input file.
            fwrite(bufferToWrite , 1 , counterToWrite, outputFile );
    
        }
    }
    
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}