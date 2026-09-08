// Copyright © 2011, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CP2PmsgNodeDlg
//

#include "stdafx.h"
#include "TargetCoreMFC.h"
#include "P2PmsgNodeDlg.h"
#include "afxdialogex.h"
#include "CDialog_Ext.h"

#include "resource.h"

// CP2PmsgNodeDlg dialog

IMPLEMENT_DYNAMIC(CP2PmsgNodeDlg, CDialogEx)

CP2PmsgNodeDlg::CP2PmsgNodeDlg ( P2PmsgMgr *pP2PmsgMgr
                               , CWnd* pParent /*=NULL*/)
	            : CDialogEx(IDD_P2PmsgNodeDlg, pParent)
{
    m_pP2PmsgMgr = pP2PmsgMgr;
}

CP2PmsgNodeDlg::~CP2PmsgNodeDlg()
{
}

///////////////////////////////////////////////////////////////////////
//  Operations

BOOL
CP2PmsgNodeDlg::AttachP2PmsgItem ( P2PmsgMgr *pP2PmsgMgr )
{
    P2Pos nP2PosRoot = pP2PmsgMgr->GetP2Pos();
    m_wndP2PmsgTreeCtrl.Connect ( pP2PmsgMgr, nP2PosRoot, m_hWnd );
    return TRUE;
}

///////////////////////////////////////////////////////////////////////
//  Property exposure

CString&
CP2PmsgNodeDlg::GetSelectedNodePath ( )
{
    return m_strSelectedItem;
}

P3PmsgObject&
CP2PmsgNodeDlg::GetSelectedObject ( )
{
    return m_oSelectedObject;
}

///////////////////////////////////////////////////////////////////////
//  MFC Virtual Overrides

void
CP2PmsgNodeDlg::DoDataExchange(CDataExchange* pDX)
{
    MANAGE_RESOURCE_STATE;
    CDialogEx::DoDataExchange(pDX);
    DDX_Control ( pDX, IDC_TREE_P2PmsgMgr, m_wndP2PmsgTreeCtrl);
}

//
//  OnInitDialog MFC Virtual override
//
BOOL
CP2PmsgNodeDlg::OnInitDialog ( )
{
    // Compliments Class Wizard
    MANAGE_RESOURCE_STATE;
  __super::OnInitDialog();
    // Establish icon
    // NOTES: Extreme left side of title bar
    CDialog_SetIconFromApp ( *this );

    if ( m_pP2PmsgMgr )
      AttachP2PmsgItem ( m_pP2PmsgMgr );
    // Create views
    /*CRect rectDummy;
    rectDummy.SetRectEmpty();
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    if (!m_wndP2PeventTreeCtrl.Create(dwViewStyle, rectDummy, this, 2))
    {
		  TRACE0("Failed to create Class View\n");
		  return -1;      // fail to create
    }*/

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

//
//  Resource management override
//
INT_PTR
CP2PmsgNodeDlg::DoModal()
{
    MANAGE_RESOURCE_STATE;
    return __super::DoModal();
}

///////////////////////////////////////////////////////////////////////////////
//  MFC Message handlers

BEGIN_MESSAGE_MAP(CP2PmsgNodeDlg, CDialogEx)
END_MESSAGE_MAP()


void
CP2PmsgNodeDlg::OnOK()
{
    // Isolate selected item
    HTREEITEM hItem = m_wndP2PmsgTreeCtrl.GetSelectedItem ( );
    if ( hItem )
    {
      P2Pos posItem = m_wndP2PmsgTreeCtrl.GetItemData ( hItem );
      if ( posItem )
        m_strSelectedItem =  m_wndP2PmsgTreeCtrl.GetItemPath ( hItem );
      if ( posItem )
        m_oSelectedObject = m_pP2PmsgMgr->P2Pos2Object(posItem);
    }

    // Delegate
  __super::OnOK ( );
}

// CP2PmsgNodeDlg message handlers
