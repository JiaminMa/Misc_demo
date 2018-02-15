import os


def pad_it(pos, padding):
    pad = (padding - (pos & (padding - 1))) & (padding - 1)
    print('Padding from {:x} to {:x} alignement with {:x} bytes\n'.format(pos, padding, pad))
    return pad


file1 = open("main.bin", "rb")
file2 = open("Img-Set0_SCFW_CM4_DDR_AP_DDR-eMMC_8bit_DDR.bin", "rb")
file3 = open("temp.bin", "wb")
file3.write(file1.read())

len_file1 = file1.tell()
pad = pad_it(len_file1, 0x1000)
print len_file1, pad

for i in range(0, pad):
    file3.write("\x5a")
file3.write(file2.read())
file1.close()
file2.close()
file3.close()
