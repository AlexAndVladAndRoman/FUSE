#!/bin/bash

mkdir -p out
cd out

echo
echo
echo Compiling:
cmake ../

echo
echo
echo Making:
make

cd ..
