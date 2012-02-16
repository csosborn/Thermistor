Thermistor
==========

The Thermistor class provides easy access to a thermistor device and supports onboard three-point calibration using the Steinhart-Hart equation.

Dependencies
------------

Thermistor requires the SafetyPin library, which may be found [here](https://github.com/csosborn/SafetyPin).

Usage
-----

### Circuit

A thermistor is a passive two-terminal component with a resistance strongly dependent on temperature. It is easy to measure a thermistor's resistance by constructing a voltage divider using a known resistance and measuring the resulting potential:

![schematic](https://raw.github.com/csosborn/Thermistor/master/docs/basic_circuit.png "Voltage divider configuration")

### Calibration

Although the resistance varies with temperature, getting from the observed resistance to the actual temperature isn't trivial. Most thermistors are highly nonlinear, so naive two-point calibration only works over small intervals. Fortunately, the behavior is well described by the [Steinhart-Hart](http://en.wikipedia.org/wiki/Steinhart–Hart_equation) equation. This empirically-derived relation includes three coefficients, and calibration amounts to solving for their values given three observed temperatures and resistances.

The Thermistor library makes this calibration process relatively easy. When a new Thermistor object is created it is uncalibrated and will return incorrect temperature values. Before using it, you should call ```calibrate(t1, v1, t2, v2, t3, v3) ```, with three observed temperature (°F) and voltage ([0..1023]) values, well separated and spanning at least the temperature range of interest. Thermistor will internally calculate the corresponding coefficients and use them for subsequent calls to ``` temperature() ```. If you want to store the calibration data across restarts, just store the inputs and call ``` calibrate() ``` in your setup method.


**Synopsis:**

```Cpp
Thermistor(AnalogIn& pin, unsigned int Rb);
```

Constructs a thermistor object.
 * ``` AnalogIn& pin ``` analog input pin attached to GND via a resistor of value Rb ohms and to Vcc via the thermistor.
 * ``` unsigned int Rb ``` value in ohms of the balance resistor

```Cpp
temperature_t temperature();
```

Returns the current temperature in °F.

```Cpp
float raw();
```

Returns a raw analog reading from the temperature sensor [0 .. 1023], averaged to reduce noise but unscaled.

```Cpp
void calibrate(  temperature_t T1, const float& V1, 
                 temperature_t T2, const float& V2, 
                 temperature_t T3, const float& V3);
```

Calibrates the probe given three temperatures in Fahrenheit and three corresponding analog sensor readings in the range [0..1023].
 * ``` temperature_t T1, T2, T3 ``` temperature values for the three sample points
 * ``` float V1, V2, V3 ``` corresponding sensor reading values for the three samples
