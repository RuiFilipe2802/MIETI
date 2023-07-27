#!/bin/sh
python3 transmitter.py 16 1 firstSignal.txt
python3 transmitter.py 16 2 secondSignal.txt
python3 transmitter.py 16 3 thirdSignal.txt
python3 channel.py
python3 receiver.py
