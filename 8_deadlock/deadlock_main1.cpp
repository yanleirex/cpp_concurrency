//
// Created by yanlei on 16-9-20.
//
/*
 * 1 死锁：每个线程都希望锁住一些列锁以执行某个操作，且每个线程持有一个不同的锁，最终每个线程都需要其它线程的锁，导致所有线程都不能向前执行。
 * 1.1 死锁的一个解决方式：每个线程对锁的请求顺序一致。C++库提供了顺序加锁机制，可以一次性锁住多个mutex而不会出现死锁：
 */
#include <mutex>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <functional>

using namespace std;

class big_object
{
public:
    big_object(int i=0):data(i){};
public:
    int data;
};

void swap(big_object& lhs, big_object& rhs)
{
    sleep(1);
    cout<<"swap()"<<endl;
}

class X
{
private:
    big_object some_detail;
    mutable std::mutex m;
public:
    X(big_object const& sd):some_detail(sd){};

    friend void swap(X& lhs, X& rhs)
    {
        cout<<"X swap.."<<endl;
        if(&lhs==&rhs)
            return;
        std::lock(lhs.m, rhs.m);    //c++库会自动生成加锁顺序，即使调用顺序不一致
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock); //adopt_lock是告诉lock_guard对象mutex已经被上锁
        //而lock_guard对象将获得mutex的所有权，这样就可以保证在lock可能出现异常导致没有unlock的情形不会出现，栈对象会在异常抛出后自动析构
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
        swap(lhs.some_detail, rhs.some_detail);
    }
};

void threadFun(X& one, X& two)
{
    swap(one, two);
}

/*
 * 1.2 可以通过比较多个mutex的地址大小实现顺序加锁，下面的程序是先锁地址小的mutex，再锁地址大的mutex。
 */

class test
{
public:
    friend void orderLock(test& one, test& two)
    {
        if(&one.m>&two.m)
        {
            lock_guard<mutex> smallGuard(two.m);
            sleep(1);
            cout<<"the second argument's mutex smaller"<<endl;
            lock_guard<mutex> bingGuard(one.m);
        }
        else if(&one.m==&two.m)
        {
            cout<<"lock the same mutex"<<endl;
        }
        else
        {
            lock_guard<mutex> smallGuard(one.m);
            sleep(1);
            cout<<"the first argument's mutex smaller"<<endl;
            lock_guard<mutex> bigGuard(two.m);
        }
    }

private:
    mutex m;
};


int main()
{
    /*
    big_object ten(10), hundred(100);
    X one(ten), two(hundred);
    thread threadOne(threadFun, ref(one), ref(two));//不同线程有不同的参数调用顺序
    thread threadTwo(threadFun, ref(two), ref(one));
    threadOne.join();
    threadTwo.join();
     */
    test one, two;
    cout<<&one<<" "<<&two<<endl;
    orderLock(one, two);
    orderLock(two, one);

    return 0;
}

