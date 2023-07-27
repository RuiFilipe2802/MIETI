import random
import sys
from scipy.linalg import hadamard
import numpy as np

def fun_menos1(array):
    x = 0
    while x < len(array):
        if(array[x] == 0):
            array[x] = -1
        x = x + 1
    return array

def fun_zeros(array):
    x = 0
    while x < len(array):
        if(array[x] == -1):
            array[x] = 0
        x = x + 1
    return array

def retirar_menos1(array):
    x = 0
    while x < len(array):
        if(array[x] == -1):
            array[x] = 0
        x = x + 1
    return array

def pseudo_generator_message(lenArray):
    array = [0]*lenArray
    x = 0
    while x < lenArray:
        array[x] = random.randint(0, 1)
        x = x + 1
    return array

def mult_array(bitAr, chip, fe):
    arraym = [0]*len(bitAr)*fe
    x = 0
    y = 0
    k = 0
    l = 0
    while x < len(bitAr):
        while y < fe:
            arraym[l] = bitAr[x] * chip[k + y]
            y = y + 1
            l = l + 1
        k = k + y
        y = 0
        x = x + 1
    return arraym

def chip_sizeM(chip, fe, lm):
    chipfe = [0]*fe * lm
    x = 0
    y = 0
    while x < len(chipfe):
        if(y == len(chip)):
            y = 0
        chipfe[x] = chip[y]
        x = x + 1
        y = y + 1
    return chipfe

def walshCodes(fe, row):
    H = hadamard(fe)
    row_array = H[row]
    #print(row_array)
    return row_array

if __name__ == "__main__":
    bitArray = pseudo_generator_message(1000)
    bitArraymenos1 = bitArray
    fe = int(sys.argv[1])
    bitArraymenos1 = fun_menos1(bitArraymenos1)
    row = int(sys.argv[2])
    #chip = pseudo_generator_message(20)
    chip = walshCodes(fe, row)
    chip = fun_menos1(chip)
    chip_ficheiro = chip
    chip_tamanho = chip_sizeM(chip, fe, len(bitArray))
    chip_save_file = fun_zeros(chip)
    chip = fun_menos1(chip_tamanho)
    toOpen = sys.argv[3]
    cdma = mult_array(bitArraymenos1, chip, fe)
    bitArrayFile = retirar_menos1(bitArraymenos1)

    with open(toOpen, 'w') as filehandle:
        x = 1
        for listitem in cdma:
            if(len(cdma) == x):
                filehandle.write('%s' % listitem)
                break
            filehandle.write('%s,' % listitem)
            x = x + 1
        filehandle.write('\n')
        x = 1
        for listitem in bitArrayFile:
            if(len(bitArrayFile) == x):
                filehandle.write('%s' % listitem)
                break
            filehandle.write('%s,' % listitem)
            x = x + 1
        filehandle.write('\n')
        x = 1
        for listitem in chip_save_file:
            if(len(chip_save_file) == x):
                filehandle.write('%s' % listitem)
                break
            filehandle.write('%s,' % listitem)
            x = x + 1
        filehandle.write('\n')
        x = 1
        filehandle.write(str(fe))