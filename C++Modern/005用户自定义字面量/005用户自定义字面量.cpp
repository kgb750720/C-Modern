// 005用户自定义字面量.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

/// <summary>
/// 单位标量信息类
/// </summary>
/// <typeparam name="Scale">标量相对于最细粒度计量的缩放比例</typeparam>
/// <typeparam name="Unit_char">标量所采用的字符串名称</typeparam>
template<int Scale,char ... Unit_char>
struct LengthUnit
{
    constexpr static int value = Scale;    //单位相对于最细粒度计量的缩放比例
    constexpr static char unit_str[sizeof...(Unit_char) + 1]{ Unit_char...,'\0' };
};

/// <summary>
/// 长度单位类型定义
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class LengthWithUnit
{
    unsigned long long length_unit_;    //显示单位内的实际所储存的最细粒度计量
public:
	LengthWithUnit():length_unit_(0){}
    LengthWithUnit(unsigned long long length):length_unit_(length * T::value){}

    /// <summary>
    /// 返回最细粒度计量
    /// </summary>
    /// <returns></returns>
    unsigned long long get_length() const { return length_unit_; }

    /// <summary>
    /// 获取单位类的标量名字符串
    /// </summary>
    /// <returns></returns>
    constexpr static const char* get_unit_str() { return T::unit_str; }

    /// <summary>
    /// 不同度量单位的加法运算 取精度较准确的作为返回单位
    /// </summary>
    /// <typeparam name="U"></typeparam>
    /// <param name="rhs"></param>
    /// <returns></returns>
    template<typename U>  
    LengthWithUnit<std::conditional_t<(T::value > U::value), U, T>> operator+(const LengthWithUnit<U>& rhs) 
    { 
        using unit_type = std::conditional_t<(T::value > U::value), U, T>;       
        return LengthWithUnit<unit_type>((length_unit_ + rhs.get_length()) / unit_type::value); 
    }
};

/// <summary>
/// 为LengthWithUnit类定义ostream输出流函数重载
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="out"></param>
/// <param name="unit"></param>
/// <returns></returns>
template<typename T>
ostream& operator<<(ostream& out, const LengthWithUnit<T>& unit)
{
    out << unit.get_length() / T::value << unit.get_unit_str();
    return out;
}

using MMUnit = LengthUnit<1, 'm', 'm'>;
using CMUnit = LengthUnit<10, 'c', 'm'>;
using DMUnit = LengthUnit<100, 'd', 'm'>;
using MUnit = LengthUnit<1000, 'm'>;
using KMUnit = LengthUnit<1000000,'k', 'm'>;

using LengthWithMMUnit = LengthWithUnit<MMUnit>; 
using LengthWithCMUnit = LengthWithUnit<CMUnit>; 
using LengthWithDMUnit = LengthWithUnit<DMUnit>; 
using LengthWithMUnit = LengthWithUnit<MUnit>; 
using LengthWithKMUnit = LengthWithUnit<KMUnit>;

/*
*  <operator "" >文本运算符重载（我今天才知道还有这玩意！！！！）
* 不能定义在类内
* 运算参数类型只能为unsigned long long和string这两种
*/
LengthWithMMUnit operator "" _mm(unsigned long long length)
{
    return LengthWithMMUnit(length);
}

LengthWithCMUnit operator "" _cm(unsigned long long length)
{
    return LengthWithCMUnit(length);
}

LengthWithDMUnit operator "" _dm(unsigned long long length)
{
    return LengthWithDMUnit(length);
}

LengthWithMUnit operator "" _m(unsigned long long length)
{
    return LengthWithMUnit(length);
}

LengthWithKMUnit operator "" _km(unsigned long long length)
{
    return LengthWithKMUnit(length);
}

int main()
{
    auto totalLength = LengthWithCMUnit(1) + LengthWithMUnit(2) + LengthWithMMUnit(4);
    cout << totalLength << endl;

    auto totalLength2 = 90_km + 400_m;
    cout << totalLength2 << endl;

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
