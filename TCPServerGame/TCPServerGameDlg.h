
// TCPServerGameDlg.h : ͷ�ļ�
//

#pragma once
#include "chess_network.h"

// CTCPServerGameDlg �Ի���
class CTCPServerGameDlg : public CDialog
{
// ����
public:
	CTCPServerGameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCPSERVERGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);  //��Ϣ����
	DECLARE_MESSAGE_MAP()

public:
	SOCKET sockSvr;	
	SOCKET sockCnnn;//������������
	sockaddr_in locAddr, fromAddr;//���ص�ַ �� �ͻ��˵�ַ
	int cnnNum;     // ȫ�ֱ����������û���Ŀ

	CListBox  m_serverList;   //�ؼ�����
///////////////////////////////////////////////����FUNCTION/////////////////////////////
	//װ���û������ֵ�
	bool LoadPwdDict(const char* pszFileName);
	//׷���û������ֵ�
	bool addUserDict(const char* pszFileName,const char* pszName,const char* pszPwd);
	//�����û����������ж��Ƿ�Ϸ�
	bool isValidUser(const char* pszName, const char* pszPwd);
	//���û����ӱ�������û�
	void addUser(const char* pszName, const char* pwd);
	//���û����ӱ���ɾ���û�
	void deleteUser(const char* pszName);
	//��Ϣ����
	void messageProcessing(const char* recvBuff,SOCKET s);
    void addClient(const char* name, SOCKET S);
    void sendPacket(const char* name, MSGTYPE msgType,
                    char* msg, char state);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
