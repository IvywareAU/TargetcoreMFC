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
//       : Intention is to simply extend MFC with CMFCPropertyGridURLProperty class
//         and second guess how Microsoft would implement such a class
//
#pragma once

#include "comdef.h"
#include "afxcontrolbarutil.h"

#include "afxglobals.h"

//#include "afxcolorpopupmenu.h"
//#include "afxheaderctrl.h"
//#include "afxpropertygridtooltipctrl.h"

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif

// C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\atlmfc\include\afxpropertygridctrl.h
// CMFCPropertyGridURLProperty

class CMFCPropertyGridURLProperty : public CMFCPropertyGridProperty
{
    DECLARE_DYNAMIC(CMFCPropertyGridURLProperty)

    // Construction
    public:
	      CMFCPropertyGridURLProperty ( const CString& strName, LPCTSTR lpszURL
                                    , DWORD dwFontDialogFlags = CF_EFFECTS | CF_SCREENFONTS
                                    , LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0, COLORREF color = (COLORREF)-1);
      virtual
       ~CMFCPropertyGridURLProperty();

    // Overrides
    public:
	    virtual void
        OnDrawValue ( CDC *pDC, CRect rect );
      virtual void
        OnClickButton ( CPoint oCPoint );
      virtual CString
        FormatProperty ( );

    // Attributes
    public:
	    LPCTSTR GetURL() { return m_strURL; }

    protected:
      CString   m_strURL;
      CString   m_strURLorig;
      COLORREF  m_Color;

	    virtual BOOL
        IsValueChanged() const;
	    virtual void
        ResetOriginalValue();
};


