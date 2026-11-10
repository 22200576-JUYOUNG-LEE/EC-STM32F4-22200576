# Smart-Farm Management System[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#smart-farm-management-system)

Date: 2026-11-01

## I. Introduction[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#i-introduction)

### Overview[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#overview)

Design an STM32-based smart farm system that provides light-triggered supplemental lighting, temperature and humidity ventilation, and a dry-soil alarm.

![Project concept illustration](https://ykkimhgu.github.io/EC-STM32F4-Book/assets/ch18/Smart_Farm_Overview.webp)

*Figure 1. Smart farm concept illustration.*

![System architecture diagram](https://ykkimhgu.github.io/EC-STM32F4-Book/assets/ch18/Smart_Farm_System_Overview.webp)

*Figure 2. System overview with the given sensors, displays and actuators.*

### Requirements[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#requirements)

#### Hardware[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#hardware)

| Category           | Component                                                    |         Qty | Notes                                                        |
| :----------------- | :----------------------------------------------------------- | ----------: | :----------------------------------------------------------- |
| **MCU**            | STM32 Nucleo-F411RE board                                    |           1 | Makes all control decisions                                  |
| **Digital Sensor** | DHT11 temperature / humidity sensor                          |           1 | Needs an external pull-up on the data line                   |
|                    | Push button                                                  |           1 | On-board B1 button; toggles AUTO / MANUAL                    |
| **Analog Sensor**  | Light sensor                                                 |           1 | Mount **inside** the greenhouse, shielded from the LED panel's direct beam |
|                    | Moisture sensor                                              |           1 | Checks the moisture in the soil                              |
| **Display**        | I2C LCD                                                      |           1 | use provided I2C library                                     |
| **Actuator**       | LED lighting panel                                           |           1 | One PWM input; the duty sets the brightness                  |
|                    | Ventilation fan                                              |           1 | needs DC motor driver                                        |
|                    | Window servo motor                                           |           1 | Operates the ventilation window; separate supply             |
| **Communication**  | Bluetooth module                                             |           1 | Serial link to USART6                                        |
| **Others**         | Acrylic greenhouse, hydroponic container, four planting cups, growing medium, mounting parts | As supplied |                                                              |

> The kit has no water pump, so dry soil raises the `DRY` alarm instead of watering.

#### Software[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#software)

- IDE: VS Code
- Compiler: PlatformIO `framework = cmsis`
- Library: EC-STM32F4
- Extra Library: EC-STM32F4 `ecDHT11`, `ecI2C2`
  - Refer to `TU_DHT11.c` and `TU_I2C_LCD.c`

## II. Problem[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#ii-problem)

### Overview[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#overview_1)

Build a smart farm system that controls the supplemental lighting, ventilation and warnings under the following conditions.

| State      | Condition        | Required behavior                                            |
| :--------- | :--------------- | :----------------------------------------------------------- |
| **AUTO**   | `mode == AUTO`   | Supplemental LED lighting set from the light sensor. Automatic ventilation window and fan. |
| **MANUAL** | `mode == MANUAL` | Lighting, ventilation, fan follow Bluetooth commands.        |
| **ALARM**  | `alarm == DRY`   | When the soil is too dry. Clearing the alarm returns to previous mode, AUTO or MANUAL as selected. |

- The push-button selects AUTO/MANUAL.
- Default is MANUAL
- At power-on the outputs start at **LED OFF, fan OFF, window CLOSED**
- The **light sensor alone sets the LED panel brightness**.
- Use hysteresis on the two threshold crossings so the panel does not toggle at a boundary, and give the panel a **minimum on-time** so that it cannot switch faster than the plants or the demonstration need.
- AUTO starts a **5 s delay** after push-button to detect the light.

### Basic Functions[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#basic-functions)

| Function        | Required behavior                                            |
| :-------------- | :----------------------------------------------------------- |
| **Lighting**    | In AUTO, drive the LED panel from the light sensor so that the LEDs make up for weak light: full brightness below the dark threshold, off above the bright threshold, and a proportional duty in between. Under mode='ALARM' **blink** the panel |
| **Ventilation** | In AUTO, run the ventilation fan and open the servo window when the temperature **or** the humidity exceeds its upper threshold. Stop and close only after **both** readings fall below their lower thresholds. |
| **Moisture**    | In every state, raise `DRY` after the reading stays **5 s** below the dry threshold. Clear it after **5 s** above the recovery threshold. No automatic watering is required. |
| **Button**      | Pressing the button toggles AUTO/MANUAL. Acknowledge the alarm through Bluetooth, display on LCD |
| **LCD**         | Shows the current state and specific information as instructed |

> Select appropriate threshold for light-sensor, moisture, temperature and humidity. Ventilation is ordinary comfort control, so choose a mildly warm temperature rather than a dangerous one.
>
> Breathing on the DHT11, or cupping both hands around it, moves the reading enough to start the ventilation, so no extra equipment is needed to demonstrate it.
>
> **The alarm does not affect the ventilation or the window.** They follow their own rule at all times: the temperature and humidity thresholds in AUTO, and the Bluetooth command in MANUAL. `DRY` changes the LED panel and the LCD only.

### Light and Display Response[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#light-and-display-response)

The LED panel is driven by **one PWM output**, and the duty sets the brightness. Show exactly one of the following, by the **first matching row**:

- ALARM = DRY: **blink** every 1 s, in every mode
- AUTO, too dark: **ON**, 100 % duty
- AUTO, very bright: **OFF**, 0 % duty
- AUTO, between those two levels: **ON**, duty set from the light sensor, **falling as the measured light rises**
- MANUAL: follow the Bluetooth command

For LCD display, you need to show

- State name: AUTO, MANUAL, ALARM
- State = AUTO, MANUAL
  - Temp, humidity reading, LED duty (00°, 00%, LED 70%)
- State + Ventilation
  - Temp, humidity reading, VENT (00°, 00%, VENT)
- State = ALARM
  - ADD WATER (blink 1 sec)

> If there is a priority conflict in the display condition, you can select the priority with appropriate explanations.
>
> **Shielding the light sensor from the LED panel is a requirement, not a suggestion.** The sensor sits inside the greenhouse, so the panel it controls also shines on it, and the duty is set continuously from that reading. Left unshielded the two form a closed loop: the panel brightens, the reading rises, the duty falls, the reading drops, and the brightness hunts instead of settling. Mount the sensor facing the glazing, or behind a small shield, so that only a small part of the panel's light reaches it. Report where you mounted it and how you confirmed that the brightness settles.

### Communication Message to server[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#communication-message-to-server)

Using the bluetooth, send a message and display on your laptop's monitor whenever a condition is changed. You may choose the message content.

- Remote Control : You can select MANUAL and command the LED duty, the fan and the window. You can also turn-off the alarm message. e.g. `ADD WATER` acknowledgment
  - You can choose the command key.
  - Acknowledgment stops the repeated message only. The alarm stays active until the moisture reading recovers.

### Interrupt, Timing and Fault Handling[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#interrupt-timing-and-fault-handling)

- Use at least 1 EXTI IRQ
- Use at least 1 timer update event interrupt IRQ (control loop etc), 500msec tick could be appropriate for this project.
- Report invalid sensor readings and watch out for a long blocking delays.
- Use JADC intead of ADC. Choose appropriate JADC TRIG Timing.
- If the DHT11 reading stays invalid in AUTO, run the fan and open the window until valid readings return. In MANUAL, report the fault without overriding commands.

## III. Hardware Configuration[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#iii-hardware-configuration)

### MCU Configuration[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#mcu-configuration)

You need to use the following pin configuration to control the given hardware.

- Do NOT change the pin
- But, you may choose a specific option such as timer period etc.
- Need to re-write the configuration table that explains your project configuration

| Function           | Peripheral                                      | Pin (Arduino)                    | Configuration       |
| :----------------- | :---------------------------------------------- | :------------------------------- | :------------------ |
| Ventilation fan    | TIM2 CH1 (PWM)                                  | PA_15 (D32)                      | 1 ms (1 kHz)        |
| Window servo       | TIM3 CH2 (PWM)                                  | PC_7 (D9)                        | 20 ms (50 Hz)       |
| LED lighting panel | TIM4 CH2 (PWM)                                  | PB_7                             | 1 ms (1 kHz)        |
| Control loop       | TIM1 (update interrupt)                         | none                             | 500 ms              |
| Light sensor       | ADC1 injected, result in JDR1                   | PA_0 (A0)                        | TIM5 TRGO of 125 ms |
| Moisture sensor    | ADC1 injected, result in JDR2                   | PA_1 (A1)                        | TIM5 TRGO of 125 ms |
| DHT11              | GPIO, bit-banged (SysTick µs timing, `ecDHT11`) | PA_6 (D12)                       | one read per 2 s    |
| Push button        | GPIO + EXTI13                                   | PC_13 (on-board B1)              | interrupt on press  |
| I2C LCD            | I2C1                                            | PB_8 (D21, SCL), PB_9 (D20, SDA) | —                   |
| Bluetooth          | USART6                                          | PA_11 (D18, TX), PA_12 (D19, RX) | 9600 baud           |
| Debug serial       | USART2                                          | PA_2 (TX), PA_3 (RX)             | —                   |
| Spare              | TIM4 CH1                                        | PB_6 free                        | —                   |

> The LED panel's default shield position is PB_5, which is TIM3 CH2 and would collide with the servo. Move its signal wire to PB_7.

### Circuit Diagram[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#circuit-diagram)

Draw a computer-generated circuit diagram showing necessary parts (sensor, actuator etc).

Add a circuit diagram or results image in the project report.

## IV. Algorithm[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#iv-algorithm)

### Logic Design[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#logic-design)

In the report, you need to explain concisely how you designed the system with state tables/diagram or flow-chart.

Listing all necessary states (states, input, output etc) to implement this design problem.

Listing all necessary conditional FLAGS for programming.

Showing the logic flow from the initialization

and more

### Code[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#code)

Your code (private repository) goes here: ADD Code LINK such as github

DO NOT paste the whole code here!

> Show only necessary code blocks with concise explanations that show the basic logic flow of your project.

*Concise Explanation Goes here*

```
//Show only necessary code blocks with comments
```

*Concise Explanation Goes here*

```
//Show only necessary code blocks with comments
```

## V. Results and Analysis[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#v-results-and-analysis)

### Results[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#results)

**Demo Video Link: [add demo video link]**

> For demonstrating the alarm, move the moisture probe between the prepared dry and moist test cups.

Show important result images to explain your output for key processes.

Add a circuit diagram or results image in the project report.

### Analysis[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#analysis)

Compare expected and observed behavior, report measured thresholds and response, and describe any limitations or failures, if there is any

## VI. Reference[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#vi-reference)

List all external sources used, including datasheets, libraries, diagrams, websites, and reused code, with links or sufficient identifying details.

> If you got AI assistance, need to include how you have used it, include the prompt link.

## Appendix[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#appendix)

### 1. Additional Libray[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#1-additional-libray)

#### DHT11 API: `ecDHT11.h`[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#dht11-api-ecdht11h)

- [Example Code for DHT](https://github.com/ykkimhgu/EC-student/blob/main/tutorial/tutorial-student/TU_DHT11.c)
  - The example sets `DHT11_PIN` to `PB_12`. This project wires the sensor to **PA_6**, so change that `#define` before you run it.
- `DHT11_read()` blocks for about 25 ms, of which roughly 5 ms runs with interrupts off.
- Do not call it from inside an interrupt handler.
- Treat `DHT11_ERR_CHECKSUM` and `DHT11_ERR_NO_REPLY` as invalid readings: hold the last valid values, report the fault, and count the failures for your report.

#### I2C API: `ecI2C2.h`[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#i2c-api-eci2c2h)

- [Example Code for I2C LCD](https://github.com/ykkimhgu/EC-student/blob/main/tutorial/tutorial-student/TU_I2C_LCD.c)

### 2. Other Appendix[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#2-other-appendix)

Include supporting measurements, calibration data, configuration tables, or supplementary diagrams that are too detailed for the main report.

### 3. Troubleshooting[¶](https://ykkimhgu.github.io/EC-STM32F4-Book/chapters/ch18-project/06-smart-farm.html#3-troubleshooting)

Describe significant hardware or software problems, how they were diagnosed and resolved, and any unresolved issues.