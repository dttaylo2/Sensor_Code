#!/usr/bin/python2

# Import things.
import os, subprocess, sys, signal
import numpy as np
import time
from RF24 import *
from datetime import datetime, tzinfo, timedelta
import MySQLdb
import struct
import codecs
import pickle

model = pickle.load(open('model.pkl', 'rb'))

# Variables
# Testing commit update
currentIndex = 0
temperatureIndex = 1
vibrationIndex = 2
speedIndex = 3
accIndex = 4

# We are reading 5 bytes of one value, a comma, and five bytes of another for a total of 11 bytes.
dataSize = 30

def printDetails(data):
	parts = data.split(',')
        try:
	    print "Current: " + parts[currentIndex] + "A"
	    print "Temperature: " + parts[temperatureIndex] + "C"
	    print "Vibration: " + parts[vibrationIndex] + "Hz"
	    print "Speed: " + parts[speedIndex] + "RPM"
	    print "Acc: " + parts[accIndex]
	    print ""
        except IndexError:
            print "Bad packet"
	
	# Insert into DB
	sql = "INSERT INTO sensorData (currentVal, temperatureVal, vibrationVal, rpmVal) VALUES (%s, %s, %s, %s)"
	cursor.execute(sql, (parts[currentIndex], parts[temperatureIndex], parts[vibrationIndex], parts[speedIndex]))
	db.commit()
	
def dumpAccelerometer(acc1, acc2):
    
    sql = "INSERT INTO accelerometer (receivedTime, magnitude1, magnitude2) VALUES (%s, %s, %s)"
    cursor.execute(sql, (datetime.now(), acc1, acc2))
    db.commit()

def dumpSensors(temp, curr, rpm, acc1Avg, acc1Std, acc2Avg, acc2Std, classifier):
    sql = "INSERT INTO sensorData (temperatureVal, currentVal, rpmVal, receivedTime, acc1Avg, acc1Std, acc2Avg, acc2Std, classification) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)"
    cursor.execute(sql, (temp, curr, rpm, datetime.now(), acc1Avg, acc1Std, acc2Avg, acc2Std, classifier))
    db.commit()

# Connect
db = MySQLdb.connect("localhost", "root", "AdamPi331", "seniordesign")
cursor = db.cursor()

# Setup for GPI 22 CE and CE0 CSN with SPI Speed @ 4Mhz
radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ)

# Pipes are the ID numbers for the transceivers. This array exists in the arduino code as well.
pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2, 0xF0F0F0F0C3]

# Initial radio setup.
radio.begin()
radio.enableDynamicPayloads()
radio.printDetails()

# Sets the radio to listen on pipe 1 and read on pipe 0. Opposite of arduino code, naturally.
radio.openWritingPipe(pipes[1])
radio.openReadingPipe(1, pipes[0])
radio.openReadingPipe(2, pipes[2])
radio.startListening()

file = open('/dev/null', 'wa')
acc1Readings = []
acc2Readings = []

# Always listening
while 1:
	# If the radio is available, read from it.
	if radio.available():
		# While the radio is still available, read from it.
		while(radio.available()):
			# Print values to console.
                        val =  radio.read(radio.getDynamicPayloadSize())
                        #print val
                        file.write(val + '\n')
                        
                        if (str(val)[0] == '2'):
                            try:
                                print(str(val))
                                accelerometer1 = str(val).strip().split(',')[1]
                                accelerometer2 = str(val).strip().split(',')[2]
                                accelerometer1 = int(accelerometer1.split('\x00')[0])
                                accelerometer2 = int(accelerometer2.split('\x00')[0])

                                #sys.stdout.write(str(accelerometer) + '\r')
                                dumpAccelerometer(accelerometer1, accelerometer2)
                                acc1Readings.append(accelerometer1)
                                acc2Readings.append(accelerometer2)
                                
                            except KeyboardInterrupt:
                                sys.exit("User killed execution")
                            except:
                                print "Warning: something wrong"
                        elif (str(val)[0] == '1'):
                            try:
                                vals = str(val).strip().split(',')
                                print str(val)
                                curr = vals[1]
                                temp = vals[2]
                                rpm = vals[3]
                                if len(acc1Readings) > 0 and len(acc2Readings) > 0:
                                    acc1Avg = np.average(acc1Readings)
                                    acc1Std = np.std(acc1Readings)
                                    acc2Avg = np.average(acc2Readings)
                                    acc2Std = np.std(acc2Readings)
                                else:
                                    acc1Avg = 0
                                    acc1Std = 0
                                    acc2Avg = 0
                                    acc2Std = 0
                                data = np.array([curr, temp, rpm, acc1Avg, acc1Std]).reshape((1, 5))
                                prediction = model.predict_classes(data)
                                dumpSensors(temp, curr, rpm, acc1Avg, acc1Std, acc2Avg, acc2Std, 2)
                                acc1Readings = []
                                acc2Readings = []
                                #print('Sensors dumped')
                            except KeyboardInterrupt:
                                sys.exit("User killed execution")
                            except:
                                print "Warning: something wrong with sensor data"

                        #TODO: Print details
                        # printDetails(radio.read(dataSize))

			# TODO: I copied this code from last semester, not sure why radio needs to be reset. Will look into it.
			# radio.stopListening()
			# radio.startListening()
