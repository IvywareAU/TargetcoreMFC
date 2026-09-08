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
//  CEnumFilesinFolder definitions and prototypes
//  NOTES: Enumerates files in Folder
//

#include "stdafx.h"
#include "EnumFilesinFolder.h"
#include "CString_Ext.h"
#include "Msgexception.h"


// ChartCtrl

CEnumFilesinFolder::CEnumFilesinFolder ( LPCTSTR lpszFolderPath )
{
    m_strFolderPath = lpszFolderPath;
}

CEnumFilesinFolder::~CEnumFilesinFolder()
{
    if ( m_hFind ) ::FindClose(m_hFind);
    m_hFind = 0;
}

BOOL
CEnumFilesinFolder::GetNextFile ( )
{
    // Initialise access nominated to nominated folder
    // NOTES: Effectively a wildcard for folder contents
    if ( m_hFind == 0 )
    {
      CString strFolderWildcard  = m_strFolderPath;
              strFolderWildcard += L"\\*.*";
      //WIN32_FIND_DATA fd; 
      m_hFind = ::FindFirstFile ( strFolderWildcard, &m_fd );
      if ( m_hFind == INVALID_HANDLE_VALUE ) {
        m_hFind = 0;
        EVERR->MODULE
             ->Message ( L"[%s] directory access failure", (LPCTSTR)m_strFolderPath )
             ->Advice  ( L"Folder enumeration failed")
             ->Cancel ( );
        return FALSE;
      }
    }

    // Enumeration pass
    else
    {
       if ( ::FindNextFile(m_hFind,&m_fd) )
         return TRUE;
       if ( GetLastError() == ERROR_NO_MORE_FILES )
         return FALSE;
       EVERR->MODULE
            ->Message ( L"[%s] directory access failure", (LPCTSTR)m_strFolderPath )
            ->Advice  ( L"Folder enumeration failed")
            ->Cancel ( );
       return FALSE;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////
//  Property exposure
BOOL
CEnumFilesinFolder::IsFolder ( )
{
    return m_fd.dwFileAttributes == FILE_ATTRIBUTE_SYSTEM ? TRUE : FALSE;
}
BOOL
CEnumFilesinFolder::IsFileType ( LPCTSTR lpszWildcard )
{
    return CString_Wildcard ( GetFilename(), lpszWildcard, TRUE );

}

LPCTSTR
CEnumFilesinFolder::GetFilename ( )
{
    m_strLastFilename = &m_fd.cFileName[0];
    return m_strLastFilename;
}

LPCTSTR
CEnumFilesinFolder::GetFilepath ( )
{
    m_strLastFilepath  = m_strFolderPath;
    m_strLastFilepath += "\\";
    m_strLastFilepath += GetFilename();
    return m_strLastFilepath;
}
///////////////////////////////////////////////////////////////////////
//  External operations


