#include"ExampleExternTemplate.h"

using namespace test;

extern template int test::foo<int>(int);	//导出函数模板实例化与其他源文件共用

template class bar<int>;			//直接使用其他原文件的类引用
template void bar<int>::foo(int);	//直接使用其他源文件的类模板引用