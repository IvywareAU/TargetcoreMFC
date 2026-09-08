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
//  CFile extension prototypes and definitions
//

#pragma   once
#include "TargetCoreMFC.h"

//
//  CFile type operations
//  NOTES: Static functions are just simpler
//       : Facilitates message broadcasting to registered clients
//
TargetCoreMFC_EXT char*
CFile_Load2Memory ( LPCTSTR lpszFilename, DWORD dwMaxSize = 200000 );

//  NOTES: The file MUST ALREADY EXIST. The handle is opened TRUNCATE_EXISTING,
//         so this overwrites the contents of a file and does NOT create one.
//       : Returns TRUE on success. It used to have this inverted - FALSE was
//         returned from the success path and TRUE from the exception handler,
//         so every caller that tested it read the result backwards. There were
//         no callers when that was found, which is the only reason it did no
//         damage.
TargetCoreMFC_EXT BOOL
CFile_Memory2File ( LPCTSTR lpszFilename, char *pcDumpBuffer, DWORD dwMaxFileSize );

TargetCoreMFC_EXT BOOL
CFile_Exists ( LPCTSTR lpszFilename );

TargetCoreMFC_EXT BOOL
CFile_IsDirectory ( LPCTSTR lpszFilename );

TargetCoreMFC_EXT ULONGLONG
CFile_Length ( LPCTSTR lpszFilename );

TargetCoreMFC_EXT CString
CFile_Drive ( LPCTSTR lpszPathname );

TargetCoreMFC_EXT CString
CFile_Directory ( LPCTSTR lpszPathname );

TargetCoreMFC_EXT CString
CFile_PreprocessPath ( LPCTSTR lpszPathname );

TargetCoreMFC_EXT int
CFile_SHFileOperation ( HWND hWndParent, LPCTSTR lpszSourceFolder, LPCTSTR lpszDestinFolder );

