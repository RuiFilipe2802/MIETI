from datetime import date, datetime
import socket
import sys
import struct
from tkinter import *
from tkinter.ttk import *
from tkinter.filedialog import askopenfile 
from time import sleep
import time
from turtle import clear

ws = Tk()
ws.title('PITI')
ws.geometry('500x250') 
packet = bytearray(2)
serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def open_file():
    global packet,fileContent
    file_path = askopenfile(mode='r', filetypes=[('Txt files', '.txt')])
    fileName = file_path.name
    if file_path is not None:
        fileOpen = open(fileName, "rb").read()
        fileContent = bytearray(fileOpen)
        packet[0] = 0b1
        packet[1] = 0b1
        packet.extend(fileContent)


def uploadFiles():
    pb1 = Progressbar(ws,orient=HORIZONTAL, length=200, mode='determinate')
    pb1.grid(row=4, columnspan=3, pady=10)
    # for i in range(5):
    #     ws.update_idletasks()
    #     pb1['value'] += 20
    #     time.sleep(1)
    pb1.destroy()
    Label(ws, text='File Uploaded Successfully!', foreground='green').grid(row=4, columnspan=3, pady=10)
    print(packet)
    serv.connect(("192.168.161.151",80))

    while True:
        from_client = ''
        serv.send(packet)
        sleep(5)
        break


if __name__ == "__main__":
    dl = Label(ws,text='Upload txt file')
    dl.grid(row=1, column=0, padx=100)
    dlbtn = Button(ws,text ='Choose File ',command = lambda:open_file()) 
    dlbtn.grid(row=1, column=1)
    upld = Button(ws,text='Upload Files',command=uploadFiles)
    upld.grid(row=3, columnspan=3, pady=10)

    ws.mainloop()
