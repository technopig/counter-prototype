#include <blynk.h>
#include <LiquidCrystal.h>


char auth[] = "f2f1dd1e59c84f75addbc1b24027c4c1";
int blynk_sVal = 0; //value from blynk slider
int bulletCount = 0;

int LOW_MAP = 1500;   //lower analogRead
int HIGH_MAP = 2500;  //upper analogRead
int FULL_MAG = 30;    //full mag number

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

  lcd.clear();
  lcd.print(bulletCount);
  delay(100);
}
