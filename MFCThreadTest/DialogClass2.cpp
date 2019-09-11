// DialogClass2.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCThreadTest.h"
#include "DialogClass2.h"
#include "afxdialogex.h"


// DialogClass2 对话框

IMPLEMENT_DYNAMIC(DialogClass2, CDialogEx)

DialogClass2::DialogClass2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

DialogClass2::~DialogClass2()
{
}

void DialogClass2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogClass2, CDialogEx)
END_MESSAGE_MAP()


// DialogClass2 消息处理程序
