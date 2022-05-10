#include "WFApp.h"

void StopWordManager::dispatch(Message message){
    if (message.body == "init"){
      this->_init(message);
    } else if(message.body == "filter"){
     this->_filter(message);
    }else{
      this->_wordFreqsManager->send(message);
    }
}

void StopWordManager::_init(Message message){
    // We store the stop words in a set
    ifstream sfile(message.fileName);
    while (sfile) {
      string word;
      if (!getline(sfile, word, ',')) break;
      _stopWords.insert(word);
    }
    this->_wordFreqsManager = message.wordFrequencyManager;
}

bool StopWordManager::_isStopWord(string word){
    return this->_stopWords.count(word) != 0;
}

void StopWordManager::_filter(Message message){
    string word = message.word;
    if (!this->_isStopWord(word)){
      this->_wordFreqsManager->send(Message("word", word));
    }
}

void DataStorageManager::dispatch(Message message){
  if (message.body == "init"){
    this->_init(message);
  } else if(message.body == "send_word_freqs"){
    this->_processWords(message);
  }else{
    //Forward
    this->_stopWordManager->send(message);
  }
}

void DataStorageManager::_init(Message message){
  ifstream tfile(message.fileName);
  this->_stopWordManager = message.stopWordManager;
  //We read the file line by line and save the words frequencies
  while (tfile) {
    string line;
    if (!getline(tfile, line)) break;
    string word = "";
    
    //Chapter by chapter, if the chapter is not alphanumeric, then we have found the end of the word
    for (auto &ch : line) {
      ch = tolower(ch);
      if (!isalnum(ch)){
        this->_data+= " " + word;
        word = "";
      } else word += ch;
    }
    
    // Last chapter might be alhpannumeric, so we may need to store one more word
    this->_data += " " + word;
  }
}

void DataStorageManager:: _processWords(Message message){
  WordFrequencyController* recipient = message.wordFrequencyController;
  stringstream ss(this->_data);
  string word;
  while (ss >> word) {
    this->_stopWordManager->send(Message("filter", word, recipient));
  }
  this->_stopWordManager->send(Message("top25", recipient));
  
}

void WordFrequencyController::dispatch(Message message){
  if (message.body == "run"){
    this->_run(message);
  } else if(message.body == "top25") {
    this->_display(message);
  }else {
    throw invalid_argument("Message not valid: "+message.body);
  }
}

void WordFrequencyController::_run(Message message){
  this->_dataStorageManager = message.dataStorageManager;
  this->_dataStorageManager->send(Message("send_word_freqs", this));
}

void WordFrequencyController::_display(Message message){
  //We explore our dictionary of sets starting from the highest frequency
  int freq = message.maxFreq;
  unordered_map<int, set<string>> freqs = message.freqs;
  
  for (int printed = 0; printed < 25; printed ++){
    while (freqs[freq].empty()) freq --;
    
    string word = *freqs[freq].begin();
    cout << word << " - " << freq << endl;
    freqs[freq].erase(word);
  }
  this->_dataStorageManager->send(Message("die"));
  this->stop();
}

void WordFrequencyManager::dispatch(Message message){
    if (message.body == "word"){
      this->_incrementCount(message.word);
    } else if(message.body == "top25") {
      this->_top25(message);
    }
}

void WordFrequencyManager::_incrementCount(string word){
    if(word.size() > 1) this->_wordCounts[word] += 1;
}

void WordFrequencyManager:: _top25(Message message){
    WordFrequencyController* recipient = message.wordFrequencyController;
    unordered_map<int, set<string>> freqs; 
    
    int maxFreq = 0; // To iterate in order at the end
    for (auto& tuple: this->_wordCounts){
      freqs[tuple.second].insert(tuple.first);
      maxFreq = max(maxFreq, tuple.second);
    }
    recipient->send(Message("top25", freqs, maxFreq));
}
