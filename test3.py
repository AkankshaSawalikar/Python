import pygame
from pygame.locals import *
from pygame import *

from serial import Serial
import sys

# It's best to have them be multiples of 6
width = 600
height = 600

pygame.init()

serialPort = "COM9"
ser = Serial(serialPort, baudrate=9600)

screen = pygame.display.set_mode((width, height), HWSURFACE | DOUBLEBUF)

def mainLoop():
    if ser.in_waiting > 0:
        byte_str = ser.read_until()
        read_str = byte_str.decode("utf-8")
        print(read_str)
        if "TABLE" in read_str:
            print("table found")
            for row in range(6):
                byte_str = ser.read_until()
                read_str = byte_str.decode("utf-8")
                read_str.replace(" ", "")
                val_list = read_str.split('|')
                print(val_list)
                for col, val in enumerate(val_list):
                    colorVal = map(int(val), 0, 1600, 0, 255)
                    if colorVal > 0:
                        colorVal = 255
                    rect = pygame.Rect((col * width / 6 , row * height / 6), (width / 6 - 1, height / 6 - 1))
                    #1rect = pygame.Rect(100,100,200,200)
                    pygame.draw.rect(screen, (colorVal, 0, 255 - colorVal), rect)
            pygame.display.update()
    for event in pygame.event.get():
        if event.type == KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                print("exiting")
                pygame.display.update()

                pygame.quit()
                ser.close()
                sys.exit()
                

def map(val, in_min, in_max, out_min, out_max):
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

while(True):
    mainLoop()