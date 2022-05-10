#include "ThreadSafeQueue.h"
#include <string>
#include <tuple>
#include <set>
#include <unordered_map>
#include <vector>
#include <thread>  
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

ThreadSafeQueue<string> wordSpace;
ThreadSafeQueue<unordered_map<string,int>> freqSpace;
set <string> stopWords;

void getStopWords(string fileName){
  ifstream sfile(fileName);
  while (sfile) {
    string word;
    if (!getline(sfile, word, ',')) break;
    stopWords.insert(word);
  }
}

bool isStopWord(string word){
    return stopWords.count(word) != 0 | word.size() <= 1;
}

void populateWordSpace(string fileName){
  ifstream tfile(fileName);
  
  //We read the file line by line and save the words frequencies
  while (tfile) {
    string line;
    if (!getline(tfile, line)) break;
    string word = "";
    
    //Chapter by chapter, if the chapter is not alphanumeric, then we have found the end of the word
    for (auto &ch : line) {
      ch = tolower(ch);
      if (!isalnum(ch)){
        wordSpace.put(word);
        word = "";
      } else word += ch;
    }
    
    // Last chapter might be alhpannumeric, so we may need to store one more word
    wordSpace.put(word);
  }
}

void processWords(){
  unordered_map<string,int> wordCounts;
  while(!wordSpace.isEmpty()){
    string word = wordSpace.pop(1000);
    if (!isStopWord(word)) 
      wordCounts[word] += 1;
  }
  freqSpace.put(wordCounts);
}


int main(int argc, char** argv) {
  
  getStopWords(argv[1]);

  populateWordSpace(argv[2]);

  vector<thread> workers;
  for(int i=0; i<5; i++)
    workers.push_back(thread(processWords));
  
  for(auto & worker: workers)
    worker.join();

  unordered_map<string,int> wordFreqs;
  while(!freqSpace.isEmpty()){
    unordered_map<string,int> wordCounts = freqSpace.pop(1000);
    for (auto& tuple: wordCounts){
      wordFreqs[tuple.first] += tuple.second;
    }
  }

  unordered_map<int, set<string>> freqs; 
  int maxFreq = 0; // To iterate in order at the end
  
  for (auto& tuple: wordFreqs){
    freqs[tuple.second].insert(tuple.first);
    maxFreq = max(maxFreq, tuple.second);
  }
  int freq = maxFreq;
  
  for (int printed = 0; printed < 25; printed ++){
    while (freqs[freq].empty()) freq --;
    
    string word = *freqs[freq].begin();
    cout << word << " - " << freq << endl;
    freqs[freq].erase(word);
  }

}