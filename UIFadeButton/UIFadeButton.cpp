#include "UIFadeButton.h"

#include <windows.h>

namespace DuiLib {

	CFadeButtonUI::CFadeButtonUI() : 
		CUIAnimation( this ), 
		m_bMouseHove( FALSE ), 
		m_bMouseLeave( FALSE ),
		m_hasFadeEffect( TRUE )
	{
	}

	CFadeButtonUI::~CFadeButtonUI()
	{
		StopAnimation();
		CControlUI::~CControlUI();
	}

	LPCTSTR CFadeButtonUI::GetClass() const
	{
		return _T("FadeButtonUI");
	}

	LPVOID CFadeButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("FadeButtonUI")) == 0 ) 
			return static_cast<CFadeButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void CFadeButtonUI::DoEvent(TEventUI& event)
	{
		if (!m_hasFadeEffect)
		{
			CButtonUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_MOUSEENTER && !IsAnimationRunning( FADE_IN_ID ) )
		{
			m_bFadeAlpha = 0;
			m_bMouseHove = TRUE;
			StopAnimation( FADE_OUT_ID );
			StartAnimation( FADE_ELLAPSE, FADE_FRAME_COUNT, FADE_IN_ID );
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE && !IsAnimationRunning( FADE_OUT_ID ) )
		{
			m_bFadeAlpha = 0;
			m_bMouseLeave = TRUE;
			StopAnimation(FADE_IN_ID);
			StartAnimation(FADE_ELLAPSE, FADE_FRAME_COUNT, FADE_OUT_ID);
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_TIMER ) 
		{
			OnTimer(  event.wParam );
		}
		CButtonUI::DoEvent( event );
	}

	void CFadeButtonUI::SetFadeEffect(BOOL hasFadeEffect)
	{
		m_hasFadeEffect = hasFadeEffect;
	}

	void CFadeButtonUI::OnTimer( int nTimerID )
	{
		OnAnimationElapse( nTimerID );
	}

	void CFadeButtonUI::PaintStatusImage(HDC hDC)
	{
		if (!m_hasFadeEffect)
		{
			CButtonUI::PaintStatusImage(hDC);
			return;
		}

		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		// duilib 版本不一样
		CDuiString m_sDisabledImage = m_diDisabled.sDrawString;
		CDuiString m_sPushedImage = m_diPushed.sDrawString;
		CDuiString m_sFocusedImage = m_diFocused.sDrawString;
		CDuiString m_sNormalImage = m_diNormal.sDrawString;
		CDuiString m_sHotImage = m_diHot.sDrawString;


		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if (!DrawImage(hDC, m_diDisabled)) m_sDisabledImage.Empty();
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedImage.IsEmpty() ) {
				if (!DrawImage(hDC, m_diPushed)) m_sPushedImage.Empty();
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if (!DrawImage(hDC, m_diPushed)) m_sFocusedImage.Empty();
				else return;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( IsAnimationRunning(FADE_IN_ID) || IsAnimationRunning(FADE_OUT_ID))
			{
				if( m_bMouseHove )
				{
					m_bMouseHove = FALSE;
					m_sLastImage = m_sHotImage;
					if (!DrawImage(hDC, m_diNormal))
						m_sNormalImage.Empty();
					return;
				}

				if( m_bMouseLeave )
				{
					m_bMouseLeave = FALSE;
					m_sLastImage = m_sNormalImage;
					if (!DrawImage(hDC, m_diNormal))
						m_sHotImage.Empty();
					return;
				}

				m_sOldImage = m_sNormalImage;
				m_sNewImage = m_sHotImage;
				if( IsAnimationRunning(FADE_OUT_ID) )
				{
					m_sOldImage = m_sHotImage;
					m_sNewImage = m_sNormalImage;
				}


				// 这里实现核心的渐变值
				m_sOldImage = UpdateFadeAttribute(m_sOldImage, 255 - m_bFadeAlpha);
				m_sNewImage = UpdateFadeAttribute(m_sNewImage, m_bFadeAlpha);


				// 兼容官方duilib处理
				if (m_sOldImage.Find(m_sDisabledImage))
				{
					m_diDisabled.uFade = 255 - m_bFadeAlpha;

					if (!DrawImage(hDC, m_diDisabled))
						m_sOldImage.Empty();
				}
				if (m_sOldImage.Find(m_sPushedImage))
				{
					m_diPushed.uFade = 255 - m_bFadeAlpha;
					if (!DrawImage(hDC, m_diPushed))
						m_sOldImage.Empty();
				}
				if (m_sOldImage.Find(m_sFocusedImage))
				{
					m_diFocused.uFade = 255 - m_bFadeAlpha;
					if (!DrawImage(hDC, m_diFocused))
						m_sOldImage.Empty();
				}
				if (m_sOldImage.Find(m_sNormalImage))
				{
					m_diNormal.uFade = 255 - m_bFadeAlpha;
					if (!DrawImage(hDC, m_diNormal))
						m_sOldImage.Empty();
				}
				if (m_sOldImage.Find(m_sHotImage))
				{
					m_diHot.uFade = 255 - m_bFadeAlpha;
					if (!DrawImage(hDC, m_diHot))
						m_sOldImage.Empty();
				}


				// 兼容官方duilib处理
				if (m_sNewImage.Find(m_sDisabledImage))
				{
					m_diDisabled.uFade = m_bFadeAlpha;
					if (!DrawImage(hDC, m_diDisabled))
						m_sNewImage.Empty();
				}
				if (m_sNewImage.Find(m_sPushedImage))
				{
					m_diPushed.uFade = m_bFadeAlpha;
					if (!DrawImage(hDC, m_diPushed))
						m_sNewImage.Empty();
				}
				if (m_sNewImage.Find(m_sFocusedImage))
				{
					m_diFocused.uFade = m_bFadeAlpha;
					if (!DrawImage(hDC, m_diFocused))
						m_sNewImage.Empty();
				}
				if (m_sNewImage.Find(m_sNormalImage))
				{
					m_diNormal.uFade = m_bFadeAlpha;
					if (!DrawImage(hDC, m_diNormal))
						m_sNewImage.Empty();
				}
				if (m_sNewImage.Find(m_sHotImage))
				{
					m_diHot.uFade = m_bFadeAlpha;
					if (!DrawImage(hDC, m_diHot))
						m_sNewImage.Empty();
				}

				// 原函数
				//if( !DrawImage(hDC, (LPCTSTR)m_sOldImage, NULL) ) 
				//	m_sOldImage.Empty();
				//if( !DrawImage(hDC, (LPCTSTR)m_sNewImage, NULL) ) 
				//	m_sNewImage.Empty();
				return;
			}
			else
			{
				if (m_sLastImage.IsEmpty())
				{
					m_sLastImage = m_sNormalImage;
				}

				if (!DrawImage(hDC, m_diNormal))
					m_sLastImage.Empty();
				return;
			}
		}
	}

	void CFadeButtonUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		m_bFadeAlpha = (nCurFrame / (double)nTotalFrame) * 255;
		if(m_bFadeAlpha == 0) 
			m_bFadeAlpha = 10;
		Invalidate();
	}

	CDuiString CFadeButtonUI::UpdateFadeAttribute(const CDuiString& imageAttribute, int fadeAlpha)
	{
		CDuiString updateImageAttribute = imageAttribute;
		if (updateImageAttribute.Find(L"file='") == -1)
		{
			updateImageAttribute = L"file='";
			updateImageAttribute += imageAttribute;
			updateImageAttribute += L"' ";
		}

		if (int startFadePos = imageAttribute.Find(L"fade='") != -1)
		{
			int endFadePos = imageAttribute.Find(L"'", startFadePos + 6);
			ASSERT(endFadePos == -1);
			updateImageAttribute = imageAttribute.Mid(startFadePos, endFadePos - startFadePos);
		}

		updateImageAttribute += L" fade='%d' ";
		updateImageAttribute.Format(updateImageAttribute.GetData(), fadeAlpha);

		return updateImageAttribute;
	}

} // namespace DuiLib