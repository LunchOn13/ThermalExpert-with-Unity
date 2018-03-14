
// MFCi3systemDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFCi3system.h"
#include "MFCi3systemDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "i3system_te.h"
#include "i3system_imgproc.h"
//#include <crtdbg.h>


using namespace i3;
using namespace std;
//using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifndef _DEBUG

#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)

#endif



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCi3systemDlg 대화 상자



CMFCi3systemDlg::CMFCi3systemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCI3SYSTEM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCi3systemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_connect, bt_connect);
	DDX_Control(pDX, IDC_Thermal, bt_Thermal);
}

BEGIN_MESSAGE_MAP(CMFCi3systemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(10000, &CMFCi3systemDlg::On10000)
	ON_BN_CLICKED(IDC_Thermal, &CMFCi3systemDlg::OnBnClickedThermal)
	ON_BN_CLICKED(IDC_connect, &CMFCi3systemDlg::OnBnClickedconnect)
	ON_BN_CLICKED(IDC_CAPTURE, &CMFCi3systemDlg::OnBnClickedCapture)
	ON_BN_CLICKED(IDC_CLOSE, &CMFCi3systemDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_SEND, &CMFCi3systemDlg::OnBnClickedSend)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCi3systemDlg 메시지 처리기

BOOL CMFCi3systemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	WSADATA data;
	WSAStartup(0x0202, &data);

	mh_my_socket = socket(AF_INET, SOCK_STREAM, 0);

	memset(&srv_addr, 0, sizeof(struct sockaddr_in));

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(8192);


	WSAAsyncSelect(mh_my_socket, m_hWnd, 10000, FD_CONNECT);

	// mfc 윈도우 숨기는 코드
	ShowWindow(SW_SHOWMINIMIZED);
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);

	OnBnClickedconnect();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCi3systemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCi3systemDlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCi3systemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CMFCi3systemDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int nRes = WindowProcTE(message, wParam, lParam);
	if (nRes == 1)
	{
		InsertString(L"Window Proc \r\n: USB device connection available (^~^)");
	}
	else if (nRes == 2) {
		InsertString(L"Window Proc \r\n: USB device connection not available(_|_)");
	}
	else if (nRes == 3) {
		InsertString(L"Window Proc \r\n: USB device removed (^-^)");
	}
	else if (nRes == 4) {
		InsertString(L"Window Proc \r\n: Failed to remove USB device (_|_)");
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CMFCi3systemDlg::InsertString(const wchar_t * message)
{
	std::cout << message;
}

void CMFCi3systemDlg::sendImage(unsigned short  errorcode, float * Timage)
{
	// 보내는 byte 배열 형식
	// 클릭한 좌표 온도 float type 4 byte
	// 이미지 데이터 byte 384 * 288 * 3


	if (mh_my_socket != NULL)
	{

		const int tempSize = 4;
		const int mmTempSize = 8;
		const int size = 384 * 288;
		const int dataSize = 4 * size;


		// 이미지 데이터 처리
		
		
			unsigned char * data = new unsigned char[dataSize + tempSize + mmTempSize];

			// 클릭된 좌표의 온도 데이터
			if (x >= 0 && y >= 0 && isTemp)
			{
				isTemp = false;
				float temp = pTE->CalcTemp(x, y, false);
				unsigned char *tempData = reinterpret_cast<unsigned char *>(&temp);

				for (int i = 0; i < tempSize; i++)
				{
					data[i] = tempData[i];
				}
			}
			else
			{
				//cout << "Send zero " << endl;
				for (int i = 0; i < tempSize; i++)
				{
					data[i] = 0;
				}
			}

			// max min temp 데이터
			float maxT;
			float minT;

			CalcTemp(&maxT, &minT);
			unsigned char *maxTempData = reinterpret_cast<unsigned char *>(&maxT);
			unsigned char *minTempData = reinterpret_cast<unsigned char *>(&minT);
			for (int i = 0; i < mmTempSize / 2; i++)
			{
				data[tempSize + i] = maxTempData[i];
				data[2 * tempSize + i] = minTempData[i];
			}

			//printf("max %f min %f\n", maxT, minT);
			// 이미지 데이터 작성

			for (int i = 0; i < size; i++)
			{
				//data[tempSize + mmTempSize + i] = data[i];

				for (int j = 0; j < 4; j++)
				{
					unsigned char *t = reinterpret_cast<unsigned char *>(&Timage[i]);
					data[tempSize + mmTempSize + 4 * i + j] = t[j];
				}
			}
			// 임시 길이 데이터 byte 로변환해야함
			unsigned char* lenbuf = new unsigned char[4];

			// c# 보내기용 int to byte 변환 코드
			for (int i = 0; i < 4; i++)
			{
				lenbuf[i] = (dataSize + tempSize + mmTempSize) >> (i * 8);
			}

			// 데이터 길이 보냄
			send(mh_my_socket, (char*)lenbuf, sizeof(int), 0);
			Sleep(10);
			// 이미지 데이터 보냄
			send(mh_my_socket, (char*)data, dataSize + tempSize + mmTempSize, 0);
			//std::cout << "sended " << std::endl;

			/*color.myname = "color";
			in->myname = "in";
			img8uc3.myname = "img8uc3";
			matAIE.myname = "mat Aie";*/
			delete data;
			delete lenbuf;
		
			// 메모리 누수 확인용

			_CrtDumpMemoryLeaks();
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

}

afx_msg LRESULT CMFCi3systemDlg::On10000(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		std::cout << "connected " << std::endl;
		if (WSAGETSELECTERROR(lParam) == 0)
		{
			OnBnClickedThermal();
			WSAAsyncSelect(mh_my_socket, m_hWnd, 10000, FD_READ | FD_CLOSE);

		}
		else
		{
			std::cout << "connect fail" << std::endl;
			OnBnClickedconnect();
		}
		break;
	case FD_READ:
		if (WSAGETSELECTERROR(lParam) == 0)
		{
			//std::cout << "read " << std::endl;
			WSAAsyncSelect(mh_my_socket, m_hWnd, 10000, FD_CLOSE);
			char * data = new char[9];

			// x ,y  좌표 || gain offset 분기문 작성
			recv(mh_my_socket, data, 9 , 0);

			if (isCalibrate == false)
			{
				if (data[0] == 0)
				{
					memcpy(&x, &data[1], 4);
					memcpy(&y, &data[5], 4);
					/*x = (((data[1])) & 0x00FF | ((data[2]) << 8));
					y = (((data[5])) & 0x00FF | ((data[6]) << 8));*/
					isTemp = true;
				}
				//else if(data[0] == 1)
				//{
				//	// _gain 할당
				//	// _offset 할당
				//	memcpy(&_gain, &data[1], 4);
				//	memcpy(&_offset, &data[5], 4);
				//}
				else if (data[0] == 2)
				{
					isCalibrate = true;
					while (pTE->ShutterCalibrationOn() == 2);
					{
						printf("Calibrating\n");
					}
					printf("Calibrate done\n");
					isCalibrate = false;
				}
				//else // data[0] == 2
				//{
				//	// isAIE 받기
				//	// AIE val 받기
				//	memcpy(&isAIE, &data[1], sizeof(bool));
				//	memcpy(&AIEval, &data[1+sizeof(bool)], sizeof(float));
				//	cout << "success" << endl;

				//}
				else
				{
					printf("다른 자료가 들어옴\n");
				}
			}

			delete data;
			WSAAsyncSelect(mh_my_socket, m_hWnd, 10000, FD_READ | FD_CLOSE);
		}
		else
		{
			std::cout << "read fail" << std::endl;
			OnBnClickedconnect();
		}
		break;
	case FD_CLOSE:
		// 소켓 닫히면 종료
		OnBnClickedClose();
		cout << "종료 " << endl;
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
		mh_my_socket = NULL;
		break;
	}

	return 0;
}


void CMFCi3systemDlg::OnBnClickedThermal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Scan connected TE.
	SCANINFO *pScan = new SCANINFO[MAX_USB_NUM];
	ScanTE(this->GetSafeHwnd(), pScan);
	cout << this->GetSafeHwnd() << endl;
	for (int i = 0; i < MAX_USB_NUM; i++) {
		if (pScan[i].bDevCon) {
			hnd_dev = i;
			break;
		}
	}
	delete[] pScan;

	// Open Connected Device
	if (hnd_dev != -1) {
		pTE = OpenTE_B(this->GetSafeHwnd(), I3_TE_Q1, hnd_dev);
		if (pTE) {
			cout << "TE Opened" << endl;
			
		}
		else {
			cout << "Open Failed" << endl;
		}
	}
	else {
		cout << "Device Not Connected" << endl;
	}

	OnBnClickedCapture();
}


void CMFCi3systemDlg::OnBnClickedconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	std::cout << "try connecting" << std::endl;
	connect(mh_my_socket, (LPSOCKADDR)&srv_addr, sizeof(struct sockaddr_in));
}



void CMFCi3systemDlg::OnBnClickedCapture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Read Flash Data
	if (pTE->ReadFlashData() == 1) {
		cout << "Read Flash Data" << endl;
		if (CaptureThread == NULL) {
			CaptureThread = AfxBeginThread(Capturing, this);

			CaptureThread->m_bAutoDelete = FALSE;
			CaptureThread->ResumeThread();
		}
		else
			cout << "이미 스레드 실행중 " << endl;
	}
	else {
		cout << "Fail to Read Flash Data" << endl;
	}
}


void CMFCi3systemDlg::OnBnClickedClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (NULL != CaptureThread)
	{
		cout << "스레드 종료 시작 " << endl;
		DWORD dwResult = ::WaitForSingleObject(CaptureThread->m_hThread, 100);
		exit = true;
		if (dwResult == WAIT_TIMEOUT)
		{

		}

		cout << "스레드 종료 " << endl;
		CaptureThread = NULL;
		delete CaptureThread;

		::TerminateThread(CaptureThread, 0);
		exit = false;
	}
	//Close Device

}


void CMFCi3systemDlg::OnBnClickedSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (isSend == true && pImgBuf != NULL)
	{
		float * buf = new float[384 * 288];

		for (int i = 0; i < 384 * 288; i++)
		{
			buf[i] = pImgBuf[i];
		}

		sendImage(1, buf);

		isSend = false;
		delete buf;
	}
}

UINT CMFCi3systemDlg::Capturing(LPVOID _method)
{
	CMFCi3systemDlg * dlg = (CMFCi3systemDlg*)AfxGetApp()->m_pMainWnd;

	int i = 0;

	int width = dlg->pTE->GetImageWidth(), height = dlg->pTE->GetImageHeight();
	cout << width << " " << height << endl;
	while (true) {
		// Get Image Data

		if (dlg->pTE != NULL && !dlg->isCalibrate)
		{
			
				dlg->fakeData = new unsigned short[width*height];
				dlg->pImgBuf = new  float[width*height];

				if (dlg->pTE->RecvImage(dlg->fakeData) == 1)
				{
					dlg->isSend = true;
					dlg->pTE->CalcEntireTemp(dlg->pImgBuf);
					//cout << "receive" << endl;
					dlg->OnBnClickedSend();
				}
				else {
					//cout << "Image not received" << endl;
				}
				delete dlg->pImgBuf;
				delete dlg->fakeData;

				// 메모리 누수 확인용
				_CrtDumpMemoryLeaks();
				_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			if (dlg->exit == true)
				break;
		}

	}
	return 0;
}



void CMFCi3systemDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (CaptureThread != NULL)
		OnBnClickedClose();

	pTE->CloseTE();
	cout << "Close Usb" << endl;
	WSACleanup();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMFCi3systemDlg::CalcTemp(float *_hot,float *_cold)
{
	float hot = 0;
	float cold = 0;
	if (pImgBuf[0] >= pImgBuf[1])
	{
		hot = pImgBuf[0];
		cold = pImgBuf[1];
	}
	else
	{
		hot = pImgBuf[1];
		cold = pImgBuf[0];
	}
	for (int i = 2; i < 384 * 288; i++)
	{
		if (pImgBuf[i] >= hot)
		{
			hot = pImgBuf[i];
		}
		else if(cold >= pImgBuf[i])
		{
			cold = pImgBuf[i];
		}
	}
	*_hot = hot;
	*_cold = cold;
}

namespace I3ColorMap {
	enum {
		COLORMAP_WHITEHOT = 0,
		COLORMAP_BLACKHOT = 1,
		COLORMAP_IRON = 2,
		COLORMAP_BLUERED = 3,
		COLORMAP_MEDICAL = 4,
		COLORMAP_PURPLE = 5,
		COLORMAP_PURPLEYELLOW = 6,
		COLORMAP_DARKBLUE = 7,
		COLORMAP_CYAN = 8,
		COLORMAP_RAINBOW = 9,
		COLORMAP_AUTUMN = 10,
		COLORMAP_BONE = 11,
		COLORMAP_JET = 12,
		COLORMAP_WINTER = 13,
		COLORMAP_OCEAN = 14,
		COLORMAP_SUMMER = 15,
		COLORMAP_SPRING = 16,
		COLORMAP_COOL = 17,
		COLORMAP_HSV = 18,
		COLORMAP_HOT = 19,
	};
}

#ifdef __OPENCV_CORE_HPP__
using namespace cv;
// Look up table
Mat					m_lutBlackHot;
Mat					m_lutBlueRed;
Mat					m_lutIron;
Mat					m_lutMedical;
Mat					m_lutPurple;
Mat					m_lutPurpleYellow;
Mat					m_lutDarkBlue;
Mat					m_lutCyan;
Mat					m_lutRainbow;
#endif
// --------------------------- 내부 함수 선언 시작 -------------------------------- //
void SetBlackHotColorTable();
void SetIronColorTable();
void SetBlueRedColorTable();
void SetMedicalColorTable();
void SetPurpleColorTable();
void SetPurpleYellowColorTable();
void SetDarkBlueColorTable();
void SetCyanColorTable();
void SetRainbowColorTalbe();
void SetColorTable();
// --------------------------- 내부 함수 선언 끝 -------------------------------- //

void SetColorTable()
{
	SetBlackHotColorTable();
	SetIronColorTable();
	SetBlueRedColorTable();
	SetMedicalColorTable();
	SetPurpleColorTable();
	SetPurpleYellowColorTable();
	SetDarkBlueColorTable();
	SetCyanColorTable();
	SetRainbowColorTalbe();
}

#ifdef __OPENCV_CORE_HPP__
void ApplyColorMap(cv::Mat &src, cv::Mat &dst, int nColor)
{
	SetColorTable();
	switch (nColor) {
	case I3ColorMap::COLORMAP_WHITEHOT:
		dst = src.clone();
		break;
	case I3ColorMap::COLORMAP_BLACKHOT:
		LUT(src, m_lutBlackHot, dst);
		break;
	case I3ColorMap::COLORMAP_IRON:
		LUT(src, m_lutIron, dst);
		break;
	case I3ColorMap::COLORMAP_BLUERED:
		LUT(src, m_lutBlueRed, dst);
		break;
	case I3ColorMap::COLORMAP_MEDICAL:
		LUT(src, m_lutMedical, dst);
		break;
	case I3ColorMap::COLORMAP_PURPLE:
		LUT(src, m_lutPurple, dst);
		break;
	case I3ColorMap::COLORMAP_PURPLEYELLOW:
		LUT(src, m_lutPurpleYellow, dst);
		break;
	case I3ColorMap::COLORMAP_DARKBLUE:
		LUT(src, m_lutDarkBlue, dst);
		break;
	case I3ColorMap::COLORMAP_CYAN:
		LUT(src, m_lutCyan, dst);
		break;
	case I3ColorMap::COLORMAP_RAINBOW:
		LUT(src, m_lutRainbow, dst);
		break;
		//case I3ColorMap::COLORMAP_AUTUMN:
		//	applyColorMap(src, dst, COLORMAP_AUTUMN);
		//	break;
		//case I3ColorMap::COLORMAP_BONE:
		//	applyColorMap(src, dst, COLORMAP_BONE);
		//	break;
		//case I3ColorMap::COLORMAP_JET:
		//	applyColorMap(src, dst, COLORMAP_JET);
		//	break;
		//case I3ColorMap::COLORMAP_WINTER:
		//	applyColorMap(src, dst, COLORMAP_WINTER);
		//	break;
		//case I3ColorMap::COLORMAP_OCEAN:
		//	applyColorMap(src, dst, COLORMAP_OCEAN);
		//	break;
		//case I3ColorMap::COLORMAP_SUMMER:
		//	applyColorMap(src, dst, COLORMAP_SUMMER);
		//	break;
		//case I3ColorMap::COLORMAP_SPRING:
		//	applyColorMap(src, dst, COLORMAP_SPRING);
		//	break;
		//case I3ColorMap::COLORMAP_COOL:
		//	applyColorMap(src, dst, COLORMAP_COOL);
		//	break;
		//case I3ColorMap::COLORMAP_HSV:
		//	applyColorMap(src, dst, COLORMAP_HSV);
		//	break;
		//case I3ColorMap::COLORMAP_HOT:
		//	applyColorMap(src, dst, COLORMAP_HOT);
		//	break;

	}
}

#endif


// --------------------------- 내부 함수 정의 ---------------------------------- //
#ifdef __OPENCV_CORE_HPP__

void SetBlackHotColorTable() {
	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b((255 - i) & 0xff, (255 - i) & 0xff, (255 - i) & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutBlackHot = lut.clone();
}

void SetIronColorTable() {
	int temp[256][3] = { { 255, 255, 250 },{ 255, 255, 247 },{ 255, 255, 242 },{ 255, 255, 235 },{ 255, 254, 229 },{ 255, 254, 223 },{ 255, 253, 218 },
	{ 255, 253, 215 },{ 255, 252, 210 },{ 255, 251, 204 },{ 255, 250, 197 },{ 255, 250, 191 },{ 255, 250, 184 },{ 255, 250, 176 },
	{ 255, 249, 169 },{ 255, 248, 162 },{ 255, 247, 156 },{ 255, 247, 149 },{ 255, 245, 142 },{ 255, 245, 134 },{ 255, 244, 126 },
	{ 255, 244, 118 },{ 255, 242, 111 },{ 255, 242, 104 },{ 255, 241, 98 },{ 255, 241, 93 },{ 255, 239, 85 },{ 255, 238, 77 },
	{ 255, 237, 71 },{ 255, 236, 67 },{ 255, 234, 62 },{ 255, 233, 56 },{ 255, 232, 49 },{ 255, 231, 45 },{ 255, 230, 42 },{ 255, 228, 36 },
	{ 255, 227, 32 },{ 255, 226, 28 },{ 255, 225, 23 },{ 255, 224, 19 },{ 255, 222, 18 },{ 255, 221, 15 },{ 255, 218, 11 },{ 255, 216, 10 },
	{ 255, 215, 9 },{ 255, 213, 8 },{ 255, 211, 6 },{ 255, 209, 5 },{ 255, 208, 3 },{ 255, 207, 1 },{ 255, 206, 1 },{ 255, 205, 0 },
	{ 255, 202, 0 },{ 255, 201, 0 },{ 255, 200, 1 },{ 255, 197, 0 },{ 255, 196, 1 },{ 255, 194, 1 },{ 255, 191, 1 },{ 255, 189, 0 },
	{ 255, 187, 1 },{ 255, 186, 0 },{ 255, 184, 0 },{ 255, 182, 0 },{ 255, 180, 1 },{ 255, 180, 1 },{ 255, 178, 0 },{ 255, 177, 0 },
	{ 255, 173, 0 },{ 255, 171, 0 },{ 255, 169, 1 },{ 255, 167, 2 },{ 255, 165, 2 },{ 255, 163, 0 },{ 255, 161, 1 },{ 255, 159, 1 },
	{ 255, 157, 0 },{ 255, 155, 0 },{ 255, 153, 0 },{ 255, 150, 0 },{ 255, 149, 1 },{ 255, 147, 0 },{ 255, 145, 0 },{ 255, 143, 0 },
	{ 255, 141, 0 },{ 255, 139, 0 },{ 255, 137, 0 },{ 255, 135, 0 },{ 255, 134, 0 },{ 255, 133, 0 },{ 255, 131, 0 },{ 255, 129, 0 },
	{ 255, 127, 0 },{ 254, 125, 0 },{ 254, 124, 0 },{ 253, 123, 0 },{ 253, 122, 0 },{ 252, 121, 0 },{ 252, 120, 0 },{ 251, 119, 0 },
	{ 251, 118, 0 },{ 250, 117, 0 },{ 250, 116, 0 },{ 249, 115, 0 },{ 249, 114, 0 },{ 248, 112, 0 },{ 248, 110, 0 },{ 247, 108, 0 },
	{ 247, 106, 0 },{ 246, 104, 1 },{ 246, 102, 2 },{ 245, 100, 3 },{ 245, 98, 4 },{ 244, 96, 5 },{ 244, 94, 6 },{ 243, 92, 7 },
	{ 243, 90, 8 },{ 242, 88, 9 },{ 242, 86, 10 },{ 241, 84, 11 },{ 241, 82, 12 },{ 240, 80, 13 },{ 240, 78, 14 },{ 239, 76, 15 },
	{ 239, 74, 16 },{ 238, 72, 17 },{ 238, 70, 18 },{ 237, 68, 19 },{ 237, 66, 23 },{ 236, 64, 27 },{ 236, 62, 31 },{ 235, 60, 36 },
	{ 235, 58, 41 },{ 234, 56, 46 },{ 233, 54, 51 },{ 232, 52, 56 },{ 231, 50, 61 },{ 230, 48, 66 },{ 229, 46, 71 },{ 228, 44, 76 },
	{ 227, 42, 81 },{ 226, 40, 86 },{ 225, 38, 91 },{ 224, 36, 96 },{ 223, 34, 99 },{ 222, 32, 102 },{ 221, 30, 105 },{ 220, 28, 108 },
	{ 219, 26, 111 },{ 218, 25, 114 },{ 217, 24, 117 },{ 216, 23, 120 },{ 215, 22, 123 },{ 214, 21, 126 },{ 213, 20, 129 },{ 212, 19, 132 },
	{ 210, 18, 135 },{ 209, 17, 138 },{ 208, 16, 140 },{ 208, 15, 142 },{ 207, 14, 144 },{ 206, 13, 146 },{ 205, 12, 147 },{ 204, 11, 147 },
	{ 202, 10, 148 },{ 200, 9, 148 },{ 198, 8, 149 },{ 196, 7, 149 },{ 194, 6, 150 },{ 192, 5, 150 },{ 190, 4, 151 },{ 188, 3, 151 },
	{ 186, 2, 152 },{ 184, 1, 152 },{ 182, 0, 153 },{ 180, 0, 153 },{ 178, 0, 154 },{ 176, 0, 154 },{ 174, 0, 155 },{ 172, 0, 155 },
	{ 170, 0, 156 },{ 168, 0, 156 },{ 166, 0, 157 },{ 164, 0, 157 },{ 162, 0, 158 },{ 160, 0, 158 },{ 158, 0, 159 },{ 156, 0, 159 },
	{ 154, 0, 160 },{ 152, 0, 160 },{ 150, 0, 161 },{ 148, 0, 161 },{ 146, 0, 162 },{ 144, 1, 162 },{ 141, 0, 163 },{ 138, 0, 163 },
	{ 135, 0, 164 },{ 132, 0, 164 },{ 129, 0, 165 },{ 126, 0, 165 },{ 123, 0, 166 },{ 120, 0, 166 },{ 117, 0, 165 },{ 114, 0, 164 },
	{ 111, 0, 163 },{ 108, 0, 162 },{ 105, 0, 161 },{ 102, 0, 160 },{ 99, 0, 159 },{ 96, 0, 158 },{ 93, 0, 157 },{ 90, 0, 156 },
	{ 87, 0, 155 },{ 84, 0, 154 },{ 81, 0, 153 },{ 78, 0, 153 },{ 75, 0, 152 },{ 72, 0, 152 },{ 69, 0, 151 },{ 66, 0, 151 },{ 63, 0, 150 },
	{ 60, 0, 150 },{ 57, 0, 148 },{ 54, 0, 148 },{ 51, 0, 146 },{ 48, 0, 144 },{ 45, 0, 142 },{ 42, 0, 140 },{ 39, 0, 138 },{ 36, 0, 136 },
	{ 34, 0, 134 },{ 32, 0, 132 },{ 30, 0, 130 },{ 28, 0, 128 },{ 26, 0, 126 },{ 24, 0, 124 },{ 22, 0, 122 },{ 20, 0, 120 },{ 18, 0, 118 },
	{ 16, 0, 116 },{ 14, 0, 111 },{ 12, 0, 106 },{ 10, 0, 101 },{ 8, 0, 96 },{ 6, 0, 91 },{ 4, 0, 86 },{ 2, 0, 81 },{ 0, 0, 76 },
	{ 0, 0, 71 },{ 0, 0, 66 },{ 0, 0, 61 },{ 0, 0, 56 },{ 1, 0, 51 },{ 1, 1, 46 },{ 0, 0, 41 },{ 0, 0, 36 } };

	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[256 - i - 1][2] & 0xff, temp[256 - i - 1][1] & 0xff, temp[256 - i - 1][0] & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutIron = lut.clone();
}


void SetBlueRedColorTable() {
	int temp[256][3] = {
		{ 255, 255, 255 },{ 255, 250, 250 },{ 255, 244, 245 },{ 255, 237, 238 },
		{ 255, 230, 231 },{ 255, 222, 224 },{ 255, 213, 215 },{ 255, 203, 206 },
		{ 255, 194, 197 },{ 255, 184, 188 },{ 255, 173, 177 },{ 255, 162, 167 },
		{ 255, 150, 156 },{ 255, 139, 146 },{ 255, 127, 135 },{ 255, 117, 125 },
		{ 255, 108, 116 },{ 255, 99, 108 },{ 255, 91, 100 },{ 255, 82, 91 },
		{ 255, 73, 83 },{ 255, 65, 75 },{ 255, 58, 67 },{ 255, 50, 59 },
		{ 255, 43, 52 },{ 255, 36, 45 },{ 255, 30, 38 },{ 255, 24, 32 },
		{ 255, 19, 26 },{ 255, 14, 20 },{ 255, 9, 15 },{ 255, 6, 10 },
		{ 255, 3, 5 },{ 255, 1, 1 },{ 255, 0, 0 },{ 255, 0, 0 },
		{ 255, 0, 0 },{ 255, 0, 0 },{ 255, 0, 0 },{ 255, 0, 0 },
		{ 255, 0, 0 },{ 255, 2, 0 },{ 255, 4, 0 },{ 255, 7, 0 },
		{ 255, 9, 0 },{ 255, 13, 0 },{ 255, 17, 0 },{ 255, 20, 0 },
		{ 255, 25, 0 },{ 255, 29, 0 },{ 255, 34, 0 },{ 255, 38, 0 },
		{ 255, 43, 0 },{ 255, 48, 0 },{ 255, 53, 0 },{ 255, 59, 0 },
		{ 255, 64, 0 },{ 255, 69, 0 },{ 255, 74, 0 },{ 255, 79, 0 },
		{ 255, 85, 0 },{ 255, 90, 0 },{ 255, 96, 0 },{ 255, 101, 0 },
		{ 255, 105, 0 },{ 255, 110, 0 },{ 255, 115, 0 },{ 255, 119, 0 },
		{ 255, 123, 0 },{ 255, 126, 0 },{ 255, 130, 0 },{ 255, 133, 0 },
		{ 255, 136, 0 },{ 255, 140, 0 },{ 255, 143, 0 },{ 255, 147, 0 },
		{ 255, 151, 0 },{ 255, 154, 0 },{ 255, 158, 0 },{ 255, 162, 0 },
		{ 255, 165, 0 },{ 255, 169, 0 },{ 255, 173, 0 },{ 255, 176, 0 },
		{ 255, 180, 0 },{ 255, 184, 0 },{ 255, 188, 0 },{ 255, 192, 0 },
		{ 255, 195, 0 },{ 255, 199, 0 },{ 255, 203, 0 },{ 255, 206, 0 },
		{ 255, 210, 0 },{ 255, 213, 0 },{ 255, 217, 0 },{ 255, 221, 0 },
		{ 255, 224, 0 },{ 255, 228, 0 },{ 255, 231, 0 },{ 255, 234, 0 },
		{ 255, 237, 0 },{ 255, 240, 0 },{ 255, 243, 0 },{ 255, 245, 0 },
		{ 255, 248, 0 },{ 255, 250, 0 },{ 255, 252, 0 },{ 255, 254, 0 },
		{ 254, 255, 0 },{ 252, 255, 0 },{ 249, 255, 0 },{ 246, 255, 0 },
		{ 243, 255, 0 },{ 240, 255, 0 },{ 237, 255, 0 },{ 233, 255, 0 },
		{ 230, 255, 0 },{ 226, 255, 0 },{ 222, 255, 0 },{ 218, 255, 0 },
		{ 214, 255, 0 },{ 210, 255, 0 },{ 205, 255, 0 },{ 200, 255, 0 },
		{ 196, 255, 0 },{ 192, 255, 0 },{ 187, 255, 0 },{ 182, 255, 0 },
		{ 176, 255, 0 },{ 172, 255, 0 },{ 166, 255, 0 },{ 161, 255, 0 },
		{ 156, 255, 0 },{ 151, 255, 0 },{ 145, 255, 0 },{ 140, 255, 0 },
		{ 134, 255, 0 },{ 129, 255, 0 },{ 119, 255, 0 },{ 105, 255, 0 },
		{ 92, 255, 0 },{ 78, 255, 0 },{ 66, 255, 0 },{ 54, 255, 0 },
		{ 42, 255, 0 },{ 31, 255, 0 },{ 22, 255, 0 },{ 14, 255, 0 },
		{ 6, 255, 0 },{ 0, 255, 0 },{ 0, 254, 5 },{ 0, 253, 10 },
		{ 0, 251, 15 },{ 0, 250, 21 },{ 0, 249, 27 },{ 0, 247, 34 },
		{ 0, 246, 41 },{ 0, 245, 49 },{ 0, 243, 58 },{ 0, 241, 66 },
		{ 0, 239, 75 },{ 0, 238, 84 },{ 0, 236, 94 },{ 0, 234, 103 },
		{ 0, 232, 112 },{ 0, 230, 122 },{ 0, 228, 132 },{ 0, 226, 141 },
		{ 0, 223, 151 },{ 0, 221, 159 },{ 0, 219, 169 },{ 0, 216, 178 },
		{ 0, 213, 186 },{ 0, 211, 195 },{ 0, 208, 204 },{ 0, 205, 212 },
		{ 0, 202, 219 },{ 0, 199, 226 },{ 0, 195, 233 },{ 0, 192, 239 },
		{ 0, 188, 245 },{ 0, 185, 249 },{ 0, 181, 254 },{ 0, 177, 255 },
		{ 0, 173, 255 },{ 0, 169, 255 },{ 0, 165, 255 },{ 0, 160, 255 },
		{ 0, 155, 255 },{ 0, 149, 255 },{ 0, 144, 255 },{ 0, 139, 255 },
		{ 0, 133, 255 },{ 0, 127, 255 },{ 0, 121, 255 },{ 0, 115, 255 },
		{ 0, 109, 255 },{ 0, 103, 255 },{ 0, 97, 255 },{ 0, 90, 255 },
		{ 0, 84, 255 },{ 0, 79, 255 },{ 0, 73, 255 },{ 0, 67, 255 },
		{ 0, 61, 255 },{ 0, 56, 255 },{ 0, 50, 255 },{ 0, 45, 255 },
		{ 0, 40, 255 },{ 0, 35, 255 },{ 0, 30, 255 },{ 0, 25, 255 },
		{ 0, 21, 255 },{ 0, 17, 255 },{ 0, 13, 255 },{ 0, 10, 255 },
		{ 0, 6, 255 },{ 0, 3, 255 },{ 0, 1, 255 },{ 0, 0, 254 },
		{ 0, 0, 252 },{ 0, 0, 249 },{ 0, 0, 247 },{ 0, 0, 245 },
		{ 0, 0, 242 },{ 0, 0, 239 },{ 0, 0, 236 },{ 0, 0, 233 },
		{ 0, 0, 229 },{ 0, 0, 225 },{ 0, 0, 222 },{ 0, 0, 218 },
		{ 0, 0, 214 },{ 0, 0, 211 },{ 0, 0, 207 },{ 0, 0, 203 },
		{ 0, 0, 199 },{ 0, 0, 195 },{ 0, 0, 191 },{ 0, 0, 187 },
		{ 0, 0, 182 },{ 0, 0, 179 },{ 0, 0, 175 },{ 0, 0, 171 },
		{ 0, 0, 167 },{ 0, 0, 164 },{ 0, 0, 159 },{ 0, 0, 156 },
		{ 0, 0, 153 },{ 0, 0, 150 },{ 0, 0, 147 },{ 0, 0, 144 },
		{ 0, 0, 141 },{ 0, 0, 138 },{ 0, 1, 136 },{ 0, 1, 134 }

	};
	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {

		Vec3b repl = Vec3b((255 - temp[i][2]) & 0xff, (255 - temp[i][1]) & 0xff, (255 - temp[i][0]) & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutBlueRed = lut.clone();

}


void SetMedicalColorTable() {
	int temp[256][3] = 
	/*{ { 110, 50, 50 } ,{ 120, 50, 50 }, { 130, 50, 50 }, { 139, 50, 50 }, { 149, 50, 50 }, { 159, 50, 50 }, { 168, 50, 50 }, { 178, 50, 50 },
	{ 188, 50, 50 }, { 197, 50, 50 }, { 207, 50, 50 }, { 217, 50, 50 }, { 226, 50, 50 }, { 236, 50, 50 }, { 246, 50, 50 }, { 255, 50, 50 },
	{ 255, 54, 50 }, { 255, 57, 50 }, { 255, 60, 50 }, { 255, 63, 50 }, { 255, 66, 50 }, { 255, 69, 50 }, { 255, 72, 50 }, { 255, 75, 50 },
	{ 255, 79, 50 }, { 255, 82, 50 }, { 255, 85, 50 }, { 255, 88, 50 }, { 255, 91, 50 }, { 255, 94, 50 }, { 255, 97, 50 }, { 255, 100, 50 },
	{ 255, 104, 50 }, { 255, 107, 50 }, { 255, 110, 50 }, { 255, 113, 50 }, { 255, 116, 50 }, { 255, 119, 50 }, { 255, 122, 50 }, { 255, 125, 50 },
	{ 255, 129, 50 }, { 255, 132, 50 }, { 255, 135, 50 }, { 255, 138, 50 }, { 255, 141, 50 }, { 255, 144, 50 }, { 255, 147, 50 }, { 255, 150, 50 },
	{ 255, 154, 50 }, { 255, 157, 50 }, { 255, 160, 50 }, { 255, 163, 50 }, { 255, 166, 50 }, { 255, 169, 50 }, { 255, 172, 50 }, { 255, 175, 50 },
	{ 255, 179, 50 }, { 255, 182, 50 }, { 255, 185, 50 }, { 255, 188, 50 }, { 255, 191, 50 }, { 255, 194, 50 }, { 255, 197, 50 }, { 255, 200, 50 },
	{ 255, 204, 50 }, { 255, 207, 50 }, { 255, 211, 50 }, { 255, 214, 50 }, { 255, 218, 50 }, { 255, 221, 50 }, { 255, 225, 50 }, { 255, 228, 50 },
	{ 255, 231, 50 }, { 255, 235, 50 }, { 255, 238, 50 }, { 255, 242, 50 }, { 255, 245, 50 }, { 255, 249, 50 }, { 255, 252, 50 }, { 255, 255, 50 },
	{ 247, 253, 50 }, { 239, 251, 50 }, { 231, 250, 50 }, { 223, 248, 50 }, { 215, 247, 50 }, { 208, 245, 50 }, { 200, 244, 50 }, { 192, 242, 50 },
	{ 184, 240, 50 }, { 176, 239, 50 }, { 169, 237, 50 }, { 161, 236, 50 }, { 153, 234, 50 }, { 145, 233, 50 }, { 137, 231, 50 }, { 130, 230, 50 },
	{ 121, 228, 52 }, { 113, 226, 54 }, { 105, 224, 55 }, { 97, 222, 57 }, { 89, 220, 58 }, { 81, 218, 60 }, { 73, 216, 61 }, { 65, 215, 63 },
	{ 56, 213, 65 }, { 48, 211, 66 }, { 40, 209, 68 }, { 32, 207, 69 }, { 24, 205, 71 }, { 16, 203, 72 }, { 8, 201, 74 }, { 0, 200, 75 },
	{ 0, 199, 77 }, { 0, 198, 79 }, { 0, 198, 80 }, { 0, 197, 82 }, { 0, 196, 83 }, { 0, 196, 85 }, { 0, 195, 86 }, { 0, 195, 88 }, { 0, 194, 90 },
	{ 0, 193, 91 }, { 0, 193, 93 }, { 0, 192, 94 }, { 0, 191, 96 }, { 0, 191, 97 }, { 0, 190, 99 }, { 0, 190, 100 }, { 0, 189, 105 },
	{ 0, 188, 110 }, { 0, 188, 115 }, { 0, 187, 120 }, { 0, 186, 125 }, { 0, 186, 130 }, { 0, 185, 135 }, { 0, 185, 140 }, { 0, 184, 145 },
	{ 0, 183, 150 }, { 0, 183, 155 }, { 0, 182, 160 }, { 0, 181, 165 }, { 0, 181, 170 }, { 0, 180, 175 }, { 0, 180, 180 }, { 0, 174, 179 },
	{ 0, 168, 178 }, { 0, 163, 177 }, { 0, 157, 176 }, { 0, 151, 175 }, { 0, 146, 174 }, { 0, 140, 173 }, { 0, 135, 172 }, { 0, 129, 171 },
	{ 0, 123, 170 }, { 0, 118, 169 }, { 0, 112, 168 }, { 0, 106, 167 }, { 0, 101, 166 }, { 0, 95, 165 }, { 0, 90, 165 }, { 0, 84, 164 },
	{ 0, 78, 163 }, { 0, 73, 162 }, { 0, 67, 161 }, { 0, 61, 160 }, { 0, 56, 159 }, { 0, 50, 158 }, { 0, 45, 157 }, { 0, 39, 156 }, { 0, 33, 155 },
	{ 0, 28, 154 }, { 0, 22, 153 }, { 0, 16, 152 }, { 0, 11, 151 }, { 0, 5, 150 }, { 0, 0, 150 }, { 2, 0, 149 }, { 4, 0, 149 }, { 6, 0, 149 },
	{ 8, 0, 148 }, { 10, 0, 148 }, { 12, 0, 148 }, { 14, 0, 147 }, { 15, 0, 147 }, { 17, 0, 147 }, { 19, 0, 146 }, { 21, 0, 146 }, { 23, 0, 146 },
	{ 25, 0, 145 }, { 27, 0, 145 }, { 29, 0, 145 }, { 30, 0, 145 }, { 32, 0, 144 }, { 34, 0, 144 }, { 36, 0, 144 }, { 38, 0, 143 }, { 40, 0, 143 },
	{ 42, 0, 143 }, { 44, 0, 142 }, { 45, 0, 142 }, { 47, 0, 142 }, { 49, 0, 141 }, { 51, 0, 141 }, { 53, 0, 141 }, { 55, 0, 140 }, { 57, 0, 140 },
	{ 59, 0, 140 }, { 60, 0, 140 }, { 63, 0, 139 }, { 65, 0, 139 }, { 68, 0, 139 }, { 70, 0, 138 }, { 73, 0, 138 }, { 75, 0, 138 }, { 78, 0, 137 },
	{ 80, 0, 137 }, { 83, 0, 137 }, { 85, 0, 136 }, { 88, 0, 136 }, { 90, 0, 136 }, { 93, 0, 135 }, { 95, 0, 135 }, { 98, 0, 135 }, { 100, 0, 135 },
	{ 102, 0, 134 }, { 104, 0, 134 }, { 106, 0, 134 }, { 108, 0, 133 }, { 110, 0, 133 }, { 112, 0, 133 }, { 114, 0, 132 }, { 115, 0, 132 },
	{ 117, 0, 132 }, { 119, 0, 131 }, { 121, 0, 131 }, { 123, 0, 131 }, { 125, 0, 130 }, { 127, 0, 130 }, { 129, 0, 130 }, { 130, 0, 130 },
	{ 121, 0, 121 }, { 113, 0, 113 }, { 105, 0, 105 }, { 97, 0, 97 }, { 89, 0, 89 }, { 81, 0, 81 }, { 73, 0, 73 }, { 65, 0, 65 },
	{ 56, 0, 56 }, { 48, 0, 48 }, { 40, 0, 40 }, { 32, 0, 32 }, { 24, 0, 24 }, { 16, 0, 16 }, { 8, 0, 8 }, { 0, 0, 0 } };*/
	{ { 0, 0, 0 },{ 8, 0, 8 },{ 16, 0, 16 },{ 24, 0, 24 },{ 32, 0, 32 },{ 40, 0, 40 },{ 48, 0, 48 },{ 56, 0, 56 },{ 65, 0, 65 },
	{ 73, 0, 73 },{ 81, 0, 81 },{ 89, 0, 89 },{ 97, 0, 97 },{ 105, 0, 105 },{ 113, 0, 113 },{ 121, 0, 121 },{ 130, 0, 130 },
	{ 129, 0, 130 },{ 127, 0, 130 },{ 125, 0, 130 },{ 123, 0, 131 },{ 121, 0, 131 },{ 119, 0, 131 },{ 117, 0, 132 },{ 115, 0, 132 },
	{ 114, 0, 132 },{ 112, 0, 133 },{ 110, 0, 133 },{ 108, 0, 133 },{ 106, 0, 134 },{ 104, 0, 134 },{ 102, 0, 134 },{ 100, 0, 135 },
	{ 98, 0, 135 },{ 95, 0, 135 },{ 93, 0, 135 },{ 90, 0, 136 },{ 88, 0, 136 },{ 85, 0, 136 },{ 83, 0, 137 },{ 80, 0, 137 },{ 78, 0, 137 },
	{ 75, 0, 138 },{ 73, 0, 138 },{ 70, 0, 138 },{ 68, 0, 139 },{ 65, 0, 139 },{ 63, 0, 139 },{ 60, 0, 140 },{ 59, 0, 140 },{ 57, 0, 140 },
	{ 55, 0, 140 },{ 53, 0, 141 },{ 51, 0, 141 },{ 49, 0, 141 },{ 47, 0, 142 },{ 45, 0, 142 },{ 44, 0, 142 },{ 42, 0, 143 },{ 40, 0, 143 },
	{ 38, 0, 143 },{ 36, 0, 144 },{ 34, 0, 144 },{ 32, 0, 144 },{ 30, 0, 145 },{ 29, 0, 145 },{ 27, 0, 145 },{ 25, 0, 145 },{ 23, 0, 146 },
	{ 21, 0, 146 },{ 19, 0, 146 },{ 17, 0, 147 },{ 15, 0, 147 },{ 14, 0, 147 },{ 12, 0, 148 },{ 10, 0, 148 },{ 8, 0, 148 },{ 6, 0, 149 },
	{ 4, 0, 149 },{ 2, 0, 149 },{ 0, 0, 150 },{ 0, 5, 150 },{ 0, 11, 151 },{ 0, 16, 152 },{ 0, 22, 153 },{ 0, 28, 154 },{ 0, 33, 155 },
	{ 0, 39, 156 },{ 0, 45, 157 },{ 0, 50, 158 },{ 0, 56, 159 },{ 0, 61, 160 },{ 0, 67, 161 },{ 0, 73, 162 },{ 0, 78, 163 },{ 0, 84, 164 },
	{ 0, 90, 165 },{ 0, 95, 165 },{ 0, 101, 166 },{ 0, 106, 167 },{ 0, 112, 168 },{ 0, 118, 169 },{ 0, 123, 170 },{ 0, 129, 171 },
	{ 0, 135, 172 },{ 0, 140, 173 },{ 0, 146, 174 },{ 0, 151, 175 },{ 0, 157, 176 },{ 0, 163, 177 },{ 0, 168, 178 },{ 0, 174, 179 },
	{ 0, 180, 180 },{ 0, 180, 175 },{ 0, 181, 170 },{ 0, 181, 165 },{ 0, 182, 160 },{ 0, 183, 155 },{ 0, 183, 150 },{ 0, 184, 145 },
	{ 0, 185, 140 },{ 0, 185, 135 },{ 0, 186, 130 },{ 0, 186, 125 },{ 0, 187, 120 },{ 0, 188, 115 },{ 0, 188, 110 },{ 0, 189, 105 },
	{ 0, 190, 100 },{ 0, 190, 99 },{ 0, 191, 97 },{ 0, 191, 96 },{ 0, 192, 94 },{ 0, 193, 93 },{ 0, 193, 91 },{ 0, 194, 90 },
	{ 0, 195, 88 },{ 0, 195, 86 },{ 0, 196, 85 },{ 0, 196, 83 },{ 0, 197, 82 },{ 0, 198, 80 },{ 0, 198, 79 },{ 0, 199, 77 },{ 0, 200, 75 },
	{ 8, 201, 74 },{ 16, 203, 72 },{ 24, 205, 71 },{ 32, 207, 69 },{ 40, 209, 68 },{ 48, 211, 66 },{ 56, 213, 65 },{ 65, 215, 63 },
	{ 73, 216, 61 },{ 81, 218, 60 },{ 89, 220, 58 },{ 97, 222, 57 },{ 105, 224, 55 },{ 113, 226, 54 },{ 121, 228, 52 },{ 130, 230, 50 },
	{ 137, 231, 50 },{ 145, 233, 50 },{ 153, 234, 50 },{ 161, 236, 50 },{ 169, 237, 50 },{ 176, 239, 50 },{ 184, 240, 50 },{ 192, 242, 50 },
	{ 200, 244, 50 },{ 208, 245, 50 },{ 215, 247, 50 },{ 223, 248, 50 },{ 231, 250, 50 },{ 239, 251, 50 },{ 247, 253, 50 },{ 255, 255, 50 },
	{ 255, 252, 50 },{ 255, 249, 50 },{ 255, 245, 50 },{ 255, 242, 50 },{ 255, 238, 50 },{ 255, 235, 50 },{ 255, 231, 50 },{ 255, 228, 50 },
	{ 255, 225, 50 },{ 255, 221, 50 },{ 255, 218, 50 },{ 255, 214, 50 },{ 255, 211, 50 },{ 255, 207, 50 },{ 255, 204, 50 },{ 255, 200, 50 },
	{ 255, 197, 50 },{ 255, 194, 50 },{ 255, 191, 50 },{ 255, 188, 50 },{ 255, 185, 50 },{ 255, 182, 50 },{ 255, 179, 50 },{ 255, 175, 50 },
	{ 255, 172, 50 },{ 255, 169, 50 },{ 255, 166, 50 },{ 255, 163, 50 },{ 255, 160, 50 },{ 255, 157, 50 },{ 255, 154, 50 },{ 255, 150, 50 },
	{ 255, 147, 50 },{ 255, 144, 50 },{ 255, 141, 50 },{ 255, 138, 50 },{ 255, 135, 50 },{ 255, 132, 50 },{ 255, 129, 50 },{ 255, 125, 50 },
	{ 255, 122, 50 },{ 255, 119, 50 },{ 255, 116, 50 },{ 255, 113, 50 },{ 255, 110, 50 },{ 255, 107, 50 },{ 255, 104, 50 },{ 255, 100, 50 },
	{ 255, 97, 50 },{ 255, 94, 50 },{ 255, 91, 50 },{ 255, 88, 50 },{ 255, 85, 50 },{ 255, 82, 50 },{ 255, 79, 50 },{ 255, 75, 50 },
	{ 255, 72, 50 },{ 255, 69, 50 },{ 255, 66, 50 },{ 255, 63, 50 },{ 255, 60, 50 },{ 255, 57, 50 },{ 255, 54, 50 },{ 255, 50, 50 },
	{ 246, 50, 50 },{ 236, 50, 50 },{ 226, 50, 50 },{ 217, 50, 50 },{ 207, 50, 50 },{ 197, 50, 50 },{ 188, 50, 50 },{ 178, 50, 50 },
	{ 168, 50, 50 },{ 159, 50, 50 },{ 149, 50, 50 },{ 139, 50, 50 },{ 130, 50, 50 },{ 120, 50, 50 },{ 110, 50, 50 } };
	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[255-i][2] & 0xff, temp[255-i][1] & 0xff, temp[255-i][0] & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutMedical = lut.clone();
}


void SetPurpleColorTable() {
	int temp[256][3] = {
		{ 255, 254, 251 },{ 255, 253, 245 },{ 255, 252, 239 },{ 255, 251, 233 },
		{ 255, 250, 226 },{ 255, 249, 218 },{ 255, 248, 210 },{ 255, 246, 201 },
		{ 255, 245, 192 },{ 255, 244, 182 },{ 255, 242, 172 },{ 255, 241, 163 },
		{ 255, 239, 154 },{ 255, 238, 144 },{ 255, 236, 134 },{ 255, 234, 124 },
		{ 255, 232, 114 },{ 255, 230, 104 },{ 255, 229, 95 },{ 255, 227, 86 },
		{ 255, 225, 78 },{ 255, 223, 71 },{ 255, 220, 64 },{ 255, 218, 57 },
		{ 255, 216, 51 },{ 255, 213, 46 },{ 255, 211, 41 },{ 255, 209, 40 },
		{ 255, 208, 40 },{ 255, 207, 40 },{ 255, 205, 40 },{ 255, 204, 40 },
		{ 255, 202, 40 },{ 255, 201, 40 },{ 255, 199, 40 },{ 255, 198, 40 },
		{ 255, 196, 40 },{ 255, 194, 40 },{ 255, 193, 40 },{ 255, 191, 40 },
		{ 255, 189, 40 },{ 255, 187, 40 },{ 255, 186, 40 },{ 255, 184, 40 },
		{ 255, 182, 40 },{ 255, 180, 40 },{ 255, 179, 40 },{ 255, 177, 40 },
		{ 255, 175, 40 },{ 255, 173, 40 },{ 255, 171, 40 },{ 255, 169, 40 },
		{ 255, 167, 40 },{ 255, 166, 40 },{ 255, 164, 40 },{ 255, 162, 40 },
		{ 255, 160, 40 },{ 255, 158, 40 },{ 255, 156, 40 },{ 255, 154, 41 },
		{ 255, 152, 42 },{ 255, 150, 43 },{ 255, 149, 44 },{ 255, 147, 46 },
		{ 255, 145, 47 },{ 255, 143, 48 },{ 255, 141, 49 },{ 255, 139, 51 },
		{ 255, 138, 52 },{ 255, 136, 53 },{ 255, 134, 54 },{ 255, 133, 55 },
		{ 255, 131, 56 },{ 255, 129, 57 },{ 255, 127, 59 },{ 255, 126, 60 },
		{ 255, 125, 60 },{ 255, 123, 62 },{ 255, 122, 62 },{ 255, 120, 63 },
		{ 255, 119, 64 },{ 254, 117, 66 },{ 254, 114, 66 },{ 254, 112, 68 },
		{ 253, 110, 69 },{ 253, 108, 70 },{ 253, 106, 72 },{ 252, 104, 73 },
		{ 252, 102, 74 },{ 251, 100, 76 },{ 251, 98, 77 },{ 250, 96, 78 },
		{ 250, 94, 80 },{ 250, 92, 81 },{ 249, 90, 83 },{ 248, 88, 84 },
		{ 248, 86, 86 },{ 247, 84, 87 },{ 246, 82, 89 },{ 246, 80, 91 },
		{ 245, 79, 92 },{ 244, 77, 94 },{ 244, 75, 96 },{ 242, 74, 97 },
		{ 242, 72, 99 },{ 241, 71, 101 },{ 240, 70, 103 },{ 239, 68, 105 },
		{ 237, 67, 106 },{ 236, 66, 108 },{ 235, 65, 110 },{ 233, 64, 112 },
		{ 232, 63, 114 },{ 230, 62, 116 },{ 229, 62, 118 },{ 227, 61, 120 },
		{ 226, 61, 122 },{ 224, 61, 125 },{ 222, 61, 127 },{ 219, 61, 129 },
		{ 217, 61, 132 },{ 215, 62, 134 },{ 212, 62, 137 },{ 210, 63, 140 },
		{ 207, 64, 142 },{ 205, 65, 145 },{ 202, 66, 148 },{ 199, 68, 151 },
		{ 196, 69, 154 },{ 193, 71, 157 },{ 190, 72, 161 },{ 187, 73, 164 },
		{ 184, 75, 167 },{ 181, 77, 170 },{ 178, 78, 173 },{ 174, 80, 176 },
		{ 171, 82, 179 },{ 168, 83, 182 },{ 164, 85, 185 },{ 161, 86, 188 },
		{ 158, 88, 191 },{ 155, 89, 193 },{ 152, 90, 196 },{ 149, 92, 199 },
		{ 146, 93, 201 },{ 143, 94, 203 },{ 139, 95, 205 },{ 137, 95, 208 },
		{ 134, 96, 210 },{ 131, 96, 212 },{ 129, 96, 213 },{ 126, 96, 215 },
		{ 124, 96, 216 },{ 121, 96, 217 },{ 120, 96, 217 },{ 119, 95, 217 },
		{ 117, 94, 217 },{ 116, 94, 217 },{ 115, 94, 217 },{ 113, 93, 217 },
		{ 112, 92, 217 },{ 110, 91, 217 },{ 109, 91, 217 },{ 108, 90, 217 },
		{ 107, 89, 217 },{ 105, 88, 217 },{ 104, 87, 217 },{ 103, 86, 217 },
		{ 102, 85, 217 },{ 100, 83, 217 },{ 99, 82, 217 },{ 98, 81, 217 },
		{ 97, 80, 217 },{ 95, 79, 217 },{ 94, 77, 217 },{ 93, 76, 217 },
		{ 92, 75, 217 },{ 91, 73, 217 },{ 90, 72, 217 },{ 89, 70, 217 },
		{ 87, 69, 217 },{ 86, 68, 217 },{ 85, 66, 217 },{ 84, 65, 217 },
		{ 83, 63, 217 },{ 82, 62, 217 },{ 81, 60, 217 },{ 80, 59, 217 },
		{ 79, 57, 217 },{ 78, 56, 217 },{ 77, 54, 217 },{ 76, 53, 217 },
		{ 74, 51, 217 },{ 74, 50, 217 },{ 73, 48, 217 },{ 72, 47, 217 },
		{ 71, 46, 217 },{ 70, 44, 217 },{ 69, 43, 217 },{ 68, 42, 217 },
		{ 67, 40, 217 },{ 66, 39, 217 },{ 65, 38, 217 },{ 64, 36, 217 },
		{ 63, 35, 217 },{ 62, 34, 217 },{ 61, 33, 216 },{ 60, 32, 214 },
		{ 59, 31, 213 },{ 58, 30, 211 },{ 57, 29, 210 },{ 56, 28, 208 },
		{ 55, 27, 206 },{ 54, 26, 203 },{ 53, 25, 200 },{ 52, 24, 197 },
		{ 51, 23, 193 },{ 50, 22, 190 },{ 48, 22, 185 },{ 47, 21, 181 },
		{ 46, 20, 177 },{ 45, 19, 172 },{ 44, 18, 168 },{ 43, 17, 163 },
		{ 42, 17, 157 },{ 41, 16, 152 },{ 39, 15, 147 },{ 39, 14, 141 },
		{ 37, 13, 136 },{ 37, 13, 130 },{ 36, 12, 124 },{ 35, 11, 119 },
		{ 34, 11, 113 },{ 32, 10, 107 },{ 32, 10, 101 },{ 31, 9, 96 },
		{ 30, 9, 90 },{ 29, 8, 84 },{ 28, 7, 78 },{ 27, 7, 73 },
		{ 26, 6, 67 },{ 26, 6, 62 },{ 25, 5, 56 },{ 24, 5, 51 },
		{ 23, 4, 46 },{ 22, 4, 41 },{ 22, 4, 36 },{ 21, 3, 32 },
		{ 20, 3, 27 },{ 20, 2, 23 },{ 19, 2, 19 },{ 19, 2, 15 },
		{ 18, 1, 11 },{ 17, 1, 8 },{ 17, 1, 5 },{ 16, 0, 2 },
	};

	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[i][2] & 0xff, temp[i][1] & 0xff, temp[i][0] & 0xff);
		lut.at<Vec3b>(0, 255 - i) = repl;
	}
	m_lutPurple = lut.clone();
}

void SetPurpleYellowColorTable() {
	int temp[256][3] = {
		{ 57, 0, 128 },{ 58, 1, 128 },{ 58, 1, 127 },{ 58, 1, 127 },
		{ 59, 2, 127 },{ 59, 2, 127 },{ 60, 3, 127 },{ 60, 3, 127 },
		{ 61, 4, 126 },{ 61, 5, 126 },{ 62, 5, 126 },{ 62, 6, 126 },
		{ 63, 6, 125 },{ 64, 7, 125 },{ 64, 8, 125 },{ 65, 9, 125 },
		{ 65, 9, 124 },{ 66, 10, 124 },{ 67, 10, 124 },{ 67, 11, 124 },
		{ 68, 12, 123 },{ 69, 12, 123 },{ 69, 13, 123 },{ 70, 14, 122 },
		{ 71, 15, 122 },{ 71, 16, 122 },{ 72, 16, 122 },{ 73, 17, 121 },
		{ 73, 18, 121 },{ 74, 18, 120 },{ 75, 19, 120 },{ 76, 20, 120 },
		{ 77, 21, 120 },{ 77, 22, 119 },{ 78, 22, 119 },{ 79, 23, 119 },
		{ 80, 24, 119 },{ 81, 25, 118 },{ 82, 26, 118 },{ 82, 27, 117 },
		{ 83, 28, 117 },{ 84, 29, 117 },{ 85, 29, 116 },{ 86, 30, 116 },
		{ 87, 31, 116 },{ 87, 32, 115 },{ 88, 33, 115 },{ 89, 34, 115 },
		{ 90, 35, 114 },{ 91, 36, 114 },{ 92, 37, 114 },{ 93, 38, 113 },
		{ 94, 39, 113 },{ 95, 40, 112 },{ 96, 41, 112 },{ 96, 42, 112 },
		{ 97, 43, 111 },{ 98, 44, 111 },{ 99, 45, 111 },{ 100, 46, 110 },
		{ 101, 47, 109 },{ 102, 48, 109 },{ 103, 49, 109 },{ 104, 50, 108 },
		{ 105, 51, 108 },{ 106, 52, 107 },{ 107, 53, 107 },{ 108, 54, 107 },
		{ 109, 55, 106 },{ 109, 56, 106 },{ 111, 57, 106 },{ 112, 58, 105 },
		{ 113, 59, 105 },{ 114, 60, 104 },{ 115, 61, 104 },{ 116, 62, 103 },
		{ 117, 63, 103 },{ 118, 64, 102 },{ 119, 66, 102 },{ 119, 67, 102 },
		{ 120, 68, 101 },{ 121, 69, 101 },{ 122, 70, 100 },{ 123, 71, 100 },
		{ 124, 72, 100 },{ 125, 73, 99 },{ 126, 74, 98 },{ 127, 75, 98 },
		{ 128, 76, 98 },{ 129, 77, 97 },{ 130, 78, 97 },{ 131, 80, 96 },
		{ 132, 81, 96 },{ 133, 82, 95 },{ 134, 83, 95 },{ 135, 84, 94 },
		{ 136, 85, 94 },{ 137, 86, 93 },{ 138, 87, 93 },{ 139, 88, 92 },
		{ 140, 89, 92 },{ 141, 91, 91 },{ 141, 92, 91 },{ 143, 92, 90 },
		{ 143, 94, 90 },{ 144, 95, 90 },{ 145, 96, 89 },{ 146, 97, 88 },
		{ 147, 98, 88 },{ 148, 99, 88 },{ 149, 100, 87 },{ 150, 101, 87 },
		{ 151, 102, 86 },{ 151, 103, 86 },{ 152, 104, 85 },{ 153, 105, 85 },
		{ 154, 106, 84 },{ 155, 107, 84 },{ 156, 108, 83 },{ 157, 109, 83 },
		{ 157, 110, 82 },{ 158, 112, 82 },{ 159, 112, 81 },{ 160, 114, 81 },
		{ 160, 115, 80 },{ 161, 116, 80 },{ 162, 117, 79 },{ 163, 117, 79 },
		{ 163, 119, 78 },{ 165, 120, 78 },{ 165, 121, 77 },{ 166, 122, 77 },
		{ 167, 123, 76 },{ 168, 124, 75 },{ 168, 126, 74 },{ 170, 127, 74 },
		{ 170, 128, 73 },{ 171, 129, 72 },{ 172, 130, 72 },{ 173, 131, 71 },
		{ 174, 133, 70 },{ 175, 134, 69 },{ 176, 135, 68 },{ 176, 136, 67 },
		{ 177, 138, 66 },{ 178, 139, 65 },{ 179, 140, 64 },{ 180, 141, 63 },
		{ 181, 143, 63 },{ 182, 144, 62 },{ 182, 146, 60 },{ 183, 147, 59 },
		{ 184, 148, 58 },{ 185, 149, 57 },{ 186, 151, 56 },{ 187, 152, 55 },
		{ 188, 153, 54 },{ 189, 155, 53 },{ 190, 156, 52 },{ 190, 158, 51 },
		{ 191, 159, 49 },{ 192, 160, 49 },{ 193, 162, 47 },{ 194, 163, 46 },
		{ 195, 164, 45 },{ 196, 165, 44 },{ 197, 167, 43 },{ 197, 168, 42 },
		{ 198, 170, 40 },{ 199, 171, 39 },{ 200, 172, 38 },{ 201, 174, 37 },
		{ 202, 175, 36 },{ 202, 177, 35 },{ 203, 178, 34 },{ 204, 180, 32 },
		{ 205, 181, 31 },{ 206, 182, 30 },{ 207, 183, 29 },{ 208, 185, 28 },
		{ 209, 186, 27 },{ 209, 187, 26 },{ 210, 189, 25 },{ 211, 190, 23 },
		{ 212, 192, 23 },{ 213, 193, 21 },{ 213, 195, 20 },{ 214, 196, 19 },
		{ 215, 197, 18 },{ 216, 198, 17 },{ 217, 200, 16 },{ 218, 201, 15 },
		{ 218, 202, 14 },{ 219, 204, 13 },{ 220, 205, 12 },{ 221, 206, 11 },
		{ 221, 208, 10 },{ 222, 209, 9 },{ 223, 210, 8 },{ 224, 211, 8 },
		{ 224, 213, 7 },{ 225, 214, 6 },{ 226, 215, 5 },{ 227, 216, 4 },
		{ 227, 218, 4 },{ 228, 219, 3 },{ 229, 220, 2 },{ 229, 221, 1 },
		{ 230, 222, 1 },{ 231, 223, 0 },{ 232, 225, 0 },{ 232, 226, 0 },
		{ 233, 227, 0 },{ 233, 228, 0 },{ 234, 229, 0 },{ 235, 230, 0 },
		{ 236, 231, 0 },{ 236, 232, 0 },{ 237, 233, 0 },{ 237, 234, 0 },
		{ 238, 235, 0 },{ 239, 236, 0 },{ 239, 238, 0 },{ 240, 238, 0 },
		{ 240, 240, 0 },{ 241, 240, 0 },{ 241, 241, 0 },{ 242, 242, 0 },
		{ 242, 243, 0 },{ 243, 244, 0 },{ 244, 245, 0 },{ 244, 246, 0 },
		{ 245, 246, 0 },{ 245, 247, 0 },{ 246, 248, 0 },{ 246, 249, 0 },
		{ 247, 250, 0 },{ 247, 251, 0 },{ 248, 251, 0 },{ 248, 252, 0 },
		{ 248, 252, 0 },{ 249, 253, 0 },{ 249, 254, 0 },{ 249, 254, 0 },
		{ 250, 255, 0 },{ 250, 255, 9 },{ 250, 255, 24 },{ 250, 255, 44 },
		{ 250, 255, 68 },{ 250, 255, 94 },{ 250, 255, 121 },{ 250, 255, 149 },
		{ 250, 255, 175 },{ 250, 255, 200 },{ 250, 253, 223 },{ 250, 252, 241 }
	};

	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[i][2] & 0xff, temp[i][1] & 0xff, temp[i][0] & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutPurpleYellow = lut.clone();
}


void SetDarkBlueColorTable() {
	int temp[256][3] = {
		{ 18, 255, 253 },{ 18, 253, 253 },{ 19, 251, 253 },{ 19, 248, 253 },
		{ 20, 246, 252 },{ 20, 243, 252 },{ 20, 240, 252 },{ 21, 236, 252 },
		{ 22, 233, 252 },{ 22, 229, 252 },{ 23, 225, 252 },{ 23, 221, 252 },
		{ 24, 217, 251 },{ 25, 213, 251 },{ 25, 209, 251 },{ 26, 205, 251 },
		{ 27, 200, 251 },{ 27, 196, 251 },{ 28, 191, 250 },{ 29, 187, 250 },
		{ 30, 183, 250 },{ 30, 178, 250 },{ 31, 173, 249 },{ 32, 169, 249 },
		{ 32, 165, 249 },{ 33, 160, 248 },{ 34, 156, 248 },{ 35, 152, 248 },
		{ 35, 148, 247 },{ 36, 144, 246 },{ 37, 141, 246 },{ 37, 137, 245 },
		{ 38, 134, 245 },{ 38, 131, 244 },{ 39, 128, 243 },{ 40, 125, 243 },
		{ 40, 122, 242 },{ 41, 120, 241 },{ 41, 117, 240 },{ 42, 115, 239 },
		{ 42, 113, 238 },{ 43, 111, 237 },{ 43, 109, 236 },{ 43, 108, 234 },
		{ 44, 106, 233 },{ 44, 104, 232 },{ 45, 103, 230 },{ 45, 101, 229 },
		{ 45, 100, 227 },{ 46, 99, 226 },{ 46, 98, 225 },{ 47, 97, 223 },
		{ 47, 95, 222 },{ 48, 94, 220 },{ 48, 94, 218 },{ 48, 92, 217 },
		{ 49, 91, 216 },{ 49, 90, 214 },{ 49, 89, 212 },{ 50, 89, 211 },
		{ 50, 88, 209 },{ 51, 87, 208 },{ 51, 86, 206 },{ 52, 85, 205 },
		{ 53, 84, 203 },{ 53, 82, 202 },{ 54, 81, 200 },{ 55, 80, 199 },
		{ 55, 79, 198 },{ 56, 77, 197 },{ 56, 76, 196 },{ 57, 75, 194 },
		{ 57, 73, 194 },{ 58, 72, 193 },{ 59, 70, 192 },{ 60, 69, 191 },
		{ 60, 68, 190 },{ 61, 66, 189 },{ 62, 65, 189 },{ 62, 63, 187 },
		{ 63, 62, 187 },{ 64, 60, 186 },{ 65, 59, 185 },{ 66, 57, 184 },
		{ 66, 56, 183 },{ 67, 54, 183 },{ 68, 53, 182 },{ 69, 51, 181 },
		{ 70, 50, 180 },{ 71, 48, 180 },{ 71, 46, 179 },{ 72, 45, 178 },
		{ 73, 44, 177 },{ 74, 42, 177 },{ 75, 41, 176 },{ 75, 39, 175 },
		{ 77, 38, 174 },{ 77, 36, 173 },{ 78, 35, 173 },{ 79, 34, 172 },
		{ 80, 32, 171 },{ 81, 31, 170 },{ 82, 30, 169 },{ 83, 28, 169 },
		{ 84, 27, 168 },{ 85, 26, 167 },{ 86, 25, 166 },{ 87, 24, 165 },
		{ 88, 23, 164 },{ 89, 22, 163 },{ 90, 21, 162 },{ 91, 20, 161 },
		{ 92, 19, 160 },{ 93, 19, 160 },{ 94, 18, 159 },{ 95, 17, 158 },
		{ 96, 16, 157 },{ 96, 15, 156 },{ 97, 14, 155 },{ 98, 14, 154 },
		{ 99, 13, 153 },{ 100, 12, 153 },{ 101, 11, 152 },{ 102, 11, 151 },
		{ 103, 10, 150 },{ 104, 9, 149 },{ 105, 9, 148 },{ 106, 8, 147 },
		{ 107, 7, 146 },{ 108, 7, 146 },{ 109, 6, 145 },{ 110, 5, 144 },
		{ 111, 5, 143 },{ 112, 4, 142 },{ 113, 4, 141 },{ 114, 4, 140 },
		{ 115, 4, 139 },{ 116, 4, 138 },{ 118, 4, 137 },{ 119, 4, 136 },
		{ 120, 4, 134 },{ 121, 4, 133 },{ 122, 4, 132 },{ 123, 4, 131 },
		{ 124, 4, 130 },{ 126, 4, 129 },{ 127, 4, 127 },{ 128, 4, 126 },
		{ 129, 4, 125 },{ 130, 4, 123 },{ 131, 4, 122 },{ 133, 4, 121 },
		{ 134, 4, 119 },{ 135, 4, 118 },{ 136, 4, 117 },{ 137, 4, 115 },
		{ 138, 5, 114 },{ 140, 5, 112 },{ 141, 6, 111 },{ 142, 6, 109 },
		{ 143, 7, 108 },{ 144, 7, 106 },{ 145, 8, 105 },{ 147, 8, 103 },
		{ 148, 9, 101 },{ 149, 10, 100 },{ 151, 10, 98 },{ 152, 11, 96 },
		{ 153, 12, 95 },{ 154, 12, 93 },{ 155, 13, 91 },{ 156, 14, 89 },
		{ 158, 15, 87 },{ 159, 15, 85 },{ 161, 16, 84 },{ 162, 17, 82 },
		{ 163, 18, 80 },{ 165, 19, 78 },{ 166, 20, 76 },{ 167, 21, 74 },
		{ 168, 22, 72 },{ 170, 23, 71 },{ 171, 24, 69 },{ 172, 25, 67 },
		{ 174, 27, 65 },{ 175, 28, 63 },{ 176, 29, 61 },{ 178, 31, 59 },
		{ 179, 32, 58 },{ 180, 33, 56 },{ 181, 35, 54 },{ 183, 36, 52 },
		{ 184, 38, 51 },{ 185, 39, 49 },{ 186, 41, 47 },{ 188, 43, 46 },
		{ 189, 45, 44 },{ 190, 47, 42 },{ 191, 49, 41 },{ 193, 51, 39 },
		{ 194, 53, 38 },{ 195, 55, 37 },{ 196, 57, 35 },{ 197, 59, 34 },
		{ 198, 61, 33 },{ 199, 63, 32 },{ 201, 66, 31 },{ 201, 68, 29 },
		{ 203, 71, 28 },{ 204, 74, 27 },{ 205, 78, 26 },{ 206, 81, 25 },
		{ 207, 84, 24 },{ 208, 88, 23 },{ 209, 91, 22 },{ 211, 95, 21 },
		{ 212, 99, 20 },{ 213, 103, 20 },{ 214, 107, 19 },{ 215, 111, 18 },
		{ 216, 115, 17 },{ 217, 120, 17 },{ 218, 124, 16 },{ 219, 128, 15 },
		{ 220, 133, 14 },{ 221, 137, 14 },{ 223, 142, 13 },{ 224, 146, 12 },
		{ 225, 151, 12 },{ 226, 156, 11 },{ 227, 160, 10 },{ 228, 165, 10 },
		{ 229, 169, 10 },{ 230, 174, 9 },{ 231, 178, 8 },{ 232, 183, 8 },
		{ 233, 187, 7 },{ 234, 191, 7 },{ 234, 196, 7 },{ 235, 200, 6 },
		{ 236, 204, 6 },{ 237, 208, 5 },{ 238, 212, 5 },{ 239, 216, 4 },
		{ 239, 220, 4 },{ 240, 224, 4 },{ 241, 228, 3 },{ 242, 231, 3 },
		{ 242, 234, 3 },{ 243, 237, 2 },{ 244, 240, 2 },{ 244, 243, 2 },
		{ 245, 246, 1 },{ 246, 248, 1 },{ 246, 251, 1 },{ 247, 253, 0 },
	};

	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[i][2] & 0xff, temp[i][1] & 0xff, temp[i][0] & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutDarkBlue = lut.clone();
}

void SetCyanColorTable() {
	int temp[256][3] = {
		{ 0, 0, 130 },{ 0, 0, 131 },{ 0, 0, 132 },{ 0, 0, 134 },
		{ 0, 0, 135 },{ 0, 0, 137 },{ 0, 0, 139 },{ 0, 0, 141 },
		{ 0, 0, 143 },{ 0, 0, 145 },{ 0, 0, 148 },{ 0, 0, 150 },
		{ 0, 0, 152 },{ 0, 0, 155 },{ 0, 0, 157 },{ 0, 0, 160 },
		{ 0, 0, 163 },{ 0, 0, 165 },{ 0, 0, 168 },{ 0, 0, 170 },
		{ 0, 0, 173 },{ 0, 0, 176 },{ 0, 0, 178 },{ 0, 0, 181 },
		{ 0, 0, 183 },{ 0, 0, 186 },{ 0, 0, 188 },{ 0, 1, 191 },
		{ 0, 2, 193 },{ 0, 3, 196 },{ 0, 5, 199 },{ 0, 6, 202 },
		{ 0, 7, 205 },{ 0, 9, 208 },{ 0, 10, 211 },{ 0, 11, 214 },
		{ 0, 13, 217 },{ 0, 14, 220 },{ 0, 16, 223 },{ 0, 18, 225 },
		{ 0, 20, 228 },{ 0, 22, 231 },{ 0, 24, 234 },{ 0, 26, 237 },
		{ 0, 28, 239 },{ 0, 30, 242 },{ 0, 32, 244 },{ 0, 35, 246 },
		{ 0, 37, 248 },{ 0, 40, 250 },{ 0, 42, 252 },{ 0, 45, 253 },
		{ 0, 48, 254 },{ 0, 51, 254 },{ 0, 54, 253 },{ 0, 57, 254 },
		{ 0, 60, 254 },{ 0, 64, 254 },{ 0, 67, 254 },{ 0, 71, 254 },
		{ 0, 74, 254 },{ 0, 78, 254 },{ 0, 82, 254 },{ 0, 85, 254 },
		{ 0, 89, 254 },{ 0, 93, 254 },{ 0, 97, 254 },{ 0, 101, 254 },
		{ 0, 105, 254 },{ 0, 109, 254 },{ 0, 113, 254 },{ 0, 117, 254 },
		{ 0, 121, 254 },{ 0, 125, 254 },{ 0, 129, 254 },{ 0, 133, 254 },
		{ 0, 137, 254 },{ 0, 141, 254 },{ 0, 145, 254 },{ 0, 150, 254 },
		{ 0, 154, 254 },{ 0, 159, 254 },{ 0, 164, 254 },{ 0, 168, 254 },
		{ 0, 173, 254 },{ 0, 178, 254 },{ 0, 183, 254 },{ 0, 188, 254 },
		{ 0, 193, 254 },{ 0, 198, 254 },{ 0, 203, 254 },{ 0, 208, 253 },
		{ 0, 212, 254 },{ 0, 217, 254 },{ 0, 221, 254 },{ 0, 226, 254 },
		{ 0, 230, 254 },{ 0, 234, 254 },{ 0, 237, 254 },{ 0, 241, 254 },
		{ 0, 244, 254 },{ 0, 247, 254 },{ 0, 249, 253 },{ 1, 251, 251 },
		{ 3, 253, 249 },{ 6, 254, 247 },{ 8, 254, 244 },{ 11, 254, 241 },
		{ 14, 254, 238 },{ 17, 254, 235 },{ 20, 254, 231 },{ 24, 254, 228 },
		{ 27, 254, 224 },{ 31, 254, 220 },{ 35, 254, 216 },{ 39, 254, 212 },
		{ 43, 254, 208 },{ 47, 254, 204 },{ 51, 254, 200 },{ 55, 254, 195 },
		{ 59, 254, 192 },{ 64, 254, 187 },{ 68, 254, 183 },{ 72, 254, 178 },
		{ 76, 254, 174 },{ 80, 254, 170 },{ 84, 254, 166 },{ 88, 254, 162 },
		{ 92, 254, 158 },{ 96, 254, 154 },{ 100, 254, 150 },{ 104, 255, 146 },
		{ 109, 254, 142 },{ 113, 255, 138 },{ 117, 255, 133 },{ 122, 255, 129 },
		{ 126, 255, 124 },{ 130, 255, 120 },{ 135, 255, 116 },{ 139, 255, 111 },
		{ 144, 255, 107 },{ 148, 255, 103 },{ 153, 255, 98 },{ 157, 255, 94 },
		{ 161, 255, 89 },{ 166, 255, 85 },{ 170, 255, 81 },{ 174, 255, 77 },
		{ 178, 255, 73 },{ 182, 255, 70 },{ 185, 255, 66 },{ 189, 255, 62 },
		{ 193, 255, 59 },{ 196, 255, 56 },{ 199, 254, 53 },{ 202, 253, 49 },
		{ 205, 252, 47 },{ 208, 251, 44 },{ 211, 250, 41 },{ 214, 249, 38 },
		{ 216, 247, 36 },{ 219, 246, 33 },{ 221, 245, 31 },{ 224, 243, 28 },
		{ 226, 242, 25 },{ 229, 240, 23 },{ 231, 238, 21 },{ 233, 236, 19 },
		{ 235, 235, 17 },{ 237, 233, 15 },{ 239, 231, 13 },{ 241, 228, 12 },
		{ 243, 226, 10 },{ 244, 224, 8 },{ 246, 221, 7 },{ 247, 218, 5 },
		{ 249, 216, 4 },{ 250, 213, 3 },{ 251, 210, 1 },{ 252, 207, 0 },
		{ 253, 204, 0 },{ 255, 201, 0 },{ 255, 197, 0 },{ 255, 193, 0 },
		{ 255, 190, 0 },{ 255, 186, 0 },{ 255, 182, 0 },{ 255, 177, 0 },
		{ 255, 173, 0 },{ 255, 169, 0 },{ 254, 164, 0 },{ 255, 159, 0 },
		{ 255, 155, 0 },{ 255, 150, 0 },{ 255, 146, 0 },{ 255, 141, 0 },
		{ 255, 137, 0 },{ 255, 132, 0 },{ 255, 127, 0 },{ 255, 122, 0 },
		{ 255, 118, 0 },{ 255, 114, 0 },{ 255, 109, 0 },{ 255, 105, 0 },
		{ 255, 100, 0 },{ 255, 97, 0 },{ 255, 93, 0 },{ 255, 88, 0 },
		{ 255, 84, 0 },{ 255, 80, 0 },{ 254, 75, 0 },{ 255, 71, 0 },
		{ 255, 67, 0 },{ 255, 62, 0 },{ 255, 58, 0 },{ 255, 53, 0 },
		{ 255, 49, 0 },{ 255, 45, 0 },{ 255, 41, 0 },{ 255, 37, 0 },
		{ 255, 33, 0 },{ 255, 29, 0 },{ 255, 25, 0 },{ 254, 21, 0 },
		{ 254, 18, 0 },{ 253, 15, 0 },{ 252, 12, 0 },{ 251, 9, 0 },
		{ 250, 6, 0 },{ 248, 4, 0 },{ 247, 2, 0 },{ 245, 1, 0 },
		{ 242, 1, 0 },{ 240, 1, 0 },{ 237, 1, 0 },{ 234, 1, 0 },
		{ 230, 1, 0 },{ 227, 1, 0 },{ 223, 1, 0 },{ 219, 1, 0 },
		{ 215, 1, 0 },{ 211, 0, 0 },{ 206, 1, 0 },{ 202, 1, 0 },
		{ 198, 1, 0 },{ 194, 1, 0 },{ 189, 1, 0 },{ 185, 1, 0 },
		{ 181, 1, 0 },{ 178, 1, 0 },{ 174, 0, 0 },{ 170, 1, 0 },
		{ 167, 1, 0 },{ 164, 1, 0 },{ 162, 1, 0 },{ 159, 1, 0 }
	};

	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[i][2] & 0xff, temp[i][1] & 0xff, temp[i][0] & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutCyan = lut.clone();
}

void SetRainbowColorTalbe() {
	int temp[256][3] = {
		{ 0, 5, 76 },{ 0, 6, 79 },{ 1, 8, 81 },{ 0, 10, 85 },
		{ 0, 12, 88 },{ 0, 14, 92 },{ 0, 16, 97 },{ 1, 18, 101 },
		{ 0, 21, 106 },{ 1, 23, 111 },{ 1, 26, 117 },{ 1, 29, 122 },
		{ 0, 32, 128 },{ 0, 35, 134 },{ 0, 38, 139 },{ 0, 41, 145 },
		{ 0, 44, 151 },{ 0, 47, 156 },{ 0, 50, 162 },{ 0, 53, 167 },
		{ 0, 56, 173 },{ 0, 60, 178 },{ 0, 63, 182 },{ 1, 66, 187 },
		{ 0, 69, 191 },{ 0, 72, 195 },{ 0, 75, 198 },{ 0, 78, 201 },
		{ 0, 81, 204 },{ 0, 84, 206 },{ 1, 87, 207 },{ 1, 90, 208 },
		{ 2, 92, 208 },{ 3, 94, 208 },{ 4, 96, 208 },{ 4, 98, 207 },
		{ 5, 100, 206 },{ 5, 102, 204 },{ 6, 104, 203 },{ 7, 106, 201 },
		{ 7, 107, 199 },{ 8, 109, 196 },{ 9, 112, 194 },{ 10, 113, 191 },
		{ 10, 115, 188 },{ 11, 117, 185 },{ 12, 119, 181 },{ 12, 121, 178 },
		{ 13, 123, 174 },{ 14, 125, 171 },{ 15, 127, 167 },{ 16, 129, 163 },
		{ 17, 130, 159 },{ 18, 132, 155 },{ 19, 134, 150 },{ 20, 136, 146 },
		{ 21, 137, 142 },{ 23, 139, 138 },{ 24, 141, 133 },{ 25, 143, 129 },
		{ 27, 145, 125 },{ 28, 146, 120 },{ 29, 148, 116 },{ 31, 150, 112 },
		{ 33, 152, 108 },{ 34, 153, 103 },{ 36, 155, 100 },{ 38, 156, 96 },
		{ 40, 158, 92 },{ 42, 159, 88 },{ 44, 161, 85 },{ 46, 162, 81 },
		{ 48, 164, 78 },{ 50, 165, 75 },{ 53, 167, 72 },{ 55, 168, 69 },
		{ 58, 169, 67 },{ 61, 171, 65 },{ 64, 172, 62 },{ 67, 174, 60 },
		{ 71, 175, 58 },{ 74, 177, 55 },{ 78, 178, 53 },{ 82, 180, 51 },
		{ 86, 181, 49 },{ 90, 183, 47 },{ 95, 184, 45 },{ 100, 186, 43 },
		{ 104, 187, 41 },{ 109, 189, 39 },{ 114, 190, 37 },{ 119, 192, 35 },
		{ 124, 193, 34 },{ 129, 194, 32 },{ 134, 196, 31 },{ 139, 197, 29 },
		{ 144, 198, 27 },{ 149, 200, 26 },{ 154, 201, 24 },{ 159, 202, 23 },
		{ 164, 203, 22 },{ 169, 204, 20 },{ 174, 205, 19 },{ 179, 206, 18 },
		{ 184, 207, 16 },{ 189, 208, 15 },{ 193, 208, 15 },{ 198, 209, 13 },
		{ 202, 210, 12 },{ 206, 210, 11 },{ 210, 210, 11 },{ 214, 210, 10 },
		{ 218, 210, 9 },{ 222, 210, 8 },{ 225, 210, 7 },{ 228, 210, 7 },
		{ 231, 210, 6 },{ 234, 210, 6 },{ 237, 210, 5 },{ 239, 210, 5 },
		{ 241, 209, 5 },{ 243, 209, 5 },{ 245, 208, 5 },{ 247, 207, 5 },
		{ 248, 205, 5 },{ 249, 204, 5 },{ 251, 203, 5 },{ 252, 202, 5 },
		{ 253, 200, 5 },{ 254, 199, 5 },{ 254, 197, 6 },{ 254, 195, 6 },
		{ 254, 193, 7 },{ 254, 191, 7 },{ 254, 189, 8 },{ 254, 187, 8 },
		{ 254, 185, 9 },{ 254, 183, 10 },{ 254, 181, 11 },{ 254, 179, 11 },
		{ 254, 176, 12 },{ 254, 174, 13 },{ 254, 172, 14 },{ 254, 169, 15 },
		{ 254, 167, 16 },{ 254, 165, 17 },{ 254, 162, 18 },{ 254, 160, 19 },
		{ 254, 157, 20 },{ 254, 155, 21 },{ 254, 152, 21 },{ 254, 150, 22 },
		{ 254, 147, 23 },{ 254, 145, 24 },{ 254, 142, 25 },{ 254, 140, 26 },
		{ 254, 138, 27 },{ 254, 135, 28 },{ 254, 133, 29 },{ 254, 130, 29 },
		{ 254, 128, 30 },{ 254, 126, 31 },{ 254, 123, 31 },{ 254, 120, 32 },
		{ 254, 117, 33 },{ 254, 114, 34 },{ 254, 110, 35 },{ 254, 107, 35 },
		{ 254, 103, 36 },{ 254, 100, 37 },{ 254, 96, 37 },{ 254, 93, 38 },
		{ 254, 89, 38 },{ 254, 85, 39 },{ 254, 82, 40 },{ 254, 78, 41 },
		{ 253, 74, 41 },{ 253, 71, 42 },{ 253, 67, 43 },{ 253, 64, 44 },
		{ 253, 60, 45 },{ 253, 57, 46 },{ 252, 54, 47 },{ 252, 51, 47 },
		{ 252, 48, 48 },{ 252, 45, 49 },{ 251, 42, 50 },{ 251, 40, 52 },
		{ 251, 37, 53 },{ 251, 35, 54 },{ 251, 33, 55 },{ 251, 31, 57 },
		{ 250, 30, 58 },{ 250, 28, 59 },{ 250, 27, 61 },{ 250, 26, 62 },
		{ 250, 25, 64 },{ 250, 25, 66 },{ 250, 25, 67 },{ 250, 26, 69 },
		{ 249, 26, 71 },{ 249, 27, 73 },{ 249, 28, 75 },{ 249, 29, 77 },
		{ 249, 31, 80 },{ 249, 33, 82 },{ 249, 35, 85 },{ 249, 37, 87 },
		{ 249, 40, 90 },{ 249, 43, 92 },{ 249, 46, 95 },{ 249, 49, 98 },
		{ 248, 52, 101 },{ 248, 56, 103 },{ 248, 59, 106 },{ 248, 63, 109 },
		{ 248, 67, 112 },{ 248, 70, 115 },{ 248, 74, 118 },{ 248, 78, 121 },
		{ 248, 82, 124 },{ 248, 86, 127 },{ 247, 90, 130 },{ 247, 94, 133 },
		{ 247, 98, 135 },{ 247, 102, 138 },{ 247, 107, 141 },{ 247, 111, 144 },
		{ 247, 114, 147 },{ 247, 118, 150 },{ 247, 122, 153 },{ 247, 126, 156 },
		{ 247, 129, 158 },{ 247, 133, 161 },{ 247, 136, 163 },{ 247, 139, 166 },
		{ 247, 144, 171 },{ 247, 150, 175 },{ 247, 156, 180 },{ 247, 162, 185 },
		{ 247, 168, 190 },{ 248, 174, 195 },{ 248, 180, 200 },{ 248, 187, 205 },
		{ 248, 193, 209 },{ 248, 199, 214 },{ 248, 205, 219 },{ 249, 211, 224 },
		{ 249, 217, 228 },{ 249, 222, 232 },{ 249, 227, 236 },{ 249, 232, 240 },
		{ 250, 237, 244 },{ 250, 241, 247 },{ 250, 245, 250 },{ 250, 248, 253 }

	};
	Mat lut(1, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		Vec3b repl = Vec3b(temp[i][2] & 0xff, temp[i][1] & 0xff, temp[i][0] & 0xff);
		lut.at<Vec3b>(0, i) = repl;
	}
	m_lutRainbow = lut.clone();
}
#endif