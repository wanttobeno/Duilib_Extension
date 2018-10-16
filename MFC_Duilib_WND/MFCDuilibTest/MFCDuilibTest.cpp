#include "stdafx.h"
#include "MFCDuilibTest.h"
#include "MFCDuilibTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMFCDuilibTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMFCDuilibTestApp::CMFCDuilibTestApp()
{

}

CMFCDuilibTestApp theApp;


BOOL CMFCDuilibTestApp::InitInstance()
{
	::CoInitialize(NULL);
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CMFCDuilibTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}

	return FALSE;
}

int CMFCDuilibTestApp::ExitInstance()
{
	CoUninitialize();
	return CWinAppEx::ExitInstance();
}
