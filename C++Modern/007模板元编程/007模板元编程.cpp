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











//////////////////////////////////////////////////////////////
// 类继承中的形参包展开
class Base1
{
public:
    Base1(){}
    Base1(const Base1&)
    {
        cout << "copy ctor Base1" << endl;
    }
};
class Base2
{
public:
    Base2() {}
    Base2(const Base2&)
    {
        cout << "copy ctor Base2" << endl;
    }
};

template<typename ...Args>
class Derived:public Args...    //继承列表中可展开类包
{
public:
    Derived(const Args&...args):Args(args).../*在构造参数列表中可做范式展开*/{}
};

void example3()
{
    Base1 b1;
    Base2 b2;
    Derived<Base1, Base2> d(b1, b2);
}
//////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////
//包含可变参模板的展开案例
//模板中class可代换成typename，这是语法中所允许的
template<template<typename...> class ...Args>    //template<typename...>只说明模板形参是一个变参模，它不能在bar中被展开。
class bar:public Args<int,double>...    //实际继承过程中template<typename...>被偏特化成了<int,double>
{
public:
    bar(const Args<int,double>& ...args):Args<int,double>(args)...{}    //构造传参的时候也将template<typename...>固定成了<int,double>
};

template<typename ...Args>
class baz3
{
public:
    baz3() {}
    baz3(const Args& ...args){}
};

template<typename ...Args>
class baz4 
{
public:
    baz4(){}
    baz4(const Args& ...args) {}
};

void example4()
{
    baz3<int, double> a1;
    baz4<int, double> a2;
    bar<baz3, baz4> b(a1, a2);
}
//////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
//利用不同层级的的不定参数模板拼包(两不定参数模板包长需一致)展开
template<typename ...> struct Tuple {};
template<typename T1,typename T2> struct Pair{};    //作为拼包中介，储存两个参数
template<typename ...Args1>
struct zip
{
    template<typename ...Args2>
    struct with
    {
        using type = Tuple<Pair<Args1, Args2>...>; //拼包
    };
};

void example5()
{
    //编译成功：
    zip<short, int>::with<unsigned short, unsigned>::type t1;  //拼成Tuple< Pair<short ,unsigned short> ,Pair<int ,unsigned> >
}
/////////////////////////////////////////////////////////////
/***************可变参数模板的概念和语法*****************/




/***************折叠表达式*****************/
/*
* 一元向左表达式   ( ... op args )折叠为((((arg0 op arg1) op arg2) op ...) op 
* 一元向右表达式   ( args op ... )折叠为(arg0 op (arg1 op ... (argN-1 op argN)))
* 二元向左表达式   ( init op ... op args )折叠为(((((init op arg0) op arg1) op arg2) op ...) op argN)
* 二元向右表达式   ( args op ... op init )折叠为(arg0 op (arg1 op ...(argN-1 op (argN op init)))
* 
* 特殊处理：
* 1. 只有&&、||和,运算符能够在空参数包的一元折叠表达式中使用。
* 2. &&的求值结果一定为true。
* 3. ||的求值结果一定为false。
* 4. ,的求值结果为void()。
* 5. 其他运算符都是非法的。
*/
///////////////////////////////////////////////////////
//以下实例为一元向有折叠
template<typename ...Args>
auto sum(Args ...args)
{
    return (args + ...);    //加法范式折叠arg0+(arg1+arg2)
}
void example6()
{
    cout << sum(1, 2.5, 11.7) << endl;
}
///////////////////////////////////////////////////




///////////////////////////////////////////////////
// 二元向左表达式
template<typename ...Args>
void print(Args ...args)
{
    (cout << ... << args) << endl;  //(((std::cout << std::string("hello ")) << "c++ ") <<"world") << std::endl;
}

void example7()
{
    print("hellow", " C++", " world!");
}
///////////////////////////////////////////////////
/***************折叠表达式*****************/






/*****************概念约束****************/
/*在模板元编程中可以通过编写概念约束来约束模板类型，
* 若概念表达式的结果（true或false）通不过约束则程序无
* 法编译通过，同时在ide中会有相关提示。
*/
///////////////////////////////////////////////
// concept 概念特性
// 约束表达式应该是一个bool类型的纯右值常量表达式，
// 当实参替换形参后，如果表达式计算结果为true，那么
// 该实参满足约束条件，概念的计算结果为true。只有当
// 概念的计算表达式为true时程序能编译通过。
template<typename T>
concept SignedIntegerType = is_integral_v<T> && is_signed_v<T>; //指定约束表达式
template<typename T>
concept IntegerFloatingType = is_integral_v<T> || is_floating_point_v<T>;   //指定约束表达式

template<SignedIntegerType T>
struct X {};

template<IntegerFloatingType T>
struct Y{};

void example8()
{
    X<int> x1;
    //X<unsigned int> x2; //不满足模板约束，编译失败

    Y<int> y1;
    Y<double> y2;
}
///////////////////////////////////////////////






///////////////////////////////////////////////
// requires 子句和约束检查顺序
// 直接在模板声明和下方定义之间或在函数定义后函数体前加入requires 概念表达式
//可以做出上面例子中 struct X 的等价效果
template<typename T>
requires is_integral_v<T>&& is_signed_v<T>
struct X1{};

//函数的情况下这样写也算对
template<typename T>
void test() requires is_integral_v<T> 
{
}
///////////////////////////////////////////////




///////////////////////////////////////////////
// concept和requires混用的情况
// 检查类型顺序：
// 1. 模板形参列表中的形参的约束表达式，其中检查顺序就是形参出现的顺序。
// 2. 模板形参列表之后的requires子句中的约束表达式。
// 3. 简写函数模板声明中每个拥有受约束auto占位符类型的形参所引入的约束表达式。
// 4. 函数模板声明尾部requires子句中的约束表达式。
template <typename T>
concept ConstType = std::is_const_v<T>; 

template <class T>
concept IntegralType = std::is_integral_v<T>; 

template <ConstType T>
requires std::is_pointer_v<T>
void foo(IntegralType auto) requires std::is_same_v<T, char* const> {}

void example9()
{
    //foo<int>(1.5);    不满足约束
    //foo<const int>(1.5);  不满足约束
    //foo<int* const>(1.5); 不满足约束
}
///////////////////////////////////////////////



///////////////////////////////////////////////
//requires 表达式
// requires 表达式根据大括号内的各个语句的断言检查情况开返回表达式结果(true 或 false)
// 
// 复合要求断言顺序：
// 1. 替换模板实参到{E}中的表达式E，检测表达式的有效性。
// 2. 如果使用了noexcept，则需要检查并确保{E}中的表达式E不会有抛出异常的可能。
// 3. 如果使用了->后的返回类型约束，则需要将模板实参替换到返回类型约束中，并且确保表达式E的结果类型，即de-cltype((E))，满足返回类型约束。

template <class T>
concept Check = requires(T a, T b) //requires表达式可加()在其中定义 require 表达式中所用到的变量列表（并不是函数形参列表）
{ 
    //复合要求
    {a.clear()} noexcept;
    {a + b} noexcept -> std::same_as<int>;  //{}表达式、noexcept、->后的返回类型约束
};
///////////////////////////////////////////////



//////////////////////////////////////////////
//约束可变参数模板
template<typename Ts> 
concept C2 = true; 

template<C2... T> 
struct s2 {};
//////////////////////////////////////////////
/*****************概念约束****************/
int main()
{
    //example1();
    //example2();
    //example3();
    //example4();
    //example5();
    //example6();
    example7();
    example8();
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
