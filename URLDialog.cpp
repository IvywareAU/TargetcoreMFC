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
//  CURLDialog implementations.
//  NOTES: Facilitates management of URL definitions
//

#include "stdafx.h"
#include "TargetcoreMFC.h"
#include "URLDialog.h"
#include "afxdialogex.h"
#include "CWndApp_Ext.h"

#include "Resource.h"

// CURLDialog dialog

IMPLEMENT_DYNAMIC(CURLDialog, CDialogEx)

CURLDialog::CURLDialog ( LPCTSTR lpszURL, CWnd *pParent )
	        : CDialogEx ( IDD_URL, pParent )
{
    RenderDialogSafe ( );
    m_strURL = lpszURL;
}
CURLDialog::CURLDialog ( LPCTSTR lpszURL
                       , DWORD dwStyleFlags
                       , CDC* pdcPrinter
                       , CWnd *pCWndParent )
	        : CDialogEx ( IDD_URL, pCWndParent )
{
    UNREFERENCED_PARAMETER(pdcPrinter);
    UNREFERENCED_PARAMETER(dwStyleFlags);
    RenderDialogSafe ( );
    m_strURL = lpszURL;
}

CURLDialog::~CURLDialog()
{
}

void
CURLDialog::RenderDialogSafe ( )
{

    // Hatched radio button exceptions

    // Enabled or Disabled status
}

///////////////////////////////////////////////////////////////////////
//  Utilities

///////////////////////////////////////////////////////////////////////
//  MFC Virtual Overrides

//
//  Resource management override
//
INT_PTR
CURLDialog::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

void
CURLDialog::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialogEx::DoDataExchange(pDX);
    DDX_Text ( pDX, IDC_EDT_URL, m_strURL);
    DDV_MaxChars ( pDX, m_strURL, 255 );
}

///////////////////////////////////////////////////////////////////////
//  MFC Message map

BEGIN_MESSAGE_MAP(CURLDialog, CDialogEx)
    ON_COMMAND(IDOK, &CURLDialog::OnIDOK)
    ON_BN_CLICKED(IDC_BTN_TEST_URL, OnBtnClickedTestURL)
END_MESSAGE_MAP()

//
//  ID_OK command handler
void
CURLDialog::OnIDOK ( )
{
    MANAGE_RESOURCE_STATE;
    UpdateData ( );
  __super::OnOK ( );  
}

//
//  [BN_CLICKED] handler for Trading {Test} button
//  NOTES: Tests Stock Trading URL reference
//
void
CURLDialog::OnBtnClickedTestURL ( )
{
    // Implementation
    UpdateData ( );
    CWndApp_PostHtmlURL ( m_strURL, FALSE );
    // Tidy up, and
    UpdateData(FALSE);
}
