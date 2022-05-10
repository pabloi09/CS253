#include <condition_variable>
#include <mutex>
#include <queue>

template <class T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue() : fifo_queue(), mtx(), cv() {}

  ~ThreadSafeQueue() {}

  void put(T t){
      std::lock_guard<std::mutex> lock(mtx);
      fifo_queue.push(t);
      cv.notify_one();
  }

  T pop(int timeout){
      T val;
      std::unique_lock<std::mutex> lock(mtx);
      if(!cv.wait_for(lock, std::chrono::milliseconds(timeout), [this] { return !fifo_queue.empty(); })){
        empty = true;
        return val;
      }
      val = fifo_queue.front();
      fifo_queue.pop();
      return val;
  }

  bool isEmpty(){
    return empty;
  }

private:
  std::queue<T> fifo_queue;
  mutable std::mutex mtx;
  std::condition_variable cv;
  bool empty;
};