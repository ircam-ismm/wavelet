#!/bin/sh

INSTALL_DIR=bin/woma
mkdir -p ${INSTALL_DIR}
cp _woma.so woma.py src/__init__.py ${INSTALL_DIR}
