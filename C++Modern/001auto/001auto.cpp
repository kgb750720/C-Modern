﻿// 001auto.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

class Base
{
public:
	virtual void f()
	{
		cout << "Base::f()" << endl;
	}
};

class Derived :public Base
{
public:
	virtual void f() override
	{
		cout << "Derived::f()" << endl;
	}
};

int main()
{
	
	Base* d = new Derived();	//实例化的是子类对象
	d->f();			//Derived::f()
	auto b = *d;	//利用auto类型推导获取到父类类型强转引用（使得重写失效）
	b.f();			//Base::f()
	auto& c = *d;	//auto &类型推导又是获取到实际对象的类型
	c.f();			//Derived::f()
	delete d;
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
