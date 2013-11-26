# Project Information

## Introduction

Milu is an efficient and flexible C mutation testing tool designed for
both first order and highe order mutation testing. The name 'Milu' is
from a deer composed of four other animals. It has a horse's head, a
deer's antlers, a donkey's body and a cow's hooves.

If you've downloaded Milu, please let me know. If you are using Milu as
part of your research work (e.g. comparing your tool against it), then
please cite the following paper.

Yue Jia and Mark Harman. Milu: A Customizable, Runtime-Optimized Higher
Order Mutation Testing Tool for the Full C Language TAIC PART'08,
Windsor, UK, 29th-31st August 2008.

## Get Source Code 

git clone https://github.com/yuejia/Milu 

## Compilation in Ubuntu

- sudo apt-get install build-essential libglib2.0-dev llvm libclang-dev
- git clone https://github.com/yuejia/Milu
- cd Milu
- make

## Usage 
- ./bin/milu -f func.txt src.c 
NB: src.c need to be processed by gcc -E, func.txt contains a list of names of the functions under test

- ./bin/milu -? (to show options)

## Acknowledgement

This project was supported by the EPSRC grant, EP/D050863(SEBASE) and
the ORSAS.
