# Contributing to TargetCoreMFC

## Before you spend time on a change

TargetCoreMFC is the visual layer of the MSCS family. Three constraints shape almost every
change here:

- **It is an MFC extension DLL.** Everything crossing the boundary is `TargetCoreMFC_EXT`,
  built with `_AFXEXT` against the shared MFC runtime. Changing an exported signature breaks
  every consumer at link time, not compile time.
- **It sits on top of three siblings** — [Msgcore](../Msgcore), [TargetCore](../TargetCore)
  and [MsgcoreMFC](../MsgcoreMFC). Its controls take `P3PmsgItem` and `P3PmsgAttr` by
  reference, so a change to how those are used here is a change against contracts other
  repositories own.
- **The store is the model.** The controls do not hold a copy of what they display; they read
  and write the same offsets a `Save` will write to disk. A change that introduces a cached
  copy needs to say what keeps it in step, and why that is better than not having one.

If you are proposing something that alters any of those, say so in the issue before writing
the code.

## Sign your work — the Developer Certificate of Origin

Every commit must carry a `Signed-off-by` line:

```
Signed-off-by: Jane Developer <jane@example.com>
```

`git commit -s` adds it for you. Use your real name and an address you read.

That line means you certify the [Developer Certificate of Origin
1.1](https://developercertificate.org/): that you wrote the contribution or otherwise have the
right to submit it under the Apache License, Version 2.0, and that you understand the
contribution and its record are public and permanent.

**Why this is enforced from the first commit rather than added later.** Only a rights holder
can license code. Once a contribution arrives with no record of who held the rights and under
what terms, the project can no longer answer that question for its own tree — and the option
of ever relicensing, dual-licensing, or granting an exception closes permanently, because
there is nobody identifiable to ask. A sign-off is the cheapest possible way to keep that
answerable. A pull request without one cannot be merged, no matter how good it is.

This repository already carries one file pair whose origin nobody can establish —
`InPlaceEdit.{h,cpp}`, see [`NOTICE`](NOTICE). That is exactly the situation the sign-off
exists to prevent happening again.

## Making a change

1. **One concern per commit.** A refactor and a behaviour change in the same commit cannot be
   reviewed, reverted or bisected independently.

2. **Write the message for someone reading it in five years.** Say what changed and *why* —
   the subject line as an imperative sentence, the body for the reasoning.

3. **Build all four configurations.** `Debug` / `Release` × `x86` / `x64`, one diagnostic
   regime — `/W4`, `/std:c++20`, Unicode, shared MFC. **Zero errors and zero warnings from
   this project is the standing bar**, and it is currently met. Note the platform naming: the
   solution calls the 32-bit platform `x86`, the project underneath calls it `Win32`.

4. **Run the suite.** `.\tests\build_run_suite.bat` — 10 cases, 22 checks. It builds against
   the `Debug|x64` DLL, so **build the library first**; the script links the import library it
   finds and will otherwise happily test the previous binary.

5. **If you touch `CFile_Ext`, add a case.** It is the one part of this library that is
   testable without a desktop, so it is the one part with no excuse.

6. **The window-bound surface has no tests, and that is written down rather than papered
   over.** Do not add a stub that always passes to close the gap — a stub reads like coverage
   and is worse than an absence that is documented. If you find a way to exercise a control
   headlessly, that is a genuinely valuable contribution.

## House style

Match the file you are editing — it is older than any convention document and it is internally
consistent. In particular:

- Spaces inside parentheses in calls and declarations: `Foo ( a, b )`.
- The return type on its own line above the function name, as every `*_Ext.cpp` does.
- `NOTES:` comment blocks above a declaration explain *why*, not what. When you fix something
  subtle, leave the reasoning behind in one of these.
- Do not delete a comment that records a hazard just because you fixed the code around it.
  Rewrite it to say what is true now.
- An unused parameter on a deliberate stub gets `UNREFERENCED_PARAMETER` and a sentence
  saying it is ignored by design, not a commented-out name.

## Three things that will get a change rejected on sight

- **A return value that means the opposite of what the caller expects.**
  `CFile_Memory2File` returned `FALSE` from its success path and `TRUE` from its exception
  handler, and was shipped that way. It had no callers, which is the only reason it did no
  damage. Get the polarity right and write a test that would notice.

- **Reading a stored blob through `operator->` or the `StructType&` conversion.** `P2Pmsg.h`
  says plainly that those overlay a struct on stored bytes with no alignment guarantee, and
  that every access through them is undefined behaviour for a type wanting alignment. Use
  `Load()` and `Store()`.

- **A version number written anywhere but `TargetCoreMFC_version.h`.** The resource script
  reads its `FILEVERSION`, `PRODUCTVERSION` and both version strings from that header. It
  exists because three numbers had drifted apart and shipped that way.

## Licence

Contributions are accepted under the Apache License, Version 2.0 — see [`LICENSE`](LICENSE).
Every new source file carries the standard header; copy one from a neighbouring file and put
the current year on it.
