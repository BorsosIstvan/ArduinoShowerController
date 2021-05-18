#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//-------------- Parameter settings: -----------------------------------<

//----------------------------------------------------------------------<

int ShowerMinute = 10;   //shower time in minutes
int WaitBeforeNext = 3; //wait time when finished
int WaitPausede = 3; // wait time if paused
//----------------------------------------------------------------------<


LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd connecting to SDA & SCL pins

const int buttonPin = 0; // arduino pin number it is connected to pushbutton
const int solenoidValve =  13; // pin number of the valve relay command
const int flowSensor = 2;     // pin number of the water flow sensor 

const int flowInterrupt = 0; // interrupt 0
volatile byte pulseCount = 0;

int buttonState = HIGH;
int ShowerSecond = 59;  //need for 60 seconds in minutes
int ss = ShowerSecond;
int sm = ShowerMinute - 1;
unsigned wpm = WaitPausede *60*10;
unsigned wbn = WaitBeforeNext -1;

//----------------------------------------------------------------<

void setup(){
  // initialize the VALVE pin as an output:
  pinMode(solenoidValve, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize the flowsensor pin as an input:
  pinMode(flowSensor, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  }

//-----------------------------------------------------------------<

void loop() 
{
//-----------------------------------------------------------------<
attachInterrupt(flowInterrupt, pulseCounter, FALLING);
//-----------------------------------------------------------------<
start:

//lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print(" Welcome! Press "); 
lcd.setCursor(0,1);
lcd.print("button to start ");

button:
if (buttonState == HIGH)
{
  buttonState = digitalRead(buttonPin);
  delay(100);
  goto button;
}
SetSolenoidValveON();
//--------------------->

lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print(" Turn on shower "); 
lcd.setCursor(0,1);
lcd.print("                ");
pulse:
pulseCount = 0;
delay(100);
if(pulseCount < 1){
  goto pulse;
}
//--------------------->
showering:
//lcd.begin(16,2);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("   Time left:   "); 
lcd.setCursor(0,1);
lcd.print("     ");
for (int m = sm; m >= 0; m--){
  for(int s = ss; s >= 0; s--){
    lcd.setCursor(5,1);
    if (m < 10){lcd.print("0");}
    lcd.print(m , DEC);
    lcd.print(":");
    if (s < 10){lcd.print("0");}
    lcd.print(s , DEC);
    pulseCount = 0;
    delay(1000);// 1000 on real
    if (s <= 0){ss = 59;}
    if (pulseCount <= 1){
      sm = m;
      ss = s;
      goto paused;}   
  }
}


//--------------------->
showerfinished:
//lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("Shower finished"); 
lcd.setCursor(0,1);
lcd.print(" Switch off tap");
SetSolenoidValveOFF();
delay(10000);
//--------------------->
lcd.setCursor(0,0);
lcd.print("   Next shower  "); 
lcd.setCursor(0,1);
lcd.print("aviable in ");
//---------------------<
for (int m = wbn; m >= 0; m--){
  for(int s = 59; s >= 0; s--){
    lcd.setCursor(11,1);
    if (m < 10){lcd.print("0");}
    lcd.print(m , DEC);
    lcd.print(":");
    if (s < 10){lcd.print("0");}
    lcd.print(s , DEC);
    delay(1000);// 1000 on real
    if (s <= 0){ss = 59;}   
  }
}
//---------------------<
pulseCount = 0;
ss = ShowerSecond;
sm = ShowerMinute - 1;
buttonState = HIGH;
goto start;
//--------------------->

paused:
int sp = wpm;
//lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("   Time left:   "); 
lcd.setCursor(0,1);
lcd.print("     paused     ");
pulsepaused: 
  if(pulseCount <= 1){
  delay(100);
  sp--;
  if(sp <=0){goto showerfinished;}
  goto pulsepaused;}
  goto showering;
  



//--------------------->
//-----------------------------------------------------------------< 
}

//-----------------------------------------------------------------<
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
//-----------------------------------------------------------------<
void SetSolenoidValveON()
{
  digitalWrite(solenoidValve, HIGH);
}
void SetSolenoidValveOFF()
{
  digitalWrite(solenoidValve, LOW);
}
//-----------------------------------------------------------------<
