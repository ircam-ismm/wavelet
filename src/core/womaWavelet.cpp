/*
 * womaWavelet.cpp
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

#include "womaWavelet.hpp"

woma::Wavelet::Wavelet(float samplerate)
    : samplerate(this, samplerate, 0.),
      scale(this, 2. / static_cast<double>(this->samplerate.get()), 0.),
      window_size(this, 1, 1),
      mode(this, RECURSIVE, RECURSIVE, SPECTRAL),
      delay(this, DEFAULT_DELAY(), 0.),
      padding(this, DEFAULT_PADDING(), 0.) {}

woma::Wavelet::Wavelet(Wavelet const& src) { _copy(this, src); }

woma::Wavelet& woma::Wavelet::operator=(Wavelet const& src) {
    if (this != &src) _copy(this, src);
    return *this;
}

void woma::Wavelet::_copy(Wavelet* dst, Wavelet const& src) {
    dst->samplerate = src.samplerate;
    dst->samplerate.set_parent(dst);
    dst->scale = src.scale;
    dst->scale.set_parent(dst);
    dst->window_size = src.window_size;
    dst->window_size.set_parent(dst);
    dst->mode = src.mode;
    dst->mode.set_parent(dst);
    dst->delay = src.delay;
    dst->delay.set_parent(dst);
    dst->padding = src.padding;
    dst->padding.set_parent(dst);
    dst->values = src.values;
}

void woma::Wavelet::init() {
    values.assign(this->window_size.get(), 0.0);
    if (this->mode.get() == RECURSIVE) {
        int pad_length = static_cast<int>(padding.get() * eFoldingTime() *
                                          this->samplerate.get());
        prepad_value_ = std::complex<double>(0., 0.);
        for (int t = -pad_length; t < 0; ++t) {
            double wavelet_arg =
                (double(t) - double(this->window_size.get() / 2)) /
                (this->scale.get() * this->samplerate.get());
            prepad_value_ += std::conj(phi(wavelet_arg));
        }
        postpad_value_ = std::complex<double>(0., 0.);
        for (int t = static_cast<int>(this->window_size.get());
             t < static_cast<int>(this->window_size.get()) + pad_length; ++t) {
            double wavelet_arg =
                (double(t) - double(this->window_size.get() / 2)) /
                (this->scale.get() * this->samplerate.get());
            postpad_value_ += std::conj(phi(wavelet_arg));
        }
        for (int t = 0; t < this->window_size.get(); ++t) {
            double wavelet_arg =
                (double(t) - double(this->window_size.get() / 2)) /
                (this->scale.get() * this->samplerate.get());
            values[t] = phi(wavelet_arg);
        }
    } else {  // mode_ == SPECTRAL
        values.assign(this->window_size.get(), std::complex<double>(0.0, 0.0));
        for (int t = 0; t < int(this->window_size.get() / 2); ++t) {
            double s_omega = this->scale.get() * 2. * M_PI * t *
                             this->samplerate.get() /
                             double(this->window_size.get());
            values[t] = phi_spectral(s_omega);
        }
        for (int t = int(this->window_size.get() / 2);
             t < int(this->window_size.get()); ++t) {
            double s_omega = this->scale.get() * 2. * M_PI * t *
                             this->samplerate.get() /
                             double(this->window_size.get());
            values[t] = phi_spectral(-s_omega);
        }
    }
}

void woma::Wavelet::onAttributeChange(AttributeBase* attr_pointer) {
    init();
    attr_pointer->changed = false;
}

std::string woma::Wavelet::info() const {
    std::stringstream infostrstream;
    infostrstream << "Wavelet:\n";
    infostrstream << "\tSampling rate: " << this->samplerate.get() << "\n";
    infostrstream << "\tScale: " << this->scale.get() << "\n";
    infostrstream << "\tEquivalent Frequency (Hz): "
                  << scale2frequency(this->scale.get()) << "\n";
    infostrstream << "\tWindow Size: " << this->window_size.get() << "\n";
    return infostrstream.str();
}

void woma::Wavelet::setAttribute_internal(std::string attr_name,
                                          boost::any const& attr_value) {
    if (attr_name == "samplerate") {
        samplerate.set(boost::any_cast<float>(attr_value));
    } else if (attr_name == "scale") {
        scale.set(boost::any_cast<double>(attr_value));
    } else if (attr_name == "window_size") {
        window_size.set(boost::any_cast<std::size_t>(attr_value));
    } else if (attr_name == "mode") {
        mode.set(boost::any_cast<WaveletDomain>(attr_value));
    } else if (attr_name == "delay") {
        delay.set(boost::any_cast<float>(attr_value));
    } else if (attr_name == "padding") {
        padding.set(boost::any_cast<float>(attr_value));
    } else {
        throw std::out_of_range("Unknown attribute name");
    }
}

boost::any woma::Wavelet::getAttribute_internal(std::string attr_name) const {
    if (attr_name == "samplerate") {
        return boost::any(samplerate.get());
    } else if (attr_name == "scale") {
        return boost::any(scale.get());
    } else if (attr_name == "window_size") {
        return boost::any(window_size.get());
    } else if (attr_name == "mode") {
        return boost::any(mode.get());
    } else if (attr_name == "delay") {
        return boost::any(delay.get());
    } else if (attr_name == "padding") {
        return boost::any(padding.get());
    } else {
        throw std::out_of_range("Unknown attribute name");
    }
}

void woma::Wavelet::setDefaultWindowsize() {
    std::size_t winsize = static_cast<std::size_t>(
        2. * delay.get() * eFoldingTime() * this->samplerate.get());
    winsize = (winsize < 3) ? 3 : winsize;
    winsize += (winsize % 2 == 0);
    window_size.set(winsize);
}

template <>
void woma::checkLimits<woma::Wavelet::WaveletDomain>(
    woma::Wavelet::WaveletDomain const& value,
    woma::Wavelet::WaveletDomain const& limit_min,
    woma::Wavelet::WaveletDomain const& limit_max) {
    if (value < limit_min || value > limit_max)
        throw std::domain_error("Attribute value out of range. Range: [" +
                                std::to_string(limit_min) + " ; " +
                                std::to_string(limit_max) + "]");
}

template <>
woma::Wavelet::WaveletDomain
woma::Attribute<woma::Wavelet::WaveletDomain>::default_limit_max() {
    return woma::Wavelet::SPECTRAL;
}
