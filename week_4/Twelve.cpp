//LETTERBOX
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
using namespace std;

//-----------Renaming tuple as a Message so the code is better understood---------------------
template<typename... _Elements>
using Message = tuple<typename __decay_and_strip<_Elements>::__type...>;

template<typename... _Elements>
Message<typename __decay_and_strip<_Elements>::__type...> newMessage(_Elements&&... __args){
  typedef Message<typename __decay_and_strip<_Elements>::__type...> __result_type;
  return __result_type(std::forward<_Elements>(__args)...);
}
//--------------------------------------------------------------------------------------------

class InputCheckManager{
public:
  bool dispatch(Message<string,int> message){
    if (get<0>(message) == "is_valid"){
      return _isValid(get<1>(message));
    }else{
      throw invalid_argument("Message not valid: "+get<0>(message));
    }
  }
private:
  bool _isValid(int argc){
    if (argc != 3){
      cout << "Please, as an input, provide:\n1. The stop words file \n 2.The file to analyse" << endl;
      return false;
    }
    return true;
  }
};

class StopWordManager{
public:
  bool dispatch(Message<string,string> message){
    if (get<0>(message) == "init"){
      _init(get<1>(message));
    } else if(get<0>(message) == "is_stop_word"){
      return _isStopWord(get<1>(message));
    }else{
      throw invalid_argument("Message not valid: "+get<0>(message));
    }
    return true;
  }
private:
  set <string> _stopWords;

  void _init(string file){
    // We store the stop words in a set
    ifstream sfile(file);
    while (sfile) {
      string word;
      if (!getline(sfile, word, ',')) break;
      _stopWords.insert(word);
    }
  }

  bool _isStopWord(string word){
    return _stopWords.count(word) != 0;
  }
};

class DataStorageManager{
public:
  vector<string> dispatch(Message<string,string> message){
    if (get<0>(message) == "init"){
      _init(get<1>(message));
    } else if(get<0>(message) == "words"){
      return _words();
    }else{
      throw invalid_argument("Message not valid: "+get<0>(message));
    }
    vector<string> nullResponse;
    return nullResponse;
  }

private:
  vector<vector<string>> _inputText;
  void _init(string file){
    ifstream tfile(file);
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
      _inputText.push_back(lineWords);
    }
  }

  vector<string> _words(){
    vector<string> words;
    for (auto &line: _inputText) for (auto & word: line) words.push_back(word);
    return words;
  }
};

class WordFrequyencyManager{
public:
  Message<unordered_map<int, set<string>>,int> dispatch(Message<string,string> message){
    if (get<0>(message) == "increment_count"){
      _incrementCount(get<1>(message));
    } else if(get<0>(message) == "freqs"){
      return _freqs();
    }else{
      throw invalid_argument("Message not valid: "+get<0>(message));
    }
    unordered_map<int, set<string>> nullResponse;
    return newMessage(nullResponse,0);
  }

private:
  unordered_map<string,int> _wordCounts; // <word, frequency>

  void _incrementCount(string word){
    if(word.size() > 1) _wordCounts[word] += 1;
  }

  Message<unordered_map<int, set<string>>,int> _freqs(){
    unordered_map<int, set<string>> freqs; // <frequency, set<words>>
    int maxFreq = 0; // To iterate in order at the end
    for (auto& tuple: _wordCounts){
      freqs[tuple.second].insert(tuple.first);
      maxFreq = max(maxFreq, tuple.second);
    }
    return newMessage(freqs, maxFreq);
  }
};

class WordFrequencyController{
public:
  void dispatch(Message<string, int, char**> message){
    Message<int,char**> args = newMessage(get<1>(message),get<2>(message));
    if (get<0>(message) == "init"){
      return _init(args);
    } else if(get<0>(message) == "run") {
      return _run();
    }else {
      throw invalid_argument("Message not valid: "+get<0>(message));
    }
  }
private:
  InputCheckManager _inputManager;
  StopWordManager _stopWordManager;
  DataStorageManager _dataStorageManager;
  WordFrequyencyManager _wordFrequencyManager;

  void _init(Message<int,char**> args){
    int argc; char ** argv; tie(argc, argv) = args;
    
    _inputManager = InputCheckManager();
    _stopWordManager = StopWordManager();
    _dataStorageManager = DataStorageManager();
    _wordFrequencyManager = WordFrequyencyManager();
    
    if (!_inputManager.dispatch(newMessage("is_valid",argc))) exit(EXIT_FAILURE);
    
    
    _stopWordManager.dispatch(newMessage("init",argv[1]));
    _dataStorageManager.dispatch(newMessage("init",argv[2]));
  }

  void _run(){
    for (auto & word: _dataStorageManager.dispatch(newMessage("words","NULL"))){
      if(! _stopWordManager.dispatch(newMessage("is_stop_word",word))){
        _wordFrequencyManager.dispatch(newMessage("increment_count",word));
      }
    }

    //We explore our dictionary of sets starting from the highest frequency
    int freq; unordered_map<int, set<string>> freqs;
    tie(freqs, freq) = _wordFrequencyManager.dispatch(newMessage("freqs","NULL"));
    
    for (int printed = 0; printed < 25; printed ++){
      while (freqs[freq].empty()) freq --;
      
      string word = *freqs[freq].begin();
      cout << word << " - " << freq << endl;
      freqs[freq].erase(word);
    }
  }

};

int main(int argc, char** argv) {
  char ** null;
  WordFrequencyController wordFreqController = WordFrequencyController();
  wordFreqController.dispatch(newMessage("init", argc, argv));
  wordFreqController.dispatch(newMessage("run", -1, null));
}