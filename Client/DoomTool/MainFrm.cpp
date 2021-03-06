
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "DoomTool.h"

#include "MainFrm.h"
#include "GameView.h"
#include "HierachyView.h"
#include "ComponentView.h"
#include "DebugView.h"
#include "ButtonView.h"

#include "ButtonView.h"
#include "DebugView.h"
#include "ResManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WORK_SPACE_X 1800
#define WORK_SPACE_Y 800

#define GAME_VIEW_X 1200
#define GAME_VIEW_Y 600

#define BUTTON_VIEW_X GAME_VIEW_X
#define BUTTON_VIEW_Y 30

#define DEBUG_VIEW_X GAME_VIEW_X
#define DEBUG_VIEW_Y WORK_SPACE_Y - GAME_VIEW_Y - BUTTON_VIEW_Y

#define HIERACH_VIEW_X 220
#define HIERACH_VIEW_Y WORK_SPACE_Y

#define COMPONENT_VIEW_X WORK_SPACE_X - GAME_VIEW_X - HIERACH_VIEW_X
#define COMPONENT_VIEW_Y WORK_SPACE_Y

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_GAMEOBJECT_ADDGAMEOBJECT, &CMainFrame::OnAddgameobject)
	ON_COMMAND(ID_SCENE_SAVE, &CMainFrame::OnSceneSave)
	ON_COMMAND(ID_SCENE_LOAD, &CMainFrame::OnSceneLoad)
	ON_COMMAND(ID_LAYER_ADDLAYER, &CMainFrame::OnLayerAddlayer)
	ON_COMMAND(ID_GAMEOBJECT_SAVEGAMEOBJECT, &CMainFrame::OnSavegameobject)
	ON_COMMAND(ID_GAMEOBJECT_LOADGAMEOBJECT, &CMainFrame::OnGLoadgameobject)
	ON_COMMAND(ID_LAYER_CHECKCOLLAYER, &CMainFrame::OnLayerCheckcollayer)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
	:m_pGameObject(NULL)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

void CMainFrame::Update()
{
	CComponentView* pComView = (CComponentView*)GetComponentView();
	CDebugView* pDebugView = (CDebugView*)GetDebugView();
	CButtonView* pButtonView = (CButtonView*)GetButtonView();

	pComView->Update();
	pDebugView->Update();
	pButtonView->update();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndMainSplit.SetBorderSize(1, 1, 0, 0);
	m_wndSubSplit.SetBorderSize(1, 1, 0, 0);

	m_wndMainSplit.CreateStatic(this, 1, 3);
	m_wndMainSplit.SetColumnInfo(0, GAME_VIEW_X - 4, GAME_VIEW_X - 4);

	m_wndSubSplit.CreateStatic(&m_wndMainSplit, 3, 1, WS_CHILD | WS_VISIBLE, m_wndMainSplit.IdFromRowCol(0, 0));

	m_wndMainSplit.CreateView(0, 1, RUNTIME_CLASS(CHierachyView), CSize(HIERACH_VIEW_X, HIERACH_VIEW_Y), NULL);
	m_wndMainSplit.CreateView(0, 2, RUNTIME_CLASS(CComponentView), CSize(COMPONENT_VIEW_X, COMPONENT_VIEW_Y), NULL);

	m_wndSubSplit.CreateView(0, 0, RUNTIME_CLASS(CButtonView), CSize(BUTTON_VIEW_X, BUTTON_VIEW_Y), NULL);
	m_wndSubSplit.CreateView(1, 0, RUNTIME_CLASS(CGameView), CSize(GAME_VIEW_X, GAME_VIEW_Y), NULL);
	m_wndSubSplit.CreateView(2, 0, RUNTIME_CLASS(CDebugView), CSize(DEBUG_VIEW_X, DEBUG_VIEW_Y), NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	cs.dwExStyle &= WS_EX_CLIENTEDGE;

	RECT rt = { 0, 0, WORK_SPACE_X, WORK_SPACE_Y };
	AdjustWindowRect(&rt, cs.style, true);

	cs.cx = rt.right - rt.left + 2;
	cs.cy = rt.bottom - rt.top + 2;

	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.

}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

#include "Scene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "GameObject.h"
#include "TransForm.h"

void CMainFrame::OnAddgameobject()
{
	CGameObject* pObj = new CGameObject;
	pObj->SetTagName(L"EmptyObject");
	pObj->AddComponent(new CTransform);

	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	CLayer* pLayer = pCurScene->FindLayer(L"Default");
	pLayer->AddGameObject(pObj);
	
	CComponentView* pView = (CComponentView*)GetComponentView();
	CDebugView* pDebugView = (CDebugView*)GetDebugView();
	pView->InitTarget();
	pView->SetTargetObj(pObj);
	pDebugView->SetTargetObj(pObj);
	m_pGameObject = pObj;
}

#include "SaveLoadManager.h"
#include "PathManager.h"

void CMainFrame::OnSceneSave()
{
	wchar_t szFilter[] = L"Scene File (*.scene) |*.scene;|";

	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);
	CString strInitPath = CPathManager::GetResPathEX();
	strInitPath += L"Save\\Scene";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		CSaveLoadManager::Manager().SaveScene(strPathName.GetBuffer());
	}
}


void CMainFrame::OnSceneLoad()
{
	wchar_t szFilter[] = L"Scene File (*.scene) |*.scene;|";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY, szFilter);
	CString strInitPath = CPathManager::GetResPathEX();
	strInitPath += L"Save\\Scene";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	if (IDOK == dlg.DoModal())
	{
		((CHierachyView*)GetHierachyView())->UpdateObject();

		CString strPathName = dlg.GetPathName();
		CSaveLoadManager::Manager().LoadScene(strPathName.GetBuffer());
		((CComponentView*)GetComponentView())->InitTarget();
		((CDebugView*)GetDebugView())->InitTarget();
		CResManager::Manager().SetChange(true);
	}
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CFrameWnd::OnCommand(wParam, lParam);
}

#include "LayerDlg.h"
void CMainFrame::OnLayerAddlayer()
{
	CLayerDlg dlg;
	dlg.m_pCurScene = CSceneManager::Manager().GetCurScene();
	dlg.DoModal();
}


void CMainFrame::OnSavegameobject()
{
	if (NULL == m_pGameObject)
	{
		MessageBox(L"세이브 실패", L"게임오브젝트를 선택하세요");
		return;
	}
	wchar_t szFilter[] = L"Scene File (*.goc) |*.goc;|";

	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);
	CString strInitPath = CPathManager::GetResPathEX();
	strInitPath += L"Save\\GameObject";
	dlg.m_ofn.lpstrInitialDir = strInitPath;

	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		CSaveLoadManager::Manager().SaveGameObjectOnly(strPathName.GetBuffer(), m_pGameObject);
	}
}


void CMainFrame::OnGLoadgameobject()
{
	wchar_t szFilter[] = L"Scene File (*.goc) |*.goc;|";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);
	CString strInitPath = CPathManager::GetResPathEX();
	strInitPath += L"Save\\GameObject";
	dlg.m_ofn.lpstrInitialDir = strInitPath;
	CGameObject* pObj;
	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		pObj = CSaveLoadManager::Manager().LoadGameObjectOnly(strPathName.GetBuffer());
		pObj->Awake();
		pObj->Start();
	}
}

#include "CheckColLayerDlg.h"

void CMainFrame::OnLayerCheckcollayer()
{
	
}
