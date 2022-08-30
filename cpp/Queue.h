#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <queue>

template<typename T>
class Queue
{
public:
  void push(T const & element)
  {
    q.push(element);
  }
  bool pop(T & element)
  {
    std::lock_guard<std::mutex> lock(m);
    if(q.empty()) return false;
    else {
      element = std::move(q.front());
      q.pop();
      return true;
    }
  }
private:
  std::queue<T> q;
  std::mutex m;
};

#endif /* QUEUE_H */
