
// MFCApplication1Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
//#include "Gdiplus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMFCApplication1Dlg

float  CMFCApplication1Dlg::Test(cv::Mat &image) {
	
	
	static const auto  Model = fdeep::load_model("my_model.json");
	

	cv::Mat image2;
	resize(image, image2, cv::Size(32, 32), cv::INTER_LINEAR);
	cv::cvtColor(image2, image2, cv::COLOR_BGR2RGB);
	assert(image2.isContinuous());
	// Use the correct scaling, i.e., low and high.
	const auto input = fdeep::tensor_from_bytes(image2.ptr(),
		static_cast<std::size_t>(image2.rows),
		static_cast<std::size_t>(image2.cols),
		static_cast<std::size_t>(image2.channels()),
		0.0f, 1.0f);

	const auto result = Model.predict_single_output({ input });
	//std::cout << (result) << std::endl;

	return result;
}


CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, CheckStatus(_T("")){
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDCheckStatus, CheckStatus);
	DDX_Control(pDX, IDImage, ImageOutElement);
	DDX_Control(pDX, IDStartCam, CamBtn);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOpenFDlg, &CMFCApplication1Dlg::OnBnClickedOpenfdlg)
	ON_BN_CLICKED(IDStartCam, &CMFCApplication1Dlg::OnBnClickedStartcam)
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplication1Dlg

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedOpenfdlg(){
	
	CFileDialog dlg(true);

	// Display the Open dialog box. 

	if (dlg.DoModal() == IDOK){
		//CheckStatus = dlg.m_ofn.lpstrFile;
		LPWSTR lp = dlg.m_ofn.lpstrFile;
		size_t FileNameBufferSize = 500;
		size_t read = 0;
		char FileNameBuffer[500];

		// First arg is the pointer to destination char, second arg is
		// the pointer to source wchar_t, last arg is the size of char buffer
		
		wcstombs_s(&read, FileNameBuffer, lp,FileNameBufferSize);
		//wcstombs_s(FileNameBuffer, lp, 500);
		float result;
		try {
			
		cv::Mat image = cv::imread(FileNameBuffer);
		if (image.data == NULL) { return; }
		
		result = CMFCApplication1Dlg::Test(image);
		
		resize(image, image, cv::Size(256, 256), cv::INTER_LINEAR);
		cv::Size size = image.size();
		Gdiplus::Bitmap bitmap(size.width, size.height, image.step1(), PixelFormat24bppRGB, image.data);
		HBITMAP IBM = NULL;
		bitmap.GetHBITMAP(0, &IBM);
		
		ImageOutElement.SetBitmap(IBM);
		//ImageOutElement.

		}catch (int a) {

		}
		;
		CheckStatus = "Status: ";
		
		if (result>0.3  ) { CheckStatus = CheckStatus + _T("No Mask"); }else {CheckStatus = CheckStatus + _T("Mask"); }
		//CString cstr(std::to_string(result).c_str());
		//CheckStatus = CheckStatus + cstr;
		//CheckStatus = CheckStatus + _T(" r = ")+ _T(result);
		 ;
		//CheckStatus = " "+ " ";
	}

	UpdateData(FALSE);
}


void CMFCApplication1Dlg::OnBnClickedStartcam(){
	//if (!AutoCheck) {
	cv::VideoCapture cap;// (0);
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
	// open selected camera using selected API
	cap.open(deviceID, apiID);

	if (!cap.isOpened()) {
		CheckStatus = "Status: Cam initialize failed";
		UpdateData(FALSE);
		return;
	}else {


		AutoCheck = true;
		CamBtn.SetWindowText(_T("Stop Auto Check"));
		float result;
		//cv::Mat image(480, 640, CV_8UC3);
		cv::Mat image;
		cv::Mat image2;
		//while(!cap.read(image));
		cap.read(image2);
		//cap >> image;
		cap.release();
		if (image2.empty()) {return;}
		//if (image.data == NULL) { return; }
		image=image2.clone();
		result = CMFCApplication1Dlg::Test(image);

		resize(image, image, cv::Size(256, 256), cv::INTER_LINEAR);
		cv::Size size = image.size();
		//cv::imshow("", image2);
		//PixelFormat32bppRGB

		//image.convertTo(image2, 4);
		Gdiplus::Bitmap bitmap(size.width, size.height, image.step1(), PixelFormat24bppRGB, image.data);
		
		//Gdiplus::Bitmap bitmap(size.width, size.height, image.step1(), PixelFormat24bppRGB, image.data);
		HBITMAP IBM = NULL;
		bitmap.GetHBITMAP(0, &IBM);

		ImageOutElement.SetBitmap(IBM);

		CheckStatus = "Status: ";

		if (result > 0.3) { CheckStatus = CheckStatus + _T("No Mask"); }
		else { CheckStatus = CheckStatus + _T("Mask"); }

		UpdateData(FALSE);

	}
	


	//}else {
	//	AutoCheck = false;
	//CamBtn.SetWindowText(_T("Start Auto Check"));
	//UpdateData(FALSE);

	//}
	
}
