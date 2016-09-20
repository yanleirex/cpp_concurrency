//
// Created by yanlei on 16-9-20.
//

#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>

#include <boost/timer.hpp>

using namespace std;

template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        //每个子序列累加，不能通过线程的返回值返回累加值，而是通过一个result引用
        //将结果返回给主线程
        //cout<<"first:"<<first<<"last:"<<last<<endl;
        result = std::accumulate(first, last, result);
        cout<<"result="<<result<<endl;
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    if(!length)
    {
        //序列为空
        return init;
    }
    unsigned long const min_per_thread = 25;    //每个线程的子线程序列大小
    unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread; //计算线程数
    unsigned long const hardware_threads = std::thread::hardware_concurrency(); //获取PC的cpu core数目，c++库可能无法访问该信息，可能返回0
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads:2, max_threads);
    unsigned long const block_size = length/num_threads;
    cout<<"min_per_thread="<<min_per_thread<<endl;
    cout<<"max_threads="<<max_threads<<endl;
    cout<<"hardware_threads="<<hardware_threads<<endl;
    cout<<"block_size="<<block_size<<endl;
    vector<T> results(num_threads);//每个线程的结果保存在results中
    vector<thread> threads(num_threads-1);//开启线程池，只保存num_threads-1个子线程，因为主线程也可以计算一个序列
    Iterator block_start = first;//序列开始位置
    for(int i=0;i<(num_threads-1);i++)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);//迭代器block_end向前移动block_size
        threads[i] = thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        //每个子线程的子序列分配
        block_start=block_end;//更新序列位置
    }
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads-1]);//主线程的任务，注意last
    std::for_each(threads.begin(), threads.end(), std::mem_fun_ref(&thread::join));//等待子线程完成
    return std::accumulate(results.begin(), results.end(), init);//汇总结果
};

int main()
{
    vector<int> vec;
    for(int i=0;i<10000;i++)
    {
        vec.push_back(10);
    }

    boost::timer t;
    int sum=parallel_accumulate(vec.begin(), vec.end(), 0);
    cout<<"sum="<<sum<<endl;
    double t1 = t.elapsed();
    cout<<"paraller_accumulate time: "<<t1<<endl;

    boost::timer t_;
    int sum1 = std::accumulate(vec.begin(), vec.end(), 0);
    cout<<"sum1="<<sum1<<endl;
    double t2 = t_.elapsed();
    cout<<"accumulate time: "<<t2<<endl;

    return 0;
}

