//  Copyright(C) 2018 Nerlci_, all right reversed.
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);
const int TPadL = 6, TPadR = 7, RstButton = 8, PowerPin = 9, RELEASE = 0, PRESS = 1, HOLD = 2, WAITSTART = 1, STARTTOUCH = 2, STOPTOUCH = 3;
long MsRec = 0, touchTime = 0, resetTime = 0, last = 0;
long Hs, Mins, Ss, Ms;
String MsNom, MsSpec, SsNom, SsSpec, MinsNom, MinsSpec, AnsTime;
bool timing = false,LEDLeave = false;
byte touchState = RELEASE, resetState = RELEASE;

inline void refresh() {
    lcd.setCursor(0, 1);
    Hs = (millis() - MsRec) / 3600000;
    Mins = (millis() - MsRec) % 3600000 / 60000;
    Ss = (millis() - MsRec) % 60000 / 1000;
    Ms = (millis() - MsRec) % 1000;
    MsSpec = (String)Ms;
    MsNom = (Ms >= 100 ? MsSpec : (Ms >= 10 ? "0" + MsSpec : "00" + MsSpec));
    SsSpec = (String)Ss + "." + MsNom;
    SsNom = (Ss >= 10 ? SsSpec : "0" + SsSpec);
    MinsSpec = (String)Mins + ":" + SsNom;
    MinsNom = (Mins >= 10 ? MinsSpec : "0" + MinsSpec);
    MinsSpec = (Mins > 0 ? (String)Mins + ":" + SsNom : SsSpec);
    AnsTime = (Hs > 0 ? (String)Hs + ":" + MinsNom : MinsSpec);
    lcd.setCursor(16 - AnsTime.length(), 1);
    lcd.print(AnsTime);
    if (digitalRead(RstButton)) {
        Serial.println(AnsTime);
        digitalWrite(13, HIGH);
    }
}

void start() {
    MsRec = millis();
    timing = true;
    refresh();
}

void stop() {
    if (timing) refresh();
    MsRec = 0;
    timing = false;
    digitalWrite(13, LOW);
    Serial.print(AnsTime);
}

void reset() {
    stop();
    digitalWrite(13, LOW);
    lcd.setCursor(0, 1);
    lcd.print("           0.000");
}

void shutdown() {
    digitalWrite(13, LOW);
    digitalWrite(PowerPin, LOW);
}

int getd(int pin) {
    return digitalRead(pin);
}

int geta(int pin) {
    return analogRead(pin);
}

void checkTouch() {
    switch (touchState) {
    case RELEASE:
        if (getd(TPadL) && getd(TPadR) && timing) {
            touchState = STOPTOUCH;
        };
        if (getd(TPadL) && getd(TPadR) && !timing) {
            touchTime = millis();
            touchState = WAITSTART;
        };
        break;
    case WAITSTART:
        if (getd(TPadL) && getd(TPadR) && (millis() - touchTime) >= 500) {
            reset();
            digitalWrite(13, HIGH);
            touchState = STARTTOUCH;
        };
        if (!(getd(TPadL) && getd(TPadR)) && (millis() - touchTime) < 500) {
            touchState = RELEASE;
        };
        break;
    case STARTTOUCH:
        if (!(getd(TPadL) && getd(TPadR))) {
            touchState = RELEASE;
            digitalWrite(13, LOW);
            start();
        };
        break;
    case STOPTOUCH:
        if (!(getd(TPadL) && getd(TPadR))) {
            touchState = RELEASE;
            stop();
        };
        break;
    }
}

void checkReset() {
    switch (resetState) {
    case RELEASE:
        if (getd(RstButton)) {
            resetTime = millis();
            digitalWrite(13, HIGH);
            resetState = PRESS;
        };
        break;
    case PRESS:
        if (getd(RstButton) && (millis() - resetTime) >= 3000 && !timing) {
            digitalWrite(13, LOW);
            resetState = HOLD;
        };
        if (!getd(RstButton) && ((millis() - resetTime) < 3000 || timing)) {
            reset();
            digitalWrite(13, LOW);
            resetState = RELEASE;
        };
        break;
    case HOLD:
        if (!getd(RstButton)) {
            shutdown();
            resetState = RELEASE;
        };
        break;
    }
}

void setup() {
    pinMode(PowerPin, OUTPUT);
    digitalWrite(PowerPin, HIGH);
    lcd.init();
    lcd.backlight();
    pinMode(TPadL, INPUT);
    pinMode(TPadR, INPUT);
    pinMode(RstButton, INPUT);
    pinMode(13, OUTPUT);
    //lcd.setCursor(0,0);
    lcd.print("  Neon Timer L");
    lcd.setCursor(0, 1);
    lcd.print("           0.000");
    Serial.begin(9600);
}

void loop() {
    if (timing) refresh();
    if (timing&&(millis()-last>=100)) {
        last=millis();
        digitalWrite(13, !digitalRead(13));
    }
    checkTouch();
    checkReset();
}
