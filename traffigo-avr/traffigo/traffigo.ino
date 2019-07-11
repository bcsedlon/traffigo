#include "Arduino.h"

int diffDistance = 1;
int diffDistanceTimeout = 500;
int toneDuration = 100;
int maxDistance = 100;

unsigned long lastDiffDistanceMillis;
unsigned long lastMillis;

const int buzzerPin = 8;
const int trigPin = 9;
const int echoPin = 10;

int distance;
int lastDistance;
#include "filter.h"
ExponentialFilter<float> distanceFiltered(16, 0);

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

void setup() {
	pinMode(buzzerPin, OUTPUT);
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	Serial.begin(9600);
	Serial.println("TRAFFIGO");
}

void loop() {
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);

	long duration;
	duration = pulseIn(echoPin, HIGH);
	distance = duration * 0.034 / 2;

	distanceFiltered.Filter(distance);
	distance = distanceFiltered.Current();

	// Prints the distance on the Serial Monitor
	Serial.print("Distance: ");
	Serial.println(distance);


	bool bBeep = true;
	if(abs(lastDistance - distance) < diffDistance + (distance / 20)) {
		if(millis() - lastDiffDistanceMillis > diffDistanceTimeout) {
			bBeep = false;
		}
	}
	else {
		lastDiffDistanceMillis = millis();
	}

	if(distance > maxDistance) {
		bBeep = false;
	}

	if(bBeep) {
		if(millis() - lastMillis > distance * 4) {
			lastMillis = millis();


			if(distance > lastDistance)
				tone(buzzerPin, c, toneDuration);
			else
				tone(buzzerPin, aH, toneDuration);


			lastDistance = distance;
		}
		else {
			noTone(buzzerPin);
		}
	}
	else {
		noTone(buzzerPin);
	}
}
