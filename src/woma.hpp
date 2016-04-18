/*
 * woma.h
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

#ifndef woma_h
#define woma_h

#include "core/womaAttribute.hpp"
#include "core/womaFilterbank.hpp"

/**
    @mainpage Wavelet - A library for online estimation of the Continuous
   Wavelet Transform
    @anchor mainpage
    @brief __wavelet__ is a cross-platform c++ library for online estimation of
   the
    Continuous Wavelet Transform (CWT). The online estimation is based on a
   filterbank
    implementation of the CWT with minimal delay per scale and optimization
   based on
    multi-rate computation. The library also allows for offline estimation of
   the CWT using FFT.

    The following wavelets are currently implemented:
    - Complex Morlet Wavelet (also called Gabor wavelet)
    - Paul wavelet

    @par Contact
    Jules Françoise: <jules.francoise@ircam.fr>

    @author
    This code has been authored by <a href="http://julesfrancoise.com">Jules
   Françoise</a>
    in the framework of the <a href="http://skatvg.iuav.it/">SkAT-VG</a>
   European project,
    with <a href="frederic-bevilacqua.net">Frederic Bevilacqua</a>, in the
    <a href="http://ismm.ircam.fr">Sound Music Movement Interaction</a> team of
   the
    <a href="http://www.ircam.fr/stms.html?&L=1">STMS Lab</a> - IRCAM - CNRS -
   UPMC (2011-2015).

    @copyright
    Copyright (C) 2015 Ircam-Centre Pompidou.

    @par Licence
    This project is released under the <a
   href="http://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> license.
    For commercial applications, a proprietary license is available upon request
   to Frederick Rousseau <frederick.rousseau@ircam.fr>. @n@n
    Wavelet is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version. @n@n
    Wavelet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details. @n@n
    You should have received a copy of the GNU General Public License
    along with Wavelet.  If not, see <http://www.gnu.org/licenses/>.

    @par Dependencies & Compatibiliy
    The library uses a subset of c++11 functions.
    It depends on the open-source matrix library
    <a href="http://arma.sourceforge.net/">Armadillo</a> for linear algebra and
   FFT (only required for offline computation),
    <a href="http://www.boost.org/">Boost</a> (header-only), and
    <a href="https://github.com/philsquared/Catch">Catch</a> for unit-testing.

    @section download Download

    The source code is available on __Github__:
   https://github.com/Ircam-RnD/wavelet

    The online transform is also implemented as a <a
   href="http://ismm.ircam.fr/pipo/">PiPo</a>
    external for Cycling'74 Max.

    @section toc Table of Contents

    1. @subpage Compilation @n
    3. @subpage Examples @n

    @page Compilation
    @section compile-lib Compiling the library
    @tableofcontents

    @subsection dependencies Dependencies

    To compile the library, you need the following dependencies installed:
    - Boost headers: <a href="http://www.boost.org/">http://www.boost.org/</a>
    - [optional] Armadillo: <a
   href="http://arma.sourceforge.net/">http://arma.sourceforge.net/</a> (only
   required for offline computation)

    @subsection compiling Compiling as a static/dynamic library
    @par XCode

    See the xcode project in "ide/xcode/"

    @par CMake

    The library can be built using <a href="http://www.cmake.org/">CMake</a>.
    In the root directory, type the following command to generate the Makefiles:
    ```
    cmake . -G"Unix Makefiles"
    ```
    The following commands can be used to build the static library and run the
   unit tests, and generate the documentation:
    ```
    make
    make tests
    ```

    The following commands can be used to generate the developer documentation
   and the api documentation:
    ```
    make doc
    make docapi
    ```
    @par Usage

    The header file "woma.h" includes all useful headers of the library.

    @section building-python Building the Python Library
    @par Dependencies

    - <a href="http://www.doxygen.org/">doxygen</a>
    - <a href="http://www.swig.org/">swig</a>
    - <a href="http://www.numpy.org/">Numpy</a>

    @par Building

    The python module can be built using <a
   href="http://www.cmake.org/">CMake</a>.
    In the python directory, type the following command to generate the
   Makefiles and build the python module:
    ```
    cmake . -G"Unix Makefiles"
    make
    ```
    The module should be installed in "${wavelet_root}/python/bin/"

    <center>Prev: \ref mainpage "Home" | Next: \ref Examples.</center>

    @page Examples

    @section cppex C++ Example

    @code{.cpp}
    #include "woma.hpp"

    int main()
    {
     float samplerate_hz(100.);
     float frequency_min = 0.1;
     float frequency_max = 50.;
     float bands_per_octave = 8;
     woma::Filterbank cwt(samplerate_hz,
                             frequency_min,
                             frequency_max,
                             bands_per_octave);
     std::size_t numbands(cwt.size());

     cwt.reset(); // Reset processing
     while (true) {
         double value = // Get data from a stream
         cwt.update(value);
         for (unsigned int band=0; band<numbands; band++) {
             std::cout << cwt.result_complex[band] << " ";
         }
         std::cout << std::endl;
     }

     return 0;
    }
    @endcode

    @section pyex Python Example

    TODO
    @todo python example

    <center>Prev: \ref Compilation | Up: \ref mainpage "Home".</center>
 */

#endif
