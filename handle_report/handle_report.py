#!/usr/bin/python
import sys

def main():

    input_file = sys.argv[1]
    temp_file = 'temp.html'

    input_f = open(input_file, 'r')
    temp_f = open(temp_file, 'w')

    for line in input_f:
        if not ('log' in line):
            temp_f.write(line)

    input_f.close()
    temp_f.close()

main()
