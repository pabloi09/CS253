#include "WFApp.h"

using namespace std;

int main(int argc, char** argv) {
  
  WordFrequencyManager* wordFrequencyManager = new WordFrequencyManager();
  
  StopWordManager* stopWordManager = new StopWordManager();
  stopWordManager->send(Message("init", argv[1], wordFrequencyManager));

  DataStorageManager* dataStorageManager = new DataStorageManager();
  dataStorageManager->send(Message("init", argv[2], stopWordManager));
  
  WordFrequencyController* wordFrequencyController = new WordFrequencyController();
  wordFrequencyController->send(Message("run", dataStorageManager));

  wordFrequencyManager->join();
  stopWordManager->join();
  dataStorageManager->join();
  wordFrequencyController->join();
  
}