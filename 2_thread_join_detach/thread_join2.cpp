//
// Created by yanlei on 16-9-20.
//
#include <iostream>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

using namespace std;

class thread_guard:boost::noncopyable
{
public:
    explicit thread_guard(boost::thread& t):t_(t)
    {

    }
    ~thread_guard()
    {
        if(t_.joinable())//检测是很有必要的，因为thread::join只能调用一次，要防止其它地方意外join了
        {
            t_.join();
        }
    }

private:
    boost::thread& t_;
};

void do_something(int& i)
{
    i++;
}

class func
{
public:
    func(int& i):i_(i)
    {

    }
    void operator()()
    {
        for(int j=0;j<100;j++)
            do_something(i_);
    }

public:
    int& i_;
};

void do_something_in_current_thread()
{

}

void fun()
{
    int local=0;
    func my_func(local);
    boost::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread();
}

int main()
{
    fun();
    return 0;
}


