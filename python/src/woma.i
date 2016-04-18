/*
 * woma.i
 *
 * Swig interface file to the woma Library
 *
 * Contact:
 * - Jules Françoise <jules.francoise@ircam.fr>
 *
 * This code has been authored by <a href="http://julesfrancoise.com">Jules Françoise</a>
 * in the framework of the <a href="http://skatvg.iuav.it/">SkAT-VG</a> European project,
 * with <a href="frederic-bevilacqua.net">Frederic Bevilacqua</a>, in the
 * <a href="http://ismm.ircam.fr">Sound Music Movement Interaction</a> team of the
 * <a href="http://www.ircam.fr/stms.html?&L=1">STMS Lab</a> - IRCAM - CNRS - UPMC (2011-2015).
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

%module(docstring="woma - Wavelets for Online Movement Analysis") woma

#pragma SWIG nowarn=362,503

%{
    #define SWIG_FILE_WITH_INIT
    #include "womaAttribute.hpp"
    #include "womaFilterbank.hpp"
    #include "womaLowpass.hpp"
    #include "womaWavelet.hpp"
    #include "womaMorlet.hpp"
    #include "womaPaul.hpp"
%}

%exception {
    try {
        $action
    }
    catch (std::exception const& e) {
        PyErr_SetString(PyExc_IndexError,e.what());
        SWIG_fail;
    }
}

%include <std_vector.i>
%include <std_string.i>
%include <std_complex.i>
%include "armanpy.i"

namespace std {
    %template(vectord) std::vector<double>;
    %template(vectorc) std::vector< std::complex<double> >;
    %template(vectorf) std::vector<float>;
    %template(vectori) std::vector<int>;
    %template(vectors) std::vector<std::string>;
};

%include ../woma_doc.i
%include "womaAttribute.hpp"
%include "womaFilterbank.hpp"
%include "womaLowpass.hpp"
%include "womaWavelet.hpp"
%include "womaMorlet.hpp"
%include "womaPaul.hpp"

// Wrap Attribute template specializations
%define WRAP_ATTR_TEMPLATES(type_name, cpp_type)
%template(Attribute_ ## type_name) Attribute<cpp_type>;
%template(_getAttribute_ ## type_name) Filterbank::getAttribute<cpp_type>;
%template(_setAttribute_ ## type_name) Filterbank::setAttribute<cpp_type>;
%enddef

namespace woma {
    WRAP_ATTR_TEMPLATES(bool, bool)
    WRAP_ATTR_TEMPLATES(unsigned_char, unsigned char)
    WRAP_ATTR_TEMPLATES(unsigned_int, unsigned int)
    WRAP_ATTR_TEMPLATES(unsigned_long, unsigned long)
    WRAP_ATTR_TEMPLATES(char, char)
    WRAP_ATTR_TEMPLATES(int, int)
    WRAP_ATTR_TEMPLATES(long, long)
    WRAP_ATTR_TEMPLATES(size_t, std::size_t)
    WRAP_ATTR_TEMPLATES(float, float)
    WRAP_ATTR_TEMPLATES(double, double)
    WRAP_ATTR_TEMPLATES(long_double, long double)
    WRAP_ATTR_TEMPLATES(Family, Family)
    WRAP_ATTR_TEMPLATES(WaveletDomain, Wavelet::WaveletDomain)
    WRAP_ATTR_TEMPLATES(Optimisation, Filterbank::Optimisation)
};

// Rewrite interface to set Attributes
%pythoncode %{
def setAttribute(self, attr_name, attr_value):
    """ Set an attribute by name

=== General attributes:

'frequency_min' [float]:
    Minimum Frequency of the Filterbank (Hz)
    Value range: ]0., samplerate/2.]
'frequency_max' [float]:
    Maximum Frequency of the Filterbank (Hz)
    Value range: ]0., samplerate/2.]
'bands_per_octave' [float]:
    Number of bands per octave of the Filterbank
    Value range: > 1.
'optimisation' [Optimisation]:
    Optimisation mode the filterbank implementation
    Value range: {None, Standard1, Standard2, Agressive1, Agressive2}
'family' [Family]:
    Wavelet Family
    Value range: {Morlet, Paul}
'samplerate' [float]:
     Sampling rate of the data
    Value range: ]0.
'delay' [float]:
     Delay relative to critical wavelet time
    Value range: > 0.
'padding' [float]:
     Padding relative to critical wavelet time
    Value range: >=0.

=== Wavelet-specific attributes:

Morlet Wavelet:
    'omega0' [float]:
        Carrier Frequency
        Value range: > 0.
Paul Wavelet:
    'order' [unsigned int]:
        Order of the Paul Wavelet
        Value range: > 0

    Args:
        attr_name -- attribute name
        attr_value -- attribute value

    Returns:
        Attribute value

    Raises:
        exception if the attribute does not exists or type error
    """
    try:
        self._setAttribute_bool(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_unsigned_char(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_unsigned_int(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_unsigned_long(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_char(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_int(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_long(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_size_t(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_float(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_double(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_long_double(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_Family(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_WaveletDomain(attr_name, attr_value)
        return
    except:
        pass
    try:
        self._setAttribute_Optimisation(attr_name, attr_value)
        return
    except:
        pass
    raise Exception("Ooops, it seems that the wrapper for this attribute is not implemented...")

Filterbank.setAttribute = setAttribute
LowpassFilter.setAttribute = setAttribute
Wavelet.setAttribute = setAttribute
MorletWavelet.setAttribute = setAttribute
PaulWavelet.setAttribute = setAttribute

def getAttribute(self, attr_name):
    """ Get an attribute by name

=== General attributes:

'frequency_min' [float]:
    Minimum Frequency of the Filterbank (Hz)
    Value range: ]0., samplerate/2.]
'frequency_max' [float]:
    Maximum Frequency of the Filterbank (Hz)
    Value range: ]0., samplerate/2.]
'bands_per_octave' [float]:
    Number of bands per octave of the Filterbank
    Value range: > 1.
'optimisation' [Optimisation]:
    Optimisation mode the filterbank implementation
    Value range: {None, Standard1, Standard2, Agressive1, Agressive2}
'family' [Family]:
    Wavelet Family
    Value range: {MORLET, PAUL}
'samplerate' [float]:
     Sampling rate of the data
    Value range: ]0.
'delay' [float]:
     Delay relative to critical wavelet time
    Value range: > 0.
'padding' [float]:
     Padding relative to critical wavelet time
    Value range: >=0.

=== Wavelet-specific attributes:

Morlet Wavelet:
    'omega0' [float]:
        Carrier Frequency
        Value range: > 0.
Paul Wavelet:
    'order' [unsigned int]:
        Order of the Paul Wavelet
        Value range: > 0

    Args:
        attr_name -- attribute name

    Returns:
        Attribute value

    Raises:
        exception if the attribute does not exists or type error
    """
    try:
        return self._getAttribute_bool(attr_name)
    except:
        pass
    try:
        return self._getAttribute_unsigned_char(attr_name)
    except:
        pass
    try:
        return self._getAttribute_unsigned_int(attr_name)
    except:
        pass
    try:
        return self._getAttribute_unsigned_long(attr_name)
    except:
        pass
    try:
        return self._getAttribute_char(attr_name)
    except:
        pass
    try:
        return self._getAttribute_int(attr_name)
    except:
        pass
    try:
        return self._getAttribute_long(attr_name)
    except:
        pass
    try:
        return self._getAttribute_size_t(attr_name)
    except:
        pass
    try:
        return self._getAttribute_float(attr_name)
    except:
        pass
    try:
        return self._getAttribute_double(attr_name)
    except:
        pass
    try:
        return self._getAttribute_long_double(attr_name)
    except:
        pass
    try:
        return self._getAttribute_Family(attr_name)
    except:
        pass
    try:
        return self._getAttribute_WaveletDomain(attr_name)
    except:
        pass
    try:
        return self._getAttribute_Optimisation(attr_name)
    except:
        pass
    raise Exception("Ooops, it seems that the wrapper for this attribute is not implemented...")

Filterbank.getAttribute = getAttribute
LowpassFilter.getAttribute = getAttribute
Wavelet.getAttribute = getAttribute
MorletWavelet.getAttribute = getAttribute
PaulWavelet.getAttribute = getAttribute
%}
