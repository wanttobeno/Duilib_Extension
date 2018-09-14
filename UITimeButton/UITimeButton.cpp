#include "UITimeButton.h"
#define TIME_BUTTON_ID 1000

CTimeButtonUI::CTimeButtonUI()
{
	m_textBak = _T("");
	m_textPadding = _T("S");
	m_nTimeLong = 0;
	m_nTimerNow = 0;
}

CTimeButtonUI::~CTimeButtonUI()
{

}

void CTimeButtonUI::SetTimeLong(unsigned long nValue)
{
	if (nValue <= 0)
		return;
	m_nTimeLong = nValue;
	m_textBak = this->GetText();
}

void CTimeButtonUI::SetPaddingText(CDuiString str)
{
	m_textPadding = str;
}

void CTimeButtonUI::UpShow()
{
	m_nTimerNow--;
	if (m_nTimerNow <= 0)
	{
		this->SetEnabled(true);
		this->SetText(m_textBak);
	}
	else
	{
		TCHAR buf[64] = { 0 };
		_itot(m_nTimerNow, buf, 10);
		_tcscat(buf, m_textPadding.GetData());
		this->SetText(buf);
		this->SetEnabled(false);
	}
}

bool CTimeButtonUI::NeedKillTimer()
{
	if (m_nTimerNow <= 0 || m_nTimerNow > m_nTimeLong)
	{
		return true;
	}
	return false;
}

void CTimeButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	__super::SetAttribute(pstrName, pstrValue);

	if (_tcsicmp(pstrName, _T("sectime")) == 0)
	{
		unsigned long nValue = _tstoi(pstrValue);
		m_nTimeLong = nValue;
		
	}
}

void CTimeButtonUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
	{
		OnTimer(event.wParam);
	}
	__super::DoEvent(event);
}

void CTimeButtonUI::OnTimer(int nTimerID)
{
	if (nTimerID == TIME_BUTTON_ID)
	{
		if (NeedKillTimer())
		{
			this->GetManager()->KillTimer(this, TIME_BUTTON_ID);
		}
		else
		{
			UpShow();
		}
	}
	__super::Activate();
}

bool CTimeButtonUI::Activate()
{
	if (m_nTimeLong > 0)
	{
		m_textBak = this->GetText();
		m_nTimerNow = m_nTimeLong;
		// ±ÜÃâ1Ãëºó²ÅÏÔÊ¾
		m_nTimerNow++;
		UpShow();
		this->GetManager()->SetTimer(this, TIME_BUTTON_ID, 1000);
	}
	bool bRet = __super::Activate();
	return bRet;
}