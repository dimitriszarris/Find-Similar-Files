
// Find Similar FilesDlg.h : header file
//

#pragma once
#include "afxeditbrowsectrl.h"
#include <string>
#include "SimilarFilesManager.h"
#include "ListCtrlEx.h"
#include "menuex.h"
#include "afxwin.h"

#define MENU_OPEN 201
#define MENU_DELETE 202


// CFindSimilarFilesDlg dialog
class CFindSimilarFilesDlg : public CDialogEx
{
	HICON _hIcon;
	CMFCEditBrowseCtrl _directory;
	CButton _btnSearch;
	SimilarFiles::CSimilarFilesManager _similarFilesManager;

	CListCtrlEx _lstDuplicates;
	CListCtrlEx _lstPaths;
	CEdit _editDiskSpace;
	CEdit _editElapsedTime;
	CEdit _editCurrentDirectory;

	std::wstring _wClickedFile;

	// Construction
public:
	CFindSimilarFilesDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_FINDSIMILARFILES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void ShowCurrentDirectorySearching(int n);

	// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonSearch();
	DECLARE_MESSAGE_MAP()

	bool OpenFile();
	bool RemoveFile();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnNMClickListDuplicateFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDuplicatePaths(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListDuplicatePaths(NMHDR *pNMHDR, LRESULT *pResult);
};
