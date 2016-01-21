# Import things.
import os, subprocess, sys, signal
import time
from RF24 import *
from datetime import datetime, tzinfo, timedelta

# Setup for GPI 22 CE and CE0 CSN with SPI Speed @ 4Mhz
radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ)

# Pipes are the ID numbers for the transceivers. This array exists in the arduino code as well.
pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

# Initial radio setup.
radio.begin()
radio.enableDynamicPayloads()
radio.printDetails()

# Sets the radio to listen on pipe 1 and read on pipe 0. Opposite of arduino code, naturally.
radio.openWritingPipe(pipes[1])
radio.openReadingPipe(1, pipes[0])
radio.startListening()

# Always listening
while 1:
	# If the radio is available, read from it.
	if radio.available():
		# While the radio is still available, read from it.
		while(radio.available()):
			# Read payload, print it to console, reset listener.
			# TODO: I copied this code from last semester, not sure why radio needs to be reset. Will look into it.
			receive_payload = ""
			payload = radio.getDynamicPayloadSize();
			print payload
			
			# We are reading 5 bytes of one value, a colon, and five bytes of another for a total of 11 bytes.
			receive_payload = radio.read(11)

			print receive_payload

			radio.stopListening()
			radio.startListening()
