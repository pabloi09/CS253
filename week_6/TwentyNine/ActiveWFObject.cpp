#include <typeinfo>
#include <thread>  
#include "ThreadSafeQueue.h"
#include "Message.h"
using namespace std;


class ActiveWFObject {
private:
  string name;
  ThreadSafeQueue<Message> fifo_queue;
  thread runnable;
  bool _stopMe;
public:
  ActiveWFObject(string name){
    this->name = name;
    this->_stopMe = false;
    this->runnable = thread(&ActiveWFObject::run, this);
  }
  
  void send (Message message){
    this->fifo_queue.put(message);
  }
  
  void run(){
    while(!this->_stopMe){
        Message message = this->fifo_queue.pop();
        this->dispatch(message);
        if (message.body == "die"){
          this->stop();
      }
    }
  }
  
  void join(){
    this->runnable.join();
  }

  void stop(){
    this->_stopMe = true;
  }
  
  virtual void dispatch(Message message) = 0;

};

