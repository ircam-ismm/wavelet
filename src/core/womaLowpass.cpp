/*
 * womaLowpass.cpp
 *
 * Chebyshev Type 1 low-pass Filter
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
#include "womaLowpass.hpp"
#include <cmath>
#include <iostream>

woma::LowpassFilter::LowpassFilter(double cutoff_, int order_,
                                   double rippleLevel_)
    : cutoff(this, cutoff_, 0., 1.),
      order(this, order_, 1),
      rippleLevel(this, rippleLevel_) {
    init();
}

woma::LowpassFilter::LowpassFilter(LowpassFilter const& src) {
    b = src.b;
    a = src.a;
}

woma::LowpassFilter& woma::LowpassFilter::operator=(LowpassFilter const& src) {
    if (this != &src) {
        b = src.b;
        a = src.a;
    }
    return *this;
}

woma::LowpassFilter::~LowpassFilter() {}

void woma::LowpassFilter::onAttributeChange(AttributeBase* attr_pointer) {
    init();
    attr_pointer->changed = false;
}

void woma::LowpassFilter::init() {
    cheby1(order.get(), rippleLevel.get(), cutoff.get());
    z.assign(order.get(), 0.);
}

void woma::LowpassFilter::cheby1(int filter_order, double ripple_db,
                                 double cutoff) {
    if (cutoff <= 0. || cutoff > 1.)
        throw std::invalid_argument("Cutoff must be between 0 and 1");
    std::vector<std::complex<double> > p;
    double k = cheby1ap(filter_order, ripple_db, p);

    double warped_cutoff = 4.0 * std::tan(M_PI * cutoff / 2.0);

    // transform to lowpass
    k = _zpklp2lp(p, k, warped_cutoff);

    // Find discrete equivalent if necessary
    std::vector<std::complex<double> > z;
    k = _zpkbilinear(z, p, k);

    // Transform to proper out type (pole-zero, state-space, numer-denom)
    zpk2tf(z, p, k);
}

double woma::LowpassFilter::cheby1ap(int filter_order, double ripple_db,
                                     std::vector<std::complex<double> >& p) {
    // Ripple factor (epsilon)
    double eps = std::sqrt(std::pow(10., 0.1 * ripple_db) - 1.0);
    double mu = 1.0 / filter_order * asinh(1 / eps);

    // Arrange poles in an ellipse on the left half of the S-plane
    p.assign(filter_order, std::complex<double>(0., 0.0));
    std::complex<double> k_c = 1.;
    for (int i = 0; i < filter_order; i++) {
        double theta = M_PI * double(-filter_order + 1 + 2 * i) /
                       (2 * double(filter_order));
        p[i] = -std::sinh(std::complex<double>(mu, theta));
        k_c *= -p[i];
    }

    double k = k_c.real();

    if (filter_order % 2 == 0) {
        k = k / std::sqrt((1 + eps * eps));
    }

    return k;
}

double woma::LowpassFilter::_zpklp2lp(std::vector<std::complex<double> >& p,
                                      double k, double wo) {
    for (int i = 0; i < p.size(); i++) {
        p[i] *= wo;
    }
    double degree = static_cast<double>(p.size());

    // Each shifted pole decreases gain by wo, each shifted zero increases it.
    // Cancel out the net change to keep overall gain the same
    return k * std::pow(wo, degree);
}

double woma::LowpassFilter::_zpkbilinear(std::vector<std::complex<double> >& z,
                                         std::vector<std::complex<double> >& p,
                                         double k) {
    double fs2 = 4.0;

    // Any zeros that were at infinity get moved to the Nyquist frequency
    std::complex<double> factor_denum = 1.;
    for (int i = 0; i < p.size(); i++) {
        factor_denum *= fs2 - p[i];
    }
    factor_denum = 1. / factor_denum;

    // Bilinear transform the poles and zeros
    z.assign(p.size(), -1.);
    for (int i = 0; i < p.size(); i++) {
        p[i] = (fs2 + p[i]) / (fs2 - p[i]);
    }

    return k * factor_denum.real();
}

void woma::LowpassFilter::convolve(std::vector<std::complex<double> > x,
                                   std::vector<std::complex<double> > y,
                                   std::vector<std::complex<double> >& result) {
    if (y.size() > x.size()) {
        x.swap(y);
    }

    result.assign(x.size() + y.size() - 1, 0.);
    for (std::size_t i = 0; i < result.size(); i++) {
        result[i] = 0.;
        std::size_t kmin, kmax, k;

        kmin = (i >= y.size() - 1) ? i - (y.size() - 1) : 0;
        kmax = (i < x.size() - 1) ? i : x.size() - 1;

        for (k = kmin; k <= kmax; k++) {
            result[i] += x[k] * y[i - k];
        }
    }
}

void woma::LowpassFilter::poly(
    std::vector<std::complex<double> > sequence_of_zeros,
    std::vector<std::complex<double> >& result) {
    result.assign(1, 1.);
    std::vector<std::complex<double> > result_tmp;
    std::vector<std::complex<double> > y(2, 1.);
    for (int i = 0; i < sequence_of_zeros.size(); i++) {
        y[1] = -sequence_of_zeros[i];
        convolve(result, y, result_tmp);
        result = result_tmp;
    }
}

void woma::LowpassFilter::zpk2tf(std::vector<std::complex<double> >& z,
                                 std::vector<std::complex<double> >& p,
                                 double k) {
    std::vector<std::complex<double> > b_tmp;
    std::vector<std::complex<double> > a_tmp;
    poly(z, b_tmp);
    poly(p, a_tmp);
    b.resize(b_tmp.size());
    a.resize(a_tmp.size());
    for (int i = 0; i < b.size(); i++) {
        b[i] = k * b_tmp[i].real();
    }
    for (int i = 0; i < a.size(); i++) {
        a[i] = a_tmp[i].real();
    }
}

double woma::LowpassFilter::filter(double value) {
    double filtered_value;
    filtered_value = b[0] * value + z[0];
    for (unsigned int i = 0; i < order.get() - 1; i++) {
        z[i] = b[i + 1] * value + z[i + 1] - a[i + 1] * filtered_value;
    }
    z[order.get() - 1] =
        b[order.get()] * value - a[order.get()] * filtered_value;
    return filtered_value;
}
