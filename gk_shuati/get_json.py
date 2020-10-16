#!/usr/bin/python
#coding=utf-8
import codecs
import sys

def main():
    output_json = sys.argv[1]
    output_file = codecs.open(output_json, 'w+', 'utf-8')
    text_file = codecs.open('text.txt', 'r', 'utf-8')
    text_array = text_file.read().split('\n')

    output_file.write('{\n')
    output_file.write('\t"tiku": [\n')

    q_text = '\t\t"Q": "'
    for t in text_array:
        if (u'' == t):
            q_text = q_text + '",\n'
            output_file.write('\t\t{\n')
            output_file.write(q_text)
            output_file.write('\t\t "A": ""\n')
            output_file.write('\t\t},\n\n')
            print('#######')
            q_text = '\t\t"Q": "' 
        else:
            q_text = q_text + t + '\\n'

    output_file.write('\t]\n')
    output_file.write('}\n')
    text_file.close()
    output_file.close()

main()
