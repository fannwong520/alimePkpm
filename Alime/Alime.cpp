// Alime.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "base/util/string_util.h"
#include <string>
#include <assert.h>
#include <locale>
#include <iostream>
#include <clocale>
#include <cstdlib>
#include "base/thread/thread.h"
using namespace std;
#include <thread>
#include "base/thread/thread.h"
std::thread s;

class C
{
public:
	int x = 3;
	int j=100;
	void doSomething(int x, int y)
	{
		Sleep(1000);
		std::cout << std::dec<<x + y + j;
	}
	~C()
	{
		std::cout <<std::hex<<this<<std::endl;
	}
};


int main()
{
	{
		Alime::thread t([]() {
			std::cout << "hello" << std::endl;
			});
		
		t.get_id();
		auto bis=t.joinable();
		t.detach();
		bis = t.joinable();
	}
	getchar();
	{
		std::thread t([]() {
			std::cout <<"hello"<< std::endl;
			});
	}
	C* cpte = nullptr;
	{
		C c;
		std::thread t([&c]() {
			while(1)
			c.doSomething(1,2);
			});		
		t.detach();
		cpte = &c;
	}
	cpte->j = 300;
	{
		int ok = 3;
		Sleep(4000);
		getchar();
	}

	std::cout << "hello";
	//test_case
	{
		std::string u8str = u8"hello你好啊";
		std::wstring wstr = L"hello你好啊";
		auto wresult=base::UTF8ToUTF16(u8str);
		assert(wresult == wstr);
	}
	{
		std::string str = "你好";
		auto strRe = base::WStringToString(L"你好", 3);
		assert(str == strRe);
	}
	{
		std::setlocale(LC_ALL, "");
		std::string str = u8"hello你好啊";
		//若 dst 是空指针，则此函数返回假设转换则会写入 dst 的字节数
		size_t nDestSize = mbstowcs(NULL, str.data(), 0) + 1;
		wchar_t* wchDest = new wchar_t[nDestSize];
		mbstowcs(wchDest, str.data(), nDestSize);
		std::wstring wstrResult = wchDest;
		delete wchDest;
		std::wstring wstr = L"hello你好啊";
		assert(wstr == wstrResult);
		//return wstrResult;
	}
	return 1;
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
