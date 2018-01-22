int lottoPins[] = {2, 3, 4};
int pianoPins[] = {5, 6, 7, 8, 9}; 
unsigned long lastStateChange[11];
bool buttonStates[9];
int handleState = 0;
bool onOffState = true;

void setup() {
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	pinMode(10, OUTPUT);
	digitalWrite(10, LOW);
	pinMode(11, OUTPUT);
	digitalWrite(11, LOW);
	for (int i = 0; i < 11; i++) {
		lastStateChange[i] = millis();
		if (i < 9) {
			buttonStates[i] = false;
		}
	}
	for (int p = 0; p < 3; p++) {
		pinMode(lottoPins[p], INPUT);
	}
	for (int p = 0; p < 5; p++) {
		pinMode(pianoPins[p], INPUT);
	}
	pinMode(12, INPUT);
	
	Serial.begin(31250); 
}

bool okToTurnOff = false;
bool stillTurningOn = true;
void loop() {
	bool currentOnOffState = digitalRead(12);
	if (currentOnOffState != onOffState) {
		lastStateChange[10] = millis();
		onOffState = currentOnOffState;
	}
	if ((millis() - lastStateChange[10] > 50) && (onOffState == true) && (stillTurningOn == true)) {
		digitalWrite(10, HIGH);
		stillTurningOn = false;
		lastStateChange[10] = millis();
	}
	if ((millis() - lastStateChange[10] > 10) && (onOffState == false) && (stillTurningOn == false) && (okToTurnOff == false)) {
		digitalWrite(11, HIGH);
		okToTurnOff = true;
		lastStateChange[10] = millis();
	}
	if ((millis() - lastStateChange[10] > 100) && (onOffState == true) && (okToTurnOff == true)) {
		digitalWrite(13, HIGH);
		lastStateChange[10] = millis();
	}
	
	for (int p = 0; p < 3; p++) {
		bool state = digitalRead(lottoPins[p]);
		if (state != buttonStates[lottoPins[p]] && millis() - lastStateChange[lottoPins[p]] > 10) {
			lastStateChange[lottoPins[p]] = millis();
			buttonStates[lottoPins[p]] = state;
			lottoFunction(p, state);
		}
	}
	for (int p = 0; p < 5; p++) {
		bool state = digitalRead(pianoPins[p]);
		if (state != buttonStates[pianoPins[p]] && millis() - lastStateChange[pianoPins[p]] > 10) {
			lastStateChange[pianoPins[p]] = millis();
			buttonStates[pianoPins[p]] = state;
			pianoFunction(p, state);
		}
	}
	int state = analogRead(5);
	state /= 8;
	if (state != handleState && millis() - lastStateChange[9] > 10) {
		handleState = state;
		lastStateChange[9] = millis();
		handleFunction(state);

	}
}	 

void lottoFunction(int buttonNum, bool state) {
	int lottoNotes[] = {102, 103, 104};
	Serial.print(char(0b10110001));
	Serial.print(char(lottoNotes[buttonNum]));
	if (state == true) {
		Serial.print(char(0));
	}
	else {
		Serial.print(char(100));
	}

}

void pianoFunction(int buttonNum, bool state) {
	int pianoNotes[] = {38, 36, 40, 43, 41};
	Serial.print(char(0b10010001));
	Serial.print(char(pianoNotes[buttonNum]));
	if (state == true) {
		Serial.print(char(0));
	}
	else {
		Serial.print(char(100));
	}
}

void handleFunction(int veloc) {
	Serial.print(char(0b10110001));
	Serial.print(char(105));
	if (veloc < 40) {
		veloc = 40;
	}
	if (veloc > 81) {
		veloc = 85;
	}
	Serial.print(char(map(veloc, 40, 81, 0, 127)));
}
