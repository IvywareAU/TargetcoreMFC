// Copyright © 2010-2013, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CMFCPropertyGridCtrl extensions, ptototypes and definitions
//  NOTES:
//

#include "stdafx.h"

#include "Msgexception.h"
#include "P2Pmsg_Ext.h"
#include "CMFCPropertyGridCtrl_Ext.h"
#include "MFCPropertyGridFontcrxProperty.h"
#include "CFont_Ext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

/*void
CMFCPropertyGridCtrl_Init ( )
{
	//SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

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

	static TCHAR BASED_CODE szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
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
}*/
//
//  Appends control with contents of P2PmsgField
//  NOTES: Recursive implementation via private function, public
//         implementation in successive function
//
//  Parameters:  CMFCPropertyGridCtrl *pCtrl
//               Control to be populated
//
//               P3PmsgField *pField
//               Grid population Field
//
//               CMFCPropertyGridProperty *pGroupParent
//               Parent property grid
//
//  Returns:     BOOL
//               Results summary
//                 0.. OK
//                 ?.. Failure, refer P2PeerLast() for further details
//BOOL
//CMFCPropertyGridCtrl_Init ( CMFCPropertyGridProperty *pGroup, P3PmsgField *pField )
CMFCPropertyGridProperty*
CMFCPropertyGridCtrl_CreateGroup ( P3PmsgField *pField )
{
P3Pmsg_fwprintf(stdout,L"----------------------------------------\n");
pField->Print(stdout,2);
    // Heading
    // NOTES: Passed parent node always assumed to contain heading
    CString strGroupname = pField -> c_name();
    CString strDescription;
    //if ( pField->ATTR.Exists(T_4GRID_TYPE_Typ)                                                  &&
    //     pField->ATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_strcmp(T_4GRID_TYPE_HEADING) == 0    )
    //{
    //  strGroupname   = pField->ATTR.SelectItem  (T_4GRID_TYPE_Lab).r_data().c_wstr();
    //  strDescription = pField->ATTR.DeclareItem(T_4GRID_TYPE_Dsc,(LPCTSTR)"").r_data().c_wstr();
    //}
    //CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty ( strGroupname );
    //                          pGroup -> SetDescription ( strDescription ); 
	  CMFCPropertyGridProperty *pProp  = 0;

    // Explore and populate with passed P2Pmsg object contents
    P3PmsgAttr oATTR = pField->r_Attr().r_Object();
    LPCTSTR lpszPropname = pField->c_name();
    if ( oATTR.Exists(T_4GRID_TYPE_Lab) )
      lpszPropname = oATTR.SelectItem(T_4GRID_TYPE_Lab).c_wstr();
    if ( oATTR.Exists(T_4GRID_TYPE_Dsc) )
      strDescription = oATTR.SelectItem(T_4GRID_TYPE_Dsc).c_wstr();
   _variant_t var = P2PmsgData_var ( pField->r_data() );
    if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                               &&
         oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_SPIN) == 0    )
    {
      int nMin = pField->r_Attr().SelectItem(L"Spin").r_Attr().SelectItem(L"Min").c_int();
      int nMax = pField->r_Attr().SelectItem(L"Spin").r_Attr().SelectItem(L"Max").c_int();
      pProp -> EnableSpinControl ( TRUE, nMin, nMax );
      pProp  -> SetData ( pField->GetP2Pos() );
    }
    else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                &&
              oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_COLOR) == 0    )
    {                              // COLOR Property
      COLORREF oCOLORREF = pField->r_data().c_uint();
      CMFCPropertyGridColorProperty *pColorProp
        = new CMFCPropertyGridColorProperty ( lpszPropname, oCOLORREF, NULL, strDescription );
	    pColorProp -> EnableOtherButton (_T("Other...") );
	    pColorProp -> EnableAutomaticButton ( L"Default", ::GetSysColor(COLOR_3DFACE));
	    //pGroup -> AddSubItem(pColorProp);
      pColorProp -> SetData ( pField->GetP2Pos() );
    }
    else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                               &&
              oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_FONT) == 0    )
    {                              // FONT Property
      LOGFONT oLOGFONT;
      if ( pField->r_data().c_size() >= sizeof(oLOGFONT) )
        memcpy ( &oLOGFONT, pField->c_vBlob(), sizeof(oLOGFONT) );
      else
        StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGFONT, L"Arial" );
	    //CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	    //font->GetLogFont(&oLOGFONT);
      //lstrcpy(oLOGFONT.lfFaceName, L"Arial");

	    CMFCPropertyGridFontProperty *pFontProp
        = new CMFCPropertyGridFontProperty( lpszPropname, oLOGFONT, CF_EFFECTS | CF_SCREENFONTS, strDescription );
      pFontProp -> SetData ( pField->GetP2Pos() );
	    //pGroup -> AddSubItem(pFontProp);
    } 
    else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                  &&
              oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_FONTcrx) == 0    )
    {                              // FONTcrx Property
      LOGFONTcrx oLOGFONTcrx;
      if ( pField->r_data().c_size() >= sizeof(oLOGFONTcrx) )
        memcpy ( &oLOGFONTcrx, pField->c_vBlob(), sizeof(oLOGFONTcrx) );
      else
        StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGFONTcrx, L"Arial" );

      CMFCPropertyGridFontcrxProperty *pFontcrxProp
        = new CMFCPropertyGridFontcrxProperty( lpszPropname, oLOGFONTcrx, CF_EFFECTS | CF_SCREENFONTS, strDescription
                                             , 0, oLOGFONTcrx.oCOLORREF ); 
      //pGroup -> AddSubItem(pFontcrxProp);
      pFontcrxProp -> SetData ( pField->GetP2Pos() );
    } 
    else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                 &&
              oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_OPTION) == 0    )
    {                              // OPTION's Property
      pProp = new CMFCPropertyGridProperty ( lpszPropname, var, strDescription );
      CString strOptions = oATTR.SelectItem(T_4GRID_TYPE_OPTION).c_wstr();
      int       nToken   = 0;
      while ( nToken >= 0 && !strOptions.IsEmpty() )
      {
        CString strOption = strOptions.Tokenize ( _T(";|"), nToken );
        if ( strOption.IsEmpty() )
          continue;
        pProp -> AddOption ( strOption );
        strOption.Empty();
      }
      pProp -> AllowEdit(FALSE);
      pProp -> SetData ( pField->GetP2Pos() );
      //pGroup -> AddSubItem ( pProp );
    }
    else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                  &&
              oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_HEADING) == 0    )
    {                              // HEADING Property
      strGroupname   = oATTR.SelectItem  (T_4GRID_TYPE_Lab).r_data().c_wstr();
      strDescription = oATTR.DeclareItem(T_4GRID_TYPE_Dsc,(LPCTSTR)"").r_data().c_wstr();
      pProp = new CMFCPropertyGridProperty ( strGroupname );
      pProp  -> SetDescription ( strDescription ); 
      //pGroup -> AddSubItem ( pProp );
    }
    else
    {
      pProp = new CMFCPropertyGridProperty ( lpszPropname, var, strDescription );
      pProp  -> SetData ( pField->GetP2Pos() );
      //pGroup -> AddSubItem ( pProp );
      pProp  -> SetData ( pField->GetP2Pos() );
    }

    // Tidy up, and
    //pGroup -> AddSubItem ( pGroup );
    return pProp;
}

//
//  Initialises control with contents of P2PmsgNode
//  NOTES: Recursive implementation via private function, public
//         implementation in successive function
//
//  Parameters:  CMFCPropertyGridCtrl *pCtrl
//               Control to be populated
//
//               P3PmsgNode *pNode
//               Grid population node
//
//               CMFCPropertyGridProperty *pGroupParent
//               Parent property grid
//
//  Returns:     BOOL
//               Results summary
//                 0.. OK
//                 ?.. Failure, refer P2PeerLast() for further details
BOOL
CMFCPropertyGridCtrl_Init ( CMFCPropertyGridCtrl *pCtrl, P3PmsgItem *pItem
                          , CMFCPropertyGridProperty *pGroupParent )
{

//pNode->Print(stdout,2);
    // Heading
    // NOTES: Passed parent node always assumed to contain heading
    CString strGroupname = pItem -> c_name();
    CString strDescription;
    if ( pItem->ATTR.Exists(T_4GRID_TYPE_Typ)                                                  &&
         pItem->ATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_HEADING) == 0    )
    {
      strGroupname   = pItem->ATTR.SelectItem  (T_4GRID_TYPE_Lab).r_data().c_wstr();
      strDescription = pItem->ATTR.DeclareItem(T_4GRID_TYPE_Dsc,(LPCTSTR)"").r_data().c_wstr();
    }
    CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty ( strGroupname );
                              pGroup -> SetDescription ( strDescription ); 
	  CMFCPropertyGridProperty *pProp  = 0;

    // Explore and populate with passed P2Pmsg object contents
    P3PmsgCurs oCursNode ( *pItem );
    for ( int i = 0; oCursNode.Goto(i); i++ )
    {
      /*if ( oCursNode.IsNode() )
      {                      // Delegate through to subordinate node
        CMFCPropertyGridCtrl_Init ( pCtrl, &oCursNode.r_node(), pGroup );
      }
      else*/ if ( oCursNode.IsList() )
        ASSERT(0);
      else if ( oCursNode.IsItem() )
      {
        P3PmsgAttr oATTR = oCursNode.r_item().r_Attr().r_Object();
        LPCTNAM lpszPropname = oCursNode.r_item().c_name();
        if ( oATTR.Exists(T_4GRID_TYPE_Lab) )
          lpszPropname = oATTR.SelectItem(T_4GRID_TYPE_Lab).c_wstr();
        if ( oATTR.Exists(T_4GRID_TYPE_Dsc) )
          strDescription = oATTR.SelectItem(T_4GRID_TYPE_Dsc).c_wstr();
       _variant_t var = P2PmsgData_var ( oCursNode.r_item().r_data() );
        if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                               &&
             oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_SPIN) == 0    )
        {
          int nMin = oCursNode.r_item().r_Attr().SelectItem(L"Spin").r_Attr().SelectItem(L"Min").c_int();
          int nMax = oCursNode.r_item().r_Attr().SelectItem(L"Spin").r_Attr().SelectItem(L"Max").c_int();
          pProp -> EnableSpinControl ( TRUE, nMin, nMax );
          pProp  -> SetData ( oCursNode.r_item().GetP2Pos() );
        }
        else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                &&
                  oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_COLOR) == 0    )
        {                              // COLOR Property
          COLORREF oCOLORREF = oCursNode.r_item().r_data().c_uint();
          CMFCPropertyGridColorProperty *pColorProp
            = new CMFCPropertyGridColorProperty ( lpszPropname, oCOLORREF, NULL, strDescription );
	        pColorProp -> EnableOtherButton (_T("Other...") );
	        pColorProp -> EnableAutomaticButton ( L"Default", ::GetSysColor(COLOR_3DFACE));
	        pGroup -> AddSubItem(pColorProp);
          pColorProp -> SetData ( oCursNode.r_item().GetP2Pos() );
        }
        else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                               &&
                  oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_FONT) == 0    )
        {                              // FONT Property
          LOGFONT oLOGFONT;
          if ( oCursNode.r_item().r_data().c_size() >= sizeof(oLOGFONT) )
            memcpy ( &oLOGFONT, oCursNode.r_item().c_vBlob(), sizeof(oLOGFONT) );
          else
            StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGFONT, L"Arial" );
	        //CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	        //font->GetLogFont(&oLOGFONT);
          //lstrcpy(oLOGFONT.lfFaceName, L"Arial");

	        CMFCPropertyGridFontProperty *pFontProp
            = new CMFCPropertyGridFontProperty( lpszPropname, oLOGFONT, CF_EFFECTS | CF_SCREENFONTS, strDescription );
          pFontProp -> SetData ( oCursNode.r_item().GetP2Pos() );
	        pGroup -> AddSubItem(pFontProp);
        } 
        else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                  &&
                  oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_FONTcrx) == 0    )
        {                              // FONTcrx Property
          LOGFONTcrx oLOGFONTcrx;
          if ( oCursNode.r_item().r_data().c_size() >= sizeof(oLOGFONTcrx) )
            memcpy ( &oLOGFONTcrx, oCursNode.r_item().c_vBlob(), sizeof(oLOGFONTcrx) );
          else
            StockObject2LOGFONT ( DEFAULT_GUI_FONT, oLOGFONTcrx, L"Arial" );

	        CMFCPropertyGridFontcrxProperty *pFontcrxProp
            = new CMFCPropertyGridFontcrxProperty( lpszPropname, oLOGFONTcrx, CF_EFFECTS | CF_SCREENFONTS, strDescription
                                                 , 0, oLOGFONTcrx.oCOLORREF ); 
	        pGroup -> AddSubItem(pFontcrxProp);
          pFontcrxProp -> SetData ( oCursNode.r_item().GetP2Pos() );
        } 
        else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                 &&
                  oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_OPTION) == 0    )
        {                              // OPTION's Property
          pProp = new CMFCPropertyGridProperty ( lpszPropname, var, strDescription );
          CString strOptions = oATTR.SelectItem(T_4GRID_TYPE_OPTION).c_wstr();
          int       nToken   = 0;
          while ( nToken >= 0 && !strOptions.IsEmpty() )
          {
            CString strOption = strOptions.Tokenize ( _T(";|"), nToken );
            if ( strOption.IsEmpty() )
              continue;
            pProp -> AddOption ( strOption );
            strOption.Empty();
          }
          pProp -> AllowEdit(FALSE);
          pProp -> SetData ( oCursNode.r_item().GetP2Pos() );
          pGroup -> AddSubItem ( pProp );
        }
        else if ( oATTR.Exists(T_4GRID_TYPE_Typ)                                                  &&
                  oATTR.SelectItem(T_4GRID_TYPE_Typ).r_data().c_wcscmp(T_4GRID_TYPE_HEADING) == 0    )
        {                              // HEADING Property
          strGroupname   = oATTR.SelectItem  (T_4GRID_TYPE_Lab).r_data().c_wstr();
          strDescription = oATTR.DeclareItem(T_4GRID_TYPE_Dsc,(LPCTSTR)"").r_data().c_wstr();
          pProp = new CMFCPropertyGridProperty ( strGroupname );
          pProp  -> SetDescription ( strDescription ); 
          pGroup -> AddSubItem ( pProp );
        }
        else
        {
          pProp = new CMFCPropertyGridProperty ( lpszPropname, var, strDescription );
          pProp  -> SetData ( oCursNode.r_item().GetP2Pos() );
          pGroup -> AddSubItem ( pProp );
          pProp  -> SetData ( oCursNode.r_item().GetP2Pos() );
        }
      }
      else
        ASSERT(0);
    }

    // Tidy up, and
    if ( pGroupParent )
	    pGroupParent -> AddSubItem ( pGroup );
    else
	    pCtrl -> AddProperty ( pGroup );
    return TRUE;
}

BOOL
CMFCPropertyGridCtrl_Init ( CMFCPropertyGridCtrl *pCtrl, P3PmsgItem *pItem )
{
    // Recursive delegation
    return CMFCPropertyGridCtrl_Init ( pCtrl, pItem, 0 );
}

//BOOL
//CMFCPropertyGridCtrl_Init ( CMFCPropertyGridProperty *pGroup, P3PmsgField *pField )
//{
//    // Recursive delegation
//    return CMFCPropertyGridCtrl_Init ( pGroup, pField );
//}


CMFCPropertyGridProperty*
Create4Grid_HEADING ( LPCSTR lpszHeading, LPCTSTR lpszDescription )
{
    CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty ( lpszHeading );
    pGroup -> SetDescription ( lpszDescription );
    return pGroup;
}
CMFCPropertyGridProperty*
Create4Grid_COLOR ( P3PmsgField& oField, LPCSTR lpszHeading, LPCTSTR lpszDescription )
{
    COLORREF oCOLORREF = oField.r_data().c_uint();
    CMFCPropertyGridColorProperty *pColorProp
      = new CMFCPropertyGridColorProperty ( lpszHeading, oCOLORREF, NULL, lpszDescription );
    pColorProp -> EnableOtherButton (_T("Other...") );
    pColorProp -> EnableAutomaticButton ( L"Default", ::GetSysColor(COLOR_3DFACE));
    pColorProp -> SetData ( oField.GetP2Pos() );
    return pColorProp;
}
CMFCPropertyGridProperty*
Create4Grid ( P3PmsgField& oField, LPCSTR lpszHeading, LPCTSTR lpszDescription )
{
   _variant_t var = P2PmsgData_var ( oField.r_data() );
    CMFCPropertyGridProperty *pProp
      = new CMFCPropertyGridProperty ( lpszHeading, var, lpszDescription );
    pProp -> SetData ( oField.GetP2Pos() );
    return pProp;
}
