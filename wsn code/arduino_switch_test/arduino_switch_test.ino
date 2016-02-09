// Certain libraries are required for this. Scroll to bottom of page at http://blog.carr3r.com/post/raspberry-pi-b-and-arduino-talking-through-nrf24l01-modules/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define RF_CE 7
#define RF_CSN 8
#define SWITCH_PIN 2

// Setup radio
RF24 radio(RF_CE, RF_CSN);

// Radio pipe addresses for 2 nodes to communicate.
const uint64_t pipes[2] = {
	0xF0F0F0F0E1LL,
	0xF0F0F0F0D2LL
};

const int max_payload_size = 32;

int val = 0;

void setup(void) {
	// Setup switch input
	pinMode(SWITCH_PIN, INPUT);
  digitalWrite(SWITCH_PIN, HIGH);

	// Start radio
	radio.begin();

	// Enable dynamic payloads
	radio.enableDynamicPayloads();

	// Write to pipe 0
	radio.openWritingPipe(pipes[0]);

	// Read on pipe 1.
	radio.openReadingPipe(1, pipes[1]);
	radio.stopListening();
}

void loop(void) {
	// Send switch payload warning
	static char send_payload[] = "Switch pressed!";

	// Read input
	val = digitalRead(SWITCH_PIN);
	if(val == HIGH) {
		radio.write(send_payload, max_payload_size);
		val = LOW;
		delay(3000);
	}
}
