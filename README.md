# projects-ui-machine
so this is bacicly like this mini machine with a screen buttons and a potentiometer powerd by a pi pico with a 2*10 connectoir for modules so you can connect whatever you want and then control it w[...]
it also uses a 1.69 inch color lcd screen so you cou could see the name of your action and know witch one you trigger.
to be more technical the code has an array of actions and the number you press the navigation button the arry moves one so it'll know what to trigger.


<img width="3840" height="2160" alt="image" src="https://github.com/user-attachments/assets/ea75229a-99f3-4b14-bc89-091b57664958" />
<img width="3840" height="2158" alt="image" src="https://github.com/user-attachments/assets/c4be15ae-c2c4-45ed-ac13-70b5cf753a86" />
<img width="3838" height="2158" alt="image" src="https://github.com/user-attachments/assets/09735aa5-08da-4307-b668-ef489db73969" />



## Wiring diagram (inferred from PCB photos and code)

Below is a wiring diagram and pin mapping inferred from the PCB pictures included above and the pin assignments in the project's code (code11.ino / Program.cs). Please verify the silk / labels on your actual board before wiring — if anything doesn't match, send a clear photo of the PCB and I will update the diagram.

Signals used by the code

- SPI (display bus)
  - SCK (Clock)  -> ESP32 GPIO 18 (Configuration.SetPinFunction(18, DeviceFunction.SPI1_CLOCK))
  - MISO         -> ESP32 GPIO 19 (Configuration.SetPinFunction(19, DeviceFunction.SPI1_MISO))
  - MOSI         -> ESP32 GPIO 23 (Configuration.SetPinFunction(23, DeviceFunction.SPI1_MOSI))
  - CS (chip select) -> ESP32 GPIO 9 (SpiConnectionSettings bus 1, chip select 9)

- Display control pins
  - D/C (Data / Command) -> ESP32 GPIO 8 (_screenDc)
  - RST (Reset)          -> ESP32 GPIO 7 (_screenRst)

- Buttons
  - BTN4 -> ESP32 GPIO 14 (_btn4)
  - BTN5 -> ESP32 GPIO 15 (_btn5)

- Rotary encoder
  - ENC_SW (switch) -> ESP32 GPIO 11 (_encSw)
  - ENC_A            -> ESP32 GPIO 12 (_encA)
  - ENC_B            -> ESP32 GPIO 13 (_encB)

- Power and ground
  - 3.3V -> supply to display and logic
  - GND  -> common ground

Simple ASCII wiring sketch

Display (SPI)         ESP32 pins
┌─────────────┐        ┌────────────┐
│  Display    │        │  ESP32     │
│             │        │            │
│ MOSI <------+------- GPIO 23 (MOSI)
│ MISO <------+------- GPIO 19 (MISO)
│ SCK  <------+------- GPIO 18 (SCK)
│ CS   <------+------- GPIO 9  (CS)
│ D/C  <------+------- GPIO 8  (D/C)
│ RST  <------+------- GPIO 7  (RST)
└─────────────┘

Buttons & encoder     ESP32 pins
- BTN4  -> GPIO 14
- BTN5  -> GPIO 15
- ENC_SW -> GPIO 11
- ENC_A  -> GPIO 12
- ENC_B  -> GPIO 13

Notes & next steps
- The code in the repository uses nanoFramework / ESP32 pin numbers. If you are actually using a Raspberry Pi Pico (RP2040), the pin numbering is different; tell me whether you want a Pico pin mapping and I will add it.
- If you want a graphical wiring diagram (SVG or PNG) I can generate one and add it to the repo — upload a clear photo of the PCB silk (connector labels) or confirm which connector pin corresponds to each signal so the diagram matches your board exactly.


| Part                       | Price (USD) | Quantity | Link                                                                                      |
| -------------------------- | ----------: | -------: | ----------------------------------------------------------------------------------------- |
| Pi Pico                    |       $3.87 |        1 | [AliExpress](https://he.aliexpress.com/item/1005008488567735.html?utm_source=chatgpt.com) |
| Rotary Encoder             |       $2.10 |        1 | [AliExpress](https://he.aliexpress.com/item/32328753358.html?utm_source=chatgpt.com)      |
| 1.69" Screen               |       $4.51 |        1 | [AliExpress](https://he.aliexpress.com/item/1005008965476007.html?utm_source=chatgpt.com) |
| Buttons                    |       $2.60 |       50 | [AliExpress](https://he.aliexpress.com/item/1005003727160427.html?utm_source=chatgpt.com) |
| Connectors                 |       $1.54 |        1 | [AliExpress](https://he.aliexpress.com/item/1005006322027216.html?utm_source=chatgpt.com) |
| PCB Manufacturing (JLCPCB) |       $4.60 |        1 | —                                                                                         |
| 3D Printing                |           — |        1 | —                                                                                         |
| **Total**                  |  **$19.22** |          |                                                                                           |
