//
// Created by yanlei on 16-9-20.
//
#include <list>
#include <mutex>
#include <algorithm>
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

list<int> myList;
mutex listMutex;

void insert()
{
    sleep(2);
    {
        lock_guard<mutex> guard(listMutex);
        myList.push_front(12);
        cout<<"insert()"<<endl;
        sleep(2);//占有锁
    }
    for(list<int>::iterator it=myList.begin();it!=myList.end();it++)
        cout<<*it<<" ";
    cout<<endl;
}

void traverse()
{
    sleep(0);//让insert先持有锁
    {
        lock_guard<mutex> guard(listMutex);
        for(list<int>::iterator it=myList.begin();it!=myList.end();it++)
            cout<<*it<<" ";
        cout<<endl;
    }
    cout<<"traverse()"<<endl;
}

int main()
{
    myList.push_back(1);
    myList.push_back(2);
    thread firstThread(insert);
    thread secondThread(traverse);
    firstThread.join();
    secondThread.join();
    return 0;
}

