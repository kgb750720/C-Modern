#pragma once
namespace test
{
	template<typename T>
	auto foo(T t)->T
	{
		return t;
	}

	template<typename T>
	class bar
	{
	public:
		void foo(T t){}
	};
}