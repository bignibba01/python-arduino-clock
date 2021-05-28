//04/04/2021 Marchetti Davide
//Clock v.4

#include <TimeLib.h>

//---VARIABLE AND CONST---
const int a = 2;
const int b = 3;
const int c = 4;
const int d = 5;
const int e = 6;
const int f = 7;
const int g = 8;
const int DP = 9;
const int D1 = 10;
const int D2 = 11;
const int D3 = 12;
const int D4 = 13;

const int delayTime = 5;
char sysTime[4];
char h1 = 'w';
char h2 = 'w';
char m1 = 'w';
char m2 = 'w';

int rcvHour;
int rcvMinute;
int rcvSecond;     //for future use

const byte chars = 64;
char receivedChars[chars];

boolean newData = false;

byte ledPin = 13;   // the onboard LED

//---MAIN---
void setup() {
  Serial.begin(115200);

  for( int i = 0; i < sizeof(sysTime);  ++i )
   sysTime[i] = (char)0;

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(DP, OUTPUT);


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH);

  Serial.println("<Arduino is ready>");
}

//---MAIN 2 XD---
void loop() {
  
  recvWithStartEndMarkers();

//  h1 = receivedChars[0];
//  h2 = receivedChars[1];
//  rcvHour = (h1 - '0') * 10 + (h2 - '0');
//  m1 = receivedChars[2];
//  m2 = receivedChars[3];
//  rcvMinute = (m1 - '0') * 10 + (m2 - '0');
  rcvHour = (receivedChars[0] - '0') * 10 + (receivedChars[1] - '0');
  rcvMinute = (receivedChars[2] - '0') * 10 + (receivedChars[3] - '0');
  rcvSecond = (receivedChars[4] - '0') * 10 + (receivedChars[5] - '0');

  if (timeStatus() != timeSet){
    setTime(rcvHour, rcvMinute, rcvSecond, 1, 1, 20);
  
  Serial.print(hour());
  sysTime[0] = minute() / 10;
  sysTime[1] = minute() - minute() / 10;
  }
  
  digitalWrite(D1, HIGH);    //Digit 1
  pickNumber(sysTime[0]);
  delay(delayTime);
  digitalWrite(D1, LOW);
  
  digitalWrite(D2, HIGH);    //Digit 2
  pickNumber(sysTime[1]);
  delay(delayTime);
  digitalWrite(D2, LOW);
  
  digitalWrite(D3, HIGH);    //Digit 3
  pickNumber(m1);
  delay(delayTime);
  digitalWrite(D3, LOW);
  
  digitalWrite(D4, HIGH);    //Digit 4
  pickNumber(m2);
  delay(delayTime);
  digitalWrite(D4, LOW);
  
  replyToPython();
}

//---FUNCTIONS---
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte k = 0;
  char startMarker = '<';
  char endMarker = '>';
  char readFromSerial;

  while (Serial.available() > 0 && newData == false) {
    readFromSerial = Serial.read();

    if (recvInProgress == true) {
      if (readFromSerial != endMarker) {
        receivedChars[k] = readFromSerial;
        k++;
        if (k >= chars) {
          k = chars - 1;
        }
      }
      else {
        receivedChars[k] = '\0'; // terminate the string
        recvInProgress = false;
        k = 0;
        newData = true;
      }
    }

    else if (readFromSerial == startMarker) {
      recvInProgress = true;
    }
  }
}

void replyToPython() {
  if (newData == true) {
    Serial.print("<");
    Serial.print(receivedChars);
    Serial.print('>');
    
    digitalWrite(ledPin, ! digitalRead(ledPin));    // change the state of the LED everytime a reply is sent
    newData = false;
  }
}

//---THE FIRETRUCKING CODE FOR 7 SEGMENT LED---
void pickNumber(char x) {
  switch (x) {
    case '1': number1(); break;
    case '2': number2(); break;
    case '3': number3(); break;
    case '4': number4(); break;
    case '5': number5(); break;
    case '6': number6(); break;
    case '7': number7(); break;
    case '8': number8(); break;
    case '9': number9(); break;
    default: number0(); break;
  }
}

void clearLEDs()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(DP, HIGH);
}

void number1()             //Number 1
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void number2()             //Number 2
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void number3()            //Number 3
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void number4()             //Number 4
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void number5()             //Number 5
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void number6()             //Number 6
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void number7()            //Number 7
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void number8()            //Number 8
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void number9()             //Number 9
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void number0()             //Number 0
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}
