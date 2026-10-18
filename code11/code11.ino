using System;
using System.Device.Gpio;
using System.Device.Spi;
using System.Threading;
using nanoFramework.Hardware.Esp32;

namespace Esp32_PCB_Setup
{
    public class Program
    {
        private static SpiDevice _screenSpi;
        private static GpioPin _screenDc;
        private static GpioPin _screenRst;
        private static GpioPin _btn4;
        private static GpioPin _btn5;

        private static GpioPin _encA;
        private static GpioPin _encB;
        private static GpioPin _encSw;
        private static int _encoderPosition = 0;
        private static PinValue _lastEncAState;
        
        // Track action index globally so it updates correctly
        private static int _currentAct = 0; 
        private static string[] _actions = new string[20];

        public static void Main()
        {
            // Initialize actions array properly
            for (int i = 0; i < _actions.Length; i++)
            {
                _actions[i] = null;
            }
            _actions[0] = "Action 1 Active";
            _actions[1] = "Action 2 Active";

            SetupHardware();
            UpdateDisplay("System Ready");

            // Main loop stays alive 
            while (true)
            {
                // Reading state on demand
                if (_encB.Read() == PinValue.Low)
                {
                    _currentAct = (_currentAct + 1) % _actions.Length;
                    if (_actions[_currentAct] != null)
                    {
                        UpdateDisplay(_actions[_currentAct]);
                    }
                    Thread.Sleep(200); // Simple debounce for loop sampling
                }
                
                Thread.Sleep(100);
            }
        }

        private static void SetupHardware()
        {
            var gpio = new GpioController();

            // 1. Setup Buttons & Event Handlers
            _btn4 = gpio.OpenPin(14, PinMode.InputPullUp);
            _btn4.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling)
                {
                    // Correct switch execution
                    switch (_currentAct)
                    {
                        case 0:
                            ExecuteAction1();
                            break;
                        case 1:
                            ExecuteAction2();
                            break;
                    }
                }
            };

            _btn5 = gpio.OpenPin(15, PinMode.InputPullUp);
            _btn5.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling) UpdateDisplay("SW5 Pressed!");
            };

            // 2. Setup Rotary Encoder Inputs
            _encA = gpio.OpenPin(12, PinMode.InputPullUp);
            _encB = gpio.OpenPin(13, PinMode.InputPullUp);
            _encSw = gpio.OpenPin(11, PinMode.InputPullUp);
            
            _lastEncAState = _encA.Read();
            _encA.ValueChanged += (s, e) => {
                PinValue currentA = _encA.Read();
                if (currentA != _lastEncAState && currentA == PinValue.Low) 
                {
                    if (_encB.Read() == PinValue.Low) _encoderPosition++; else _encoderPosition--;
                    UpdateDisplay($"Value: {_encoderPosition}");
                }
                _lastEncAState = currentA;
            };

            _encSw.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling) UpdateDisplay("Enc Clicked!");
            };

            // 3. ESP32 Specific SPI Pin Configuration
            // Replace 18, 19, 23 with your actual PCB's Clock, MISO, and MOSI pins
            Configuration.SetPinFunction(18, DeviceFunction.SPI1_CLOCK);
            Configuration.SetPinFunction(19, DeviceFunction.SPI1_MISO);
            Configuration.SetPinFunction(23, DeviceFunction.SPI1_MOSI);

            _screenDc = gpio.OpenPin(8, PinMode.Output);
            _screenRst = gpio.OpenPin(7, PinMode.Output);

            // Bus 1, CS Pin 9
            var spiSettings = new SpiConnectionSettings(1, 9) 
            {
                ClockFrequency = 24_000_000,
                Mode = SpiMode.Mode0
            };
            _screenSpi = SpiDevice.Create(spiSettings);

            // Pulse the reset pin
            _screenRst.Write(PinValue.Low);
            Thread.Sleep(50);
            _screenRst.Write(PinValue.High);
            Thread.Sleep(50);
        }

        private static void UpdateDisplay(string message) 
        {
            Console.WriteLine("[Display Out]: working!!!");
            _screenDc.Write(PinValue.High);
            byte[] rawBytes = System.Text.Encoding.UTF8.GetBytes(message);
            _screenSpi.Write(rawBytes);
        }

        // Dummy implementations for missing actions
        private static void ExecuteAction1() => UpdateDisplay("Executed Act1");
        private static void ExecuteAction2() => UpdateDisplay("Executed Act2");
    }
}
