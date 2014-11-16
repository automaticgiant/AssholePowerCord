#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  lcd.begin(16, 2);
  analogReference(INTERNAL);
  pinMode(7, OUTPUT);
  pinMode(13, INPUT_PULLUP);
  digitalWrite(7, HIGH);
}

boolean powered = true;
char character = 0;
const int N = 830;
const boolean DEBUG = false;
unsigned long credits = 2000;
const int threshold = 4;
const int warning = 1000;
const int scale = 2;
const int extra_credits = 2000;
unsigned short samples[N];
void loop() {
  // do sampling
  for (int sample = 0; sample < N; sample++) {
    samples[sample] = analogRead(0);
  }
  unsigned short max = 0;
  unsigned short min = 1000;

  // min/max samples
  for (int sample = 0; sample < N; sample++) {
    if (samples[sample] > max) max = samples[sample];
    if (samples[sample] < min) min = samples[sample];
  }
  
  if (digitalRead(13) == HIGH) {
    delay(75);
    if (digitalRead(13) == HIGH) credits = credits + extra_credits;
  }
  
  // lcd output  
  lcd.clear();

  // line 1 is remaining and consumption
  lcd.home();
  lcd.print("C:");
  lcd.print(credits, DEC);
  float average = (max + min) / 2;
  lcd.print(" Draw:");
  float magnitude = max - average;
  lcd.print((int) magnitude, DEC);
  
  // line 2 is possible warning
  lcd.setCursor(0, 1);
  if (credits < warning) lcd.print("Feed Me!");

  // do decrement math
  if (credits > 0) {
    unsigned short round_debt = fmin(magnitude / scale, credits);
    if (magnitude > threshold)
      credits = fmin(credits, credits - round_debt);

  // turn off power
  } else {
    boolean credited = false;
    if (powered) {
      powered = false;
      digitalWrite(7, LOW);
      credited = false;
    }
     while (credited == false) {
      delay(100);
      if (digitalRead(13) == HIGH) {
        credited = true;
        delay(500);
      }
    }
    credits = credits + extra_credits;
    powered = true;
    digitalWrite(7, HIGH);
  }
    
}
