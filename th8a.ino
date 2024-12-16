#include <Tlv493d.h>
#include <Keyboard.h>

Tlv493d Tlv493dMagnetic3DSensor = Tlv493d();

// Magnet position vairables
int azimuth = 0;
int polar = 0;

// Gear variables
int gearboxMode;
int gear = 0;
int previousGear = 0;
int lastButtonState = 0;

// Debaunce time variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 20;

void setup() {
  Serial.begin(9600);
  
  Tlv493dMagnetic3DSensor.begin();
  Tlv493dMagnetic3DSensor.setAccessMode(Tlv493dMagnetic3DSensor.MASTERCONTROLLEDMODE);
  Tlv493dMagnetic3DSensor.disableTemp();

  Keyboard.begin();
  
  pinMode(15, INPUT_PULLUP);
  gearboxMode = digitalRead(15);
}

void loop() {
  delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
  Tlv493dMagnetic3DSensor.updateData();

  azimuth = Tlv493dMagnetic3DSensor.getAzimuth()*100;
  polar = Tlv493dMagnetic3DSensor.getPolar()*100;
  gearboxMode = digitalRead(15);

  if (gearboxMode == HIGH) {
    if ((azimuth >= -305 &&  azimuth <= -285) && (polar >= 65 &&  polar <= 90)) {
      gear = 49;  
    }
    else if ((azimuth >= -32 &&  azimuth <= -15) && (polar >= 80 &&  polar <= 112)) {
      gear = 50;
    }
    else if ((azimuth >= 240 &&  azimuth <= 270) && (polar >= 55 &&  polar <= 80)) {
      gear = 51;
    }
    else if ((azimuth >= 50 &&  azimuth <= 80) && (polar >= 70 &&  polar <= 90)) {
      gear = 52;
    }
    else if ((azimuth >= 195 &&  azimuth <= 225) && (polar >= 15 &&  polar <= 35)) {
      gear = 53;
    }
    else if ((azimuth >= 90 &&  azimuth <= 115) && (polar >= 25 &&  polar <= 40)) {
      gear = 54;
    }
    else if ((azimuth >= 165 &&  azimuth <= 205) && (polar >= -5 &&  polar <= 10)) {
      gear = 55;
    }
    else if ((azimuth >= 95 &&  azimuth <= 125) && (polar >= -5 &&  polar <= 15)) {
      gear = 56;
    } else {
      gear = 0;
    }
  } else {
    if (polar >= 120) {
      gear = 43;
    }
    else if (polar <= 90) {
      gear = 45;
    } else {
      gear = 0;
    }
  }

  if (gear != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (gear != previousGear) {
      Keyboard.releaseAll();
      previousGear = gear;
      if (gear != 0){
        Keyboard.press(gear);
      }
    }
  }

  lastButtonState = gear;

  // Calibration
  // Serial.print("Azimuth: ");
  // Serial.print(azimuth);
  // Serial.print(" ; Polar:  ");
  // Serial.println(polar);
}