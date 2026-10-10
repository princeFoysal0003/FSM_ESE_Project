# ESP32-based Voltage Protector Device Simulation

## How to run the simulation?
I have completed my project in wokwi.com. Following is the simulation link.
Link: https://wokwi.com/projects/477390728075774977

Clicking the green play button starts the simulation after a few seconds (usually around 10 s).

## How to check whether it works?
After starting the simulation, press the reset button once. If everything's okay, the LCD display will show Voltage and Current in real-time. There are two potentiometer in the circuit. One of them mimics the voltage input from PT, and another does the job of current from the CT. These two inputs are fed to the ESP32. Both voltage and current values can be seen in the LCD display.

### Overvoltage and Overcurrent Protection
To change the voltage the dial of the **voltage potentiometer needs to be rotated**. Changed values can be seen in the LCD. If the voltage falls below the minimum threshold (207 V by default) or rises above the maximum threshold (253 V by default), the device detects fault and the relay operates and consequently the appliance gets disconnected from the supply. This feature protects the appliance from **overvoltage and overcurrent faults**.

### What to Do after A Fault?
The (**green**) button should be pressed to get the device back to its running condition each time after a fault is occured.

### Surge Voltage Protection
In that case, the device does not wait until the voltage crosses limits. Instead it detects the spike of the voltage and immediately disconnects the appliance. This is the protection against the **surge voltage** (typically occurs due to lightning strike in the grid power lines).

**But, how to check it in the simulation?** 
To do this, the minimum threshold should be reduced to a very low value (e.g. 20 V) and the maximum threshold should be set to a much higher value (e.g. 300V). So that we can vary the voltage rapidly without violating the threshold limits. Thus, we can be sure that the voltage spike (surge voltage fault) can be detected by the device and in that case the appliance should get separated.

### Function of the Current Potentiometer
It's very simple. Rotate the dial of the current potentiometer. If the current exceeds its current rating, the relay and circuit breaker should trip. Thus a fault is detected and the appliance is detached from the supply.

### Change the Settings
1. Press the blue button once to enter settings option.
	1. Press the red button to increase the threshold voltage
	2. Press the yellow button to decrease the threshold voltage
2. Press the blue button again
	1. Press the red button to increase current threshold
	2. Press the yell button to decrease current threshold
3. Press the blue button again to get back to the voltage-current monitor again.
