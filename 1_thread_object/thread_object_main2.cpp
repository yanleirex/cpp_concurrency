//
// Created by yanlei on 16-9-20.
//
#include <iostream>
#include <unistd.h>
#include <boost/thread.hpp>

using namespace std;

void fun()
{
    for(int i=0;i<5;i++)
    {
        cout<<"thread::fun()"<<endl;
        sleep(1);
    }
}

int main()
{
    {
        boost::thread t(fun);
        //t.join(); //主线程等待子线程完成后退出
        t.detach(); //主线程不需要等待子线程完成
    }
    //当一个thread对象销毁前，必须显示的指定是join还是detach线程
    cout<<"main exit"<<endl;
    sleep(2);
    return 0;
}

