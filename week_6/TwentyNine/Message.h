#include <tuple>
#include <string>
#include <unordered_map>
#include <set>

using namespace std;
class WordFrequencyManager;
class StopWordManager;
class DataStorageManager;
class WordFrequencyController;

class Message{
public:
  string body;
  string word;
  string fileName;
  WordFrequencyManager* wordFrequencyManager;
  WordFrequencyController* wordFrequencyController;
  StopWordManager* stopWordManager;
  DataStorageManager* dataStorageManager;
  unordered_map<int, set<string>> freqs;
  int maxFreq;

  Message(string body, string fileName, WordFrequencyManager* wordFrequencyManager)
    : body(body)
    , fileName(fileName)
    , wordFrequencyManager(wordFrequencyManager){}

  Message(string body, string fileName, StopWordManager* stopWordManager)
    : body(body)
    , fileName(fileName)
    , stopWordManager(stopWordManager){}

  Message(string body, DataStorageManager* dataStorageManager)
    : body(body)
    , dataStorageManager(dataStorageManager){}

  Message(string body, string word)
    : body(body)
    , word(word){}

  Message(string body, string word, WordFrequencyController* wordFrequencyController)
    : body(body)
    , word(word)
    , wordFrequencyController(wordFrequencyController) {}

  Message(string body, WordFrequencyController* wordFrequencyController)
    : body(body)
    , wordFrequencyController(wordFrequencyController) {}

  Message(string body, unordered_map<int, set<string>> freqs,
  int maxFreq)
    : body(body)
    , freqs(freqs)
    , maxFreq(maxFreq){}

  Message(string body)
    : body(body){}
};