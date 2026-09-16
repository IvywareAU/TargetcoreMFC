// Copyright © 2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CBrushDialog prototypes and definitions
//  NOTES: Facilitates population of LOGBRUSH structure
//
#pragma once
#include "CBrush_Ext.h"

#define CBrushDialog_OPACITY 0x00000001L         // OPACITY control
#define CBrushDialog_EoD     0x00000002L         // Enable or Disable brush control

//
//  CBrushDialog dialog
//  NOTES: Brush parameter management
class TargetcoreMFC_EXT CBrushDialog : public CDialogEx
{
	  DECLARE_DYNAMIC(CBrushDialog)
      void
        RenderDialogSafe ( ) noexcept;
    // Constructors and destructor
    public:
        CBrushDialog ( LOGBRUSHcrx *pLOGBRUSHinitialcrx = NULL
                     , DWORD dwStyleFlags = CF_EFFECTS | CF_SCREENFONTS
                     , CWnd *pCWndParent = NULL);
	      CBrushDialog ( LOGBRUSHcrx& oLOGBRUSHcrx
                     , CWnd* pParent = NULL );
	    virtual
       ~CBrushDialog ( );

    // Utilities
    protected:
      void
        DoStyleUpdate ( );
      void
        DoHatchUpdate ( );

    // Attributes
    public:
      LOGBRUSHcrx m_oLOGBRUSHcrx;
      DWORD       m_dwStyleFlags{0};
      int         m_nStyle;
      int         m_bDash;
      int         m_bDot;
      int         m_bDashDot;
      int         m_bDashDotDot;
      int         m_nHatched;
      int         m_bBDiagonal;
      int         m_bCross;
      int         m_bDiagCross;
      int         m_bFDiagonal;
      int         m_bHorizontal;
      int         m_bVeritical;
      int         m_bEoD{TRUE};
      DWORD       m_uAlpha{75};
      CBrush     *m_pCBrush{nullptr};
      CMFCSpinButtonCtrl m_spnOpacity;
      CEdit              m_edtOpacity;

      const LOGBRUSHcrx*
        GetCurrentBrush ( ) const noexcept { return &m_oLOGBRUSHcrx; }
      const COLORREF
        GetColor ( ) const noexcept { return m_oLOGBRUSHcrx.lbColor; }
	  //enum { IDD = IDD_LOGBRUSH };
    // MFC Virtual Overrides
    public:
      virtual INT_PTR
        DoModal ( );
    protected:
	    virtual void
        DoDataExchange ( CDataExchange *pDX );    // DDX/DDV support
      virtual BOOL
        OnInitDialog ( );
      afx_msg HBRUSH
        OnCtlColor ( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

    // MFC Message map
	  DECLARE_MESSAGE_MAP()
    public:
      afx_msg void
        OnIDOK();
      afx_msg void
        OnBnClickedBtnColor ( );
      afx_msg void
        OnBnClickedRadio ( );
      afx_msg void
        OnBnClickedEoD ( );
};
