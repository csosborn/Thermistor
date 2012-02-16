#include "WProgram.h"
#include "Thermistor.h"
#include <math.h>

float det3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
  return a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h; 
}

Thermistor::Thermistor(AnalogIn& pin, unsigned int Rb): m_pin(pin), m_Rb(Rb) {}

temperature_t Thermistor::temperature() {
  float lnR = log(RtForV(raw()));
  float K = 1.0 / (c1 + (c2 * lnR) + (c3 * lnR * lnR * lnR));
  return KtoF(K);
}

float Thermistor::raw() {
  int samples = 5;
  
  float rawVal = 0; 
  for (int sample = 0; sample < samples; sample++) {
    rawVal += m_pin.read();
  }
  
  return rawVal / samples;
}

void Thermistor::calibrate(
  temperature_t T1, const float& V1, 
  temperature_t T2, const float& V2, 
  temperature_t T3, const float& V3) {
  
  // @TODO: the code below uses Cramer's Rule on a 3x3 matrix to solve for
  // the three Steinhart-Hart coefficients. This works fine, but it might 
  // be more compact to use Gaussian elimination. Worth considering if 
  // space is tight. 

  float lnR1 = log(RtForV(V1));
  float lnR2 = log(RtForV(V2));
  float lnR3 = log(RtForV(V3));
  float lnR1cb = lnR1*lnR1*lnR1;
  float lnR2cb = lnR2*lnR2*lnR2;
  float lnR3cb = lnR3*lnR3*lnR3;
  
  T1 = 1.0 / FtoK(T1);
  T2 = 1.0 / FtoK(T2);
  T3 = 1.0 / FtoK(T3);
  
  float det = det3(1, lnR1, lnR1cb, 1, lnR2, lnR2cb, 1, lnR3, lnR3cb);

  c1 = det3(T1, lnR1, lnR1cb, T2, lnR2, lnR2cb, T3, lnR3, lnR3cb) / det;
  c2 = det3(1, T1, lnR1cb, 1, T2, lnR2cb, 1, T3, lnR3cb) / det;
  c3 = det3(1, lnR1, T1, 1, lnR2, T2, 1, lnR3, T3) / det;
}

float Thermistor::RtForV(float V) {
  return m_Rb * (1024.0/V - 1);
}

temperature_t Thermistor::FtoK(temperature_t F) {
  return (F - 32) * 5/9 + 273.15;
}

temperature_t Thermistor::KtoF(temperature_t K) {
  return (K - 273.15) * 9/5 + 32;
}

