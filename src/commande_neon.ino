#define LED_BLUE 5
#define BUTTON 2

#define LED_RED 10
#define LED_YELLOW 12
#define LED_GREEN 11

volatile byte mode = 0;

int initStick[] = {100,300,100,250,100,500};
int tabLed[] = {LED_RED, LED_YELLOW, LED_GREEN};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), toggle, RISING);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void toggle()
{
  Serial.println("Mode + 1");
  digitalWrite(tabLed[mode], LOW);
  mode = (mode + 1) % 3;
  delay(10);
  digitalWrite(tabLed[mode], HIGH);
}

void loop() {

  // put your main code here, to run repeatedly:
  int size = sizeof(initStick)/2;
  Serial.println(mode, DEC);
  for (int i=0; i < size; i++) 
  {
      digitalWrite(LED_BLUE, LOW);
      delay(initStick[i]);
      digitalWrite(LED_BLUE,HIGH);
      delay(initStick[i+1]);
  }
  digitalWrite(LED_BLUE, HIGH);
  //delay(5000);
  Serial.println("STOP");
delay(1000);
}

