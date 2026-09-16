// Copyright © 2021, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
#pragma once
#include "TargetcoreMFC.h"

// CefCtrl

class TargetcoreMFC_EXT CEnumFilesinFolder
{
    public:
	      CEnumFilesinFolder ( LPCTSTR lpszFolderPath );
	    virtual
       ~CEnumFilesinFolder ( );

    // Operations
    public:
      BOOL
        GetNextFile ( );

    // Property exposure
    public:
      BOOL
        IsFolder ( );
      BOOL
        IsFileType ( LPCTSTR lpszWildcard );
      LPCTSTR
        GetFilename ( );
      LPCTSTR
        GetFilepath ( );

    // Attributes
    protected:
      HANDLE          m_hFind{0};
    public:
      WIN32_FIND_DATA m_fd; 
      CString m_strFolderPath;
      CString m_strLastFilepath;
      CString m_strLastFilename;
};

///////////////////////////////////////////////////////////////////////
//  CefCtrl helpers
