/*
//	计时按钮	
//   20180522

xml 配置
<TimeButton name="lc_time_btn" text="计时按钮" width="200" bkcolor="0x5500ff00" sectime="60" textcolor="0xFFFFFFFF" disabledtextcolor="0xFFFFFFFF"> </TimeButton>

// 初始化方法
//	pTime->SetTimeLong(60);

// 消息响应
// 按钮点击的时候执行计时
	
*/

#pragma once

#include "..\UiLib\UIlib.h"
using namespace DuiLib;

class CTimeButtonUI :
	public CButtonUI
{
public:
	CTimeButtonUI();
	~CTimeButtonUI();
	void SetTimeLong(unsigned long nValue);
	void SetPaddingText(CDuiString str);

	void DoEvent(TEventUI& event);
	void OnTimer(int nTimerID);
	// 按钮被点击
	bool Activate();
	// 给定时器调用
	void UpShow();
	// 判断是否需要关闭计时器
	bool NeedKillTimer();
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
protected:
	static unsigned int __stdcall TimeThreadFun(PVOID pM);
private:
	CDuiString		m_textBak;
	CDuiString		m_textPadding;
	unsigned long	m_nTimeLong;
	unsigned long	m_nTimerNow;
};

