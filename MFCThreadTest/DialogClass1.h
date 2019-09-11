#pragma once


// DialogClass1 对话框

class DialogClass1 : public CDialogEx
{
	DECLARE_DYNAMIC(DialogClass1)

public:
	DialogClass1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DialogClass1();
	void ThreadWork();
	void AddEvent(void* e)
	{
		event_ = e;
	}
	HANDLE event_;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
