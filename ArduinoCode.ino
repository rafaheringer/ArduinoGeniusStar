const int button1Pin  = 2;        //Push button 1
const int button2Pin  = 3;        //Push button 2
const int button3Pin  = 4;        //Push button 3

const int led1Pin     = 11;       //Led pin 1
const int led2Pin     = 12;       //Led pin 2  
const int led3Pin     = 13;       //Led pin 3

const int led4Pin     = 9;       //Led pin 4 
const int led5Pin     = 8;        //Led pin 5

const int buzzerPin   = 10;       //Buzzer pin

int button1State = 0;             //Button press state 1
int button2State = 0;             //Button press state 2
int button3State = 0;             //Button press state 3

int buzzerTone   = 0;             //Buzzer melody tone

//Genius configuration
int geniusSequence[100] = {};                                 //Genius sequence array
int geniusStage = 0;                                          //Current genius stage
int geniusStageStep = 0;                                      //Current step on stage
int geniusFinish = false;                                     //End of the game
int geniusPressedButton = 0;                                  //Pressed button 
int geniusNotes[3] = {294, 392, 440};                         //Genius notes sequence
int geniusLedPins[3] = {led1Pin, led2Pin, led3Pin};           //Led pins sequence
int geniusButtons[3] = {button1Pin, button3Pin, button2Pin};  //Genius buttons
int geniusWinStage = 5;                                       //Steps to win the game
int geniusWinTheGame = false;                                 //Bool to control the win state



void setup() {
  Serial.begin(9600);
    
  //Pin Modes
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  
  pinMode(led4Pin, OUTPUT);
  pinMode(led5Pin, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);
  
  randomSeed(analogRead(0));
}

void loop() {
  //If game is finished, reset the game
  Serial.print("geniusFinish: ");
  Serial.println(geniusFinish);
  if(geniusFinish) {
    int geniusSequence[100] = {};
    geniusStage = 0;    
    geniusStageStep = 0;
    
    geniusFinish = false;
  }
  
  //Play melody to start the game!
  Serial.print("geniusStage: ");
  Serial.println(geniusStage);
  if(geniusStage == 0) {
    startGameMelody();
    launchLedsMain();
    delay(500);
  }
  
  if(geniusStage == geniusWinStage) {
    geniusWinGame();
  }
  
  //Start the nex stage
  geniusStartNextStage();
  
  //Reproduce sequence
  geniusReproduceSequence();
  
  //Wait for player
  geniusWaitForPlayer();
  
  //Next stage wait
  delay(1000);
}


void startGameMelody() {
  
  
}

void geniusStartNextStage() {
  Serial.print("Starting next stage. Stage: ");
  Serial.println(geniusStage);
  int sortedNumber = random(0, 3);
  geniusSequence[geniusStage++] = sortedNumber;
}

void geniusReproduceSequence() {
  for (int i = 0; i < geniusStage; i++) {
    tone(buzzerPin, geniusNotes[geniusSequence[i]]);

    digitalWrite(geniusLedPins[geniusSequence[i]], HIGH);
    delay(500);

    noTone(buzzerPin);

    digitalWrite(geniusLedPins[geniusSequence[i]], LOW);
    delay(100);
  }

  noTone(buzzerPin);
}

void geniusWaitForPlayer() {
  for (int i = 0; i < geniusStage; i++) {
    geniusWaitForPlay();
    geniusCheckPlay();

    if (geniusFinish) {
      break;
    }

    geniusStageStep++;
  }

  geniusStageStep = 0;
}

void geniusWinGame() {
  geniusWinTheGame = true;
  
  tone(buzzerPin, geniusNotes[0]);
  delay(100);
  tone(buzzerPin, geniusNotes[1]);
  delay(100);
  tone(buzzerPin, geniusNotes[2]);
  
  for(int o = 0; o < 5; o++) {
    
    for(int i =0; i < 3; i++) {
      digitalWrite(geniusLedPins[i], HIGH);
    }
  
    delay(100);
  
    for(int i =0; i < 3; i++) {
      digitalWrite(geniusLedPins[i], LOW);
    }
    
    delay(100);
  }

  delay(500);
  
  tone(buzzerPin, geniusNotes[0]);
  digitalWrite(geniusLedPins[2], HIGH);
  delay(1000);
  tone(buzzerPin, geniusNotes[1]);
  digitalWrite(geniusLedPins[0], HIGH);
  delay(1000);
  tone(buzzerPin, geniusNotes[2]);
  digitalWrite(geniusLedPins[1], HIGH);
  delay(1000);
  
  noTone(buzzerPin);
  
  
  while(true) {
    launchLedsMain();
  }
  
}
void launchLedsMain() {
  if(geniusWinTheGame == false) {
    digitalWrite(led4Pin, HIGH);
    digitalWrite(led5Pin, LOW);
  } else {
    digitalWrite(led4Pin, LOW);
    digitalWrite(led5Pin, HIGH);
  }
  
}

void geniusWaitForPlay() {
  boolean played = false;

  while (!played) {
    
    launchLedsMain();
    
    for (int i = 0; i < 3; i++) {
      //Serial.print("Button pressed: ");
      //Serial.print(i);
      //Serial.print(" is ");
      //Serial.print(digitalRead(geniusButtons[i]));
      
      if (digitalRead(geniusButtons[i]) == HIGH) {
        geniusPressedButton = i;
        
        tone(buzzerPin, geniusNotes[i]);
        digitalWrite(geniusLedPins[i], HIGH);

        delay(300);

        digitalWrite(geniusLedPins[i], LOW);
        noTone(buzzerPin);

        played = true;

      }
      
      //Serial.println("");
    }

    delay(10);
  }
}

void geniusCheckPlay() {
  Serial.print("Check play. ");
  Serial.print("geniusStageStep: ");
  Serial.print(geniusStageStep);
  Serial.print(" geniusPressedButton: ");
  Serial.println(geniusPressedButton);
  
  if (geniusSequence[geniusStageStep] != geniusPressedButton) {
    // GAME OVER
    for (int i = 0; i < 3; i++) {
      tone(buzzerPin, geniusNotes[i]);
      digitalWrite(geniusLedPins[i], HIGH);

      delay(200);

      digitalWrite(geniusLedPins[i], LOW);
      noTone(buzzerPin);
    }

    tone(buzzerPin, geniusNotes[2]);

    for (int i = 0; i < 3; i++) {
      digitalWrite(geniusLedPins[0], HIGH);
      digitalWrite(geniusLedPins[1], HIGH);
      digitalWrite(geniusLedPins[2], HIGH);

      delay(100);

      digitalWrite(geniusLedPins[0], LOW);
      digitalWrite(geniusLedPins[1], LOW);
      digitalWrite(geniusLedPins[2], LOW);

      delay(100);
    }

    noTone(buzzerPin);

    geniusFinish = true;

  }
}
