
// TCPServerGame.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTCPServerGameApp:
// �йش����ʵ�֣������ TCPServerGame.cpp
//

class CTCPServerGameApp : public CWinAppEx
{
public:
	CTCPServerGameApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTCPServerGameApp theApp;