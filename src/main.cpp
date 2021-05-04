#include <Arduino.h>

#define SM_IN1 8
#define SM_IN2 9
#define SM_IN3 10
#define SM_IN4 11

void setup() {
    pinMode(SM_IN1, OUTPUT);
    pinMode(SM_IN2, OUTPUT);
    pinMode(SM_IN3, OUTPUT);
    pinMode(SM_IN4, OUTPUT);

    Serial.begin(9600);
}

int motor_step_delay = 255;
uint8_t pv[4] = {HIGH, LOW, LOW, HIGH};

void doMotorStep() {
    if (motor_step_delay >= 240) return;
    digitalWrite(SM_IN1, pv[0]);
    digitalWrite(SM_IN2, pv[1]);
    digitalWrite(SM_IN3, pv[2]);
    digitalWrite(SM_IN4, pv[3]);
    delay(motor_step_delay);
}

void shiftR() {
    int temp = pv[3];
    for (int i = 3; i > 0; i--) {
        pv[i] = pv[i - 1];
    }
    pv[0] = temp;
}

void shiftL() {
    int temp = pv[0];
    for (int i = 0; i < 3; i++) {
        pv[i] = pv[i + 1];
    }
    pv[3] = temp;
}

void calculate(int joy_axis_value) {
    int mapped_joy_x = map(joy_axis_value, 0, 1023, -512, 512);

    motor_step_delay = 252 - map(abs(mapped_joy_x), 0, 512, 0, 250);

    if (mapped_joy_x < 0) {
        shiftL();
    } else {
        shiftR();
    }
}

void loop() {
    int joy_x = analogRead(A0);

    calculate(joy_x);
    doMotorStep();
}