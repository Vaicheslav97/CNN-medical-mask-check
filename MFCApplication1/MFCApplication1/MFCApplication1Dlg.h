
// MFCApplication1Dlg.h: файл заголовка
//

#pragma once
#include <fdeep/fdeep.hpp>

// Диалоговое окно CMFCApplication1Dlg
class CMFCApplication1Dlg : public CDialogEx
{
// Создание
public:
	float Test(cv::Mat& image);
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// стандартный конструктор
	
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfdlg();
	CString CheckStatus;
	CStatic ImageOutElement;
	afx_msg void OnBnClickedStartcam();
	CButton CamBtn;
	bool AutoCheck = false;
};
