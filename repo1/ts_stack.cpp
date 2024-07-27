//THREAD-SAFE STACK IMPLEMENTATION
#include <iostream>
#include <stack>
#include <mutex>
#include <stdexcept>

using namespace std;

template <typename T>
class threadsafeStack{

private:
    stack<T> st;
    mutable std::mutex m;
public:
    threadsafeStack() =default;
    T top()  const
    {
        std::lock_guard<mutex> ll(m);
        if(st.empty())
            {
                throw std::runtime_error("stack is empty during top operation");
            }
        T tmp=st.top();
        return tmp;
    }
    bool empty () const
    {

                std::lock_guard<mutex> ll(m);
                if(!st.empty())
                    return false;
                return true;
    }
    void push(const T &obj)
    {
                std::lock_guard<mutex> ll(m);
                st.push(std::move(obj));
    }
    void pop()
    {
               std::lock_guard<mutex> ll(m);
               if(st.empty())
                {
                    throw std::runtime_error("pop in an empty stack\n");
                }
                st.pop();
    }
};

int main()
{
    threadsafeStack<int> st;
    st.push(10);
    st.push(5);
    st.top();
    st.pop();
    st.pop();
  //  st.top();
 //   st.pop();  //uncommenting would lead to runtime error-> popping from empty stack
    return 0;
}
