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
//  CSVDialog implementations.
//  NOTES: Facilitates management of CSV definitions
//

#include "stdafx.h"
#include "TargetcoreMFC.h"
#include "CSVDialog.h"
#include "afxdialogex.h"
#include "CWndApp_Ext.h"

#include "Resource.h"

// CSVDialog dialog

IMPLEMENT_DYNAMIC(CSVDialog, CDialogEx)

CSVDialog::CSVDialog ( LPCTSTR lpszCSV, CWnd *pParent )
	       : CDialogEx ( IDD_CSV, pParent )
{
    RenderDialogSafe ( );
    m_strCSV = lpszCSV;
}
CSVDialog::CSVDialog ( LPCTSTR lpszCSV
                     , DWORD dwStyleFlags
                     , CDC* pdcPrinter
                     , CWnd *pCWndParent )
	       : CDialogEx ( IDD_CSV, pCWndParent )
{
    UNREFERENCED_PARAMETER(pdcPrinter);
    UNREFERENCED_PARAMETER(dwStyleFlags);
    RenderDialogSafe ( );
    m_strCSV = lpszCSV;
}

CSVDialog::~CSVDialog()
{
}

void
CSVDialog::RenderDialogSafe ( )
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
CSVDialog::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

void
CSVDialog::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialogEx::DoDataExchange(pDX);
    DDX_Text ( pDX, IDC_EDT_URL, m_strCSV);
    DDV_MaxChars ( pDX, m_strCSV, 255 );
}

///////////////////////////////////////////////////////////////////////
//  MFC Message map

BEGIN_MESSAGE_MAP(CSVDialog, CDialogEx)
    ON_COMMAND(IDOK, &CSVDialog::OnIDOK)
    ON_BN_CLICKED(IDC_BTN_TEST_URL, OnBtnClickedTestCSV)
END_MESSAGE_MAP()

//
//  ID_OK command handler
void
CSVDialog::OnIDOK ( )
{
    MANAGE_RESOURCE_STATE;
    UpdateData ( );
  __super::OnOK ( );  
}

//
//  [BN_CLICKED] handler for Trading {Test} button
//  NOTES: Tests CSV string formation
//
void
CSVDialog::OnBtnClickedTestCSV ( )
{
    // Implementation
    UpdateData ( );
    CWndApp_PostHtmlURL ( m_strCSV, FALSE );
    // Tidy up, and
    UpdateData(FALSE);
}
