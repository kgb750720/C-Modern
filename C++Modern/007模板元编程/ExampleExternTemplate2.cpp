#include"ExampleExternTemplate.h"
using namespace test;

template int test::foo(int);	//ֱ����������Դ�ļ��ĺ���ģ��ʵ����

extern template bar<int>;					//������ģ��ʵ����������Դ�ļ�����
extern template void bar<int>::foo(int);	//�����ຯ��ģ��ʵ����������Դ�ļ�����