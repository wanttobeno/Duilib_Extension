#pragma once

using namespace DuiLib;

class MainWnd : public WindowImplBase
{
public:
	MainWnd();
	~MainWnd();

	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual HWND Subclass(HWND hWnd);

protected:
	void AfterSubClassWnd();

private:
	DuiLib::CWebBrowserUI *m_pBrowser;
};
