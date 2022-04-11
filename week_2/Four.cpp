//MONOLITH
#include <fstream>
#include <iostream>
#include <set> 
#include <string>
#include <unordered_map>
using namespace std;
// Strings, sets, maps, arrays and i/o operations need to be imported in C++
// They come imported by default in some other languages such as python

int main(int argc, char** argv) {
  if (argc != 3){
    cout << "Please, as an input, provide:\n1. The stop words file \n 2.The file to analyse" << endl;
    exit(EXIT_FAILURE);
  }
  
  set <string> stopWords;
  ifstream sfile(argv[1]);
  
  // We store the stop words in a set
  while (sfile) {
    string word;
    if (!getline(sfile, word, ',')) break;
    stopWords.insert(word);
  }

  unordered_map<string,int> wordCounts; // <word, frequency>
  unordered_map<int, set<string>> freqs; // <frequency, set<words>>
  int maxFreq = 0; // To iterate in order at the end
  ifstream tfile(argv[2]);
  
  //We read the file line by line and save the words frequencies
  while (tfile) {
    string line;
    if (!getline(tfile, line)) break;
    string word = "";
    //Chapter by chapter, if the chapter is not alphanumeric, then we have found the end of the word
    for (auto &ch : line) {
      ch = tolower(ch);
      if (!isalnum(ch)){
        if (word.size() > 1 && stopWords.count(word) == 0){
          freqs[wordCounts[word]].erase(word);
          wordCounts[word] += 1; 
          freqs[wordCounts[word]].insert(word);
          maxFreq = max(maxFreq, wordCounts[word]);
        }
        word = "";
      } else word += ch;
    }
    
    // Last chapter might be alhpannumeric, so we may need to store one more word
    if (word.size()>0 && stopWords.count(word) == 0){
      freqs[wordCounts[word]].erase(word); 
      wordCounts[word] += 1; 
      freqs[wordCounts[word]].insert(word);
      maxFreq = max(maxFreq, wordCounts[word]);
    }
  }
  
  //We explore our dictionary of sets starting from the highest frequency
  int freq = maxFreq;
  for (int printed = 0; printed < 25; printed ++){
    while (freqs[freq].empty()) freq --;
    
    string word = *freqs[freq].begin();
    cout << word << " - " << freq << endl;
    freqs[freq].erase(word);
  }
  
}