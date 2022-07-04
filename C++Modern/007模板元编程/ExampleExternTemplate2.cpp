#include"ExampleExternTemplate.h"
using namespace test;

template int test::foo(int);	//直接引用其他源文件的函数模板实例化

extern template bar<int>;					//导出类模板实例化与其他源文件共用
extern template void bar<int>::foo(int);	//导出类函数模板实例化与其他源文件共用