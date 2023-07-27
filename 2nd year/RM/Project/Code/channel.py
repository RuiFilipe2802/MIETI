import numpy as np

def fun_menos1(array):
    x = 0
    while x < len(array):
        if(array[x] == 0):
            array[x] = -1
        x = x + 1
    return array


def getAttributes(ficheiro):
    lines = ficheiro.readlines()
    count = 0
    for line in lines:
        count += 1
        if(count % 4 == 1):   # 1st line  (sinal)
            cdma = lines[count-1].strip().split(',')
        if(count % 4 == 2):   # 1st line  (sinal)
            message = lines[count-1].strip().split(',')
        if(count % 4 == 3):   # 3rd line  (chip)
            chip = lines[count-1].strip().split(',')
        if(count % 4 == 0):   # 4th line  (fe)
            fe = lines[count-1]

    return cdma, message, chip, fe


if __name__ == "__main__":

    ficheiros = []
    f_atenuacao = []
    configFile = "configFile.txt"
    output = ''
    nFile = 0
    desvio = 0
    size = 0
    noise = ''

    with open(configFile, "r") as filestream:                         # OPEN CONFIG FILE
        counter = 0
        for line in filestream:
            counter += 1
            if(counter % 5 == 1):   # 1st line
                ficheiros = line.strip().split(",")
            if(counter % 5 == 2):   # 2nd line
                f_atenuacao = line.strip().split(",")
            if(counter % 5 == 3):   # 3rd line
                output = line.strip()
            if(counter % 5 == 4):   # 4th line
                desvio = float(line.strip())
            if(counter % 5 == 0):   # 4th line
                size = int(line.strip())

    stream = open(output, "w")
    finalSignal = [0] * 100
    sinaisSomados = [0] * size
    count = 0
    for i in ficheiros:
        # OPEN FILES FROM CONFIG FILE
        with open(ficheiros[nFile], "r") as filestream:
            cdma, message, chip, fe = getAttributes(filestream)
            sinalAtenuado = [0] * len(cdma)
            counter = 0
            for x in cdma:
                sinalAtenuado[counter] = float(
                    f_atenuacao[nFile]) * float(cdma[counter])
                counter += 1
            x = 0
            while x < len(cdma):
                sinaisSomados[x] = sinalAtenuado[x] + sinaisSomados[x]
                x += 1
        nFile += 1
    noise = np.random.normal(0, desvio, len(cdma))
    finalSignal = sinaisSomados + noise
    x = 1
    for listitem in finalSignal:
        if(len(finalSignal) == x):
            stream.write('%s' % listitem)
            break
        stream.write('%s,' % listitem)
        x = x + 1
    stream.write('\n')

    nFile = 0
    for i in ficheiros:
        with open(ficheiros[nFile], "r") as filestream:
            cdma, message, chip, fe = getAttributes(filestream)
            x = 1
            for listitem in message:
                if(len(message) == x):
                    stream.write('%s' % listitem)
                    break
                stream.write('%s,' % listitem)
                x = x + 1
            stream.write('\n')
            x = 1
            for listitem in chip:
                if(len(chip) == x):
                    stream.write('%s' % listitem)
                    break
                stream.write('%s,' % listitem)
                x = x + 1
            x = 1
            stream.write('\n')
            stream.write(str(fe))
            stream.write('\n')
        nFile += 1
