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
//  P2PmsgTreeCtrl prototypes and definitions.
//  NOTES: Integrates the CTreeCtrl and P2PmsgMgr objects
//
#pragma   once

#include "P2Peer.h"
#include "P2PeerTarget.h"
#include "P2PmsgMgr.h"
#include "TargetcoreMFC.h"
#include "CString_Ext.h"

//  Pre-definitions
typedef CMap<P2Pos, P2Pos, HTREEITEM, HTREEITEM> CMap_HTREE_P2Pos;
//typedef CList<CString> CMap_CString;

class TargetcoreMFC_EXT CP2PmsgTreeCtrl : public CTreeCtrl, public P2PeerTarget
{
	  DECLARE_DYNAMIC(CP2PmsgTreeCtrl)

    // Constructors and destructor
    public:
	      CP2PmsgTreeCtrl();
	    virtual
       ~CP2PmsgTreeCtrl();
      bool
        Connect ( P2PmsgMgr *pP2PmsgMgr, P2Pos posRoot
                , HWND hWndTriggers
                , int nRootImage = 0, int nRootSelectedImage = 0 );
      void
        RecyleThisObject ( );

    // Notifications
    public:
      bool
        NotifyP2Pmsg_AddNode   ( CWnd *pCWndNotify
                               , const P3PmsgItem& oNodeParent
                               , const P3PmsgItem& oNodeChild );

    // Operations
    public:
      HTREEITEM
        InsertP2Pmsg ( HTREEITEM hItemParent, const P3PmsgItem& oItem );
      HTREEITEM
        UpdateP2Pmsg ( HTREEITEM hItem, const P3PmsgItem& oItem );
      virtual BOOL
        RefreshP2Pmsg( HTREEITEM ) { return FALSE; }
      void
        RecoverP2Pmsg( HTREEITEM hDispItem, P3PmsgItem& oDispItem );

    // CTreeCtrl extensions
    public:
	    void
        ShuffleItemUp ( HTREEITEM hItem );
	    void
        ShuffleItemDown ( HTREEITEM hItem );
      static LPARAM
        MoveItemCB ( CTreeCtrl *pThis
                   , HTREEITEM hItemSource, HTREEITEM hItemDestin );

    // Attributes
    protected:
      CMap_HTREE_P2Pos  m_oCMapHTREE;
      CMap_CString      m_oCListP2PmsgFilters;
      HTREEITEM         m_hRootItem{NULL};
      P2PmsgMgr        *m_pP2PmsgMgr_Ext{nullptr};
      P2PmsgMgr        *m_pP2PmsgMgr{nullptr};
      HWND              m_hWndTriggers{NULL};
      HTREEITEM         m_hItemCut{NULL};

    // Life cycle management
    public:
      UINT
        AddRef ( );
      UINT
        Release ( );

    // Properties
    public:
      virtual P2Pos
        IsTreeFolder ( HTREEITEM hItem );
      HTREEITEM
        IsItem ( P2Pos pos );
      P2Pos
        IsItem ( HTREEITEM hItem );
      P2PmsgMgr&
        GetP2PmsgMgr ( );
      P3PmsgItem
        GetItemItem ( HTREEITEM hItem );
      P3PmsgObject
        GetItemObject ( HTREEITEM hItem );
      P2Pos
        GetItemP2Pos ( HTREEITEM hItem );
      P3PmsgAttr
        GetItemAttr ( HTREEITEM hItem );
      CString
        GetItemPath ( HTREEITEM hItem );
      CString
        GetSelectedPath ( );
      CMap_CString&
        GetListP2PmsgFilters ( );
      bool
        HasFilteredItems ( P3PmsgObject& oObject );
      bool
        IsFilteredItem ( P3PmsgObject& oObject );
      HTREEITEM
        GetCursorItem ( UINT *puiFlags = 0 );
      HTREEITEM
        SelectRootItem ( LPCTSTR lpszRootPath );
      HTREEITEM
        SelectCutItem ( HTREEITEM hItemCut );

    // MFC Virtual overrides
    protected:
	  virtual BOOL
        OnNotify ( WPARAM wParam, LPARAM lParam, LRESULT *pResult );

    // MFC message map
    protected:
	  DECLARE_MESSAGE_MAP()
    public:
      LRESULT
        OnP2PmsgTrigINSERT ( WPARAM wParam, LPARAM lParam );
      LRESULT
        OnP2PmsgTrigUPDATE ( WPARAM wParam, LPARAM lParam );
      LRESULT
        OnP2PmsgTrigDELETE ( WPARAM wParam, LPARAM lParam );
      afx_msg void
        OnTvnDeleteItem ( NMHDR *pNMHDR, LRESULT *pResult );
      afx_msg BOOL
        OnTvnGetDispInfo ( NMHDR *pNMHDR, LRESULT *pResult );
      afx_msg BOOL
        OnTvnItemExpanding ( NMHDR *pNMHDR, LRESULT *pResult );
      afx_msg BOOL
        OnNMDblclk ( NMHDR *pNMHDR, LRESULT *pResult );
};

//
//  
TargetcoreMFC_EXT bool
CTreeCtrl_EverExpanded ( CTreeCtrl *pCTreeCtrl, HTREEITEM hItem );
TargetcoreMFC_EXT BOOL
CTreeCtrl_IsExpanded ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem );
TargetcoreMFC_EXT void
CTreeCtrl_ClearUserData( CTreeCtrl *pCTreeCtrl, HTREEITEM hItem );
TargetcoreMFC_EXT void
CTreeCtrl_DeleteAllChildren ( CTreeCtrl *pCTreeCtrl, HTREEITEM hItem );

///////////////////////////////////////////////////////////////////////
//  P2PmsgTreeCtrl extensions

TargetcoreMFC_EXT CString
P2PmsgTreeCtrl_UniqueItemName ( P3PmsgItem& oItem
                              , LPCTSTR lpszItemName, UINT uiCopies = 100 );
TargetcoreMFC_EXT void
CP2PmsgTreeCtrl_PrintItem ( CP2PmsgTreeCtrl *pP2PmsgTreeCtrl, FILE *fd, HTREEITEM hItem );
TargetcoreMFC_EXT void
CP2PmsgTreeCtrl_PrintItemAttr ( CP2PmsgTreeCtrl *pP2PmsgTreeCtrl, FILE *fd, HTREEITEM hItem );
TargetcoreMFC_EXT void
CP2PmsgTreeCtrl_SerialiseSession ( CP2PmsgTreeCtrl& oP2PmsgTreeCtrl, HTREEITEM hItem );
TargetcoreMFC_EXT void
CP2PmsgTreeCtrl_RestoreSession ( CP2PmsgTreeCtrl& oP2PmsgTreeCtrl, HTREEITEM hItem );


//  Sort options associated with above toolbar
constexpr UINT PSort_ALPHA         = (1<<0);
constexpr UINT PSort_ACCESS        = (1<<2);
constexpr UINT PSort_Group_FOLDERS = (1<<4);
constexpr UINT PSort_Group_TYPE    = (1<<5);
TargetcoreMFC_EXT BOOL
CP2PmsgTreeCtrl_Sort4Folder ( CP2PmsgTreeCtrl& wndP2PmsgTreeCtrl, HTREEITEM hItem
                            , UINT uiSParams, CList<CString>& oCListGroups
                            , UINT uiHintsmask = -1 );

//
//  Windows message loop integration prototypes and definitions
//  NOTES: Definition sequence is designed to trap use of the
//         message number elsewhere
constexpr DWORD WM_APP_0x0001       = WM_APP + 0x0001;
constexpr DWORD WM_P2Pmsg_MergeNode = WM_APP_0x0001;
constexpr DWORD WM_APP_0x0002       = WM_APP + 0x0002;
constexpr DWORD WM_P2Pmsg_AddNode   = WM_APP_0x0002;
constexpr DWORD WM_APP_0x0003       = WM_APP + 0x0003;
constexpr DWORD WM_P2Pmsg_DropNode  = WM_APP_0x0003;

