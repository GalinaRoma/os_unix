#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 1) {
        printf("There are no input arguments!\n");
        return 1;
    }

    FILE *inputFile;
    FILE *outputFile;
    char buffer[1024];
    // Variable to read and save number from file and process it then.
    char strForNumber[20];
    // Transformed string by atoi.
    int currentNumber;
    // Index for digit in strForNumber.
    int digitIndex = 0;
    int bytesRead = 0;
    // Flag for need to write if number in input is ended.
    bool needToWrite = false;
    // List for number to write in output file.
    vector<int> numbersToWrite = vector<int>();

    outputFile = fopen(argv[argc - 1], "w");

    if (outputFile == NULL)
    {
        printf("Some problems with open output file!\n");
        return 1;
    }

    // Process the input files are given as args.
    if (argc > 1) {
        for (int i=1; i<argc - 1; i++) {
            inputFile = fopen(argv[i], "r");
            if (inputFile == NULL)
            {
                printf("Some problems with open input file!\n");
                continue;
            }
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
            {
                for (int i=0; i < bytesRead; i++) {
                    if (buffer[i] >= 48 && buffer[i] <= 57) {
                        if (i > 0 && buffer[i-1] == '-') {
                            strForNumber[digitIndex] = buffer[i-1];
                            digitIndex++;
                        }
                        strForNumber[digitIndex] = buffer[i];
                        digitIndex++;
                        needToWrite = true;
                    } else {
                        if (needToWrite) {
                            strForNumber[digitIndex] = '\0';
                            digitIndex = 0;
                            currentNumber = atoi(strForNumber);
                            numbersToWrite.push_back(currentNumber);
                            needToWrite = false;
                        }
                    }
                }        
            }
            fclose(inputFile);
        }
    }
    if (needToWrite) {
        strForNumber[digitIndex] = '\0';
        digitIndex = 0;
        currentNumber = atoi(strForNumber);
        numbersToWrite.push_back(currentNumber);
        needToWrite = false;
    }

    sort(numbersToWrite.begin(), numbersToWrite.end());

    for (int i=0; i < numbersToWrite.size(); i++) {
        fprintf(outputFile, "%i\n", numbersToWrite[i]);
    }

    fclose(outputFile);

    return 0;
}