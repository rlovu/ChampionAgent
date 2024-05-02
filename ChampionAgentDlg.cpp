
// ChampionAgentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ChampionAgent.h"
#include "ChampionAgentDlg.h"
#include "afxdialogex.h"

#include "../../include/ChampionCommAPIDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CommApi에게 통신상태를 알려주기 위한 메세지 정의
UINT g_uSocketNotifyMsg = ::RegisterWindowMessage((LPCTSTR)_T("SOCKET_NOTIFY_MSG"));

// Define Values

#define DLG_TYPE_DOMESTIC_HOGA _T("3");
#define DLG_TYPE_DOMESTIC_ORDER _T("4");
#define DLG_TYPE_DOMESTIC_NEWS _T("5");

#define IDC_CHAMPIONCOMMAGENTCTRL1    1000


// CChampionAgentDlg 대화 상자

CChampionAgentDlg::CChampionAgentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAMPIONAGENT_DIALOG, pParent)
{
	m_bLoginYN = FALSE;
	m_strModulePath = _T("");
}

void CChampionAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChampionAgentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_EU_VERSION_COMPLITE, OnVersionComplite)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CChampionAgentDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDOK, &CChampionAgentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNACC, &CChampionAgentDlg::OnBnClickedBtnacc)
END_MESSAGE_MAP()

// agent로부터 이벤트 수신 함수를 정의한다.
BEGIN_EVENTSINK_MAP(CChampionAgentDlg, CDialog)
	ON_EVENT(CChampionAgentDlg, IDC_CHAMPIONCOMMAGENTCTRL1, 1, CChampionAgentDlg::OnGetTranData, VTS_I4 VTS_BSTR VTS_I4)
END_EVENTSINK_MAP()


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChampionAgentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChampionAgentDlg::OnDestroy()
{
	DisconnectAndRemove();

	CDialogEx::OnDestroy();
}

void CChampionAgentDlg::DisconnectAndRemove()
{
	CString strKey;
	CWnd* pWnd = NULL;
	POSITION pos = m_mapScreen.GetStartPosition();
	while (pos != NULL)
	{
		pWnd = NULL;
		m_mapScreen.GetNextAssoc(pos, strKey, (void*&)pWnd);
		if (pWnd)
		{
			delete pWnd;
			pWnd = NULL;
		}
	}
	m_mapScreen.RemoveAll();

	if (theApp.m_pCommAgent && theApp.m_pCommAgent->GetSaveHwnd() != NULL)
	{
		CString strID;

		// 이전 로그인 정보가 있다면 이전 ID로 로그아웃 해준다.
		if (theApp.m_strUserID != _T(""))
			strID = theApp.m_strUserID;
		else
			GetDlgItemText(IDC_EDIT_ID, strID);

		// 로그아웃
		theApp.m_pCommAgent->CommLogout(strID);
		// 통신 종료 처리
		theApp.m_pCommAgent->CommTerminate(TRUE);

		theApp.m_pCommAgent->DestroyWindow();
		delete theApp.m_pCommAgent;
		theApp.m_pCommAgent = NULL;
		theApp.m_strUserID = _T("");
	}

	CDialogEx::OnDestroy();
}

void CChampionAgentDlg::OnClose()
{
	CDialogEx::OnClose();
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChampionAgentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// CChampionAgentDlg 메시지 처리기

BOOL CChampionAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bLoginYN = FALSE;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CChampionAgentDlg::RequestChe(CString strCode, CString strNextKey)
{
	// TODO: 여기에 구현 코드 추가.
}








void CChampionAgentDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CChampionAgentDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CChampionAgentDlg::OnBnClickedBtnacc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


LRESULT CChampionAgentDlg::OnVersionComplite(WPARAM wParam, LPARAM lParam)
{
	// 버전키(wParam)가 0 이상이고 버전처리 상태값(lParam)이 1 일 경우 성공일 경우 로그인 진행
	// '-' 값일 경우는 실패원인을 찾아봐야함.
	if (wParam > 0)
	{
		switch (lParam)
		{
			case VERSIONUP_FAILD           : break;
			case VERSIONUP_ERR_DOWN        : break;
			case VERSIONUP_ERR_COPY        : break;
			case VERSIONUP_ERR_ETC         : break;
			case VERSIONUP_ERR_FIND_MODULE : break;
			case VERSIONUP_ERR_RUN_MODULE  : break;
			case VERSIONUP_ERR_SERVER_IP   : break;
			case VERSIONUP_ERR_USER_CANCEL : break;
			case VERSIONUP_SUCCESS         : LoginProcess((long)wParam); break;
		}
	}
	return 0L;
}
