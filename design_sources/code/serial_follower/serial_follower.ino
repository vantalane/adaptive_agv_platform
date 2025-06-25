/* WARNING, CODE IS INCOMPLETE, IT IS UP TO YOU TO COMPLETE IT */

/* ========================
      Pin Definitions 
 ======================== */

/* --- Encoder Inputs --- */
#define ENC_B4 22
#define ENC_A4 23
#define ENC_B3 24
#define ENC_A3 25
#define ENC_B2 26
#define ENC_A2 27
#define ENC_B1 28
#define ENC_A1 29

/* --- Motor Driver PWM Outputs --- */
#define MOT_DRV_PWM_B4 2
#define MOT_DRV_PWM_A4 3
#define MOT_DRV_PWM_B2 4
#define MOT_DRV_PWM_A2 5
#define MOT_DRV_PWM_A3 6
#define MOT_DRV_PWM_B3 7
#define MOT_DRV_PWM_A1 8
#define MOT_DRV_PWM_B1 9

/* --- Analog Inputs --- */
#define VBAT_ADC A3
#define MOT_CURR_V_1 A4
#define MOT_CURR_V_2 A5
#define MOT_CURR_V_3 A6
#define MOT_CURR_V_4 A7

/*
  === STUDENT TEMPLATE START ===

  This Arduino code handles serial communication with a master device.

  Serial Command Structure:
    [CMD][ARG1][ARG2]...

  Implemented Commands:
    0x01: Echo back 1 byte
    0x02: Set a GPIO pin HIGH/LOW
    0x03: Read a GPIO pin's state
    0x04: Read ADC value from analog pin
    0x05: Set PWM output on any PWM-capable pin

  Extend this sketch to:
    - Add encoder reading support
    - Add additional motor control logic
    - Implement safety or timing mechanisms

  NOTE: Ensure pin numbers are valid on your board. Use digital pins 0-53 and analog pins A0-A15.
        Use the PCBA schematic as reference. 

  === STUDENT TEMPLATE END ===
*/

void setup() {
  Serial.begin(115200);
}

/*  =====================================
                  Main Loop
    ===================================== */

void loop() {
  if (Serial.available()) {
    uint8_t command = Serial.read();

    /* */
    switch (command) {
      case 0x01: handleEcho(); break;
      case 0x02: handleSetGPIO(); break;
      case 0x03: handleReadGPIO(); break;
      case 0x04: handleReadADC(); break;
      case 0x05: handleSetPWM(); break;
      default: handleUnknownCommand(); break;
    }
  }
}



// =====================================
// Command Handlers
// =====================================
void handleEcho() {
  waitForBytes(1);
  uint8_t value = Serial.read();
  Serial.write(0x81);
  Serial.write(value);
}

void handleSetGPIO() {
  waitForBytes(2);
  uint8_t pin = Serial.read();
  uint8_t val = Serial.read();
  pinMode(pin, OUTPUT);
  digitalWrite(pin, val ? HIGH : LOW);
  Serial.write(0x82);
  Serial.write(pin);
  Serial.write(val);
}

void handleReadGPIO() {
  waitForBytes(1);
  uint8_t pin = Serial.read();
  pinMode(pin, INPUT);
  uint8_t val = digitalRead(pin);
  Serial.write(0x83);
  Serial.write(pin);
  Serial.write(val);
}

void handleReadADC() {
  waitForBytes(1);
  uint8_t analogPin = Serial.read();
  int adcVal = analogRead(analogPin);
  Serial.write(0x84);
  Serial.write(analogPin);
  Serial.write(highByte(adcVal));
  Serial.write(lowByte(adcVal));
}

void handleSetPWM() {
  waitForBytes(2);
  uint8_t pin = Serial.read();
  uint8_t pwmVal = Serial.read();
  pinMode(pin, OUTPUT);
  analogWrite(pin, pwmVal);
  Serial.write(0x85);
  Serial.write(pin);
  Serial.write(pwmVal);
}

void handleUnknownCommand() {
  Serial.write(0xFF);  // Error or unsupported command
}

// =====================================
// Utility
// =====================================
void waitForBytes(uint8_t n) {
  while (Serial.available() < n) {
    // Busy-wait (could be replaced with timeout for robustness)
  }
}
