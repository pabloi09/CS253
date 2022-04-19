//THE ONE
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
using namespace std;

template <typename T>

class Wrapper{
  public:
    T value;
    Wrapper(T v){
      value = v;
    }
};

class TFTheOne {
private:
  Wrapper<> value;
public:
  TFTheOne(T v){
    value = v;
  }

  TFTheOne bind(T (*func)(T)){
        this->value = func(this->value);
        return this;
  }

  void printme(){
    cout << this->value << endl;
  }
};

char** checkInput(tuple<int, char**> args){
  int argc; char** argv; tie(argc, argv) = args;
  if (argc != 3){
    cout << "Please, as an input, provide:\n1. The stop words file \n 2.The file to analyse" << endl;
    exit(EXIT_FAILURE);
  }
  return argv;
}

tuple<char**, set<string>> readStopWords(char** argv){
  // We store the stop words in a set
  ifstream sfile(argv[1]);
  set <string> stopWords;
  while (sfile) {
    string word;
    if (!getline(sfile, word, ',')) break;
    stopWords.insert(word);
  }
  return make_tuple(argv, stopWords);
  
}

tuple<set<string>, vector<vector<string>>> readInputFile(tuple<char**, set<string>> args){
  char** argv; set<string> stopWords; tie(argv, stopWords) = args;
  
  vector<vector<string>> inputText;
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
  return make_tuple(stopWords, inputText);
}

unordered_map<string,int> filterWords(tuple<set<string>, vector<vector<string>>> args){
  set<string> stopWords; vector<vector<string>> inputText; tie(stopWords, inputText) = args;
  
  unordered_map<string,int> wordCounts; // <word, frequency>
  for (auto &line: inputText){
    for (auto & word: line){
      if (stopWords.count(word) == 0 && word.size() > 1)
        wordCounts[word] += 1;
    }
  }

  return wordCounts;
}


tuple<unordered_map<int, set<string>>,int> getFrequencies(unordered_map<string,int> wordCounts){
  unordered_map<int, set<string>> freqs; // <frequency, set<words>>
  int maxFreq = 0; // To iterate in order at the end
  
  for (auto& tuple: wordCounts){
    freqs[tuple.second].insert(tuple.first);
    maxFreq = max(maxFreq, tuple.second);
  }

  return make_tuple(freqs, maxFreq);
    
}

string top25(tuple<unordered_map<int, set<string>>,int> args){
  //We explore our dictionary of sets starting from the highest frequency
  unordered_map<int, set<string>> freqs; int freq;  tie(freqs, freq) = args;
  string top25 = "";
  for (int printed = 0; printed < 25; printed ++){
    while (freqs[freq].empty()) freq --;
    
    string word = *freqs[freq].begin();
    top25 += word + " - " + to_string(freq) + "\n";
    freqs[freq].erase(word);
  }

  return top25;
}

int main(int argc, char** argv) {
  TFTheOne theOne(make_tuple(argc,argv));
  TFTheOne(sys.argv[1])
    .bind(read_file)\
    .bind(filter_chars)\
    .bind(normalize)\
    .bind(scan)\
    .bind(remove_stop_words)\
    .bind(frequencies)\
    .bind(sort)\
    .bind(top25_freqs)\
    .printme()
  checkInput(argc);
  readStopWords(argv);
  readInputFile(argv);
  filterWords();
  getFrequencies();
  printResult();
}