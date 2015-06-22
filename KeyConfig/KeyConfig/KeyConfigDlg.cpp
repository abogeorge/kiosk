
// KeyConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyConfig.h"
#include "KeyConfigDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// letters char array is used to compare the received data from dialog with actual values
char letters[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeyConfigDlg dialog



CKeyConfigDlg::CKeyConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKeyConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comboKeyS);
	DDX_Control(pDX, IDC_COMBO2, comboKeyE);
	DDX_Control(pDX, IDC_EDITBROWSE, editBrwose);
}

BEGIN_MESSAGE_MAP(CKeyConfigDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CKeyConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BROWSE, &CKeyConfigDlg::OnBnClickedBrowse)
END_MESSAGE_MAP()


// CKeyConfigDlg message handlers

BOOL CKeyConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeyConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyConfigDlg::OnPaint()
{

	/// initializing the dialog combobox selections with the last configured keys
	FileUtils file;
	char keySwitch = file.readConfigSwitch();
	char keyExit = file.readConfigExit();

	for (int i = 0; i < 26; i++){
		if (letters[i] == keySwitch)
			comboKeyS.SetCurSel(i);
		if (letters[i] == keyExit)
			comboKeyE.SetCurSel(i);
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/// The system calls this function when OK is pressed in the dialog box
void CKeyConfigDlg::OnBnClickedOk()
{

	int sIndex = comboKeyS.GetCurSel();
	int eIndex = comboKeyE.GetCurSel();

	if (letters[sIndex] == letters[eIndex]){
		AfxMessageBox(_T("Provided keys cannot match! Please choose other keys."));
	}
	else {
		FileUtils fileU;
		RegistryUtilities registryU;
		fileU.writeConfig(letters[sIndex], letters[eIndex]);
		registryU.writeKeys(sIndex, eIndex);
		exit(0);
	}

}


void CKeyConfigDlg::OnBnClickedBrowse()
{
	LPCTSTR szFilters = L"Executable files (*.exe)|*.exe";
	CFileDialog fileDlg(TRUE, L"exe", L"*.exe", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this, 0, TRUE);

	if (fileDlg.DoModal() == IDOK)
	{
		CString m_strPathname = fileDlg.GetPathName();
		editBrwose.SetWindowText(m_strPathname);
		FileUtils fileU;
		fileU.writeApplicationExe(m_strPathname);
	}
}