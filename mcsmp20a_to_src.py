#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# MCSMP20A.bin to rom_basic_const.c convert program 0.1
# for Pico2MEMEmuCosmac
# date: 2025/11/03
# Kazuhiro Ouchi @kanpapa
#
# Usage:
#        mcsmp20a_to_src.py MSMP20A.bin > rom_basic_const.c
#

import sys

# Get Binary file name
sfilename = sys.argv[1]

# Open binary file
f = open(sfilename, 'rb')

linecounter = 0
eof = False

# 
print("const unsigned char rom_basic[0x8000] = {")

# command buffer
command = ''
savecounter = 0

while not eof: 
    b = f.read(1)
    if not b:
        eof = True
    else:
        command = command + ' 0x' + b.hex().upper() + ','

    linecounter += 1
    if linecounter > 15 or eof == True:
        print(command + f" // 0x{savecounter:04X}")
        savecounter = savecounter + linecounter
        linecounter = 0
        command = ''

f.close();

print("};")
