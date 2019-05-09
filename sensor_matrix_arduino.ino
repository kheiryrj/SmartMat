
//Our modified code

#include <FastMap.h>
#include <FastGPIO.h>
//#include <SPI.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // Macro to clear a bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit)) // Macro to set a bit

//FastMap mapper; // A faster map() function


const int latchPin = 5; // Shift Register controls
const int clockPin = 6; // Shift Register controls
const int dataPin = 7;  // Shift Register controls
const int selectPins[3] = {4, 3, 2}; // Multiplexer controls
const int rowMap[8] = {0,1,2,3,4,5,6,7}; // Translates row number to multiplexer pin
int inputVal = 1;

void setup() {
  //SPI.begin();
  pinMode(latchPin, OUTPUT);
  digitalWrite(latchPin, HIGH);
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);
  pinMode(clockPin, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  
  digitalWrite(clockPin, HIGH);
  for (int i=0; i<3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
  
  Serial.begin(500000);
  delay(100);
  Serial.write(0); // 0 indicates start of new frame
  //mapper.init(0, 1023, 1, 255);
}

void loop() {
  for (int row=0; row<44; row++) // Changing rows is slow, so it's the outer loop
  {
      int pin = rowMap[row%8];
      /*digitalWrite(4,LOW);
      digitalWrite(3, HIGH);
      digitalWrite(2, LOW);*/
      FastGPIO::Pin<2>::setOutput(bitRead(pin,0)); // Select multiplexer pin
      FastGPIO::Pin<3>::setOutput(bitRead(pin,1)); // Select multiplexer pin
      FastGPIO::Pin<4>::setOutput(bitRead(pin,2)); // Select multiplexer pin
      
      FastGPIO::Pin<latchPin>::setOutput(LOW);  // Send 1 high bit to the shift register chain
      FastGPIO::Pin<dataPin>::setOutput(HIGH);  // Send 1 high bit to the shift register chain
      FastGPIO::Pin<clockPin>::setOutput(LOW);  // Send 1 high bit to the shift register chain
      //delayMicroseconds(2);
      delay(1000);
      FastGPIO::Pin<clockPin>::setOutput(HIGH); // Send 1 high bit to the shift register chain
      //delayMicroseconds(2);
      delay(1000);
      FastGPIO::Pin<clockPin>::setOutput(LOW);  // Send 1 high bit to the shift register chain
      FastGPIO::Pin<dataPin>::setOutput(LOW);   // Send 1 high bit to the shift register chain
      FastGPIO::Pin<latchPin>::setOutput(HIGH); // Send 1 high bit to the shift register chain
      //delayMicroseconds(2);
      delay(1000);
      
      for (int col=0; col<8; col++) // Changing cols is fast, so it's the inner loop
      {
        inputVal = readMux(row);
        Serial.write((byte)map(inputVal, 0, 1023, 1, 255)); // Actually write 1 byte to serial
        FastGPIO::Pin<latchPin>::setOutput(LOW);  // Shift shift register data 1 step
        FastGPIO::Pin<clockPin>::setOutput(HIGH); // Shift shift register data 1 step
        delayMicroseconds(2);
        FastGPIO::Pin<latchPin>::setOutput(HIGH); // Shift shift register data 1 step
        FastGPIO::Pin<clockPin>::setOutput(LOW);  // Shift shift register data 1 step
        delayMicroseconds(2);
        
      }
  }
  Serial.write(0); // Mark the beginning of the next frame
}

int readMux(int row)
{
  if (row < 8)
    inputVal = analogRead(A0);
  else if (row < 16)
    inputVal = analogRead(A1);
  else if (row < 24)
    inputVal = analogRead(A2);
  else if (row < 32)
    inputVal = analogRead(A3);
  else if (row < 40)
    inputVal = analogRead(A4);
  else
    inputVal = analogRead(A5);
}
/*
int adc_single_channel_read(byte readAddress)
{
  byte dataMSB =    0;
  byte dataLSB =    0;
  byte JUNK    = 0x00;

  PORTB = PORTB & 0xFB;
  SPI.transfer         (0x01);                                 // Start Bit
  dataMSB =            SPI.transfer(readAddress << 4) & 0x03;  // Send readAddress and receive MSB data, masked to two bits
  dataLSB =            SPI.transfer(JUNK);                     // Push junk data and get LSB byte return
  PORTB = PORTB | 0x04;
  return               dataMSB << 8 | dataLSB;
}
*/
