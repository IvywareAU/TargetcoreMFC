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
//  Base class for VideomsGroupView tile implementation and management
//  NOTES: Intention is all contained tiles be derived from this object.
//
//
#include "stdafx.h"

//#include "ServerMain.h"
#include "COleDropTarget_Ext.h"
#include "CString_Ext.h"
#include "Kernel32_Ext.h"

#include "Msgexception.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL TargetcoreMFC_EXT
COleDropTarget_Wildcard ( COleDataObject *pDataObject, LPCTSTR lpszWildcard )
{
    CString strWildcard = lpszWildcard;
    BOOL      bResult   = FALSE;
    // (Implemented by VargaG., on 2020.02.17.)
    //m_droppedFilePathNames.RemoveAll();

    // (Implemented by VargaG., on 2020.02.17.)
    CLIPFORMAT clipFormat = CF_HDROP;
    HGLOBAL shGlobalDropFiles = pDataObject->GetGlobalData(clipFormat);

    // (Implemented by VargaG., on 2020.02.18.)
    if (shGlobalDropFiles != NULL)
    {
		  // (Implemented by VargaG., on 2020.02.17.)
		  try
		  {
			  // (Implemented by VargaG., on 2020.02.17.)
			  LPDROPFILES pDropFiles = (LPDROPFILES)::GlobalLock(shGlobalDropFiles);
			  if (pDropFiles == NULL)
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  ASSERT(FALSE);
				  return FALSE;
			  }

			  // (Implemented by VargaG., on 2020.02.17.)
			  if (pDropFiles->fWide)
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  bool bAdditionalFileNameExistst = true;

				  // (Implemented by VargaG., on 2020.02.17.)
				  PWCHAR pDroppedFilePathAndName = (PWCHAR)((PCHAR)pDropFiles + pDropFiles->pFiles);

				  do
				  {
					  // (Implemented by VargaG., on 2020.02.17.)
            CString strDroppedFilename = pDroppedFilePathAndName;
            if ( CString_Wildcard(strDroppedFilename,strWildcard,TRUE) )
              bResult = TRUE;
					  //m_droppedFilePathNames.Add(pDroppedFilePathAndName);

					  // Get the number of characters in the string, not including the terminating null character.
					  // (Implemented by VargaG., on 2020.02.17.)
					  size_t nDroppedFilePathAndNameLengthWithoutTerminatingNullCharacter = wcsnlen_s(pDroppedFilePathAndName, MAX_PATH);

					  // (Implemented by VargaG., on 2020.02.17.)
					  pDroppedFilePathAndName = pDroppedFilePathAndName + nDroppedFilePathAndNameLengthWithoutTerminatingNullCharacter + 1;

					  // (Implemented by VargaG., on 2020.02.17.)
					  bAdditionalFileNameExistst = (*pDroppedFilePathAndName != L'\x0');
				  }
				  while (bAdditionalFileNameExistst);
			  }
			  else
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  TRACE("ANSI characters are not supported, only Unicode characters!");
				  ASSERT(FALSE);
			  }

			  return bResult;
		  }
		  catch(...)
		  {
			  // (Implemented by VargaG., on 2020.02.17.)
			  BOOL bGlobalUnlockResult = ::GlobalUnlock(shGlobalDropFiles);
			  DWORD dwLastError = GetLastError();
			  if ((bGlobalUnlockResult == FALSE) && (dwLastError != NO_ERROR))
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  ASSERT(FALSE);
				  return FALSE;
			  }
		  }
	  }

	  // (Implemented by VargaG., on 2020.02.18.)
	  return bResult;
}

///////////////////////////////////////////////////////////////////////
//  COleDataObject extension
//

//  Constructors & destructor
COleDataObjectEx::COleDataObjectEx ( COleDataObject *pDataObject ) noexcept
{
    m_pDataObject = pDataObject;
}
 
COleDataObjectEx::~COleDataObjectEx ( ) noexcept
{
    if ( m_hDropFiles )
      ::GlobalUnlock ( m_hDropFiles );
}

// Operations
POSITION
COleDataObjectEx::EnumerateFiles ( LPCTSTR lpszWildcard )
{
    m_strWildcard = lpszWildcard;
    m_oCListDropFiles.RemoveAll();

    // Preamble, expect issues
		try
		{
      const CLIPFORMAT clipFormat = CF_HDROP;
      if ( !m_hDropFiles )
        m_hDropFiles = m_pDataObject->GetGlobalData(clipFormat);
      if ( !m_hDropFiles )
        EVERR->MODULE
             ->Message( L"GetGlobalData() failed" )
             ->HResult( 0 )
             ->Throw();
			// Initialse search
			LPDROPFILES pDropFiles = (LPDROPFILES)::GlobalLock(m_hDropFiles);
			if ( pDropFiles == NULL )
        return 0;

			// Observe character type
			if ( !pDropFiles->fWide )
        EVERR->MODULE
             ->Message( L"ANSI characters are not supported, only Unicode" )
             ->Throw();

			// (Implemented by VargaG., on 2020.02.17.)
			//bool bAdditionalFileNameExistst = true;

			// Parse out files
			PWCHAR pDroppedFilePathAndName = (PWCHAR)((PCHAR)pDropFiles + pDropFiles->pFiles);
		  while ( *pDroppedFilePathAndName )
			{
        // Confirm against wildcard
        CString strDroppedFilename = pDroppedFilePathAndName;
        if ( CString_Wildcard(strDroppedFilename,m_strWildcard,TRUE) )
          m_oCListDropFiles.AddTail ( strDroppedFilename );

        // Move forward
        pDroppedFilePathAndName += (strDroppedFilename.GetLength() + 1);
      }
			// Tidy up, and
      return m_oCListDropFiles.GetHeadPosition();
    }
    // Exceptions
    catch_pP2Pevent_Cancel
    catch_pCException_Cancel
    catch_ALL_Cancel

	  // Tidy up, and
	  return 0;
}

LPCTSTR
COleDataObjectEx::GetNextFile ( POSITION& pos )
{
    if ( !pos || m_oCListDropFiles.IsEmpty() ) {
      pos = 0;
      return L"\0";
    }
    return (LPCTSTR)m_oCListDropFiles.GetNext(pos);
}

//
//  Summarises number of encapsulated files matching the wildcard
//
//  Parameters:  LPCTSTR lpszWildcard
//               Matching wildcard
//
//  Returns:     BOOL
//               Number of matching files
BOOL
COleDataObjectEx::Wildcard ( LPCTSTR lpszWildcard )
{
    EnumerateFiles(lpszWildcard);
    return m_oCListDropFiles.GetCount() ? TRUE : FALSE;
}


/*extern*/ UINT g_uCustomClipbrdFormat;
///////////////////////////////////////
//  Constructor and destructor

COleDropTargetEx::COleDropTargetEx()
{
    m_DropMsg = NULL;
}

COleDropTargetEx::~COleDropTargetEx()
{
}

///////////////////////////////////////
//  Operations

//  Register client callbacks
//  NOTES: If not defined simply fall through to the base class,
//         which in turn delegates through to the CView etc
//
//  Parameters:  DWORD_PTR dwUser
//               User defined data common to all callbacks
//
//               OnDragEnterCB pFncOnDragEnterCB
//               OnDragEnter() function callback
void
COleDropTargetEx::RegisterCallbacks ( DWORD_PTR dwUser
                     , fpntOnDragEnter pnfOnDragEnterCB
                     , fpntOnDragOver  pnfOnDragOverCB
                     , fpntOnDrop      pnfOnDropCB )
{
    m_fpntOnDragEnterCB = pnfOnDragEnterCB;
    m_fpntOnDragOverCB  = pnfOnDragOverCB;
    m_fpntOnDropCB      = pnfOnDropCB;
    m_dwUser            = dwUser;
}

///////////////////////////////////////
//  MFC virtual overrides
//  NOTES: All simply delegate back to parent if defined

DROPEFFECT
COleDropTargetEx::OnDragEnter ( CWnd *pCWnd, COleDataObject *pDataObject
		                          , DWORD dwKeyState, CPoint point )
{
    // Simply delegate according to settings
    if ( m_fpntOnDragEnterCB == nullptr )
      return __super::OnDragEnter ( pCWnd, pDataObject, dwKeyState, point );
    return (*m_fpntOnDragEnterCB)(m_dwUser, pDataObject, dwKeyState, point);
}

DROPEFFECT
COleDropTargetEx::OnDragOver ( CWnd* pCWnd, COleDataObject *pDataObject
		                         , DWORD dwKeyState, CPoint point )
{
    // Simply delegate according to settings
    if ( m_fpntOnDragOverCB == nullptr )
      return __super::OnDragOver ( pCWnd, pDataObject, dwKeyState, point );
    return (*m_fpntOnDragOverCB)(m_dwUser, pDataObject, dwKeyState, point);
}

BOOL
COleDropTargetEx::OnDrop ( CWnd* pCWnd, COleDataObject* pDataObject
		                          , DROPEFFECT dropEffect, CPoint point )
{
    // Simply delegate according to settings
    if ( m_fpntOnDragOverCB == nullptr )
      return __super::OnDrop ( pCWnd, pDataObject, dropEffect, point );
    return (*m_fpntOnDropCB)( pDataObject, dropEffect, point);
}

void
COleDropTargetEx::OnDragLeave ( CWnd* )
{
}


/////////////////////////////////////////////////////////////////////////////

/*COleDropTargetEx1::COleDropTargetEx1()
{
}
COleDropTargetEx1::~COleDropTargetEx1()
{
}

// (Implemented by VargaG., on 2020.02.17.)
BOOL
COleDropTargetEx1::IsFilePathNameListIsDragged(COleDataObject* a_pDataObject)
{
    // (Implemented by VargaG., on 2020.02.17.)
    m_droppedFilePathNames.RemoveAll();

    // (Implemented by VargaG., on 2020.02.17.)
    CLIPFORMAT clipFormat = CF_HDROP;
    HGLOBAL shGlobalDropFiles = a_pDataObject->GetGlobalData(clipFormat);

    // (Implemented by VargaG., on 2020.02.18.)
    if (shGlobalDropFiles != NULL)
    {
		  // (Implemented by VargaG., on 2020.02.17.)
		  try
		  {
			  // (Implemented by VargaG., on 2020.02.17.)
			  LPDROPFILES pDropFiles = (LPDROPFILES)::GlobalLock(shGlobalDropFiles);
			  if (pDropFiles == NULL)
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  ASSERT(FALSE);
				  return FALSE;
			  }

			  // (Implemented by VargaG., on 2020.02.17.)
			  if (pDropFiles->fWide)
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  bool bAdditionalFileNameExistst = true;

				  // (Implemented by VargaG., on 2020.02.17.)
				  PWCHAR pDroppedFilePathAndName = (PWCHAR)((PCHAR)pDropFiles + pDropFiles->pFiles);

				  do
				  {
					  // (Implemented by VargaG., on 2020.02.17.)
					  m_droppedFilePathNames.Add(pDroppedFilePathAndName);

					  // Get the number of characters in the string, not including the terminating null character.
					  // (Implemented by VargaG., on 2020.02.17.)
					  size_t nDroppedFilePathAndNameLengthWithoutTerminatingNullCharacter = wcsnlen_s(pDroppedFilePathAndName, MAX_PATH);

					  // (Implemented by VargaG., on 2020.02.17.)
					  pDroppedFilePathAndName = pDroppedFilePathAndName + nDroppedFilePathAndNameLengthWithoutTerminatingNullCharacter + 1;

					  // (Implemented by VargaG., on 2020.02.17.)
					  bAdditionalFileNameExistst = (*pDroppedFilePathAndName != L'\x0');
				  }
				  while (bAdditionalFileNameExistst);
			  }
			  else
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  TRACE("ANSI characters are not supported, only Unicode characters!");
				  ASSERT(FALSE);
			  }

			  return TRUE;
		  }
		  catch(...)
		  {
			  // (Implemented by VargaG., on 2020.02.17.)
			  BOOL bGlobalUnlockResult = ::GlobalUnlock(shGlobalDropFiles);
			  DWORD dwLastError = GetLastError();
			  if ((bGlobalUnlockResult == FALSE) && (dwLastError != NO_ERROR))
			  {
				  // (Implemented by VargaG., on 2020.02.17.)
				  ASSERT(FALSE);
				  return FALSE;
			  }
		  }
	  }

	  // (Implemented by VargaG., on 2020.02.18.)
	  return FALSE;
}

// (Implemented by VargaG., on 2020.02.13.)
DROPEFFECT
COleDropTargetEx1::OnDragEnter ( CWnd *a_pWnd, COleDataObject *a_pDataObject
                               , DWORD a_dwKeyState, CPoint a_point )
{
    UNREFERENCED_PARAMETER(a_point);
    DROPEFFECT dropEffect = DROPEFFECT_NONE;

    // (Implemented by VargaG., on 2020.02.17.)
    if (IsFilePathNameListIsDragged(a_pDataObject))
    {
      // (Implemented by VargaG., on 2020.02.17.)
      dropEffect = DROPEFFECT_COPY;
    }

    //Tidy up, and
    return dropEffect;
}

// (Implemented by VargaG., on 2020.02.17.)
DROPEFFECT COleDropTargetEx1::OnDragOver(CWnd* a_pWnd, COleDataObject* a_pDataObject, DWORD a_dwKeyState, CPoint a_point)
{
	// (Implemented by VargaG., on 2020.02.17.)
	DROPEFFECT dropEffect = DROPEFFECT_NONE;

	// (Implemented by VargaG., on 2020.02.17.)
	if (IsFilePathNameListIsDragged(a_pDataObject))
	{
		// (Implemented by VargaG., on 2020.02.17.)
		dropEffect = DROPEFFECT_COPY;
	}

	// (Implemented by VargaG., on 2020.02.17.)
	return dropEffect;
}

// (Implemented by VargaG., on 2020.02.13.)
BOOL COleDropTargetEx1::OnDrop(CWnd* a_pWnd, COleDataObject* a_pDataObject, DROPEFFECT a_dropEffect, CPoint a_point)
{
	// (Implemented by VargaG., on 2020.02.17.)
	BOOL bDropAllowed = IsFilePathNameListIsDragged(a_pDataObject);

	// (Implemented by VargaG., on 2020.02.17.)
	if (bDropAllowed)
	{
		// (Implemented by VargaG., on 2020.02.17.)
		TRACE("Number of files have been dropped: %d.\n", m_droppedFilePathNames.GetCount());

		// (Implemented by VargaG., on 2020.02.17.)
		for (int nDroppedFileDataIndex = 0; nDroppedFileDataIndex < m_droppedFilePathNames.GetCount(); nDroppedFileDataIndex++)
		{
			// (Implemented by VargaG., on 2020.02.17.)
			CString stringFileDataIndexed = m_droppedFilePathNames.GetAt(nDroppedFileDataIndex);
			TRACE(
				_T("Dropped file path and name (%d of %d): %s.\n"),
				nDroppedFileDataIndex + 1,
				m_droppedFilePathNames.GetCount(),
				stringFileDataIndexed
			);
		}
	}

	return bDropAllowed;
}*/

