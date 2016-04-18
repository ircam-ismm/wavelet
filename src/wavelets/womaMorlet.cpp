/*
 * womaMorlet.cpp
 *
 * Morlet Wavelet
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

#include "womaMorlet.hpp"
#include <sstream>

woma::MorletWavelet::MorletWavelet(float samplerate)
    : Wavelet(samplerate), omega0(this, DEFAULT_OMEGA0(), 0.) {
    init();
}

woma::MorletWavelet::MorletWavelet(MorletWavelet const& src) : Wavelet(src) {
    omega0 = src.omega0;
    omega0.set_parent(this);
}

woma::MorletWavelet& woma::MorletWavelet::operator=(MorletWavelet const& src) {
    if (this != &src) {
        Wavelet::_copy(this, src);
        omega0 = src.omega0;
        omega0.set_parent(this);
    }
    return *this;
}

woma::MorletWavelet::~MorletWavelet() {}

std::complex<double> woma::MorletWavelet::phi(double arg) const {
    //// Next line: Complete Morlet Wavelet (correction term for low omega0)
    return std::exp(-0.5 * arg * arg) *
           (std::exp(std::complex<double>(0., 1. * this->omega0.get() * arg)) -
            std::exp(-std::complex<double>(
                0.5 * this->omega0.get() * this->omega0.get(), 0.))) *
           sqrt(1. / double(this->scale.get() * this->samplerate.get())) *
           pow(M_PI, -0.25);

    return std::exp(-0.5 * arg * arg) *
           std::exp(std::complex<double>(0., 1. * this->omega0.get() * arg)) *
           sqrt(1. / double(this->scale.get() * this->samplerate.get())) *
           pow(M_PI, -0.25);
}

std::complex<double> woma::MorletWavelet::phi_spectral(double s_omega) const {
    if (s_omega > 0) {
        return pow(M_PI, -0.25) * exp(-0.5 * (s_omega - this->omega0.get()) *
                                      (s_omega - this->omega0.get())) *
               sqrt(2. * M_PI * this->scale.get() * this->samplerate.get());
    } else {
        return 0.;
    }
}

double woma::MorletWavelet::scale2frequency(double scale) const {
    return (this->omega0.get() +
            sqrt(2. + this->omega0.get() * this->omega0.get())) /
           (4. * M_PI * scale);
}

double woma::MorletWavelet::frequency2scale(double frequency) const {
    return (this->omega0.get() +
            sqrt(2 + this->omega0.get() * this->omega0.get())) /
           (4. * M_PI * frequency);
}

std::string woma::MorletWavelet::info() const {
    std::stringstream infostrstream;
    infostrstream << Wavelet::info();
    infostrstream << "\tType: Morlet\n";
    infostrstream << "\tOmega0 (carrier frequency): " << this->omega0.get()
                  << "\n";
    return infostrstream.str();
}

double woma::MorletWavelet::eFoldingTime() const {
    return M_SQRT2 * this->scale.get();
}

void woma::MorletWavelet::setAttribute_internal(std::string attr_name,
                                                boost::any const& attr_value) {
    try {
        Wavelet::setAttribute_internal(attr_name, attr_value);
    } catch (std::out_of_range& e) {
        if (attr_name == "omega0") {
            omega0.set(boost::any_cast<float>(attr_value));
        } else {
            throw std::out_of_range(e);
        }
    }
}

boost::any woma::MorletWavelet::getAttribute_internal(
    std::string attr_name) const {
    try {
        return Wavelet::getAttribute_internal(attr_name);
    } catch (std::out_of_range& e) {
        if (attr_name == "omega0") {
            return boost::any(omega0.get());
        } else {
            throw std::out_of_range(e);
        }
    }
}
