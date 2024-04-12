#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define number_of_samples 4
#define prog 4

int sensorPin = A0;
int ledPin = 8;  

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Serial.begin(9600);
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2); 
  lcd.print("Heart rate cnt.");  
  pinMode(ledPin, OUTPUT);  
}

void loop() {
  float reading[number_of_samples], sum;
  long int current_time, ptr;
  float last, current, start;
  float first, second, third, fourth, fifth, sixth, seventh, eighth, ninth, ten, previous, pulse_value;
  bool increase;
  int increase_counter;
  int n;
  long int previous_time;

  for (int i = 0; i < number_of_samples; i++)
    reading[i] = 0;
  sum = 0;
  ptr = 0;

  while (1) {

    n = 0;
    start = millis();
    current = 0.;
    do {
      current += analogRead(sensorPin);
      n++;
      current_time = millis();
    } 
    while (current_time < start + 20);
    current /= n;  // average

    sum -= reading[ptr];
    sum += current;
    reading[ptr] = current;
    last = sum / number_of_samples;

    if (last > previous) {

      increase_counter++;

      if (!increase && increase_counter > prog) {
        increase = true;
        first = millis() - previous_time;
        previous_time = millis();

        pulse_value = 60000. / (0.165 * (first + second + third + fourth + fifth + sixth));

        Serial.print(pulse_value);
        Serial.print('\n');

        lcd.setCursor(0, 1); 
        lcd.print("Pulse: " + String(pulse_value));

        sixth = fifth; 
        fifth = fourth;
        fourth = third;
        third = second;
        second = first;

        digitalWrite(ledPin, HIGH);  
        delay(150); 
        digitalWrite(ledPin, LOW);  

      }
    } 
    else {
      increase = false;
      increase_counter = 0;
    }
    previous = last;

    ptr++;
    ptr %= number_of_samples;
  }
}
