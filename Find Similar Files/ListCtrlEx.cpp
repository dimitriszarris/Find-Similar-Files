#include "stdafx.h"
#include "ListCtrlEx.h"
#include "afxtoolbarimages.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	ON_MESSAGE(UWM_SELECT, OnSelect)
END_MESSAGE_MAP()

CListCtrlEx::CListCtrlEx()
{
}


CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::InitList(const DWORD &dwStylesEx)
{
	SetExtendedStyle(dwStylesEx);

	// Setup the column headings
	InitListCtrlCols();


	LOGFONT lf;
	CFont* pFont = GetFont();
	if (!pFont)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
		if (hFont)
			pFont = CFont::FromHandle(hFont);
	}
	ASSERT(pFont->GetSafeHandle());

	pFont->GetLogFont(&lf);
	_stdFont.CreateFontIndirect(&lf);
	lf.lfUnderline = (BYTE)TRUE;
	_underlineFont.CreateFontIndirect(&lf);
}

void CListCtrlEx::InitListCtrlCols()
{
	CRect rect;
	GetClientRect(&rect);
}

void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	_mousePos = point;

	CListCtrl::OnRButtonDown(nFlags, point);
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&point);

	ScreenToClient(&point);
	int index = HitTest(point);
	if (index == -1)
		return;

	CRect rect;
	GetSubItemRect(index, 1, LVIR_BOUNDS, rect);
	if (/*pTarget->GetPreviewStatus().compare(VizCapture::LoadString(IDS_STRING_READY)) == */0)
	{
		if (point.x >= rect.left && point.x <= rect.right)
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		}
	}
	else if (/*pTarget->GetPreviewStatus().compare(VizCapture::LoadString(IDS_STRING_PENDING)) == */0)
	{
		if (point.x >= rect.left && point.x <= rect.right)
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CListCtrlEx::ToggleSelect(int index)
{
	if (index >= static_cast<int>(_vectorSelected.size()))
		return;
	_vectorSelected[index] = !_vectorSelected[index];
	SendMessage(UWM_SELECT, WPARAM(index));
}

LRESULT CListCtrlEx::OnSelect(WPARAM wParam, LPARAM lParam)
{
	int index = LOWORD(wParam);
	if (index >= static_cast<int>(_vectorSelected.size()))
		return 1;

	CRect rect;
	GetSubItemRect(index, 1, LVIR_BOUNDS, rect);
	CDC *pDC = GetDC();
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(_vectorSelected[index] ? IDI_ICON_SELECTED : IDI_ICON_UNSELECTED), IMAGE_ICON, 48, 48, 0);
	DrawIcon(index, 1, pDC, hIcon, rect);
	return 0;
}

void CListCtrlEx::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		//pTarget = m_repository->GetTargetAt(pLVCD->nmcd.dwItemSpec);
		//olib::t_string str = pTarget->GetPreviewStatus();
		//m_strPreviewStatus = str.c_str();
		//if (pTarget)
		//{
		//	if (m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_PENDING))
		//		_columnClrText = RGB(0, 0, 0);
		//	else if (m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_READY))
		//		_columnClrText = RGB(0, 0, 255);
		//	else if (m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_UNAVAILABLE) || m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_NOT_INSTALLED))
		//		_columnClrText = RGB(255, 0, 0);
		//	else if (m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_CANCELED))
		//		_columnClrText = RGB(255, 0, 0);
		//	else
		//		_columnClrText = RGB(0, 0, 0);
		//}

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		if (1 == pLVCD->iSubItem)
		{
			pLVCD->clrText = _columnClrText;

			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CRect rect;
			GetSubItemRect(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, LVIR_BOUNDS, rect);
			if (pLVCD->nmcd.dwItemSpec >= static_cast<DWORD_PTR>(_vectorSelected.size()))
				return;
			HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(_vectorSelected[pLVCD->nmcd.dwItemSpec] ? IDI_ICON_SELECTED : IDI_ICON_UNSELECTED), IMAGE_ICON, 48, 48, 0);
			DrawIcon(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, hIcon, rect);
			//HICON hIconEnabled, hIconDisabled, hIcon;
			//hIconEnabled = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_PRECUT), IMAGE_ICON, 48, 48, 0);
			//hIconDisabled = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DISABLED_PRECUT), IMAGE_ICON, 48, 48, 0);
			//if (m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_READY))
			//	hIcon = hIconEnabled;
			//else
			//	hIcon = hIconDisabled;
			//if (IsIngest())
			//{
			//	DrawIcon(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, hIcon, rect);
			//	rect.DeflateRect(16, 0);
			//	DrawText(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, m_strPreviewStatus, _columnClrText, (hIcon == hIconEnabled), rect);
			//}
			//else
			//{
			//	std::map<int, int>::iterator iter = m_mapProgressPercentage.find(pLVCD->nmcd.dwItemSpec);
			//	DrawProgress(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, RGB(0, 0, 0), RGB(0, 255, 0), rect, iter->second);
			//}
		}

		*pResult = CDRF_NOTIFYPOSTPAINT;
	}
	else if ((CDDS_ITEMPOSTPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, LVIR_BOUNDS, rect);
		if (1 == pLVCD->iSubItem)
		{
			if (pLVCD->nmcd.dwItemSpec >= static_cast<DWORD_PTR>(_vectorSelected.size()))
				return;
			HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(_vectorSelected[pLVCD->nmcd.dwItemSpec] ? IDI_ICON_SELECTED : IDI_ICON_UNSELECTED), IMAGE_ICON, 48, 48, 0);
			DrawIcon(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, hIcon, rect);
			//HICON hIconEnabled, hIconDisabled, hIcon;
			//hIconEnabled = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_PRECUT), IMAGE_ICON, 48, 48, 0);
			//hIconDisabled = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DISABLED_PRECUT), IMAGE_ICON, 48, 48, 0);
			//if (m_strPreviewStatus == VizCapture::LoadString(IDS_STRING_READY))
			//	hIcon = hIconEnabled;
			//else
			//	hIcon = hIconDisabled;
			//if (IsIngest())
			//{
			//	DrawIcon(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, hIcon, rect);
			//	rect.DeflateRect(16, 0);
			//	DrawText(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, m_strPreviewStatus, _columnClrText, (hIcon == hIconEnabled), rect);
			//}
			//else
			//{
			//	std::map<int, int>::iterator iter = m_mapProgressPercentage.find(pLVCD->nmcd.dwItemSpec);
			//	DrawProgress(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pDC, RGB(0, 0, 0), RGB(0, 255, 0), rect, iter->second);
			//}
		}

		*pResult = CDRF_SKIPDEFAULT;
	}
}

void CListCtrlEx::DrawText(int nItem, int nSubItem, CDC *pDC, CString strItemText, COLORREF crText, bool underline, CRect& rect)
{
	ASSERT(pDC);

	if (rect.IsRectEmpty())
		return;

	if (!strItemText.IsEmpty())
	{
		UINT nFormat = DT_LEFT;

		CFont *pOldFont = NULL;
		CFont underlinefont;

		CFont *font = pDC->GetCurrentFont();
		if (font)
		{
			LOGFONT lf;
			font->GetLogFont(&lf);
			lf.lfUnderline = (BYTE)underline;
			underlinefont.CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(&underlinefont);
		}
		pDC->SetTextColor(crText);
		rect.DeflateRect(2, 2);
		pDC->DrawText(strItemText, &rect, nFormat);
		rect.InflateRect(2, 2);
		if (pOldFont)
			pDC->SelectObject(pOldFont);
	}
}

void CListCtrlEx::DrawIcon(int nItem, int nSubItem, CDC *pDC, HICON hIcon, CRect& rect)
{
	ASSERT(pDC);

	if (rect.IsRectEmpty())
		return;

	CDC dc;
	CPoint p;
	HDC hDC = pDC->GetSafeHdc();
	if (hIcon)
		DrawIconEx(hDC, rect.left, rect.top, hIcon, 16, 16, NULL, NULL, DI_IMAGE);
}

void CListCtrlEx::DrawPicture(int nItem, int nSubItem, CDC *pDC, int uResource, CRect &rect)
{
	CBitmap bitmap;
	CStatic picture;

	if (!picture.GetSafeHwnd())
	{
		picture.Create(_T(""), SS_BITMAP | WS_VISIBLE | WS_CHILD, rect, this);
		CPngImage image;
		image.Load(uResource, AfxGetInstanceHandle());
		bitmap.Attach(image.Detach());
		picture.SetBitmap(bitmap);
	}
}

void CListCtrlEx::DrawProgress(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect, int pos)
{
	UNUSED_ALWAYS(nItem);

	ASSERT(pDC);

	if (rect.IsRectEmpty())
		return;

	rect.bottom -= 1;
	rect.left += 1;		// leave margin in case row is highlighted

	// fill interior with light gray
	pDC->FillSolidRect(rect, RGB(240, 240, 240));

	// draw border
	pDC->Draw3dRect(&rect, RGB(0, 0, 0), ::GetSysColor(COLOR_BTNSHADOW));

	if (pos > 0)
	{
		// draw progress bar and text

		CRect LeftRect, RightRect;
		LeftRect = rect;
		LeftRect.left += 1;
		LeftRect.top += 1;
		LeftRect.bottom -= 1;
		RightRect = LeftRect;
		int percentage = LeftRect.Width() * pos / 100;
		LeftRect.right = LeftRect.left + percentage - 1;
		RightRect.left = LeftRect.right;
		pDC->FillSolidRect(LeftRect, crBkgnd);

		CString str, format;
		str.Format(_T("%d%%"), pos);

		pDC->SetBkMode(TRANSPARENT);

		CRect TextRect;
		TextRect = rect;
		TextRect.DeflateRect(1, 1);

		CRgn rgn;
		rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
		pDC->SelectClipRgn(&rgn);
		pDC->SetTextColor(crText);
		pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		rgn.DeleteObject();
		rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
		pDC->SelectClipRgn(&rgn);
		pDC->SetTextColor(crText);
		pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rgn.DeleteObject();
		pDC->SelectClipRgn(NULL);
	}
}

void CListCtrlEx::SetProgressPos(int index, int pos)
{
	CRect rect;
	GetSubItemRect(index, 4, LVIR_BOUNDS, rect);
	CDC *pDC = GetDC();
	//m_mapProgressPercentage.insert(std::pair<int, int>(index, pos));
	DrawProgress(index, 4, pDC, RGB(0, 0, 0), RGB(0, 255, 0), rect, pos);
	InvalidateRect(rect, TRUE);
}

