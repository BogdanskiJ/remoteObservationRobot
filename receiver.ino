#include <SoftwareServo.h> 
#include <VirtualWire.h> 

#define odbior 3 
#define TRIG 12
#define ECHO 13
#define silnikpwm 11
#define silnik1 4
#define silnik2 7
#define servopwm1 10
#define servopwm2 9

SoftwareServo mojeservo1;
SoftwareServo mojeservo2;
int odczyt1; 
int odczyt2;
int a=0;
int licz=1;
unsigned long czas;
float odleglosc;

void setup()
{
    pinMode(silnikpwm,OUTPUT); 
    pinMode(silnik2, OUTPUT); 
    pinMode(silnik1, OUTPUT);
    pinMode(servopwm1, OUTPUT); 
    pinMode(servopwm2, OUTPUT); 
    pinMode(TRIG, OUTPUT); 
    pinMode(ECHO, INPUT);
    mojeservo1.attach(servopwm1); 
    mojeservo2.attach(servopwm2); 
    Serial.begin(9600); 
    vw_set_rx_pin(odbior); 
    vw_setup(2000);   
    vw_rx_start(); 
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN]; 
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    int i,last;
    String odczytX1, odczytY1;
    if  (vw_get_message(buf, &buflen))
    {
       for  (i = 0; i < buflen; i++) 
       {
          if  (char(buf[i]) != '$')
              {
                odczytY1+=char(buf[i]);
              }
          else
              {
               i++;
               last = i; 
               break;
              } 
       }
       for  (i = last; i < buflen; i++) 
       {
          if  (char(buf[i]) != '$')
             { 
              odczytX1+=char(buf[i]);
             }
          else 
          break; 
       } 
        int odczytX = odczytX1.toInt();
        int odczytY = odczytY1.toInt();
        odczyt2=odczytX;
        odczyt1=odczytY;
    }
    
  licz=licz+1;
  if  (licz==200)
  {
    pomiar();
    if  (odleglosc<10.00)
    {
      stopp();
      delay(500);
      tyl();
      delay(500);
    }
  }

  if  (odczyt1>600) 
  {
     przod();
  }
  if  (odczyt1<450)
  {
     tyl();
  }
  if  (odczyt1<600 && odczyt1>500)
  {
     stopp();
  }
  if  (odczyt2>600)
  {
    prawo();
  }
  if  (odczyt2<450)
  {
    lewo();
  }
  if  (odczyt2>450 && odczyt2<600)
  {
    stopservo();
  }
}

void przod() 
{
  int sterY;
  sterY=map(odczyt1,600,1025,0,255);
  a=sterY;
  digitalWrite(silnik1,LOW);
  digitalWrite(silnik2,HIGH);
  analogWrite(silnikpwm,a);
}
void tyl()
{
  int sterY;
  sterY=map(odczyt1,450,0,0,255);
  a=sterY;
  digitalWrite(silnik1,HIGH);
  digitalWrite(silnik2,LOW);
  analogWrite(silnikpwm,a);
}
void stopp()
{
  digitalWrite(silnik1,LOW);
  digitalWrite(silnik2,LOW);
  analogWrite(silnikpwm,0);
}
void prawo()
{
  int pozservo1;
  int pozservo2;
  pozservo1=map(odczyt2,600,1025,100,180);
  pozservo2=180-pozservo1;
  mojeservo1.write(pozservo1);
  mojeservo2.write(pozservo2);
  SoftwareServo::refresh();
}
void lewo()
{
  int pozservo1;
  int pozservo2;
  pozservo1=map(odczyt2,0,450,0,80);
  pozservo2=180-pozservo1;
  mojeservo1.write(pozservo1);
  mojeservo2.write(pozservo2);
  SoftwareServo::refresh();
}
void stopservo()
{
  int pozservo1;
  pozservo1=90;
  mojeservo1.write(pozservo1);
  mojeservo2.write(pozservo1);
  SoftwareServo::refresh();
}
float pomiar() 
{
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  czas = pulseIn(ECHO,HIGH);
  odleglosc = czas/58.00;
  licz=1;
  if(odleglosc>100)
  {
    odleglosc=100;
  }
  return odleglosc;
}
