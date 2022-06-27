// 004结构化绑定.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

/*结构化绑定的3中类型
* 
* 原生数组、结构体和类对象、元组和类元组的对象
* 
*/

/*****************绑定到原生数组**********************/
//求别名的数量与数组元素的个数一致，否则可能会使数组退化为指针。
void sample1()
{
    int a[3]{ 1,3,5 };
    auto [x, y, z] = a;
    cout << "[x,y,z]=[" << x << "," << y << "," << z  << "]" << endl;
}
/*****************绑定到原生数组**********************/





/************绑定到结构体和类对象***************/
class BindTest
{
public:
    //必须都是公有成员才能获取到
    int a = 42;
    double b = 11.7;
};
class BindTest2
{
    int a = 42;
public:
    double b = 11.7;
};
void sample2()
{
    BindTest bt;
    auto [x, y] = bt;
    cout << "x:" << x << endl;
    cout << "y:" << y << endl;

    BindTest2 bt2;
    //auto [x2, y2] = bt2;  x2无法获取私有成员变量a
}
/************绑定到结构体和类对象***************/




/************绑定到元组和类元组对象*************/
void sample3()
{
    auto [tupleX, tuplueY] = make_tuple<int, double>(8, 2.5);
    cout << "[tupleX, tuplueY] = [" << tupleX << "," << tuplueY << "]" << endl;
    auto [pairFirst, pairSecond] = make_pair<int, string>(8, "zzz");
    cout << "[pairFirst, pairSecond] = [" << pairFirst << "," << pairSecond << "]" << endl;
}
/************绑定到元组和类元组对象*************/




/*************实现自定义结构化绑定类型****************/
//可自定义结构化绑定的值映射对应，get<idx>()所获取的对应类型值

/////////////////////////////////////////////////////////////////////////
//实现方案1(类外实现)：
//定义BindTest3类型
class BindBase3
{
public:
    int a = 42;
};
class BindTest3 :public BindBase3
{
public:
    double b = 11.7;
};
namespace std
{
    //为标准库添加对于BindTest3的所绑定的子对象和元素个数
    template<>
    struct tuple_size<BindTest3>
    {
        static constexpr size_t value = 2;
    };

    //为BindTest3明确每个子对象的元素类型
    template<>
    struct tuple_element<0,BindTest3>
    {
        using type = int;
    };
    template<>
    struct tuple_element<1, BindTest3>
    {
        using type = double;
    };

    //定义get<>()函数对于BindTest3不同下标所取得的值
    //防止编译器生成偏特化版本外的get函数，防止get在BindTest3上被滥用
    template<size_t Idx>
    auto& get(BindTest3& bt) = delete;

    //偏特化<0>的情况
    template<>
    auto& get<0>(BindTest3& bt)
    {
        return bt.a;
    }

    //偏特化<1>的情况
    template<>
    auto& get<1>(BindTest3& bt)
    {
        return bt.b;
    }
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//实现方案2（类内实现）:
class BindTest4 :public BindBase3
{
public:
    double b = 11.7;

    //定义get<>()函数对于BindTest4不同下标所取得的值
    //防止编译器生成偏特化版本外的get函数，防止get在BindTest4上被滥用
    template<size_t Idx>
    auto& get() = delete;
};
namespace std
{
    //为标准库添加对于BindTest4的所绑定的子对象和元素个数
    template<>
    struct tuple_size<BindTest4>
    {
        static constexpr size_t value = 2;
    };

    //为BindTest4明确每个子对象的元素类型
    template<>
    struct tuple_element<0, BindTest4>
    {
        using type = int;
    };
    template<>
    struct tuple_element<1, BindTest4>
    {
        using type = double;
    };
}
//偏特化<0>的情况
template<>
auto& BindTest4::get<0>() { return a; }
//偏特化<1>的情况
template<>
auto& BindTest4::get<1>(){return b;}
//////////////////////////////////////////////////////////////////////////
void sample4()
{
    //方案1
    BindTest3 bt3;
    auto& [x3, y3] = bt3;
    x3 = 77;
    cout << "bt3.a=" << bt3.a << endl;

    //方案2
    BindTest4 bt4;
    auto& [x4, y4] = bt4;
    x4 = 77;
    cout << "bt4.a=" << bt4.a << endl;
}
/*************实现自定义结构化绑定类型****************/

int main()
{
    //sample1();
    //sample2();
    //sample3();
    sample4();
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
