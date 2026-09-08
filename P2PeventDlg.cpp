// Copyright © 2010, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for P2PeventDlg 
//

#include "stdafx.h"
#include <afxpriv.h>                   // For WM_IDLEUPDATECMDUI
#include "P2PeventDlg.h"
#include "CDialog_Ext.h"
#include "P2Pwin32.h"
#include "Msgexception.h"
#include "Resource.h"


///////////////////////////////////////////////////////////////////////
//  CP2PeventDlg dialog
//  NOTES: Facilitates exploration and exposure of P2Pevent contents

IMPLEMENT_DYNAMIC(CP2PeventDlg, CDialog)
CP2PeventDlg::CP2PeventDlg ( )
            : CDialog ( IDD_P2Pevent, 0 )
{
    // Firstly
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    StartupP2Pevent ( );
    m_pP2PeventTreeCtrl = 0;
    m_nP2PeventSinkID   = 0;
}
CP2PeventDlg::CP2PeventDlg ( CWnd *pParent /*=NULL*/)
	          : CDialog ( IDD_P2Pevent, pParent )
{
    // Firstly
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    StartupP2Pevent ( );
    m_pP2PeventTreeCtrl = 0;
    m_nP2PeventSinkID   = 0;
}

CP2PeventDlg::~CP2PeventDlg()
{
}

void CP2PeventDlg::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialog::DoDataExchange(pDX);
}

BOOL CP2PeventDlg::Create ( CWnd *pParent )
{
    // Locals
    BOOL bResult = FALSE;

    // Creation
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if ( CDialog::Create((UINT)IDD_P2Pevent,pParent) < 0 )
      return FALSE;

    // P2Pevents tree
    m_pP2PeventTreeCtrl = new CTreeCtrl ( );
    //this->GetDlgItem (IDC_PIC_P2Pevent
    //m_pP2PeventTreeCtrl -> Create ( TVS_HASBUTTONS, oRect, this, IDC_PIC_P2PeventTree );

    // P2Pevents registration
//  This line was commented out for development and debug purposes
//  compiled issue with expected version of CreateP2PeventSink LJm 2025-05-15
    //m_nP2PeventSinkID = CreateP2PeventSink ( (DWORD_PTR)m_hWnd, P2PeventCB_HWND );
    ASSERT(m_nP2PeventSinkID);
    m_dwP2PeventNotn  = RegP2PeventCmd ( m_nP2PeventSinkID
                                       , P2Pevent_AddMask, P2Pevotn_FULL );

    // Tidy up, and
    return bResult;
}

///////////////////////////////////////////////////////////////////////
//  MFC Virtual Overrides

//
//  Resource management override
//
INT_PTR
CP2PeventDlg::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

//
//  OnInitDialog MFC Virtual override
//
BOOL CP2PeventDlg::OnInitDialog ( )
{
    // Compliments Class Wizard
    MANAGE_RESOURCE_STATE;
  __super::OnInitDialog();
    // Establish icon
    // NOTES: Extreme left side of title bar
    CDialog_SetIconFromApp ( *this );

    // Create toolbar
    // NOTES: Follow CMainFrame pattern with the additional
    //        CToolBar_OnInitDialog() integration step.
    if ( !m_wndToolBar.Create(this)                          ||
         !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1_P2PeventDlg) ||
         !m_wndToolBar.CDialog_OnInitDialog(this)               )
    {
      TRACE0("Failed to create and integrate dialog toolbar\n");
      EndDialog ( IDCANCEL );
    }

    // Size control bars
    /*CRect oCRectClientStart;
    CRect oCRectClientNow;
    GetClientRect ( oCRectClientStart );
    RepositionBars ( AFX_IDW_CONTROLBAR_FIRST
                   , AFX_IDW_CONTROLBAR_LAST
                   , 0, reposQuery, oCRectClientNow );

    // Reposition controls
    CPoint oCPointOS( oCRectClientNow.left - oCRectClientStart.left 
                    , oCRectClientNow.top -oCRectClientStart.top );
    CRect oCRectChild;
    CWnd  *pCWndChild = GetWindow ( GW_CHILD );
    while ( pCWndChild )
    {
      pCWndChild->GetWindowRect ( oCRectChild );
      ScreenToClient ( oCRectChild );
      oCRectChild.OffsetRect ( oCPointOS );
      pCWndChild -> MoveWindow ( oCRectChild, FALSE );
      pCWndChild =  pCWndChild -> GetNextWindow();
    }

    // Adjust the dialog window dimensions to make room for control bars
    CRect oCRectWindow;
    GetWindowRect ( oCRectWindow );
    oCRectWindow.right += oCRectClientStart.Width() - oCRectClientNow.Width();
    oCRectWindow.bottom += oCRectClientStart.Height() - oCRectClientNow.Height();
    MoveWindow ( oCRectWindow, FALSE );

    // Position Control Bars
    RepositionBars ( AFX_IDW_CONTROLBAR_FIRST
                   , AFX_IDW_CONTROLBAR_LAST, 0 );*/

    // Centre dialog on the screen
    CenterWindow ( );

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////
//  CP2PeventDlg message handlers
BEGIN_MESSAGE_MAP(CP2PeventDlg, CDialog)
    ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
    ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
    ON_MESSAGE(WM_P2PeventNOTN,OnP2PeventNOTN)

    ON_COMMAND(ID_P2Pevent_CLEAR, OnP2PeventClear)
    ON_UPDATE_COMMAND_UI(ID_P2Pevent_CLEAR, OnUpdateP2PeventClear)
    ON_COMMAND(ID_P2Pevent_EVERR, OnP2PeventEVERR)
    ON_UPDATE_COMMAND_UI(ID_P2Pevent_EVERR, OnUpdateP2PeventEVERR)
    ON_COMMAND(ID_P2Pevent_EVDBG, OnP2PeventEVDBG)
    ON_UPDATE_COMMAND_UI(ID_P2Pevent_EVDBG, OnUpdateP2PeventEVDBG)
    ON_COMMAND(ID_P2Pevent_EVTRC, OnP2PeventEVTRC)
    ON_UPDATE_COMMAND_UI(ID_P2Pevent_EVTRC, OnUpdateP2PeventEVTRC)
END_MESSAGE_MAP()

LRESULT
CP2PeventDlg::OnIdleUpdateCmdUI ( WPARAM, LPARAM )
{
    SendMessageToDescendants ( WM_IDLEUPDATECMDUI );
    return 0L;
}
//
//  WM_KICKIDLE message handler
//  NOTES: Specifically intercepted and utilised to initiate
//         WM_IDLEUPDATECMDUI processing in the integrated CToolBar
void
CP2PeventDlg::OnKickIdle ( )
{
    SendMessageToDescendants ( WM_IDLEUPDATECMDUI );
}

//
//  WM_P2PeventNOTN message handler
//  NOTES: P2Pevent notifications from P2Peer API received at random
//
//
//  Parameters:  WPARAM wParam
//               Precipitating P2Pevent object.  Handler becomes
//               responsible for life cycle
//
//               LPARAM lParam
//               Not used
//
LRESULT
CP2PeventDlg::OnP2PeventNOTN ( WPARAM wParam, LPARAM )
{
    // Introduce locals
    m_pP2PeventTreeCtrl = (CTreeCtrl *)GetDlgItem(IDC_TREE_P2Pevent);
    P2Pevent *pP2Pevent = (P2Pevent *)wParam;
    if ( pP2Pevent == NULL )           // SNHappen, but
      return 0;
    HTREEITEM hItemHeader;

    // Header
    CString strSummary;
    strSummary += pP2Pevent -> GetClassText ( );
    hItemHeader = m_pP2PeventTreeCtrl -> InsertItem ( strSummary, TVI_ROOT, TVI_LAST );

    // Module
    if ( _tcslen(pP2Pevent->GetModule()) > 0 )
    {
      CString strModule("Module: ");
      strModule += pP2Pevent->GetModule();
      m_pP2PeventTreeCtrl -> InsertItem ( strModule
                                        , hItemHeader, TVI_LAST );
    }

    // HRESULT
    if ( pP2Pevent->GetHRESULT() )
    {
      CString strHRESULT;
      strHRESULT.Format ( _T("HRESULT: [%i] "), pP2Pevent->GetHRESULT() );
      strHRESULT += pP2Pevent->GetHRESULText();
      m_pP2PeventTreeCtrl -> InsertItem ( strHRESULT
                                        , hItemHeader, TVI_LAST );
    }

    // Message
    if ( pP2Pevent->Exists(LDesc) )
    {
      CString strMessage("Message: ");
      strMessage += (*pP2Pevent)[LDesc].c_wstr();
      HTREEITEM hItem = m_pP2PeventTreeCtrl
                 -> InsertItem ( strMessage, hItemHeader, TVI_LAST );
      if ( (*pP2Pevent)[LDesc].r_Object().IsField() )
      {
        //P3PmsgItem& oNode = dynamic_cast<P3PmsgField&>((*pP2Pevent)[LDesc]);
        P3PmsgCurs  oCurs ( dynamic_cast<P3PmsgField&>((*pP2Pevent)[LDesc]) );
        for ( int i = 0; oCurs.Goto(i); i++ )
        {
          m_pP2PeventTreeCtrl -> InsertItem ( oCurs.r_data().c_wstr()
                                            , hItem, TVI_LAST );
        }
      }
    }
    

    // Advice
    if ( pP2Pevent->Exists(LAdvice) )
    {
      CString strMessage("Advice: ");
      strMessage += (*pP2Pevent)[LAdvice].c_wstr();
      HTREEITEM hItem = m_pP2PeventTreeCtrl
                 -> InsertItem ( strMessage, hItemHeader, TVI_LAST );
      UNREFERENCED_PARAMETER ( hItem );   // used only by the block commented out below
      //if ( (*pP2Pevent)[LAdvice].r_Object().IsNode() )
      //{
      //  //P3PmsgNode& oNode = dynamic_cast<P3PmsgNode&>((*pP2Pevent)[LAdvice]);
      //  P3PmsgCurs  oCurs ( dynamic_cast<P3PmsgField&>((*pP2Pevent)[LAdvice]) );
      //  for ( int i = 0; oCurs.Goto(i); i++ )
      //  {
      //    m_pP2PeventTreeCtrl -> InsertItem ( oCurs.r_data().c_wstr()
      //                                      , hItem, TVI_LAST );
      //  }
      //}
    }

    // Tidy up and
    delete pP2Pevent;
    return 0;
}

//
//
//  [P2Pevent >> Clear] menu command handlers
//  NOTES: Clears P2Pevent list control contents
//
void
CP2PeventDlg::OnP2PeventClear ( ) 
{
    m_pP2PeventTreeCtrl = (CTreeCtrl *)GetDlgItem(IDC_TREE_P2Pevent);
    if ( m_pP2PeventTreeCtrl->m_hWnd )
      m_pP2PeventTreeCtrl -> DeleteAllItems ( );
}
void
CP2PeventDlg::OnUpdateP2PeventClear ( CCmdUI *pCmdUI ) 
{
    m_pP2PeventTreeCtrl = (CTreeCtrl *)GetDlgItem(IDC_TREE_P2Pevent);
    if ( m_pP2PeventTreeCtrl->m_hWnd    &&
         m_pP2PeventTreeCtrl->GetCount()   )
      pCmdUI -> SetCheck ( TRUE );
    else
      pCmdUI -> SetCheck ( FALSE );
}

//
//
//  [P2Pevent >> EVERR] menu command handlers
//  NOTES: Toggles P2Pevent on and off for this object
//
void
CP2PeventDlg::OnP2PeventEVERR ( ) 
{
    // Delegate
    DWORD dwCmd = P2Pevent_AddMask;
    if ( (m_dwP2PeventNotn&P2Pevotn_ERROR) == P2Pevotn_ERROR )
      dwCmd = P2Pevent_RemMask;
    m_dwP2PeventNotn = RegP2PeventCmd ( m_nP2PeventSinkID
                                      , dwCmd, P2Pevotn_ERROR );
}
void
CP2PeventDlg::OnUpdateP2PeventEVERR ( CCmdUI *pCmdUI ) 
{
    // Summarise current state
    if ( (m_dwP2PeventNotn&P2Pevotn_ERROR) == P2Pevotn_ERROR )
      pCmdUI -> SetCheck ( TRUE );
    else
      pCmdUI -> SetCheck ( FALSE );
}

void
CP2PeventDlg::OnP2PeventEVDBG ( ) 
{
    // Delegate
    DWORD dwCmd = P2Pevent_AddMask;
    if ( (m_dwP2PeventNotn&P2Pevotn_DEBUG) == P2Pevotn_DEBUG )
      dwCmd = P2Pevent_RemMask;
    m_dwP2PeventNotn = RegP2PeventCmd ( m_nP2PeventSinkID
                                      , dwCmd, P2Pevotn_DEBUG );
}
void
CP2PeventDlg::OnUpdateP2PeventEVDBG ( CCmdUI *pCmdUI ) 
{
    // Summarise current state
    if ( (m_dwP2PeventNotn&P2Pevotn_DEBUG) == P2Pevotn_DEBUG )
      pCmdUI -> SetCheck ( TRUE );
    else
      pCmdUI -> SetCheck ( FALSE );
}

void
CP2PeventDlg::OnP2PeventEVTRC ( ) 
{
    // Delegate
    DWORD dwCmd = P2Pevent_AddMask;
    if ( (m_dwP2PeventNotn&P2Pevotn_TRACE) == P2Pevotn_TRACE )
      dwCmd = P2Pevent_RemMask;
    m_dwP2PeventNotn = RegP2PeventCmd ( m_nP2PeventSinkID
                                      , dwCmd, P2Pevotn_TRACE );
}
void
CP2PeventDlg::OnUpdateP2PeventEVTRC ( CCmdUI *pCmdUI ) 
{
    // Summarise current state
    if ( (m_dwP2PeventNotn&P2Pevotn_TRACE) == P2Pevotn_TRACE )
      pCmdUI -> SetCheck ( TRUE );
    else
      pCmdUI -> SetCheck ( FALSE );
}
