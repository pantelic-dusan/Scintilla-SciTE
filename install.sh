#!/bin/bash

cd scintilla/gtk/
make
cd ../../scite/gtk
make
sudo make install
cd ../../
