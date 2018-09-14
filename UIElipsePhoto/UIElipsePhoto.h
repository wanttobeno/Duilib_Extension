#pragma once
#include "..\UiLib\UIlib.h"
using namespace DuiLib;
#include <gdiplus.h>
class CElipsePhotoUI :
	public CControlUI
{
public:
	CElipsePhotoUI();
	virtual ~CElipsePhotoUI();

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetBkImage(LPCTSTR pStrImage);

	virtual void PaintBkImage(HDC hDC);
private:
	Gdiplus::Bitmap*		m_bkGDIPimage;
};

