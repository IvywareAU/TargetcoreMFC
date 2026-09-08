// Copyright © 2022, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Extends COleDropTarget for integration with all CWnd derived classes
//  NOTES: ColeDropTarget mostly functions within CView.
//
//
#pragma once
#include <afxole.h>
#include "TargetCoreMFC.h"

//
// Helpers
TargetCoreMFC_EXT BOOL
COleDropTarget_Wildcard ( COleDataObject *pDataObject, LPCTSTR lpszWildcard );

//
//  Parent callbacks
//  NOTES: Implementation similar to CView implementation. Parent CWnd
//         provides callback's for standard implementation
typedef void (CALLBACK *fDropMsg)(DWORD_PTR dwUser, DWORD dwUniqueID);
typedef DROPEFFECT (CALLBACK *fpntOnDragEnter)( DWORD_PTR dwUser, COleDataObject* pDataObject
	                                            , DWORD dwKeyState, CPoint point );
typedef DROPEFFECT (CALLBACK *fpntOnDragOver)(DWORD_PTR dwUser, COleDataObject* pDataObject
	                                     , DWORD dwKeyState, CPoint point );
typedef BOOL (CALLBACK *fpntOnDrop)(/*DWORD_PTR dwUser,*/ COleDataObject* pDataObject
                                   , DROPEFFECT dropEffect, CPoint point );

//////////////////////////////////////////////////////////////////////
// CMyDropTarget implements a drop target for the MultiFiler dialog.
// I was also messing around with the IDropTargetHelper stuff in 
// Win2K, which lets the shell draw the nifty shaded drag image when
// you drag into the dialog.  If you're using 9x or NT 4, that stuff
// is disabled.
 
struct IDropTargetHelper;   // forward reference, in case the latest PSDK isn't installed.

class TargetCoreMFC_EXT COleDropTargetEx : public COleDropTarget  
{
    // Constructors & destructor
    public:
        COleDropTargetEx ();
      virtual
       ~COleDropTargetEx();

    // Attributes
    private:
      DWORD_PTR       m_dwUser;
      fDropMsg        m_DropMsg;
      fpntOnDragEnter m_fpntOnDragEnterCB{nullptr};
      fpntOnDragOver  m_fpntOnDragOverCB{nullptr};
      fpntOnDrop      m_fpntOnDropCB{nullptr};

    // Operations
    public:
      void
        RegisterCallbacks ( DWORD_PTR dwUser
                          , fpntOnDragEnter fpntOnDragEnterCB
                          , fpntOnDragOver  fpntOnDragOverCB
                          , fpntOnDrop      fpntOnDropCB );
    void
      SetCallback ( DWORD_PTR dwUser, fDropMsg cbDropMsg);

    // MFC overides
    virtual DROPEFFECT
      OnDragEnter ( CWnd* pWnd, COleDataObject* pDataObject
	                , DWORD dwKeyState, CPoint point );
    virtual DROPEFFECT
      OnDragOver ( CWnd* pWnd, COleDataObject* pDataObject
	               , DWORD dwKeyState, CPoint point );
    virtual BOOL
      OnDrop ( CWnd* pWnd, COleDataObject* pDataObject
             , DROPEFFECT dropEffect, CPoint point );
    virtual void
      OnDragLeave ( CWnd *pCWnd );


//    BOOL ReadHdropData ( COleDataObject* pDataObject );
};

class TargetCoreMFC_EXT COleDataObjectEx
{
    // Constructors & destructor
    public:
        COleDataObjectEx ( COleDataObject *pCOleDataObject ) noexcept;
      virtual
       ~COleDataObjectEx ( ) noexcept;

    // Operations:
    public:
      POSITION
        EnumerateFiles ( LPCTSTR lpszWildcard );
      LPCTSTR
        GetNextFile ( POSITION& pos );
      BOOL
        Wildcard ( LPCTSTR lpszWildcard );

    // Attributes:
    private:
      COleDataObject *m_pDataObject{nullptr};
      CString         m_strWildcard;
      HGLOBAL         m_hDropFiles{0};
      CList<CString>  m_oCListDropFiles;
};
