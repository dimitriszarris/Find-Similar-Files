#pragma once

#include <vector>

#define UWM_SELECT WM_USER + 100


class CListCtrlEx :
	public CListCtrl
{
	DECLARE_MESSAGE_MAP()

	CPoint		_mousePos;
	COLORREF	_columnClrText;
	CFont		_stdFont;
	CFont		_underlineFont;
	std::vector<bool> _vectorSelected;

public:
	CListCtrlEx();
	~CListCtrlEx();

	void InitList(const DWORD &dwStylesEx);
	void InitListCtrlCols();
	void UninitializeList();

	CPoint GetMousePos() const;

	void DrawText(int nItem, int nSubItem, CDC *pDC, CString strItemText, COLORREF crText, bool underline, CRect& rect);
	void DrawIcon(int nItem, int nSubItem, CDC *pDC, HICON hIcon, CRect& rect);
	void DrawPicture(int nItem, int nSubItem, CDC *pDC, int image, CRect &rect);
	void DrawProgress(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect, int pos);
	void SetProgressPos(int index, int pos);
	void ToggleSelect(int index);
	void AddEntry() { _vectorSelected.push_back(false); }
	void ClearEntries() { _vectorSelected.clear(); }

protected:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnSelect(WPARAM wParam, LPARAM lParam);
};
