import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.*;
import java.util.stream.Collectors;
import java.lang.reflect.*;

class StopWordManager{
  private HashSet<String> _stopWords;
  public StopWordManager(String pathToFile){
    String file = "";
    try{
      file  = Files.readString(Paths.get(pathToFile));
    }catch(IOException e){
      System.out.println("File not found");
    }
    this._stopWords = new HashSet(Arrays.asList(file.split(",")));
      for (String letter: "abcdefghijklmnopqrstuvwxyz".split("")){
        this._stopWords.add(letter);
      }
  }

  public boolean isStopWord(String word){
    return this._stopWords.contains(word);
  }
}
  
class DataStorageManager{
  private String _data;
  public DataStorageManager(String pathToFile){
    try{
      this._data = Files.readString(Paths.get(pathToFile));
    }catch(IOException e){
      System.out.println("File not found");
    }
    this._data = this._data.replaceAll("[\\W_]+", " ").toLowerCase(); 
  }

  public String[] words(){
    return this._data.split(" ");
  }
}

class WordFrequencyManager{
  private HashMap<String, Integer> _wordFreqs;
  public WordFrequencyManager(){
    this._wordFreqs = new HashMap<>();
  }

  public void incrementCount(String word){
    this._wordFreqs.put(word, this._wordFreqs.getOrDefault(word, 0) + 1);
  }

  public HashMap<String, Integer> sorted(){
    return this._wordFreqs.entrySet().stream()
    .sorted(Comparator.comparing(Map.Entry::getValue, Comparator.reverseOrder()))
    .collect(Collectors.toMap(
      Map.Entry::getKey,
                      Map.Entry::getValue,
                      (e1, e2) -> e1, LinkedHashMap::new));
  }
}

public class App2 implements IApp {
  private StopWordManager _stopWordM;
  private DataStorageManager _dataStorageM;
  private WordFrequencyManager _wordFrequencyM;
  
  public App2(String stopWordFile, String dataFile){
      this._stopWordM = new StopWordManager(stopWordFile);
    
      this._dataStorageM = new DataStorageManager(dataFile);
    
      this._wordFrequencyM = new WordFrequencyManager();
  }

  public void run(){
    try{
      
      for (String word : this._dataStorageM.words()){
        if (!this._stopWordM.isStopWord(word)){
          this._wordFrequencyM.incrementCount(word);
        }
      }
      int count = 0;
      HashMap<String, Integer> wordFreqs = this._wordFrequencyM.sorted();
      for (Map.Entry<String, Integer> entry : wordFreqs.entrySet()) {
        System.out.println(entry.getKey() + " - " + entry.getValue());
        count ++;
        if (count==25){
          break;
        }
      }
    } catch (Throwable e){
      System.err.println(e);
    }
    
  }
}