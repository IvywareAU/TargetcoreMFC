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
//  CP2PmsgNodeDlg prototypes and definitions.
//
#pragma once

#include "TargetCoreMFC.h"
#include "P2PmsgMgr.h"
#include "P2PmsgTreeCtrl.h"

// CP2PmsgNodeDlg dialog

class TargetCoreMFC_EXT CP2PmsgNodeDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CP2PmsgNodeDlg)
    // Constructors and destructor
    public:
        CP2PmsgNodeDlg ( P2PmsgMgr *pP2PmsgMgr
                       , CWnd *pParent = NULL );   // standard constructor
      virtual
       ~CP2PmsgNodeDlg ( );

    // Attributes
    public:
      P2PmsgMgr        *m_pP2PmsgMgr{nullptr};
      CP2PmsgTreeCtrl   m_wndP2PmsgTreeCtrl;
      CString           m_strSelectedItem;
      P3PmsgObject      m_oSelectedObject;
     //enum { IDD = IDD_P2PmsgNodeDlg };
      CString&
        GetSelectedNodePath ( );
      P3PmsgObject&
        GetSelectedObject ( );

    // Operations
    public:
      BOOL
        AttachP2PmsgItem ( P2PmsgMgr *pP2PmsgMgr );

    // MFC Virtuals
    public:
      virtual INT_PTR
        DoModal ( );
    protected:
      virtual void
        DoDataExchange ( CDataExchange *pDX );    // DDX/DDV support
      virtual BOOL
        OnInitDialog ( );
      void
        OnOK ( );

    // MFC message map
    protected:
    DECLARE_MESSAGE_MAP()
};
