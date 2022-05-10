#include <string>
#include <tuple>
#include <set>
#include <unordered_map>
#include <vector> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

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

string readFile(string fileName){
  ifstream tfile(fileName);
  string data = "";
  //We read the file line by line and save the words frequencies
  while (tfile) {
    string line;
    if (!getline(tfile, line)) break;
    data += line + "\n";
  }
  return data;
}

vector<string> partition(string dataStr, int nLines){
  stringstream ss(dataStr);
  int line = 0;
  vector<string> partitions;
  string partition = "";
  while (ss) {
    string str;
    if (!getline(ss, str)) break;
    partition += str + "\n";
    line++;
    if (line == nLines){
      partitions.push_back(partition);
      partition = "";
      line=0;
    }
  }
  partitions.push_back(partition);
  
  return partitions;
}


vector<tuple<string,int>> splitWords(string str){
  vector<tuple<string,int>> result;
  
  stringstream ss(str);
  
  //We read the file line by line and save the words frequencies
  while (ss) {
    string line;
    if (!getline(ss, line)) break;
    string word = "";
    
    //Chapter by chapter, if the chapter is not alphanumeric, then we have found the end of the word
    for (auto &ch : line) {
      ch = tolower(ch);
      if (!isalnum(ch)){
        if (!isStopWord(word))
          result.push_back(make_tuple(word,1));
        word = "";
      } else word += ch;
    }
    // Last chapter might be alhpannumeric, so we may need to store one more word
    if (!isStopWord(word)) result.push_back(make_tuple(word,1));
  }
  return result;
}

string getRange(string word){
  char c = word[0];
  if (c<='e') return "a-e";
  else if (c<='j') return "f-j";
  else if (c<='o') return "k-o";
  else if (c<='t') return "p-t";
  else return "u-z";
  
}

unordered_map<string, vector<tuple<string,int>>> regroup(vector<vector<tuple<string,int>>> pairsList){
  unordered_map<string, vector<tuple<string,int>>> mapping;
  for (auto & group: pairsList)
    for (auto & pair: group)
      mapping[getRange(get<0>(pair))].push_back(pair);
  return mapping;
}

unordered_map<string, int> countWords(unordered_map<string, int> wordCount, vector<tuple<string,int>> pairsList){
  for (auto & pair: pairsList)
    wordCount[get<0>(pair)] += get<1>(pair);
  return wordCount;
}

bool comp(pair<string,int> a, pair<string,int> b) {
      return a.second > b.second;
}

vector<pair<string,int>> sort(unordered_map<string, int> wordCount){
  vector<pair<string,int>> result (wordCount.begin(), wordCount.end());
  sort(result.begin(), result.end(), comp);
  return result;
}

int main(int argc, char** argv) {
  getStopWords(argv[1]);
  
  vector<vector<tuple<string,int>>> splits;
  
  for (auto & split: partition(readFile(argv[2]),200))
    splits.push_back(splitWords(split));
  
  unordered_map<string, vector<tuple<string,int>>> splitsPerRange = regroup(splits);
  
  unordered_map<string, int> wordCount;
  for (auto & keyValue: splitsPerRange)
    wordCount = countWords(wordCount, keyValue.second);
  
  vector<pair<string,int>> sortedValues = sort(wordCount);
  
  for(int i=0; i < 25; i++)
    cout << sortedValues[i].first << " - " << sortedValues[i].second << endl;
  
}