// Copyright © 2016, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CURLDialog prototypes and definitions
//  NOTES: Facilitates management of URL definitions
//
#pragma once

#include "CBrush_Ext.h"

// CURLDialog dialog

class TargetcoreMFC_EXT CURLDialog : public CDialogEx
{
	  DECLARE_DYNAMIC(CURLDialog)
      void
        RenderDialogSafe ( );
    // Constructors and destructor
    public:
        CURLDialog ( LPCTSTR lpszURL
                   , DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS
                   , CDC* pdcPrinter = NULL
                   , CWnd *pCWndParent = NULL);
	      CURLDialog ( LPCTSTR lpszURL
                   , CWnd* pParent = NULL );
	    virtual
       ~CURLDialog ( );

    // Utilities
    protected:

    // Attributes
    public:
      CString   m_strURL;

	  //enum { IDD = IDD_URL };
    // MFC Virtual Overrides
    public:
      virtual INT_PTR
        DoModal ( );
    protected:
	    virtual void
        DoDataExchange ( CDataExchange *pDX );    // DDX/DDV support

    // MFC Message map
	  DECLARE_MESSAGE_MAP()
    public:
      afx_msg void
        OnIDOK ( );
      afx_msg void
        OnBtnClickedTestURL ( );
};
