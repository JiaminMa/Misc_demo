import os
import sys

f = open('dsdt.txt')
out = open('dsdt.dat', 'wb+')
for line in f:
    line = line.replace('\n', '')
    val = int(line, 16) 
    print(val)
    out.write(val.to_bytes(4,  byteorder='little'))

f.close()
out.close()

os.system('iasl -d dsdt.dat')
