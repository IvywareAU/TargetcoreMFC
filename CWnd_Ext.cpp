// Copyright © 2002-2004, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CWnd extensions
//

#include "StdAfx.h"
#include "CWnd_Ext.h"

///////////////////////////////////////////////////////////////////////
//  CWnd message broadcasting
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients
typedef struct
{
    CWnd *pCWnd;
    UINT  nID;
} BCastRegister;                       // Registration management
static CList<BCastRegister*>
               s_oCListBCastRegister;  // Registered clients list

//
//  Manages client interest in broadcasted  message
//
//
//  Parameters:  CWnd *pCWnd
//               Client
//
//               UINT nID
//               Message identification
//
//               BOOL bRegister
//               Registration mode
//                 TRUE... Register for broadcasts
//                 FALSE.. Deregister
//
//  Returns:     BOOL
//               Registered clients summary
BOOL
CWnd_BCastRegister( CWnd *pCWnd, UINT nID, BOOL bRegister )
{
    // Introduce locals
    BCastRegister *pBCastRegister = 0;

    // Loop through all registered clients
    for ( int k = 0; k < s_oCListBCastRegister.GetCount(); k++ )
    {
      pBCastRegister = s_oCListBCastRegister.RemoveHead();
      if ( pBCastRegister->nID   == nID   &&
           pBCastRegister->pCWnd == pCWnd    )
        break;
      s_oCListBCastRegister.AddTail ( pBCastRegister );
      pBCastRegister = 0;
    }

    // List addition
    if ( bRegister )
    {
      if ( pBCastRegister == 0 )
        pBCastRegister = new BCastRegister;
      pBCastRegister -> nID   = nID;
      pBCastRegister -> pCWnd = pCWnd;
      s_oCListBCastRegister.AddTail ( pBCastRegister );
      pBCastRegister = 0;
    }

    // List removal
    else
      delete pBCastRegister;

    // Tidy up and
    return TRUE;
}

//
//  Broadcasts passed message to all registered clients
//
//
//  Parameters:  UINT nID
//               Message identification
//
//               WPARAM wParam
//               Message dependant
//
//               LPARAM lParam
//               Message dependant
//
//  Returns:     BOOL
//               Registered clients summary
BOOL
CWnd_BCastMessage ( UINT nID, WPARAM wParam, LPARAM lParam )
{
    // Introduce locals
    BCastRegister *pBCastRegister;
    BOOL           bSummary = FALSE;

    // Loop through all registered clients
    POSITION pos = s_oCListBCastRegister.GetHeadPosition();
    while ( pos )
    {
      pBCastRegister = s_oCListBCastRegister.GetNext ( pos );
      if ( pBCastRegister->nID != nID )
        continue;
      pBCastRegister -> pCWnd->PostMessage ( pBCastRegister->nID 
                                           , wParam, lParam );
      bSummary = TRUE;
    }

    // Tidy up and
    return bSummary;
}

//
//  Cancels all message registrations for window
//
//
//  Parameters:  CWnd *pCWnd
//               Client for which all message registrations are
//               to be cancelled
//
//  Returns:     BOOL
//               Registered clients summary
BOOL
CWnd_BCastCancel ( CWnd *pCWnd )
{
    // Introduce locals
    BCastRegister *pBCastRegister = 0;
    BOOL           bSummary = FALSE;

    // Loop through all registered clients
    for ( int k = 0; k < s_oCListBCastRegister.GetCount(); k++ )
    {
      pBCastRegister = s_oCListBCastRegister.RemoveHead();
      if ( pBCastRegister->pCWnd == pCWnd )
      {
        delete pBCastRegister;
        bSummary = TRUE;
        k--;
      }
      else
        s_oCListBCastRegister.AddTail ( pBCastRegister );
    }

    // Tidy up and
    return bSummary;
}
