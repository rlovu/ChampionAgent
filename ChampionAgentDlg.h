
// ChampionAgentDlg.h: 헤더 파일
//

#pragma once


// CChampionAgentDlg 대화 상자
class CChampionAgentDlg : public CDialogEx
{
// 생성입니다.
public:
	CChampionAgentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAMPIONAGENT };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
public:
	CMapStringToPtr m_mapScreen; 
	LONG            m_intFidType;
	BOOL            m_bLoginYN;
	CString         m_strModulePath;

	BOOL Init_Control();
	void DisconnectAndRemove();

	void LoginProcess(long nVersionKey);
	int RegisterOCX(CString strFileName);

	// 통신요청
	void RequestSise(CString strCode);
	void RequestHoga(CString strCode);
	void RequestChe(CString strCode, CString strNextKey = _T(""));
	void RequestFIDPortfolio(CString strCode);

protected:
	HICON   m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnVersionComplite(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void OnGetTranData(long nRequestId, LPCTSTR pBlock, long nBlockLength);
	void OnGetFidData(long nRequestId, LPCTSTR pBlock, long nBlockLength);
	void OnGetRealData(short nPBID, LPCTSTR strRealKey, LPCTSTR pBlock, long nBlockLength);
	void OnAgentEventHandler(long nEventType, long nParam, LPCTSTR strParam);
	DECLARE_EVENTSINK_MAP();

public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnacc();

};
