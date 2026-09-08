// Copyright © 2019, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CSVDialog prototypes and definitions
//  NOTES: Facilitates management of CSV definitions
//
#pragma once

#include "CBrush_Ext.h"

// CSVDialog dialog

class TargetCoreMFC_EXT CSVDialog : public CDialogEx
{
	  DECLARE_DYNAMIC(CSVDialog)
      void
        RenderDialogSafe ( );
    // Constructors and destructor
    public:
        CSVDialog ( LPCTSTR lpszCSV
                  , DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS
                  , CDC* pdcPrinter = NULL
                  , CWnd *pCWndParent = NULL);
	      CSVDialog ( LPCTSTR lpszCSV
                  , CWnd* pParent = NULL );
	    virtual
       ~CSVDialog ( );

    // Utilities
    protected:

    // Attributes
    public:
      CString   m_strCSV;

	  //enum { IDD = IDD_CSV };
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
        OnBtnClickedTestCSV ( );
};
