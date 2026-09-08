// Copyright © 2026 Ivyware Pty Ltd, Khrustal & Mann
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
// SuiteMain.cpp
//
// Entry point for this repository's unit-test runner.
//
// Until this landed, TargetCoreMFC had no tests at all. What it has now is
// deliberately modest, and the modesty is the point: this library is the
// VISUAL layer of the family, and the great majority of it cannot be reached
// without a desktop and a message pump. One suite covers what genuinely can.
//
// WHAT IS DELIBERATELY NOT HERE: every window-bound control and dialog. They
// are left visibly absent rather than stubbed; a stub that always passes reads
// like coverage and is worse than a gap that is written down.
//
// A suite that is compiled out must SAY SO. The guard below has an #else that
// reports the skip, and tf_runner_finish refuses to call a run with
// nSkipped > 0 a pass: a dropped suite otherwise produces output
// indistinguishable from a clean full run.

#include <afx.h>
#include <afxwin.h>

#include <cstdio>

#include "TestFramework.h"

// ---------------------------------------------------------------------------
int main(int /*argc*/, char* /*argv*/[])
{
    if (!tf_runner_startup("TargetCoreMFC unit tests"))
        return 1;

    int nSkipped = 0;

#if !defined(TARGETCOREMFC_NO_SUITE)
    printf("\n[TargetCoreMFC]\n");
    RunTargetCoreMFCSuite();
#else
    printf("\n[TargetCoreMFC]\n  SKIPPED (TARGETCOREMFC_NO_SUITE)\n");
    ++nSkipped;
#endif

    return tf_runner_finish(nSkipped);
}
