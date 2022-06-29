// 006协程.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<string>
#include<future>    //引入future类
#include<coroutine> //引入coroutine_handle<>
#include<chrono>
//#include<experimental/coroutine>    //引入std::experimental名字空间
using namespace std;


/*************** co_yield 运算符 ****************/
/*
* co_yield 后跟对象必须实现promise_type类型内包类
* promise_type内包类需包含以下五个函数：
* <外部类型> get_return_object();
* suspend_always initial_suspend() const noexcept;
* suspend_always final_suspend() const noexcept;
* suspend_always yield_value(<type>& value);
* void return_void(){} 或 void return_value(int value)
* void unhandled_exception(){}
*/
struct my_int_generator
{
    //定义内包promise_type类
    struct promise_type
    {
        int* value_ = nullptr;
        my_int_generator get_return_object()
        {
            return my_int_generator{ *this };
        }
        auto initial_suspend() const noexcept
        {
            return suspend_always{};
        }
        auto final_suspend() const noexcept
        {
            return suspend_always{};
        }
        auto yield_value(int& value)
        {
            value_ = &value;
            return suspend_always{};
        }
        void return_void() {}
        void unhandled_exception() {}
    };

    explicit/*禁用"="复制构造转换*/ my_int_generator(promise_type& p) :handle_(coroutine_handle<promise_type>::from_promise(p))
    {}

    ~my_int_generator()
    {
        if (handle_)
            handle_.destroy();
    }

    coroutine_handle<promise_type> handle_;
};
my_int_generator foo1()
{
    for (int i = 0; i < 10; i++)
    {
        co_yield i;
    }
}
void example1()
{
    auto obj = foo1();
}
/*************** co_yield 运算符 ****************/










/*************** co_await 运算符 ****************/
/*co_awit 后跟随的对象的类必须实现以下三个函数 :
* bool awit_ready() const;  //协程是否可挂起
* void await_suspend(std::experimental::coroutine_handle<> h) const; //调度协程执行流程
* <type> await_resume() const;  //协程返回的结果
*/

//自定义的 string 子类可被 co_awit 运算符运算的类型
//class awaitable_string :public string
//{
//public:
//    using string::string;   //using 关键字直接引入父类string()的构造函数作为可选构造方式
//    bool await_ready() const { return true; }   //该协程默认均为可挂起
//    void await_suspend(coroutine_handle<> h) const //默认调度协程执行流程
//    {
//        std::thread t{ [h]() 
//            {
//                this_thread::sleep_for(3s);
//                h();
//            } 
//        };
//        t.detach();
//    }
//    string await_resume() const { return *this; }
//};
//
//future<string> foo1()
//{
//    auto str = co_await awaitable_string{ "hello" };
//    co_return str;
//}
//class awaitable_string : public std::string
//{
//public:
//    using std::string::string;
//    bool await_ready() const { return true; } 
//    void await_suspend(std::coroutine_handle<> h) const {}  
//    std::string await_resume() const { return *this; }
//}; 
//void foo1() 
//{ 
//    auto str = co_await awaitable_string{ "hello" };  
//}
/*************** co_await 运算符 ****************/






/*************** co_return 运算符 ****************/
struct my_int_return
{
    struct promise_type
    {
        int value_ = 0;
        my_int_return get_return_object() { return my_int_return{ *this }; }
        auto initial_suspend() const noexcept
        {
            return suspend_never{};
        }
        auto final_suspend() const noexcept
        {
            return suspend_never{};
        }
        auto yield_value(int& value)
        {
            value_ = value;
            return suspend_always{};
        }
        void unhandled_exception() {}
/**********************************************************/
        void return_value(int value)
        {
            value_ = value;
        }
    };

    explicit my_int_return(promise_type& p):handle_(coroutine_handle<promise_type>::from_promise(p))
    {}
    ~my_int_return()
    {
        if (handle_)
            handle_.destroy();
    }

    int get()
    {
        if (!ready_)
        {
            value_ = handle_.promise().value_;
            ready_ = true;
            if (handle_.done())
            {
                handle_.destroy();
            }
        }
        return value_;
    }
    

    coroutine_handle<promise_type> handle_;
    int value_ = 0;
    bool ready_ = false;
};

my_int_return foo()
{
    co_return 5;
}
/*************** co_return 运算符 ****************/

int main()
{
    example1();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
