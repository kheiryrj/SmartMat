const int latchPin = 5;               //RCLK
const int clockPin = 6;               //SRCLK
const int dataPin = 7;                //SER
const int selectPins[3] = {4, 3, 2};  //shift register data select pins A= 4 B=3 C=2
const int rowMap[8] = {0,1,2,3,4,5,6,7}; //each mux reads 8 rows
int inputVal = 1;

void setup() {
  pinMode(latchPin, OUTPUT);  
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(A0, INPUT); // Y
  pinMode(A1, INPUT); // W
  for (int i=0; i<3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
  
  Serial.begin(57600);
  Serial.println(0);
}

void loop() {
  //8 rows (blue, red and green wires)
  for (int row=0; row< 8; row++) 
  {
    selectMuxPin(rowMap[row%8]);
     //14 columns (blue, white and yellow wires) 
    for (byte col=0; col<14; col++)
    {
      selectRegisterPin(col);
      inputVal = analogRead(A0);
      /*
      if (row < 8)
        inputVal = analogRead(A0);
      else if (row < 16)
        inputVal = analogRead(A1);
     for now, we only have 1 mux
      else
        inputVal = analogRead(A2);
 */
      inputVal = map(inputVal, 0, 1023, 1, 255);
      Serial.println(inputVal);
    }
  }
  Serial.println(0);
}

void selectRegisterPin(int col){
  digitalWrite(latchPin, LOW);
  byte bitsToSend = 0;
  bitWrite(bitsToSend, col%8, HIGH);
  if (col < 8){
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
  }
  else if (col < 16){
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
  }
  else {
    shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
  }
  digitalWrite(latchPin, HIGH);
}

void selectMuxPin(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}
