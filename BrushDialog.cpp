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
//  CBrushDialog implementations.
//  NOTES: Facilitates population of LOGBRUSH structure
//

#include "stdafx.h"
#include "TargetcoreMFC.h"
#include "BrushDialog.h"
#include "afxdialogex.h"

#include "Resource.h"
#include "Msgexception.h"

// CBrushDialog dialog

IMPLEMENT_DYNAMIC(CBrushDialog, CDialogEx)

CBrushDialog::CBrushDialog ( LOGBRUSHcrx& oLOGBRUSHcrx, CWnd *pParent )
	          : CDialogEx ( IDD_LOGBRUSH, pParent )
{
    m_oLOGBRUSHcrx = oLOGBRUSHcrx;
    m_dwStyleFlags = CBrushDialog_OPACITY;
    RenderDialogSafe ( );
}
CBrushDialog::CBrushDialog ( LOGBRUSHcrx *pLOGBRUSHinitial
                           , DWORD dwStyleFlags
                           , CWnd *pCWndParent )
	          : CDialogEx ( IDD_LOGBRUSH, pCWndParent )
{
    m_dwStyleFlags = dwStyleFlags;
    if ( pLOGBRUSHinitial )
      memcpy ( &m_oLOGBRUSHcrx, pLOGBRUSHinitial, sizeof(m_oLOGBRUSHcrx) );
    RenderDialogSafe ( );
}

CBrushDialog::~CBrushDialog()
{
    delete m_pCBrush;
}

void
CBrushDialog::RenderDialogSafe ( ) noexcept
{
    if ( m_oLOGBRUSHcrx.lbStyle == BS_DIBPATTERN )
      m_nStyle = 0;
    else if ( m_oLOGBRUSHcrx.lbStyle == BS_HATCHED )
      m_nStyle = 1;
    else if ( m_oLOGBRUSHcrx.lbStyle == BS_HOLLOW )
      m_nStyle = 2;
    else if ( m_oLOGBRUSHcrx.lbStyle == BS_PATTERN )
      m_nStyle = 3;
    else if ( m_oLOGBRUSHcrx.lbStyle == BS_SOLID )
      m_nStyle = 4;
    else
      m_nStyle = BS_SOLID;

    // Hatched radio button exceptions
    if ( m_oLOGBRUSHcrx.lbHatch == HS_HORIZONTAL )
      m_nHatched = 0;
    else if ( m_oLOGBRUSHcrx.lbHatch == HS_VERTICAL )
      m_nHatched = 1;
    else if ( m_oLOGBRUSHcrx.lbHatch == HS_FDIAGONAL )
      m_nHatched = 2;
    else if ( m_oLOGBRUSHcrx.lbHatch == HS_BDIAGONAL )
      m_nHatched = 3;
    else if ( m_oLOGBRUSHcrx.lbHatch == HS_CROSS )
      m_nHatched = 4;
    else if ( m_oLOGBRUSHcrx.lbHatch == HS_DIAGCROSS )
      m_nHatched = 5;
    else
      m_nHatched = HS_HORIZONTAL;

    // Enabled or Disabled status
    m_bEoD    = m_oLOGBRUSHcrx.uEoD;
    m_uAlpha  = m_oLOGBRUSHcrx.uAlpha;
}

///////////////////////////////////////////////////////////////////////
//  Utilities

void
CBrushDialog::DoStyleUpdate ( )
{
    // Style radio button exceptions
    if ( m_nStyle == 0 )
      m_oLOGBRUSHcrx.lbStyle = BS_DIBPATTERN;
    else if ( m_nStyle == 1 )
      m_oLOGBRUSHcrx.lbStyle = BS_HATCHED;
    else if ( m_nStyle == 2 )
      m_oLOGBRUSHcrx.lbStyle = BS_HOLLOW;
    else if ( m_nStyle == 3 )
      m_oLOGBRUSHcrx.lbStyle = BS_PATTERN;
    else if ( m_nStyle == 4 )
      m_oLOGBRUSHcrx.lbStyle = BS_SOLID;
    else
      ASSERT(0);
}

void
CBrushDialog::DoHatchUpdate ( )
{
    // Hatched radio button exceptions
    if ( m_nHatched == 0 )
      m_oLOGBRUSHcrx.lbHatch = HS_HORIZONTAL;
    else if ( m_nHatched == 1 )
      m_oLOGBRUSHcrx.lbHatch = HS_VERTICAL;
    else if ( m_nHatched == 2 )
      m_oLOGBRUSHcrx.lbHatch = HS_FDIAGONAL;
    else if ( m_nHatched == 3 )
      m_oLOGBRUSHcrx.lbHatch = HS_BDIAGONAL;
    else if ( m_nHatched == 4 )
      m_oLOGBRUSHcrx.lbHatch = HS_CROSS;
    else if ( m_nHatched == 5 )
      m_oLOGBRUSHcrx.lbHatch = HS_DIAGCROSS;
    else
      ASSERT(0);
}

///////////////////////////////////////////////////////////////////////
//  MFC Virtual Overrides

//
//  Resource management override
//
INT_PTR
CBrushDialog::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

void
CBrushDialog::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio  ( pDX, IDC_RAD_DIBPATTERN, m_nStyle);
    DDX_Radio  ( pDX, IDC_RAD_BDIAGONAL, m_nHatched);
    DDX_Check  ( pDX, IDC_CHK_EoD, m_bEoD );
    DDX_Control( pDX, IDC_SPN_OPACITY, m_spnOpacity );
    DDX_Control( pDX, IDC_EDT_OPACITY, m_edtOpacity );
    //DDX_Radio ( pDX, IDC_RAD_DASH, m_bDash);
    //DDX_Radio ( pDX, IDC_RAD_DOT, m_bDot);
    //DDX_Radio ( pDX, IDC_RAD_DASHDOT, m_bDashDot);
    //DDX_Radio ( pDX, IDC_RAD_DASHDOTDOT, m_bDashDotDot);
}

//
//  OnInitDialog MFC Virtual override
//
BOOL
CBrushDialog::OnInitDialog ( )
{
    // Compliments Class Wizard
    MANAGE_RESOURCE_STATE;
  __super::OnInitDialog();

    // Expect issues
    try
    {
      m_spnOpacity.SetRange ( 10, 255 );
      //m_spnOpacity.SetBuddy ( &m_edtOpacity );
      if ( m_uAlpha < 10 )
        m_uAlpha = 10;
      else if ( m_uAlpha > 255 )
        m_uAlpha = 255;
      m_spnOpacity.SetPos   ( m_uAlpha );
    }

    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

//  ON_WM_CTLCOLOR handler lifted from Microsoft example
//  NOTES: This OnCtlColor handler will change the color of a static control 
//         with the ID of IDC_STA_COLOR. The code assumes that the CBStoreShadesetDlg
//         class has an initialized and created CBrush member named m_pCBrush. 
//         The control will be painted with selected background color of m_brush.
HBRUSH
CBrushDialog::OnCtlColor ( CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    // Call the base class implementation first! Otherwise, it may 
    // undo what we're trying to accomplish here.
    MANAGE_RESOURCE_STATE;
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // Isolate control of interest 
    if ( IDC_STA_BGBRUSH == pWnd->GetDlgCtrlID() )
    {
      if ( m_pCBrush == nullptr )
      {
        m_pCBrush = new CBrush();
        m_pCBrush -> CreateBrushIndirect ( &m_oLOGBRUSHcrx );
      }
      // Set the selected color
      pDC->SetTextColor(m_oLOGBRUSHcrx.lbColor);

      // Set the background mode for text to transparent  
      // so background will show thru.
      pDC->SetBkMode(TRANSPARENT);

      // Return handle to our CBrush object
      hbr = *m_pCBrush;
    }

    return hbr;
}

///////////////////////////////////////////////////////////////////////
//  MFC Message map

BEGIN_MESSAGE_MAP(CBrushDialog, CDialogEx)
    ON_WM_CTLCOLOR() 
    ON_COMMAND(IDOK, &CBrushDialog::OnIDOK)
    ON_BN_CLICKED(IDC_BTN_COLOR, &CBrushDialog::OnBnClickedBtnColor)
    ON_BN_CLICKED(IDC_RAD_DIBPATTERN, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_HATCHED, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_HOLLOW, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_PATTERN, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_SOLID, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_HORIZONTAL, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_VERTICAL, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_FDIAGONAL, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_BDIAGONAL, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_CROSS, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_RAD_DIAGCROSS, &CBrushDialog::OnBnClickedRadio)
    ON_BN_CLICKED(IDC_CHK_EoD, &CBrushDialog::OnBnClickedEoD)
END_MESSAGE_MAP()

//
//  ID_OK command handler
void
CBrushDialog::OnIDOK ( )
{
    MANAGE_RESOURCE_STATE;
    UpdateData ( );
    DoStyleUpdate( );
    DoHatchUpdate( );
    m_uAlpha = m_spnOpacity.GetPos ( );
    m_oLOGBRUSHcrx.uEoD   = m_bEoD;
    m_oLOGBRUSHcrx.uAlpha = m_uAlpha;
    //m_oLOGBRUSHcrx.lopnWidth.x = m_edtLineThicknes;
    //m_oLOGBRUSHcrx.lopnWidth.y = 0;    
  __super::OnOK ( );  
}

void
CBrushDialog::OnBnClickedBtnColor()
{
    // TODO: Add your control notification handler code here
    MANAGE_RESOURCE_STATE;
    CColorDialog oColorDialog ( m_oLOGBRUSHcrx.lbColor, 0u, this );
    if ( oColorDialog.DoModal() == IDOK )
      m_oLOGBRUSHcrx.lbColor = oColorDialog.GetColor();

    // Refresh
    delete m_pCBrush;
           m_pCBrush = nullptr;
    GetDlgItem(IDC_STA_BGBRUSH)->Invalidate();
}

//
//  [BN_CLICKED] handler for [Radio] buttons
//
void
CBrushDialog::OnBnClickedRadio ( )
{
    // Delegate
    MANAGE_RESOURCE_STATE;
    UpdateData ( );
    DoStyleUpdate( );
    DoHatchUpdate( );

    // Refresh
    delete m_pCBrush;
           m_pCBrush = nullptr;
    GetDlgItem(IDC_STA_BGBRUSH)->Invalidate();
}


//
//  [BN_CLICKED] handler for [EoD] check button
//
void
CBrushDialog::OnBnClickedEoD ( )
{
    // Delegate
    MANAGE_RESOURCE_STATE;
    UpdateData ( );
    m_oLOGBRUSHcrx.uEoD = m_bEoD;
}
