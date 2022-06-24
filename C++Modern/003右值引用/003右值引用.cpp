// 003右值引用.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include<vector>
using namespace std;

/*****************右值引用测试****************/
class X
{
    bool isCopy;
public:
    X()
    {
        cout << "X ctor" << endl;
        isCopy = false;
        i = 0;
    }
    X(X const& x)
    {
        cout << "X copy ctor" << endl;
        isCopy = true;
        i = 0;
    }
    ~X()
    {
        cout << (isCopy ? "X copy dtor" : "X dtor") << endl;
    }
    int i;
};


X make_x1()
{
    return X();
}

X make_x2()
{
    X x;
    return x;
}

void sample1()
{
    /*左值和右值的定义
    * 表达式中能用&（取地址符）取得地址的为左值，不能取得地址的为右值
    * 右值的生命周期只有一瞬间，除非是被特定类型“获取”的用两种手段延长（临时量实质化和两种类型）
    */

    //临时量实质化手段
    int i = X().i;  //临时量实质化 S()作为纯右值转化为将亡值后访问变量i

    //能够获取右值延长右值的生命周期的两种类型手段
    int const& i1 = 0;  //<type> const& 常引用类型
    int&& i2 = 0;       //<type>&&      右值引用类型

    //右值引用无法获得左值
    //int&& i3 = i; 报错：无法将右值绑定到左值


    //麻了，C++ 11 以后RVO返回值优化被强制启用还关不了，实验结果和理论结果不一致


    //直接在返回语句中创建对象
    {
        cout << "x1 = make_x1()" << endl;
        X x1 = make_x1();           //构造1次
    }

    //返回语句中返回函数作用域内的返回值同类型对象
    {
        cout << "\nx2 = make_x2()" << endl;
        X x2 = make_x2();           //构造1次、复制构造1次
    }

    //函数产生的返回值为右值，无法用一般引用变量(左值引用)去接
    //X& x = make_x1();
    //X& x = make_x2();

    {
        cout << "\nx3 = make_x2()" << endl;
        X const& x3 = make_x2();    //构造1次、复制构造1次
    }

    {
        cout << "\nx4 = make_x2()" << endl;
        X&& x4 = make_x2();
    }

}
/*****************右值引用测试****************/

/***************************函数形参左值右值测试******************************/

//无论一个函数的实参是左值还是右值，其形参都是一个左值，即使这个形参看上去是一个右值引用
X forwaord_x(X&& x)
{
    return x;
}

void sample2()
{

    X xTemp;
    {
        //X&& x = forwaord_x(xTemp); //无法将右值转换为左值
        cout << "\nforwaord_x( X() )" << endl;
        X&& x1 = forwaord_x(X());    //依旧构造了3次,即使forwaord_x的形参是右值引用
    }

    {
        cout << "\nforword_x( static_cast<X&&>(xTemp) )" << endl;
        X&& x = forwaord_x(static_cast<X&&>(xTemp));
    }

    {
        cout << "\nforword_x( std::move(xTemp) )" << endl;
        X&& x = forwaord_x(move(xTemp));    //move()等效于static_cast<Type&&>()
    }

    
}
/***************************函数形参左值右值测试******************************/


/********************完美转发***********************/
/*
* 右值引用模板类型在递归推导时会折叠退化成左值引用，
* 为了确保推导正确应在模板中间层使用forward<T>来根据
* 本层实际推导类型的左右值属性来将值转换成准确的左值
* 或者右值。
*/

template<typename T>
void show_type(T t)
{
    cout << typeid(t).name() << endl;
}

template<typename T>
void perfect_forword_x1(T&& x)
{
    cout << "show_type(static_cast<T&&>(x))" << endl;
    show_type(static_cast<T&&>(x));
}

template<typename T>
void perfect_forword_x2(T&& x)
{
    cout << "show_type(forward<T>(x))" << endl;
    show_type(forward<T>(x));   //std::forward<T>(x) 等效于 static_cast<T&&>(x)
}

void sample3()
{
    cout << "\nperfect_forword_x1( X() )" << endl;
    perfect_forword_x1( X() );

    cout << "\nperfect_forword_x2( X() )" << endl;
    perfect_forword_x2( X() );

    cout << "\nperfect_forword_x2( x )" << endl;
    X x;
    perfect_forword_x2(x);
}

/********************完美转发***********************/

int main()
{
    //sample1();
    //sample2();
    sample3();
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
