/*Water level monitoring system with the New Blynk app
   https://srituhobby.com
*/
//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "dpE01zVF_0X3m9rZ7S6utkESV0v_RpOB";//Enter your Auth token
char ssid[] = "Redmi 9T";//Enter your WIFI name
char pass[] = "12345678";//Enter your WIFI password

BlynkTimer timer;

// Define the component pins
#define trig D7
#define echo D8
#define LED1 D0
#define LED2 D3
#define buzzer D5
#define relay 3

//Enter your tank max value(CM)
int MaxLevel = 20;

int Level1 = (MaxLevel * 75) / 100;
int Level2 = (MaxLevel * 65) / 100;
int Level3 = (MaxLevel * 55) / 100;


void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(0, 0);
  lcd.print("Water level");
  lcd.setCursor(4, 1);
  lcd.print("Monitoring");
  delay(4000);
  lcd.clear();

  //Call the functions
  timer.setInterval(100L, ultrasonic);
}

//Get the ultrasonic sensor values
void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  int distance = t / 29 / 2;

  int blynkDistance = (distance - MaxLevel) * -1;
  if (distance <= MaxLevel) {
    Blynk.virtualWrite(V0, blynkDistance);
  } else {
    Blynk.virtualWrite(V0, 0);
  }
  lcd.setCursor(0, 0);
  lcd.print("WLevel:");

  if (Level1 <= distance) {
    lcd.setCursor(8, 0);
    lcd.print("Low");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(buzzer, LOW);
   
  } else if (Level2 <= distance && Level1 > distance) {
    lcd.setCursor(8, 0);
    lcd.print("Medium");
    lcd.print("      ");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(buzzer, LOW);
  } else if (Level3 >= distance) {
    lcd.setCursor(8, 0);
    lcd.print("HIGH");
    lcd.print("      ");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(buzzer, HIGH);
   
  }
}

//Get the button value
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  } else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
  }
}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
