import numpy as np
from scipy.stats import spearmanr
from scipy.stats.stats import SpearmanrResult

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

def mult_array(arr, chip):
    arraym = [0]*len(arr)
    x = 0
    y = 0
    k = 0
    tam = 0
    while x < len(arr):
        while y < 1:
            if(tam == len(chip)):
                tam = 0
            arraym[k] = float(arr[x]) * float(chip[tam + y])
            y = y + 1
            k = k + 1
        tam = tam + 1
        y = 0
        x = x + 1
    return arraym

def rotateChip(chip, posicao):
    array = np.array(chip)
    array_new = np.roll(array, posicao)
    return array_new

if __name__ == "__main__":
    channel = "channel.txt"
    with open(channel, "r") as filestream:
        lines = filestream.readlines()
        count = 0
        firstTime = 0
        message = []
        chip = []
        fe = []

        for line in lines:
            count += 1
            if(firstTime == 0):
                if(count % 4 == 1):   # 1st line  (sinal)
                    sinal = lines[count-1].strip().split(',')
                if(count % 4 == 2):   # 2nd line  (message)
                    message.append(lines[count-1].strip().split(','))
                if(count % 4 == 3):   # 3rd line  (chip)
                    chip.append(lines[count-1].strip().split(','))
                if(count % 4 == 0):   # 4th line  (fe)
                    fe.append(int(lines[count-1].strip()))
                    firstTime = 1
                    count = 4
            else:
                if(count % 3 == 2):   # 5th/8th/... line  (message)
                    message.append(lines[count-1].strip().split(','))
                if(count % 3 == 0):   # 6th/9th/... line  (chip)
                    chip.append(lines[count-1].strip().split(','))
                if(count % 3 == 1):   # 7th/10th/... line  (fe)
                    fe.append(int(lines[count-1].strip()))

    #posicao = 0
    #while posicao < 16:
        soma = 0
        x = 0
        multChip = []
        while(x < len(chip)):
            integer_map = map(float, chip[x])
            integer_list = list(integer_map)
            chipMinus1 = fun_menos1(integer_list)
            #chip_rotate = rotateChip(chipMinus1,posicao)
            multChip.append(mult_array(sinal, chipMinus1))
            x += 1
        q = 0
        x = 0
        array_media = []
        while q < len(message):
            array = multChip[q]
            finalArray = []
            count = 0
            fim = len(array)
            f = 0
            while f < len(array):
                t = 0
                while t < fe[0]:
                    soma = float(soma) + float(array[f+t])
                    t = t + 1
                if(soma > 0):
                    finalArray.append(1)
                else:
                    finalArray.append(0)
                soma = 0
                f = f + fe[0]

            erros = 0
            mess = message[q]
            r = 0
            while r < len(finalArray):
                if(str(finalArray[r]) != str(mess[r])):
                    erros += 1
                r = r + 1
        
            print("---- Sinal "+str(q+1)+" ----")
            #print("chip atrasado:"+str(posicao))
            print("Erros:"+str(erros))
            print("BER:"+str(erros/1000))
            array_media.append((erros/1000))
            #coef, p = spearmanr(mess, finalArray)
            #print('Spearman Coefficient:%.3f' % coef)
            q = q + 1
        print('-----------------')

        #posicao = posicao + 1
    print("BER mean:"+str(np.mean(array_media)))