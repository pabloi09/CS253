#include <unordered_map>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include "ActiveWFObject.cpp"

class WordFrequencyManager;
class StopWordManager;
class DataStorageManager;
class WordFrequencyController;


class WordFrequencyManager : public ActiveWFObject {
public:
  WordFrequencyManager():ActiveWFObject("WordFrequencyManager"){};
  void dispatch(Message message) override;

private:
  unordered_map<string,int> _wordCounts; 

  void _incrementCount(string word);

  void _top25(Message message);
};

class StopWordManager: public ActiveWFObject {
public:
  StopWordManager():ActiveWFObject("StopWordManager"){};
  void dispatch(Message message) override;
private:
  set <string> _stopWords;
  WordFrequencyManager* _wordFreqsManager;

  void _init(Message message);

  bool _isStopWord(string word);

  void _filter(Message message);
};

class DataStorageManager: public ActiveWFObject{
public:
  DataStorageManager():ActiveWFObject("DataStorageManager"){};
  void dispatch(Message message) override;

private:
  string _data = "";
  StopWordManager* _stopWordManager;

  void _init(Message message);

  void _processWords(Message message);
};

class WordFrequencyController: public ActiveWFObject{
public:
  WordFrequencyController():ActiveWFObject("WordFrequencyController"){};
  void dispatch(Message message) override;
private:
  DataStorageManager* _dataStorageManager;

  void _run(Message message);

  void _display(Message message);

};