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
//  CefCtrl implementation
//  NOTES: Base class for chart stack items
//

#include "stdafx.h"
#include "Resource.h"
#include "CefCtrl.h"
#include "Msgexception.h"


// ChartCtrl

IMPLEMENT_DYNAMIC(CefCtrl, CWnd)

CefCtrl::CefCtrl ( LPCTSTR lpszChartType, LPCTSTR lpszUniqueTag )
{
    //  Both are ignored by design: this control is a non-visual stub, and the
    //  signature is kept so a real implementation can drop straight in.
    UNREFERENCED_PARAMETER ( lpszChartType );
    UNREFERENCED_PARAMETER ( lpszUniqueTag );
}

CefCtrl::~CefCtrl()
{
}

///////////////////////////////////////////////////////////////////////
//  Property exposure

///////////////////////////////////////////////////////////////////////
//  External operations



///////////////////////////////////////////////////////////////////////
//  MFC Virtual overrides and specialisations

BOOL
CefCtrl::PreTranslateMessage ( MSG *pMsg)
{
    // Market replay control keys
    // NOTES: Ctrl must have focus and Market Reply active

    // CXxxx can be a CFormView, CDialog, or CPropertyPage class.
    return __super::PreTranslateMessage(pMsg);
}

//
//  CWnd::Create specialisation
//

BOOL
CefCtrl::Create ( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle
                  , const RECT& oRECT, CWnd * pParentWnd, UINT nID
                  , CCreateContext *pContext )
{
    // Direct delegation
    // NOTES: Actual window creation is negated on non-visual of scanning mode
    //      : Every parameter is therefore ignored. They are named rather than
    //        commented out so the override still reads as the CWnd::Create it
    //        is standing in for.
    UNREFERENCED_PARAMETER ( lpszClassName );
    UNREFERENCED_PARAMETER ( lpszWindowName );
    UNREFERENCED_PARAMETER ( dwStyle );
    UNREFERENCED_PARAMETER ( oRECT );
    UNREFERENCED_PARAMETER ( pParentWnd );
    UNREFERENCED_PARAMETER ( nID );
    UNREFERENCED_PARAMETER ( pContext );
    RECT oRectClient = { 0, 0, 800, 100 }; // Arbituary values

    // Persistance
    // NOTES: Reference height set in first instance

    // Tidy up, and
    return TRUE;
}


///////////////////////////////////////////////////////////////////////
//  MFC Message handlers
//  NOTES: Multiple MFC inheritance used and as such CWnd MUST be first
//         in the list

BEGIN_MESSAGE_MAP ( CefCtrl, CWnd )
    ON_WM_SIZE()
    //ON_WM_CONTEXTMENU()
    ON_WM_PAINT()
END_MESSAGE_MAP()

//
//  WM_PAINT handler
//
void
CefCtrl::OnPaint ( )
{
    // Problematic
    try
    {
    }
    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel
}

//
//  WM_SIZE message handler
//  NOTES: Triggers certain layout calculations
void
CefCtrl::OnSize ( UINT nType, int cx, int cy )
{
    MANAGE_RESOURCE_STATE;             // Focus on DLL resources
    CWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
}
