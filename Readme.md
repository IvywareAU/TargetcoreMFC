# TargetCoreMFC

**The visual layer of the MSCS family: MFC controls, property grids and dialogs bound to a live
TargetCore store.**

[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/C%2B%2B-20-00599C.svg)](#requirements)
[![Windows](https://img.shields.io/badge/Windows-MFC%20extension%20DLL-0078D6.svg)](#requirements)
[![Version](https://img.shields.io/badge/version-3.0.0.0-6f42c1.svg)](#versioning)
[![Since](https://img.shields.io/badge/since-2002-6f42c1.svg)](#targetcoremfc)

[MsgcoreMFC](../MsgcoreMFC) can put a control's *state* into a store and take it out again. This
library does the other half: it puts the *store itself* on screen, as something a person can look
at, expand, drag and edit.

A `P2PmsgMgr` is a tree of named fields with typed values and `@`-qualified attributes. That is
already the shape of a tree control and of a property grid, and TargetCoreMFC is the binding
between them — a `CTreeCtrl` whose nodes are the store's nodes, and a `CMFCPropertyGridCtrl` whose
rows are the store's fields, edited in place and written straight back.

```cpp
#include "P2PropertiesWnd.h"

//  A grid row backed by a field of the store: edits land on the store, not on a copy.
m_wndProperties.AddItem ( pGroup, mgr.r_Desc()[L"window"][L"title"],
                          L"Title", L"Caption shown in the title bar" );
```

> **This library was called `P2PmsgMFC` until 2026** and produced `P2PmsgMFC.dll`. It was renamed
> to match the component it is the MFC layer of: it links `TargetCore.lib`, and the old name
> described a message type rather than a component. Consumers link `TargetCoreMFC.lib` and include
> `TargetCoreMFC.h`; the export macro is `TargetCoreMFC_EXT`.

---

## Status

All four configurations build clean — zero errors and **zero warnings** from this project at `/W4`,
under one C++ standard. The unit suite runs green: 10 cases, 22 checks. Version identity is 3.0.0.0
and comes from a single header.

The 32-bit configurations build for the first time in a long while. They had been failing with
`LNK1104: cannot open file 'TargetCore.lib'` because nothing had ever built TargetCore for Win32;
the project references added here build it.

The window-bound surface — which is most of this library — has no automated coverage, because
exercising it needs a desktop and a message pump. That gap is real, and is stated here and in
[`SECURITY.md`](SECURITY.md) rather than filled with stubs that would only look like coverage.

## What this library provides

### 1. Controls bound to a live store

The three big components each marry one MFC control to a `P2PmsgMgr`:

| | |
|---|---|
| `P2PmsgTreeCtrl` | A `CTreeCtrl` whose items are the store's nodes. Expanding an item walks the store; the tree does not hold a copy. |
| `P2PropertiesWnd` | A `CMFCPropertyGridCtrl` over a node's fields, edited in place and written back. |
| `P2PexploreCtrl` | The explorer-style pairing of the two, plus the `P2Pevent` display used to show what the diagnostics vocabulary raised. |

Because the store is the model, there is no synchronisation step and no second copy to keep in
step — the control reads and writes the same offsets that a `Save` will write to disk.

### 2. The property vocabulary that renders the crx types

A `CMFCPropertyGridCtrl` only knows the types Microsoft gave it. The store carries pens, brushes,
fonts, enumerations, URLs and CSV selections — including the widened `LOGPENcrx` and `LOGBRUSHcrx`
forms that MsgcoreMFC produces — so this library supplies the grid rows that can display and edit
them:

```
CMFCPropertyGridPenProperty        CMFCPropertyGridPencrxProperty
CMFCPropertyGridBrushProperty      CMFCPropertyGridFontcrxProperty
CMFCPropertyGridEnumProperty       CMFCPropertyGridBnumProperty
CMFCPropertyGridURLProperty        CMFCPropertyGridCSVProperty
```

Each owns its editing UI — `CBrushDialog`, `CSVDialog`, `CURLDialog`, the pen dialogs — so a
consumer gets an editor for a stored `LOGBRUSHcrx` by adding one row, not by writing a dialog.

### 3. The surrounding MFC extensions

`COleDropTarget_Ext` and `COleDataObjectEx` carry the drag-and-drop that moves subtrees between
windows; `CListViewEx` and `CInPlaceEdit` give a list view in-place subitem editing;
`CHeaderCtrlEx`, `CMFCRibbonDatePickerControl`, `CFile_Ext`, `CWnd_Ext`, `CPoint_Ext` and
`CEnumFilesinFolder` are the smaller helpers the controls above are built from. `CFile_Ext` is the
one part of the library that touches no window at all, which is why it is also the one part with
tests.

---

## Requirements

* Visual Studio 2026, platform toolset **v145**, Windows SDK 10.
* MFC, used as a **shared** (dynamic) library — this project is an MFC extension DLL, built with
  `_AFXEXT`, `/std:c++20` and Unicode throughout, in all four configurations.
* **Msgcore**, **TargetCore** and **MsgcoreMFC**, checked out as sibling directories. They are
  built for you: see *Building*.

No environment variable is required. `$(WDMSCS_LIB)` defaults to `MSCS\lib` through the repo-root
`Directory.Build.props`, and `Directory.Build.targets` fails the build rather than let a bad value
send the import library somewhere nobody will look for it. An explicit `WDMSCS_LIB` still wins.

**Outside the MSCS tree**, this repository's own `Directory.Build.props` takes over: it chains to
an outer one when there is one, so nothing changes for an in-tree build, and otherwise defaults
`$(WDMSCS_LIB)` to the `lib\` *beside* this repository — the shared staging directory for a row of
sibling clones, since this project links `Msgcore.lib`, `TargetCore.lib` and `MsgcoreMFC.lib` and
must find all three where those builds left them. That default only covers the simplest layout.
**If you clone the components side by side, set `WDMSCS_LIB` once to a single absolute directory
and build them all with it**; the sibling repositories do not all agree on a default, and
`TargetCore` has no repo-local `Directory.Build.props` at all, so it needs the variable rather
than merely preferring it.
`WDMSCS_VSUTILS` is optional: when defined, a post-build step publishes the DLL into the shared
MSCS tree.

## Building

```
msbuild "TargetCoreMFC(2026).sln" -p:Configuration=Debug   -p:Platform=x64
msbuild "TargetCoreMFC(2026).sln" -p:Configuration=Release -p:Platform=x64
msbuild "TargetCoreMFC(2026).sln" -p:Configuration=Debug   -p:Platform=x86
msbuild "TargetCoreMFC(2026).sln" -p:Configuration=Release -p:Platform=x86
```

The solution names the 32-bit platform **`x86`**, while the project underneath names it `Win32`.
Building the solution takes `x86`.

**The three siblings are built first, automatically**, through `<ProjectReference>` entries whose
only job is ordering — `LinkLibraryDependencies` is false, so the explicit `.lib` list stays the
one that is linked. Their own warnings appear in the log as a result; they belong to those
projects, and this one contributes none.

Everything lands under one root, `out\<Platform>\<Configuration>\`, with intermediates in `obj\`
inside it. At run time a consumer needs `Msgcore.dll`, `TargetCore.dll` and `MsgcoreMFC.dll`
beside `TargetCoreMFC.dll`.

## Tests

```
.\tests\build_run_suite.bat
```

Builds and runs the suite against the `Debug|x64` DLL — **build the library first**, or the script
will test the previous binary. 10 cases, 22 checks, exit code 0 only when every check passed.

Coverage is `CFile_Ext` — path splitting, existence and directory tests, and a
`Memory2File`/`Load2Memory` byte round trip. That is the honest extent of what this library can be
tested on without a desktop, and the suite says so at the top of the file rather than padding
itself out.

## Versioning

`TargetCoreMFC_version.h` is the single source of version identity, at **3.0.0.0**, matching
TargetCore. The resource script reads `FILEVERSION`, `PRODUCTVERSION` and both version strings from
it, and `TargetCoreMFC.h` exposes the macros to consumers:

```cpp
#if !TARGETCOREMFC_VERSION_AT_LEAST(3,0,0)
#  error TargetCoreMFC 3.0.0 or later is required
#endif
```

The header exists because those numbers had drifted: the resource declared `FILEVERSION 2,1,0,1`
beside `PRODUCTVERSION 3,0,0,0` and a `FileVersion` string of `"3.0.0.0"` — three numbers, no two
of them agreeing.

## Security

See [`SECURITY.md`](SECURITY.md) for how to report a vulnerability, what is in scope — the
drag-and-drop path is the widest attack surface here — and the limitations already known.

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md). Every commit needs a `Signed-off-by` line, all four
configurations must build warning-free, and the suite must stay green.

---

# Licence

TargetCoreMFC is licensed under the **Apache License, Version 2.0**. See [`LICENSE`](LICENSE) for
the full text, or <http://www.apache.org/licenses/LICENSE-2.0>.

```
Copyright 2002-2026 Ivyware Pty Ltd, Khrustal & Mann

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

## Files not covered by the licence

Some files here are Microsoft project-template, wizard-generated or sample-derived files. They keep
Microsoft's own notices and are **not** licensed under Apache 2.0: `Targetver.h`, `Resource.h`,
`TargetCoreMFC.rc`, `res/TargetCoreMFC.rc2`, `stdafx.h`, `stdafx.cpp`, and the Visual Studio
solution and project files.

**`InPlaceEdit.h` and `InPlaceEdit.cpp` carry no licence header at all, on purpose.** They are the
only sources here with no copyright notice of any kind, and the class they declare matches a widely
circulated public sample. Asserting ownership over code whose origin cannot be established would be
worse than leaving a documented gap. See [`NOTICE`](NOTICE) — this must be resolved before the
repository is published.

## Dependencies licensed separately

This library is an MFC extension DLL and links against the Microsoft Foundation Classes, the Visual
C++ runtime, and the Windows SDK — all licensed by Microsoft, none redistributed here.

It also depends on **Msgcore**, **TargetCore** and **MsgcoreMFC**, separate repositories in the
same family, under the same licence and the same copyright holders. All are dependencies rather
than bundled code: no source from any of them is included in this repository.
