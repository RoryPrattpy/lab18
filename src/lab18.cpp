/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/CTD_2023/lab18/src/lab18.ino"
void setup();
void loop();
#line 1 "/Users/admin/CTD_2023/lab18/src/lab18.ino"
SYSTEM_THREAD(ENABLED);

#define Green 0
#define Yellow 1
#define Red 2
#define YellowStateRed 3

#include "MQTT.h"

void callback(char *topic, byte *payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);

int currentState = 0;
char * output;
int input = 0;
int counter = 5;

struct State {
  char* output;
  unsigned long waitTime;
  int nextState[4];  // index of state
};

typedef struct State State_t;

State_t fsm[4] = {
  {"[0, 2]", 0, {Green, Yellow}},  // Green
  {"[1, 2]", 5000, {Red, Red}},  // Yellow
  {"[2, 0]", 0, {YellowStateRed, Red}}, // Red
  {"[2, 1]", 5000, {Green, Green}} // YellowStateRed
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (!client.isConnected()) {
    client.connect(System.deviceID());
    client.subscribe("HipposAreTasty/CarDirection");
    return;
  }
  client.loop();

  if (counter < 4) {
    delay(fsm[currentState].waitTime);
    currentState = fsm[currentState].nextState[input];
    client.publish("HipposAreTasty/Output", fsm[currentState].output);
    counter++;
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  input = atoi(p);
  if (counter >= 4) {
    counter = 1;
  }
}