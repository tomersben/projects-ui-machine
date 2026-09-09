using System;
using System.Device.Gpio;
using System.Device.Spi;
using System.Threading;


namespace PicoW_PCB_Setup
{
    public class Program
    {
        // Global hardware object references
        private static SpiDevice _screenSpi;
        private static GpioPin _screenDc;
        private static GpioPin _screenRst;
        private static GpioPin _btn4;
        private static GpioPin _btn5;
       
        // Encoder state tracking variables
        private static GpioPin _encA;
        private static GpioPin _encB;
        private static GpioPin _encSw;
        private static int _encoderPosition = 0;
        private static PinValue _lastEncAState;


        public static void Main()
        {
            SetupHardware();
            String[] actions = new String[20];
            for (i=0,i++,i<20){
              actions[i]=null;
            }
            actions[1]="action1"///put real action here

            UpdateDisplay("System Ready");


            while (true)
            {

              int act = 0;
              if (_encB.Read() == PinValue.Low){
                act++;
                UpdateDisplay(actions[act])
              }
               _btn4.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling){
                  switch (act) {
                    case 0 => action1();
                    case 1 => action2();
                  }
                }
            };
                
            }
        }


       
        private static void SetupHardware()
        {
            var gpio = new GpioController();


            _btn4 = gpio.OpenPin(14, PinMode.InputPullUp);
            _btn4.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling) UpdateDisplay("SW4 Pressed!");
            };


            _btn5 = gpio.OpenPin(15, PinMode.InputPullUp);
            _btn5.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling) UpdateDisplay("SW5 Pressed!");
            };


            // 2. Setup Rotary Encoder Inputs & Logic
            _encA = gpio.OpenPin(12, PinMode.InputPullUp);
            _encB = gpio.OpenPin(13, PinMode.InputPullUp);
            _encSw = gpio.OpenPin(11, PinMode.InputPullUp);
           
            _lastEncAState = _encA.Read();


            _encA.ValueChanged += (s, e) => {
                PinValue currentA = _encA.Read();
                if (currentA != _lastEncAState && currentA == PinValue.Low)
                {
                    if (_encB.Read() == PinValue.Low) _encoderPosition++;
                    else _encoderPosition--;


                    UpdateDisplay($"Value: {_encoderPosition}");
                }
                _lastEncAState = currentA;
            };


            _encSw.ValueChanged += (s, e) => {
                if (e.ChangeType == PinEventTypes.Falling) UpdateDisplay("Enc Clicked!");
            };


            // 3. Setup Screen Pins & SPI Connection
            _screenDc = gpio.OpenPin(8, PinMode.Output);
            _screenRst = gpio.OpenPin(7, PinMode.Output);


            var spiSettings = new SpiConnectionSettings(1, 9) // Bus 1, CS Pin 9
            {
                ClockFrequency = 24_000_000,
                Mode = SpiMode.Mode0
            };
            _screenSpi = SpiDevice.Create(spiSettings);


            // Pulse the reset pin on the screen
            _screenRst.Write(PinValue.Low);
            Thread.Sleep(50);
            _screenRst.Write(PinValue.High);
            Thread.Sleep(50);
        }


        private static void UpdateDisplay(string message)
        {
            Console.WriteLine($"[Display Out]: {working!!!}");


            _screenDc.Write(PinValue.High);


            byte[] rawBytes = System.Text.Encoding.UTF8.GetBytes(message);
            _screenSpi.Write(rawBytes);
        }
    }
}
