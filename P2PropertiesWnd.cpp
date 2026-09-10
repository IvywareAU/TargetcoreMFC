// Copyright © 2014, 2019, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  P2PropertiesWnd implementations.
//  NOTES: Integrates the CMFCPropertyGridCtrl and P2PmsgMgr objects
//
#include "stdafx.h"

#include "CWndApp_Ext.h"
#include "P2PropertiesWnd.h"
#include "P2Pwin32.h"
#include "P2Pmsg_Ext.h"
#include "CMFCPropertyGridCtrl_Ext.h"
#include "MFCPropertyGridPenProperty.h"
#include "MFCPropertyGridPencrxProperty.h"
#include "MFCPropertyGridBrushProperty.h"
#include "MFCPropertyGridBnumProperty.h"
#include "MFCPropertyGridEnumProperty.h"
#include "MFCPropertyGridFontcrxProperty.h"
#include "MFCPropertyGridURLProperty.h"
#include "MFCPropertyGridCSVProperty.h"
#include "Resource.h"                  // DLL Resource
#include "CFont_Ext.h"
#include "CPen_Ext.h"
//#include "winHTTP_Ext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////
//  Toolbar properties
void
CP2PropertiesToolBar::OnUpdateCmdUI ( CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
    UNREFERENCED_PARAMETER(pTarget);
    MANAGE_RESOURCE_STATE;
    CFrameWnd *pTargetRedirect = (CFrameWnd *)GetOwner();
    CMFCToolBar::OnUpdateCmdUI( pTargetRedirect, bDisableIfNoHndler);
}
static CString s_cMfcToolbarTipText;
BOOL
CP2PropertiesToolBar::OnNeedTipText(UINT id, NMHDR* pNMH, LRESULT *pResult )
{
    CMFCToolBar::OnNeedTipText ( id, pNMH, pResult );
	/*if (m_pToolTip->GetSafeHwnd() == NULL || pNMH->hwndFrom != m_pToolTip->GetSafeHwnd())
	{
		return FALSE;
	}

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
	ti.cbSize = sizeof(AFX_OLDTOOLINFO);
	INT_PTR nHit = (INT_PTR) OnToolHitTest(point, &ti);

	if (nHit < 0 || ti.lpszText == NULL || ti.lpszText == LPSTR_TEXTCALLBACK)
	{
		return FALSE;
	}

	LPNMTTDISPINFO pTTDispInfo = (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	s_cMfcToolbarTipText = ti.lpszText;
	free(ti.lpszText);

	pTTDispInfo->lpszText = const_cast<LPTSTR>((LPCTSTR) s_cMfcToolbarTipText);

	m_pToolTip->SetFont(&(GetGlobalData()->fontRegular), FALSE);*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//  Constructors and destructor

CP2PropertiesWnd *g_pP2PropertiesWnd = 0;
CP2PropertiesWnd::CP2PropertiesWnd ( LPCTSTR lpszIdentificationTag )
{
    MANAGE_RESOURCE_STATE;
    m_strIdentificationTag = lpszIdentificationTag;
    m_pItemProps           = nullptr;
    m_pCWndNotify          = 0;
    m_nCWndNotifyWM        = 0;
    m_nNotifyP2PsysID      = 0;
    m_nComboHeight         = 0;
    m_hP2PmsgHeap          = 0;
    m_nGridSinkID          = 0;
    m_nP2PmsgSinkID        = 0;
}

CP2PropertiesWnd::~CP2PropertiesWnd()
{
    if ( g_pP2PropertiesWnd == this )
      g_pP2PropertiesWnd = 0;
    if ( m_nP2PmsgSinkID )
      P2PmsgSinkClose ( m_nP2PmsgSinkID );
}

//  Global exposure
CP2PropertiesWnd*
CP2PropertiesWnd::GetDefaultWnd ( )
{
    return g_pP2PropertiesWnd;
}

void
CP2PropertiesWnd::EmptyDefaultWnd ( )
{
    CP2PropertiesWnd *pCWndDefault = GetDefaultWnd ( );
    if ( pCWndDefault )
    {
      CoCreateGuid ( &pCWndDefault->m_oGUID );   // Effectively clears GUID
      pCWndDefault -> DetachPropList ( pCWndDefault->m_pCWndNotify );
      pCWndDefault -> ClearPropList ( );
      pCWndDefault -> Invalidate ( );
      pCWndDefault -> m_wndPropList.Invalidate();
      pCWndDefault -> m_wndObjectCombo.ResetContent ( );
    }
}

void
CP2PropertiesWnd::DetachDefaultWnd ( CWnd *pCWndNotify )
{
    CP2PropertiesWnd *pCWndDefault = GetDefaultWnd ( );
    if ( pCWndDefault )
      pCWndDefault -> DetachPropList ( pCWndNotify );
}

/////////////////////////////////////////////////
// Toolbar and associated mapping operations
// NOTES: Toolbar is specialised and managed through the derived class.  However,
//        only the base class is exposed throughout the code base.  These
//        methods are intended to be specialised in derived class
void
CP2PropertiesWnd::ClearToolbarMaps ( )
{
}
BOOL
CP2PropertiesWnd::SetToolbarURLmap ( LPCTSTR lpszMapTag, LPCTSTR lpszMapURL )
{
    UNREFERENCED_PARAMETER(lpszMapTag);
    UNREFERENCED_PARAMETER(lpszMapURL);
    ASSERT(0);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//  Operations

//
//  Initialise property list
//  NOTES: Mandatory for configuration settings
//
void
CP2PropertiesWnd::InitPropList()
{
    // Initialisations
  	SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    // Debug only for cross checking etc
#ifdef _DEBUG
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance(Debug Only)"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t) false, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, L"Arial");

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), NULL, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(L"Default", ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, L"", _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(L"Folder", _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
#endif
}

void
CP2PropertiesWnd::CreatePropList ( GUID oGUID, CWnd *pCWndNotify, UINT nCWndNotifyWM )
{
    // Initialise
    ASSERT(0);
    m_wndPropList.RemoveAll ( );
    m_wndPropList.Invalidate( );
    m_oItems.r_Desc().Truncate();
//InitPropList();
    m_oGUID           = oGUID;
    m_pCWndNotify     = pCWndNotify;
    m_nCWndNotifyWM   = nCWndNotifyWM;
    m_nNotifyP2PsysID = 0;
    m_hP2PmsgHeap     = 0;
    if ( m_pItemProps != nullptr )
      delete m_pItemProps;
    m_pItemProps = nullptr;
    if ( m_nP2PmsgSinkID )
      P2PmsgSinkCancel ( m_nP2PmsgSinkID, nullptr );
}
void
CP2PropertiesWnd::CreatePropList ( P2PeerTarget *pTargetNotify, GUID oGUID, P2PsysID nNotifyP2PsysID )
{
ASSERT(pTargetNotify->IsValidObject());
    // Initialise
    m_wndPropList.RemoveAll ( );
    m_wndPropList.Invalidate( );
    m_oItems.r_Desc().Truncate();
//InitPropList();
    m_pTargetNotify   = pTargetNotify;
    m_oGUID           = oGUID;
    m_pCWndNotify     = 0;
    m_nCWndNotifyWM   = 0;
    m_nNotifyP2PsysID = nNotifyP2PsysID;
    m_hP2PmsgHeap     = 0;
    if ( m_pItemProps != nullptr )
      delete m_pItemProps;
    m_pItemProps = nullptr;

    // Notifications sink
    if ( m_nP2PmsgSinkID == 0 )
      m_nP2PmsgSinkID = P2PmsgSinkCreate ( L"P2PropertiesWnd" );
    if ( m_nP2PmsgSinkID <= 0 )
      ThrowP2Pevent ( );               // Throws internal event
    P2PmsgSinkCancel   ( m_nP2PmsgSinkID, nullptr, nNotifyP2PsysID );
    P2PmsgSinkRegister ( m_nP2PmsgSinkID, pTargetNotify, nNotifyP2PsysID );

    // Combo list identification
    m_wndObjectCombo.ResetContent ( );
}

//
//  Sets combo list tag
//
//  Parameters:  LPCTSTR lpszComboTag
//               Combo list tag for properties selection
//
void
CP2PropertiesWnd::SetComboTag ( LPCTSTR lpszComboTag )
{
    const int ndx = m_wndObjectCombo.AddString( lpszComboTag );
    ASSERT(ndx>=0);
    m_wndObjectCombo.SetCurSel ( ndx );
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddGroup ( CMFCPropertyGridProperty *pGroup )
{
    m_wndPropList.AddProperty ( pGroup );
    return pGroup;
}
//CMFCPropertyGridProperty*
//CP2PropertiesWnd::AddGroup ( CMFCPropertyGridProperty *pGroupParent
//                           , LPCSTR lpszGroupname, LPCTSTR lpszDescription )
//{
//    CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty ( lpszGroupname );
//                              pGroup -> SetDescription ( lpszDescription ); 
//    if ( pGroupParent )
//	    pGroupParent -> AddSubItem ( pGroup );
//    else
//	    m_wndPropList.AddProperty ( pGroup );
//    return pGroup;
//}
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddGroup_HEADING ( CMFCPropertyGridProperty *pGroupParent
                                   , LPCTSTR lpszHeading, LPCTSTR lpszDescription )
{
    CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty ( lpszHeading );
    pGroup -> SetDescription ( lpszDescription );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGroup );
    else
	    m_wndPropList.AddProperty ( pGroup );
    return pGroup;
}
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddGroup ( CMFCPropertyGridProperty *pGroupParent, LPCTSTR lpszValue
                           , LPCTSTR lpszPropname, LPCTSTR lpszDescription )
{
   _variant_t var = lpszValue;
    CMFCPropertyGridProperty *pGroup
      = new CMFCPropertyGridProperty ( lpszPropname, var, lpszDescription );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGroup );
    else
	    m_wndPropList.AddProperty ( pGroup );
    pGroup -> Enable(FALSE);
    m_wndPropList.ExpandAll();
    return pGroup;
}
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                          , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                          , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
   _variant_t var = P2PmsgData_var ( oItem.r_data() );
    CMFCPropertyGridProperty *pGridProp
      = new CMFCPropertyGridProperty ( lpszHeading, var, lpszDescription );
    pGridProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGridProp );
    else
	    m_wndPropList.AddProperty ( pGridProp );
    return pGridProp;
}
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_COLOR ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                                , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                                , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    COLORREF oCOLORREF = oItem.r_data().c_uint();
    CMFCPropertyGridColorProperty *pColorProp
      = new CMFCPropertyGridColorProperty ( lpszHeading, oCOLORREF, NULL, lpszDescription );
    pColorProp -> EnableOtherButton ( L"Other..." );
    pColorProp -> EnableAutomaticButton ( L"Default", ::GetSysColor(COLOR_3DFACE));
    pColorProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pColorProp );
    else
	    m_wndPropList.AddProperty ( pColorProp );
    return pColorProp;
}
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_FONT ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                               , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                               , P3PmsgAttr *pAttr )
{
    ASSERT(0); //TODO:LJM no longer utilised
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    LOGFONT oLOGFONT;
    if ( oItem.r_data().c_size() >= sizeof(oLOGFONT) )
      memcpy ( &oLOGFONT, oItem.c_vBlob(), sizeof(oLOGFONT) );
    else
      StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGFONT, L"Arial" );
	    CMFCPropertyGridFontProperty *pFontProp
        = new CMFCPropertyGridFontProperty( lpszHeading, oLOGFONT, CF_EFFECTS | CF_SCREENFONTS, lpszDescription );
      pFontProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pFontProp );
    else
	    m_wndPropList.AddProperty ( pFontProp );
    return pFontProp;
}
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_FONTcrx ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                                  , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                                  , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    LOGFONTcrx oLOGFONTcrx;
//int nSize1=oItem.r_data().c_size();
//int nSize2=sizeof(oLOGFONTcrx);
//int nSize3=sizeof(LOGFONT);
    if ( oItem.r_data().c_size() >= sizeof(oLOGFONTcrx) )
      memcpy ( &oLOGFONTcrx, oItem.c_vBlob(), sizeof(oLOGFONTcrx) );
    else
      StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGFONTcrx, L"Arial" );
	  CMFCPropertyGridFontcrxProperty *pFontProp
      = new CMFCPropertyGridFontcrxProperty( lpszHeading, oLOGFONTcrx, CF_EFFECTS | CF_SCREENFONTS, lpszDescription, 0, oLOGFONTcrx.oCOLORREF );
    pFontProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pFontProp );
    else
	    m_wndPropList.AddProperty ( pFontProp );
    return pFontProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_PEN ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                              , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                              , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    LOGPEN oLOGPEN;
    if ( oItem.r_data().c_size() >= sizeof(oLOGPEN) )
      memcpy ( &oLOGPEN, oItem.c_vBlob(), sizeof(oLOGPEN) );
    //else
    //  StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGPEN, L"Arial" );
    //_variant_t var = (int)0;
	  CMFCPropertyGridPenProperty *pPenProp
        = new CMFCPropertyGridPenProperty( lpszPropname, oLOGPEN, NULL, lpszDescription );
    pPenProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pPenProp );
    else
	    m_wndPropList.AddProperty ( pPenProp );
    return pPenProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_PENcrx ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                                 , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                                 , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    LOGPENcrx oLOGPENcrx;
    if ( oItem.r_data().c_size() >= sizeof(oLOGPENcrx) )
      memcpy ( &oLOGPENcrx, oItem.c_vBlob(), sizeof(oLOGPENcrx) );
    //else
    //  StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGPEN, L"Arial" );
    //_variant_t var = (int)0;
	  CMFCPropertyGridPencrxProperty *pPenProp
        = new CMFCPropertyGridPencrxProperty( lpszPropname, oLOGPENcrx, NULL, lpszDescription );
    pPenProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pPenProp );
    else
	    m_wndPropList.AddProperty ( pPenProp );
    return pPenProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_BRUSHcrx ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                                   , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                                   , P3PmsgAttr *pAttr )
{
    // Preprocess passed oBrushcrx
    // NOTES: Transform LOGBRUSHcrx from 32 to 64 bit
    if ( oField.r_data().c_size() <= sizeof(LOGBRUSH32crx) )
    {
       LOGBRUSH32crx *pLOGBRUSH32crx = (LOGBRUSH32crx*)oField.r_data().c_vBlob();
       LOGBRUSHcrx    oLOGBRUSHcrx;
       oLOGBRUSHcrx.lbColor = pLOGBRUSH32crx->lbColor;
       oLOGBRUSHcrx.lbHatch = pLOGBRUSH32crx->lbHatch;
       oLOGBRUSHcrx.lbStyle = pLOGBRUSH32crx->lbStyle;

       oLOGBRUSHcrx.uAlpha  = pLOGBRUSH32crx->uAlpha;
       oLOGBRUSHcrx.uEoD    = pLOGBRUSH32crx->uEoD;
       oLOGBRUSHcrx.uSpare1 = pLOGBRUSH32crx->uSpare1;
       oField.r_data().c_memcpy(&oLOGBRUSHcrx,sizeof(oLOGBRUSHcrx));
       ASSERT(oField.r_data().VerifyContainment());
    }

    // Proceed as per normal
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    LOGBRUSHcrx oLOGBRUSHcrx;
    if ( oItem.r_data().c_size() >= sizeof(oLOGBRUSHcrx) )
      memcpy ( &oLOGBRUSHcrx, oItem.c_vBlob(), sizeof(oLOGBRUSHcrx) );
    //else
    //  StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGPEN, L"Arial" );
    //_variant_t var = (int)0;
	  CMFCPropertyGridBrushProperty *pBrushProp
        = new CMFCPropertyGridBrushProperty( lpszPropname, oLOGBRUSHcrx, NULL, lpszDescription );
    pBrushProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pBrushProp );
    else
	    m_wndPropList.AddProperty ( pBrushProp );
    return pBrushProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_BNUM ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                               , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                               , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    P2Pint oEnum = oItem.r_data().c_uint();
	  CMFCPropertyGridBnumProperty *pBnumProp
        = new CMFCPropertyGridBnumProperty ( lpszPropname, (int)0, NULL, lpszDescription );
    pBnumProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pBnumProp );
    else
	    m_wndPropList.AddProperty ( pBnumProp );
//oField.Print(stdout,1);
//oItem.Print(stdout,1);
    return pBnumProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_ENUM ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                               , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                               , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    P2Pint oEnum = oItem.r_data().c_uint();
	  CMFCPropertyGridEnumProperty *pEnumProp
        = new CMFCPropertyGridEnumProperty ( lpszPropname, (int)oEnum, NULL, lpszDescription );
    pEnumProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pEnumProp );
    else
	    m_wndPropList.AddProperty ( pEnumProp );
    // Tidy up, and
    return pEnumProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_URL ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                              , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                              , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    CString strURL = oField.c_wstr();
    //LOGPEN oLOGPEN;
    //if ( oItem.r_data().c_size() >= sizeof(oLOGPEN) )
    //  memcpy ( &oLOGPEN, oItem.c_vBlob(), sizeof(oLOGPEN) );
    //else
    //  StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGPEN, L"Arial" );
    //_variant_t var = (int)0;
	  CMFCPropertyGridURLProperty *pURLProp
        = new CMFCPropertyGridURLProperty( lpszPropname, strURL, NULL, lpszDescription );
    pURLProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pURLProp );
    else
	    m_wndPropList.AddProperty ( pURLProp );
    return pURLProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_CSV ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                              , LPCTSTR lpszPropname, LPCTSTR lpszDescription
                              , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    CString strCSV = oField.c_wstr();
    //LOGPEN oLOGPEN;
    //if ( oItem.r_data().c_size() >= sizeof(oLOGPEN) )
    //  memcpy ( &oLOGPEN, oItem.c_vBlob(), sizeof(oLOGPEN) );
    //else
    //  StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGPEN, L"Arial" );
    //_variant_t var = (int)0;
	  CMFCPropertyGridCSVProperty *pCSVProp
        = new CMFCPropertyGridCSVProperty( lpszPropname, strCSV, NULL, lpszDescription );
    pCSVProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pCSVProp );
    else
	    m_wndPropList.AddProperty ( pCSVProp );
    return pCSVProp;
}

CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_OleDateTime ( CMFCPropertyGridProperty *pGroupParent, P3PmsgItem& oField
                               , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                               , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oField);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oField.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
    COleDateTime odtItem = oField.c_double();
   _variant_t var = (LPCTSTR)odtItem.Format(L"%d %m %Y");
    CMFCPropertyGridProperty *pGridProp
      = new CMFCPropertyGridProperty ( lpszHeading, var, lpszDescription );
    pGridProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGridProp );
    else
	    m_wndPropList.AddProperty ( pGridProp );
    return pGridProp;
}

//
//  Adds spin control to list
//  NOTES: WARNING issue, AFX_WM_PROPERTY_CHANGED message is received for each
//         spin click.  Causes data change message which eventually causes 
//         control to be redrawn which in turn clobbers the drawn control state
//       : Essentually a bad feed back loop.
CMFCPropertyGridProperty*
CP2PropertiesWnd::AddItem_SPIN ( CMFCPropertyGridProperty *pGroupParent
                               , P3PmsgItem& oValue, UINT uiMin, UINT uiMax
                               , LPCTSTR lpszHeading, LPCTSTR lpszDescription
                               , P3PmsgAttr *pAttr )
{
    P3PmsgItem oItem = m_oItems.r_Desc().PushBack(oValue);
    if ( pAttr )
      oItem.r_Attr() += *pAttr;
    if ( m_nNotifyP2PsysID == 0 )
      oItem = oItem.r_Object();
    ASSERT(m_hP2PmsgHeap==oItem.r_Object().m_hVBList||m_hP2PmsgHeap==0);
    if ( m_hP2PmsgHeap == 0 )
      m_hP2PmsgHeap = oItem.r_Object().m_hVBList;
   _variant_t var = P2PmsgData_var ( oValue.r_data() );
    CMFCPropertyGridProperty *pGridProp
      = new CMFCPropertyGridProperty( lpszHeading, (long)var, lpszDescription );
    pGridProp -> EnableSpinControl (TRUE, uiMin, uiMax );
    pGridProp -> SetData ( oItem.GetP2Pos() );
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGridProp );
    else
	    m_wndPropList.AddProperty ( pGridProp );
    return pGridProp;
}

void
CP2PropertiesWnd::AttachPropList ( P3PmsgItem *pItemProps, CWnd *pCWndNotify, UINT nCWndNotifyWM  )
{
    // Initialise
//DiagnosticA(__FUNCTION__" P3PmsgItem");
    CoCreateGuid ( &m_oGUID );         // Effectively clears GUID
    m_wndPropList.RemoveAll ( );
    m_pCWndNotify   = pCWndNotify;
    m_nCWndNotifyWM = nCWndNotifyWM;
    m_hP2PmsgHeap = pItemProps->r_Object().m_hVBList;
    if ( m_pItemProps != nullptr )
      delete m_pItemProps;
    m_pItemProps  = new P3PmsgItem ( pItemProps->r_Object() );
    m_hP2PmsgHeap = pItemProps->r_Object().m_hVBList;

    // Recursive delegation
    if ( m_pItemProps )
    {
      InitPropList ( );
      CMFCPropertyGridCtrl_Init ( &m_wndPropList, m_pItemProps );
    }
}

void
CP2PropertiesWnd::DetachPropList ( CWnd *pCWndNotify )
{
    if ( pCWndNotify == m_pCWndNotify )
    {
      CoCreateGuid ( &m_oGUID );       // Effectively clears GUID
      m_pCWndNotify   = nullptr;
      m_nCWndNotifyWM = 0;
      delete m_pItemProps;
      m_pItemProps    = 0;
      m_hP2PmsgHeap   = 0;
      m_wndPropList.RemoveAll ( );
      m_wndPropList.Invalidate();
      m_oItems.r_Desc().Truncate ( );
    }
}


//void
//CP2PropertiesWnd::DetachPropList ( P3PmsgNode *pNodeProps )
//{
//    m_pCWndNotify   = 0;
//    m_nCWndNotifyWM = 0;
//    delete m_pItemProps;
//           m_pItemProps = nullptr;
//    m_hP2PmsgHeap   = 0;
//    m_wndPropList.RemoveAll ( );
//    CoCreateGuid ( &m_oGUID );         // Effectively clears GUID
//}

void CP2PropertiesWnd::InitPropList ( P3PmsgItem *pItem, CMFCPropertyGridProperty *pGroupParent )
{
    InitPropList();
ASSERT(0);
    // Parent group
    CString strGroupname = pItem -> c_name();
    if ( !pItem->r_data().IsNull() )
    {
      strGroupname += L" - ";
      strGroupname += pItem->r_data().ToString();
    }
    if ( strGroupname.IsEmpty() )
      strGroupname = L"Hierarchy Heading";
	CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty ( strGroupname );
	CMFCPropertyGridProperty *pProp  = 0;

    // Explore and populate with passed P2Pmsg object attributes
    P3PmsgCurs oCursAttr ( pItem->r_Attr() );
    for ( int i = 0; oCursAttr.Goto(i); i++ )
    {
      //if ( oCursAttr.IsNode() )
      //{
      //  InitPropList ( &oCursAttr.r_node(), pGroup );
      //}
      if ( oCursAttr.IsList() )
        ASSERT(0);
      else if ( oCursAttr.IsItem() )
      {
        if ( oCursAttr.r_item().GetAccess(AttrField_HIDDEN) )
          continue;
        LPCTNAM lpszPropname = oCursAttr.r_item().c_name();
        LPCTSTR lpszDesc     = 0;
        if ( oCursAttr.r_item().r_Attr().Exists(L"Dsc") )
          lpszDesc = oCursAttr.r_item().r_Attr().SelectItem(L"Dsc").c_wstr();
       _variant_t var = P2PmsgData_var ( oCursAttr.r_item().r_data() );
        pProp = new CMFCPropertyGridProperty ( lpszPropname, var, lpszDesc );
        pProp  -> SetData ( oCursAttr.r_item().GetP2Pos() );
        pGroup -> AddSubItem ( pProp );
      }
      else
        ASSERT(0);
    }

    // Tidy up, and
    //if ( pGroupParent )
	//  pGroupParent -> AddSubItem ( pGroup );
    //else
	//  m_wndPropList.AddProperty ( pGroup );

    // Explore and populate with passed P2Pmsg object contents
    P3PmsgCurs oCursNode ( *pItem );
    for ( int i = 0; oCursNode.Goto(i); i++ )
    {
      /*if ( oCursNode.IsNode() )
      {
        InitPropList ( &oCursNode.r_node(), pGroup );
      }
      else*/ if ( oCursNode.IsList() )
        ASSERT(0);
      else if ( oCursNode.IsItem() )
      {
        LPCTSTR lpszPropname = oCursNode.r_item().c_name();
        if ( oCursNode.r_item().r_Attr().Exists(L"#Lab") )
          lpszPropname = oCursNode.r_item().r_Attr().SelectItem(L"#Lab").c_wstr();
        LPCTSTR lpszDesc     = 0;
        if ( oCursNode.r_item().r_Attr().Exists(L"#Dsc") )
          lpszDesc = oCursNode.r_item().r_Attr().SelectItem(L"#Dsc").c_wstr();
       _variant_t var = P2PmsgData_var ( oCursNode.r_item().r_data() );
        pProp = new CMFCPropertyGridProperty ( lpszPropname, var, lpszDesc );
        pProp  -> SetData ( oCursNode.r_item().GetP2Pos() );
        pGroup -> AddSubItem ( pProp );
      }
      else
        ASSERT(0);
    }

    // Tidy up, and
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGroup );
    else
	    m_wndPropList.AddProperty ( pGroup );

}

void
CP2PropertiesWnd::ClearPropList ( )
{
    // Initialise
    m_wndPropList.RemoveAll ( );
    m_wndPropList.Invalidate( );
    m_oItems.r_Desc().Truncate();
    // Button clear
    //m_strWikiURL.Empty();
    //m_strUTubeURL1.Empty();
    //m_strGenericURL2.Empty();
    //m_strHomePageURL.Empty();
    //m_strUserURL1.Empty();
    //m_strUserURL2.Empty();
}

void
CP2PropertiesWnd::AdjustLayout()
{
    if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
    {
      return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight+10, rectClient.Width(), cyTlb+1, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb+12, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb-2), SWP_NOACTIVATE | SWP_NOZORDER);
}


//CMap_CString&
//CP2PropertiesWnd::GetListP2PmsgFilters ( )
//{
//    return m_oCListP2PmsgFilters;
//}

//bool
//CP2PropertiesWnd::HasFilteredItems ( P3PmsgObject& oObject )
//{
//    if ( m_oCListP2PmsgFilters.IsEmpty() )
//      return true;
//    P3PmsgAttr oAttr;
//    if ( oObject.IsNode() )
//      oAttr = P3PmsgNode(oObject).r_Attr().r_Object();
//    else
//      return false;
//
//    // Apply filters
//    POSITION pos = m_oCListP2PmsgFilters.GetHeadPosition();
//    while ( pos )
//    {
//      if ( oAttr.Exists(m_oCListP2PmsgFilters.GetNext(pos)) )
//        return true;
//    }
//    return false;
//}
//bool
//CP2PropertiesWnd::IsFilteredItem ( P3PmsgObject& oObject )
//{
//    if ( m_oCListP2PmsgFilters.IsEmpty() )
//      return true;
//    P3PmsgAttr oAttr;
//    if ( oObject.IsNode() )
//      oAttr = P3PmsgNode(oObject).r_Attr().r_Object();
//    else if ( oObject.IsField() )
//      oAttr = P3PmsgField(oObject).r_Attr().r_Object();
//    else
//      return false;
//
//    // Apply filters
//    POSITION pos = m_oCListP2PmsgFilters.GetHeadPosition();
//    while ( pos )
//    {
//      if ( oAttr.Exists(m_oCListP2PmsgFilters.GetNext(pos)) )
//        return true;
//    }
//    return false;
//}

///////////////////////////////////////////////////////////////////////////////
//  MFC Message handlers
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

BEGIN_MESSAGE_MAP(CP2PropertiesWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
    ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    //ON_COMMAND(ID_PROPERTIES1, OnProperties1)
    //ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
    //ON_COMMAND(ID_PROPERTIES2, OnProperties2)
    //ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
    //ON_COMMAND(ID_PROPERTIES_WIKI, OnBtnWikiURL)
    //ON_UPDATE_COMMAND_UI(ID_PROPERTIES_WIKI, OnUpdateBtnWikiURL)
    //ON_COMMAND(ID_PROPERTIES_UTUBE1, OnBtnUTubeURL1)
    //ON_UPDATE_COMMAND_UI(ID_PROPERTIES_UTUBE1, OnUpdateBtnUTubeURL1)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

int
CP2PropertiesWnd::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
    // Compliments wizard
    MANAGE_RESOURCE_STATE;
    if ( CDockablePane::OnCreate(lpCreateStruct) == -1 )
		  return -1;

    // Identification tag
    ASSERT ( g_pP2PropertiesWnd==0 || !m_strIdentificationTag.IsEmpty() );
    if ( m_strIdentificationTag.IsEmpty() )
      g_pP2PropertiesWnd = this;
    CRect rectDummy;
    rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_P2PROPERTIES_WND);
	//m_wndToolBar.LoadToolBar(IDR_P2PROPERTIES_WND, 0, 0, TRUE /* Is locked */);
	//m_wndToolBar.CleanUpLockedImages();
	////m_wndToolBar.LoadBitmap(/*theApp.*/m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_P2PROPERTIES_WND, 0, 0, TRUE /* Locked */);
	//m_wndToolBar.LoadBitmap(/*theApp.*/m_bHiColorIcons ? IDR_P2PROPERTIES_WND : IDR_P2PROPERTIES_WND, 0, 0, TRUE /* Locked */);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//m_wndToolBar.SetOwner(this);
  //ASSERT(m_wndToolBar.GetShowTooltips());
  //m_wndToolBar.SetShowTooltips(TRUE); //TODO:Added by LJM

	//// All commands will be routed via this control , not via the parent frame:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//AdjustLayout();
    // Set number of GridControl description rows (at base)
    // NOTES: Normally the default of 3 is exceeded
    m_wndPropList.SetDescriptionRows ( 8 );
	  return 0;
}

void
CP2PropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	  AdjustLayout();
}

LRESULT
CP2PropertiesWnd::OnPropertyChanged ( WPARAM wparam, LPARAM lparam )
{
    wparam;
    // Isolate responsible grid and extract new value
    // NOTES: PropertyGrid contains a user defined data field that maps to
    //        the P3PmsgObject holding the original data
    CMFCPropertyGridProperty *pGrid  = (CMFCPropertyGridProperty *)lparam;
    const P2Pos               nP2Pos = pGrid -> GetData ( );
    if ( nP2Pos == 0 )
      return FALSE;
    P3PmsgItem   oItem;
    P3PmsgObject oObject;
    oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
    ASSERT(oObject.IsField());
    oItem = oObject;

    if ( dynamic_cast<CMFCPropertyGridFontcrxProperty*>(pGrid) && lparam==0)
    {
      CMFCPropertyGridFontcrxProperty *pGridFontcrx = dynamic_cast<CMFCPropertyGridFontcrxProperty*>(pGrid);
      //P3PmsgObject   oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      //oItem = oObject;
      oItem.c_memcpy((const void*)pGridFontcrx->GetLogFontcrx(),sizeof(LOGFONTcrx));
      //P2P_LOGFONTcrx oLOGFONTcrx;
      //oLOGFONTcrx = oItem.r_Object();
      //oLOGFONTcrx->oCOLORREF = pGridFontcrx->GetColor();
//ASSERT(AfxCheckMemory());
//if(m_pItemProps)
//m_pItemProps->AssertValid();
    }
    else if ( dynamic_cast<CMFCPropertyGridFontcrxProperty*>(pGrid) )
    {
      CMFCPropertyGridFontcrxProperty *pGridFontcrx = dynamic_cast<CMFCPropertyGridFontcrxProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGFONT;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField.c_memcpy((const void*)pGridFontcrx->GetLogFontcrx(),sizeof(LOGFONTcrx));
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      //else if ( oObject.IsNode() )
      //{
      //  P3PmsgNode oNode ( oObject );
      //  oNode.c_memcpy((const void *)pGridFont->GetLogFont(),sizeof(LOGFONT));
      //}
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridFontProperty*>(pGrid) )
    {
      CMFCPropertyGridFontProperty *pGridFont = dynamic_cast<CMFCPropertyGridFontProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGFONT;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField.c_memcpy((const void*)pGridFont->GetLogFont(),sizeof(LOGFONT));
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      //else if ( oObject.IsNode() )
      //{
      //  P3PmsgNode oNode ( oObject );
      //  oNode.c_memcpy((const void *)pGridFont->GetLogFont(),sizeof(LOGFONT));
      //}
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridColorProperty*>(pGrid) )
    {
      CMFCPropertyGridColorProperty *pGridColor = dynamic_cast<CMFCPropertyGridColorProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        oItem = oObject;
        P3PmsgField oField ( oObject );
        oField.c_uint(pGridColor->GetColor());
      }
      //else if ( oObject.IsNode() )
      //{
      //  P3PmsgNode oNode ( oObject );
      //  oNode.c_uint(pGridColor->GetColor());
      //}
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridPencrxProperty*>(pGrid) )
    {
      CMFCPropertyGridPencrxProperty *pGridPen = dynamic_cast<CMFCPropertyGridPencrxProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGPEN;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField.c_memcpy((const void*)pGridPen->GetLogPen(),sizeof(LOGPENcrx));
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      //else if ( oObject.IsNode() )
      //{
      //  P3PmsgNode oNode ( oObject );
      //  oNode.c_memcpy((const void *)pGridPen->GetLogPen(),sizeof(LOGFONT));
      //}
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridPenProperty*>(pGrid) )
    {
      CMFCPropertyGridPenProperty *pGridPen = dynamic_cast<CMFCPropertyGridPenProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGPEN;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField.c_memcpy((const void*)pGridPen->GetLogPen(),sizeof(LOGPEN));
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      //else if ( oObject.IsNode() )
      //{
      //  P3PmsgNode oNode ( oObject );
      // oNode.c_memcpy((const void *)pGridPen->GetLogPen(),sizeof(LOGFONT));
      //}
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridBrushProperty*>(pGrid) )
    {
      CMFCPropertyGridBrushProperty *pGridBrush = dynamic_cast<CMFCPropertyGridBrushProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGPEN;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField.c_memcpy((const void*)pGridBrush->GetLogBrush(),sizeof(LOGBRUSHcrx));
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      //else if ( oObject.IsNode() )
      //{
      //  ASSERT(0);
      //  P3PmsgNode oNode ( oObject );
      //  oNode.c_memcpy((const void *)pGridBrush->GetLogBrush(),sizeof(LOGBRUSHcrx));
      //}
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridBnumProperty*>(pGrid) )
    {
      CMFCPropertyGridBnumProperty *pGridEnum = dynamic_cast<CMFCPropertyGridBnumProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        oItem = oObject;
        oItem.c_uint(pGridEnum -> GetBMask ());
      }
      else
        ASSERT(0);
      //_variant_t     var = pGridEnum -> GetBMask ( );
      //if ( P3PmsgData_var(m_hP2PmsgHeap,nP2Pos,var) == 0 )
      //  return FALSE;
      //CString       strOption = var;
      //DWORD         dwBMask = pGridEnum -> GetBMask ();
      //P3PmsgObject  oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      //P2PDWORD  oBMask = oObject;
      //oBMask = pGridEnum -> GetBMask ();

      //TODO:LJM testing vvvvvvvvvvvvvvvvvvvvv
//m_oNodeItems.Print(stdout,0);
      //oItem = oObject;
//oItem.Print(stdout,0);
      //TODO:LJM testing ^^^^^^^^^^^^^^^^^^^^^
    }
    else if ( dynamic_cast<CMFCPropertyGridEnumProperty*>(pGrid) )
    {
      CMFCPropertyGridEnumProperty *pGridEnum = dynamic_cast<CMFCPropertyGridEnumProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        oItem = oObject;
        oItem.c_uint(pGridEnum -> GetBMask ());
      }
      else
        ASSERT(0);
      //_variant_t     var = pGridEnum -> GetBMask ( );
      //if ( P3PmsgData_var(m_hP2PmsgHeap,nP2Pos,var) == 0 )
      //  return FALSE;
      //CString       strOption = var;
      //DWORD         dwBMask = pGridEnum -> GetBMask ();
      //P3PmsgObject  oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      //P2PDWORD  oBMask = oObject;
      //oBMask = pGridEnum -> GetBMask ();

      //TODO:LJM testing vvvvvvvvvvvvvvvvvvvvv
//m_oNodeItems.Print(stdout,0);
      //oItem = oObject;
//oItem.Print(stdout,0);
      //TODO:LJM testing ^^^^^^^^^^^^^^^^^^^^^
    }
    else if ( dynamic_cast<CMFCPropertyGridURLProperty*>(pGrid) )
    {
      CMFCPropertyGridURLProperty *pGridURL = dynamic_cast<CMFCPropertyGridURLProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGPEN;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField = P3PmsgData ( pGridURL->GetURL() );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      else
        ASSERT(0);
    }
    else if ( dynamic_cast<CMFCPropertyGridCSVProperty*>(pGrid) )
    {
      CMFCPropertyGridCSVProperty *pGridCSV = dynamic_cast<CMFCPropertyGridCSVProperty*>(pGrid);
      //P3PmsgObject oObject;
      //oObject.Connectx ( m_hP2PmsgHeap, nP2Pos, 0 );
      if ( oObject.IsField() )
      {
        //LOGFONT oLOGPEN;
        oItem = oObject;
        P3PmsgField oField ( oObject );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
        oField = P3PmsgData ( pGridCSV->GetCSV() );
        //memcpy(&oLOGFONT,oField.c_vBlob(),sizeof(LOGFONT));
      }
      else
        ASSERT(0);
    }
    
    else if ( pGrid && pGrid->GetOptionCount() )
    {
     //_variant_t vIndex = pGrid -> GetValue ( );
     _variant_t var    = pGrid -> GetValue ( );
      if ( P3PmsgData_var(m_hP2PmsgHeap,nP2Pos,var) == 0 )
        return FALSE;
    }
    else if ( pGrid )
    {
     _variant_t var = pGrid -> GetValue ( );
      if ( P2PmsgData_var(oItem.r_data(),var) == 0 )
        return FALSE;
    }

    // Perform property changed notification
    if ( m_pCWndNotify   &&
         m_nCWndNotifyWM    )
      m_pCWndNotify->PostMessage ( m_nCWndNotifyWM, (WPARAM)nP2Pos, 0 );
    if ( m_nP2PmsgSinkID   &&
         m_nNotifyP2PsysID    )
      PostP2PmsgSink ( m_nP2PmsgSinkID, m_nNotifyP2PsysID, 0, (P3PmsgItem&)oItem, (WPARAM)nP2Pos, 0 );
    return FALSE;
}

//
//  List management
void
CP2PropertiesWnd::OnExpandAllProperties()
{
    m_wndPropList.SetAlphabeticMode ( FALSE );
    m_wndPropList.ExpandAll();
}
void
CP2PropertiesWnd::OnUpdateExpandAllProperties(CCmdUI *pCmdUI )
{
    pCmdUI -> SetCheck ( !m_wndPropList.IsAlphabeticMode() );
}

void
CP2PropertiesWnd::OnSortProperties()
{
    m_wndPropList.SetAlphabeticMode ( !m_wndPropList.IsAlphabeticMode() );
}
void
CP2PropertiesWnd::OnUpdateSortProperties ( CCmdUI *pCmdUI )
{
    pCmdUI -> SetCheck ( m_wndPropList.IsAlphabeticMode() );
}

void CP2PropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CP2PropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CP2PropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}

/////////////////////////////////////////////////
//  Pre-assigned remote Generic and Home page URL's
//  NOTES: Pre-loaded according to context

//
//  ID_PROPERTIES_WIKI command handler
//
/*void
CP2PropertiesWnd::OnBtnWikiURL ( )
{
//CImage oCImage;
//winHTTP_GetFavicon(this,L"http://www.hotcopper.com.au", oCImage );
//CMFCToolBarButton *pButton = m_wndToolBar.GetButton(0);

    if ( !m_strWikiURL.IsEmpty() )
      CWndApp_PostHtmlURL ( m_strWikiURL, FALSE );
      //ShellExecute ( this->m_hWnd, NULL
      //             , m_strWikiURL
      //             , NULL, NULL
      //             , SW_SHOWNORMAL );
}
void
CP2PropertiesWnd::OnUpdateBtnWikiURL ( CCmdUI *pCmdUI )
{
    pCmdUI->Enable ( m_strWikiURL.IsEmpty() ? TRUE : TRUE );
}

//
//  ID_PROPERTIES_UTUBE1 command handler
//
void
CP2PropertiesWnd::OnBtnUTubeURL1 ( )
{
    if ( !m_strUTubeURL1.IsEmpty() )
      CWndApp_PostHtmlURL ( m_strUTubeURL1, FALSE );
      //ShellExecute ( this->m_hWnd, NULL
      //             , m_strUTubeURL1
      //             , NULL, NULL
      //             , SW_SHOWNORMAL );
}
void
CP2PropertiesWnd::OnUpdateBtnUTubeURL1 ( CCmdUI *pCmdUI )
{
    pCmdUI->Enable ( m_strUTubeURL1.IsEmpty() ? FALSE : TRUE );
}

void
CP2PropertiesWnd::OnBtnHomePageURL1 ( )
{
}
void
CP2PropertiesWnd::OnUpdateBtnHomePageURL1 ( CCmdUI *pCmdUI )
{
    pCmdUI->Enable ( m_strHomePageURL.GetLength() > 0 ? TRUE : FALSE );
}*/


///////////////////////////////////////////////////////////////////////////
//  Safe P2PropertiesWnd grid configuration container
//  NOTES: Instances of this container can exist in the classes configuring
//         the grid.  When an instance of this safe container goes out of
//         scope the P2PropertiesWnd backward connections are canceled and
//         resources recovered.
//       : In summary protects against orphaned P2PropertiesWnd grids trying
//         to reference back into objects that no longer exist
//
SafeP2PropsConfig::SafeP2PropsConfig ( CP2PropertiesWnd *pP2PropsWnd )
{
    MANAGE_RESOURCE_STATE;
    *this = pP2PropsWnd;
}
SafeP2PropsConfig::SafeP2PropsConfig ( ) noexcept
{   MANAGE_RESOURCE_STATE; }
SafeP2PropsConfig::~SafeP2PropsConfig ( )
{ try {Release();}catch(...){} }
// Operators
SafeP2PropsConfig&
SafeP2PropsConfig::operator = ( CP2PropertiesWnd *pP2PropsWnd ) noexcept
{
    if ( pP2PropsWnd == m_pP2PropertiesWnd )
      return *this;                    // No context change
    return *this;
}
SafeP2PropsConfig::operator CP2PropertiesWnd* ( ) noexcept { return m_pP2PropertiesWnd; }
CP2PropertiesWnd*
SafeP2PropsConfig::CreatePropList ( P2PeerTarget *pTargetNotify, GUID oGUID
                                  , P2PsysID nNotifyP2PsysID )
{
    CP2PropertiesWnd *pP2PropsWnd = CP2PropertiesWnd::GetDefaultWnd ( );
    if ( pP2PropsWnd == NULL )
      return nullptr;
    pP2PropsWnd -> CreatePropList ( pTargetNotify, oGUID,  nNotifyP2PsysID );
    m_pP2PropertiesWnd = pP2PropsWnd;
    m_oGUID            = oGUID;
    m_pTargetNotify    = pTargetNotify;
    m_nNotifyP2PsysID  = nNotifyP2PsysID;
    return m_pP2PropertiesWnd;
}
void
SafeP2PropsConfig::Release ( )
{
    if ( m_pP2PropertiesWnd                &&
         m_pP2PropertiesWnd->GetSafeHwnd()    ) {
      if ( m_oGUID == m_pP2PropertiesWnd->m_oGUID )
        CP2PropertiesWnd::EmptyDefaultWnd ( );
      m_pP2PropertiesWnd = nullptr;
      m_pTargetNotify    = nullptr;
      m_nNotifyP2PsysID  = 0;
    }
}

BOOL
SafeP2PropsConfig::IsLoaded ( const P2PeerTarget *pTargetNotify, GUID oGUID )
{
    const CP2PropertiesWnd *pP2PropsWnd
                  = CP2PropertiesWnd::GetDefaultWnd ( );
    if ( pP2PropsWnd == NULL )
      return FALSE;
    if ( pP2PropsWnd->m_pTargetNotify == pTargetNotify &&
         pP2PropsWnd->m_oGUID         == oGUID            )
      return TRUE;
    return FALSE;
}
