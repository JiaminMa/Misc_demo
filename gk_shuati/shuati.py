#!/usr/bin/python3
#coding=utf-8
import os
import json
import sys

class Question():

    def __init__(self, question, answer):
        self.question = question
        self.answer = answer
        self.is_correct = False
        self.your_answer = ''

def start_to_answer(q_array):

    os.system('clear')
    for q in q_array:
        print(q.question)
        a = input('输入答案:')
        a = a.replace('1', 'A').replace('2', 'B').replace('3', 'C').replace('4', 'D').replace('5', 'E')
        q.your_answer = a
        if a == q.answer:
            q.is_correct = True
        os.system('clear')

def report_wrong_answer(q_array):

    wrong_ratio = 0.0
    report = open('report.txt', 'w+')
    all_num = len(q_array)
    for q in q_array:
        if q.is_correct == False:
            report.write(q.question)
            report.write('\n')
            report.write('你的答案:{} 正确答案:{}\n\n'.format(q.your_answer, q.answer))
            wrong_ratio = wrong_ratio + 1.0
    report.write('错误率:{}%\n'.format(wrong_ratio / all_num * 100))
    report.close()

def main():

    if (len(sys.argv) < 2):
        print('请输入题库文件...')
        return

    tiku = sys.argv[1]
    json_file = open(tiku, 'r')
    json_data = json_file.read()
    json_file.close()

    json_objects = json.loads(json_data)
    q_array = []
    for t in json_objects['tiku']:
        question = t['Q']
        answer = t['A']
        q = Question(question, answer)
        q_array.append(q)

    start_to_answer(q_array)
    report_wrong_answer(q_array)

main()
