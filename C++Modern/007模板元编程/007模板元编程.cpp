// 007模板元编程.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<vector>
#include<numeric>
using namespace std;

/***************可变参数模板的概念和语法*****************/
/*具体语法：
* 1.    typename ...Args  模板形参包
* 2.    Args ...args      函数形参包
* 3.    ...               形参包展开(可使自定范式展开，如下面例子：baz(&arg, arg)... 所示)
* 
* 允许形参包展开(...)的场景包括一下几种。
* 1. 表达式列表。
* 2. 初始化列表。{,,,}
* 3. 基类描述。
* 4. 成员初始化列表。
* 5. 函数参数列表。
* 6. 模板参数列表。
* 7. 动态异常列表（C++17已经不再使用）。
* 8. lambda表达式捕获列表。
* 9. sizeof...()运算符。
* 10. 对其运算符。
* 11. 属性列表。
*/





///////////////////////////////////////////////////////////////
//每个不定形参按指定模式解包案例1
template<typename T, typename U>
T baz1(T t, U u)
{
    cout << t << ":" << u << endl;
    return t;
}

/// <summary>
/// 无任何具体功能，仅用作辅助函数形参包的展开
/// </summary>
/// <typeparam name="...Args"></typeparam>
/// <param name="...args"></param>
template<typename ...Args>
void foo1(Args ...args) {}

/// <summary>
/// bar1仿函数
/// </summary>
/// <typeparam name="...Args"></typeparam>
template<typename ...Args>
struct bar1
{
    bar1(Args ...args)
    {
        //模板实例化：baz(<具体的arg的地址>,<具体的arg的值>>)
        foo1(baz1(&args, args)...);    //利用foo1的不定形参列表作为辅助将函数形参包中每个函数形参做<baz1(&args,args)>的范式展开
    }
    /*以上代码相当于：
    * bar(int a1, double a2, unsigned int a3) 
    * { 
    *       foo1(baz1(&a1, a1), baz1(&a2, a2), baz1(&a3, a3)); 
    * }
    */
};
void example1()
{
    cout << "\n每个不定形参按指定模式解包:" << endl;
    bar1<int, double, unsigned int> b(1, 5.0, 8);
}
///////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////
//每个不定形参按指定模式解包案例2
template<typename T, typename ...Args>
struct TypeCampare
{
    using T2 = typename TypeCampare<Args...>::type;
    using type = conditional_t<(numeric_limits<T>::has_denorm > numeric_limits<T2>::has_denorm), T, T2>;
};

template<typename T>
struct TypeCampare<T>
{
    using type = T;
};


template<typename ...Args>
auto baz2(Args... args)->TypeCampare<Args...>::type
{
    using Res_t = TypeCampare<Args...>::type;
    cout << "baz2()函数中表达式args...的展开过程:" << endl;
    vector<Res_t> temp = { (cout << "args:" << args << endl ,static_cast<Res_t>(args))... };
    return accumulate(temp.begin(), temp.end(),0);
}



template<typename ...Args>
void foo2(Args ...args)
{
    cout << "foo2()函数中args...的展开过程:" << endl;
    int temp[] = { (cout << "bar2下baz2(args...)+arg的展开结果：" << args << endl,0)...};
}

/// <summary>
/// bar2仿函数
/// </summary>
/// <typeparam name="...Args"></typeparam>
template<typename ...Args>
struct bar2
{
    bar2(Args ...args)
    {
        cout << "表达式(baz2(args...) + args)...中baz2(args...) + args)的展开过程：" << endl;
        foo2((baz2(args...) + args)...);  //baz2(args...) + args)...中的args...表示为引用一整个形参包
    }
};
void example2()
{
    cout << "\nexample2:" << endl;
    bar2<int, double, unsigned int>(1, 5.0, 8);
}
///////////////////////////////////////////////////////////////
/***************可变参数模板的概念和语法*****************/
int main()
{
    example1();
    example2();
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
