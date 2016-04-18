/*
 * womaFilterbank.cpp
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

#include "womaFilterbank.hpp"
#include <memory>

woma::Filterbank::Filterbank(float samplerate_, float frequency_min_,
                             float frequency_max_, float bands_per_octave_)
    : frequency_min(this, frequency_min_, 1e-12, frequency_max_),
      frequency_max(this, frequency_max_, frequency_min_, samplerate_ / 2.),
      bands_per_octave(this, bands_per_octave_, 1.),
      optimisation(this, Optimisation::None),
      family(this, DEFAULT_FAMILY),
      rescale(this, true) {
    switch (family.get()) {
        case woma::Family::Morlet:
            reference_wavelet_ =
                std::unique_ptr<MorletWavelet>(new MorletWavelet(samplerate_));
            break;

        case woma::Family::Paul:
            reference_wavelet_ =
                std::unique_ptr<PaulWavelet>(new PaulWavelet(samplerate_));
            break;

        default:
            throw std::runtime_error("Wavelet not implemented");
            break;
    }

    init();
}

woma::Filterbank::Filterbank(Filterbank const& src) {
    this->frequency_min = src.frequency_min;
    this->frequency_min.set_parent(this);
    this->frequency_max = src.frequency_max;
    this->frequency_max.set_parent(this);
    this->bands_per_octave = src.bands_per_octave;
    this->bands_per_octave.set_parent(this);
    this->optimisation = src.optimisation;
    this->optimisation.set_parent(this);
    this->family = src.family;
    this->family.set_parent(this);
    this->rescale = src.rescale;
    this->rescale.set_parent(this);
    switch (this->family.get()) {
        case woma::Family::Morlet:
            this->reference_wavelet_ = std::unique_ptr<MorletWavelet>(
                new MorletWavelet(*std::static_pointer_cast<MorletWavelet>(
                    src.reference_wavelet_)));
            break;

        case woma::Family::Paul:
            this->reference_wavelet_ = std::unique_ptr<PaulWavelet>(
                new PaulWavelet(*std::static_pointer_cast<PaulWavelet>(
                    src.reference_wavelet_)));
            break;

        default:
            throw std::runtime_error("Wavelet not implemented");
            break;
    }
    this->init();
}

woma::Filterbank& woma::Filterbank::operator=(Filterbank const& src) {
    if (this != &src) {
        this->frequency_min = src.frequency_min;
        this->frequency_min.set_parent(this);
        this->frequency_max = src.frequency_max;
        this->frequency_max.set_parent(this);
        this->bands_per_octave = src.bands_per_octave;
        this->bands_per_octave.set_parent(this);
        this->optimisation = src.optimisation;
        this->optimisation.set_parent(this);
        this->family = src.family;
        this->family.set_parent(this);
        this->rescale = src.rescale;
        this->rescale.set_parent(this);
        this->reference_wavelet_ = std::unique_ptr<MorletWavelet>(
            new MorletWavelet(src.reference_wavelet_->samplerate.get()));
        *(this->reference_wavelet_) = *(src.reference_wavelet_);
        this->init();
    }
    return *this;
}

woma::Filterbank::~Filterbank() {}

std::string woma::Filterbank::info() const {
    std::stringstream infostrstream;
    infostrstream << "Wavelet Filter:\n";
    infostrstream << "\tFrequency Range: " << frequency_min.get() << " "
                  << frequency_max.get() << "\n";
    infostrstream << "\tBands per Octave: " << bands_per_octave.get() << "\n";
    infostrstream << "\tOptimisation: " << static_cast<int>(optimisation.get())
                  << "\n";
    if (!wavelets_.empty()) {
        infostrstream << reference_wavelet_->info();
    }
    return infostrstream.str();
}

std::vector<int> woma::Filterbank::delaysInSamples() const {
    std::vector<int> delays(size());
    unsigned int i(0);
    for (auto& wav : wavelets_) {
        if (optimisation.get() != Optimisation::None) {
            delays[i] =
                wav->delay.get() * wav->eFoldingTime() *
                    reference_wavelet_->samplerate.get() +
                ((downsampling_factors[i] > 1) ? downsampling_factors[i] : 0);
            i++;
        } else {
            delays[i++] = wav->delay.get() * wav->eFoldingTime() *
                          reference_wavelet_->samplerate.get();
        }
    }
    return delays;
}

std::size_t woma::Filterbank::size() const { return wavelets_.size(); }

void woma::Filterbank::onAttributeChange(AttributeBase* attr_pointer) {
    if (attr_pointer == &family) {
        float samplerate = reference_wavelet_->samplerate.get();
        switch (family.get()) {
            case woma::Family::Morlet:
                reference_wavelet_ = std::unique_ptr<MorletWavelet>(
                    new MorletWavelet(samplerate));
                break;

            case woma::Family::Paul:
                reference_wavelet_ =
                    std::unique_ptr<PaulWavelet>(new PaulWavelet(samplerate));
                break;

            default:
                throw std::runtime_error("Wavelet not implemented");
                break;
        }
    }
    init();
    attr_pointer->changed = false;
}

void woma::Filterbank::setAttribute_internal(std::string attr_name,
                                             boost::any const& attr_value) {
    if (attr_name == "frequency_min") {
        frequency_min.set(boost::any_cast<float>(attr_value));
        frequency_max.set_limit_min(frequency_min.get());
    } else if (attr_name == "frequency_max") {
        frequency_max.set(boost::any_cast<float>(attr_value));
        frequency_min.set_limit_max(frequency_max.get());
    } else if (attr_name == "bands_per_octave") {
        bands_per_octave.set(boost::any_cast<float>(attr_value));
    } else if (attr_name == "family") {
        family.set(boost::any_cast<Family>(attr_value));
    } else if (attr_name == "optimisation") {
        optimisation.set(boost::any_cast<Optimisation>(attr_value));
    } else if (attr_name == "rescale") {
        rescale.set(boost::any_cast<bool>(attr_value));
    } else {
        if (attr_name != "scale" && attr_name != "window_size") {
            reference_wavelet_->setAttribute(attr_name, attr_value);
            if (attr_name == "samplerate") {
                frequency_max.set_limit_max(boost::any_cast<float>(attr_value) /
                                            2.);
            }
        } else {
            throw std::runtime_error(
                "Attribute " + attr_name +
                " does not exist or is not shared among filters.");
        }
        init();
    }
}

boost::any woma::Filterbank::getAttribute_internal(
    std::string attr_name) const {
    if (attr_name == "frequency_min") return boost::any(frequency_min.get());
    if (attr_name == "frequency_max") return boost::any(frequency_max.get());
    if (attr_name == "bands_per_octave")
        return boost::any(bands_per_octave.get());
    if (attr_name == "optimisation") return boost::any(optimisation.get());
    if (attr_name == "family") return boost::any(family.get());
    if (attr_name == "rescale") return boost::any(rescale.get());
    if (attr_name != "scale" && attr_name != "window_size")
        return reference_wavelet_->getAttribute_internal(attr_name);
    throw std::runtime_error("Attribute " + attr_name +
                             "does not exist or is not shared among filters.");
}

void woma::Filterbank::init() {
    // Compute Scales of the Filterbank
    double scale_0 = 2. / reference_wavelet_->samplerate.get();
    double min_scale = reference_wavelet_->frequency2scale(frequency_max.get());
    double max_scale = reference_wavelet_->frequency2scale(frequency_min.get());
    long min_index =
        1 + long(log2(min_scale / scale_0) * bands_per_octave.get());
    long max_index =
        1 + long(log2(max_scale / scale_0) * bands_per_octave.get());
    //    std::size_t _first_scale_index = min_index; // NEEDED FOR RIDGE
    //    EXTRACTION
    // std::cout << "Scale offset: " << min_index << std::endl;

    scales.resize(max_index - min_index);
    frequencies.resize(max_index - min_index);
    for (long scale_index = min_index, i = 0; scale_index < max_index;
         scale_index++, i++) {
        scales[i] =
            scale_0 * pow(2., double(scale_index) / bands_per_octave.get());
        frequencies[i] = reference_wavelet_->scale2frequency(scales[i]);
    }
    if (optimisation.get() != Optimisation::None) {
        downsampling_factors.resize(max_index - min_index);
        for (long scale_index = min_index, i = 0; scale_index < max_index;
             scale_index++, i++) {
            double samplerate_ratio;
            if (optimisation.get() == Optimisation::Standard1 ||
                optimisation.get() == Optimisation::Aggressive1) {
                samplerate_ratio =
                    (reference_wavelet_->samplerate.get() / 8) / frequencies[i];
            } else {
                samplerate_ratio =
                    (reference_wavelet_->samplerate.get() / 4) / frequencies[i];
            }
            downsampling_factors[i] = static_cast<int>(samplerate_ratio);
            downsampling_factors[i] =
                (downsampling_factors[i] > 1) ? downsampling_factors[i] : 1;
        }
    }

    // Allocate and initialize wavelets
    switch (family.get()) {
        case woma::Family::Morlet:
            wavelets_.resize(max_index - min_index);
            for (unsigned int i = 0; i < scales.size(); i++) {
                wavelets_[i] = std::shared_ptr<MorletWavelet>(
                    new MorletWavelet(*std::static_pointer_cast<MorletWavelet>(
                        reference_wavelet_)));
                if (optimisation.get() != Optimisation::None)
                    wavelets_[i]->samplerate.set(
                        reference_wavelet_->samplerate.get() /
                        double(downsampling_factors[i]));
                wavelets_[i]->scale.set(scales[i]);
                wavelets_[i]->setDefaultWindowsize();
            }
            break;

        case woma::Family::Paul:
            wavelets_.resize(max_index - min_index);
            for (unsigned int i = 0; i < scales.size(); i++) {
                wavelets_[i] = std::shared_ptr<PaulWavelet>(
                    new PaulWavelet(*std::static_pointer_cast<PaulWavelet>(
                        reference_wavelet_)));
                if (optimisation.get() != Optimisation::None)
                    wavelets_[i]->samplerate.set(
                        reference_wavelet_->samplerate.get() /
                        double(downsampling_factors[i]));
                wavelets_[i]->scale.set(scales[i]);
                wavelets_[i]->setDefaultWindowsize();
            }
            break;

        default:
            throw std::runtime_error("Wavelet not implemented");
            break;
    }

    data_.clear();
    filters_.clear();
    if (optimisation.get() == Optimisation::None) {
        data_[1].resize(wavelets_[wavelets_.size() - 1]->window_size.get());
        downsampling_factors.clear();
    } else {
        for (int i = 0; i < wavelets_.size(); i++) {
            data_[downsampling_factors[i]].resize(
                wavelets_[i]->window_size.get() * downsampling_factors[i]);
            data_[downsampling_factors[i]].clear();
            if ((downsampling_factors[i] > 1) &&
                (filters_.count(downsampling_factors[i]) == 0)) {
                filters_[downsampling_factors[i]].cutoff.set(
                    0.8 / double(downsampling_factors[i]));
            }
        }
    }
    frame_index_ = 0;
    result_complex.assign(wavelets_.size(), std::complex<double>(0.0));
    result_power.assign(wavelets_.size(), 0.0);
}

void woma::Filterbank::reset() {
    for (auto data_it = data_.begin(); data_it != data_.end(); data_it++) {
        data_it->second.clear();
    }
    frame_index_ = 0;
}

void woma::Filterbank::update(float value) {
    // Update Buffers
    auto data_it = data_.begin();
    if (data_it->first == 1) {
        if (data_it->second.size() > 0) {
            data_it->second.push_back(value);
        } else {
            for (unsigned int i = 0; i < 2 * data_it->second.capacity() - 1;
                 ++i) {
                data_it->second.push_back(value);
            }
        }
        data_it++;
    }
    if (optimisation.get() != Optimisation::None) {
        double filtered_value(value);
        for (auto filters_it = filters_.begin(); filters_it != filters_.end();
             filters_it++, data_it++) {
            filtered_value = filters_it->second.filter(value);
            if (data_it->second.size() > 0) {
                data_it->second.push_back(filtered_value);
            } else {
                for (unsigned int i = 0; i < 2 * data_it->second.capacity() - 1;
                     ++i) {
                    filtered_value = filters_it->second.filter(value);
                }
                for (unsigned int i = 0; i < 2 * data_it->second.capacity() - 1;
                     ++i) {
                    data_it->second.push_back(filtered_value);
                }
            }
        }
    }

    // Update filter
    data_it = data_.begin();
    int previous_downrate(1);
    for (std::size_t filter_index = 0; filter_index < wavelets_.size();
         filter_index++) {
        if (optimisation.get() != Optimisation::None) {
            if (downsampling_factors[filter_index] != previous_downrate) {
                if (filter_index > 0) data_it++;
                previous_downrate = downsampling_factors[filter_index];
            }
        }

        if (optimisation.get() == Optimisation::Aggressive1 ||
            optimisation.get() == Optimisation::Aggressive2) {
            if ((frame_index_ % downsampling_factors[filter_index]) != 0) {
                continue;
            }
        }
        result_complex[filter_index] = std::complex<double>(0., 0.);

        // Padding: before
        result_complex[filter_index] =
            std::complex<double>(data_it->second[0], 0) *
            wavelets_[filter_index]->prepad_value_;
        // Data
        std::size_t wvt_index(0);
        std::size_t decim = static_cast<std::size_t>(data_it->first);
        for (std::size_t data_index =
                             data_it->second.size() -
                             decim * wavelets_[filter_index]->window_size.get();
             data_index < data_it->second.size();
             data_index += decim, wvt_index++) {
            result_complex[filter_index] +=
                std::complex<double>(data_it->second[data_index], 0) *
                std::conj(wavelets_[filter_index]->values[wvt_index]);
        }
        // Padding: after
        result_complex[filter_index] +=
            std::complex<double>(data_it->second[data_it->second.size() - 1],
                                 0) *
            wavelets_[filter_index]->postpad_value_;
        // Rescale
        if (rescale.get())
            result_complex[filter_index] /=
                std::sqrt(wavelets_[filter_index]->scale.get());
        result_complex[filter_index] *= std::sqrt(double(decim));
        result_power[filter_index] = std::norm(result_complex[filter_index]);
    }
    frame_index_++;
}

#ifdef USE_ARMA
arma::cx_mat woma::Filterbank::process(std::vector<double> values) {
    //// SPECTRAL METHOD
    arma::cx_vec sig_spectral =
        arma::fft(arma::conv_to<arma::vec>::from(values));
    arma::cx_vec sig_spectral_tmp;
    arma::cx_mat scalogram(values.size(), size());
    for (std::size_t filter_index = 0; filter_index < size(); filter_index++) {
        std::size_t previous_window_size =
            wavelets_[filter_index]->window_size.get();
        wavelets_[filter_index]->mode.set(Wavelet::SPECTRAL);
        wavelets_[filter_index]->window_size.set(values.size());
        sig_spectral_tmp = sig_spectral % arma::conv_to<arma::cx_vec>::from(
                                              wavelets_[filter_index]->values);
        scalogram.col(filter_index) = arma::ifft(sig_spectral_tmp);
        if (rescale.get())
            scalogram.col(filter_index) /=
                (sqrt(wavelets_[filter_index]->scale.get()));
        wavelets_[filter_index]->window_size.set(previous_window_size);
        wavelets_[filter_index]->mode.set(Wavelet::RECURSIVE);
    }
    return scalogram;
}

arma::cx_mat woma::Filterbank::process_online(std::vector<double> values) {
    reset();
    arma::cx_mat scalogram(values.size(), size());
    for (std::size_t t = 0; t < values.size(); t++) {
        update(values[t]);
        scalogram.row(t) = arma::conv_to<arma::cx_rowvec>::from(result_complex);
    }
    return scalogram;
}
#endif

template <>
void woma::checkLimits<woma::Family>(woma::Family const& value,
                                     woma::Family const& limit_min,
                                     woma::Family const& limit_max) {
    if (value < limit_min || value > limit_max)
        throw std::domain_error(
            "Attribute value out of range. Range: [" +
            std::to_string(static_cast<int>(limit_min)) + " ; " +
            std::to_string(static_cast<int>(limit_max)) + "]");
}

template <>
void woma::checkLimits<woma::Filterbank::Optimisation>(
    woma::Filterbank::Optimisation const& value,
    woma::Filterbank::Optimisation const& limit_min,
    woma::Filterbank::Optimisation const& limit_max) {
    if (value < limit_min || value > limit_max)
        throw std::domain_error(
            "Attribute value out of range. Range: [" +
            std::to_string(static_cast<int>(limit_min)) + " ; " +
            std::to_string(static_cast<int>(limit_max)) + "]");
}

template <>
woma::Family woma::Attribute<woma::Family>::default_limit_max() {
    return woma::Family::Paul;
}

template <>
woma::Filterbank::Optimisation
woma::Attribute<woma::Filterbank::Optimisation>::default_limit_max() {
    return woma::Filterbank::Optimisation::Aggressive2;
}
