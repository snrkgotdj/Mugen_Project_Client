
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

class CGameObject;

#include "MySplitterWnd.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:
	void Update();

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CGameObject* m_pGameObject;

	CMySplitterWnd m_wndMainSplit;
	CMySplitterWnd m_wndSubSplit;

	HWND GetChildHwnd() { return m_wndSubSplit.GetPane(1, 0)->m_hWnd; }
	CWnd* GetComponentView() { return m_wndMainSplit.GetPane(0, 2); }
	CWnd* GetHierachyView() { return m_wndMainSplit.GetPane(0, 1); }
	CWnd* GetDebugView() { return m_wndSubSplit.GetPane(2, 0); }
	CWnd* GetGameView() {return  m_wndSubSplit.GetPane(1, 0); }
	CWnd* GetButtonView() { return m_wndSubSplit.GetPane(0, 0); }

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnAddgameobject();
	afx_msg void OnSceneSave();
	afx_msg void OnSceneLoad();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLayerAddlayer();
	afx_msg void OnSavegameobject();
	afx_msg void OnGLoadgameobject();
	afx_msg void OnLayerCheckcollayer();
};


