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
//  Decalarations for CHeaderCtrl extensions
//  NOTES: Primarily draws multi-line headers
//
#pragma once
#include "TargetcoreMFC.h"


/////////////////////////////////////////////////
// CHeaderCtrlEx window

class TargetcoreMFC_EXT CHeaderCtrlEx : public CHeaderCtrl
{
    // Constructors and destructor
    public:
        CHeaderCtrlEx ( );
      virtual
       ~CHeaderCtrlEx ( );

    // Operations
    public:
      BOOL
        SetOwnerDraw ( int nCol );

    // MFC virtuals
    protected:
      virtual void
        DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );

    // MFC Message map
    protected:
    DECLARE_MESSAGE_MAP()
};
