#!/usr/bin/python
# coding=utf-8
from bs4 import BeautifulSoup
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import xlrd
import smtplib
from email.mime.text import MIMEText
from email.header import Header

class TEST():

    USDHC = 'USDHC'
    MISC = 'MISC'
    FLEXSPI_NOR = 'FLEXSPI_NOR'
    FLEXSPI_NAND = 'FLEXSPI_NAND'
    USB = 'USB'
    LPSPI_NOR = 'LPSPI_NOR'
    DUAL = 'DUAL'
    FUSE = 'FUSE'

    USDHC_IDX = 0
    FLEXSPI_NOR_IDX = 1
    FLEXSPI_NAND_IDX = 2
    USB_IDX = 3
    LPSPI_NOR_IDX = 4
    MISC_IDX = 5
    DUAL_IDX = 6
    FUSE_IDX = 7
    UNKNOWN_IDX = 8

    TYPE_NUM = 8

    def __init__(self):
        self.name = ''
        self.status = ''
        self.log = 'Not Exist'
        self.type = 'Unknown'
        self.owner = ''

    def dump(self):
        print('========')
        print('    name:{}'.format(self.name))
        print('    status:{}'.format(self.status))
        print('    log:{}'.format(self.log))
        print('    type:{}'.format(self.type))
        print('    owner:{}'.format(self.owner))

    def handle_test(self, owner_dict):
        if ('UT-USDHC' in self.name):
            self.type = self.USDHC
        if (('Single_Internal-USDHC' in self.name) or ('Single-USDHC' in self.name) ):
            self.type = self.USDHC
        if ('UT-USB' in self.name):
            self.type = self.USB
        if ('MISC-' in self.name):
            self.type = self.MISC
        if ('Single_Internal-FlexSPI2_NAND' in self.name):
            self.type = self.FLEXSPI_NAND
        if ('Single-FlexSPI2_NAND' in self.name):
            self.type = self.FLEXSPI_NAND
        if ('UT-FlexSPI0_NAND' in self.name):
            self.type = self.FLEXSPI_NAND
        if ('UT-FlexSPI2_NAND' in self.name):
            self.type = self.FLEXSPI_NAND
        if ('UT-M33-LPSPI' in self.name):
            self.type = self.LPSPI_NOR
        if ('UT-A35-LPSPI' in self.name):
            self.type = self.LPSPI_NOR
        if ('-LPSPI' in self.name):
            self.type = self.LPSPI_NOR
        if ('UT-FlexSPI2_NOR' in self.name):
            self.type = self.FLEXSPI_NOR
        if ('UT-FlexSPI0_NOR' in self.name):
            self.type = self.FLEXSPI_NOR
        if ('Single_Internal-FlexSPI2_NOR' in self.name):
            self.type = self.FLEXSPI_NOR
        if ('Dual-' in self.name):
            self.type = self.DUAL
        if ('Dual_Internal' in self.name):
            self.type = self.DUAL
        if ('Serial_Download-USB' in self.name):
            self.type = self.USB
        if ('LP_Internal-CM33_FlexSPI' in self.name):
            self.type = self.FLEXSPI_NOR
        if ('SD_Recovery' in self.name):
            self.type = self.USDHC
        if ('LPSPI_Recovery' in self.name):
            self.type = self.LPSPI_NOR
        if ('SDP' in self.name):
            self.type = self.USB
        if ('FUSE_TEST' in self.name):
            self.type = self.FUSE
        if ('UT-Exception' in self.name):
            self.type = self.MISC

        self.owner = owner_dict[self.name]


def get_test_array(test_array):
    test_dict = {}
    fp = open('./8ulp_test.html', 'r')
    soup = BeautifulSoup(fp, 'lxml')
    test_item = soup.find('table', class_ = 'sortable sml summary_table')
    for child in test_item.children:
        test_case = child.find('td')
        if (test_case == None or test_case == -1):
            continue

        test = TEST()
        test.name = child.find('td', 'STRING_cell').text.replace(' ', '').replace('\n', '') \
                .replace('/rtl_rom_default/rtl_rom_default_SYS_BOOT/rtl_rom_default_sys_boot_vectors_set/', '')

        if child.find('td', class_ = 'range100') != None:
            test.status = 'passed'
        else:
            test.status = 'failed'

        logs = child.find('td', class_ = 'FILE_cell')
        for log in logs:
            l = str(log)
            if 'vm_brun.log' in l:
                continue

            if '<br/>' == l:
                continue

            if 'local_log.log' in l:
                continue

            if '\n' == l:
                continue

            l = l.replace('<a href="', '').replace('</a>', '').replace(' ', '').replace('\n', '')
            ls = l.split('/')
            num = len(ls)
            s = ls[num - 1]
            test.log = s
        test_dict[test.name] = test

    for ti in test_dict.values():
        test_array.append(ti)

def show_result(test_array):


    size = TEST.TYPE_NUM
    success_array = size * [0]
    failure_array = size * [0]
    success_cnt = 0;
    fail_cnt = 0;

    for test in test_array:
        test.dump();
        if (test.status == 'passed'):
            if test.type == TEST.USDHC:
                success_array[TEST.USDHC_IDX] = success_array[TEST.USDHC_IDX] + 1
            elif test.type == TEST.FLEXSPI_NOR:
                success_array[TEST.FLEXSPI_NOR_IDX] = success_array[TEST.FLEXSPI_NOR_IDX] + 1
            elif test.type == TEST.FLEXSPI_NAND:
                success_array[TEST.FLEXSPI_NAND_IDX] = success_array[TEST.FLEXSPI_NAND_IDX] + 1
            elif test.type == TEST.LPSPI_NOR:
                success_array[TEST.LPSPI_NOR_IDX] = success_array[TEST.LPSPI_NOR_IDX] + 1
            elif test.type == TEST.USB:
                success_array[TEST.USB_IDX] = success_array[TEST.USB_IDX] + 1
            elif test.type == TEST.MISC:
                success_array[TEST.MISC_IDX] = success_array[TEST.MISC_IDX] + 1
            elif test.type == TEST.DUAL:
                success_array[TEST.DUAL_IDX] = success_array[TEST.DUAL_IDX] + 1
            elif test.type == TEST.FUSE:
                success_array[TEST.FUSE_IDX] = success_array[TEST.FUSE_IDX] + 1
            else:
                success_array[TEST.UNKNOWN_IDX] = success_array[TEST.UNKNOWN_IDX] + 1
            success_cnt = success_cnt + 1
        else:
            if test.type == TEST.USDHC:
                failure_array[TEST.USDHC_IDX] = failure_array[TEST.USDHC_IDX] + 1
            elif test.type == TEST.FLEXSPI_NOR:
                failure_array[TEST.FLEXSPI_NOR_IDX] = failure_array[TEST.FLEXSPI_NOR_IDX] + 1
            elif test.type == TEST.FLEXSPI_NAND:
                failure_array[TEST.FLEXSPI_NAND_IDX] = failure_array[TEST.FLEXSPI_NAND_IDX] + 1
            elif test.type == TEST.LPSPI_NOR:
                failure_array[TEST.LPSPI_NOR_IDX] = failure_array[TEST.LPSPI_NOR_IDX] + 1
            elif test.type == TEST.USB:
                failure_array[TEST.USB_IDX] = failure_array[TEST.USB_IDX] + 1
            elif test.type == TEST.MISC:
                failure_array[TEST.MISC_IDX] = failure_array[TEST.MISC_IDX] + 1
            elif test.type == TEST.DUAL:
                failure_array[TEST.DUAL_IDX] = failure_array[TEST.DUAL_IDX] + 1
            elif test.type == TEST.FUSE:
                failure_array[TEST.FUSE_IDX] = failure_array[TEST.FUSE_IDX] + 1
            else:
                failure_array[TEST.UNKNOWN_IDX] = failure_array[TEST.UNKNOWN_IDX] + 1
            fail_cnt = fail_cnt + 1

    print success_array
    lables = ['USDHC', 'FSPI_NOR', 'FSPI_NAND', 'USB', 'SPI-NOR', 'MISC', 'DUAL', 'FUSE']
    colors = ['cornflowerblue','cornflowerblue', 'cornflowerblue','cornflowerblue', 'cornflowerblue', 'cornflowerblue', 'cornflowerblue','cornflowerblue']
    color2 = ['lightpink','lightpink', 'lightpink','lightpink', 'lightpink', 'lightpink', 'lightpink','lightpink']

    plt.figure(1, figsize=(10,8))
    plt.bar(range(0, size), success_array, label = 'PASS', tick_label=lables, color=colors)
    plt.xticks(range(0, size), lables, fontsize = 8)
    plt.title('Pattern Overview')
    plt.bar(range(0, size), failure_array, bottom = success_array, label = 'FAIL', color=color2)

    for x, y in zip(range(0, size), success_array):
        if (y != 0):
            plt.text(x, y / 2, str(y), ha = 'center', fontsize = 7)

    for x, y, z in zip(range(0, size), failure_array, success_array):
        if (y != 0):
            plt.text(x, y / 3 + z , str(y), ha = 'center', fontsize = 7)


    #plt.legend()
    plt.savefig('overview.png')
    #plt.show()

    pie_labels = ['PASS', 'FAIL']
    sizes = [success_cnt, fail_cnt]
    explode = [0, 0]
    colors = ['cornflowerblue', 'lightpink']
    plt.figure(2)
    plt.pie(sizes, explode = explode, labels = pie_labels, colors = colors, autopct='%1.1f%%', startangle=50)
    plt.axis('equal')
    plt.title('Pattern Pass Rate')
    plt.savefig('pass_rate.png')
    print('show_result done')

def get_sys_boot_dict(owner_dict):
    data = xlrd.open_workbook('owner.xlsx')
    table = data.sheet_by_name('SYS_BOOT')
    for i in range(0, table.nrows):
        owner_dict[table.cell(i, 0).value] = table.cell(i, 22).value

def send_mail():
    sender = 'jiamin.ma_2@nxp.com'
    receivers = ['jiamin.ma_2@nxp.com']

    mail_host="smtp-mail.outlook.com"
    mail_user="jiamin.ma_2@nxp.com"

    mail_msg = open('8ulp_result.html').read()
    message = MIMEText(mail_msg, 'html', 'utf-8')
    message['From'] = sender
    message['To'] =  ";".join(receivers)

    subject = 'Python SMTP Test'
    message['Subject'] = Header(subject, 'utf-8')

    try:
        smtpObj = smtplib.SMTP()
        smtpObj.set_debuglevel(1)
        smtpObj.connect('smtp-mail.outlook.com', 587)
        smtpObj.ehlo()
        smtpObj.starttls()
        smtpObj.login(mail_user,mail_pass)
        smtpObj.sendmail(sender, receivers, message.as_string())
    except smtplib.SMTPException:
        print('send mail fail')

def create_html(test_array):

    print('create_html entry')
    # Create two picture
    html_file = open('8ulp_result.html', 'w+')

    html = ''
    html = '<div align="left">' \
            '<img src="overview.png" />' \
            '</div>' \
            '' \
            '<div align="left">' \
            '<img src="pass_rate.png" />' \
            '</div>'
    html_file.write(html)


    print('create failure talbe')
    html_file.write('<div align="left">\n')
    html_file.write('<table border="0">\n')
    html_file.write('	<tr>\n')
    html_file.write('		<th>Failed Pattern Name</th>\n')
    #html_file.write('		<th>Owner</th>\n')
    #html_file.write('		<th>Log</th>\n')
    html_file.write('	</tr>\n')

    for test in test_array:
        if test.status == 'failed':
            html_file.write('	<tr>\n')
            html_file.write('		<td>{}</td>\n'.format(test.name))
            #html_file.write('		<td>{}</td>\n'.format(test.owner))
            #html_file.write('		<td>{}</td>\n'.format(test.log))
            html_file.write('	<tr>\n')

    html_file.write('</table>')
    html_file.write('</div>')

    html_css = '\n<style type="text/css">body { margin:0;padding:20px; font:13px "Lucida Grande", "Lucida Sans Unicode", Helvetica, Arial, sans-serif;}p,table, caption, td, tr, th {margin:0;padding:0;font-weight:normal;}p {margin-bottom:15px;}table {border-collapse:collapse;margin-bottom:15px;width:90%;}caption {text-align:left;font-size:15px;padding-bottom:10px;}table td,table th {padding:5px;border:1px solid #fff;border-width:0 1px 1px 0;}thead th {background:#6699FF;}thead th[colspan],thead th[rowspan] {background:#6699FF;}tbody th,tfoot th {text-align:left;background:#6699FF;}tbody td,tfoot td {text-align:center;background:#F2F3F4 ;}tfoot th {background:#F2F3F4 ;}tfoot td {background:#F2F3F4 ;font-weight:bold;}tbody tr.odd td { background:#F2F3F4 ;}</style>'
    html_file.write(html_css)
    html_file.close()

def main():

    owner_dict = {}
    get_sys_boot_dict(owner_dict)

    test_array = []
    get_test_array(test_array)

    for test in test_array:
        test.handle_test(owner_dict)
        #test.dump()

    show_result(test_array)

    create_html(test_array)

    #send_mail()


if __name__ == '__main__':
    main()
