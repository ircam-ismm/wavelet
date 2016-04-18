/*
 * womaFilterbank.h
 *
 * Wavelet Filter Bank
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

#ifndef womaFilterbank_h
#define womaFilterbank_h

#include "../wavelets/womaMorlet.hpp"
#include "../wavelets/womaPaul.hpp"
#include "womaLowpass.hpp"
#include "womaWavelet.hpp"
#include <boost/circular_buffer.hpp>
#include <map>
#ifdef USE_ARMA
#include <armadillo>
#endif

namespace woma {
/**
 * @brief Implemented wavelet families
 */
enum class Family {
    /**
     * @brief Morlet Wavelet
     * @see MorletWavelet
     */
    Morlet = 0,

    /**
     * @brief Paul Wavelet
     * @see PaulWavelet
     */
    Paul = 1
};

/**
 * @brief Default Wavelet family
 */
const Family DEFAULT_FAMILY = Family::Morlet;

/**
 * @class Filterbank
 * @brief Minimal-delay Wavelet Filterbank
 * @details Online scalogram estimation with minimal delay per scale (per
 * frequency band).
 * Several optimization modes are possible for estimating the online scalogram,
 * using wavelet decimation,
 * and optionally signal decimation. If using Armadillo, "process" methods can
 * be used to compute the scalogram
 * offline.
 */
class Filterbank : public AttributeHandler {
  public:
    /**
     * @brief Optimisation level of the filterbank implementation
     */
    enum class Optimisation {
        /**
         * @brief No optimisation (no wavelet downsampling)
         */
        None,

        /**
         * @brief Standard Optimisation (Wavelet Downsampling with frame-based
         * calculations)
         */
        Standard1,

        /**
         * @brief Standard Optimisation (Wavelet Downsampling with frame-based
         * calculations)
         */
        Standard2,

        /**
         * @brief Agressive Optimisation (Wavelet Downsampling with Signal
         * Downsampling)
         */
        Aggressive1,

        /**
         * @brief Agressive Optimisation (Wavelet Downsampling with Signal
         * Downsampling)
         */
        Aggressive2
    };

#pragma mark -
#pragma mark === Public Interface ===
#pragma mark > Constructors
    /** @name Constructors */
    ///@{

    /**
     * @brief Constructor
     * @param samplerate sampling rate
     * @param frequency_min minimum frequency of the filterbank
     * @param frequency_max maximum frequency of the filterbank
     * @param bands_per_octave number of bands per octave
     */
    Filterbank(float samplerate, float frequency_min, float frequency_max,
               float bands_per_octave);

    /**
     * @brief Copy Constructor
     * @param src source Filterbank
     */
    Filterbank(Filterbank const& src);

    /**
     * @brief Assignment operator
     * @param src source Filterbank
     */
    Filterbank& operator=(Filterbank const& src);

    /**
     * @brief Destructor
     */
    virtual ~Filterbank();

///@}

#pragma mark > Accessors
    /** @name Accessors */
    ///@{

    /**
     * @brief set attribute value by name
     * @param attr_name attribute name
     * @param attr_value attribute value
     * @details Possible attributes are
     * @throws runtime_error if the attribute does not exist for the current
     * wavelet or is not shared among filters,
     * or if the type does not match the attribute's internal type
     *
     * ### General attributes:
     *
     * Attribute name | Attribute type | Description | Value Range
     * ------------- | ------------- | ---------- | ----------
     * frequency_min | float | Minimum Frequency of the Filterbank (Hz) | ]0.,
     * samplerate/2.]
     * frequency_max | float | Maximum Frequency of the Filterbank (Hz) | ]0.,
     * samplerate/2.]
     * bands_per_octave | float | Number of bands per octave of the Filterbank |
     * > 1.
     * optimisation | Optimisation | Optimisation mode the filterbank
     * implementation | {NONE, STANDARD, AGRESSIVE}
     * family | Family | Wavelet Family | {MORLET, PAUL}
     * samplerate | float |  Sampling rate of the data | ]0.
     * delay | float |  Delay relative to critical wavelet time | > 0.
     * padding | float |  Padding relative to critical wavelet time | >=0.
     *
     * === Wavelet-specific attributes:
     *
     * Wavelet | Attribute name | Attribute type | Description | Value Range
     * ------- | ------------- | ------------- | ---------- | ----------
     * Morlet | omega0 | float | Carrier Frequency | > 0.
     * Paul | order | unsigned int | Order of the Paul Wavelet | > 0
     */
    template <typename T>
    void setAttribute(std::string attr_name, T attr_value) {
        try {
            setAttribute_internal(attr_name, boost::any(attr_value));
        } catch (const boost::bad_any_cast&) {
            throw std::runtime_error(
                "Argument value type does not match Attribute type");
        }
    }

    /**
     * @brief get attribute value by name
     * @param attr_name attribute name
     * @return attribute value
     * @details Possible attributes are
     * @throws runtime_error if the attribute does not exist for the current
     * wavelet or is not shared among filters,
     * or if the type does not match the attribute's internal type
     *
     * ### General attributes:
     *
     * Attribute name | Attribute type | Description
     * ------------- | ------------- | ----------
     * frequency_min | float | Minimum Frequency of the Filterbank (Hz)
     * frequency_max | float | Maximum Frequency of the Filterbank (Hz)
     * bands_per_octave | float | Number of bands per octave of the Filterbank
     * optimisation | Optimisation | Optimisation mode the filterbank
     * implementation
     * family | Family | Wavelet Family
     * samplerate | float |  Sampling rate of the data
     * delay | float |  Delay relative to critical wavelet time
     * padding | float |  Padding relative to critical wavelet time
     *
     * === Wavelet-specific attributes:
     *
     * Wavelet | Attribute name | Attribute type | Description
     * ------- | ------------- | ------------- | ----------
     * Morlet | omega0 | float | Carrier Frequency
     * Paul | order | unsigned int | Order of the Paul Wavelet
     */
    template <typename T>
    T getAttribute(std::string attr_name) const {
        try {
            return boost::any_cast<T>(getAttribute_internal(attr_name));
        } catch (const boost::bad_any_cast&) {
            throw std::runtime_error(
                "Return value type does not match Attribute type");
        }
    }

///@}

#pragma mark > Online Estimation
    /** @name Online Estimation */
    ///@{

    /**
     * @brief update the filter with an incoming value
     */
    void update(float value);

    /**
     * @brief clear the current data buffer
     */
    void reset();

///@}

#pragma mark > Offline Estimation
#ifdef USE_ARMA
    /** @name Offline Estimation */
    ///@{

    /**
     * @brief offline computation of the scalogram (uses FFT)
     * @param values array of values (complete signal : length T)
     * @return complex scalogram (C-like array with size: T * number of bands)
     * @warning this method is only accessible if compiled with armadillo
     * (USE_ARMA preprocessor macro defined)
     */
    arma::cx_mat process(std::vector<double> values);

    /**
     * @brief batch computation of the scalogram using online estimation
     * @param values array of values (complete signal : length T)
     * @return complex scalogram (C-like array with size: T * number of bands)
     * @warning this method is only accessible if compiled with armadillo
     * (USE_ARMA preprocessor macro defined)
     */
    arma::cx_mat process_online(std::vector<double> values);

///@}
#endif

#pragma mark > Utilities
    /** @name Utilities */
    ///@{

    /**
     * @brief get number of bands
     * @return number of wavelet filter bands
     */
    std::size_t size() const;

    /**
     * @brief get info on the current configuration
     * @return information string
     */
    std::string info() const;

    /**
     * @brief get the delays in sample for each filter
     * @return vector of delays in samples
     */
    std::vector<int> delaysInSamples() const;

#ifdef SWIGPYTHON
    /**
     * @brief "print" method for python => returns the results of write method
     * @return Python info string
     * @warning only defined if SWIGPYTHON is defined
     */
    std::string __str__() { return info(); }
#endif

///@}

#pragma mark -
#pragma mark === Public Attributes ===
    /**
     * @brief Minimum Frequency of the Filterbank (Hz)
     */
    Attribute<float> frequency_min;

    /**
     * @brief Maximum Frequency of the Filterbank (Hz)
     */
    Attribute<float> frequency_max;

    /**
     * @brief Number of bands per octave of the Filterbank
     */
    Attribute<float> bands_per_octave;

    /**
     * @brief Optimisation mode the filterbank implementation
     */
    Attribute<Optimisation> optimisation;

    /**
     * @brief Wavelet Family
     */
    Attribute<Family> family;

    /**
     * @brief rescale power frames
     */
    Attribute<bool> rescale;

    /**
     * @brief Scales of each band in the filterbank
     */
    std::vector<double> scales;

    /**
     * @brief Equivalent Fourier frequencies of each band in the filterbank
     */
    std::vector<double> frequencies;

    /**
     * @brief Downsampling factor for each band
     */
    std::vector<int> downsampling_factors;

    /**
     * @brief Results of the filtering process (scalogram slice)
     */
    std::vector<std::complex<double>> result_complex;

    /**
     * @brief Resulting Power of the filtering process (power scalogram slice)
     */
    std::vector<double> result_power;

///@cond DEVDOC
#ifndef WAVELET_TESTING
  protected:
#endif
#pragma mark -
#pragma mark === Protected Methods ===
    /** @name Protected Methods */
    ///@{

    /**
     * @brief Allocate and initialize the filter
     */
    void init();

    /**
     * @brief initialize the filter when an attribute changes
     * @param attr_pointer pointer to the changed attribute
     */
    virtual void onAttributeChange(AttributeBase* attr_pointer);

    /**
     * @brief set attribute value by name
     * @param attr_name attribute name
     * @param attr_value attribute value
     * @throws runtime_error if the attribute does not exist for the current
     * wavelet or is not shared among filters,
     * or if the type does not match the attribute's internal type
     */
    virtual void setAttribute_internal(std::string attr_name,
                                       boost::any const& attr_value);

    /**
     * @brief get attribute value by name
     * @param attr_name attribute name
     * @throws runtime_error if the attribute does not exist for the current
     * wavelet or is not shared among filters,
     * or if the type does not match the attribute's internal type
     * @return attribute value
     */
    virtual boost::any getAttribute_internal(std::string attr_name) const;

///@}

#pragma mark -
#pragma mark === Protected Attributes ===
    /**
     * @brief Data buffer (circular buffer shared among bands associated with
     * the same samplerate)
     */
    std::map<int, boost::circular_buffer<float>> data_;

    /**
     * @brief Low-pass Filters for decimation
     */
    std::map<int, LowpassFilter> filters_;

    /**
     * @brief Wavelets
     */
    std::vector<std::shared_ptr<Wavelet>> wavelets_;

    /**
     * @brief reference wavelet (stores common attributes)
     */
    std::shared_ptr<Wavelet> reference_wavelet_;

    /**
     * @brief Current frame index (used to skip frames in agressive
     * optimization)
     */
    int frame_index_;

    ///@endcond
};

#pragma mark -
#pragma mark === Attribute Specializations ===
///@cond DEVDOC
template <>
void checkLimits<Family>(Family const& value, Family const& limit_min,
                         Family const& limit_max);

template <>
void checkLimits<Filterbank::Optimisation>(
    Filterbank::Optimisation const& value,
    Filterbank::Optimisation const& limit_min,
    Filterbank::Optimisation const& limit_max);

template <>
Family Attribute<Family>::default_limit_max();

template <>
Filterbank::Optimisation
Attribute<Filterbank::Optimisation>::default_limit_max();
///@endcond
}

#endif
