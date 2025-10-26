#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# Binary to Hex-code convert program 0.1
# for EMU1802
# date: 2022/06/03
# Kazuhiro Ouchi @kanpapa
#
# Usage:
#        bin2hexsrc.py <Binary filename>
#

import sys

# Get Binary file name
sfilename = sys.argv[1]

# Open binary file
f = open(sfilename, 'rb')

linecounter = 0
eof = False

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
