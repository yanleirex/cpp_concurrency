//
// Created by yanlei on 16-9-20.
//
#include <mutex>
#include <iostream>

using namespace std;

class some_data
{
public:
    void do_something()
    {
        cout<<"there will use data do_something"<<endl;
    }

private:
    int a;
};

class data_wrapper
{
public:
    template<typename Functor>
            void process_data(Functor func)
    {
        lock_guard<mutex> guard(m);
        func(data);//类将保护数据泄露了
    }

private:
    some_data data;
    mutex m;
};

some_data* unproected;
void malicious_function(some_data& protected_data)
{
    //保护数据的类成员函数通过引用泄露了保护数据
    unproected = &protected_data;
}

data_wrapper x;

int main()
{
    x.process_data(malicious_function);
    unproected->do_something();//用户函数在没有任何保护措施下使用了数据
    return 0;
}

