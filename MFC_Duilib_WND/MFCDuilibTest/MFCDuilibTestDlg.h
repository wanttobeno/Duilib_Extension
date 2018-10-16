#pragma once

#include "MainWnd.h"

class CMFCDuilibTestDlg : public CDialog
{
public:
	CMFCDuilibTestDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_MFCDUILIBTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	HICON m_hIcon;
	MainWnd m_duiControl;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
