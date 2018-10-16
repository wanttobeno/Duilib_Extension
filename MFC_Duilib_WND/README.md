

#### Duilib和MFC混合使用


##### duilib子窗口作为成员变量初始化

duilib 窗口最为MFC的子窗口，在OnInitDialog中初始化

```c++
m_duiControl.Subclass(GetDlgItem(IDC_DUISTATIC)->GetSafeHwnd());
```

##### 创建duilib实现
```c++
class MainWnd:public WindowImplBase
{

HWND MainWnd::Subclass(HWND hWnd)
{
	CWindowWnd::Subclass(hWnd);
	AfterSubClassWnd();
	return m_hWnd;
}
// ...
void AfterSubClassWnd()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetWindowRect(*this, &rcClient);
	HWND parentWnd=::GetParent(m_hWnd);
	POINT pt={rcClient.left,rcClient.top};
	if (parentWnd!=NULL)
	{
		::ScreenToClient(parentWnd,&pt);	
	}
	::SetWindowPos(*this, NULL, pt.x, pt.y, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);

	CDialogBuilder builder;
	CDuiString strResourcePath=m_PaintManager.GetInstancePath();
	strResourcePath+=GetSkinFolder().GetData();
	m_PaintManager.SetResourcePath(strResourcePath.GetData());

	switch(GetResourceType())
	{
	case UILIB_ZIP:
		m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
		break;
	case UILIB_ZIPRESOURCE:
		{
			HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
			if( hResource == NULL )
				return;
			DWORD dwSize = 0;
			HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
			if( hGlobal == NULL ) 
			{
#if defined(WIN32) && !defined(UNDER_CE)
				::FreeResource(hResource);
#endif
				return;
			}
			dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
			if( dwSize == 0 )
				return;
			m_lpResourceZIPBuffer = new BYTE[ dwSize ];
			if (m_lpResourceZIPBuffer != NULL)
			{
				::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
			}
#if defined(WIN32) && !defined(UNDER_CE)
			::FreeResource(hResource);
#endif
			m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
		}
		break;
	}

	CControlUI* pRoot=NULL;
	if (GetResourceType()==UILIB_RESOURCE)
	{
		STRINGorID xml(_ttoi(GetSkinFile().GetData()));
		pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
	}
	else
		pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_PaintManager);
	ASSERT(pRoot);
	if (pRoot==NULL)
	{
		MessageBox(NULL,_T("加载资源文件失败"),_T("Duilib"),MB_OK|MB_ICONERROR);
		ExitProcess(1);
		return;
	}

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	m_PaintManager.SetBackgroundTransparent(TRUE);

	InitWindow();
	return;
}

```