/*
 * womaLowpass.h
 *
 * Digital Chebyshev Type 1 low-pass Filter (adapted from NUMPY library)
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

#ifndef __wavelet__lowpass__
#define __wavelet__lowpass__

#include "womaAttribute.hpp"
#include <complex>
#include <vector>

namespace woma {
/**
 * @class LowpassFilter
 * @brief Chebyshev Type 1 low-pass Filter
 */
class LowpassFilter : public AttributeHandler {
  public:
#pragma mark -
#pragma mark === Public Interface ===
#pragma mark > Constructors & Destructors
    /** @name Constructors & Destructors */
    ///@{

    /**
     * @brief Constructor
     * @param cutoff Normalized cutoff frequency
     * @param order Filter order
     * @param rippleLevel Ripple level (dB)
     */
    LowpassFilter(double cutoff = 1., int order = 4, double rippleLevel = 0.05);

    /**
     * @brief Copy Constructor
     * @param src Source filter
     */
    LowpassFilter(LowpassFilter const& src);

    /**
     * @brief Assignment Operator
     * @param src Source filter
     */
    LowpassFilter& operator=(LowpassFilter const& src);

    /**
     * @brief Destructor
     */
    virtual ~LowpassFilter();

///@}

#pragma mark > Filtering
    /** @name Filtering */
    ///@{

    /**
     * @brief Filter an incoming value
     * @param value incoming signal value
     * @return filtered value (low-pass)
     */
    double filter(double value);

/*&}*/

#pragma mark -
#pragma mark === Public Attributes ===
    /**
     * @brief Normalized Cutoff Frequency
     */
    Attribute<double> cutoff;

    /**
     * @brief Filter order
     */
    Attribute<int> order;

    /**
     * @brief Ripple level (dB)
     */
    Attribute<double> rippleLevel;

  protected:
#pragma mark -
#pragma mark === Protected Methods ===
    /**
     * @brief initialize the filter parameters
     */
    void init();

    /**
     * @brief initialize the filter when an attribute changes
     * @param attr_pointer pointer to the changed attribute
     */
    void onAttributeChange(AttributeBase* attr_pointer);

    /**
     * @brief estimate filter parameters as Chebishev Type 1 filter
     * @param filter_order filter order
     * @param ripple_db Ripple level (dB)
     * @param cutoff Normalized Cutoff Frequency
     */
    void cheby1(int filter_order, double ripple_db, double cutoff);

    /**
     * @brief Estimate Chebishev Type 1 analog filter prototype
     * @param filter_order filter order
     * @param ripple_db Ripple level (dB)
     * @param p Poles of the analog IIR filter transfer function.
     * @return System gain of the analog IIR filter transfer function.
     */
    double cheby1ap(int filter_order, double ripple_db,
                    std::vector<std::complex<double> >& p);

    /**
     * @brief Transform a lowpass filter prototype to a different frequency.
     * @param p Poles of the analog IIR filter transfer function.
     * @param k System gain of the analog IIR filter transfer function.
     * @param wo Normalized Cutoff Frequency
     * @return System gain of the analog IIR filter transfer function.
     */
    double _zpklp2lp(std::vector<std::complex<double> >& p, double k,
                     double wo);

    /**
     * @brief Estimates digital filter from an analog one using a bilinear
     * transform.
     * @param z Zeros of the analog IIR filter transfer function.
     * @param p Poles of the analog IIR filter transfer function.
     * @param k System gain of the analog IIR filter transfer function.
     * @return System gain of the analog IIR filter transfer function.
     */
    double _zpkbilinear(std::vector<std::complex<double> >& z,
                        std::vector<std::complex<double> >& p, double k);

    /**
     * @brief polynomial transfer function representation from zeros and poles
     * @param z Zeros of the analog IIR filter transfer function.
     * @param p Poles of the analog IIR filter transfer function.
     * @param k System gain of the analog IIR filter transfer function.
     */
    void zpk2tf(std::vector<std::complex<double> >& z,
                std::vector<std::complex<double> >& p, double k);

    /**
     * @brief Discrete convolution
     * @param x input vector 1
     * @param y input vector 2
     * @param result convolution of x and y
     */
    void convolve(std::vector<std::complex<double> > x,
                  std::vector<std::complex<double> > y,
                  std::vector<std::complex<double> >& result);

    /**
     * @brief Estimate the polynomial from a sequence of zeros
     * @param sequence_of_zeros sequence of zeros
     * @param result resulting polynomial
     */
    void poly(std::vector<std::complex<double> > sequence_of_zeros,
              std::vector<std::complex<double> >& result);

#pragma mark -
#pragma mark === Protected Attributes ===
    /**
     * @brief Numerator of the linear filter.
     */
    std::vector<double> b;

    /**
     * @brief Denumerator of the linear filter.
     */
    std::vector<double> a;

    /**
     * @brief Intermediate computation values of the filter (filter memory)
     */
    std::vector<double> z;
};
}

#endif
