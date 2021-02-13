#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SD.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 //4 Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int chipSelect = 4;

int pulsePin = 6;
int tempPin =7;
//int sdPin = 8;
//int displayPin = 9;
float pulseValue = 0;
float tempValue = 0;
int counter = 10;

void setup() {
  Serial.begin(9600);
  pinMode(pulsePin, INPUT);
  //pinMode(displayPin, OUTPUT);
  //pinMode(sdPin, OUTPUT);
  delay(300);
  while(!Serial){ }
  
  Serial.print("Initializzing SD card and display.");
  if(!SD.begin(chipSelect)){
    Serial.println("SD card is not initialized.");
    while(1);
  }
  Serial.println("Card initialized.");
  delay(1500);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // Address 0x3D for 128x64
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)){
    Serial.println("SSD1306 display allocation failed");
    while(1);
  }
  Serial.println("display allocation successful.");
  delay(1500);
}

void displayValue(float pulseValue, float tempValue){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,5);        //start 5 pixel right of top-left corner
  display.println("Pulse: " + String(pulseValue));
  display.println("Temperature: " + String(tempValue));
  display.display();
  delay(5000);
}

void saveValue(String data){
  File dataFile = SD.open("Record.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(data);
    dataFile.close();
    Serial.println(data);
  }
  else{
    Serial.println("error opening Record.txt");
  }
}

void loop() {
  String dataString = "";
  
  pulseValue = digitalRead(pulsePin);
  tempValue = digitalRead(tempPin);
  displayValue(pulseValue,tempValue);
  
  if(pulseValue < 60 && pulseValue > 72){
    dataString ="A: PulseRate:  " + String(pulseValue) + ", Temperature: " + String(tempValue) + "\n";
    //save the value in sd card indicating abnormal condition
    saveValue(dataString);
    //alram
    delay(5000);
    counter = 10;
  }
  else{
    delay(5000);
    counter = counter - 1;
    if(counter = 0){
      dataString ="N: PulseRate:  " + String(pulseValue) + ", Temperature: " + String(tempValue) + "\n";
      //save the value in sd card 
      //indicating normal condition after 1 hour time
      saveValue(dataString);
      counter = 10;
    }
  }

}
