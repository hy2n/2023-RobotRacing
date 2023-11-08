/*
* 2023 천안학생로봇대회 로봇 4Watt
* 
* Coder:hy2n,0813jake
* https://by.donghyun.cc/software
*
* 모든 코드에는 MIT 라이선스가 적용됩니다
*/
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
Servo servo;

/*
* Port Config
*/

#define A_TRIG A0
#define A_ECHO A1
#define B_TRIG A2
#define B_ECHO A3
#define L_IR 11
#define R_IR 12
#define FADE_AMOUNT 15


int pin = 3;
int numPixels = 8;
int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel *pixels;
#define DELAYVAL 500
int motorA1 = 4;
int motorA2 = 5;
int motorB1 = 6;
int motorB2 = 7;

void setup() {
  Serial.begin(9600);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A_TRIG, OUTPUT);
  pinMode(A_ECHO, INPUT);
  pinMode(B_TRIG, OUTPUT);
  pinMode(B_ECHO, INPUT);
  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);

  servo.attach(A5);
  pixels = new Adafruit_NeoPixel(numPixels, pin, pixelFormat);
  pixels->begin();
  Animation();
  StartVM();
}
//====================================================================================//

void loop() {
  while (NoObject(30)) {  //1차
    Forward();
  }
  Stop();
  NeoPixel();
  delay(50);

  Stop();
  delay(50);

  R(300);

  while (NoObject(20)) {  //2차
    Forward();
  }

  Backward();
  delay(500);
  Stop();
  delay(130);

  L(300);
  delay(150);
  Stop();
  Trace_R(37,35); //3차

  Stop();
  Sound();
  servo.write(90);
  delay(50);
  servo.write(90);
  delay(50);
  L(300);
  Stop();
  delay(50);

  while (NoObject(20)) {  //4차
    Forward();
  }
  Recalibrate();
  Backward();
  delay(200); //변경금지
  Stop();
  delay(100);

  R(300);
  Stop();
  delay(50);

  while (NoObject(20)) {  //4차
    Forward();
  }
  delay(100);
  End();
}

//====================================================================================//


void Forward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void Backward() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void Stop() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void Right() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void Left() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}


int Read_F() {
  long duration, distance;
  duration = 0;
  distance = 0;
  digitalWrite(A_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(A_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(A_TRIG, LOW);
  duration = pulseIn(A_ECHO, HIGH);
  distance = duration * 17 / 1000;
  return distance;
}

int Read_B() {
  long duration, distance;
  duration = 0;
  distance = 0;
  digitalWrite(B_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(B_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(B_TRIG, LOW);
  duration = pulseIn(B_ECHO, HIGH);
  distance = duration * 17 / 1000;
  return distance;
}

void Animation() {
  for (int repeat = 0; repeat < 2; repeat++) {
    pixels->clear();
    for (int i = 0; i < numPixels; i++) {
      int red = random(156);
      int green = random(156);
      int blue = random(156);
      pixels->setPixelColor(i, pixels->Color(red, green, blue));
      pixels->setPixelColor(i - 1, pixels->Color(0, 0, 0));
      pixels->show();
      delay(50);
    }
  }
  noTone(8);
  pixels->clear();
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(255, 255, 255));
    pixels->show();
    delay(50);
  }
  for (int brightness = 255; brightness >= 0; brightness -= FADE_AMOUNT) {
    for (int i = 0; i < numPixels; i++) {
      pixels->setPixelColor(i, pixels->Color(brightness, brightness, brightness));
    }
    pixels->show();
    delay(50);  // 0.05초 대기
  }

  // 다시 밝게 만들기
  for (int brightness = 0; brightness <= 255; brightness += FADE_AMOUNT) {
    for (int i = 0; i < numPixels; i++) {
      pixels->setPixelColor(i, pixels->Color(brightness, brightness, brightness));
    }
    pixels->show();
    delay(50);  // 0.05초 대기
  }
  for (int brightness = 255; brightness >= 0; brightness -= FADE_AMOUNT) {
    for (int i = 0; i < numPixels; i++) {
      pixels->setPixelColor(i, pixels->Color(brightness, brightness, brightness));
    }
    pixels->show();
    delay(50);  // 0.05초 대기
  }
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 0));
  }
  pixels->clear();
  pixels->show();
}

void NeoPixel() {
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(255, 0, 0));
  }
  pixels->show();
  delay(500);
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 255));
  }
  pixels->show();
  delay(500);
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(255, 0, 0));
  }
  pixels->show();
  delay(500);
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 255));
  }
  pixels->show();
  delay(500);
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(255, 0, 0));
  }
  pixels->show();
  delay(500);
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 255));
  }
  pixels->show();
  delay(500);
  
  for (int i = 0; i < numPixels; i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 0));
  }
  pixels->clear();
  pixels->show();
}

int NoObject(int value) {
  if (value > Read_F()) {
    delay(10);
    if (value > Read_F()) {
      return 0;
    } else return 1;
  } else return 1;
}

void Recalibrate() {  //LOW HIGH 반대임
  
}

void Buzz(int time) {
  tone(8, 1174);
  delay(time);
  noTone(8);
}

int NoObject_V() {
  if ((digitalRead(L_IR) == LOW) || (digitalRead(R_IR) == LOW)) {
    delay(50);
    if ((digitalRead(L_IR) == LOW) || (digitalRead(R_IR) == LOW)) {
      return 0;
    } else return 1;
  } else return 1;
}

void Sound() {
  tone(8, 2000);
  delay(600);
  noTone(8);
  delay(100);

  tone(8, 2000);
  delay(150);
  noTone(8);
  delay(50);

  tone(8, 2000);
  delay(300);
  noTone(8);
  delay(100);

  tone(8, 2000);
  delay(150);
  noTone(8);
  delay(200);

  tone(8, 2000); //박수
  delay(150);
  noTone(8);
  delay(50);

  tone(8, 2000);
  delay(300);
  noTone(8);
  delay(100);

  tone(8, 2000);
  delay(150);
  noTone(8);
  delay(50);

  tone(8, 2000);
  delay(300);
  noTone(8);
  delay(100);

  tone(8, 2000);
  delay(300);
  noTone(8);
  delay(100);
}

void Trace_L(int value,int value_2) {
  servo.write(170);
  delay(100);
  servo.write(170);
  delay(100);
  while (1) {
    if ((digitalRead(L_IR) == LOW) && (digitalRead(R_IR) == LOW)) {
      delay(50);
      if ((digitalRead(L_IR) == LOW) && (digitalRead(R_IR) == LOW)) return 0;
    }
    int Sensor = Read_F();
    if (Sensor < value_2) {
      Stop();
      delay(100);
      Right();
      delay(150);
      Forward();
      delay(100);
      Stop();
    } else if (Sensor > value) {
      Stop();
      delay(100);
      Left();
      delay(150);
      Forward();
      delay(100);
      Stop();
    } else {
      Forward();
      delay(100);
    }
    delay(200);
  }
}


void Trace_R(int value, int value_2) { //최대값 , 최솟값 기본 ( 13 , 8 )
  servo.write(10);
  delay(100);
  servo.write(10);
  delay(100);
  while (1) {
    if (Read_B() < 10) {
      delay(50);
      if (Read_B() < 10) return 0;
    }
    int Sensor = Read_F();
    if (Sensor < value_2) {
      Stop();
      delay(100);
      Left();
      delay(50);
      Forward();
      delay(100);
      Stop();
    } else if (Sensor > value) {
      Stop();
      delay(100);
      Right();
      delay(50);
      Forward();
      delay(100);
      Stop();
    } else {
      Forward();
      delay(100);
    }
    delay(200);
  }
}

void GoBackLittle() {
  Stop();
  delay(200);
  Backward();
  delay(250);
  Stop();
  delay(200);
}

void StartVM() {
  while (1) {
    if ((digitalRead(L_IR) == LOW) && (digitalRead(R_IR) == HIGH)) {  //L쪽만 물체 감지
      pixels->setPixelColor(0, pixels->Color(0, 0, 0));
      pixels->setPixelColor(1, pixels->Color(0, 0, 0));
      pixels->setPixelColor(2, pixels->Color(0, 0, 0));
      pixels->setPixelColor(3, pixels->Color(0, 0, 0));
      pixels->setPixelColor(4, pixels->Color(255, 0, 0));
      pixels->setPixelColor(5, pixels->Color(255, 0, 0));
      pixels->setPixelColor(6, pixels->Color(255, 0, 0));
      pixels->setPixelColor(7, pixels->Color(255, 0, 0));
      pixels->show();
      delay(50);
    } else if ((digitalRead(L_IR) == HIGH) && (digitalRead(R_IR) == LOW)) {  //R쪽만 물체 감지
      pixels->setPixelColor(0, pixels->Color(0, 0, 255));
      pixels->setPixelColor(1, pixels->Color(0, 0, 255));
      pixels->setPixelColor(2, pixels->Color(0, 0, 255));
      pixels->setPixelColor(3, pixels->Color(0, 0, 255));
      pixels->setPixelColor(4, pixels->Color(0, 0, 0));
      pixels->setPixelColor(5, pixels->Color(0, 0, 0));
      pixels->setPixelColor(6, pixels->Color(0, 0, 0));
      pixels->setPixelColor(7, pixels->Color(0, 0, 0));
      pixels->show();
      delay(50);
    } else if ((digitalRead(L_IR) == LOW) && (digitalRead(R_IR) == LOW)) {  //둘다 감지
      pixels->setPixelColor(0, pixels->Color(0, 255, 0));
      pixels->setPixelColor(1, pixels->Color(0, 255, 0));
      pixels->setPixelColor(2, pixels->Color(0, 255, 0));
      pixels->setPixelColor(3, pixels->Color(0, 255, 0));
      pixels->setPixelColor(4, pixels->Color(0, 255, 0));
      pixels->setPixelColor(5, pixels->Color(0, 255, 0));
      pixels->setPixelColor(6, pixels->Color(0, 255, 0));
      pixels->setPixelColor(7, pixels->Color(0, 255, 0));
      pixels->show();
      delay(50);
      pixels->setPixelColor(0, pixels->Color(0, 0, 0));
      pixels->setPixelColor(1, pixels->Color(0, 0, 0));
      pixels->setPixelColor(2, pixels->Color(0, 0, 0));
      pixels->setPixelColor(3, pixels->Color(0, 0, 0));
      pixels->setPixelColor(4, pixels->Color(0, 0, 0));
      pixels->setPixelColor(5, pixels->Color(0, 0, 0));
      pixels->setPixelColor(6, pixels->Color(0, 0, 0));
      pixels->setPixelColor(7, pixels->Color(0, 0, 0));
      pixels->show();
      return;
    } else {
      pixels->setPixelColor(0, pixels->Color(0, 0, 0));
      pixels->setPixelColor(1, pixels->Color(0, 0, 0));
      pixels->setPixelColor(2, pixels->Color(0, 0, 0));
      pixels->setPixelColor(3, pixels->Color(0, 0, 0));
      pixels->setPixelColor(4, pixels->Color(0, 0, 0));
      pixels->setPixelColor(5, pixels->Color(0, 0, 0));
      pixels->setPixelColor(6, pixels->Color(0, 0, 0));
      pixels->setPixelColor(7, pixels->Color(0, 0, 0));
      pixels->show();
      delay(50);
      noTone(8);
    }
  }
}

void End() {
  while (1) {
    Stop();
    pixels->clear();
    for (int brightness = 0; brightness <= 255; brightness += FADE_AMOUNT) {
      for (int i = 0; i < numPixels; i++) {
        pixels->setPixelColor(i, pixels->Color(brightness, brightness, brightness));
      }
      pixels->show();
      delay(50);  // 0.05초 대기
    }
    for (int brightness = 255; brightness >= 0; brightness -= FADE_AMOUNT) {
      for (int i = 0; i < numPixels; i++) {
        pixels->setPixelColor(i, pixels->Color(brightness, brightness, brightness));
      }
      pixels->show();
      delay(50);  // 0.05초 대기
    }
  }
}

void R(int value) {
  Right();
  delay(value);
  Stop();
  delay(100);
}

void L(int value) {
  Left();
  delay(value);
  Stop();
  delay(100);
}
