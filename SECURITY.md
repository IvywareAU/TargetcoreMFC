# Security policy

## Reporting a vulnerability

Please do **not** open a public issue for a suspected vulnerability. Report it through
**GitHub private vulnerability reporting** — the *Report a vulnerability* button on this
repository's *Security* tab. That channel is private to the maintainers until an advisory is
published, and it keeps the report attached to the code it concerns.

> **One setup step, and it must be done before this file is published:** private vulnerability
> reporting is off by default. Enable it under *Settings → Code security → Private
> vulnerability reporting*. Until it is on, the button this file tells you to press is not on
> the page, and a policy that names a channel which does not exist is worse than one that
> names none.

Include in the report:

- the version or commit you tested, and which of the four configurations you built,
- which surface you reached it through — the drop target, a property editor, the tree control,
  or `CFile_Ext`,
- the input that triggers it: a dropped data object, a stored `.p2p` image, or the exact path
  or string handed to the helper,
- what you observed: a crash and its faulting address, a heap corruption report, a read or
  write outside an allocation, or an assertion.

Expect an acknowledgement within a week. There is no bounty programme.

## What is in scope

This library is a user-interface layer, so most of it only sees data the user already had. The
exposure is concentrated where bytes arrive from outside the process:

- **The OLE drop target.** `COleDropTarget_Ext` and `COleDataObjectEx` accept a data object
  that *any* process on the desktop can offer, and interpret its private format as a store
  subtree. This is the widest attack surface in the repository and the most valuable class of
  report against it.
- **Rendering a store written elsewhere.** `P2PmsgTreeCtrl` and `P2PropertiesWnd` walk a
  `P2PmsgMgr` image that may have come from a file another party wrote. A malformed store that
  makes either read outside an allocation is in scope.
- **The property editors.** `CMFCPropertyGridPencrxProperty`, `...Fontcrx...`, `...Bnum...`
  and the rest read fixed-size structures out of stored blobs. A blob whose declared extent
  does not match the structure is exactly the interesting case.
- **`CFile_Ext` path handling.** `CFile_PreprocessPath` performs substitution into a path
  string, and `CFile_Load2Memory` reads a whole file into a heap buffer under a caller-supplied
  cap.

## What is NOT in scope, and will not be treated as a vulnerability

- **Passing a malformed structure directly to an exported function in-process.** These are C++
  APIs taking references; a caller that hands one a garbage `P3PmsgItem` is already inside the
  trust boundary. The interesting question is always whether the *stored or dropped bytes* were
  trusted.
- **The unaligned-overlay operators on `P2Pc_vBlob`.** `operator->` and the `StructType&`
  conversion reinterpret stored bytes with no alignment guarantee. This is known, documented in
  `P2Pmsg.h`, owned by Msgcore, and reported there — not here.
- **`ASSERT` firing on malformed input in a Debug build.** That is the assertion doing its job.
  A Release build that then proceeds to corrupt memory *is* in scope; say so explicitly if that
  is what you saw.
- **Anything requiring the attacker to already run code in the process**, control the DLL
  search path, or replace one of the sibling DLLs beside the binary.

## Known limitations we already know about

These are recorded so a report does not spend effort rediscovering them. None is a secret, and
a working exploit for any of them is still worth reporting — it changes the priority.

- **The window-bound surface is untested.** That is most of this library: every control, every
  property editor, every dialog, and the drop target. The `tests/` suite covers `CFile_Ext`
  only, because exercising the rest needs a desktop and a message pump. This is a coverage gap,
  stated plainly rather than hidden behind stubs.

- **`CFile_Length` on a path that does not exist trips an MFC assertion in a Debug build.** The
  value it returns is correct (0), but reaching it runs a `CFileException` through
  `catch_pCException_SetLast`, and that path asserts inside `afxwin1.inl`. The unit suite
  deliberately does not cover that case, and says why at the point where the case would be.

- **`CFile_Memory2File` cannot create a file.** It opens `TRUNCATE_EXISTING`, so the target
  must already exist. It also returned its success and failure values the wrong way round until
  2026 — the success path returned `FALSE`. It had no callers, which is the only reason that
  did no damage.

- **`InPlaceEdit.{h,cpp}` are of unestablished origin** and carry no licence header. See
  [`NOTICE`](NOTICE). This is a licensing problem rather than a security one, but it means two
  files in the tree have had no provenance review of any kind.

- **The drop target has no fuzz harness.** Msgcore has `tests/fuzz`; this repository has no
  equivalent for the data object a drop accepts, which is the surface that most deserves one.

## Supported versions

| Version | Supported |
|---|---|
| 3.1.x | Yes |
| earlier, and everything released as P2PmsgMFC | No |

Version identity comes from `TargetCoreMFC_version.h`; the DLL reports it in its `VERSIONINFO`
resource, so `FileVersion` on the shipped binary is the number to quote in a report.
