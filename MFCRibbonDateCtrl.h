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
//  CMFCRibbonDatePicker  extensions, ptototypes and definitions
//  NOTES:
//
#pragma once
#include "TargetcoreMFC.h"
//#include <afxribbon.h>
#include <afxdtctl.h>   // for CMonthCalCtrl
// CalendarPopup.h

class CMFCRibbonDatePickerControl;

// CalendarPopup.h (or top of .cpp)
#pragma once
#include <afxwin.h>
#include <afxcmn.h> // CMonthCalCtrl
#include <afxext.h>

class CMFCRibbonDatePickerControl; // forward

class CCalendarPopup : public CWnd
{
    DECLARE_DYNAMIC(CCalendarPopup)
    public:
      explicit CCalendarPopup(CMFCRibbonDatePickerControl* pOwner);
      virtual ~CCalendarPopup();

      // Create the popup at screen point ptScreen; pParentFrame must be a valid top-level window
      bool CreatePopup(CWnd* pParentFrame, const CPoint& ptScreen);

    // Sttributes
    protected:
      CMFCRibbonDatePickerControl *m_pOwner{nullptr};
      CMonthCalCtrl                m_wndCal;

    // message handlers
    DECLARE_MESSAGE_MAP()
      afx_msg void
        OnNcDestroy();
      afx_msg void
        OnKillFocus(CWnd* pNewWnd);
      virtual BOOL
        PreTranslateMessage(MSG* pMsg) override;
      afx_msg void
        OnCalSelect(NMHDR* pNMHDR, LRESULT* pResult);
};

class TargetcoreMFC_EXT CMFCRibbonDatePickerControl : public CMFCRibbonBaseElement
{
    DECLARE_DYNCREATE(CMFCRibbonDatePickerControl)
    friend class CCalendarPopup;

    public:
        CMFCRibbonDatePickerControl();
        CMFCRibbonDatePickerControl(UINT nID, LPCTSTR lpszLabel);

      void
        SetDate(const COleDateTime& date);
      COleDateTime
        GetDate() const { return m_Date; }
      void
        SetRibbonBar(CMFCRibbonBar* pRibbon) { m_pRibbonBar = pRibbon; }
      CMFCRibbonBar*
        GetParentRibbonBar() const { return m_pRibbonBar; }
      virtual CSize
        GetSize(CDC* pDC) override;
      virtual CMFCRibbonBaseElement*
        HitTest ( CPoint point ) override;
      int
        HitTestPart ( CPoint point );
      BOOL IsDisabled() const override
        { return m_bIsDisabled; }
      void 
        SetEnabled ( BOOL bEnable );
protected:
      COleDateTime   m_Date;
      CString        m_strDate; // formatted date string  
      CRect          m_rectLabel;     // bounding rect provided by ribbon
      CRect          m_rectEdit;    // area for text
      CRect          m_rectButton;  // area for arrow
      CMFCRibbonBar *m_pRibbonBar{nullptr};
      CSize          m_sizeLabel;
      BOOL           m_bIsDisabled{FALSE};
      BOOL           m_bPopupVisible{FALSE};
    // Overrides
      virtual void
        OnDraw(CDC* pDC) override;
      virtual CSize
        GetRegularSize(CDC* pDC) override;
      virtual void
        OnLButtonDown(CPoint point) override;
      virtual CString
        GetToolTipText() const override;

    // Helpers
    public:
      void
        ShowCalendarPopup();
      bool
        IsArrowClicked(CPoint pt) const { return m_rectButton.PtInRect(pt); }
};
