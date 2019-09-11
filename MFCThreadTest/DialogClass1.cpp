// DialogClass1.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCThreadTest.h"
#include "DialogClass1.h"
#include "afxdialogex.h"


// DialogClass1 对话框

IMPLEMENT_DYNAMIC(DialogClass1, CDialogEx)

DialogClass1::DialogClass1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

DialogClass1::~DialogClass1()
{
}

void DialogClass1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

extern HANDLE event2;
#pragma warning(disable:4996)
void DialogClass1::ThreadWork()
{
	int i = 500000;
	SetEvent(event_);
	while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &event2, FALSE, INFINITE, QS_ALLINPUT))
	{
		MSG msg;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  //取消息，检索应用程序的消息队列，PM_REMOVE取过之后从消息队列中移除
		{
			//发消息
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	//WaitForSingleObject(event2,INFINITE);
}


BEGIN_MESSAGE_MAP(DialogClass1, CDialogEx)
	ON_BN_CLICKED(IDOK, &DialogClass1::OnBnClickedOk)
END_MESSAGE_MAP()


// DialogClass1 消息处理程序


void DialogClass1::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ThreadWork();
	CDialogEx::OnOK();
}
