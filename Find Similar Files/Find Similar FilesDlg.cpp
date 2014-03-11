
// Find Similar FilesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Find Similar Files.h"
#include "Find Similar FilesDlg.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace std::chrono;
using namespace SimilarFiles;

high_resolution_clock::time_point time_now()
{
	return high_resolution_clock::now();
}

double time_elapsed(high_resolution_clock::time_point const &start)
{
	return duration_cast<duration<double>>(time_now() - start).count();
}

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


// CFindSimilarFilesDlg dialog



CFindSimilarFilesDlg::CFindSimilarFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindSimilarFilesDlg::IDD, pParent)
{
	_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindSimilarFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_TARGETDIR, _directory);
	DDX_Control(pDX, ID_BUTTON_SEARCH, _btnSearch);
	DDX_Control(pDX, IDC_LIST_DUPLICATE_FILES, _lstDuplicates);
	DDX_Control(pDX, IDC_LIST_DUPLICATE_PATHS, _lstPaths);
	DDX_Control(pDX, IDC_EDIT_DISK_SPACE, _editDiskSpace);
	DDX_Control(pDX, IDC_EDIT_ELAPSED_TIME, _editElapsedTime);
	DDX_Control(pDX, IDC_EDIT_CURRENTDIRECTORY, _editCurrentDirectory);
}

BEGIN_MESSAGE_MAP(CFindSimilarFilesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUTTON_SEARCH, &CFindSimilarFilesDlg::OnBnClickedButtonSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DUPLICATE_FILES, &CFindSimilarFilesDlg::OnNMClickListDuplicateFiles)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DUPLICATE_PATHS, &CFindSimilarFilesDlg::OnNMClickListDuplicatePaths)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DUPLICATE_PATHS, &CFindSimilarFilesDlg::OnNMRClickListDuplicatePaths)
END_MESSAGE_MAP()


// CFindSimilarFilesDlg message handlers

BOOL CFindSimilarFilesDlg::OnInitDialog()
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
	SetIcon(_hIcon, TRUE);			// Set big icon
	SetIcon(_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	_directory.EnableFolderBrowseButton();
	CRect r;
	_lstDuplicates.GetClientRect(&r);
	_lstDuplicates.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	_lstDuplicates.InsertColumn(0, _T("#"), LVCFMT_LEFT, 40);
	_lstDuplicates.InsertColumn(1, _T("Filename"), LVCFMT_LEFT, r.Width() * 2 / 3 - 20);
	_lstDuplicates.InsertColumn(2, _T("Size (Bytes)"), LVCFMT_LEFT, r.Width() / 3 - 20);

	_lstPaths.GetClientRect(&r);
	_lstPaths.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	_lstPaths.InsertColumn(0, _T("Path"), LVCFMT_LEFT, r.Width() - 40);
	_lstPaths.InsertColumn(1, _T("-"), LVCFMT_LEFT, 40);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFindSimilarFilesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFindSimilarFilesDlg::OnPaint()
{
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
		dc.DrawIcon(x, y, _hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindSimilarFilesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(_hIcon);
}

bool CFindSimilarFilesDlg::OpenFile()
{
	HINSTANCE hInstance = ShellExecute(NULL, _T("open"), _wClickedFile.c_str(), NULL, NULL, SW_SHOW);
	return ((int)hInstance > 32);
}

bool CFindSimilarFilesDlg::RemoveFile()
{
	return DeleteFile(_wClickedFile.c_str()) != 0;
}

BOOL CFindSimilarFilesDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nCtrlId = LOWORD(wParam);

	if (HIWORD(wParam) == BN_CLICKED || HIWORD(wParam) == 0 /* Menu */)
	{
		switch (nCtrlId)
		{
		case MENU_OPEN:
			OpenFile();
			break;
		case MENU_DELETE:
			CString strTmp, strTitle, strMsg;
			strTmp.LoadString(IDS_DELETION_WARNING);
			strMsg.Format(_T("%s %s?"), strTmp, _wClickedFile.c_str());
			strTitle.LoadStringW(IDS_WARNING);
			if (MessageBox(strMsg, strTitle, MB_YESNO) == IDYES)
				AfxMessageBox(_T("File Removed"));
				//if (RemoveFile())
					//_lstPaths.DeleteItem();
			break;
		};
		_wClickedFile.clear();
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

void CFindSimilarFilesDlg::OnBnClickedButtonSearch()
{
	wstring wFolder;
	CString strFolder;
	_directory.GetWindowText(strFolder);
	if (!strFolder.GetLength())
		return;

	_btnSearch.EnableWindow(false);
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	_similarFilesManager.Clear();
	_lstDuplicates.DeleteAllItems();

	wFolder = strFolder.GetBuffer();
	double elapsed;
	if (wFolder.c_str())
	{
		auto const start = time_now();
		_similarFilesManager.SearchDuplicateFiles(wFolder);
		elapsed = time_elapsed(start);
	}

	int i = 0;
	wstring filename, filesize;
	VectorFiles vFiles = _similarFilesManager.GetVDuplicates();
	for (auto w : vFiles)
	{
		_similarFilesManager.GetFileInfo(w, filename, filesize);
		_lstDuplicates.InsertItem(LVIF_TEXT, i, L"", 0, LVIS_SELECTED, 0, 0);
		_lstDuplicates.SetItemText(i, 0, to_wstring(i+1).c_str());
		_lstDuplicates.SetItemText(i, 1, filename.c_str());
		_lstDuplicates.SetItemText(i++, 2, filesize.c_str());
	}

	CString tmp;
	tmp.Format(_T("%s MB"), to_wstring(_similarFilesManager.GetDiskSpaceOccupied()).c_str());
	_editDiskSpace.SetWindowTextW(tmp);
	wstring str = to_wstring(elapsed) + _T(" sec");
	_editElapsedTime.SetWindowTextW(str.c_str());
	_btnSearch.EnableWindow(true);
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CFindSimilarFilesDlg::OnNMClickListDuplicateFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	_lstPaths.DeleteAllItems();
	POINT point;
	GetCursorPos(&point);

	_lstDuplicates.ScreenToClient(&point);
	int index = _lstDuplicates.HitTest(point);
	if (index == -1)
		return;

	MapFileAttributes::iterator iter;
	CString s;

	s = _lstDuplicates.GetItemText(index, 1) + _T(":") + _lstDuplicates.GetItemText(index, 2);
	wstring wstr = s;
	VectorFiles vPaths = _similarFilesManager.GetFilePaths(wstr);
	if (!vPaths.size())
		return;

	_lstPaths.ClearEntries();
	int i = 0;
	CRect rect;
	CDC* pDC = _lstPaths.GetDC();
	for (auto w : vPaths)
	{
		_lstPaths.InsertItem(LVIF_TEXT, i, L"", 0, LVIS_SELECTED, 0, 0);
		_lstPaths.SetItemText(i++, 0, w.c_str());
		_lstPaths.GetSubItemRect(0, 1, LVIR_BOUNDS, rect);
		_lstPaths.AddEntry();
	}
}

void CFindSimilarFilesDlg::OnNMClickListDuplicatePaths(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	POINT point;
	GetCursorPos(&point);

	_lstPaths.ScreenToClient(&point);
	int index = _lstPaths.HitTest(point);
	if (index == -1)
		return;

	CRect rectSub0, rectSub1;
	_lstPaths.GetSubItemRect(index, 0, LVIR_BOUNDS, rectSub0);
	_lstPaths.GetSubItemRect(index, 1, LVIR_BOUNDS, rectSub1);
	if (point.x >= rectSub1.left && point.x <= rectSub1.right && point.y >= rectSub1.top && point.y <= rectSub1.bottom)
		_lstPaths.ToggleSelect(index);
	else if (point.x >= rectSub0.left && point.x <= rectSub0.right && point.y >= rectSub0.top && point.y <= rectSub0.bottom)
	{
	}
}

void CFindSimilarFilesDlg::ShowCurrentDirectorySearching(int n)
{
	//_editCurrentDirectory.SetWindowText(folder.c_str());
}

void CFindSimilarFilesDlg::OnNMRClickListDuplicatePaths(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	POINT point;
	GetCursorPos(&point);

	_lstPaths.ScreenToClient(&point);
	int index = _lstPaths.HitTest(point);
	if (index == -1)
		return;

	CRect rectSub1;
	_lstPaths.GetSubItemRect(index, 0, LVIR_BOUNDS, rectSub1);
	if (point.x < rectSub1.left || point.x > rectSub1.right || point.y < rectSub1.top || point.y > rectSub1.bottom)
		return;

	_wClickedFile = _lstPaths.GetItemText(index, 0);
	POSITION pos = _lstDuplicates.GetFirstSelectedItemPosition();
	if (pos)
	{
		int idx = _lstDuplicates.GetNextSelectedItem(pos);
		_wClickedFile += _T("\\") + _lstDuplicates.GetItemText(idx, 1);
	}
	else
	{
		MessageBox(_T("Error getting the filename."), _T("Error"), MB_ICONERROR | MB_OK);
	}

	CMenuEx menuCommand;
	menuCommand.CreatePopupMenu();

	CString strText;
	UINT flags(MF_STRING);
	strText.LoadString(IDS_MENUITEM_OPEN);
	menuCommand.InsertMenu(0, flags, MENU_OPEN, strText);
	strText.LoadString(IDS_MENUITEM_DELETE);
	menuCommand.InsertMenu(1, flags, MENU_DELETE, strText);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_DELETE);
	menuCommand.SetMenuItemBitmaps(1, MF_BYPOSITION, &bitmap, &bitmap);
	_lstPaths.ClientToScreen(&point);
	menuCommand.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}
