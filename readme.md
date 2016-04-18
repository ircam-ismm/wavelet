# woma - Wavelets for Online Movement Analysis

__woma__ is a cross-platform c++/python library for online estimation of the Continuous Wavelet Transform (CWT). The online estimation is based on a filterbank implementation of the CWT with minimal delay per scale and optimization based on multi-rate computation. The library also allows for offline estimation of the CWT using FFT.

The following wavelets are currently implemented:
- Complex Morlet (= Gabor) Wavelet
- Paul wavelet

### Contact

Jules Françoise: <jules.francoise@ircam.fr>

### author

This code has been authored by <a href="http://julesfrancoise.com">Jules Françoise</a> in the framework of the <a href="http://skatvg.iuav.it/">SkAT-VG</a> European project, with <a href="frederic-bevilacqua.net">Frederic Bevilacqua</a>, in the <a href="http://ismm.ircam.fr">Sound Music Movement Interaction</a> team of the <a href="http://www.ircam.fr/stms.html?&L=1">STMS Lab</a> - IRCAM - CNRS - UPMC (2011-2015).

### Copyright

Copyright (C) 2015 Ircam-Centre Pompidou.

### Licence

This project is released under the <a href="http://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> license.
For commercial applications, a proprietary license is available upon request to Frederick Rousseau <frederick.rousseau@ircam.fr>.

Wavelet is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Wavelet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Wavelet.  If not, see <http://www.gnu.org/licenses/>.

### Dependencies & Compatibiliy

The library uses a subset of c++11 functions.
It depends on the open-source matrix library <a href="http://arma.sourceforge.net/">Armadillo</a> for linear algebra and FFT (only required for offline computation), <a href="http://www.boost.org/">Boost</a> (header-only), and <a href="https://github.com/philsquared/Catch">Catch</a> for unit-testing.

## Download

The source code is available on __Github__: https://github.com/Ircam-RnD/wavelet

The online transform is also implemented as a <a href="http://ismm.ircam.fr/pipo/">PiPo</a> external for Cycling'74 Max.

## Documentation

The full documentation is available on Github Pages: http://ircam-rnd.github.io/wavelet/

## Compilation and Usage

### Dependencies

To compile the library, you need the following dependencies installed:
- Boost: <a href="http://www.boost.org/">http://www.boost.org/</a>
- [optional] Armadillo: <a href="http://arma.sourceforge.net/">http://arma.sourceforge.net/</a> (only required for offline computation)

### Compiling as a static/dynamic library
#### XCode

See the xcode project in "ide/xcode/"

#### CMake

The library can be built using <a href="http://www.cmake.org/">CMake</a>.
In the root directory, type the following command to generate the Makefiles:
```
cmake . -G"Unix Makefiles"
```
The following commands can be used to build the static library and run the unit tests, and generate the documentation:
```
make
make tests
```

The following commands can be used to generate the developer documentation and the api documentation:
```
make doc
make docapi
```
#### Usage

The header file "woma.h" includes all useful headers of the library.

### Building the Python Library
#### Dependencies

* <a href="http://www.doxygen.org/">doxygen</a>
* <a href="http://www.swig.org/">swig</a>
* <a href="http://www.numpy.org/">Numpy</a>
* <a href="http://matplotlib.org/">Matplotlib</a> (for plotting utilities)

#### Building

The python module can be built using <a href="http://www.cmake.org/">CMake</a>.
In the python directory, type the following command to generate the Makefiles and build the python module:
```
cmake . -G"Unix Makefiles"
make
```
The module should be installed in "${wavelet_root}/python/bin/"
