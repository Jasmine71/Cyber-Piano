import pygame
import sys
from pygame.locals import *
import serial
import time
import random

pygame.init()

pygame.mixer.init()

notes = {
    'e': 'do.wav',
    'r': 're.wav',
    't': 'mi.wav',
    'y': 'fa.wav',
    'u': 'sol.wav',
    'i': 'la.wav',
    'o': 'si.wav'
}

screen = pygame.display.set_mode((640, 480))
# Setup the serial connection (COM port may vary, adjust as needed)
portName = "/dev/cu.usbserial-1110"
# portName = "COM4"
ser = serial.Serial(portName, 9600, timeout=1)
time.sleep(2)  # wait for the connection to initialize
volume = 0.5
 
while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        if event.type == KEYDOWN:
            ser.write(b'1')  # send a byte to signal Arduino to start88
            time.sleep(0.1)
            line = ser.readline().decode().strip()
            if line.isdigit():
                pressValue = int(line)
                volume = pressValue / 255.0
            #     print(volume)
            time.sleep(0.1)  # prevent multiple signals on a single press
            # print(f"Key {key} pressed, signal sent to Arduino.")
            key = pygame.key.name(event.key)
            if key in notes:
                ser.write(key.encode('utf-8'))
                volume = random.uniform(0.3, 1.0)
                print(volume)
                sound = pygame.mixer.Sound(notes[key])
                sound.set_volume(volume)
                sound.play()
                # send the pressed key using ser.write()


    pygame.display.update()
