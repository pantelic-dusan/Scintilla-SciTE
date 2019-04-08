#!/bin/bash

cd scintilla/gtk/
make clean
cd ../../scite/gtk
make clean
sudo make uninstall
cd ../../
