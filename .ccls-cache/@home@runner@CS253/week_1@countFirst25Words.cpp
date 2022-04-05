#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

set<string> getStopWords(string fileName);
unordered_map<string,int> getWordCounts(string fileName, set<string> stopWords);
unordered_map<int, set<string>> getReversed(unordered_map<string,int> wordCounts);
int getMaxFreq(unordered_map<int, set<string>> freqs);
vector<string> getWords(string line);
void printFirst(int nWords, int maxFreq, unordered_map<int, set<string>> freqs);
int isStopChar(char c);


int main(int argc, char** argv) {
  if (argc != 3){
    cout << "Please, as an input, provide:\n1. The stop words file \n 2.The file to analyse" << endl;
    return 0;
  }
  set<string> stopWords = getStopWords(argv[1]);
  
  unordered_map<string,int> wordCounts = getWordCounts(argv[2], stopWords);

  unordered_map<int, set<string>> freqs = getReversed(wordCounts);
  
  int maxFreq = getMaxFreq(freqs);

  printFirst(25, maxFreq, freqs);
  
  return 0;
}

set<string> getStopWords(string fileName){
  set <string> stopWords;
  ifstream sfile(fileName);
  while (sfile) {
    string line;
    if (!getline(sfile, line))
      break;
    
    istringstream lineStream(line);
    
    while (lineStream) {
      string word;
      if (!getline(lineStream, word, ','))
        break;
      stopWords.insert(word);
    }
  }
  return stopWords;
}

unordered_map<string,int> getWordCounts(string fileName, set<string> stopWords){
  unordered_map<string,int> wordCounts;
  ifstream tfile(fileName);
  
  while (tfile) {
    string line;
    if (!getline(tfile, line))
      break;
    for (auto &word : getWords(line)) {
      if (stopWords.count(word) == 0)
          wordCounts[word] += 1;
    }
  }
  return wordCounts;
}

vector<string> getWords(string line){
  string word = "";
  vector<string> words;
  for (auto &ch : line) {
    ch = tolower(ch);
    if (isStopChar(ch)){
      if (word.size()>0)
        words.push_back(word);

      word = "";
    } else { 
        word += ch;
      }
  }
  if (word.size()>0)
        words.push_back(word);
  
  return words;
}

unordered_map<int, set<string>> getReversed(unordered_map<string,int> wordCounts){
  unordered_map<int, set<string>> freqs;
  for (auto wordCount : wordCounts)
    freqs[wordCount.second].insert(wordCount.first);
  
  return freqs;

}

int getMaxFreq(unordered_map<int, set<string>> freqs){
  int maxFreq = 0;
  for (auto freq : freqs)
    maxFreq = max(maxFreq, freq.first);

  return maxFreq;
}

void printFirst(int nWords, int maxFreq, unordered_map<int, set<string>> freqs){
  int freq = maxFreq;
  int printed = 0;
  
  while (printed < 25){
    while (freqs[freq].empty()){
      freq --;
    }
    string word = *freqs[freq].begin();
    if (word.size() >  1){
      cout << word << " - " << freq << endl;
      printed ++;
    }
    freqs[freq].erase(word);
  }
}

int isStopChar(char c){
  return !isalnum(c);
}



