// Copyright © 2019, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CHeaderCtrl extensions
//  NOTES: Primarily draws multi-line headers
//

#include "stdafx.h"
#include "CHeaderCtrl_Ext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

CHeaderCtrlEx::CHeaderCtrlEx()
{
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}

/////////////////////////////////////////////////
//  Operations

//
//  Sets CHeaderCtrlEx item to owner draw
//
//  Parameters:  int nCol
//               Column number
//
//  Returns:     BOOL
//               Result code
BOOL
CHeaderCtrlEx::SetOwnerDraw ( int nCol )
{
    BOOL bResult;
    HDITEM hdItem;
    hdItem.mask = HDI_FORMAT;
    GetItem ( nCol, &hdItem );
    hdItem.fmt |= HDF_OWNERDRAW;
    bResult = SetItem ( nCol, &hdItem );
    ASSERT(bResult);
    return bResult;
}

void
CHeaderCtrlEx::DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    ASSERT(lpDrawItemStruct->CtlType == ODT_HEADER);

    HDITEM hdi;
    TCHAR  lpBuffer[256] = {0};

    hdi.mask = HDI_TEXT;
    hdi.pszText = &lpBuffer[0];
    hdi.cchTextMax = 256;

    GetItem(lpDrawItemStruct->itemID, &hdi);
  	
	  CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	  //THIS FONT IS ONLY FOR DRAWING AS LONG AS WE DON'T DO A SetFont(...)
	  pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
    // Draw the button frame.
    ::DrawFrameControl(lpDrawItemStruct->hDC, 
       &lpDrawItemStruct->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);

    UINT uFormat = DT_CENTER;
    //DRAW THE TEXT
    ::DrawTextW(lpDrawItemStruct->hDC, lpBuffer, (int)wcslen(lpBuffer), 
               &lpDrawItemStruct->rcItem, uFormat);

    pDC->SelectStockObject(SYSTEM_FONT);
}

/////////////////////////////////////////////////
//  MFC Message handlers
BEGIN_MESSAGE_MAP ( CHeaderCtrlEx, CHeaderCtrl )
END_MESSAGE_MAP()

