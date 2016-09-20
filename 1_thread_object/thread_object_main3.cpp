//
// Created by yanlei on 16-9-20.
//
#include <iostream>
#include <thread>

using namespace std;

void do_something(int& i)
{
    i++;
    cout << i << " ";
}

class func
{
public:
    func(int& i) : i_(i)
    {

    }

    void operator()()
    {
        for(int j=0;j<100;j++)
        {
            do_something(i_);
        }
    }

public:
    int &i_;
};

void loops()
{
    int local = 0;
    func my_func(local);
    thread my_thread(my_func);
    //my_thread.detach();       //如果不等待线程完成，一定要确保线程存取的对象是有效的
    my_thread.join();
    cout<<"Loops()"<<local<<endl;
}

int main()
{

    loops();

    return 0;
}

