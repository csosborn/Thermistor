#ifndef temperature_probe_h
#define temperature_probe_h

#include "WProgram.h"
#include <SafetyPin.h>

typedef float temperature_t;

class Thermistor {
  private:
    AnalogIn& m_pin;
    
    // balance resistor value, in ohms.
    unsigned int m_Rb;
    
    // Steinhart-Hart coefficients
    float c1;
    float c2;
    float c3;
    
    // Calculate the thermistor resistence implied by the given analog reading.
    float RtForV(float V);

  public:
        
    // Construct a thermistor probe using the given analog input pin.
    Thermistor(AnalogIn& pin, unsigned int Rb);
  
    // Read the current temperature in °F
    temperature_t temperature();
    
    // Get a raw analog reading from the temperature sensor, averaged but unscaled [0 .. 1023].
    float raw();
    
    /**
     * Calibrate the probe given three temperatures in Fahrenheit and three corresponding
     * analog sensor readings in the range [0..1023].
     *
     * @param T1, T2, T3 temperature_t values for the three sample points
     * @param V1, V2, V3 float values for the three sensor readings
     */
    void calibrate(  temperature_t T1, const float& V1, 
                     temperature_t T2, const float& V2, 
                     temperature_t T3, const float& V3);
                 
    // Convert Fahrenheit to Kelvin
    static temperature_t FtoK(temperature_t F);

    // Convert Kelvin to Fahrenheit
    static temperature_t KtoF(temperature_t K);
    
};

#endif
