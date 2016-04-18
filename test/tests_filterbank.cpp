/*
 * tests_wavelet_filter.cpp
 *
 * Test suite for the Wavelet Filter
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

TEST_CASE("Filterbank: Attributes", "[Filterbank]") {
    float samplerate(100.);
    float frequency_min = 1.;
    float frequency_max = 30.;
    float bands_per_octave = 4;
    woma::Filterbank filterbank(samplerate, frequency_min, frequency_max,
                                bands_per_octave);
    CHECK(filterbank.getAttribute<float>("samplerate") == samplerate);
    CHECK(filterbank.getAttribute<float>("frequency_min") == frequency_min);
    CHECK(filterbank.getAttribute<float>("frequency_max") == frequency_max);
    CHECK(filterbank.getAttribute<float>("bands_per_octave") ==
          bands_per_octave);
    CHECK(filterbank.getAttribute<woma::Family>("family") ==
          woma::Family::Morlet);
    CHECK(filterbank.getAttribute<woma::Wavelet::WaveletDomain>("mode") ==
          woma::Wavelet::WaveletDomain::RECURSIVE);
    CHECK(filterbank.getAttribute<float>("delay") ==
          woma::Wavelet::DEFAULT_DELAY());
    CHECK_THROWS(filterbank.getAttribute<std::size_t>("window_size"));
    CHECK_THROWS(filterbank.getAttribute<std::size_t>("scale"));
    samplerate = 200.;
    filterbank.setAttribute("samplerate", samplerate);
    CHECK(filterbank.getAttribute<float>("samplerate") == samplerate);
    for (auto &wav : filterbank.wavelets_)
        CHECK(wav->getAttribute<float>("samplerate") == samplerate);
    int numbands = int(filterbank.size());
    filterbank.setAttribute<float>("bands_per_octave", bands_per_octave / 2.);
    CHECK(filterbank.size() == numbands / 2);
    filterbank.bands_per_octave.set(bands_per_octave);
    CHECK(filterbank.size() == numbands);
    frequency_min = 10.;
    filterbank.setAttribute<float>("frequency_min", frequency_min);
    CHECK(filterbank.getAttribute<float>("frequency_min") == frequency_min);
    CHECK(filterbank.getAttribute<float>("omega0") == 5.);
}

TEST_CASE("Filterbank: Scales", "[Filterbank]") {
    float samplerate(100.);
    float frequency_min = 1.;
    float frequency_max = 30.;
    float bands_per_octave = 4;
    woma::Filterbank filter(samplerate, frequency_min, frequency_max,
                            bands_per_octave);
    std::vector<double> scales(20);
    scales[0] = 0.02828427;
    scales[1] = 0.03363586;
    scales[2] = 0.04;
    scales[3] = 0.04756828;
    scales[4] = 0.05656854;
    scales[5] = 0.06727171;
    scales[6] = 0.08;
    scales[7] = 0.09513657;
    scales[8] = 0.11313708;
    scales[9] = 0.13454343;
    scales[10] = 0.16;
    scales[11] = 0.19027314;
    scales[12] = 0.22627417;
    scales[13] = 0.26908685;
    scales[14] = 0.32;
    scales[15] = 0.38054628;
    scales[16] = 0.45254834;
    scales[17] = 0.53817371;
    scales[18] = 0.64;
    scales[19] = 0.76109255;
    for (unsigned int i = 0; i < 20; i++) {
        CHECK(filter.wavelets_[i]->scale.get() == Approx(scales[i]));
    }
}

// TEST_CASE( "Filterbank: online filtering", "[Filterbank]" )
//{
//    float samplerate(100.);
//    float frequency_min = 0.1;
//    float frequency_max = 50.;
//    float bands_per_octave = 32;
//    woma::Filterbank filter(samplerate,
//                               frequency_min,
//                               frequency_max,
//                               bands_per_octave);
//    std::vector< std::complex<double> > results(200);
//    results[0] = std::complex<double>(-0.00554768288556, -0.0130671503136);
//    results[1] = std::complex<double>(-0.00563367216558, -0.0117092224567);
//    results[2] = std::complex<double>(-0.000959295678364, -0.00829527102609);
//    results[3] = std::complex<double>(-0.010302464962, -0.0100331836467);
//    results[4] = std::complex<double>(-0.0129332895027, -0.00923987059751);
//    results[5] = std::complex<double>(-0.0121094786135, -0.00827967060155);
//    results[6] = std::complex<double>(-0.0135207140656, +3.46944695195e-18);
//    results[7] = std::complex<double>(-0.0169005185267, -0.00709453219344);
//    results[8] = std::complex<double>(-0.0200109897176, -0.00653357515678);
//    results[9] = std::complex<double>(-0.0224544713432, -1.73472347598e-18);
//    results[10] = std::complex<double>(-0.0222619233309, +0);
//    results[11] = std::complex<double>(-0.0247106135007, -0.00501659182333);
//    results[12] = std::complex<double>(-0.0289012762068, -0.00461993529875);
//    results[13] = std::complex<double>(-0.0312891903879, -0.00423683322924);
//    results[14] = std::complex<double>(-0.0333458924124, -8.32667268469e-17);
//    results[15] = std::complex<double>(-0.0370975829751, -5.55111512313e-17);
//    results[16] = std::complex<double>(-0.0410846240436, -0.00326678757839);
//    results[17] = std::complex<double>(-0.0444140985691, -0.00299589350715);
//    results[18] = std::complex<double>(-0.0483500242921, +0);
//    results[19] = std::complex<double>(-0.052365560199, -0.00251876268303);
//    results[20] = std::complex<double>(-0.0133477693373, -0.0182440368116);
//    results[21] = std::complex<double>(-0.0129328429524, -0.0157512037861);
//    results[22] = std::complex<double>(-0.00730141377366, -0.0133693505464);
//    results[23] = std::complex<double>(-0.0160496471766, -0.0098301851889);
//    results[24] = std::complex<double>(-0.017491234692, -0.00784021281586);
//    results[25] = std::complex<double>(-0.0164597009248, -0.007226480912);
//    results[26] = std::complex<double>(-0.0165363406251, +0.00195521413144);
//    results[27] = std::complex<double>(-0.0197073715799, -0.00533138516485);
//    results[28] = std::complex<double>(-0.0220996124327, -0.0046269274331);
//    results[29] = std::complex<double>(-0.023761168016, +0.00195153707612);
//    results[30] = std::complex<double>(-0.0236747280258, +0.00173614689845);
//    results[31] = std::complex<double>(-0.026142684051, -0.00347055630058);
//    results[32] = std::complex<double>(-0.0299473021489, -0.00312312070244);
//    results[33] = std::complex<double>(-0.0322131644662, -0.00285688812898);
//    results[34] = std::complex<double>(-0.0340916490696, +0.00128212005513);
//    results[35] = std::complex<double>(-0.0377197460375, +0.00118242203224);
//    results[36] = std::complex<double>(-0.0416751772078, -0.00218439332381);
//    results[37] = std::complex<double>(-0.0449527690155, -0.00200304355807);
//    results[38] = std::complex<double>(-0.0487902926146, +0.000914459719828);
//    results[39] = std::complex<double>(-0.0528054959194, -0.00168266404581);
//    results[40] = std::complex<double>(-0.00955877648537, -0.0350677285995);
//    results[41] = std::complex<double>(-0.0146821898345, -0.0321729659081);
//    results[42] = std::complex<double>(-0.00941709823441, -0.0298802180887);
//    results[43] = std::complex<double>(-0.0262086747556, -0.0167432762307);
//    results[44] = std::complex<double>(-0.0275646739051, -0.0101011665824);
//    results[45] = std::complex<double>(-0.0259712163966, -0.0090314036299);
//    results[46] = std::complex<double>(-0.0241251755551, +0.00368259506646);
//    results[47] = std::complex<double>(-0.0265780961497, -0.0034246965502);
//    results[48] = std::complex<double>(-0.0274111890995, -0.00173057220067);
//    results[49] = std::complex<double>(-0.027310043414, +0.00546736357743);
//    results[50] = std::complex<double>(-0.0272199027105, +0.00485483896791);
//    results[51] = std::complex<double>(-0.029559499429, -0.000677787557687);
//    results[52] = std::complex<double>(-0.0324702073762, -0.000278102571382);
//    results[53] = std::complex<double>(-0.0343927963452, -0.000197012515735);
//    results[54] = std::complex<double>(-0.035836409507, +0.00379188135529);
//    results[55] = std::complex<double>(-0.0391571709486, +0.00351711521562);
//    results[56] = std::complex<double>(-0.0430053824089, -4.35913179956e-05);
//    results[57] = std::complex<double>(-0.0461447877984, -3.44250709254e-05);
//    results[58] = std::complex<double>(-0.0497578905265, +0.00273551522493);
//    results[59] = std::complex<double>(-0.0537530941965, -1.86659015332e-05);
//    results[60] = std::complex<double>(0.00840762887088, -0.030758769039);
//    results[61] = std::complex<double>(0.00262540889127, -0.0419146363183);
//    results[62] = std::complex<double>(0.00565226697408, -0.0458205850395);
//    results[63] = std::complex<double>(-0.0308156300119, -0.03391331997);
//    results[64] = std::complex<double>(-0.0385360044376, -0.0212245647503);
//    results[65] = std::complex<double>(-0.037888680329, -0.0180525666272);
//    results[66] = std::complex<double>(-0.0361985651974, +0.00182205996442);
//    results[67] = std::complex<double>(-0.0378014836777, -0.00384108454841);
//    results[68] = std::complex<double>(-0.0366696231214, +0.000593468421015);
//    results[69] = std::complex<double>(-0.0339349905081, +0.0097279076501);
//    results[70] = std::complex<double>(-0.0335405590913, +0.00871996445544);
//    results[71] = std::complex<double>(-0.0354612985218, +0.00287847071119);
//    results[72] = std::complex<double>(-0.0368909560981, +0.00365080610151);
//    results[73] = std::complex<double>(-0.0381578880055, +0.00356898275781);
//    results[74] = std::complex<double>(-0.0388368473401, +0.00742870351443);
//    results[75] = std::complex<double>(-0.041607222657, +0.00694502060108);
//    results[76] = std::complex<double>(-0.0452272223473, +0.00311294017523);
//    results[77] = std::complex<double>(-0.0481069472525, +0.002880893255);
//    results[78] = std::complex<double>(-0.0513417264538, +0.00544782566947);
//    results[79] = std::complex<double>(-0.0552771696554, +0.00246011098748);
//    results[80] = std::complex<double>(-0.00574466237027, -0.0130990435275);
//    results[81] = std::complex<double>(0.0130493730378, -0.0250161405038);
//    results[82] = std::complex<double>(0.0271831826364, -0.0399175837173);
//    results[83] = std::complex<double>(-0.0198824668087, -0.0514492115877);
//    results[84] = std::complex<double>(-0.0413126184068, -0.0405834979179);
//    results[85] = std::complex<double>(-0.0458818261439, -0.0358275565725);
//    results[86] = std::complex<double>(-0.0501705492262, -0.00687151993806);
//    results[87] = std::complex<double>(-0.0521445169804, -0.0092893076873);
//    results[88] = std::complex<double>(-0.0498281003692, +0.000329065793688);
//    results[89] = std::complex<double>(-0.0441709338627, +0.0135013031113);
//    results[90] = std::complex<double>(-0.0430949621226, +0.0124239875448);
//    results[91] = std::complex<double>(-0.0442366887219, +0.00653622820895);
//    results[92] = std::complex<double>(-0.0435906774146, +0.00827954366523);
//    results[93] = std::complex<double>(-0.0438206584641, +0.00819005083369);
//    results[94] = std::complex<double>(-0.0433450984528, +0.0120431327393);
//    results[95] = std::complex<double>(-0.045267459622, +0.0113763361767);
//    results[96] = std::complex<double>(-0.0484933736432, +0.0072227955303);
//    results[97] = std::complex<double>(-0.0509569908525, +0.00670125912611);
//    results[98] = std::complex<double>(-0.0536319647285, +0.00902820956322);
//    results[99] = std::complex<double>(-0.0574472965088, +0.00573543060803);
//    results[100] = std::complex<double>(-0.0281077674066, -0.0365842954816);
//    results[101] = std::complex<double>(-0.0103594606783, -0.0138756503504);
//    results[102] = std::complex<double>(0.0254520895088, -0.0178114103739);
//    results[103] = std::complex<double>(0.0012962533788, -0.0520168213155);
//    results[104] = std::complex<double>(-0.0294703566752, -0.0585346774453);
//    results[105] = std::complex<double>(-0.0430034547653, -0.0582773846568);
//    results[106] = std::complex<double>(-0.0611539933717, -0.0236521302059);
//    results[107] = std::complex<double>(-0.066694250522, -0.0217608049603);
//    results[108] = std::complex<double>(-0.0658028027477, -0.00460264355709);
//    results[109] = std::complex<double>(-0.0580564626108, +0.0152537079078);
//    results[110] = std::complex<double>(-0.0560467459952, +0.0148403836306);
//    results[111] = std::complex<double>(-0.0561019933941, +0.00947873281063);
//    results[112] = std::complex<double>(-0.0528781193576, +0.013107850821);
//    results[113] = std::complex<double>(-0.0516585400784, +0.0133377137543);
//    results[114] = std::complex<double>(-0.049599944114, +0.0174353906382);
//    results[115] = std::complex<double>(-0.0503311231136, +0.0166892252665);
//    results[116] = std::complex<double>(-0.0529548103051, +0.0122030468583);
//    results[117] = std::complex<double>(-0.0548123518147, +0.0113719451673);
//    results[118] = std::complex<double>(-0.056719386659, +0.0134453237122);
//    results[119] = std::complex<double>(-0.0603334586373, +0.00978376798995);
//    results[120] = std::complex<double>(-0.00580868035595, -0.0600695474357);
//    results[121] = std::complex<double>(-0.0304694147274, -0.0417808837434);
//    results[122] = std::complex<double>(-0.00283154546771, -0.01554057784);
//    results[123] = std::complex<double>(0.0107136575003, -0.0314543168193);
//    results[124] = std::complex<double>(-0.00760315042212, -0.0615428158999);
//    results[125] = std::complex<double>(-0.0266497879186, -0.0760036745926);
//    results[126] = std::complex<double>(-0.063573670653, -0.0461891938656);
//    results[127] = std::complex<double>(-0.0773180760747, -0.0414850630401);
//    results[128] = std::complex<double>(-0.0824124101669, -0.015801047442);
//    results[129] = std::complex<double>(-0.074978211265, +0.0133530491645);
//    results[130] = std::complex<double>(-0.0721695889154, +0.0147197399563);
//    results[131] = std::complex<double>(-0.0710450955603, +0.0107785741861);
//    results[132] = std::complex<double>(-0.0649564109009, +0.0175314704345);
//    results[133] = std::complex<double>(-0.0618961281825, +0.0186095912242);
//    results[134] = std::complex<double>(-0.0578173517036, +0.0233554566688);
//    results[135] = std::complex<double>(-0.0569822564529, +0.0227291565869);
//    results[136] = std::complex<double>(-0.0587582249717, +0.0179497245385);
//    results[137] = std::complex<double>(-0.0597888021394, +0.0168247759211);
//    results[138] = std::complex<double>(-0.0606947065908, +0.0186593902373);
//    results[139] = std::complex<double>(-0.0640056789928, +0.0145761574957);
//    results[140] = std::complex<double>(0.00827959289952, -0.0424098219242);
//    results[141] = std::complex<double>(-0.00962395805448, -0.0696861171365);
//    results[142] = std::complex<double>(-0.0196371967026, -0.047813537092);
//    results[143] = std::complex<double>(-0.00809523644782, -0.0105596044038);
//    results[144] = std::complex<double>(0.00809279859769, -0.0440927457909);
//    results[145] = std::complex<double>(-0.00235880832351, -0.0788630752712);
//    results[146] = std::complex<double>(-0.0538416827783, -0.0683228526882);
//    results[147] = std::complex<double>(-0.0797755623527, -0.0662060177339);
//    results[148] = std::complex<double>(-0.0966070965686, -0.0338052861802);
//    results[149] = std::complex<double>(-0.0936012223908, +0.00634973798493);
//    results[150] = std::complex<double>(-0.0907836667833, +0.0108256910583);
//    results[151] = std::complex<double>(-0.0887804046758, +0.00946133845602);
//    results[152] = std::complex<double>(-0.0798914084844, +0.0208635702519);
//    results[153] = std::complex<double>(-0.0746870762294, +0.0235370026075);
//    results[154] = std::complex<double>(-0.0681806320356, +0.0295048654341);
//    results[155] = std::complex<double>(-0.0653905386282, +0.0293089565285);
//    results[156] = std::complex<double>(-0.066043297713, +0.0243376057656);
//    results[157] = std::complex<double>(-0.0659990223806, +0.0229778864302);
//    results[158] = std::complex<double>(-0.065647846196, +0.02462197696);
//    results[159] = std::complex<double>(-0.0685336295497, +0.0200780651511);
//    results[160] = std::complex<double>(-0.00975083044242, -0.0381008623638);
//    results[161] = std::complex<double>(0.0145203782854, -0.0585456269831);
//    results[162] = std::complex<double>(0.00111342928571, -0.0800864963441);
//    results[163] = std::complex<double>(-0.0428807943231, -0.0168425906674);
//    results[164] = std::complex<double>(0.00107739945059, -0.0172022585439);
//    results[165] = std::complex<double>(0.0170647065814, -0.0628850337602);
//    results[166] = std::complex<double>(-0.0329520150968, -0.0818293795776);
//    results[167] = std::complex<double>(-0.0712445254875, -0.0911856659433);
//    results[168] = std::complex<double>(-0.105007243319, -0.0576329275633);
//    results[169] = std::complex<double>(-0.111922653344, -0.00670318319065);
//    results[170] = std::complex<double>(-0.110740253617, +0.00209994984579);
//    results[171] = std::complex<double>(-0.108721636473, +0.00458605797612);
//    results[172] = std::complex<double>(-0.09758415308, +0.0223653642952);
//    results[173] = std::complex<double>(-0.0900968398921, +0.0275968249436);
//    results[174] = std::complex<double>(-0.0808305165527, +0.0355403576012);
//    results[175] = std::complex<double>(-0.0757059324639, +0.0362096436064);
//    results[176] = std::complex<double>(-0.0749398474302, +0.0312203407469);
//    results[177] = std::complex<double>(-0.0735511041686, +0.0297356247163);
//    results[178] = std::complex<double>(-0.0716671707961, +0.0312758362683);
//    results[179] = std::complex<double>(-0.0739862232154, +0.0262492870108);
//    results[180] = std::complex<double>(-0.01360384128, -0.0549245541517);
//    results[181] = std::complex<double>(0.00483191676259, -0.0416471311686);
//    results[182] = std::complex<double>(0.0333420390157, -0.0778156638102);
//    results[183] = std::complex<double>(-0.0609376294067, -0.0523648769509);
//    results[184] = std::complex<double>(-0.0301481904536, -0.00322340341262);
//    results[185] = std::complex<double>(0.0180469912095, -0.0349646247658);
//    results[186] = std::complex<double>(-0.00749334127233, -0.0798903126372);
//    results[187] = std::complex<double>(-0.0517727326271, -0.110159589166);
//    results[188] = std::complex<double>(-0.104684039933, -0.0846047711031);
//    results[189] = std::complex<double>(-0.127469172345, -0.0259459947625);
//    results[190] = std::complex<double>(-0.13046709908, -0.0121610019908);
//    results[191] = std::complex<double>(-0.129978430302, -0.0046625792297);
//    results[192] = std::complex<double>(-0.117750055445, +0.0212852188054);
//    results[193] = std::complex<double>(-0.108087250135, +0.0302275910759);
//    results[194] = std::complex<double>(-0.0958554952713, +0.0410794706434);
//    results[195] = std::complex<double>(-0.088053262058, +0.0431821014773);
//    results[196] = std::complex<double>(-0.0855649059023, +0.0384309424351);
//    results[197] = std::complex<double>(-0.0825469992079, +0.0369886094641);
//    results[198] = std::complex<double>(-0.078838692093, +0.0385548073453);
//    results[199] = std::complex<double>(-0.0804311892949, +0.0330438754805);
//    unsigned int numbands = static_cast<unsigned
//    int>(filter.wavelets_.size());
//    for (unsigned int i=0; i<10; ++i) {
//        filter.update(0.3 + i/10.);
//        for (unsigned int band=0; band<numbands; band++) {
//            CHECK(filter.result_complex[band].real() == Approx(results[i *
//            numbands + band].real()));
//            CHECK(filter.result_complex[band].imag() == Approx(results[i *
//            numbands + band].imag()));
//        }
//    }
//}
