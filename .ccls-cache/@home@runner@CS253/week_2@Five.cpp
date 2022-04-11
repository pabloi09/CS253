//COOKBOOK
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

set <string> stopWords;
vector<vector<string>> inputText;
unordered_map<string,int> wordCounts; // <word, frequency>
unordered_map<int, set<string>> freqs; // <frequency, set<words>>
int maxFreq = 0; // To iterate in order at the end

void checkInput(int argc){
  if (argc != 3){
    cout << "Please, as an input, provide:\n1. The stop words file \n 2.The file to analyse" << endl;
    exit(EXIT_FAILURE);
  }
}

void readStopWords(char** argv){
  // We store the stop words in a set
  ifstream sfile(argv[1]);
  while (sfile) {
    string word;
    if (!getline(sfile, word, ',')) break;
    stopWords.insert(word);
  }
}

void readInputFile(char** argv){
  ifstream tfile(argv[2]);
  
  //We read the file line by line and save the words frequencies
  while (tfile) {
    string line;
    if (!getline(tfile, line)) break;
    string word = "";
    //Chapter by chapter, if the chapter is not alphanumeric, then we have found the end of the word
    vector<string> lineWords;
    for (auto &ch : line) {
      ch = tolower(ch);
      if (!isalnum(ch)){
        lineWords.push_back(word);
        word = "";
      } else word += ch;
    }
    
    // Last chapter might be alhpannumeric, so we may need to store one more word
    lineWords.push_back(word);
    inputText.push_back(lineWords);
  }
}

void filterWords(){
  for (auto &line: inputText){
    for (auto & word: line){
      if (stopWords.count(word) == 0 && word.size() > 1)
        wordCounts[word] += 1;
    }
  }
}


void getFrequencies(){
  for (auto& tuple: wordCounts){
    freqs[tuple.second].insert(tuple.first);
    maxFreq = max(maxFreq, tuple.second);
  }
    
}

void printResult(){
  //We explore our dictionary of sets starting from the highest frequency
  int freq = maxFreq;
  for (int printed = 0; printed < 25; printed ++){
    while (freqs[freq].empty()) freq --;
    
    string word = *freqs[freq].begin();
    cout << word << " - " << freq << endl;
    freqs[freq].erase(word);
  }
}

int main(int argc, char** argv) {
  checkInput(argc);
  readStopWords(argv);
  readInputFile(argv);
  filterWords();
  getFrequencies();
  printResult();
}