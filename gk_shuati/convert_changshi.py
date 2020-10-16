#!/usr/bin/python
#coding=utf-8
import pytesseract
import codecs
from PIL import Image
import sys

def is_start_token(string):
    if (u'例' in string) \
        or (u'A.' in string) \
        or (u'A' in string) \
        or (u'B.' in string) \
        or (u'C.' in string) \
        or (u'D.' in string):
        return True
    else:
        return False

def parse_png(png_name):
    image = Image.open(png_name)
    text = pytesseract.image_to_string(image,lang='chi_sim')
    text = text.replace(' ', '')
    text_array = text.split('\n')
    i = 0
    text_len = len(text_array)
    print_text = u''
    update_array = []

    for t in text_array:
        if not (u'' == t) and not (u'\x0c' == t):
            update_array.append(t)


    del update_array[0]
    for t in update_array:
        print_text = print_text + t + '\n'
    text_len = len(update_array)

    while (i < 0):
        if is_start_token(update_array[i]):
            while not (u'D' in update_array[i]) and (i < text_len):
                print_text = print_text + update_array[i] + '\n'
                i = i + 1
            if (u'D' in update_array[i]):
                print_text = print_text + update_array[i] + '\n\n'

        i = i + 1
    return print_text

def main():

    text_file = codecs.open('text.txt', 'w+', 'utf-8')
    start = int(sys.argv[1])
    end = int(sys.argv[2])
    for i in range(start, end):
        png_name = 'changshi/理论精讲(整合)-' + str(i).zfill(3) + '.png'
        print('parse ' + png_name + '...')
        text = parse_png(png_name)
        text_file.write(text)
        print text
    text_file.close()
main()
