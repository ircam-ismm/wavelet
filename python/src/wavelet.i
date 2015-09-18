/*
 * wavelet.i
 *
 * Swig interface file to the Wavelet Library
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

%module(docstring="Wavelet - A library for online estimation of the Continuous Wavelet Transform") wavelet

#pragma SWIG nowarn=362,503

%{
    #define SWIG_FILE_WITH_INIT
    #include "attribute.hpp"
    #include "wavelet.hpp"
    #include "morlet.hpp"
    #include "paul.hpp"
	#include "filterbank.hpp"
    #include "lowpass.hpp"
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

%include ../wavelet_doc.i
%include "attribute.hpp"
%include "wavelet.hpp"
%include "morlet.hpp"
%include "paul.hpp"
%include "filterbank.hpp"
%include "lowpass.hpp"

// Wrap Attribute template specializations
%define WRAP_ATTR_TEMPLATES(type_name, cpp_type)
%template(Attribute_ ## type_name) Attribute<cpp_type>;
%template(getAttribute_ ## type_name) Filterbank::getAttribute<cpp_type>;
%template(setAttribute_ ## type_name) Filterbank::setAttribute<cpp_type>;
%enddef

namespace wavelet {
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
    try:
        self.setAttribute_unsigned_char(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_unsigned_int(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_unsigned_long(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_char(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_int(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_long(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_size_t(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_float(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_double(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_long_double(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_Family(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_WaveletDomain(attr_name, attr_value)
        return
    except:
        pass
    try:
        self.setAttribute_Optimisation(attr_name, attr_value)
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
    try:
        return self.getAttribute_unsigned_char(attr_name)
    except:
        pass
    try:
        return self.getAttribute_unsigned_int(attr_name)
    except:
        pass
    try:
        return self.getAttribute_unsigned_long(attr_name)
    except:
        pass
    try:
        return self.getAttribute_char(attr_name)
    except:
        pass
    try:
        return self.getAttribute_int(attr_name)
    except:
        pass
    try:
        return self.getAttribute_long(attr_name)
    except:
        pass
    try:
        return self.getAttribute_size_t(attr_name)
    except:
        pass
    try:
        return self.getAttribute_float(attr_name)
    except:
        pass
    try:
        return self.getAttribute_double(attr_name)
    except:
        pass
    try:
        return self.getAttribute_long_double(attr_name)
    except:
        pass
    try:
        return self.getAttribute_Family(attr_name)
    except:
        pass
    try:
        return self.getAttribute_WaveletDomain(attr_name)
    except:
        pass
    try:
        return self.getAttribute_Optimisation(attr_name)
    except:
        pass
    raise Exception("Ooops, it seems that the wrapper for this attribute is not implemented...")

Filterbank.getAttribute = getAttribute
LowpassFilter.getAttribute = getAttribute
Wavelet.getAttribute = getAttribute
MorletWavelet.getAttribute = getAttribute
PaulWavelet.getAttribute = getAttribute
%}
