#include "I2C_Devices.h"
#include "Servos.h"

// globals initialization
const int LCD_ADDR = 0x3F;
const int OLED_ADDR = 0x3C;
const int LIGHT_SENSOR_ADDR = 0x29;
const int IR_PIN = 26;
const int GESTURE_ADDR = 0x43;
const int GYRO_ADDR = 0x68;
uint8_t gestureData = 0;

// device object initialization
LiquidCrystal_I2C LCD(LCD_ADDR, 16, 2);
Adafruit_SSD1306 EYE_L(128, 64); // left eye (NIKO's left)
Adafruit_SSD1306 EYE_R(128, 64); // right eye (NIKO's right)
VL53L0X EDGE_SENSOR; // analog light distance sensor
Adafruit_MPU6050 GYRO; // gyro (auto detects address I think?)

// Big LCD functions
// init LCD
void LCDInit() {
  // initialize LCD
  LCD.init();
  // turn on LCD backlight                      
  LCD.backlight();
}

// function to print messages on either or both lines of center display
void LCDPrint(const char* message1, const char* message2) {
  LCD.clear();
  // set cursor to first column, first row
  LCD.setCursor(0, 0);
  // print message
  LCD.print(message1);
  
  // set cursor to first column, second row
  LCD.setCursor(0,1);
  LCD.print(message2);
}

// eye functions (some of these are kinda funky cause most of the time, each eye will do the same thing, but we want the option
// open to being able to manipulate them separately)
// inits both eyes and draws first eye frame
void eyesInit() { 
  // init EYE_L
  EYE_L.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); // init
  EYE_L.clearDisplay();         // clear software buffer
  EYE_L.setTextColor(WHITE);    // set text color
  EYE_L.display();
  // init EYE_R
  EYE_R.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); // init
  EYE_R.clearDisplay();         // clear software buffer
  EYE_R.setTextColor(WHITE);    // set text color
  EYE_R.display();
  // TODO: draw eyes?
}

// various drawing methods (these may need tweaking/repurposing because they might interrupt control flow of script too much?)
// (i.e., the eye animation may look nothing like this functional approach by the end)
// NIKO's eyes flicker and open
void wakeUp() {
  
}

// winks eye based on eye char passed in (default 'r' for right)
void winkEye(Adafruit_SSD1306* eye) {
  
}

// blinks both eyes
void blinkEyes() {
  
}

// displays a message at (0,0) on one or both eye(s)
void displayEyeMessage(const char* message, Adafruit_SSD1306* eye) {
  eye->clearDisplay();
  eye->setCursor(0, 0);
  eye->setTextSize(2);
  eye->print(message);
  eye->display();
}

// light (edge) distance sensor functions
// init EDGE_SENSOR (still unsure of why the constructor never needs to be called ¯\_(ツ)_/¯)
void edgeSensorInit() {
  EDGE_SENSOR.init();
  EDGE_SENSOR.setTimeout(500);
  EDGE_SENSOR.startContinuous();
}

// IR obst functions
void IRAM_ATTR objectDetected() {
  moveState = backward;
}
void IRAM_ATTR objectUndetected() {
  moveState = idle;
}
void IRObstInit() {
  pinMode(IR_PIN, INPUT);
  attachInterrupt(IR_PIN, objectDetected, FALLING);
  attachInterrupt(IR_PIN, objectUndetected, RISING);
}

// gesture sensor functions
void gestureInit() {
  paj7620Init();
}

uint8_t gestureRead() {
  return paj7620ReadReg(GESTURE_ADDR, 1, &gestureData);
}

// gyroscope functions
void gyroInit() {
  GYRO.begin();
}

// initialize I2C devices
void I2CInit() {
  LCDInit();
  Serial.println("LCD initialized");
  LCDPrint("Pairing... Go to", "Processing!");
  
  eyesInit();
  Serial.println("Left eye initialized");
  Serial.println("Right eye initialized");
  
  edgeSensorInit();
  Serial.println("Edge (light) sensor initialized");
  
  IRObstInit();
  Serial.println("Infrared Obstacle Avoidance (front) sensor initialized");
  
  gestureInit();
  Serial.println("Gesture sensor initialized");
  
  gyroInit();
  Serial.println("Gyroscope initialized");
  Serial.println("");
}
