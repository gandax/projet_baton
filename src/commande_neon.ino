#define LED_BLUE 5

#define BUTTON_MODE 2
#define BUTTON_START_STOP 3

#define LED_RED 10
#define LED_YELLOW 12
#define LED_GREEN 11

#define SLEEP_DELAY 30

volatile byte mode = 0;
volatile byte isRunning = 0;
volatile byte timerDegueu = 0;

// Activation spéciale 1 fois
// Mode allumé normal
// Plantage 1 fois

int initStick[] = {100,300,100,250,100,500,100, 750, 100, 1000};
int fuckedStick[] = {2000,300,1500,200,500,100, 250, 100};
int tabLed[] = {LED_RED, LED_YELLOW, LED_GREEN};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_START_STOP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), toggleMode, RISING);
  attachInterrupt(digitalPinToInterrupt(3), toggleStartStop, RISING);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  noInterrupts();

  // mode CTC
  bitClear(TCCR1A, WGM10);
  bitClear(TCCR1A, WGM11);
  bitSet(TCCR1B, WGM12); 
  bitClear(TCCR1B, WGM13);

  // prediviseur = 256
  bitClear(TCCR1B, CS10);
  bitClear(TCCR1B, CS11);
  bitSet(TCCR1B, CS12);
  bitSet(TIMSK1, OCIE1A);

  TCNT1 = 0;
  OCR1A = 62500;
  interrupts();
  
}

void toggleMode()
{
  Serial.println("Change mode");
  mode = (mode + 1) % 3;
  digitalWrite(tabLed[mode], HIGH);
  switch(mode)
  {
    case 0:
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      break;
    case 1:
      digitalWrite(0, LOW);
      digitalWrite(2, LOW);
      break;
    case 2:
      digitalWrite(0, LOW);
      digitalWrite(1, LOW); 
      break;
    default:
      break; 
  }
  delay(100);
}

void toggleStartStop()
{
  if(isRunning)
  {
    isRunning = 0;
    Serial.println("Stop button");
  }
  else
  {
    isRunning = 1;
    Serial.println("Start button");
  }

  switch (mode) {
    case 1:
      initMode();      
      break;
    case 2:
      runMode();
      break;
    case 3:
      bugMode();
      break;      
    default:
      break;
  }

}

void initMode()
{
  int size = sizeof(initStick)/2;
  Serial.println("Init MODE");
  for (int i=0; i < size; i++) 
  {
    digitalWrite(LED_BLUE, LOW);
    delay(initStick[i]);
    digitalWrite(LED_BLUE,HIGH);
    delay(initStick[i+1]);
  }
    runMode();
}

void bugMode()
{
  int size = sizeof(fuckedStick)/2;
  Serial.println("Bug MODE");
  for (int i=0; i < size; i++) 
  {
    digitalWrite(LED_BLUE, HIGH);
    delay(fuckedStick[i]);
    digitalWrite(LED_BLUE,LOW);
    delay(fuckedStick[i+1]);
  }
}

void runMode()
{
  Serial.println("Run MODE");
}

// ======================
// Routine d'interruption
// ======================
ISR(TIMER1_COMPA_vect) {
  Serial.print("Mode=");
  Serial.print(mode);
  Serial.print(" / Status=");
  Serial.println(isRunning);
  // Check if we need to put the stick in sleep mode
  if (!isRunning)
  {
    if (timerDegueu >= SLEEP_DELAY)
    {
      timerDegueu = 0;
      // On stoppe le baton
      isRunning = 0;
      digitalWrite(LED_BLUE,LOW);
    }
    else
    {
      timerDegueu++;
    }
  }
}

void loop() {

}
