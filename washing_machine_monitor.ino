#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 1, 0, 9, 8, 7);

int prewash_pwr = 13;
int wash_pwr = 12;
int spin_pwr = 11;
int complete_pwr = 10;

int prewash_sense = 2;
int wash_sense = 3;
int spin_sense = 4;
int complete_sense = 5;

int prewash_max_time = 600;
int wash_max_time = 1200;
int spin_max_time = 600;

String wash_cycle;
boolean has_run = false;

void setup() {
  // put your setup code here, to run once:
  delay(10000);
  lcd.begin(16, 2);
  lcd.print("STARTING WASH");
  delay(1000);
  lcd.clear();
  pinMode(prewash_pwr, OUTPUT);
  pinMode(wash_pwr, OUTPUT);
  pinMode(spin_pwr, OUTPUT);
  pinMode(complete_pwr, OUTPUT);

  pinMode(prewash_sense, INPUT);
  pinMode(wash_sense, INPUT);
  pinMode(spin_sense, INPUT);
  pinMode(complete_sense, INPUT);
}

void controlCycle(int cycle_sense, int cycle_pwr, int timeout) {
  int seconds = timeout;
  while (digitalRead(cycle_sense) == 1 && seconds > 0) {
    int cycle_state = digitalRead(cycle_sense);
    delay(1);
    seconds--;

    // Convert to minutes:seconds
    int minutes = seconds / 60;
    int remaining_seconds = seconds % 60;

    // Display time in minutes:seconds format
    lcd.setCursor(0, 0);
    lcd.print("TIME LEFT: ");
    
    // Display minutes with leading zero
    if (minutes < 10) {
      lcd.print("0");
    }
    lcd.print(minutes);
    lcd.print(":");
    
    // Display seconds with leading zero
    if (remaining_seconds < 10) {
      lcd.print("0");
    }
    lcd.print(remaining_seconds);

    if (seconds < 5) {
      digitalWrite(cycle_pwr, LOW);  // Turn off LED
      delay(50);                      // Give it a moment to settle

      // Force the sensor pin to LOW by setting it as OUTPUT temporarily
      pinMode(cycle_sense, OUTPUT);
      digitalWrite(cycle_sense, LOW);  // Clear any floating state
      delay(50);
      pinMode(cycle_sense, INPUT);     // Return to input mode
    }

    if (seconds <= 0) {
      break;
    }
  }
}

void displayCurrentCycle() {
  lcd.setCursor(0, 1);
  lcd.print("CYCLE: ");
  lcd.print(wash_cycle);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (has_run == false) {
    while (wash_cycle != "FINISHED") {
      digitalWrite(prewash_pwr, HIGH);
      int prewash_state = digitalRead(prewash_sense);
      if (prewash_state == 1) {
        wash_cycle = "prewash";
        displayCurrentCycle();
        controlCycle(prewash_sense, prewash_pwr, prewash_max_time);
      }

      digitalWrite(wash_pwr, HIGH);
      int wash_state = digitalRead(wash_sense);
      if (wash_state == 1) {
        wash_cycle = "WASHING";
        displayCurrentCycle();
        controlCycle(wash_sense, wash_pwr, wash_max_time);
      }

      digitalWrite(spin_pwr, HIGH);
      int spin_state = digitalRead(spin_sense);
      if (spin_state == 1) {
        wash_cycle = "SPINNING";
        displayCurrentCycle();
        controlCycle(spin_sense, spin_pwr, spin_max_time);
      }

      digitalWrite(complete_pwr, HIGH);
      int complete_state = digitalRead(complete_sense);
      if (complete_state == 1) {
        wash_cycle = "FINISHED";
        lcd.clear();
        displayCurrentCycle();
      }
    }
    has_run = true;
  }
}