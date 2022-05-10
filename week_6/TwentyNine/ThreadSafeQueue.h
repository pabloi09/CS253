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

  T pop(void){
      std::unique_lock<std::mutex> lock(mtx);
      while (fifo_queue.empty()){
      // release lock as long as the wait and reaquire it afterwards.
          cv.wait(lock);
      }
      T val = fifo_queue.front();
      fifo_queue.pop();
      return val;
  }

private:
  std::queue<T> fifo_queue;
  mutable std::mutex mtx;
  std::condition_variable cv;
};