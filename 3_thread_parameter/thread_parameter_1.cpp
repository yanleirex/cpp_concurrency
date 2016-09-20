//
// Created by yanlei on 16-9-20.
//

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <unistd.h>
#include <boost/shared_ptr.hpp>

using namespace std;

class test
{
public:
    test(int i=0):data(i){}
    test(const test& one)
    {
        data = one.data;
        cout<<"test::copy constructor"<<endl;
    }
    test&operator=(const test& one)
    {
        data = one.data;
        cout<<"test::operator=()"<<endl;
        return *this;
    }

    ~test()
    {
        cout<<"test::destructor"<<endl;
    }

    void operator()()
    {
        cout<<"test::operator() "<<data<<endl;
    }

    void show()
    {
        cout<<"test::show() "<<data<<endl;
    }

public:
    int data;
};

void func(test& one)
{
    cout<<"func get the data "<<one.data<<endl;
}

void func1(test& one)
{
    cout<<"func get the data "<<one.data++<<endl;
}

void loops()
{
    /*
    test one(10);
    boost::thread t(func, one);
    t.join();
     */
    test one(10);
    boost::thread t(func1, boost::ref(one));
    t.join();
    cout<<"loops() get the data "<<one.data<<endl;
}

void loops1()
{
    test one(10);
    boost::thread my_thread(&test::show, &one);
    my_thread.join();
}

void fun(boost::shared_ptr<test> ptr)
{
    ptr->data++;
}

void fun_(test* ptr)
{
    ptr->data++;
}

void loops3()
{
    boost::shared_ptr<test> ptr(new test(10));
    boost::thread my_thread(fun, ptr);
    my_thread.join();
    cout<<"shared_ptr "<<ptr->data<<endl;

    test* one = new test(10);
    boost::thread t(fun_, one);
    t.join();
    cout<<"test* "<<one->data<<endl;
    delete one;
}


int main()
{
    //loops();
    //loops1();
    loops3();
    return 0;
}

