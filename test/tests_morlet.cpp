/*
 * tests_morlet.cpp
 *
 * Test suite for the Morlet Wavelet
 *
 * Contact:
 * - Jules Françoise <jules.francoise@ircam.fr>
 *
 * This code has been authored by <a href="http://julesfrancoise.com">Jules
 * Françoise</a>
 * in the framework of the <a href="http://skatvg.iuav.it/">SkAT-VG</a> European
 * project,
 * with <a href="frederic-bevilacqua.net">Frederic Bevilacqua</a>, in the
 * <a href="http://ismm.ircam.fr">Sound Music Movement Interaction</a> team of
 * the
 * <a href="http://www.ircam.fr/stms.html?&L=1">STMS Lab</a> - IRCAM - CNRS -
 * UPMC (2011-2015).
 *
 * Copyright (C) 2015 Ircam-Centre Pompidou.
 *
 * This File is part of Wavelet.
 *
 * Wavelet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wavelet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wavelet.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "catch.hpp"
#include "catch_utilities.hpp"
#define WAVELET_TESTING
#include "woma.hpp"

TEST_CASE("MorletWavelet: Construction & Destruction", "[MorletWavelet]") {
    float samplerate = 100.;
    woma::MorletWavelet morlet(samplerate);
    CHECK(morlet.scale.get() == 2. / samplerate);
    CHECK(morlet.samplerate.get() == samplerate);
    CHECK(morlet.mode.get() == woma::Wavelet::RECURSIVE);
    morlet.setDefaultWindowsize();
}

TEST_CASE("MorletWavelet: Values (Recursive)", "[MorletWavelet]") {
    float samplerate = 100.;
    woma::MorletWavelet morlet(samplerate);
    CHECK(morlet.scale.get() == 2. / samplerate);
    CHECK(morlet.samplerate.get() == samplerate);
    CHECK(morlet.mode.get() == woma::Wavelet::RECURSIVE);
    morlet.window_size.set(8);
    std::vector<std::complex<double> > morlet_ref1(8);
    morlet_ref1[0] = std::complex<double>(-0.06031253, 0.03910428);
    morlet_ref1[1] = std::complex<double>(0.05977080, -0.16174061);
    morlet_ref1[2] = std::complex<double>(0.09138012, 0.30891188);
    morlet_ref1[3] = std::complex<double>(-0.37550965, -0.28051408);
    morlet_ref1[4] = std::complex<double>(0.53112597, -0.);
    morlet_ref1[5] = std::complex<double>(-0.37550965, 0.28051408);
    morlet_ref1[6] = std::complex<double>(0.09138012, -0.30891188);
    morlet_ref1[7] = std::complex<double>(0.05977080, 0.16174061);
    for (unsigned int i = 0; i < morlet.window_size.get(); i++) {
        CHECK(morlet.values[i].real() == Approx(morlet_ref1[i].real()));
        CHECK(morlet.values[i].imag() == Approx(morlet_ref1[i].imag()));
    }

    morlet.scale.set(1.3);
    morlet.window_size.set(100);
    std::vector<std::complex<double> > morlet_ref2(100);
    morlet_ref2[0] = std::complex<double>(-0.0211099388884, -0.0574240210325);
    morlet_ref2[1] = std::complex<double>(-0.0189416540714, -0.0583639710611);
    morlet_ref2[2] = std::complex<double>(-0.0167313782369, -0.0592188572996);
    morlet_ref2[3] = std::complex<double>(-0.0144825659197, -0.0599867828565);
    morlet_ref2[4] = std::complex<double>(-0.0121987586152, -0.060665995648);
    morlet_ref2[5] = std::complex<double>(-0.00988357852852, -0.0612548924501);
    morlet_ref2[6] = std::complex<double>(-0.00754072213207, -0.0617520226739);
    morlet_ref2[7] = std::complex<double>(-0.00517395354333, -0.062156091854);
    morlet_ref2[8] = std::complex<double>(-0.0027870977363, -0.0624659648417);
    morlet_ref2[9] = std::complex<double>(-0.00038403360014, -0.0626806686953);
    morlet_ref2[10] = std::complex<double>(0.00203131314124, -0.0627993952591);
    morlet_ref2[11] = std::complex<double>(0.00445497713428, -0.0628215034256);
    morlet_ref2[12] = std::complex<double>(0.00688296071104, -0.0627465210733);
    morlet_ref2[13] = std::complex<double>(0.00931124129959, -0.0625741466763);
    morlet_ref2[14] = std::complex<double>(0.0117357789146, -0.0623042505794);
    morlet_ref2[15] = std::complex<double>(0.0141525237124, -0.0619368759365);
    morlet_ref2[16] = std::complex<double>(0.0165574235953, -0.0614722393064);
    morlet_ref2[17] = std::complex<double>(0.0189464318489, -0.0609107309071);
    morlet_ref2[18] = std::complex<double>(0.0213155147968, -0.0602529145227);
    morlet_ref2[19] = std::complex<double>(0.023660659457, -0.0594995270656);
    morlet_ref2[20] = std::complex<double>(0.025977881183, -0.0586514777912);
    morlet_ref2[21] = std::complex<double>(0.0282632312747, -0.0577098471666);
    morlet_ref2[22] = std::complex<double>(0.0305128045418, -0.0566758853944);
    morlet_ref2[23] = std::complex<double>(0.0327227468051, -0.055551010594);
    morlet_ref2[24] = std::complex<double>(0.0348892623183, -0.0543368066421);
    morlet_ref2[25] = std::complex<double>(0.0370086210957, -0.0530350206772);
    morlet_ref2[26] = std::complex<double>(0.0390771661292, -0.0516475602716);
    morlet_ref2[27] = std::complex<double>(0.0410913204803, -0.0501764902761);
    morlet_ref2[28] = std::complex<double>(0.0430475942304, -0.0486240293433);
    morlet_ref2[29] = std::complex<double>(0.0449425912763, -0.0469925461357);
    morlet_ref2[30] = std::complex<double>(0.0467730159544, -0.045284555226);
    morlet_ref2[31] = std::complex<double>(0.0485356794809, -0.0435027126974);
    morlet_ref2[32] = std::complex<double>(0.0502275061929, -0.041649811453);
    morlet_ref2[33] = std::complex<double>(0.0518455395778, -0.0397287762418);
    morlet_ref2[34] = std::complex<double>(0.0533869480775, -0.037742658414);
    morlet_ref2[35] = std::complex<double>(0.0548490306545, -0.0356946304125);
    morlet_ref2[36] = std::complex<double>(0.056229222109, -0.0335879800156);
    morlet_ref2[37] = std::complex<double>(0.0575250981335, -0.0314261043389);
    morlet_ref2[38] = std::complex<double>(0.0587343800963, -0.0292125036111);
    morlet_ref2[39] = std::complex<double>(0.0598549395409, -0.0269507747355);
    morlet_ref2[40] = std::complex<double>(0.060884802394, -0.0246446046496);
    morlet_ref2[41] = std::complex<double>(0.0618221528705, -0.0222977634984);
    morlet_ref2[42] = std::complex<double>(0.0626653370695, -0.0199140976333);
    morlet_ref2[43] = std::complex<double>(0.0634128662506, -0.0174975224529);
    morlet_ref2[44] = std::complex<double>(0.0640634197863, -0.0150520150991);
    morlet_ref2[45] = std::complex<double>(0.0646158477813, -0.0125816070243);
    morlet_ref2[46] = std::complex<double>(0.0650691733551, -0.0100903764461);
    morlet_ref2[47] = std::complex<double>(0.0654225945817, -0.00758244070273);
    morlet_ref2[48] = std::complex<double>(0.0656754860822, -0.00506194852797);
    morlet_ref2[49] = std::complex<double>(0.0658274002675, -0.00253307225985);
    morlet_ref2[50] = std::complex<double>(0.0658780682279, -0);
    morlet_ref2[51] = std::complex<double>(0.0658274002675, 0.00253307225985);
    morlet_ref2[52] = std::complex<double>(0.0656754860822, 0.00506194852797);
    morlet_ref2[53] = std::complex<double>(0.0654225945817, 0.00758244070273);
    morlet_ref2[54] = std::complex<double>(0.0650691733551, 0.0100903764461);
    morlet_ref2[55] = std::complex<double>(0.0646158477813, 0.0125816070243);
    morlet_ref2[56] = std::complex<double>(0.0640634197863, 0.0150520150991);
    morlet_ref2[57] = std::complex<double>(0.0634128662506, 0.0174975224529);
    morlet_ref2[58] = std::complex<double>(0.0626653370695, 0.0199140976333);
    morlet_ref2[59] = std::complex<double>(0.0618221528705, 0.0222977634984);
    morlet_ref2[60] = std::complex<double>(0.060884802394, 0.0246446046496);
    morlet_ref2[61] = std::complex<double>(0.0598549395409, 0.0269507747355);
    morlet_ref2[62] = std::complex<double>(0.0587343800963, 0.0292125036111);
    morlet_ref2[63] = std::complex<double>(0.0575250981335, 0.0314261043389);
    morlet_ref2[64] = std::complex<double>(0.056229222109, 0.0335879800156);
    morlet_ref2[65] = std::complex<double>(0.0548490306545, 0.0356946304125);
    morlet_ref2[66] = std::complex<double>(0.0533869480775, 0.037742658414);
    morlet_ref2[67] = std::complex<double>(0.0518455395778, 0.0397287762418);
    morlet_ref2[68] = std::complex<double>(0.0502275061929, 0.041649811453);
    morlet_ref2[69] = std::complex<double>(0.0485356794809, 0.0435027126974);
    morlet_ref2[70] = std::complex<double>(0.0467730159544, 0.045284555226);
    morlet_ref2[71] = std::complex<double>(0.0449425912763, 0.0469925461357);
    morlet_ref2[72] = std::complex<double>(0.0430475942304, 0.0486240293433);
    morlet_ref2[73] = std::complex<double>(0.0410913204803, 0.0501764902761);
    morlet_ref2[74] = std::complex<double>(0.0390771661292, 0.0516475602716);
    morlet_ref2[75] = std::complex<double>(0.0370086210957, 0.0530350206772);
    morlet_ref2[76] = std::complex<double>(0.0348892623183, 0.0543368066421);
    morlet_ref2[77] = std::complex<double>(0.0327227468051, 0.055551010594);
    morlet_ref2[78] = std::complex<double>(0.0305128045418, 0.0566758853944);
    morlet_ref2[79] = std::complex<double>(0.0282632312747, 0.0577098471666);
    morlet_ref2[80] = std::complex<double>(0.025977881183, 0.0586514777912);
    morlet_ref2[81] = std::complex<double>(0.023660659457, 0.0594995270656);
    morlet_ref2[82] = std::complex<double>(0.0213155147968, 0.0602529145227);
    morlet_ref2[83] = std::complex<double>(0.0189464318489, 0.0609107309071);
    morlet_ref2[84] = std::complex<double>(0.0165574235953, 0.0614722393064);
    morlet_ref2[85] = std::complex<double>(0.0141525237124, 0.0619368759365);
    morlet_ref2[86] = std::complex<double>(0.0117357789146, 0.0623042505794);
    morlet_ref2[87] = std::complex<double>(0.00931124129959, 0.0625741466763);
    morlet_ref2[88] = std::complex<double>(0.00688296071104, 0.0627465210733);
    morlet_ref2[89] = std::complex<double>(0.00445497713428, 0.0628215034256);
    morlet_ref2[90] = std::complex<double>(0.00203131314124, 0.0627993952591);
    morlet_ref2[91] = std::complex<double>(-0.00038403360014, 0.0626806686953);
    morlet_ref2[92] = std::complex<double>(-0.0027870977363, 0.0624659648417);
    morlet_ref2[93] = std::complex<double>(-0.00517395354333, 0.062156091854);
    morlet_ref2[94] = std::complex<double>(-0.00754072213207, 0.0617520226739);
    morlet_ref2[95] = std::complex<double>(-0.00988357852852, 0.0612548924501);
    morlet_ref2[96] = std::complex<double>(-0.0121987586152, 0.060665995648);
    morlet_ref2[97] = std::complex<double>(-0.0144825659197, 0.0599867828565);
    morlet_ref2[98] = std::complex<double>(-0.0167313782369, 0.0592188572996);
    morlet_ref2[99] = std::complex<double>(-0.0189416540714, 0.0583639710611);
    for (unsigned int i = 0; i < morlet.window_size.get(); i++) {
        CHECK(morlet.values[i].real() == Approx(morlet_ref2[i].real()));
        CHECK(morlet.values[i].imag() == Approx(morlet_ref2[i].imag()));
    }
}
