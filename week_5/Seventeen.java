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

class WordFrequencyController{
  private StopWordManager _stopWordM;
  private DataStorageManager _dataStorageM;
  private WordFrequencyManager _wordFrequencyM;
  
  public WordFrequencyController(String stopWordFile, String dataFile){
    this._stopWordM = new StopWordManager(stopWordFile);
    this._dataStorageM = new DataStorageManager(dataFile);
    this._wordFrequencyM = new WordFrequencyManager();
  }

  public void run(){
    try{
      Class stringParam = Class.forName("java.lang.String");
      
      Class DSM = Class.forName("DataStorageManager");
      Method words = DSM.getMethod("words");

      Class SWM = Class.forName("StopWordManager");
      Method isStopWord = SWM.getMethod("isStopWord", stringParam);

      Class WFM = Class.forName("WordFrequencyManager");
      Method incrementCount = WFM.getMethod("incrementCount", stringParam);
      Method sorted = WFM.getMethod("sorted");

      
      for (String word : (String [])words.invoke(this._dataStorageM)){
        if (!(Boolean)isStopWord.invoke(this._stopWordM, word)){
          incrementCount.invoke(this._wordFrequencyM, word);
        }
      }
      int count = 0;
      HashMap<String, Integer> wordFreqs = (HashMap<String, Integer>) sorted.invoke(this._wordFrequencyM);
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

public class Seventeen {

  public static void printClass(String name){
    try {
      Class cls = Class.forName(name);
      System.out.println("-------------CLASS' FIELDS-------------");
      for(Field field : cls.getDeclaredFields()){
        System.out.println("Field name: " + field.getName());
        System.out.println("Field type: " + field.getType()); 
        System.out.println();
      }
      System.out.println("--------------------------------------");
      System.out.println("------------CLASS' METHODS------------");
      for(Method method : cls.getDeclaredMethods()){
        System.out.println("Method name: " + method.getName());
        System.out.println("Parameters: ");
        for(Class pmt: method.getParameterTypes()){
          System.out.println("      -" + pmt.toString());
        }
        System.out.println("Return type: " + method.getReturnType());
        System.out.println();
      }
      System.out.println("-------------------------------------");
      System.out.println("----------CLASS' INTERFACES----------");
      for(Class intf : cls.getInterfaces()){
        System.out.println("Interface name: " + intf.getName());
        System.out.println();
      }
      System.out.println("-------------------------------------");
      System.out.println("Superclass name: " + cls.getSuperclass().getName());
    } catch(Throwable e){
      System.err.println(e);
    }
      
  }
  
  public static void main(String[] args) {
    new WordFrequencyController(args[0], args[1]).run();
    
    Scanner userInput = new Scanner(System.in);  
    System.out.println("Welcome to the class inspector program:");
    while (true){
      System.out.print("Write the name of the class to inspect. Write exit to finish the program: ");
      String className = userInput.nextLine(); 
      if (className.equals("exit")){
        System.out.println("Killing...");
        break;
      }
      printClass(className);
    }
    

    
    
  }
}
