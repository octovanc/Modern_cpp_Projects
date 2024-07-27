//THREAD-SAFE STACK IMPLEMENTATION
#include <iostream>
#include <queue>
#include <mutex>
#include <stdexcept>

using namespace std;

template <typename T>
class threadsafe_queue{

private:
    queue<T> q;
    mutable std::mutex m;
public:
    threadsafe_queue() =default;
    T front()  const
    {
        std::lock_guard<mutex> ll(m);
        if(q.empty())
            {
                throw std::runtime_error("queue is empty during front operation");
            }
        T tmp=q.front();
        return tmp;
    }
    bool empty () const
    {

                std::lock_guard<mutex> ll(m);
                if(!q.empty())
                    return false;
                return true;
    }
    void enqueue(const T &obj)
    {
                std::lock_guard<mutex> ll(m);
                q.push(std::move(obj));
    }
    void dequeue()
    {
               std::lock_guard<mutex> ll(m);
               if(q.empty())
                {
                    throw std::runtime_error("dequeue in an empty queue\n");
                }
                q.pop();
    }
};

int main()
{
    threadsafe_queue<int> q;
    q.enqueue(10);
    q.enqueue(5);
    cout<<q.front();
    q.dequeue();
    q.dequeue();
   // q.front();
 //   q.dequeue();  //uncommenting would lead to runtime error-> dequeueping from empty queue
    return 0;
}
