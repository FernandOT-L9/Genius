// ================== VARIABLES ================== //

// LEDS //
#define LED_RED   2
#define LED_YELOW 3
#define LED_GREEN 4
#define LED_BLUE  5
#define LED_WHITE 10

// BUTTONS //
#define BTN_RED  6
#define BTN_YELOW 7
#define BTN_GREEN 8
#define BTN_BLUE 9

// ================== FUNCTIONS PROTOTYPE ================== //

void playLedSequence (int *sequence, int elements, int mode);
int  getColor        (int *buttons);
void readButton      (int button, int pressed);
int  countElements   (int *array, int length);
int  toPlay          (int *player, int *sequence, int difficulty);
void clearArray      (int *player, int elements);
int  setDifficulty   ();

// ================== MAIN PROGRAM ================== //
void setup() {
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_YELOW, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	pinMode(LED_WHITE, OUTPUT);
	
	pinMode(BTN_RED, INPUT);
	pinMode(BTN_YELOW, INPUT);
	pinMode(BTN_GREEN, INPUT);
	pinMode(BTN_BLUE, INPUT);

	Serial.begin(9600);
	randomSeed(analogRead(A0));
}


int buttons[4] = {BTN_RED, BTN_YELOW, BTN_GREEN, BTN_BLUE};
int leds[4] = {LED_RED, LED_YELOW, LED_GREEN, LED_BLUE};

int win = 1;


void loop() 
{	
	const int difficulty = setDifficulty();
	int sequence[difficulty];
	int score = 0;

	clearArray(sequence, difficulty);

	for (int i = 0; i < difficulty && win; i++) {
		int color = random(4);
		sequence[i] = leds[color];
		
		playLedSequence(sequence, i+1 ,1);
		
		int player[difficulty];
		win = toPlay(player, sequence, difficulty);
		
		if (win) {
			score++;
			digitalWrite(LED_WHITE, HIGH);
			delay(1000);
			digitalWrite(LED_WHITE, LOW);
			delay(500);
		}

		else {
			Serial.println("LOSE");
			playLedSequence(leds, 4, 2);
		}
	}

	if (score == difficulty) {
		for (int i = 0; i < 3; i++)
			playLedSequence(leds, 4, 2);
			delay(500);
	}
	win = 1;
}

// ================== FUNCTIONS ================== //

int countElements(int *array, int length) {
	int elements = 0;
	for (int i = 0; i < length; i++) {
		if (array[i] != -1) elements++;
	}
	return elements;
}
// SET //

int setDifficulty() {
	int time = 0;
	int pressed = 0;

	while (1) {
		int btnValue = digitalRead(BTN_RED);
		
		if (btnValue == 1) {
			pressed = 1;
			time++;
			digitalWrite(LED_WHITE, HIGH);
		}

		if (btnValue != pressed) 
			return time;

		delay(100);
		digitalWrite(LED_WHITE, LOW);
		delay(900);
	}	
}

// LEDS //
void playLedSequence(int *sequence, int elements,  int mode)
{
	if (mode == 1) {
		for (int i = 0; i < elements; i++) {
			digitalWrite(sequence[i], HIGH);
			delay(700);
			digitalWrite(sequence[i], LOW);
			delay(300);
		}
	}

	else if (mode == 2) {
		for (int i = 0; i < elements; i++) {
			digitalWrite(sequence[i], HIGH);
		}
		delay(700);

		for (int i = 0; i < elements; i++) {
			digitalWrite(sequence[i], LOW);
		}
		delay(300);
	}
}


// BUTTONS //
int getColor(int *buttons)
{
	int pressed = 0;
	while (1) {
		for (int i = 0; i < 4; i++) {
			int btnValue = digitalRead(buttons[i]);
			
			if (btnValue){
				pressed = 1;
				readButton(buttons[i], pressed);
				return buttons[i];
			} 
		}
	}
}

void readButton(int button, int pressed)
{
	while (1) {
		digitalWrite(button - 4, HIGH);
		int btnValue = digitalRead(button);
		
		if (btnValue != pressed) {
			digitalWrite(button - 4, LOW);
			break;
		}
	}
}


// PLAYERS //
int toPlay(int *player, int *sequence, int difficulty) {
	int elements = countElements(sequence, difficulty);
	clearArray(player, elements);
	
	for (int i = 0; i < elements; i++) {
		int color = getColor(buttons);
		
		player[i] = color;
		delay(150);
			
		if (player[i] != sequence[i] + 4) {
			return 0;
			break;
		}
	}
	return 1;
}

void clearArray(int *player, int elements) {
	for (int i = 0; i < elements; i++) {
		player[i] = -1;
	}
}