// Copyright © 2017, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CFile extensions
//

#include "StdAfx.h"
#include <filesystem>
#include "Kernel32_Ext.h"
#include "CFile_Ext.h"
#include "CWndApp_Ext.h"

#include "Msgexception.h"

///////////////////////////////////////////////////////////////////////
//  CWnd message broadcasting
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients

//
//  Parameters:  LPCTSTR lpszFilename
//               File whose contents are to be loaded into memory
//
//               DWORD dwMaxFileSize
//               Maximum size of file to be loaded
//
//  Returns:     char*
//               Pointer to loaded file contents.  Client becomes 
//               responsible for life cycle
//
char*
CFile_Load2Memory ( LPCTSTR lpszFilename, DWORD dwMaxFileSize )
{
    // Locals
    BOOL   bResult;
    char  *pFilecontents = nullptr;

    // Because this is problematic
    try
    {
      // Open existing disk file
      P2PsafeHANDLE
        shFile = CreateFile ( lpszFilename
                            , GENERIC_READ
                            , 0 // No shared access
                            , 0 // No security attributes
                            , OPEN_EXISTING
                            , FILE_ATTRIBUTE_NORMAL 
                            , 0 ); // No template
      if ( shFile == INVALID_HANDLE_VALUE )
        EVERR->MODULE
             ->Message( L"CreateFile(%s) failed", lpszFilename )
             ->HResult( 0 )->Throw();

      // Load file contents
      DWORD dwFileSizeHi = 0;
      DWORD dwFileSizeLo = GetFileSize ( shFile, &dwFileSizeHi );
      if ( dwFileSizeHi                 ||
           dwFileSizeLo > dwMaxFileSize    )
        EVERR->MODULE->AFP(lpszFilename)
             ->Message( L"GetFileSize(%s) failed", lpszFilename )
             ->HResult( 0 )->Throw();
      pFilecontents = new char [dwFileSizeLo+4];
      DWORD dwBytesRead = 0;
      bResult = ReadFile( shFile, pFilecontents, dwFileSizeLo, &dwBytesRead, 0 );
      if ( !bResult || dwBytesRead != dwFileSizeLo )
        EVERR->MODULE
            ->Message( L"ReadFile(%s) data failed", lpszFilename )
            ->HResult( 0 )->Throw();
      pFilecontents[dwBytesRead]   = 0;
      pFilecontents[dwBytesRead+1] = 0;
      return pFilecontents;
    }

    // Exceptions
    catch_pP2Pevent_SetLast
    catch_pCException_SetLast
    catch_ALL_SetLast

    // Tidy up, and
    delete [] pFilecontents;
    return nullptr;
}

//
//  Parameters:  LPCTSTR lpszFilename
//               File to which memory contents are to be dumped
//
//               char *pcDumpBuffer
//               Memory to be dumped
//
//               DWORD dwMaxFileSize
//               Maximum size of file to be loaded
//
//  Returns:     BOOL
//               File dump result
//                 TRUE... Failed
//                 FALSE.. Success
//
BOOL
CFile_Memory2File ( LPCTSTR lpszFilename, char *pcDumpBuffer, DWORD dwDumpBufferSize )
{
    // Because this is problematic
    try
    {
      // Open existing disk file
      P2PsafeHANDLE
        shFile = CreateFile ( lpszFilename
                            , GENERIC_WRITE
                            , 0 // No shared access
                            , 0 // No security attributes
                            , TRUNCATE_EXISTING
                            , FILE_ATTRIBUTE_NORMAL 
                            , 0 ); // No template
      if ( shFile == INVALID_HANDLE_VALUE )
        EVERR->MODULE
             ->Message( L"CreateFile(%s) failed", lpszFilename )
             ->HResult( 0 )->Throw();

      // Load file contents
      DWORD dwBytesWritten = 0;
      if ( !WriteFile(shFile,pcDumpBuffer,dwDumpBufferSize,&dwBytesWritten,0) )
        EVERR -> MODULE -> AFP(lpszFilename)
              -> Message( L"WriteFile(%s) failed", (LPCTSTR)lpszFilename )
              -> HResult( GetLastError() )
              -> Throw();
      return TRUE;
    }

    // Exceptions
    catch_pP2Pevent_SetLast
    catch_pCException_SetLast
    catch_ALL_SetLast

    // Tidy up, and
    return FALSE;
}

//
//  Parameters:  LPCTSTR lpszFilename
//               File whose existance is to be checked
//
//  Returns:     BOOL
//                 TRUE... File exists
//                 FALSE.. File does not exist
//
TargetCoreMFC_EXT BOOL
CFile_Exists ( LPCTSTR lpszFilename )
{
    return PathFileExists ( lpszFilename );
}

//
//  Parameters:  LPCTSTR lpszFilename
//               File whose type is to be checked
//
//  Returns:     BOOL
//                 TRUE... Directory file
//                 FALSE.. Not a directory file
//
TargetCoreMFC_EXT BOOL
CFile_IsDirectory ( LPCTSTR lpszFilename )
{
    CFileStatus status;
    if ( CFile::GetStatus(lpszFilename, status) )
    {
      if ( status.m_attribute == 0x10 )  // Directory
        return TRUE;
      if ( status.m_attribute == 0x11 )  // Directory - Read only
        return TRUE;
    }
    return FALSE;
}

//
//  Parameters:  LPCTSTR lpszFilename
//               File whose length is to be returned
//
//  Returns:     ULONGULONG
//                 0... Error
//                 ?... File length
//
TargetCoreMFC_EXT ULONGLONG
CFile_Length ( LPCTSTR lpszFilename )
{
    // Make exception safe
    try
    {
      CFile  oCFile ( lpszFilename, CFile::modeRead | CFile::shareDenyNone );
      return oCFile.GetLength();
    }

    // Exceptions
    catch_pP2Pevent_SetLast
    catch_pCException_SetLast
    catch_ALL_SetLast
    return 0;
}


TargetCoreMFC_EXT CString  
CFile_Directory(LPCTSTR lpszPathname)  
{  
    wchar_t szDrive[_MAX_DRIVE];  
    wchar_t szDirectory[_MAX_DIR];  
    wchar_t szFilename[_MAX_FNAME];  
    wchar_t szExtension[_MAX_EXT];  
    _wsplitpath_s(lpszPathname,  
                  szDrive, static_cast<size_t>(_MAX_DRIVE),  
                  szDirectory, static_cast<size_t>(_MAX_DIR),  
                  szFilename, static_cast<size_t>(_MAX_FNAME),  
                  szExtension, static_cast<size_t>(_MAX_EXT));  
    return CString(szDirectory);  
}

TargetCoreMFC_EXT CString
CFile_Drive ( LPCTSTR lpszPathname )
{
    wchar_t szDrive    [_MAX_DRIVE];
    wchar_t szDirectory[_MAX_DIR];
    wchar_t szFilename [_MAX_FNAME];
    wchar_t szExtension[_MAX_EXT];
   _wsplitpath_s( lpszPathname
                , szDrive, ARRAYSIZE(szDrive)
                , szDirectory, ARRAYSIZE(szDirectory)
                , szFilename, ARRAYSIZE(szFilename)
                , szExtension, ARRAYSIZE(szExtension) );
    return CString(szDrive);
}

//
//  Replaces standard fields in passed pathname
//  NOTES: <user>, <username>, <name>
//         All translate to active user
//
//  Parameters:  lpszPathname
//               Pathname to be translated
//
//  Returns:     CString
//               Translated path
TargetCoreMFC_EXT CString
CFile_PreprocessPath ( LPCTSTR lpszPathname )
{
    CString strPathname = lpszPathname;
    if ( strPathname.Find(L"<user>") >= 0 ) {
      CString strUsername = CWndApp_GetUsername();
      strPathname.Replace ( L"<user>", strUsername );
    }
    else if ( strPathname.Find(L"<username>") >= 0 ) {
        CString strUsername = CWndApp_GetUsername();
        strPathname.Replace ( L"<username>", strUsername );
    }
    else if ( strPathname.Find(L"<name>") >= 0 ) {
      CString strUsername = CWndApp_GetUsername();
      strPathname.Replace ( L"<name>", strUsername );
    }
    return strPathname;
}

//
//  Copies file from source to destination
//  NOTES: Uses SHFileOperation() utility
//
//  Parameters:  HWND hWndParent
//               Parent window for notifications
//
//               LPCTSTR lpszSourceFolder
//               Source file pathname
//
//               LPCTSTR lpszDestinFolder
//               Destination folder path
//
//  Returns:     int
//               SHFileOperation() return result
//
TargetCoreMFC_EXT int
CFile_SHFileOperation ( HWND hWndParent, LPCTSTR lpszSourceFolder, LPCTSTR lpszDestinFolder )
{
	//Log(_T("Recursive file copy lpszSourceFolder %s to %s"), lpszSourceFolder, to);
    // Setup Folder paths
    TCHAR szSourceFolder[MAX_PATH*2];
    _tcscpy_s(szSourceFolder, lpszSourceFolder);
    szSourceFolder[_tcsclen(lpszSourceFolder) + 1] = NULL;
    szSourceFolder[_tcsclen(lpszSourceFolder) + 2] = NULL;

    TCHAR szDestinFolder[MAX_PATH*2];
    _tcscpy_s(szDestinFolder, lpszDestinFolder);
    szDestinFolder[_tcsclen(lpszDestinFolder) + 1] = NULL;
    szDestinFolder[_tcsclen(lpszDestinFolder) + 2] = NULL;

    // Command structure
    SHFILEOPSTRUCT oFileOp = {0};
    oFileOp.hwnd   = hWndParent;
    oFileOp.wFunc  = FO_COPY;
    oFileOp.pFrom  = szSourceFolder;
    oFileOp.pTo    = szDestinFolder;
    oFileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;

    // Implementation	
    int result = SHFileOperation(&oFileOp);
    return result;
}
