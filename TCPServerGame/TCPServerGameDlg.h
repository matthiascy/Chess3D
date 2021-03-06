
// TCPServerGameDlg.h : 头文件
//

#pragma once
#include "chess_network.h"

// CTCPServerGameDlg 对话框
class CTCPServerGameDlg : public CDialog
{
// 构造
public:
	CTCPServerGameDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPSERVERGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);  //消息处理
	DECLARE_MESSAGE_MAP()

public:
	SOCKET sockSvr;	
	SOCKET sockCnnn;//仅仅用于连接
	sockaddr_in locAddr, fromAddr;//本地地址 和 客户端地址
	int cnnNum;     // 全局变量，连接用户数目

	CListBox  m_serverList;   //控件变量
///////////////////////////////////////////////函数FUNCTION/////////////////////////////
	//装载用户数据字典
	bool LoadPwdDict(const char* pszFileName);
	//追加用户数据字典
	bool AddPwdDict(const char* pszFileName,const char* pszName,const char* pszPwd);
	//根据用户名和密码判断是否合法
	bool IsValidUser(const char* pszName, const char* pszPwd);
	//在用户连接表中添加用户
	void AddUser(const char* pszName);
	//在用户连接表中删除用户
	void DeleteUser(const char* pszName);
	//消息处理
	void messageProcessing(const char* recvBuff,SOCKET s);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
