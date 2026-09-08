//
// Copyright © 2025, 2026 Ivyware Pty Ltd, Khrustal & Mann
//              MELBOURNE, VICTORIA, AUSTRALIA, 3000
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
//
//  CMFCRibbonDateCtrl  extensions, ptototypes and definitions
//  NOTES:
//

#include "stdafx.h"

#include "Msgexception.h"
#include "MFCRibbonDateCtrl.h"


//  Constructors and Destructor
IMPLEMENT_DYNAMIC(CCalendarPopup, CWnd)
CCalendarPopup::CCalendarPopup ( CMFCRibbonDatePickerControl *pOwner )
              : m_pOwner(pOwner)
{
    m_pOwner -> m_bPopupVisible = TRUE;
}

CCalendarPopup::~CCalendarPopup()
{
}

bool
CCalendarPopup::CreatePopup(CWnd* pParentFrame, const CPoint& ptScreen)
{
    if ( !pParentFrame                            ||
         !::IsWindow(pParentFrame->GetSafeHwnd())    )
        return false;

    // create tiny popup first; we'll resize below
    CRect rcInit(ptScreen.x, ptScreen.y, ptScreen.x + 8, ptScreen.y + 8);

    if (!CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
                  AfxRegisterWndClass(CS_SAVEBITS),
                  _T("CalendarPopup"),
                  WS_POPUP | WS_BORDER,
                  rcInit,
                  pParentFrame,
                  0))
    {
        return false;
    }

    // create calendar as child at (0,0) relative to popup
    if (!m_wndCal.Create(WS_CHILD | WS_VISIBLE | MCS_NOTODAY, CRect(0,0,0,0), this, 1))
    {
        DestroyWindow();
        return false;
    }

    // initialize calendar selection from owner (if present)
    if (m_pOwner)
    {
        SYSTEMTIME st{};
        COleDateTime dt = m_pOwner->GetDate();
CString strDate = dt.Format(VAR_DATEVALUEONLY); //TODO:LJM debugging only
        if (dt.GetStatus() == COleDateTime::valid)
            dt.GetAsSystemTime(st);
        else
            COleDateTime::GetCurrentTime().GetAsSystemTime(st);
        m_wndCal.SetCurSel(&st);
    }

    // Make sure the control is realized before asking size
    m_wndCal.ShowWindow(SW_SHOW);
    m_wndCal.UpdateWindow();

    // Ask calendar for required size (MCM_GETMINREQRECT)
    RECT rcMin{};
    m_wndCal.SendMessage(MCM_GETMINREQRECT, 0, (LPARAM)&rcMin);
    int nWidth  = rcMin.right - rcMin.left;
    int nHeight = rcMin.bottom - rcMin.top;

    // Safety fallback
    if (nWidth < 100)  nWidth  = 220;
    if (nHeight < 100) nHeight = 160;

    // Resize popup to fit, position at ptScreen (screen coords)
    SetWindowPos(&wndTopMost, ptScreen.x, ptScreen.y, nWidth, nHeight,
                 SWP_NOACTIVATE | SWP_SHOWWINDOW);

    // Place calendar at 0,0 inside popup and size to fill
    m_wndCal.MoveWindow(0, 0, nWidth, nHeight);

    // Activate calendar so keyboard nav works
    m_wndCal.SetFocus();

    // We do NOT call SetCapture here (optional); PreTranslateMessage will catch outside clicks.
    return true;
}

//  MFC message map for CCalendarPopup
BEGIN_MESSAGE_MAP(CCalendarPopup, CWnd)
    ON_WM_KILLFOCUS()
    ON_WM_NCDESTROY()
    ON_NOTIFY(MCN_SELECT,1, OnCalSelect)     // user selects a date (click)
END_MESSAGE_MAP()

//  MCN_SELECT handler
//  NOTES: Called when user clicks a date (MCN_SELECT) — update owner and close
//
//  Parameters:  pNMHDR[IN] - notification header
//
//               pResult[OUT] - result code
void
CCalendarPopup::OnCalSelect(NMHDR* pNMHDR, LRESULT* pResult)
{
    if ( pNMHDR && m_pOwner )
    {
      LPNMSELCHANGE pSel = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
      if (pSel)
      {
        const COleDateTime dt(pSel->stSelStart);
        m_pOwner->m_bPopupVisible = FALSE;
        m_pOwner->SetDate(dt);
        m_pOwner->m_bPopupVisible = TRUE;

        // Optionally notify app
        // NOTES: We post a command message to main window with the
        //        ID of this date picker control as wParam.
        if ( CWnd *pMain = AfxGetMainWnd() )
          pMain -> SendMessage ( WM_COMMAND, MAKEWPARAM(m_pOwner->GetID(), 0), 0);
      }
    }

    // close popup
    DestroyWindow();
    if (pResult) *pResult = 0;
    return;
}

//  If focus moves outside popup, close — but ignore focus shifts to child
//  controls inside popup
void
CCalendarPopup::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    if (pNewWnd != nullptr)
    {
        HWND hNew = pNewWnd->GetSafeHwnd();
        if (::IsWindow(hNew))
        {
            // if new window is our calendar (or child), keep open
            if (hNew == m_wndCal.GetSafeHwnd() || ::IsChild(m_wndCal.GetSafeHwnd(), hNew))
                return;
        }
    }

    // otherwise close
    DestroyWindow();
}

//  Called when destroyed — self delete if necessary
//
void
CCalendarPopup::OnNcDestroy()
{
    CWnd::OnNcDestroy();
    m_pOwner -> m_bPopupVisible = FALSE;
    delete this; // self delete pattern
}

// Catch mouse clicks anywhere (global-ish) and close when click falls outside popup.
// PreTranslateMessage receives mouse messages with screen coords in pMsg->pt
BOOL
CCalendarPopup::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_LBUTTONDOWN ||
        pMsg->message == WM_RBUTTONDOWN ||
        pMsg->message == WM_MBUTTONDOWN)
    {
        POINT pt = { GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) };
        // For messages posted from other windows, pMsg->pt is screen coords; for some messages, use GetMessagePos
        POINT ptScreen;
        // If no oCPoint data, fallback:
        if (pMsg->hwnd == NULL)
            ::GetCursorPos(&ptScreen);
        else
            ptScreen = pMsg->pt; // usually screen coords

        CRect rcWnd;
        GetWindowRect(rcWnd);
        if (!rcWnd.PtInRect(ptScreen))
        {
            // click outside -> close
            DestroyWindow();
            return TRUE; // eat it so underlying window doesn't also process (optional)
        }
    }
    if ( pMsg->message == WM_KEYDOWN &&
         pMsg->wParam  == VK_ESCAPE     )
    {
        DestroyWindow();  // close popup
        return TRUE;      // handled
    }
    return CWnd::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CMFCRibbonDatePickerControl, CMFCRibbonBaseElement)

CMFCRibbonDatePickerControl::CMFCRibbonDatePickerControl()
{
    m_Date = COleDateTime::GetCurrentTime();
}

CMFCRibbonDatePickerControl::CMFCRibbonDatePickerControl(UINT nID, LPCTSTR lpszLabel)
{
    SetID(nID);
    SetText(lpszLabel);
    m_Date = COleDateTime::GetCurrentTime();
}

//
//  Sets date value of control
//
//  Parameters:  const COleDateTime& date[IN]
//               New date value
//
void
CMFCRibbonDatePickerControl::SetDate(const COleDateTime& date)
{
    if ( m_bPopupVisible == FALSE )
      m_Date = date;
    if ( m_Date.GetStatus() == COleDateTime::valid )
      m_strDate = m_Date.Format(VAR_DATEVALUEONLY);
    else
      m_strDate.Empty();
CString strDate = m_Date.Format(VAR_DATEVALUEONLY); //TODO:LJM debugging only
    Redraw();   // refresh element display
}
CString CMFCRibbonDatePickerControl::GetToolTipText() const
{
    return _T("Click to pick a date");
}
//
//  Sets enabled/disabled state of control
//  NOTES: Disabled state is drawn with grayed text; control does not respond
//         to clicks
//       : If state changes, forces parent ribbon to recalc layout and redraw
//       : Redundant calls with same state are ignored, causes update 
//         recursion
// 
//  Parameters:  bEnable[IN]
//                 TRUE to enable,
//                 FALSE to disable
void
CMFCRibbonDatePickerControl::SetEnabled(BOOL bEnable)
{
    if ( m_pRibbonBar  != nullptr &&
         m_bIsDisabled != !bEnable    )
    {
      m_bIsDisabled = !bEnable;
      m_pRibbonBar -> ForceRecalcLayout();
      m_pRibbonBar -> RedrawWindow();
    }
}
//
//  Check if oCPoint is inside element
//  Parameters:  oCPoint[IN]
//               oCPoint to check (in element coords)
//
//  Returns:     CMFCRibbonBaseElement*
//               Pointer to self if oCPoint is inside element,
CMFCRibbonBaseElement*
CMFCRibbonDatePickerControl::HitTest ( CPoint oCPoint )
{
    // Entire element bounds
    if ( m_rectButton.PtInRect(oCPoint) ||
         m_rectEdit.PtInRect(oCPoint)      )
    {
        return this; // return self, ribbon routing will call OnLButtonDown
    }

    return nullptr; // not in this element
}
int
CMFCRibbonDatePickerControl::HitTestPart(CPoint point)
{
    if (m_rectButton.PtInRect(point))
        return 1; // button
    if (m_rectEdit.PtInRect(point))
        return 2; // edit
    if (m_rectLabel.PtInRect(point))
        return 3; // label
    return 0; // nothing
}

CSize
CMFCRibbonDatePickerControl::GetRegularSize ( CDC *pDC )
{
    ASSERT_VALID(pDC);

    // Measure label text once
    m_sizeLabel = pDC->GetTextExtent(m_strText);

    // base width = label + edit box + button
    const int nButtonWidth = 17;
    const int nEditWidth   = 80;  // or adjust to taste
    int nWidth = m_sizeLabel.cx + 6 + nEditWidth + nButtonWidth;

    // fixed height from ribbon font
    int nHeight = max(m_sizeLabel.cy + 6, 22);

    return CSize(nWidth, nHeight);
}
CSize
CMFCRibbonDatePickerControl::GetSize(CDC* pDC)
{
    ASSERT_VALID(pDC);

    // measure label
    CString sLabel = GetText();
    CSize szLabel(0, 0);
    if (!sLabel.IsEmpty())
        szLabel = pDC->GetTextExtent(sLabel);

    // Observe button
    int nButtonWidth = 17;  

    // fixed editbox width + padding
    int nEditWidth  = 100;
    int nHeight     = max(szLabel.cy, 22); // at least ribbon edit height
    int nTotalWidth = szLabel.cx + 6 + nEditWidth + nButtonWidth;

    return CSize(nTotalWidth, nHeight);
}

void
CMFCRibbonDatePickerControl::OnDraw(CDC* pDC)
{
    ASSERT_VALID(pDC);

    // ---- 1. Layout rects ----
    CRect rc = m_rect;    // full element rect

    // label rect
    CRect rcLabel = rc;
    rcLabel.right = rcLabel.left + m_sizeLabel.cx + 6; // text + padding
    m_rectLabel = rcLabel; // save for hit test

    // edit+button rect
    CRect rcEdit = rc;
    rcEdit.left = rcLabel.right;

    // button rect (17px wide)
    const int nBtnWidth = 17;
    CRect rcButton = rcEdit;
    rcButton.left = rcButton.right - nBtnWidth;

    // text rect inside edit (excluding button)
    CRect rcText = rcEdit;
    rcText.right = rcButton.left;

    // ---- 2. Draw label ----
    //CRect rcLabel = m_rectLabel;
    CString strLabel = m_strText;

    COLORREF clrText = m_bIsDisabled ? 
        afxGlobalData.clrGrayedText :  // stock disabled text color
        afxGlobalData.clrBarText;      // normal

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(clrText);
    pDC->DrawText(strLabel, rcLabel, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(m_strText, rcLabel, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

    // ---- 3. Draw edit box ----
    pDC->DrawEdge(rcEdit, EDGE_SUNKEN, BF_RECT);
    m_rectEdit = rcEdit; // save for hit test

    // draw date string
    CString sDate;
    if (m_Date.GetStatus() == COleDateTime::valid)
        sDate = m_Date.Format(_T("%d/%m/%Y"));
    else
        sDate.Empty();

    CRect rcTxt = rcText;
    rcTxt.DeflateRect(4, 2);
    pDC->DrawText(sDate, rcTxt, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

    // ---- 4. Draw drop-down button ----
    pDC->FillSolidRect(rcButton, ::GetSysColor(COLOR_BTNFACE));
    pDC->DrawEdge(rcButton, EDGE_RAISED, BF_RECT);
    m_rectButton = rcButton; // save for hit test

    // small ▼ arrow centered
    CPoint ptCenter = rcButton.CenterPoint();
    CPoint pts[3] =
    {
        CPoint(ptCenter.x - 4, ptCenter.y - 1),
        CPoint(ptCenter.x + 4, ptCenter.y - 1),
        CPoint(ptCenter.x,     ptCenter.y + 3)
    };

    CBrush brArrow(::GetSysColor(COLOR_BTNTEXT));
    CBrush* pOldBr = pDC->SelectObject(&brArrow);
    pDC->Polygon(pts, 3);
    pDC->SelectObject(pOldBr);
}

void
CMFCRibbonDatePickerControl::OnLButtonDown(CPoint point)
{
    int nPart = HitTestPart(point);
    if ( IsDisabled() )
      return;

    if ( nPart == 1 ||
         nPart == 2    ) // button or edit
    {
        ShowCalendarPopup();
    }
    else
    {
        CMFCRibbonBaseElement::OnLButtonDown(point);
    }
}

//
//  Show calendar popup below control
//  NOTES: Popup self deletes when closed. Not modal - user
//         can click elsewhere to dismiss  
void
CMFCRibbonDatePickerControl::ShowCalendarPopup()
{
    CRect rc = m_rect;
    CPoint pt(rc.left, rc.bottom);

    CMFCRibbonBar* pRibbon = GetParentRibbonBar();
    ASSERT(pRibbon);
    if (pRibbon)
        pRibbon->ClientToScreen(&pt);   // screen coords

    auto* pPopup = new CCalendarPopup(this);
    if (!pPopup->CreatePopup(pRibbon ? pRibbon->GetTopLevelFrame() : AfxGetMainWnd(), pt))
        delete pPopup;
}
