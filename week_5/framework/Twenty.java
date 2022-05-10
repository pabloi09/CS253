import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;

class Twenty {
  public static void main(String [] args) {
    try{
      String [] config  = Files.readString(Paths.get("/home/runner/CS253/week_5/config.ini")).split("\n");
      URL [] classURLs = {new URL(config[1])};
      URLClassLoader cloader = new URLClassLoader(classURLs);
      Class cls = cloader.loadClass(config[2]);
      System.out.println("Using plugin of name: " + config[2]);
      ((IApp) cls.getDeclaredConstructor(String.class, String.class).newInstance(args[0], args[1])).run();
      
    } catch(Exception e){
      System.err.println(e);
      System.err.println("E1");
    }
  }
}