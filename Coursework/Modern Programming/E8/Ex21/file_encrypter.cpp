#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h> 

using namespace std;

int main(int argc, char** argv) {

    if(argc < 5){
        cout << "Input args are missing!";
        exit(1);
    }

    string operation = argv[1];
    string key = argv[2];
    string inputFilePath = argv[3];
    string outputFilePath = argv[4];

    if (operation.compare("en") == 0) {

        ifstream inputFilePointer;
        inputFilePointer.open(inputFilePath);

        char c;
        
        ofstream outputFilePointer;
        outputFilePointer.open(outputFilePath);

        int iKey = stoi(key);
        
        srand (iKey);
        
        while (inputFilePointer.get(c))
        {
            char encrypted_c = (char) int(c)^rand()%256;
            outputFilePointer << encrypted_c;
        }

        inputFilePointer.close();
        outputFilePointer.close();

    }else if (operation.compare("de") == 0) {

        ifstream inputFilePointer;
        inputFilePointer.open(inputFilePath);

        char c;
        
        ofstream outputFilePointer;
        outputFilePointer.open(outputFilePath);

        int iKey = stoi(key);
        
        srand (iKey);
        

        while (inputFilePointer.get(c))
        {
            char decrypted_c = (char) int(c)^rand()%256;
            outputFilePointer << decrypted_c;
        }

        inputFilePointer.close();
        outputFilePointer.close();

    }

        

    return 0;
}