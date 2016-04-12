/*
 * catch_utilities.h
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

#ifndef xmm_lib_catch_utilities_h
#define xmm_lib_catch_utilities_h

#include "catch.hpp"

/**
 * @brief Check for vector approximate equality
 */
template <typename T>
void CHECK_VECTOR_APPROX(std::vector<T> const& a, std::vector<T> const& b,
                         double epsilon = -1.) {
    REQUIRE(a.size() == b.size());
    std::string errormsg = "CHECK_VECTOR_APPROX:\n{";
    std::string errormsg2 = "}\n==\n{";
    std::ostringstream convert;
    std::ostringstream convert2;
    for (std::size_t i = 0; i < a.size(); ++i) {
        convert << a[i] << " ";
        convert2 << b[i] << " ";
    }
    INFO(errormsg + convert.str() + errormsg2 + convert2.str());
    if (epsilon > 0.0) {
        for (std::size_t i = 0; i < a.size(); ++i) {
            REQUIRE(a[i] == Approx(b[i]).epsilon(epsilon));
        }
    } else {
        for (std::size_t i = 0; i < a.size(); ++i) {
            REQUIRE(a[i] == Approx(b[i]));
        }
    }
};

#endif
