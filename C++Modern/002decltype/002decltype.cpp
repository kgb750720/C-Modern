// 002decltype.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

/********** typeid 关键字**************/
void sample1()
{
    //typeid可以在运行时获取类型信息
    int x1 = 0;
    double x2 = 1.1;
    //获取类型名称
    cout << typeid(x1).name() << endl;
    cout << typeid(x1 + x2).name() << endl;
    cout << typeid(int).name() << endl;

    //简化名信息(比如.N表示int .H表示double 
    cout << typeid(x1).raw_name() << endl;
    cout << typeid(x1 + x2).raw_name() << endl;
    cout << typeid(int).raw_name() << endl;
}
/********** typeid 关键字**************/

/******************** decltype 的推导规则*****************************/
void sample2()
{
    //decltype 是在编译期推导类型的，auto是运行期推导的
    //1. 如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，则decltype(e)推断出的类型是e的类型T。如果并不存在这样的类型，或者e是一组重载函数，则无法进行推导。
    //2.如果e是一个函数调用或者仿函数调用，那么de - cltype(e)推断出的类型是其返回值的类型。
    //3.如果e是一个类型为T的左值，则decltype(e)是T & 。
    //4.如果e是一个类型为T的将亡值，则decltype(e)是T && 。
    //5.除去以上情况，则decltype(e)是T。

    int i;
    int* j;
    int n[10];
    const int&& foo();
    struct A 
    {
        int operator()()
        {
            return 0;
        }
    } a;

    cout << typeid(decltype(static_cast<short>(i))).name() << endl;     //short
    cout << typeid(decltype(j)).name() << endl;                         //int *
    cout << typeid(decltype(foo)).name() << endl;                       //int const && (void)
    cout << typeid(decltype(a())).name() << endl;                       //int 
    cout << typeid(decltype(i=0)).name() << endl;                       //int 
    cout << typeid(decltype(n)).name() << endl;                         //int[10]
    cout << typeid(decltype(*j)).name() << endl;                        //int
    cout << typeid(decltype(++i)).name() << endl;                       //int
    cout << typeid(decltype("hellow world")).name() << endl;            //char const[13]
}
/******************** decltype 的推导规则*****************************/

int main()
{
    //sample1();
    sample2();
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
