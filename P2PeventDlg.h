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
//  P2PeventDlg prototypes and definitions.
//
#pragma   once

#include "Msgexception.h"
#include "DlgToolBar.h"
#include "TargetCoreMFC.h"


// CP2PeventDlg dialog

class TargetCoreMFC_EXT CP2PeventDlg : public CDialog
{
	  DECLARE_DYNAMIC(CP2PeventDlg)
    // Constructors and destructor
    public:
        CP2PeventDlg ( );
	      CP2PeventDlg ( CWnd* pParent );   // standard constructor
	    virtual
       ~CP2PeventDlg();
      BOOL
        Create ( CWnd *pParent );

    // Dialog Data
    //	enum { IDD = IDD_P2Pevent };

    // Attributes
    public:
      CDlgToolBar     m_wndToolBar;
      CTreeCtrl      *m_pP2PeventTreeCtrl;
      P2PeventSinkID  m_nP2PeventSinkID;
      DWORD           m_dwP2PeventNotn;

    // MFC Virtuals
    public:
      virtual INT_PTR
        DoModal ( );
    protected:
	    virtual void
        DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    public:
      virtual BOOL
        OnInitDialog ( );

    // MFC message map
    protected:
	  DECLARE_MESSAGE_MAP()
      afx_msg LRESULT
        OnP2PeventNOTN ( WPARAM wParam, LPARAM lParam );
      afx_msg void
        OnKickIdle ( );
      afx_msg LRESULT
        OnIdleUpdateCmdUI ( WPARAM wParam, LPARAM lParam );
      afx_msg void
        OnP2PeventClear ( );
      afx_msg void
        OnUpdateP2PeventClear ( CCmdUI *pCmdUI );
      afx_msg void
        OnP2PeventEVERR ( );
      afx_msg void
        OnUpdateP2PeventEVERR ( CCmdUI *pCmdUI );
      afx_msg void
        OnP2PeventEVDBG ( );
      afx_msg void
        OnUpdateP2PeventEVDBG ( CCmdUI *pCmdUI );
      afx_msg void
        OnP2PeventEVTRC ( );
      afx_msg void
        OnUpdateP2PeventEVTRC ( CCmdUI *pCmdUI );
};
