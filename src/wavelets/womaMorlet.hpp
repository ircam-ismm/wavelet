/*
 * womaMorlet.h
 *
 * Complex Morlet Wavelet
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

#ifndef womaMorlet_h
#define womaMorlet_h

#include "../core/womaWavelet.hpp"

namespace woma {
/**
 * @class MorletWavelet
 * @brief Complex Morlet Wavelet
 *
 * @details the Morlet wavelet is a complex plane wave windowed by a gaussian
 * envelope.@n
 * see Wikipedia: http://en.wikipedia.org/wiki/Morlet_wavelet
 */
class MorletWavelet : public Wavelet {
  public:
    /**
     * @brief Default value of the Carrier frequency
     */
    static const float DEFAULT_OMEGA0() { return 5.; }

#pragma mark -
#pragma mark === Public Interface ===
#pragma mark > Constructors
    /** @name Constructors */
    ///@{

    /**
     * @brief Constructor
     * @param samplerate wavelet sampling rate
     */
    MorletWavelet(float samplerate);

    /**
     * @brief Copy Constructor
     * @param src source Wavelet
     */
    MorletWavelet(MorletWavelet const& src);

    /**
     * @brief Assignment operator
     * @param src source Wavelet
     */
    MorletWavelet& operator=(MorletWavelet const& src);

    /**
     * @brief Constructor
     */
    virtual ~MorletWavelet();

///@}

#pragma mark > Utilities
    /** @name Utilities */
    ///@{

    /**
     * @brief get info on the current configuration
     * @return information string
     */
    virtual std::string info() const;

///@}

#pragma mark > Core Wavelet Methods
    /** @name Core Wavelet Methods */
    ///@{

    /**
     * @brief convert scale to fourier frequency (Hz)
     * @return equivalent fourier frequency (Hz)
     */
    virtual double scale2frequency(double scale) const;

    /**
     * @brief convert fourier frequency (Hz) to scale
     * @return equivalent scale
     */
    virtual double frequency2scale(double frequency) const;

    /**
     * @brief compute the critical time of the wavelet
     * @return e-folding time at the current scale in seconds
     */
    virtual double eFoldingTime() const;

    /**
     * @brief rescaled wavelet function
     * @param arg wavelet argument [(n' - n) * delta_t / scale]
     * @return value of the rescaled wavelet for the given argument
     */
    virtual std::complex<double> phi(double arg) const;

    /**
     * @brief rescaled wavelet function in the spectral domain
     * @param s_omega spectral argument (scale * omega)
     * @return value of the rescaled wavelet spectrum for the given argument
     */
    virtual std::complex<double> phi_spectral(double s_omega) const;

///@}

#pragma mark -
#pragma mark === Public Attributes ===
    /**
     * @brief Carrier Frequency
     */
    Attribute<float> omega0;

///@cond DEVDOC
#ifndef WAVELET_TESTING
  protected:
#endif
#pragma mark -
#pragma mark === Protected Methods ===
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
     * @return attribute value
     * @throws out_of_range if the attribute does not exist
     */
    virtual boost::any getAttribute_internal(std::string attr_name) const;

#pragma mark -
#pragma mark === Protected Attributes ===

    ///@endcond
};
}

#endif
