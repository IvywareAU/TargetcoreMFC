// Copyright © 2021, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CefCtrl definitions and prototypes
//
#pragma once
#include "TargetcoreMFC.h"

// CefCtrl

class TargetcoreMFC_EXT CefCtrl : public CWnd
{
    DECLARE_DYNAMIC(CefCtrl)

    public:
	      CefCtrl ( LPCTSTR lpszCtrlType, LPCTSTR lpszUniqueTag );
	    virtual
       ~CefCtrl ( );

    // Attributes
    public:

    // Property exposure
    public:

    // Operations
    public:

    // Python Python Advisor and Scanning operations

    // Drawing activities
    // NOTES: Enable and disable drawing activities.  Override in derived class
    public:

    // MFC Virtual overrides and specialisations
    public:
      virtual BOOL
        PreTranslateMessage ( MSG *pMsg);
      virtual BOOL
        Create ( DWORD dwStyle, const RECT& oRECT, CWnd *pParentWnd, UINT nID ) = 0;
      virtual BOOL
        Create ( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle
               , const RECT& oRECT, CWnd *pParentWnd, UINT nID
               , CCreateContext *pContext = NULL );
      afx_msg virtual void
        OnPaint ( );
      afx_msg void
        OnLButtonUp ( UINT nFlags, CPoint oCPoint );
      afx_msg void
        OnLButtonDown ( UINT nFlags, CPoint oCPoint );
      afx_msg void
        OnMouseMove ( UINT nFlags, CPoint point );
      afx_msg void
        OnMouseLeave ( );
      afx_msg void
        OnLButtonDblClk ( UINT nFlags, CPoint oCPoint );
      afx_msg void
        OnSize ( UINT nType, int cx, int cy );

    // MFC Message map
    protected:
	  DECLARE_MESSAGE_MAP()
      // Application messages
};

///////////////////////////////////////////////////////////////////////
//  CefCtrl helpers
