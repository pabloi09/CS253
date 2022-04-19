//Infinite  mirror
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;


set <string> stopWords;
vector<vector<string>> inputText;
unordered_map<string,int> wordCounts; // <word, frequency>
vector<pair<string, int>> words;
void mergeSort(int left, int right);
void mergeList(int left, int med, int right);



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

void getList(){
  for (pair <string, int> keyValue: wordCounts){
    words.push_back(keyValue);
  }
}

void sortList(){
  mergeSort(0, words.size()-1);
}

void mergeSort(int left, int right){
  if (left < right) {
    int med = (left + right) / 2;
    
    mergeSort(left, med);
    mergeSort(med+1, right);

    mergeList(left, med, right);
  }
}

void mergeList(int left, int med, int right){
  int size1 = med - left + 1;
  int size2 = right - med;
  
  vector<pair<string,int>> leftList(size1);
  vector<pair<string,int>> rightList(size2);

  for (int i = 0; i < size1; i++)
    leftList[i] = words[left+i];
  for (int i = 0; i < size2; i++)
    rightList[i] = words[med+1+i];

  int i, j, k;
  i = 0;
  j = 0;
  k = left;

  while (i < size1 && j < size2){
    if(leftList[i].second > rightList[j].second){
      words[k] = leftList[i];
      i++;
    } else {
      words[k] = rightList[j];
      j++;
    }
    k++;
  }

  while (i < size1){
    words[k] = leftList[i];
    i++;
    k++;
  }
  
  while (j < size2){
    words[k] = rightList[j];
    j++;
    k++;
  }
  
}


void printResult(){
  for (int i= 0; i < 25; i ++){
    cout << words[i].first << " - " << words[i].second << endl;
  }
}

int main(int argc, char** argv) {
  checkInput(argc);
  readStopWords(argv);
  readInputFile(argv);
  filterWords();
  getList();
  sortList();
  printResult();
}