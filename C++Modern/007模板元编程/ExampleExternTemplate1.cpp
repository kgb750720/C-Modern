#include"ExampleExternTemplate.h"

using namespace test;

extern template int test::foo<int>(int);	//��������ģ��ʵ����������Դ�ļ�����

template class bar<int>;			//ֱ��ʹ������ԭ�ļ���������
template void bar<int>::foo(int);	//ֱ��ʹ������Դ�ļ�����ģ������