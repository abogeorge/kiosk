
// KeyConfigDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CKeyConfigDlg dialog
class CKeyConfigDlg : public CDialogEx
{
// Construction
public:
	CKeyConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KEYCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboKeyS;
	CComboBox comboKeyE;
	afx_msg void OnBnClickedOk();
};
