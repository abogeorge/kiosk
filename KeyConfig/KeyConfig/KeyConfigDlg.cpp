
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

/// keepAppsArray char array is used to compare the received data from dialog with actual values
char keepAppsArray[2] = { '0', '1'};

/// the path of executable
CString m_strPathname;

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
	DDX_Control(pDX, IDC_CHECK1, checkExplorer);
	DDX_Control(pDX, IDC_BROWSE, buttonBrowse);
	DDX_Control(pDX, IDC_CHECK2, checkKeepRuning);
	DDX_Control(pDX, IDC_CHECK3, checkDeactivateSwitch);
}

BEGIN_MESSAGE_MAP(CKeyConfigDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CKeyConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BROWSE, &CKeyConfigDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_CHECK1, &CKeyConfigDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CKeyConfigDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CKeyConfigDlg::OnBnClickedCheck3)
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
	char keepApp = file.readConfigKeepApp();
	char deactivateSwitch = file.readConfigDeactivateSwitch();

	if (keepApp == '1')
	{
		checkKeepRuning.SetCheck(1);
	}
	else
	{
		checkKeepRuning.SetCheck(0);
	}

	if (deactivateSwitch == '1')
	{
		checkDeactivateSwitch.SetCheck(1);
	}
	else
	{
		checkDeactivateSwitch.SetCheck(0);
	}

	for (int i = 0; i < 26; i++){
		if (letters[i] == keySwitch)
			comboKeyS.SetCurSel(i);
		if (letters[i] == keyExit)
			comboKeyE.SetCurSel(i);
	}

	ifstream in("application.conf", ios::in);
	char cApp[100];
	in.getline(cApp, 100);

	//char* cApp = file.readAppConfig();
	size_t len = mbstowcs(nullptr, &cApp[0], 0);
	wstring wstr(len, 0);
	mbstowcs(&wstr[0], &cApp[0], wstr.size());
	const wchar_t * cs = wstr.c_str();
	LPCTSTR appLoaded = cs;

	if (wcscmp(appLoaded, L"#") == 0)
	{
		/// initializing check box
		checkExplorer.SetCheck(1);

		/// disabling browse button
		CWnd* pfield = GetDlgItem(IDC_BROWSE);
		pfield->EnableWindow(FALSE);
	}
	else
	{
		editBrwose.SetReadOnly(false);
		editBrwose.SetWindowTextW(appLoaded);
		m_strPathname = appLoaded;
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
	int checked = checkExplorer.GetCheck();
	int checkedKeppApp = checkKeepRuning.GetCheck();
	int checkedDeactivateSwitch = checkDeactivateSwitch.GetCheck();

	FileUtils fileU;
	RegistryUtilities registryU;

	switch (checked)
	{
	case 0:
	{
		fileU.writeApplicationExe(m_strPathname);
		break;
	}
	case 1:{
		fileU.writeApplicationEmpty();
		break;
	}
	default:
		break;
	}

	if (letters[sIndex] == letters[eIndex]){
		AfxMessageBox(_T("Provided keys cannot match! Please choose other keys."));
	}
	else {
		fileU.writeConfig(letters[sIndex], letters[eIndex], keepAppsArray[checkedKeppApp], keepAppsArray[checkedDeactivateSwitch]);
		registryU.writeKeys(sIndex, eIndex, checkedKeppApp, checkedDeactivateSwitch);
		exit(0);
	}

}


void CKeyConfigDlg::OnBnClickedBrowse()
{
	LPCTSTR szFilters = L"Executable files (*.exe)|*.exe";
	CFileDialog fileDlg(TRUE, L"exe", L"*.exe", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this, 0, TRUE);

	if (fileDlg.DoModal() == IDOK)
	{
		m_strPathname = fileDlg.GetPathName();
		editBrwose.SetWindowText(m_strPathname);
	}
}

void CKeyConfigDlg::OnBnClickedCheck1()
{
	CWnd* pfield = GetDlgItem(IDC_BROWSE);
	if (checkExplorer.GetCheck() == 0){
		editBrwose.SetReadOnly(false);
		pfield->EnableWindow(TRUE);
	}
	else{
		editBrwose.SetReadOnly(true);
		pfield->EnableWindow(FALSE);
	}
}


void CKeyConfigDlg::OnBnClickedCheck2()
{
	int checkVal = checkKeepRuning.GetCheck();
	switch (checkVal){
	case 0:{
		AfxMessageBox(_T("All processes opened in the new Desktop will be closed when the Kiosk application exits."));
		break;
	}
	case 1:{
		AfxMessageBox(_T("All processes opened in the new Desktop will be available at a new Kiosk application run."));
		break;
	}
	default:
		break;
	}
}


void CKeyConfigDlg::OnBnClickedCheck3()
{
	int checkVal = checkDeactivateSwitch.GetCheck();
	switch (checkVal){
	case 0:{
		AfxMessageBox(_T("When the Kiosk application will start, the user will have the capability to switch back and forth between Desktops."));
		break;
	}
	case 1:{
		AfxMessageBox(_T("When the Kiosk application will start, the user will not be able to switch back and forth between Desktops. When the exit combo will be detected, the user will be returned to the original Desktop."));
		break;
	}
	default:
		break;
	}
}
