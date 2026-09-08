// Copyright © 2011-2015, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  P2PmsgTreeCtrl implementation.
//  NOTES: All objects within a virtual P2Peer network must derive
//         from P2PeerTarget
//

#include "stdafx.h"
//#include "P2Pexplorer.h"
#include "P2PmsgTreeCtrl.h"
#include "CTreeCtrl_Ext.h"

///////////////////////////////////////////////////////////////////////
//  CTReeCtrl helpers

bool
CTreeCtrl_EverExpanded ( CTreeCtrl *pCTreeCtrl, HTREEITEM hItem )
{
    UINT uiState = pCTreeCtrl->GetItemState ( hItem, TVIF_STATE );
    return (uiState&TVIS_EXPANDEDONCE) ? true : false;
}

void
CTreeCtrl_ClearUserData( CTreeCtrl *pCTreeCtrl, HTREEITEM hItem )
{
    if ( hItem == NULL )
      return;
    pCTreeCtrl -> SetItemData ( hItem, 0 );
    HTREEITEM hItemChild = pCTreeCtrl -> GetChildItem ( hItem );
    while ( hItemChild )
    {
      //pCTreeCtrl -> SetItemData ( hItemChild, 0 );
      CTreeCtrl_ClearUserData ( pCTreeCtrl, hItemChild );
      hItemChild = pCTreeCtrl -> GetNextSiblingItem ( hItemChild );
    }
}

void
CTreeCtrl_DeleteAllChildren ( CTreeCtrl *pCTreeCtrl, HTREEITEM hItemParent )
{
    // Delete all of the children of Parent.
    if ( pCTreeCtrl->ItemHasChildren(hItemParent) )
    {
      HTREEITEM hNextItem  = 0;
      HTREEITEM hChildItem = pCTreeCtrl -> GetChildItem(hItemParent);
      pCTreeCtrl -> Expand ( hItemParent, TVE_COLLAPSE|TVE_COLLAPSERESET );
      while ( hChildItem != NULL )
      {
        if ( pCTreeCtrl->ItemHasChildren(hChildItem) )
          CTreeCtrl_DeleteAllChildren(pCTreeCtrl,hChildItem);
        hNextItem = pCTreeCtrl -> GetNextItem ( hChildItem, TVGN_NEXT );
        pCTreeCtrl -> DeleteItem ( hChildItem );
        hChildItem = hNextItem;
      }
    }
    pCTreeCtrl -> SetItemState ( hItemParent, 0 , TVIF_CHILDREN );
    //pCTreeCtrl -> Expand ( hItemParent, TVE_COLLAPSE|TVE_COLLAPSERESET );
}
    /*TVINSERTSTRUCT oTVS = {0};
    oTVS.item.mask      = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_CHILDREN|TVIF_PARAM;
    CString strText = oItem.c_name();
    oTVS.item.pszText   = strText.GetBuffer();         //LPSTR_TEXTCALLBACK;
	  oTVS.item.iImage    = I_IMAGECALLBACK;
	  oTVS.item.iSelectedImage = I_IMAGECALLBACK;
    oTVS.item.cChildren = I_CHILDRENCALLBACK;
    oTVS.item.lParam    = posP2Pobject;
    oTVS.hParent        = hItemParent;
    oTVS.hInsertAfter   = TVI_LAST;
    HTREEITEM hItem = InsertItem ( &oTVS ) ;*/

BOOL
CTreeCtrl_IsExpanded ( CTreeCtrl& oCTreeCtrl, HTREEITEM hItem )
{
    return oCTreeCtrl.GetItemState(hItem,TVIS_EXPANDED) & TVIS_EXPANDED ? TRUE : FALSE; 
}

//
//  Serialises the P2PmsgTreeCtrl session
//  NOTES: Usually performed immediately before underlying P2PmsgMgr contents
//         are saved.
//       : Refer CP2PmsgTreeCtrl_DeserialiseSession() for restoration
//
//  Parameters:  CTreeCtrl *pCTreeCtrl
//               Session to be serialised
//
//               HTREEITEM hItem
//               CtreeCtrl item at which serialisation is to commence
//
void
CP2PmsgTreeCtrl_SerialiseSession ( CP2PmsgTreeCtrl& oP2PmsgTreeCtrl, HTREEITEM hItem )
{
    // Process current item
    if ( hItem == nullptr )
      return;
    const BOOL bExpanded = CTreeCtrl_IsExpanded ( oP2PmsgTreeCtrl, hItem );
    P3PmsgObject oObject = oP2PmsgTreeCtrl.GetItemObject ( hItem );
    if ( oObject.IsField() ) {
      P3PmsgItem oItem = oObject;
      if ( bExpanded )
        oItem.r_Desc().SetPermissions ( AttrField_EXPAND, 0 );
      else
        oItem.r_Desc().SetPermissions ( 0, AttrField_EXPAND );
    }
    // Process all the children
    HTREEITEM hItemChild = oP2PmsgTreeCtrl.GetChildItem ( hItem );
    while ( hItemChild )
    {
      CP2PmsgTreeCtrl_SerialiseSession ( oP2PmsgTreeCtrl, hItemChild );
      hItemChild = oP2PmsgTreeCtrl.GetNextSiblingItem ( hItemChild );
    }
}
TargetCoreMFC_EXT void
CP2PmsgTreeCtrl_RestoreSession ( CP2PmsgTreeCtrl& oP2PmsgTreeCtrl, HTREEITEM hItem )
{
    // Observe serialised session expansion
    if ( hItem == nullptr )
      return;
    P3PmsgItem oItemRestore = oP2PmsgTreeCtrl.GetItemObject(hItem);
    if ( oItemRestore.r_Desc().GetPermissions(AttrField_EXPAND) == 0 )
      return;
    oItemRestore.r_Desc().SetPermissions ( 0, AttrField_EXPAND );
    oP2PmsgTreeCtrl.Expand ( hItem, TVE_EXPAND );
    // Now process all children
    HTREEITEM hItemChild = oP2PmsgTreeCtrl.GetChildItem ( hItem );
    while ( hItemChild )
    {
      CP2PmsgTreeCtrl_RestoreSession ( oP2PmsgTreeCtrl, hItemChild );
      hItemChild = oP2PmsgTreeCtrl.GetNextSiblingItem ( hItemChild );
    }
}

// CP2PmsgTreeCtrl

IMPLEMENT_DYNAMIC(CP2PmsgTreeCtrl, CTreeCtrl)

CP2PmsgTreeCtrl::CP2PmsgTreeCtrl()
{
    m_pP2PmsgMgr_Ext = nullptr;
    m_pP2PmsgMgr     = nullptr;
    m_hRootItem      = 0;
    m_hWndTriggers   = 0;
}

CP2PmsgTreeCtrl::~CP2PmsgTreeCtrl()
{
    if ( m_pP2PmsgMgr != m_pP2PmsgMgr_Ext )
      delete m_pP2PmsgMgr;
    m_pP2PmsgMgr     = 0;
    m_pP2PmsgMgr_Ext = 0;
}

bool
CP2PmsgTreeCtrl::Connect ( P2PmsgMgr *pP2PmsgMgr, P2Pos posRoot
                         , HWND hWndTriggers
                         , int nRootImage, int nRootSelectedImage )
{
    m_hWndTriggers = hWndTriggers;
    if ( pP2PmsgMgr )
    {
      ASSERT(m_pP2PmsgMgr==0);
      m_pP2PmsgMgr     = pP2PmsgMgr;
      ASSERT(m_pP2PmsgMgr_Ext==0);
      m_pP2PmsgMgr_Ext = pP2PmsgMgr;
    }

    // Ensure root visiblity
    if ( m_hRootItem == 0 &&
         m_pP2PmsgMgr        )
    {
      LPCTSTR lpszRootName = m_pP2PmsgMgr->c_name();
      ASSERT(lpszRootName&&wcslen(lpszRootName)>0);
	    m_hRootItem = InsertItem ( lpszRootName, nRootImage, nRootSelectedImage, TVI_ROOT, 0);
	    SetItemState ( m_hRootItem, TVIS_BOLD, TVIS_BOLD);
      //P2Pos posRoot = m_pP2PmsgMgr->GetP2Pos();
      SetItemData  ( m_hRootItem, posRoot );
      m_oCMapHTREE.SetAt ( posRoot, m_hRootItem );
    }

    // Ensure first child visiblity
    if ( m_hRootItem  &&
           pP2PmsgMgr    )
    {
      P3PmsgItem oItemRoot = m_pP2PmsgMgr->P2Pos2Object(posRoot);
      P3PmsgCurs oCurs(oItemRoot);
      for ( int i = 0; oCurs.Goto(i); i++ )
      {
        if ( oCurs.IsItem() )
          InsertP2Pmsg ( m_hRootItem, oCurs.r_item() );
        //else if ( oCurs.IsNode() )
        //  InsertP2Pmsg ( m_hRootItem, oCurs.r_node() );
        else if ( oCurs.IsList() )
          ASSERT(0);
        else if ( oCurs.IsItem() )
          ASSERT(0);
        else ASSERT(0);
      }

      // Create root trigger
      if ( m_hWndTriggers )
        m_pP2PmsgMgr -> CreateTrigger ( P2PmsgTrig_ALL, m_hWndTriggers, oItemRoot.GetP2Pos() );
    }

    // Observe serialised session expansion
    CP2PmsgTreeCtrl_RestoreSession ( *this, m_hRootItem );

    // Tidy up, and
    SelectCutItem ( 0 );               // Clears previous cut item selection
    return true;
}

void
CP2PmsgTreeCtrl::RecyleThisObject ( )
{
    SelectCutItem ( 0 );               // Clears previous cut item selection
    if ( m_hWnd )
      DeleteAllItems ( );
    m_hRootItem = 0;
    m_hItemCut  = 0;
    if ( m_pP2PmsgMgr != m_pP2PmsgMgr_Ext )
      delete m_pP2PmsgMgr;
    m_pP2PmsgMgr     = nullptr;
    m_pP2PmsgMgr_Ext = nullptr;
}

///////////////////////////////////////////////////////////////////////
//  Notifications
//  NOTES: Asynchronously reflected back to parent

bool
CP2PmsgTreeCtrl::NotifyP2Pmsg_AddNode ( CWnd *pCWndNotify
                                      , const P3PmsgItem& oNodeParent
                                      , const P3PmsgItem& oNodeChild )
{
    // Locals
    P2Pos posParent = oNodeParent.GetP2Pos ( );
    HTREEITEM hItemParent = 0;
    m_oCMapHTREE.Lookup ( posParent, hItemParent );

    // Parent has no form
    // NOTES: Populated upon parent expansion
    if ( hItemParent == NULL )
      return false;

    // GrandParent has never been expanded
    // NOTES: Populated upon grand parent expansion
    //HTREEITEM hItemGParent = GetParentItem ( hItemParent );
    //if (  hItemGParent                              &&
    //     !CTreeCtrl_EverExpanded(this,hItemGParent)    )
    //  return false;

    // Establish visiblity parent node
    // NOTES:
    InsertP2Pmsg ( hItemParent, oNodeChild );
    ASSERT(0);
    pCWndNotify -> PostMessage ( WM_P2Pmsg_AddNode
                               , oNodeParent.GetP2Pos()
                               , oNodeChild.GetP2Pos() );
    return true;
}

/*bool
CP2PmsgTreeCtrl::NotifyP2Pmsg_MergeNode ( CWnd *pCWndNotify
                                        , const P3PmsgNode& oNode )
{
    // Visiblity
    HTREEITEM   hItem;
    P2Pos     posNode = oNode.GetP2Pos();
    if ( !m_oCMapHTREE.Lookup(posNode,hItem) ||
            hItem == 0                          )
      return false;

    // Tidy up, and
    ASSERT(0);
    pCWndNotify -> PostMessage ( WM_P2Pmsg_MergeNode
                               , oNode.GetP2Pos(), 0 );
    return true;
}*/

///////////////////////////////////////////////////////////////////////
//  Operations

//
//  Inserts new item into the CTreeCtrl
//
//  Parameters:  HTREEITEM hItemParent
//
//               const P3PmsgNode& oNode
//
//  Returns:     HTREEITEM
//               Handle for inserted item
/*HTREEITEM
CP2PmsgTreeCtrl::InsertP2Pmsg ( HTREEITEM hItemParent, const P3PmsgNode& oNode )
{
    //LPCTSTR  lpszNodename  = oNode.c_name();
	  //HTREEITEM   hItem      = InsertItem ( lpszNodename
    //                                    , I_IMAGECALLBACK | I_CHILDRENCALLBACK
    //                                    , 1, hItemParent );
    //P2Pos     posP2Pobject = oNode.GetP2Pos ( );
    //ASSERT(posP2Pobject);
    //SetItemData ( hItem, posP2Pobject );
    //m_oCMapHTREE.SetAt ( posP2Pobject, hItem );
//-----------------------------------------------------------------------------------
    P2Pos  posP2Pobject = oNode.r_Object().GetP2Pos ( );
ASSERT(posP2Pobject<1000000);
    TVINSERTSTRUCT oTVS = {0};
    oTVS.item.mask      = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_CHILDREN|TVIF_PARAM;
    CString strText = oNode.c_name();
    oTVS.item.pszText   = strText.GetBuffer();         //LPSTR_TEXTCALLBACK;
	  oTVS.item.iImage    = I_IMAGECALLBACK;
	  oTVS.item.iSelectedImage = I_IMAGECALLBACK;
    oTVS.item.cChildren = I_CHILDRENCALLBACK;
    oTVS.item.lParam    = posP2Pobject;
    oTVS.hParent        = hItemParent;
    oTVS.hInsertAfter   = TVI_LAST;
    HTREEITEM hItem = InsertItem ( &oTVS ) ;
    ASSERT(hItem);
    m_oCMapHTREE.SetAt ( posP2Pobject, hItem );
//-----------------------------------------------------------------------------------
    // Set icon details
    // NOTES: Derived class specialises GetImage() and GetSelectedImage()
    //        according to requirements.  Default implementation provide
    //        indices of 0 and 1 respectively
    //SetItemImage ( hItem, GetImage(posP2Pobject), GetSelectedImage(posP2Pobject) );

    // Create triggers
    if ( m_hWndTriggers )
      m_pP2PmsgMgr -> CreateTrigger ( P2PmsgTrig_ALL, m_hWndTriggers, oNode.GetP2Pos() );
    return hItem;
}*/

HTREEITEM
CP2PmsgTreeCtrl::InsertP2Pmsg ( HTREEITEM hItemParent, const P3PmsgItem& oItem )
{
    P2Pos  posP2Pobject = oItem.r_Object().GetP2Pos ( );
ASSERT(posP2Pobject<200000000);
    TVINSERTSTRUCT oTVS = {0};
    oTVS.item.mask      = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_CHILDREN|TVIF_PARAM;
    CString strText = oItem.c_name();
    oTVS.item.pszText   = strText.GetBuffer();         //LPSTR_TEXTCALLBACK;
	  oTVS.item.iImage    = I_IMAGECALLBACK;
	  oTVS.item.iSelectedImage = I_IMAGECALLBACK;
    oTVS.item.cChildren = I_CHILDRENCALLBACK;
    oTVS.item.lParam    = posP2Pobject;
    oTVS.hParent        = hItemParent;
    oTVS.hInsertAfter   = TVI_LAST;
    HTREEITEM hItem = InsertItem ( &oTVS ) ;
    ASSERT(hItem);
    m_oCMapHTREE.SetAt ( posP2Pobject, hItem );

    // Create triggers
    if ( m_hWndTriggers )
      m_pP2PmsgMgr -> CreateTrigger ( P2PmsgTrig_ALL, m_hWndTriggers, oItem.GetP2Pos() );
    // Refresh underlying P2Pmsg structure
    RefreshP2Pmsg ( hItem );
    return hItem;
}

//HTREEITEM
//CP2PmsgTreeCtrl::UpdateP2Pmsg ( HTREEITEM hItem, const P3PmsgNode& oNode )
//{
//    LPCTSTR  lpszNodename = oNode.c_name();
//    SetItemText ( hItem, lpszNodename );
//    return hItem;
//}

HTREEITEM
CP2PmsgTreeCtrl::UpdateP2Pmsg ( HTREEITEM hItem, const P3PmsgItem& oItem )
{
    LPCTSTR  lpszItemname = oItem.c_name();
    SetItemText ( hItem, lpszItemname );
    return hItem;
}

//
//  Recovers previously serialised session state
//
//  Parameters:  HTREEITEM hItem
//               Item for which serialised state is to be recovered
void
CP2PmsgTreeCtrl::RecoverP2Pmsg ( HTREEITEM hDispItem, P3PmsgItem& oDispItem )
{
      // Process possible session startup expansion
      if ( oDispItem.r_Desc().GetPermissions(AttrField_EXPAND) ) {
        CTreeCtrl_AsyncExpand ( *this, hDispItem );
      }
}
///////////////////////////////////////////////////////////////////////
//  CTreeCtrl extensions

LPARAM
CP2PmsgTreeCtrl::MoveItemCB ( CTreeCtrl *pCTreeCtrl
                    , HTREEITEM hItemSource, HTREEITEM hItemDestin )
{
    // Locals
    CP2PmsgTreeCtrl *pThis = dynamic_cast<CP2PmsgTreeCtrl*>(pCTreeCtrl);
    ASSERT(pThis);
    P2PmsgMgr       *pP2PmsgMgr = &pThis -> GetP2PmsgMgr();
    ASSERT(pP2PmsgMgr);

    // Source trigger configuration
    P2Pos posSource  = pThis -> GetItemData ( hItemSource );
                       pThis -> SetItemData ( hItemSource, 0 );
    DWORD dwTrigMask = pP2PmsgMgr -> SelectTrigger ( posSource, pCTreeCtrl->m_hWnd );
    pP2PmsgMgr -> DropTriggers ( TRIGGER_ALL, pCTreeCtrl->m_hWnd, posSource );

    // Destin trigger configuration
    if ( dwTrigMask )
      pP2PmsgMgr -> CreateTrigger ( dwTrigMask, pCTreeCtrl->m_hWnd, posSource );
    pThis -> SetItemData ( hItemDestin, posSource );
    ASSERT(posSource);
    return posSource;
}

void
CP2PmsgTreeCtrl::ShuffleItemUp ( HTREEITEM hItem )
{
    SelectCutItem ( 0 );               // Clears previous cut item selection
    // Isolate selected P2PeerHub
    // NOTES: Skip root item
    //HTREEITEM hItem = m_wndFileView.GetSelectedItem ( );
    if ( hItem == NULL )
      return;
    HTREEITEM hItemParent = GetParentItem ( hItem );
    if ( hItemParent == NULL )
      return;
    P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return;
    HTREEITEM hItemPrev = GetPrevSiblingItem ( hItem );
    if ( hItemPrev == NULL )
      return;
    HTREEITEM hItemAfter = GetPrevSiblingItem ( hItemPrev );
    if ( hItemAfter == NULL )
      hItemAfter = TVI_FIRST;
    P2Pos posItemPrev = GetItemData ( hItemPrev );
    if ( posItemPrev == NULL )
      return;

    // Isolate tree clearance
    try
    {
      //P2PmsgMgr *pP2PmsgMgr = theApp.GetP2PmsgMgr();
      P3PmsgItem oItem  = m_pP2PmsgMgr->P2Pos2Object ( posItem );
      P3PmsgItem oPrev  = m_pP2PmsgMgr->P2Pos2Object ( posItemPrev );
      // Clear Triggers

      // Move P2Pmsg item up
      ASSERT(0);
      //P2PmsgNode_Swap ( oItem, oPrev );

      // Move visual tree item up
      CTreeCtrl_MoveItem ( this, hItem, hItemParent, hItemAfter
                         , CP2PmsgTreeCtrl::MoveItemCB );
      //m_wndFileView.MoveItemUp ( hItem );

      // Re-establish triggers
    }

    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel
}

void
CP2PmsgTreeCtrl::ShuffleItemDown ( HTREEITEM hItem )
{
    UNREFERENCED_PARAMETER(hItem);ASSERT(0);
    SelectCutItem ( 0 );               // Clears previous cut item selection
}

///////////////////////////////////////////////////////////////////////
//  Properties

HTREEITEM
CP2PmsgTreeCtrl::IsItem ( P2Pos posItem )
{
    // Map dependancy
    HTREEITEM hItem = 0;
    if ( !m_oCMapHTREE.Lookup(posItem,hItem) ||
            hItem == 0                          )
      return 0;
    if (  m_pP2PmsgMgr->IsField(posItem) )
      return hItem;
    //if (  m_pP2PmsgMgr->IsNode(posItem) ) {
    //  ASSERT(0);
    //  return hItem;
    //}
    return 0;
}

P2Pos
CP2PmsgTreeCtrl::IsTreeFolder ( HTREEITEM hItem )
{
    // Map dependancy
    if ( hItem == NULL )
      return 0;                        // Nothing
    const P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return 0;                        // Nothing
    if ( m_pP2PmsgMgr->IsField(posItem) )
    {
    //ASSERT(AfxCheckMemory());
      P3PmsgItem oItem = m_pP2PmsgMgr->P2Pos2Object(posItem);
      if ( oItem.ATTR.Exists(L"$Folder") )
        return posItem;
    }
    return 0;
}

P2Pos
CP2PmsgTreeCtrl::IsItem ( HTREEITEM hItem )
{
    // Map dependancy
    if ( hItem == NULL )
      return 0;                        // Nothing
    const P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return 0;                        // Nothing
    if ( m_pP2PmsgMgr->IsField(posItem) )
      return posItem;
    return 0;
}

P2PmsgMgr&
CP2PmsgTreeCtrl::GetP2PmsgMgr ( )
{
    return *m_pP2PmsgMgr;
}

P3PmsgItem
CP2PmsgTreeCtrl::GetItemItem ( HTREEITEM hItem )
{
    P3PmsgItem oItem;
    // Isolate passed item
    if ( hItem == NULL )
      return oItem;
    const P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return oItem;
    return m_pP2PmsgMgr->P2Pos2Field ( posItem );
}

P3PmsgObject
CP2PmsgTreeCtrl::GetItemObject ( HTREEITEM hItem )
{
    P3PmsgObject oObject;
    // Isolate passed item
    if ( hItem == NULL )
      return oObject;
    const P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return oObject;
    return m_pP2PmsgMgr->P2Pos2Object ( posItem );
}

P2Pos
CP2PmsgTreeCtrl::GetItemP2Pos ( HTREEITEM hItem )
{
    // Isolate passed item
    if ( hItem == NULL )
      return 0u;
    return GetItemData ( hItem );
}

P3PmsgAttr
CP2PmsgTreeCtrl::GetItemAttr ( HTREEITEM hItem )
{
    P3PmsgAttr oAttr;
    // Isolate passed item
    if ( hItem == NULL )
      return oAttr;
    const P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return oAttr;
    return m_pP2PmsgMgr->P2Pos2Attr ( posItem );
}

CString
CP2PmsgTreeCtrl::GetItemPath ( HTREEITEM hItem )
{
    // Isolate passed item
    if ( hItem == NULL )
      return CString();
    const P2Pos posItem = GetItemData ( hItem );
    if ( posItem == NULL )
      return CString();
    return m_pP2PmsgMgr->P2Pos2Path ( posItem );
}

CString
CP2PmsgTreeCtrl::GetSelectedPath ( )
{
    // Isolate selected item
    HTREEITEM hItem = GetSelectedItem ( );
    if ( hItem == NULL )
      return CString();
    return GetItemPath(hItem);
}

CMap_CString&
CP2PmsgTreeCtrl::GetListP2PmsgFilters ( )
{
    return m_oCListP2PmsgFilters;
}

bool
CP2PmsgTreeCtrl::HasFilteredItems ( P3PmsgObject& oObject )
{
    if ( m_oCListP2PmsgFilters.IsEmpty() )
      return true;
    P3PmsgAttr oAttr;
    if ( oObject.IsField() )
      oAttr = P3PmsgItem(oObject).r_Attr().r_Object();
    //else if ( oObject.IsNode() )
    //  oAttr = P3PmsgNode(oObject).r_Attr().r_Object();
    else { ASSERT(0); return false; }

    // Apply filters
    POSITION pos = m_oCListP2PmsgFilters.GetHeadPosition();
    while ( pos )
    {
      LPCTSTR lpszName = m_oCListP2PmsgFilters.GetNext(pos);
      if ( oAttr.Exists(lpszName) )
        return true;
    }
    return false;
}
bool
CP2PmsgTreeCtrl::IsFilteredItem ( P3PmsgObject& oObject )
{
    if ( m_oCListP2PmsgFilters.IsEmpty() )
      return true;
    P3PmsgAttr oAttr;
    if ( oObject.IsField() )
      oAttr = P3PmsgField(oObject).r_Attr().r_Object();
    //else if ( oObject.IsNode() )
    //  oAttr = P3PmsgNode(oObject).r_Attr().r_Object();
    else return false;

    // Apply filters
    POSITION pos = m_oCListP2PmsgFilters.GetHeadPosition();
    while ( pos )
    {
      LPCTSTR lpszName = m_oCListP2PmsgFilters.GetNext(pos);
      if ( oAttr.Exists(lpszName) )
        return true;
    }
    return false;
}

//
//  Fetch item beneath cursor
//         to requirements
//
//  Parameters:  UINT *puiFlags
//               CTreeCtrl::HitTest() associated flags
//
//  Returns:     HTREEITEM
//               Identified CTreeCtrl item
HTREEITEM
CP2PmsgTreeCtrl::GetCursorItem ( UINT *puiFlags )
{
    CPoint    oCPoint;
    GetCursorPos   ( &oCPoint );
    ScreenToClient ( &oCPoint );
    UINT      uiFlags;
    HTREEITEM hItem = HitTest ( oCPoint, &uiFlags);
    if ( puiFlags )
      *puiFlags = uiFlags;
    return hItem;
}

//
//  Select item from path
//
//  Parameters:  LPCTSTR lpszItemPath
//               Item path to be selected
//
//  Returns:     HTREEITEM
//               Selected item
//                 0.. Not located
HTREEITEM
CP2PmsgTreeCtrl::SelectRootItem ( LPCTSTR lpszItemPath )
{
    Expand ( TVI_ROOT, TVE_EXPAND );
    if ( *lpszItemPath != T_RootDelim )
      return NULL;
    HTREEITEM hItemParent = TVI_ROOT;
    while ( hItemParent && *lpszItemPath )
    {
      CString strNextItem;
      lpszItemPath++;
      while ( *lpszItemPath != T_DescDelim && *lpszItemPath )
        strNextItem += *lpszItemPath++;
      if ( strNextItem.IsEmpty() )
        return 0;
      hItemParent = CTreeCtrl_FindItem ( *this, hItemParent, strNextItem );
      if ( hItemParent )
        Expand ( hItemParent, TVE_EXPAND );
    }
    return hItemParent;
}

//
//  Manages selection of cut item
//  NOTES: Certain tree operations automatically cancel selected cut item.
//
//  Parameters:  HTREEITEM hItemCut
//               Item to be cut
//                 0.. Clears cut selection item
//
//  Returns:     HTREEITEM
//               Selected item
//                 0.. Nothing selected
HTREEITEM
CP2PmsgTreeCtrl::SelectCutItem ( HTREEITEM hItemCut )
{
    // Verify item exists
    if ( hItemCut )
    {
      const P2Pos posItem = GetItemData ( hItemCut );
      if ( posItem == NULL )
        hItemCut = 0;
    }

    // Tidy up, and
    m_hItemCut = hItemCut;
    return m_hItemCut;
}

//
//  Fetch normal CImageList index for passed P2Pobject address
//  NOTES: Specialise GetImage() and GetSelectedImage() according
//         to requirements
//
//  Returns:     int
//               Default CImageList index
//int
//CP2PmsgTreeCtrl::GetImage ( P2Pos posP2Pobject )
//{
//    posP2Pobject;
//    return 0;
//}
//int
//CP2PmsgTreeCtrl::GetSelectedImage ( P2Pos posP2Pobject )
//{
//    posP2Pobject;
//    return 1;
//}

///////////////////////////////////////////////////////////////////////
//  MFC Virtual overrides

BOOL
CP2PmsgTreeCtrl::OnNotify ( WPARAM wParam, LPARAM lParam, LRESULT *pResult )
{
	  const BOOL   bRes   = CTreeCtrl::OnNotify(wParam, lParam, pResult);
	  const NMHDR *pNMHDR = (NMHDR *)lParam;
	  ASSERT(pNMHDR != NULL);
	  if ( pNMHDR                    && 
         pNMHDR->code  == TTN_SHOW &&
         GetToolTips() != NULL)
		  GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
    return bRes;
}

//  MFC Message handlers
BEGIN_MESSAGE_MAP(CP2PmsgTreeCtrl, CTreeCtrl)
    ON_MESSAGE(WM_P2Pmsg_TrigINSERT,OnP2PmsgTrigINSERT)
    ON_MESSAGE(WM_P2Pmsg_TrigUPDATE,OnP2PmsgTrigUPDATE)
    ON_MESSAGE(WM_P2Pmsg_TrigDELETE,OnP2PmsgTrigDELETE)
    ON_NOTIFY_REFLECT ( TVN_DELETEITEM, &CP2PmsgTreeCtrl::OnTvnDeleteItem )
    ON_NOTIFY_REFLECT_EX ( TVN_GETDISPINFO, &CP2PmsgTreeCtrl::OnTvnGetDispInfo )
    ON_NOTIFY_REFLECT_EX ( TVN_ITEMEXPANDING, &CP2PmsgTreeCtrl::OnTvnItemExpanding)
    ON_NOTIFY_REFLECT_EX (NM_DBLCLK, &CP2PmsgTreeCtrl::OnNMDblclk)
END_MESSAGE_MAP()

LRESULT
CP2PmsgTreeCtrl::OnP2PmsgTrigINSERT ( WPARAM wParam, LPARAM lParam )
{
    const P2Pos  posItem    = static_cast<P2Pos>(wParam);
    P2PmsgMgr   *pP2PmsgMgr = (P2PmsgMgr *)lParam;
    HTREEITEM    hItem      = IsItem ( posItem );
    if ( hItem == 0 || posItem == 0 || pP2PmsgMgr == nullptr )
      return 0;                        // Inappropriate
    if ( pP2PmsgMgr != &GetP2PmsgMgr() )
      return 0;
    //P3PmsgNode oNode = pP2PmsgMgr->P2Pos2NodeHdl ( posItem );
    P3PmsgCurs oCurs;
    if ( pP2PmsgMgr->IsField(posItem) )
      oCurs = pP2PmsgMgr->P2Pos2Field ( posItem );
    //else if ( pP2PmsgMgr->IsNode(posItem) )
    //  oCurs = pP2PmsgMgr->P2Pos2Node ( posItem );
    else return 0;

    // Process missing tree nodes
    for ( int i = 0; oCurs.Goto(i); i++ )
    {
      if ( IsItem(oCurs.r_Object().GetP2Pos()) )
        continue;                      // Already exists
      if ( oCurs.IsItem() )
        InsertP2Pmsg ( hItem, oCurs.r_item() );
      //else if ( oCurs.IsNode() )
      //  InsertP2Pmsg ( hItem, oCurs.r_node() );
      else ASSERT(0);
    }

    // Tidy up and
    Expand ( hItem, TVE_EXPAND );
    return 0;
}
//
//  WM_P2Pmsg_TrigUPDATE handler
//
//  Parameters:  WPARAM wParam
//               P2Pos address of updated item
//
//               LPARAM lParam
//               Pointer to P2PmsgMgr from which notification posted
//
LRESULT
CP2PmsgTreeCtrl::OnP2PmsgTrigUPDATE ( WPARAM wParam, LPARAM lParam )
{
    const P2Pos  posItem    = static_cast<P2Pos>(wParam);
    P2PmsgMgr   *pP2PmsgMgr = (P2PmsgMgr *)lParam;
    HTREEITEM    hItem      = IsItem ( posItem );
    if ( hItem == 0 || posItem == 0 || pP2PmsgMgr == nullptr )
      return 0;                        // Inappropriate
    if ( pP2PmsgMgr != &GetP2PmsgMgr() )
      return 0;

    // Isolate nominated item
    if ( GetItemData(hItem) != posItem )
      return 0;

    // Perform update
    if ( IsItem(posItem) )
    {
      P3PmsgItem oItem = pP2PmsgMgr->P2Pos2Field ( posItem );
      UpdateP2Pmsg ( hItem, oItem );
    }
    //else if ( IsNode(posItem) )
    //{
    //  P3PmsgNode oNode = pP2PmsgMgr->P2Pos2NodeHdl ( posItem );
    //  UpdateP2Pmsg ( hItem, oNode );
    //}
    else { ASSERT(0); }

    // Tidy up and
    return 0;
}
//
//  WM_P2Pmsg_TrigDELETE handler
//  NOTES: Assume posItem no longer exists
//
//  Parameters:  WPARAM wParam
//               P2Pos address of deleted item.  Assumed not to exist.
//
//               LPARAM lParam
//               Pointer to P2PmsgMgr from which notification posted
//
LRESULT
CP2PmsgTreeCtrl::OnP2PmsgTrigDELETE ( WPARAM wParam, LPARAM lParam )
{
    const P2Pos  posItem    = static_cast<P2Pos>(wParam);
    P2PmsgMgr   *pP2PmsgMgr = (P2PmsgMgr *)lParam;
    HTREEITEM    hItem      = 0; m_oCMapHTREE.Lookup(posItem,hItem);
    if ( hItem == 0 || posItem == 0 || pP2PmsgMgr == nullptr )
      return 0;                        // Inappropriate
    if ( pP2PmsgMgr != &GetP2PmsgMgr() )
      return 0;

    // Isolate nominated item
    // NOTES: Skip root item, always remains, remove the key
    const HTREEITEM hItemParent = GetParentItem ( hItem );
    if ( hItemParent == NULL )
      return 0;
    m_oCMapHTREE.RemoveKey ( posItem );
    ASSERT(posItem == GetItemData(hItem) );
    //if ( posItem == NULL )
    //  return 0;

    // Recursively and synchronously clean up triggers for this window
    // NOTES: Nothing to fire back to, all other triggers still
    //        remain in place.  Registration self delete
    //P2PmsgMgr *pP2PmsgMgr = theApp.GetP2PmsgMgr();
    CTreeCtrl_ClearUserData ( this, hItem );
    //P3PmsgNode oNode = m_pPDStoreMgr -> P2Pos2NodeHdl ( posItem );
    //pP2PmsgMgr -> DropTrigger ( posItem, m_hWnd, ~0, true );

    // Recursively and asynchronously activate DELETE triggers
    //m_pPDStoreMgr -> TriggerDELETE ( oNode );
    DeleteItem ( hItem );
ASSERT(!m_oCMapHTREE.Lookup(posItem,hItem));
    return 0;
}

void
CP2PmsgTreeCtrl::OnTvnDeleteItem ( NMHDR *pNMHDR, LRESULT *pResult )
{
    // Locals
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    // House keeping
    P2Pos posP2Pobject = pNMTreeView -> itemOld.lParam;
    m_oCMapHTREE.RemoveKey ( posP2Pobject );
    if ( posP2Pobject &&
         m_pP2PmsgMgr    )
      m_pP2PmsgMgr -> DropTriggers ( TRIGGER_ALL, m_hWnd, posP2Pobject );

    // Tidy up, and
   *pResult = 0;
}

//
//  TVN_GETDISPINFO notification
//  NOTES: Handled prior to equivalent parent notification
//
BOOL
CP2PmsgTreeCtrl::OnTvnGetDispInfo ( NMHDR *pNMHDR, LRESULT *pResult )
{
    // Locals
    LPNMTVDISPINFO   pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    TVITEM          *pTVITEM     = &pTVDispInfo -> item;
    HTREEITEM        hItem       =  pTVDispInfo -> item.hItem;
    int              cChildren   = 0;

    // Exposure
    // NOTES: Full tree is exposed in _DEBUG mode, only immediate non-folder
    //        items in other modes.
    if ( IsTreeFolder(hItem) )
    {
      P2Pos     posItem = GetItemP2Pos ( hItem );
      P3PmsgItem  oItem = GetP2PmsgMgr().P2Pos2Object(posItem);
      cChildren = oItem.r_Desc().GetCount();
    }
#ifdef _DEBUG
    else if ( IsItem(hItem) )
      cChildren = GetItemItem(hItem).r_Desc().GetCount();
    else return TRUE;
#endif

    // Children
    // NOTES: Item MUST be created and inserted with equivalent mask
    //      : Assigned value may be modified in derived class
    if ( (pTVITEM->mask&TVIF_CHILDREN) == TVIF_CHILDREN )
      pTVITEM -> cChildren = cChildren;

   *pResult = TRUE;
    return FALSE;                      // Flag parent notification
}
//
//  TVN_ITEMEXPANDING notification
//  NOTES: Handled BEFORE equivalent parent notification
//
BOOL
CP2PmsgTreeCtrl::OnTvnItemExpanding ( NMHDR *pNMHDR, LRESULT *pResult )
{
    // Locals
    LPNMTREEVIEW  pNMTreeView =  reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    TVITEM       *pTVITEM     = &pNMTreeView -> itemNew;
    P2Pos       posParent     =  pTVITEM -> lParam;
    HTREEITEM     hItemParent =  pTVITEM -> hItem;

    // To be sure, to be sure
   *pResult = 0;
    if ( posParent == NULL )
      return FALSE;
//static int iTimes=0;
//if(++iTimes%2==0)
//return;
//CTreeCtrl
    // Confirm children fully populated
    HTREEITEM hItemLoop = GetChildItem ( hItemParent );
hItemLoop=0; //TODO: Resolve masking out loop
    while ( hItemLoop )
    {
      HTREEITEM hItemChild = hItemLoop;
      hItemLoop = GetNextSiblingItem ( hItemLoop );
      if ( ItemHasChildren(hItemChild) )
        continue;
      P2Pos posChild = GetItemData ( hItemChild );
      if ( posChild == NULL )
        continue;

//problem lies here
// TODO: Displaced by logic below 23/09/2013 vvvvv
//      P3PmsgNode oNodeParent = m_pP2PmsgMgr -> P2Pos2NodeHdl ( posChild );
//      P3PmsgCurs oCurs ( oNodeParent );
//      for ( int i = 0; oCurs.Goto(i); i++ )
//      {
//        if ( oCurs.IsNode() )
//          InsertP2Pmsg ( hItemParent, oCurs.r_node() );
//        else if ( oCurs.IsList() )
//          ASSERT(0);
//        else if ( oCurs.IsField() )
//          InsertP2Pmsg ( hItemChild, oCurs.r_field() );
//        else
//          ASSERT(0);
//      }
// TODO: Displace by logic below 23/09/2013 ^^^^
    }

    // TODO: Added 23/09/2013 vvvv
    // Confirm all P2PmsgItem children exist in the CTreeCtrl
    //P3PmsgNode oNodeParent = m_pP2PmsgMgr -> P2Pos2NodeHdl ( posParent );
    //P3PmsgCurs oCurs ( oNodeParent );
    P3PmsgCurs oCurs;
    if ( m_pP2PmsgMgr->IsField(posParent) )
      oCurs = m_pP2PmsgMgr -> P2Pos2Field ( posParent );
    //else if ( m_pP2PmsgMgr->IsNode(posParent) )
    //  oCurs = m_pP2PmsgMgr -> P2Pos2Node ( posParent );
    else { ASSERT(0); }
    for ( int i = 0; oCurs.Goto(i); i++ )
    {
      if ( IsItem(oCurs.r_Object().GetP2Pos()) )
        continue;                      // Already exists in tree
      if ( oCurs.IsItem() )
        InsertP2Pmsg ( hItemParent, oCurs.r_item() );
      //else if ( oCurs.IsNode() )
      //  InsertP2Pmsg ( hItemParent, oCurs.r_node() );
      else if ( oCurs.IsList() )
        ASSERT(0);
      else
        ASSERT(0);
    }
    // TODO: Added 23/09/2013 ^^^^

    //if ( pNMTreeView->itemOld.lParam )
   *pResult = TRUE;
    return FALSE;                      // Flag parent notification
}

//
//  NM_DBLCLK notification for m_wndP2PmsgTreeCtrl
//  NOTES: Some implementations expect to intercept this notification,
//         consequently the following settings facilitate this
//  
BOOL
CP2PmsgTreeCtrl::OnNMDblclk ( NMHDR *pNMHDR, LRESULT *pResult )
{
    // TODO: Add your control notification handler code here
    UNREFERENCED_PARAMETER(pNMHDR);
   *pResult = TRUE;
    return FALSE;
}


///////////////////////////////////////////////////////////////////////
//  P2PmsgTreeCtrl extensions

// 
//  Allocate a unique name
//
//  Parameters:  P3PmsgNode& oNode
//               Domain under which name is to be unique
//
//               LPCTSTR lpszItemName
//               Item name
//
//               UINT uiCopies
//               Maximum copies
//
//  Returns      CString
//               Allocated unique name
//               Format: lpszItemName(nnn)
//
/*CString
P2PmsgTreeCtrl_UniqueItemName ( P3PmsgNode& oNode
                              , LPCTSTR lpszItemName, UINT uiCopies )
{
    CString strUniqueItemName = lpszItemName;
    for ( unsigned int i = 0; i < uiCopies; i++ )
    {
      if ( !oNode.Exists(strUniqueItemName) )
        return strUniqueItemName;
      strUniqueItemName.Format(L"%s(%i)", lpszItemName, i );
    }
    strUniqueItemName.Empty();
    return strUniqueItemName;
}*/
CString
P2PmsgTreeCtrl_UniqueItemName ( P3PmsgItem& oItem
                              , LPCTSTR lpszItemName, UINT uiCopies )
{
    CString strUniqueItemName = lpszItemName;
    for ( unsigned int i = 0; i < uiCopies; i++ )
    {
      if ( !oItem.Exists(strUniqueItemName) )
        return strUniqueItemName;
      strUniqueItemName.Format(L"%s(%i)", lpszItemName, i );
    }
    strUniqueItemName.Empty();
    return strUniqueItemName;
}

//
//  Prints P2PmsgTreeCtrl item contents to console window
//
//  Parameters:  CP2PmsgTreeCtrl *pPPmsgTreeCtrl
//               Control from which contents to be displayed
//
//               FILE *fd
//               File descriptor
//
//               HTREEITEM hItem
//               Item contents to be displayed
void
CP2PmsgTreeCtrl_PrintItem ( CP2PmsgTreeCtrl *pP2PmsgTreeCtrl, FILE *fd, HTREEITEM hItem )
{
    CString strItemPath = pP2PmsgTreeCtrl -> GetItemPath ( hItem );
    P3Pmsg_fwprintf ( fd, L"\n------Item Path: %s ------\n", (LPCTSTR)strItemPath );
    P2Pos posItem = pP2PmsgTreeCtrl -> GetItemData ( hItem );
    if ( pP2PmsgTreeCtrl->GetP2PmsgMgr().IsField(posItem) )
      pP2PmsgTreeCtrl->GetP2PmsgMgr().P2Pos2Field(posItem).Print(fd,1);
    //else if ( wndP2PmsgTreeCtrl->GetP2PmsgMgr().IsNode(posItem) )
    //  wndP2PmsgTreeCtrl->GetP2PmsgMgr().P2Pos2Node(posItem).Print(fd,1);
    else ASSERT(0);
    pP2PmsgTreeCtrl -> GetItemPath ( hItem );
    P3Pmsg_fwprintf ( fd, L"--------------------------\n" );
}
void
CP2PmsgTreeCtrl_PrintItemAttr ( CP2PmsgTreeCtrl *pP2PmsgTreeCtrl, FILE *fd, HTREEITEM hItem )
{
    CString strItemPath = pP2PmsgTreeCtrl -> GetItemPath ( hItem );
    P3Pmsg_fwprintf ( fd, L"\n------Attr Path: %s@ ------\n", (LPCTSTR)strItemPath );
    P2Pos posItem = pP2PmsgTreeCtrl -> GetItemData ( hItem );
    if ( pP2PmsgTreeCtrl->GetP2PmsgMgr().IsField(posItem) )
      pP2PmsgTreeCtrl->GetP2PmsgMgr().P2Pos2Field(posItem).ATTR.Print(fd,1);
    //else if ( wndP2PmsgTreeCtrl->GetP2PmsgMgr().IsNode(posItem) )
    //  wndP2PmsgTreeCtrl->GetP2PmsgMgr().P2Pos2Node(posItem).ATTR.Print(fd,1);
    else ASSERT(0);
    pP2PmsgTreeCtrl -> GetItemPath ( hItem );
    P3Pmsg_fwprintf ( fd, L"--------------------------\n" );
}

//
//  Sorts items under folder according to passed paremeters
//
//  Parameters:  CP2PmsgTreeCtrl *wndP2PmsgTreeCtrl
//               Control for which contents to be sorted
//
//               HTREEITEM hItem
//               Item for which descendants are to be sorted
//
//               UINT uiParamsort
//               Sort control parameters
//
//               CList<CString>& oCListGroups
//               Sorting groups, sorted in order of groups
// 
//               UINT uiHintsmask
//               Sorting hints mask.  Without match assumes sorting
//               parameters have not changed.
//                 0... Nothing sorted
//                 PSort_Group_ALPHA... Sorts if defined
//                 PSort_Group_ACCESS.. Sorts if defined
//
//  Returns:     BOOL
//                 TRUE... Active sorting
//                 FALSE.. Nothing sorted
TargetCoreMFC_EXT BOOL
CP2PmsgTreeCtrl_Sort4Folder ( CP2PmsgTreeCtrl& wndP2PmsgTreeCtrl, HTREEITEM hItem
                            , UINT uiParamsort, CList<CString>& oCListGroups
                            , UINT uiHintsmask )
{
    // Locals
    // NOTES: Client handles exceptions
    if ( (uiParamsort&uiHintsmask) == 0 )
      return FALSE;                    // Nothing to sort
    BOOL bSwap = FALSE;
    P2PmsgMgr *pP2PmsgMgr = &wndP2PmsgTreeCtrl.GetP2PmsgMgr();
    if ( pP2PmsgMgr == nullptr )
      return FALSE;                    // Expected circumstances
    // Preamble
    if ( pP2PmsgMgr == nullptr )
      return FALSE;
    const P2Pos posItem = wndP2PmsgTreeCtrl.GetItemData ( hItem );
    if ( posItem == NULL )
      return FALSE;

    // Observe type
    // NOTES: Folders must be flagged as such
    if ( !pP2PmsgMgr->IsField(posItem) )
      return FALSE;
    P3PmsgItem oItemFolder = pP2PmsgMgr -> P2Pos2Object ( posItem );
    const BOOL bExpanded   = CTreeCtrl_IsExpanded ( wndP2PmsgTreeCtrl, hItem );
    CString  strItemPrev, strItemThis;

    // Sort non-folder items at bottom of list
    // NOTES: Ensure sequence is isolated on stack
    BOOL       bSkipGroup = FALSE;
    P2Pos      posTrigger = 0;
    P2Pos      posEoGroup = 0;
    POSITION posType = oCListGroups.GetHeadPosition();
    while ( posType )
    {
      CString strGroupTag = oCListGroups.GetNext(posType);
TOPF: if ( hItem )
      {
        bSkipGroup = posEoGroup ? TRUE : FALSE;
        posTrigger = posEoGroup;
        P3PmsgItem  oItemPrev, oItemThis;
        P3PmsgCurs  oCurs(oItemFolder);
        BOOL        bSync = TRUE;
        for ( int i = 0; oCurs.Goto(i); i++ )
        {
          // Skip until End-of-Group(last) is encountered
          if ( bSkipGroup ) {
            if ( posEoGroup == oCurs.r_Object().GetP2Pos() ) {
              bSkipGroup = FALSE;
              posTrigger = posEoGroup;
            }
            continue;
          }
          // Synchronise the next group
          // NOTES: If no descendant it MUST be empty
          if ( bSync ) {
            oItemPrev = oCurs.r_Object();
            bSync     = FALSE;
            continue;
          }
          oItemThis = oCurs.r_Object();
          // Firstly FOLDER and TYPE groups
          // NOTES: Grouping may NOT be enabled in which case we fall thru
          if ( (uiParamsort&(PSort_Group_TYPE|PSort_Group_FOLDERS)) != 0 )
          {
            if ( !oItemPrev.r_Attr().Exists(strGroupTag) &&
                  oItemThis.r_Attr().Exists(strGroupTag)    ) {
              P2PmsgMgr_Swap ( *pP2PmsgMgr, oItemPrev, oItemThis );
              bSwap = TRUE;
              goto TOPF;
            }
            if (  oItemPrev.r_Attr().Exists(strGroupTag) &&
                 !oItemThis.r_Attr().Exists(strGroupTag)    ) {
              posTrigger = oItemPrev.GetP2Pos();
              oItemPrev  = oItemThis.r_Object();
              continue;
            }
            if ( oItemThis.r_Attr().Exists(strGroupTag) ) {
              posTrigger = oItemThis.GetP2Pos();
            }
          }
          // Secondly Alphanumeric sort
          if ( (uiParamsort&PSort_ALPHA) == PSort_ALPHA )
          {
            strItemPrev = oItemPrev.r_name().c_name();
            strItemPrev.MakeUpper();
            strItemThis = oItemThis.r_name().c_name();
            strItemThis.MakeUpper();
            if ( strItemPrev.CompareNoCase(strItemThis) > 0 ) {
              P2PmsgMgr_Swap ( *pP2PmsgMgr, oItemPrev, oItemThis );
              bSwap = TRUE;
              goto TOPF;
            }
          }
          // Thirdly access sort
          if ( (uiParamsort&PSort_ACCESS) == PSort_ACCESS )
          {
            const time_t tsItemPrev = P3Pmsg_GetTStamp(oItemPrev);
            const time_t tsItemThis = P3Pmsg_GetTStamp(oItemThis);
            if ( tsItemPrev > tsItemThis ) {
              P2PmsgMgr_Swap ( *pP2PmsgMgr, oItemPrev, oItemThis );
              bSwap = TRUE;
              goto TOPF;
            }
          }
          oItemPrev = oItemThis.r_Object();
        }
      }
      posEoGroup = posTrigger;
    }

    // Refresh contents
    if ( bSwap ) {
      CTreeCtrl_DeleteAllChildren ( &wndP2PmsgTreeCtrl, hItem );
      if ( bExpanded )
        wndP2PmsgTreeCtrl.Expand ( hItem, TVE_EXPAND );
    }
    // Tidy up, and
    return bSwap;
}
