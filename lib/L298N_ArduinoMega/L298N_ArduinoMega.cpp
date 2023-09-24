#include "L298N_ArduinoMega.h"
#include <Arduino.h>

L298N::L298N(int ena, int in1, int in2, int enb, int in3, int in4)
{
  this->_ena = ena;
  this->_in1 = in1;
  this->_in2 = in2;
  this->_in3 = in3;
  this->_in4 = in4;
  this->_enb = enb;

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void L298N::moveForward()
{
  digitalWrite(this->_in1, LOW);
  digitalWrite(this->_in2, HIGH);
  digitalWrite(this->_in3, HIGH);
  digitalWrite(this->_in4, LOW);
}

void L298N::moveBackward()
{
  digitalWrite(this->_in1, HIGH);
  digitalWrite(this->_in2, LOW);
  digitalWrite(this->_in3, LOW);
  digitalWrite(this->_in4, HIGH);
}

void L298N::moveLeft()
{
  digitalWrite(this->_in1, LOW);
  digitalWrite(this->_in2, HIGH);
  digitalWrite(this->_in3, LOW);
  digitalWrite(this->_in4, LOW);
}

void L298N::moveRight()
{
  digitalWrite(this->_in1, LOW);
  digitalWrite(this->_in2, LOW);
  digitalWrite(this->_in3, HIGH);
  digitalWrite(this->_in4, LOW);
}

void L298N::stopMotors()
{
  digitalWrite(this->_in1, LOW);
  digitalWrite(this->_in2, LOW);
  digitalWrite(this->_in3, LOW);
  digitalWrite(this->_in4, LOW);
}

void L298N::setSpeed(int speed)
{
  this->speed = speed;
  analogWrite(this->_ena, speed);
  analogWrite(this->_enb, speed);
}
