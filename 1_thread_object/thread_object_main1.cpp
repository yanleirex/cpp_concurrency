//
// Created by yanlei on 16-9-20.
//
#include <boost/thread.hpp>
#include <unistd.h>
#include <sys/syscall.h>
#include <iostream>

using namespace std;

void show()
{
    cout<<" Hello world "<<syscall(SYS_gettid)<<" "<<endl;;
}

int main()
{
    boost::thread t(show);
    cout<<"main pid "<<syscall(SYS_gettid)<<" "<<t.get_id()<<endl;

    if(!t.joinable())
    {
        cout<<"thread unjoinable"<<endl;
    }
    else
    {
        cout<<"thread joinable"<<endl;
        t.join();
    }
    return 0;
}

