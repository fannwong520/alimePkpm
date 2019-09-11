// AlimeTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "windows.h"
#include <process.h>
#include "Runnable.h"

template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))


#include <thread>
#include "stdThread.h"
class task :public Runnable
{
	virtual void run()
	{
		while(1)
			std::cout << "to do something";
	}
};
class task2 :public Runnable
{
	virtual void run()
	{
		while (1)
			std::cout << "fuck";
	}
};

#include <thread>


class A
{
public:
	int z= 3;
	void print(int x, int y)
	{
		std::cout << x + y + z << std::endl;
	}
};
int main()
{
	{
		A *a = new A();
		std::thread t([a]() {
			Sleep(100);
			a->print(3, 4);
			});
		delete a;
		a = nullptr;
		Sleep(2000);
		t.detach();

	}
	{
		//std::thread t([]() {
		//	while (1)
		//		std::cout << "helloWorld";
		//});
		//t.detach();
	}
	std::cout << "Hello World!\n";
	{
		//Runnable* ta = new task();
		//alime::StdThread tr(ta);
		//tr.detach();
	}
	std::cout << "Hello World!\n";
	{
		int A_array[64];
		constexpr size_t size=arraysize(A_array);
		getchar();
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
