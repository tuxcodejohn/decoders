/****************************************************************************
**
**  This file is a part of the program "CMDFAX".
**
**  Copyright © 2020 Jürgen Reche
**
** CMDFSK is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** CMDFSK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "FIRFilter.h"

#include <stdlib.h>

FIRFilter::FIRFilter() {
  History = NULL;
  init(FLT_DEFAULT);
}

FIRFilter::FIRFilter(FILTER_TYPE ft) {
  History = NULL;
  init(ft);
}

FIRFilter::~FIRFilter() {
  delete [] History;
}

void FIRFilter::init(FILTER_TYPE ft) {

  FilterType = ft;
  
  switch (ft) {
  case FLT_DEFAULT :
  case FLT_NO_FILTER :
    Taps = TapsDummy;
    nTaps = 4;
    break;
  case FLT_FIRLP17 :
    Taps = TapsFLT_FIRLP17;
    nTaps = 17;
    break;
  case FLT_FIRLP33 :
    Taps = TapsFLT_FIRLP33;
    nTaps = 33;
    break;
  case FLT_FIRLP41 :
    Taps = TapsFLT_FIRLP41;
    nTaps = 41;
    break;
  case FLT_FIRLP75 :
    Taps = TapsFLT_FIRLP75;
    nTaps = 75;
    break;
  }
  if (History != NULL)
    delete [] History;
  History    = new double [nTaps];
  HistPos    = 0;
  for (int i = 0; i < nTaps; i++)
    History[i] = 0.0;
}

void FIRFilter::processSamples(REAL * InBuffer, int nInBuffer, REAL * OutBuffer) {
int  i, iTap, iHist;
double acc;

  for (i = 0; i < nInBuffer; i++) {
    History[HistPos] = double(InBuffer[i]);
    acc = 0.0;
    iTap = 0;
    for (iHist = HistPos; iHist < nTaps; iHist++)
      acc += History[iHist] * Taps[iTap++];
    for (iHist = 0; iHist < HistPos; iHist++)
      acc += History[iHist] * Taps[iTap++];
    HistPos--;
    if (HistPos < 0)
      HistPos = nTaps - 1;
    OutBuffer[i] = REAL(acc);
  }
}

double FIRFilter::TapsDummy[4] = { 1.0, 1.0, 1.0, 1.0 };

double FIRFilter::TapsFLT_FIRLP17[17] = {
  -0.011036781383643182,
  -0.016680759016180383,
  -0.016606059932688193,
  -0.0008355223769623179,
  0.03525624541241494,
  0.0882038628885327,
  0.14566363828357665,
  0.19035080567888754,
  0.20723237662028737,
  0.19035080567888754,
  0.14566363828357665,
  0.0882038628885327,
  0.03525624541241494,
  -0.0008355223769623179,
  -0.016606059932688193,
  -0.016680759016180383,
  -0.011036781383643182
};

double FIRFilter::TapsFLT_FIRLP41[41] = {
  0.007085817982802199,
  0.0070235629102127625,
  0.008835905990920667,
  0.009393894155368255,
  0.008045514457023478,
  0.004440914049571914,
  -0.0013322988172519783,
  -0.008614048550869039,
  -0.01616481135471674,
  -0.022298652469428557,
  -0.025134977958403398,
  -0.022933175902296106,
  -0.014457504839439295,
  0.0006991871758669469,
  0.021909837541089088,
  0.04748432155581245,
  0.07483906496372171,
  0.10084489311163836,
  0.12229705220163795,
  0.13642615878944395,
  0.1413568830994063,
  0.13642615878944395,
  0.12229705220163795,
  0.10084489311163836,
  0.07483906496372171,
  0.04748432155581245,
  0.021909837541089088,
  0.0006991871758669469,
  -0.014457504839439295,
  -0.022933175902296106,
  -0.025134977958403398,
  -0.022298652469428557,
  -0.01616481135471674,
  -0.008614048550869039,
  -0.0013322988172519783,
  0.004440914049571914,
  0.008045514457023478,
  0.009393894155368255,
  0.008835905990920667,
  0.0070235629102127625,
  0.007085817982802199
};

double FIRFilter::TapsFLT_FIRLP75[75] = {
  0.005897199325549442,
  0.002130942371947245,
  0.0018493267578349926,
  0.001026295054454951,
  -0.000330905564887262,
  -0.0021156013132621475,
  -0.004118605036035563,
  -0.006051634399077273,
  -0.007585952922665054,
  -0.008395412515817047,
  -0.008206951399059368,
  -0.006870131148607181,
  -0.004422146807032172,
  -0.0010486430967276744,
  0.002837705096418659,
  0.0066987313575210695,
  0.009901136018637608,
  0.011824451001354812,
  0.011954963056719147,
  0.009986586759971085,
  0.005904204600202541,
  0.00003182642608088757,
  -0.00696853709031807,
  -0.014142496252406466,
  -0.020329596883373744,
  -0.02431693547217144,
  -0.024976374634762665,
  -0.02144416360716185,
  -0.013247829642845918,
  -0.0004170546913644272,
  0.016479595904357155,
  0.03635679938813447,
  0.057704167180121904,
  0.0787352661249996,
  0.09758007904848945,
  0.11249565011590322,
  0.12206244126950792,
  0.12535936285786026,
  0.12206244126950792,
  0.11249565011590322,
  0.09758007904848945,
  0.0787352661249996,
  0.057704167180121904,
  0.03635679938813447,
  0.016479595904357155,
  -0.0004170546913644272,
  -0.013247829642845918,
  -0.02144416360716185,
  -0.024976374634762665,
  -0.02431693547217144,
  -0.020329596883373744,
  -0.014142496252406466,
  -0.00696853709031807,
  0.00003182642608088757,
  0.005904204600202541,
  0.009986586759971085,
  0.011954963056719147,
  0.011824451001354812,
  0.009901136018637608,
  0.0066987313575210695,
  0.002837705096418659,
  -0.0010486430967276744,
  -0.004422146807032172,
  -0.006870131148607181,
  -0.008206951399059368,
  -0.008395412515817047,
  -0.007585952922665054,
  -0.006051634399077273,
  -0.004118605036035563,
  -0.0021156013132621475,
  -0.000330905564887262,
  0.001026295054454951,
  0.0018493267578349926,
  0.002130942371947245,
  0.005897199325549442
};

double FIRFilter::TapsFLT_FIRLP33[33] = {
-0.00067060786374659,
-0.0028024263671805176,
-0.006559791024261584,
-0.01011859045136201,
-0.009701765162178464,
-0.002119713467090786,
0.01112852705487889,
0.02179863483371811,
0.018600373632200837,
-0.0036082553434888007,
-0.035153897673409366,
-0.051768065102405864,
-0.027267495014326383,
0.047481121373119356,
0.15263417421041953,
0.2453012577229926,
0.2822271671405825,
0.2453012577229926,
0.15263417421041953,
0.047481121373119356,
-0.027267495014326383,
-0.051768065102405864,
-0.035153897673409366,
-0.0036082553434888007,
0.018600373632200837,
0.02179863483371811,
0.01112852705487889,
-0.002119713467090786,
-0.009701765162178464,
-0.01011859045136201,
-0.006559791024261584,
-0.0028024263671805176,
-0.00067060786374659
};
