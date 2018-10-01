#include <iostream>
#include <fstream>
using namespace std;

//Initializes the streams and handles invalid inputs.
void initStreams(ifstream & input, ofstream & output){
    string inputPath, outputPath;
    do{
        cout << "Please specify the input file path: ";
        cin >> inputPath;
        input.open(inputPath);
        if(!input) cout << "\nInvalid input file path!" << endl;
    }while(!input);
    do{
        cout << "Please specify the output file path: ";
        cin >> outputPath;
        output.open(outputPath);
        if(!output) cout << "\nInvalid output file path!" << endl;
    }while(!output);
}
//Returns true if a character is a number and false if not.
bool isNumber(char c){
    return c >= 48 && c <= 57;
}
//Calculates the amount of digits in a number.
int getDigitAmount(int number){
    int digits = 0;
    while(number != 0){
        digits++;
        number = (number - (number % 10))/10;
    }
    return digits;
}
//Calculates a power of ten given the input.
int getTenPower(int power){
    int startValue = 1;
    for(int i = 0; i < power; i++){
        startValue *= 10;
    }
    return startValue;
}
//Puts a character in the output file. Mainly helpful for debugging.
int put(ofstream & output, char c){
    static int amountUsed = 0;
    amountUsed++;
    output.put(c);
    return amountUsed;
}
//Calculates the compression rate with the original length and compressed length of files.
int getCompressionRate(int original, int compressed){
    return (int)((((double)compressed * 100)/(double)original) + 0.5);
}
void printClassCount(int * list){
    for(int i = 0; i < 10; i++){
        cout << "Amount with " << i+1 << " digits: " << list[i] << endl;
    }
}
void handleCommands(char c, char previous, int & accumulated){
    if(previous == ':' && c == 'R'){
        accumulated = 0;
    } else if(previous == ':' && c == 'P'){
        //palinidrome
    }
}

void handleNumbers(char c, char previous, int * list, int currentNumber, int accumulated){
    if(currentNumber == 0 && isNumber(c)){
        currentNumber = c - 48;
    } else if(isNumber(previous) && isNumber(c)){
        currentNumber = (currentNumber * 10) + (c-48);
    } else if(currentNumber != 0){
        list[getDigitAmount(currentNumber)-1]++;
        accumulated += currentNumber;
        currentNumber = 0;
    }
}
void handleCharacters(ofstream & output, int & regularLength, int & compressedLength, int & sameCharCount, char & c, char & previous){
    regularLength++;
    if(c == previous){
        sameCharCount++;
    } else{
        if(sameCharCount != 0){
            sameCharCount++; //plus one for the to-be-encoded character that's not counted.
            int digits = getDigitAmount(sameCharCount);
            for(int i = 0; i < digits; i++){
                int tenPower = getTenPower(digits-1); //power of ten corresponding to the position of the digit in the number.
                int biggestNumber = sameCharCount - (sameCharCount % tenPower); //most left digit in the number gives the biggest number.
                compressedLength = put(output,(biggestNumber/tenPower) + 48);
                sameCharCount -= biggestNumber;
            }
        }
        if(isNumber(c) || c == '\\'){
            compressedLength = put(output,'\\');
            compressedLength = put(output,c);
        }else{
            compressedLength = put(output,c);
        }
    }
}
//Compresses a file
void compress(ifstream & input, ofstream & output){
    int classCount[10] = {0,0,0,0,0,0,0,0,0,0}; //counter for numbers with the same amount of digits encountered.
    int regularLength = 0; //length of original file.
    int compressedLength; //length of compressed file.
    int sameCharCount = 0; //count for identical characters in a row.
    char c = input.get(); //most recently read character from input stream
    char previous; //previous character read from the input stream
    int currentNumber = 0; //current number that's being read
    int accumulatedNumbers = 0;

    while(!input.eof()){
        handleNumbers(c,previous,classCount, currentNumber, accumulatedNumbers);
        handleCharacters(output,regularLength,compressedLength,sameCharCount,c,previous);
        previous = c;
        c = input.get();
    }
    printClassCount(classCount);
    cout << "Succesfully compressed the file with a compression rate of " << getCompressionRate(regularLength, compressedLength) << "\n";
}
//main function
int main(){
    ifstream input; //input file stream, to read from
    ofstream output; //output file strea, to read to
    initStreams(input, output);
    cout << "Now compressing..." << endl;
    compress(input,output);
    //close streams.
    input.close();
    output.close();
}
