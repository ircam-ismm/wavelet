#!/bin/sh

INSTALL_DIR=bin/wavelet
mkdir -p ${INSTALL_DIR}
cp _wavelet.so wavelet.py src/__init__.py ${INSTALL_DIR}
