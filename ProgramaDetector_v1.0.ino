/*Este es un programa para poder controlar Leds desde el propio terminal*/
//Library EEPROM
#include <EEPROM.h>
#include <SoftwareSerial.h> //It would be use to communicate with the Ionization Chamber via RS232

#define delaycom 50 //this is the delay that has to be after sending a command via RS232 port

SoftwareSerial RS232(2, 3);//Rx & Tx pins

///String to receive Response for the Ionization Chamber
String reply232;


boolean debug = false;
boolean mode = true;
//Declaración de variables
//
///Led Status
//
#define Led1 13
///System Booleans
//
bool NewSessionMakerValidator = false; //New Session Confirmator Boolean
bool ModeOperationSelectorValidator = false; //Operation Mode Confirmator Boolean
bool DeliveryRequestValidator =  false; //Delivery Dose Confirmator Boolean
bool MeasureOperationSelectorValidator = false; //Measurement Mode Confirmator Boolean
bool ConversionMethodSelectorValidator = false; //Conversion Method Confirmator Boolean
bool IsCollimator = false; //Collimator On/Off Confirmator Boolean
bool SampleMaterialValidator = false; //Sample Material Confirmator Boolean
bool EnergyValidator = false; //Energy Confirmator Boolean
bool FrecuencyValidator = false; //Frecuency Confirmator Boolean
bool NewSessionNeeded =  true; //New Session Creation Validator Boolean

///Session Settings (I do not know if it is in the PING)
//
#define direcEEPROM 0 //EEPROM implemented byte

int SC = 0; //This is the session code

///Adquisition Dates To PING
//
unsigned int IM; //Irradiation Mode
float DT = 0.00; //Delivered Dose or Time
int MD = 0; //Dose Determination Mode
float F = 0.00; //Eventual Conversion Factor
char AC = '0'; //Collimator
unsigned int MAT = 0; //Sample Material
unsigned int E = 0; //Energy Beam
float Frep = 0; //Pulse Repetition Frecuency

///Ping Variables
//
char DTString[10] = "0";
char FString[10] = "0";
char FrepString[10] = "0";
String Space = "/";

// Variables declared for SerialTranslator function
char act[5];// define ACTION to do with register
char reg[5]; //define wich REGISTER is in use to make the defined action
byte nParam;//  contain de numbers of params
char stParam[10];// contain the string pased to param max 10 chars
float flParam;// contain the param coverted to float
int inParam;// contain the param converted to int


char buffer[20] = "";

String response = "";
String resp = "";

///Dose/Time Operation needed variables
//
float K = 0;
float TExpoDuration = 0;
float DExpo = 0;

unsigned long cmdT = 0; // contain time stamp of last cmd proces

///Dosimeters in Dose Delivery
//
float Dtoget = 0.00;
float Dmoment = 0.00;

///Timers in Dose Delivery
//
unsigned long Ttoget;
unsigned long TIter;

///Run Control Timer
unsigned long int TotalTime;
unsigned long int MomentumTimer;

///Maximum Time to do a PING
const unsigned long CTmax = 600000000; //1 segundo
unsigned long CT = 0; //Connection Timeout

///Task Control Timers
#define Tping 100000 //periodo tarea1
unsigned long int ts1;
unsigned long int t01;

///Interrupt Tick
//
volatile int Rele = 3;

///Execution Flags
//
bool Beam_OnFlag = false;
bool Beam_OffFlag = false;
bool Beam_StpFlag = false;

///Execution Indicator
//
char EState[17] = "Awaiting...";
unsigned long ATime;
unsigned long ATimeEmitted = 0.000;

void setup() {
  // OUTPUT Pin definitions
  //
  pinMode(Led1, OUTPUT);

  // Here starts the Serial Communication
  //
  Serial.begin(9600);
  Serial.println("RS232 running");
  //Set communication velocity
  RS232.begin(9600);
  Serial.print("Waiting for data input...");
  // Timers Initialization
  //
  ts1 = 0;
  t01 = 0;
  ///Here start the timers
  //
  cli();

  TCCR1A = 0x00;
  //prescaler
  TCCR1B = 0x00;
  TCNT1 = 0;
  OCR1A = 3126; //(16*10^6)/(5*1024)-1
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();
  attachInterrupt(digitalPinToInterrupt(2), SafetyOpen, FALLING);
}
ISR(TIMER1_COMPA_vect) {
  SftEvent();
}
void SafetyOpen() {
  digitalWrite(Rele, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (comandoSerial() == 1) {
    ///Those commands that the Detector can reads
    if ((act[1] == 'W') || (act[1] == 'R') || (act[1] == 'P') || (act[1] == 'A')) {
      SerialTranslator();
    }
    else {
      badsyn();
    }
  }
  //Program start Timer
  TotalTime = micros();
  //MomentumTimer = micros();
  //  ts1 = MomentumTimer - t01;
  //  if (ts1 >= Tping) {
  //    PingEvent();
  //    Serial.write(Ping.length());
  //    TExecPing = micros();
  //    t01 = MomentumTimer;
  //  }
  if (BeamShouldStart() == 1) {
    BeamControlExecuter();
  }
  // Vaciar:  act, reg, stParam
  memset(act, '\0', sizeof(act));
  memset(reg, '\0', sizeof(reg));
  memset(stParam, '\0', sizeof(stParam));
  nParam = 0; inParam = 0; flParam = 0;
}

byte comandoSerial() {
  byte cmdok = 0;
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '(') {
      double t = millis();
      char c = 0;
      byte j = 0;
      boolean salir = true;
      while ( salir ) {
        c = (char)Serial.read();
        if ( c > 0 ) {
          buffer[j + 1] = c;
          j++;
          if ( debug ) {
            Serial.print(c); Serial.print("--"); Serial.println(c, DEC);
          }
          if ( c == ')' ) {
            j--;
            cmdok = 1;
            break;
          }
        }
      }
      if ( cmdok == 1 ) {
        buffer[0] = j + 1; buffer[j + 1] = '\0'; //finalizar buffer quitar el carracter ')' final
        char * pch; //puntero array de chars
        if ( debug ) {
          Serial.println(buffer);
        }
        pch = strtok(buffer, "/"); //buscar hasta primer / para encontrar la accion
        strcpy(act, pch); act[5] = '\0'; //copiar en act el valor del puntero
        int count = 1;
        while ( pch != NULL ) {
          if ( count == 2 ) {
            strcpy(reg, pch);  //segundo es el Reg
            reg[5] = '\0';
          }
          if ( count == 3 ) {
            strcpy(stParam, pch);  //tercero es el parametro
            inParam = atoi(pch);
            flParam = atof(pch);
          }
          pch = strtok(NULL, "/");
          count++;
        }
        nParam = count - 3;

        if ( debug ) {
          Serial.print("act="); Serial.println(act);
          Serial.print("reg="); Serial.println(reg);
          Serial.print("number= "); Serial.println(nParam);
          Serial.print("stParam="); Serial.println(stParam);
          Serial.print("inParam="); Serial.println(inParam);
          Serial.print("flParam="); Serial.println(flParam);
        }
      }
    }
    return cmdok;
  }
}
