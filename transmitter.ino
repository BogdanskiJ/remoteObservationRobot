#include <VirtualWire.h>
#define transmit_pin 8 
int sec = 1;
int odczytY=0;
int odczytX=0;
String   toSend="";
void setup()
{
  Serial.begin(9600);
  vw_set_tx_pin(transmit_pin);
  vw_setup(2000);
}
void loop()
{
  odczytY=analogRead(A0);
  odczytX=analogRead(A1);
  toSend= String(odczytX, DEC) + "$" + String(odczytY, DEC) + "$";
  char msg[23]; // tworzymy tablicę typu char
  toSend.toCharArray(msg, toSend.length() + 1);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); 
  delay(10);
}
