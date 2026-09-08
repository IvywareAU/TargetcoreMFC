// Copyright © 2014, 2023, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  P2PropertiesWnd prototypes and definitions.
//  NOTES: Integrates the CMFCPropertyGridCtrl and P2PmsgMgr objects
//

#pragma once
#include "TargetCoreMFC.h"
#include "P2PeerTarget.h"
#include "P2Pmsg.h"

//
//  P2PmsgSnc message loop integration prototypes and definitions
//  NOTES: Definition sequence is designed to trap use of the
//         message number elsewhere
const DWORD P2PmsgSNC_0x0BF1            = 0x0BF1;
const DWORD P2PmsgSNC_P2PmsgProperties  = P2PmsgSNC_0x0BF1;

class CP2PropertiesToolBar : public CMFCToolBar
{
    public:
	    virtual void
        OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler);
	      //{
        //  MANAGE_RESOURCE_STATE;
		    //  CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	      //}

	    virtual BOOL
        AllowShowOnList() const { return FALSE; }
      BOOL
        OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/);
};

class TargetCoreMFC_EXT CP2PropertiesWnd : public CDockablePane
{
    friend class SafeP2PropsConfig;
    // Constructors and destructor
    public:
	      CP2PropertiesWnd ( LPCTSTR lpszIdentificationTag = 0 );
      virtual
       ~CP2PropertiesWnd ( );

    // Global exposure
    public:
      static CP2PropertiesWnd*
        GetDefaultWnd ( );
      static void
        EmptyDefaultWnd ( );
      static void
        DetachDefaultWnd ( CWnd *pCWndNotify );

    // Toolbar and associated mapping operations
    public:
      virtual void
        ClearToolbarMaps ( );
      virtual BOOL
        SetToolbarURLmap ( LPCTSTR lpszMapTag, LPCTSTR lpszMapURL );

    // Operations
    public:
      void
        InitPropList ( );
      void
        CreatePropList ( GUID oGUID, CWnd *pCWndNotify, UINT nCWndNotifyWM );
      void
        CreatePropList ( P2PeerTarget *pTargetNotify, GUID oGUID, P2PsysID nNotifyP2PsysID );
      void
        SetComboTag ( LPCTSTR lpszComboTag );
      CMFCPropertyGridProperty*
        AddGroup ( CMFCPropertyGridProperty *pGroup );
      CMFCPropertyGridProperty*
        AddGroup_HEADING ( CMFCPropertyGridProperty *pGroupParent
                         , LPCTSTR lpszHeading, LPCTSTR lpszDescription );
      CMFCPropertyGridProperty*
        AddGroup ( CMFCPropertyGridProperty *pGroupParent, LPCTSTR lpszValue
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription );
      CMFCPropertyGridProperty*
        AddItem  ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_COLOR ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_FONT ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_FONTcrx ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_PEN ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_PENcrx ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_BRUSHcrx ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_BNUM ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_ENUM ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_OleDateTime ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_URL ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_CSV ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_Option ( CMFCPropertyGridProperty *pGroupParent
                 , P3PmsgItem& oItem
                 , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                 , P3PmsgAttr *pAttr = nullptr );
      CMFCPropertyGridProperty*
        AddItem_SPIN ( CMFCPropertyGridProperty *pGroupParent
                     , P3PmsgItem& oItem, UINT uiMin, UINT uiMax
                     , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                     , P3PmsgAttr *pAttr = nullptr );
      void
        AttachPropList ( P3PmsgItem *pItemProps, CWnd *pCWndNotify, UINT nCWndNotifyWM );
      void
        DetachPropList ( CWnd *pCWndNotify ); 
      //void
      //  DetachPropList ( P3PmsgNode *pNodeProps );
      void
        InitPropList ( P3PmsgItem *pItem, CMFCPropertyGridProperty *pGroupParent );
      void
        ClearPropList ( );
      void
        SetPropListFont ( );
      void
        AdjustLayout();
    // Filtering
    //public:
    //  CMap_CString&
    //    GetListP2PmsgFilters ( );
    //  bool
    //    HasFilteredItems ( P3PmsgObject& oObject );
    //  bool
    //    IsFilteredItem ( P3PmsgObject& oObject );
    // Property exposure
    public:
      CMFCPropertyGridCtrl*
       GetGridCtrl ( ) noexcept { return &m_wndPropList; }

    // Attributes
    public:
      P3PmsgItem    m_oItems;
      P2PmsgSinkID  m_nP2PmsgSinkID;
      UINT          m_nGridSinkID;
      bool          m_bHiColorIcons;
      P3PmsgItem   *m_pItemProps{nullptr};
      CWnd         *m_pCWndNotify;
      UINT          m_nCWndNotifyWM;
      P2PeerTarget *m_pTargetNotify{nullptr};
      P2PsysID      m_nNotifyP2PsysID{0};
      GUID          m_oGUID{0};
	    void SetVSDotNetLook(BOOL bSet)
	    {
		    m_wndPropList.SetVSDotNetLook(bSet);
		    m_wndPropList.SetGroupNameFullWidth(bSet);
	    }
      //CString              m_strWikiURL;
      //CString              m_strUTubeURL1;
    protected:
      P2PmsgHANDLE         m_hP2PmsgHeap;
      //CMap_CString         m_oCListP2PmsgFilters;
	    CFont                m_fntPropList;
	    CComboBox            m_wndObjectCombo;
	    CP2PropertiesToolBar m_wndToolBar;
	    CMFCPropertyGridCtrl m_wndPropList;
      int                  m_nComboHeight;
      CString              m_strIdentificationTag;
      //CString              m_strGenericURL2;
      //CString              m_strHomePageURL;
      //CString              m_strUserURL1;
      //CString              m_strUserURL2;

// Implementation
public:

    // MFC message map
    protected:
    DECLARE_MESSAGE_MAP()
      afx_msg LRESULT
        OnPropertyChanged ( WPARAM wparam, LPARAM lparam );
      afx_msg int
        OnCreate ( LPCREATESTRUCT lpCreateStruct );
      afx_msg void
        OnSize ( UINT nType, int cx, int cy );
      afx_msg void
        OnExpandAllProperties ( );
      afx_msg void
        OnUpdateExpandAllProperties ( CCmdUI *pCmdUI );
      afx_msg void
        OnSortProperties ( );
      afx_msg void
        OnUpdateSortProperties ( CCmdUI *pCmdUI );
      //afx_msg void
      //  OnProperties1 ( );
      //afx_msg void
      //  OnUpdateProperties1 ( CCmdUI *pCmdUI );
      //afx_msg void
      //  OnProperties2 ( );
      //afx_msg void
      //  OnUpdateProperties2 ( CCmdUI *pCmdUI );
      // Pre-assigned remote Generic and Home page URL's
      // NOTES: Pre-loaded according to context
      /*afx_msg void
        OnBtnWikiURL ( );
      afx_msg void
        OnUpdateBtnWikiURL ( CCmdUI *pCmdUI );
      afx_msg void
        OnBtnUTubeURL1 ( );
      afx_msg void
        OnUpdateBtnUTubeURL1 ( CCmdUI *pCmdUI );
      afx_msg void
        OnBtnHomePageURL1 ( );
      afx_msg void
        OnUpdateBtnHomePageURL1 ( CCmdUI *pCmdUI );
      afx_msg void
        OnBtnUserURL1 ( );
      afx_msg void
        OnUpdateUserURL1 ( CCmdUI *pCmdUI );
      afx_msg void
        OnBtnUserURL2 ( );
      afx_msg void
        OnUpdateUserURL2 ( CCmdUI *pCmdUI );*/
      afx_msg void
        OnSetFocus ( CWnd *pOldWnd );
      afx_msg void
        OnSettingChange ( UINT uFlags, LPCTSTR lpszSection );
};

//
//  Safe P2PropertiesWnd grid configuration container
//  NOTES: Instances of this container can exist in the classes configuring
//         the grid.  When an instance of this safe container goes out of
//         scope the P2PropertiesWnd backward connections are canceled and
//         resources recovered.
//       : In summary protects against orphaned P2PropertiesWnd grids trying
//         to reference back into objects that no longer exist
//
class TargetCoreMFC_EXT SafeP2PropsConfig
{
    // Constructors and destruction
    public:
        SafeP2PropsConfig ( CP2PropertiesWnd *pP2PropsWnd );
        SafeP2PropsConfig ( ) noexcept;
       ~SafeP2PropsConfig ( );
    // Operators
    public:
      SafeP2PropsConfig&
        operator = ( CP2PropertiesWnd *pP2PropsWnd ) noexcept;
        operator CP2PropertiesWnd* ( ) noexcept;
     CP2PropertiesWnd*
        CreatePropList ( P2PeerTarget *pTargetNotify, GUID oGUID, P2PsysID nNotifyP2PsysID );
      void
        Release ( );
      CP2PropertiesWnd*
        r_pP2PropertiesWnd() noexcept { return m_pP2PropertiesWnd; }
      BOOL
        IsLoaded ( const P2PeerTarget *pTarget, GUID oGUID );
    // Attributes
    private:
        GUID              m_oGUID{0};
        CP2PropertiesWnd *m_pP2PropertiesWnd{nullptr};
        P2PeerTarget     *m_pTargetNotify{nullptr};
        P2PsysID          m_nNotifyP2PsysID{0};
};

