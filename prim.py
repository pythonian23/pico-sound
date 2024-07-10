#!/usr/bin/env python3

import serial
import mido
import time
import sys


dev = serial.Serial("/dev/ttyACM0", 115200)

song = []

port = mido.open_input()

try:
    for msg in port:
        dev.write(msg.bytes())
        sys.stdout.write(dev.read_all().decode("ascii"))
        if msg.type in ["note_on", "note_off"]:
            continue
        if msg.type == "control_change" and any((
                msg.control == 0x07,
                msg.control == 0x0b,
                msg.control == 0x78 and msg.value == 0x00,
                msg.control == 0x7b,
        )):
            continue
        print(msg, msg.bytes())
except KeyboardInterrupt:
    dev.write([0xf0, 0x78, 0x00])
    sys.stdout.write(dev.read_all().decode("ascii"))
    print("Goodbye")
