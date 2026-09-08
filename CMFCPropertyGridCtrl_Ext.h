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
#pragma   once
#include "TargetCoreMFC.h"
#include "CFont_Ext.h"
#include "P2Pmsg.h"

//
//  Initialises control with contents of P2PmsgNode
//
//  Parameters:  CMFCPropertyGridCtrl *pCtrl
//               Control to be populated
//
//               P3PmsgNode *pNode or P3PmsgField *pField
//               Poluation data
//
TargetCoreMFC_EXT CMFCPropertyGridProperty*
CMFCPropertyGridCtrl_CreateGroup ( P3PmsgItem *pItem );
TargetCoreMFC_EXT BOOL
CMFCPropertyGridCtrl_Init ( CMFCPropertyGridCtrl *pCtrl, P3PmsgItem *pItem );

TargetCoreMFC_EXT CMFCPropertyGridProperty*
Create4Grid_HEADING ( LPCSTR lpszHeading, LPCTSTR lpszDescription );
TargetCoreMFC_EXT CMFCPropertyGridProperty*
Create4Grid_COLOR ( P3PmsgItem& oItem, LPCSTR lpszHeading, LPCTSTR lpszDescription );
TargetCoreMFC_EXT CMFCPropertyGridProperty*
Create4Grid ( P3PmsgItem& oItem, LPCSTR lpszHeading, LPCTSTR lpszDescription );

//
//  CMFCPropertyGridFontProperty extension
/*class TargetCoreMFC_EXT CMFCPropertyGridFontcrxProperty : public CMFCPropertyGridFontProperty
{
    public:
        CMFCPropertyGridFontcrxProperty ( const CString& strName
                                        , LOGFONTcrx& lfcrx
                                        , DWORD dwFontDialogFlags = CF_EFFECTS | CF_SCREENFONTS
                                        , LPCTSTR lpszDescr = NULL
                                        , DWORD_PTR dwData = 0
                                        , COLORREF oCOLORREF = (COLORREF)-1 )
          : CMFCPropertyGridFontProperty( strName, lfcrx, dwFontDialogFlags, lpszDescr, dwData, oCOLORREF ) { };
      virtual
       ~CMFCPropertyGridFontcrxProperty ( ) { };
};*/