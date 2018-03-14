
// MFCi3systemDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "i3system_te.h"

void SetMedicalColorTable();
#ifdef __OPENCV_CORE_HPP__
void ApplyColorMap(cv::Mat &src, cv::Mat &dst, int nColor);
#endif // __OPENCV_CORE_HPP__

// CMFCi3systemDlg 대화 상자
class CMFCi3systemDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCi3systemDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCI3SYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void InsertString(const wchar_t * message);

public :

	SOCKET mh_my_socket;
	struct sockaddr_in srv_addr;
	void sendImage(unsigned short errorCode ,float * a);
protected:
	afx_msg LRESULT On10000(WPARAM wParam, LPARAM lParam);
public:
	CButton bt_connect;
	CButton bt_Thermal;
	afx_msg void OnBnClickedThermal();
	afx_msg void OnBnClickedconnect();
	afx_msg void OnBnClickedCapture();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSend();
	afx_msg void OnDestroy();

	static UINT Capturing(LPVOID);
																			unsigned short * fakeData;
	float *pImgBuf;
	bool exit = false;
	int hnd_dev = -1;
	bool isSend = false;
	bool isTemp = false;
	unsigned int x = 0;
	unsigned int y = 0;
	float _gain = 0.811111f / 256.f;
	float _offset = 0.5f;
	bool isCalibrate = false;
	//bool isAIE = false;
	//float AIEval = 10;

	CWinThread *CaptureThread = NULL;
	CWinThread *SendingThread = NULL;
	i3::TE_B *pTE;

	void CalcTemp(float *, float*);
};
