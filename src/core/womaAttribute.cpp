/*
 * womaAttribute.cpp
 *
 * Generic Attributes
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

#include "womaAttribute.hpp"
#include <string>

template <>
void woma::checkLimits<bool>(bool const& value, bool const& limit_min,
                             bool const& limit_max) {}

template <>
void woma::checkLimits<std::size_t>(std::size_t const& value,
                                    std::size_t const& limitMin,
                                    std::size_t const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<unsigned char>(unsigned char const& value,
                                      unsigned char const& limitMin,
                                      unsigned char const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<char>(char const& value, char const& limitMin,
                             char const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<unsigned int>(unsigned int const& value,
                                     unsigned int const& limitMin,
                                     unsigned int const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<int>(int const& value, int const& limitMin,
                            int const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<long>(long const& value, long const& limitMin,
                             long const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<float>(float const& value, float const& limitMin,
                              float const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}

template <>
void woma::checkLimits<double>(double const& value, double const& limitMin,
                               double const& limitMax) {
    if (value < limitMin || value > limitMax)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limitMin) + " ; " +
                                std::to_string(limitMax) + "]");
}
