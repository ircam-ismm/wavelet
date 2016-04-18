/*
 * womaWavelet.h
 *
 * Wavelet base class (Abstract)
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

#ifndef womaWavelet_h
#define womaWavelet_h

#include "womaAttribute.hpp"
#include <cmath>
#include <complex>
#include <iostream>
#include <string>

namespace woma {
/**
 * @class Wavelet
 * @brief Abstract class containing common wavelet definitions and methods
 */
class Wavelet : public AttributeHandler {
  public:
    friend class Filterbank;

    ///@cond DEVDOC

    /**
     * @brief Computation mode of the Wavelet
     */
    enum WaveletDomain : unsigned char {
        /**
         * @brief recursive computation (online estimation)
         */
        RECURSIVE = 0,

        /**
         * @brief spectral computation (uses FFT for offline estimation)
         */
        SPECTRAL = 1
    };

    /**
     * @brief Default ratio of the delay to the wavelet's critical time
     */
    static const float DEFAULT_DELAY() { return 1.5; }

    /**
     * @brief Default ratio of the padding to the wavelet's critical time
     */
    static const float DEFAULT_PADDING() { return 1.; }

///@endcond

#pragma mark -
#pragma mark === Public Interface ===
#pragma mark > Constructors
    /** @name Constructors */
    ///@{

    /**
     * @brief Constructor
     * @param samplerate wavelet sampling rate
     */
    Wavelet(float samplerate);

    /**
     * @brief Copy Constructor
     * @param src source Wavelet
     */
    Wavelet(Wavelet const& src);

    /**
     * @brief Assignment operator
     * @param src source Wavelet
     */
    Wavelet& operator=(Wavelet const& src);

    /**
     * @brief Destructor
     */
    virtual ~Wavelet() {}

///@}

#pragma mark > Utilities
    /** @name Utilities */
    ///@{

    /**
     * @brief set attribute value by name
     * @param attr_name attribute name
     * @param attr_value attribute value
     */
    template <typename T>
    void setAttribute(std::string attr_name, T attr_value) {
        setAttribute_internal(attr_name, boost::any(attr_value));
    }

    /**
     * @brief get attribute value by name
     * @param attr_name attribute name
     * @return attribute value
     */
    template <typename T>
    T getAttribute(std::string attr_name) const {
        return boost::any_cast<T>(getAttribute_internal(attr_name));
    }

    /**
     * @brief set the window size to default value with respect to the delay
     */
    void setDefaultWindowsize();

    /**
     * @brief get info on the current configuration
     * @return information string
     */
    virtual std::string info() const;

#ifdef SWIGPYTHON
    /**
     * @brief "print" method for python => returns the results of info() method
     * @return Python info string
     * @warning only defined if SWIGPYTHON is defined
     */
    std::string __str__() { return info(); }
#endif

///@}

#pragma mark > Core Wavelet Methods (pure virtual)
    /** @name Core Wavelet Methods (pure virtual) */
    ///@{

    /**
     * @brief convert scale to fourier frequency (Hz)
     * @return equivalent fourier frequency (Hz)
     */
    virtual double scale2frequency(double scale) const = 0;

    /**
     * @brief convert fourier frequency (Hz) to scale
     * @return equivalent scale
     */
    virtual double frequency2scale(double frequency) const = 0;

    /**
     * @brief compute the critical time of the wavelet
     * @return e-folding time at the current scale in seconds
     */
    virtual double eFoldingTime() const = 0;

    /**
     * @brief rescaled wavelet function
     * @param arg wavelet argument [(n' - n) * delta_t / scale]
     * @return value of the rescaled wavelet for the given argument
     */
    virtual std::complex<double> phi(double arg) const = 0;

    /**
     * @brief rescaled wavelet function in the spectral domain
     * @param s_omega spectral argument (scale * omega)
     * @return value of the rescaled wavelet spectrum for the given argument
     */
    virtual std::complex<double> phi_spectral(double s_omega) const = 0;

///@}

#pragma mark -
#pragma mark === Public Attributes ===
    /**
     * @brief Sampling rate of the data
     */
    Attribute<float> samplerate;

    /**
     * @brief current scale
     */
    Attribute<double> scale;

    /**
     * @brief size of the computation window
     */
    Attribute<std::size_t> window_size;

    /**
     * @brief Wavelet computation mode
     */
    Attribute<WaveletDomain> mode;

    /**
     * @brief Delay relative to critical wavelet time
     */
    Attribute<float> delay;

    /**
     * @brief Padding relative to critical wavelet time
     */
    Attribute<float> padding;

    /**
     * @brief wavelet on the window at the current scale
     */
    std::vector<std::complex<double> > values;

///@cond DEVDOC
#ifndef WAVELET_TESTING
  protected:
#endif
#pragma mark -
#pragma mark === Protected Methods ===
    /**
     * @brief Copy two wavelets
     * @param dst destination object
     * @param src source object
     */
    static void _copy(Wavelet* dst, Wavelet const& src);

    /**
     * @brief Allocate and initialize the wavelet
     * @details method to be overloaded in wavelet instances
     */
    virtual void init();

    /**
     * @brief initialize the filter when an attribute changes
     * @param attr_pointer pointer to the changed attribute
     */
    virtual void onAttributeChange(AttributeBase* attr_pointer);

    /**
     * @brief set attribute value by name
     * @param attr_name attribute name
     * @param attr_value attribute value
     * @throws out_of_range if the attribute does not exist
     */
    virtual void setAttribute_internal(std::string attr_name,
                                       boost::any const& attr_value);

    /**
     * @brief get attribute value by name
     * @param attr_name attribute name
     * @throws out_of_range if the attribute does not exist
     * @return attribute value
     */
    virtual boost::any getAttribute_internal(std::string attr_name) const;

#pragma mark -
#pragma mark === Protected Attributes ===
    /**
     * @brief conjugate value for pre-padding of the wavelet
     */
    std::complex<double> prepad_value_;

    /**
     * @brief conjugate value for post-padding of the wavelet
     */
    std::complex<double> postpad_value_;

    ///@endcond
};

///@cond DEVDOC

template <>
void checkLimits<Wavelet::WaveletDomain>(
    Wavelet::WaveletDomain const& value,
    Wavelet::WaveletDomain const& limit_min,
    Wavelet::WaveletDomain const& limit_max);

template <>
Wavelet::WaveletDomain Attribute<Wavelet::WaveletDomain>::default_limit_max();

///@endcond
}

#endif
