int prewash_pwr = 13;
int wash_pwr = 12;
int spin_pwr = 11;
int complete_pwr = 10;

int prewash_sense = 0;
int wash_sense = 1;
int spin_sense = 2;
int complete_sense = 3;

int prewash_max_time = 10;
int wash_max_time = 10;
int spin_max_time = 10;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(prewash_pwr, OUTPUT);
  pinMode(wash_pwr, OUTPUT);
  pinMode(spin_pwr, OUTPUT);
  pinMode(complete_pwr, OUTPUT);

  pinMode(prewash_sense, INPUT);
  pinMode(wash_sense, INPUT);
  pinMode(spin_sense, INPUT);
  pinMode(complete_sense, INPUT);
}

void displayClear() {
}

void loop() {
  // put your main code here, to run repeatedly:
  String wash_cycle;

  while (wash_cycle != "finished") {
    int prewash_state = digitalRead(prewash_sense);
    digitalWrite(prewash_pwr, HIGH);
    if (prewash_state == 1) {
      wash_cycle = "prewash";
      int seconds = prewash_max_time;
      while (prewash_state == 1) {
        prewash_state = digitalRead(prewash_sense);
        seconds--;
        if (seconds <= 5) {
          digitalWrite(prewash_pwr, LOW);
        }
      }
    }

    int wash_state = digitalRead(wash_sense);
    digitalWrite(wash_pwr, HIGH);
    if (wash_state == 1) {
      wash_cycle = "washing";
      int seconds = wash_max_time;
      while (wash_state == 1 and seconds >= 0) {
        wash_state = digitalRead(wash_sense);
        seconds--;
        if (seconds <= 5) {
          digitalWrite(wash_pwr, LOW);
        }
      }
    }

    int spin_state = digitalRead(spin_sense);
    digitalWrite(spin_pwr, HIGH);
    if (spin_sense == 1) {
      wash_cycle = "spinning";
      int seconds = spin_max_time;
      while (spin_sense == 1 and seconds >= 0) {
        spin_state = digitalRead(spin_sense);
        seconds--;
        if (seconds <= 5) {
          digitalWrite(spin_pwr, LOW);
        }
      }
    }

    int complete_sense = digitalRead(complete_sense);
    digitalWrite(complete_pwr, HIGH);
    if (complete_sense == 1) {
      wash_cycle = "finished";
      }
    }
  }
