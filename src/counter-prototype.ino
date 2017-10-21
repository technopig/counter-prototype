//This setup uses a simple voltage divider:
//R_known goes from GND to pin A5
//R_unknown (resistVal) goes from pin A5 to V_in
//The two resistances should be as similar as possible
//  for best accuracy.

//The blink app should have a slider on V1
//  and a value display on V0
//  The slider simply writes a value to the virtual pin

//Note that analogRead returns a value from 0 to 4094
//  mapped linearly from 0V to 3.3V on the pin (ref to GND)


#include <blynk.h>
#include <LiquidCrystal.h>


char auth[] = "f2f1dd1e59c84f75addbc1b24027c4c1";
int blynk_sVal = 0;   //value from blynk slider
int bulletCount = 0;  //used to store the count
int raw = 0;          //read raw input from A5
double resistVal = 0;    //stores resistance measured
double rollingSum = 0;   //make an rolling average

int LOW_MAP = 1500;   //lower analogRead
int HIGH_MAP = 2500;  //upper analogRead
int FULL_MAG = 30;    //full mag number
int R_KNOWN = 51;     //known resistance from GND to A5
double V_IN = 3.3;    //input voltage
double V_OUT = 0.0;   //Ground voltage

BlynkTimer timer;
LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);


BLYNK_WRITE(V1)
{
  blynk_sVal = param.asInt(); //get the value from the slider on V1
}

void sendBulletCount()
{
  Blynk.virtualWrite(V0, bulletCount); //send the value to the display on V0
}

void setup()
{
  delay(500); // Allow board to settle

  Particle.variable("count", &bulletCount, INT); //register real resistor measured value

  Blynk.begin(auth); //begin Blynk
  timer.setInterval(100L, sendBulletCount); //every 1000ms, execute sendResistVal(){}

  lcd.begin(16, 2);
}

void loop()
{
  Blynk.run();
  timer.run();

  bulletCount = map(blynk_sVal, LOW_MAP, HIGH_MAP, FULL_MAG, 0);

  raw = analogRead(A5); //uncomment this line for measured resistances
  V_OUT = V_IN*raw/4094; //can possibly equal exactly 3.3
  for (int i = 0; i<10; i++)
  {
    rollingSum = rollingSum + (V_IN-V_OUT)*R_KNOWN/V_OUT;
    delay(10);
  }
  resistVal=rollingSum/10;
  lcd.clear();
  lcd.print(String("Bullets: ") + String(bulletCount));
  lcd.setCursor(0,1);
  lcd.print(String("Resistance: ") + String(resistVal));
  delay(100);
  rollingSum=0;
}

/*
To Do:
Add functionality to map resistances 0.0-3.0 ohms to 0-30 bullets.
This is 0.1 ohms per bullet - should be feasible.
*/
