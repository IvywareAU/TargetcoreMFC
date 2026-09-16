// Copyright © 2016, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CMFCPropertyGridURLProperty implementations.
//  NOTES: Integrates URL definitions into CMFCPropertyGridCtrl
//       : Implementation pattern modelled upon MFCPropertyGridFontProperty
//         which in turn was located in the following MFC code base file
//         C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\atlmfc\src\mfc\afxpropertygridctrl.cpp
//

#include "stdafx.h"
#include "TargetcoreMFC.h"
#include "URLDialog.h"
#include "MFCPropertyGridURLProperty.h"
#include "afxcontrolbarutil.h"
#include "afxpropertygridctrl.h"

//#include "afxcolorbar.h"
//#include "afxwinappex.h"
//#include "afxshellmanager.h"
//#include "afxmenuimages.h"
//#include "afxvisualmanager.h"
//#include "afxtoolbarcomboboxbutton.h"
//#include "afxpopupmenu.h"
//#include "afxdrawmanager.h"
//#include "afxmaskededit.h"
//#include "afxspinbuttonctrl.h"
//#include "afxtagmanager.h"
//#include "afxctrlcontainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define AFX_ID_HEADER 1
//#define AFX_ID_SCROLL_VERT 2
//#define AFX_STRETCH_DELTA 2

//#define AFX_TEXT_MARGIN 4

//#define AFX_UM_UPDATESPIN (WM_USER + 101)

//#define visualManager CMFCVisualManager::GetInstance()

#define AFX_PROP_HAS_LIST 0x0001
#define AFX_PROP_HAS_BUTTON 0x0002
//#define AFX_PROP_HAS_SPIN 0x0004

//#define AFX_FORMAT_CHAR   _T("%c")
//#define AFX_FORMAT_SHORT  _T("%d")
//#define AFX_FORMAT_LONG   _T("%ld")
//#define AFX_FORMAT_USHORT _T("%u")
//#define AFX_FORMAT_ULONG  _T("%u")
//#define AFX_FORMAT_FLOAT  _T("%f")
//#define AFX_FORMAT_DOUBLE _T("%lf")

/////////////////////////////////////////////////////////////////////////////
// CMFCPropertyGridFontProperty object

IMPLEMENT_DYNAMIC(CMFCPropertyGridURLProperty, CMFCPropertyGridProperty)

CMFCPropertyGridURLProperty::CMFCPropertyGridURLProperty ( const CString& strName, LPCTSTR lpszURL
                                 , DWORD dwPenDialogFlags, LPCTSTR lpszDescr, DWORD_PTR dwData, COLORREF color )
                           : CMFCPropertyGridProperty ( strName, COleVariant((long)0), lpszDescr, dwData )
                                                      //, m_dwPenDialogFlags(dwPenDialogFlags)
{
    UNREFERENCED_PARAMETER(dwPenDialogFlags);
    m_dwFlags        = AFX_PROP_HAS_BUTTON;
    m_Color          = color;
    //m_bIsValueList   = TRUE;
    m_strURL         = lpszURL;
    m_strURLorig     = lpszURL;
    //m_oLOGPENorig    = oLOGPEN;
    m_bAllowEdit     = TRUE; //FALSE;
    //m_Color          = oLOGPEN.lopnColor; //color;
}

CMFCPropertyGridURLProperty::~CMFCPropertyGridURLProperty()
{
}
void
CMFCPropertyGridURLProperty::OnDrawValue ( CDC *pDC, CRect rect )
{
AllowEdit();
    //CRect rectColor = rect;
    //rect.left += rect.Height();
    CMFCPropertyGridProperty::OnDrawValue ( pDC, rect );

    //rectColor.right = rectColor.left + rectColor.Height();
    //rectColor.DeflateRect(1, 1);
    //rectColor.top++;
    //rectColor.left++;

    //CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color);
    //pDC -> FillRect  (rectColor, &br);
    //pDC -> Draw3dRect(rectColor, 0, 0);
}

void
CMFCPropertyGridURLProperty::OnClickButton(CPoint /*point*/)
{
    ASSERT_VALID(this);
    ASSERT_VALID(m_pWndList);
    ASSERT_VALID(m_pWndInPlace);
    ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

    //LOGPEN   lfPrev     = m_oLOGPENcrx;
    //COLORREF nColorPrev = m_Color;

    m_bButtonIsDown = TRUE;
    Redraw();

#pragma warning(disable : 4244)
    CURLDialog dlg ( m_strURL, 0, NULL, m_pWndList );
#pragma warning(default : 4244)

    //if ( m_Color != (COLORREF)-1 )
    //{
    //  dlg.m_cf.rgbColors = m_Color;
    //}

    if ( dlg.DoModal() == IDOK )
    {
      m_strURL = dlg.m_strURL;
      //memcpy ( &m_oLOGPENcrx, dlg.GetCurrentPen(), sizeof(m_oLOGPENcrx) );
      //m_Color = dlg.GetColor();

      //if (memcmp(&lfPrev, &m_oLOGPENcrx, sizeof(lfPrev) - sizeof(lfPrev.lfFaceName)) || _tcscmp( lfPrev.lfFaceName, m_oLOGPENcrx.lfFaceName) || nColorPrev != m_Color)
      if ( IsValueChanged() )
      {
        m_pWndList->OnPropertyChanged(this);
      }

      if (m_pWndInPlace != NULL)
      {
        m_pWndInPlace->SetWindowText(FormatProperty());
      }
      else
      {
        m_varValue = (LPCTSTR) FormatProperty();
      }
    }

    if (m_pWndInPlace != NULL)
    {
      m_pWndInPlace->SetFocus();
    }
    else
    {
      m_pWndList->SetFocus();
    }

    m_bButtonIsDown = FALSE;
    Redraw();
}

CString
CMFCPropertyGridURLProperty::FormatProperty ( )
{
    CString str;
    CWindowDC dc(m_pWndList);

    int nLogY = dc.GetDeviceCaps(LOGPIXELSY);
    if (nLogY != 0)
    {
      str.Format ( L"%s\0", (LPCTSTR)m_strURL );
    }
    else
    {
      str.Format ( L"nLogY=0" );
      //str = m_oLOGPENcrx.lfFaceName;
    }

    return str;
}

void
CMFCPropertyGridURLProperty::ResetOriginalValue()
{
    CMFCPropertyGridProperty::ResetOriginalValue();
    m_strURL = m_strURLorig;
}
BOOL
CMFCPropertyGridURLProperty::IsValueChanged() const
{
    return (m_strURL == m_strURLorig) ? FALSE : TRUE;
}


