// Copyright © 2023, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  CListViewEx declaration
//  NOTES: Extends the CListView class primarily with in-place-editing
//       : Parent CListCtrl manually delegated.  Hence can be used with
//         variants
//
#pragma once
#include "stdafx.h"
#include "TargetcoreMFC.h"
#include "InPlaceEdit.h"

class TargetcoreMFC_EXT CEditableListCtrl : public CListCtrl
{
    // Constructors and destructor
    public:
        CEditableListCtrl ( );
      virtual
       ~CEditableListCtrl ( );
	int GetRowFromPoint( CPoint &point, int *col ) const;
	CEdit* EditSubLabel( int nItem, int nCol );

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLButtonDown(UINT nFlags, CPoint point);
    void
      OnNMDblClick ( NMHDR *pNMHDR, LRESULT *pResult );
    // Attributes
    public:
      DWORD m_dwWM_APP_LISTCTRL_IPEDIT{0};
};