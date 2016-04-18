/*
 * womaPaul.cpp
 *
 * Paul Wavelet
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

#include "womaPaul.hpp"
#include <sstream>

woma::PaulWavelet::PaulWavelet(float samplerate)
    : Wavelet(samplerate), order(this, DEFAULT_ORDER(), 1) {
    init();
}

woma::PaulWavelet::PaulWavelet(PaulWavelet const& src) : Wavelet(src) {
    order = src.order;
    order.set_parent(this);
}

woma::PaulWavelet& woma::PaulWavelet::operator=(PaulWavelet const& src) {
    if (this != &src) {
        Wavelet::_copy(this, src);
        order = src.order;
        order.set_parent(this);
    }
    return *this;
}

woma::PaulWavelet::~PaulWavelet() {}

unsigned int woma::PaulWavelet::factorial(unsigned int n) const {
    unsigned int result(1);
    for (unsigned int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

std::complex<double> woma::PaulWavelet::phi(double arg) const {
    std::complex<double> numer =
        std::pow(2. * std::complex<double>(0, 1), order.get()) *
        double(factorial(order.get()));
    std::complex<double> denum = std::sqrt(M_PI * factorial(2 * order.get()));
    std::complex<double> second_arg =
        std::pow(std::complex<double>(1, -arg), -int(order.get() + 1));
    return (numer / denum) * second_arg *
           sqrt(1. / double(this->scale.get() *
                            this->samplerate.get()));  // normalization
}

std::complex<double> woma::PaulWavelet::phi_spectral(double s_omega) const {
    if (s_omega > 0) {
        double numer = std::pow(2., order.get());
        double denum = std::sqrt(order.get() * factorial(2 * order.get() - 1));
        double second_arg = std::pow(s_omega, order.get());
        double third_arg = std::exp(-s_omega);
        return (numer / denum) * second_arg * third_arg;
    } else {
        return 0.;
    }
}

double woma::PaulWavelet::scale2frequency(double scale) const {
    return (2 * order.get() + 1) / (4. * M_PI * scale);
}

double woma::PaulWavelet::frequency2scale(double frequency) const {
    return (2 * order.get() + 1) / (4. * M_PI * frequency);
}

std::string woma::PaulWavelet::info() const {
    std::stringstream infostrstream;
    infostrstream << Wavelet::info();
    infostrstream << "\tType: Morlet\n";
    infostrstream << "\tOrder: " << this->order.get() << "\n";
    return infostrstream.str();
}

double woma::PaulWavelet::eFoldingTime() const {
    return this->scale.get() / M_SQRT2;
}

void woma::PaulWavelet::setAttribute_internal(std::string attr_name,
                                              boost::any const& attr_value) {
    try {
        Wavelet::setAttribute_internal(attr_name, attr_value);
    } catch (std::out_of_range& e) {
        if (attr_name == "order") {
            order.set(boost::any_cast<float>(attr_value));
        } else {
            throw std::out_of_range(e);
        }
    }
}

boost::any woma::PaulWavelet::getAttribute_internal(
    std::string attr_name) const {
    try {
        return Wavelet::getAttribute_internal(attr_name);
    } catch (std::out_of_range& e) {
        if (attr_name == "order") {
            return boost::any(order.get());
        } else {
            throw std::out_of_range(e);
        }
    }
}
