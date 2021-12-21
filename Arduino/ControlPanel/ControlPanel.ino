/*
   Control Panel for Star Citizen - Joystick version

   CopyRight 2021 - Black Otters
   Authors : Racailloux - Nemehy

   External libraries references :
   Joystick : https://github.com/MHeironimus/ArduinoJoystickLibrary
   PCF8574  : https://github.com/xreef/PCF8574_library

   TODO :
   * Managing outputs for "fatal leds"
*/
#include <Joystick.h>
#include <PCF8574.h>
#include <Filter.h>

// Handle serial messages only if debug is active
#define DEBUG   0


//////////////////////////////////////////////////////////////////////////////////////////////
// Constant declarations
// Backlight
const int PIN_PWM_BACKLIGHT = 10;
const int PIN_ANA_BACKLIGHT = A2;
// Mining laser power
const int PIN_MINING_POWER  = A3;
// Rotary encoder
const int PIN_ENC_CLK      = 0;
const int PIN_ENC_DT       = 1;
const int ENC_STEP_NR      = 20; // Number of increments of the encoder
const int ENC_STRIKE_TIMER = 50; // Timer before button rise down for encoder movement in ms
// Arduino inputs
const int LED_SELF_DESTRUCT  = 4;
const int PIN_SELF_DESTRUCT  = 5;
const int LED_EJECT          = 6;
const int PIN_EJECT          = 7;
const int LED_JETTISON_CARGO = 8;
const int PIN_JETTISON_CARGO = 9;
const int PIN_MOTOR          = 16;
const int PIN_SYSTEM_READY   = 14;
const int PIN_LIGHTS         = 15;
const int PIN_LOCK_DOORS     = 18;
const int PIN_OPEN_DOORS     = 19;


//////////////////////////////////////////////////////////////////////////////////////////////
// Input statement memorization variables
// Backlight
byte backlight_level = 0;
// Rotary encoder
int  state_enc_clk = LOW;
long enc_position = -1;  // will change to "0" at startup
// Arduino inputs
byte state_self_destruct  = false;
byte state_eject          = false;
byte state_jettison_cargo = false;
byte state_motor          = false;
byte state_system_ready   = false;
byte state_lights         = false;
byte state_lock_doors     = false;
byte state_open_doors     = false;
// PCF8574 @addr=20
int state_target_hat1 = JOYSTICK_HATSWITCH_RELEASE;
int state_target_hat2 = JOYSTICK_HATSWITCH_RELEASE;
// PCF8574 @addr=21
byte state_power      = false;
byte state_gimbal     = false;
byte state_decoupled  = false;
byte state_esp        = false;
byte state_gforce     = false;
byte state_match_tvel = false;
byte state_decoy      = false;
byte state_noise      = false;
// PCF8574 @addr=22
byte state_power_shield = false;
byte state_power_weapon = false;
byte state_power_thrust = false;
byte state_power_reset  = false;
byte state_qt_drive     = false;
byte state_vtol         = false;
byte state_qt_travel    = false;
byte state_landing      = false;
// PCF8574 @addr=23
byte state_shield_top    = false;
byte state_shield_front  = false;
byte state_shield_right  = false;
byte state_shield_reset  = false;
byte state_shield_left   = false;
byte state_shield_back   = false;
byte state_shield_bottom = false;
// PCF8574 @addr=24
byte state_scan         = false;
byte state_mining       = false;
byte state_laser_mode   = false;
byte state_consumable_1 = false;
byte state_consumable_2 = false;
byte state_consumable_3 = false;


//////////////////////////////////////////////////////////////////////////////////////////////
// Timers declaration
unsigned long enc_timer;
DigitalFilter<200> filterSelfDestruct;
DigitalFilter<200> filterEject;
DigitalFilter<200> filterJettisonCargo;

  
//////////////////////////////////////////////////////////////////////////////////////////////
// Libraries objects declaration

// Joystick declaration
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, // hidReportId
  JOYSTICK_TYPE_JOYSTICK,     // joystickType. Supported values: JOYSTICK_TYPE_JOYSTICK or 0x04 - Joystick / JOYSTICK_TYPE_GAMEPAD or 0x05 - Gamepad / JOYSTICK_TYPE_MULTI_AXIS or 0x08 - Multi-axis Controller
  39,                         // buttonCount
  2,                          // hatSwitchCount
  false,                      // includeXAxis
  false,                      // includeYAxis
  true,                       // includeZAxis => Mining laser power
  false,                      // includeRxAxis
  false,                      // includeRyAxis
  false,                      // includeRzAxis
  false,                      // includeRudder
  false,                      // includeThrottle
  false,                      // includeAccelerator
  false,                      // includeBrake
  false);                     // includeSteering

// I²C declarations
PCF8574 PCF20(0x20);
PCF8574 PCF21(0x21);
PCF8574 PCF22(0x22);
PCF8574 PCF23(0x23);
PCF8574 PCF24(0x24);


//////////////////////////////////////////////////////////////////////////////////////////////
// Initialization of the board
void setup()
{
  // power-up safety delay
  delay(1000);
  DebugInit();
  delay(1000);

  // Backlight management
  analogWrite(PIN_PWM_BACKLIGHT, backlight_level);
  
  // Initialize Joystick mode
  Joystick.begin();
  
  // Prepare analog input for Mining laser power
  Joystick.setZAxisRange(0,1024);

  // Prepare rotary encoder
  pinMode(PIN_ENC_CLK, INPUT_PULLUP);
  pinMode(PIN_ENC_DT, INPUT_PULLUP);
  
  // Prepare I/O from Arduino
  pinMode(PIN_SELF_DESTRUCT,  INPUT);
  pinMode(PIN_EJECT,          INPUT);
  pinMode(PIN_JETTISON_CARGO, INPUT);
  
  pinMode(PIN_MOTOR,          INPUT_PULLUP);
  pinMode(PIN_SYSTEM_READY,   INPUT_PULLUP);
  pinMode(PIN_LIGHTS,         INPUT_PULLUP);
  pinMode(PIN_LOCK_DOORS,     INPUT_PULLUP);
  pinMode(PIN_OPEN_DOORS,     INPUT_PULLUP);
  // Prepare PCF8574 @addr=20
  Joystick.setHatSwitch(0, state_target_hat1);
  Joystick.setHatSwitch(1, state_target_hat2);
  // Prepare PCF8574
  PreparePcfInputs(&PCF20);
  PreparePcfInputs(&PCF21);
  PreparePcfInputs(&PCF22);
  PreparePcfInputs(&PCF23);
  PreparePcfInputs(&PCF24);
  // Led management
  pinMode(LED_SELF_DESTRUCT,  INPUT_PULLUP);
  pinMode(LED_EJECT,          INPUT_PULLUP);
  pinMode(LED_JETTISON_CARGO, INPUT_PULLUP);
} // End of "setup"


//////////////////////////////////////////////////////////////////////////////////////////////
/// Main program cyclically called
void loop()
{  
  // Update backlight PWM level from Analog input
  int nAnaBackLight = analogRead(PIN_ANA_BACKLIGHT);
  int newBackLightLevel = (nAnaBackLight/4);
  if((newBackLightLevel < (backlight_level-1)) || (newBackLightLevel > (backlight_level+1)))
  {
    // Updating backlight level
    backlight_level = (nAnaBackLight/4);
    analogWrite(PIN_PWM_BACKLIGHT, backlight_level);
    DebugLn("New backlight level : " + String(backlight_level));
  }
  
  // Update analog axis for mining power
  Joystick.setZAxis(analogRead(PIN_MINING_POWER));
  
  // Rotary rotary encoder management
  EncoderManagement(PIN_ENC_CLK, PIN_ENC_DT, 0, 1);
  
  // Update buttons from Arduino
  UpdateBoardButton(PIN_SELF_DESTRUCT,  2, &state_self_destruct, &filterSelfDestruct);
  UpdateBoardButton(PIN_EJECT,          3, &state_eject, &filterEject);
  UpdateBoardButton(PIN_JETTISON_CARGO, 4, &state_jettison_cargo, &filterJettisonCargo);
  UpdateBoardButton(PIN_MOTOR,          6, &state_motor, NULL);
  UpdateBoardButton(PIN_SYSTEM_READY,   7, &state_system_ready, NULL);
  UpdateBoardButton(PIN_LIGHTS,         8, &state_lights, NULL);
  UpdateBoardButton(PIN_LOCK_DOORS,     9, &state_lock_doors, NULL);
  UpdateBoardButton(PIN_OPEN_DOORS,    10, &state_open_doors, NULL);

  // Update buttons from PCF8574 @addr=20
  PCF8574::DigitalInput di20 = PCF20.digitalReadAll();
  int nHat1 = JOYSTICK_HATSWITCH_RELEASE;
  if(!di20.p0) {nHat1 = 180;}
  else if(!di20.p1) {nHat1 = 360;}
  else if(!di20.p2) {nHat1 = 270;}
  else if(!di20.p3) {nHat1 = 90;}
  if (nHat1 != state_target_hat1)
  {
    Joystick.setHatSwitch(0, nHat1);
    state_target_hat1 = nHat1;
    DebugLn("Hat1=" + String(nHat1));
  }

  int nHat2 = JOYSTICK_HATSWITCH_RELEASE;
  if(!di20.p4) {nHat2 = 180;}
  else if(!di20.p5) {nHat2 = 360;}
  else if(!di20.p6) {nHat2 = 270;}
  else if(!di20.p7) {nHat2 = 90;}
  if (nHat2 != state_target_hat2)
  {
    Joystick.setHatSwitch(1, nHat2);
    state_target_hat2 = nHat2;
    DebugLn("Hat2=" + String(nHat2));
  }

  // Update buttons from PCF8574 @addr=21
  PCF8574::DigitalInput di21 = PCF21.digitalReadAll();
  UpdateButton(di21.p0,  5, &state_power);
  UpdateButton(di21.p1, 11, &state_gimbal);
  UpdateButton(di21.p2, 12, &state_decoupled);
  UpdateButton(di21.p3, 13, &state_esp);
  UpdateButton(di21.p4, 14, &state_gforce);
  UpdateButton(di21.p5, 15, &state_match_tvel);
  UpdateButton(di21.p6, 16, &state_decoy);
  UpdateButton(di21.p7, 17, &state_noise);
  
  // Update buttons from PCF8574 @addr=22
  PCF8574::DigitalInput di22 = PCF22.digitalReadAll();
  UpdateButton(di22.p0, 18, &state_power_shield);
  UpdateButton(di22.p1, 19, &state_power_weapon);
  UpdateButton(di22.p2, 20, &state_power_thrust);
  UpdateButton(di22.p3, 21, &state_power_reset);
  UpdateButton(di22.p4, 22, &state_qt_drive);
  UpdateButton(di22.p5, 23, &state_vtol);
  UpdateButton(di22.p6, 24, &state_qt_travel);
  UpdateButton(di22.p7, 25, &state_landing);
  
  // Update buttons from PCF8574 @addr=23
  PCF8574::DigitalInput di23 = PCF23.digitalReadAll();
  UpdateButton(di23.p0, 26, &state_shield_top);
  UpdateButton(di23.p1, 27, &state_shield_front);
  UpdateButton(di23.p2, 28, &state_shield_right);
  UpdateButton(di23.p3, 29, &state_shield_reset);
  UpdateButton(di23.p4, 30, &state_shield_left);
  UpdateButton(di23.p5, 31, &state_shield_back);
  UpdateButton(di23.p6, 32, &state_shield_bottom);
  
  // Update buttons from PCF8574 @addr=24
  PCF8574::DigitalInput di24 = PCF24.digitalReadAll();
  UpdateButton(di24.p0, 33, &state_scan);
  UpdateButton(di24.p1, 34, &state_mining);
  UpdateButton(di24.p2, 35, &state_laser_mode);
  UpdateButton(di24.p3, 36, &state_consumable_1);
  UpdateButton(di24.p4, 37, &state_consumable_2);
  UpdateButton(di24.p5, 38, &state_consumable_3);

} // End of "loop"


//////////////////////////////////////////////////////////////////////////////////////////////
// Local functions

// Generic button status update from I/O board
byte UpdateBoardButton(int SwitchPinNr, int JoyButtonNr, byte* PreviousState, DigitalFilter<200>* filter)
{
  int filtered;
  if(filter != NULL) {
    filtered = filter->update(digitalRead(SwitchPinNr));
  }
  else {filtered = digitalRead(SwitchPinNr);}
  
  UpdateButton(filtered, JoyButtonNr, PreviousState);
} // End of "UpdateButton"


// Generic button status update
byte UpdateButton(byte CurrentState, int JoyButtonNr, byte* PreviousState)
{
  if (CurrentState != *PreviousState)
  {
    Joystick.setButton(JoyButtonNr, !CurrentState);
    *PreviousState = CurrentState;
    DebugTxt("Button changed: " + String(JoyButtonNr));
    DebugLn(" to value " + String(CurrentState));
  }
} // End of "UpdateButton"


// Rotatory encoder management as 2 switches
void EncoderManagement(int EncClkPinNr, int EncDtPinNr, int JoyCcwButtonNr, int JoyCwButtonNr)
{
  int state_clk = digitalRead(EncClkPinNr);
   
  if ((state_enc_clk == LOW) && (state_clk == HIGH))   // Rotary encoder moved
  {     
    if (digitalRead(EncDtPinNr) == LOW)                // AntiClockWise move
    {      
      enc_position--;
      if ( enc_position < 0 ) {enc_position = ENC_STEP_NR;}
      Joystick.setButton(JoyCwButtonNr, false);
      Joystick.setButton(JoyCcwButtonNr, true);
      enc_timer = millis();
      DebugLn("Rotary encoder CounterClockWise, position " + String(enc_position));
    }
    else {                                             // ClockWise move
      enc_position++;
      if ( enc_position > ( ENC_STEP_NR - 1 ) ) {enc_position = 0;}
      Joystick.setButton(JoyCwButtonNr, true);
      Joystick.setButton(JoyCcwButtonNr, false);
      enc_timer = millis();

      DebugLn("Rotary encoder ClockWise, position " + String(enc_position));
    }
  }
  else {                                               // No move, wait for end of timer
    if((enc_timer + ENC_STRIKE_TIMER) < millis())
    {
      Joystick.setButton(JoyCwButtonNr, false);
      Joystick.setButton(JoyCcwButtonNr, false);
    }
  }
   
  state_enc_clk = state_clk;
} // End of "EncoderManagement"


//////////////////////////////////////////////////////////////////////////////////////////////
// Preparing all inputs of the PCF8574
void PreparePcfInputs(PCF8574* Pcf)
{
  DebugLn("Preparing PCF8574");
  Pcf->pinMode(P0, INPUT_PULLUP);
  Pcf->pinMode(P1, INPUT_PULLUP);
  Pcf->pinMode(P2, INPUT_PULLUP);
  Pcf->pinMode(P3, INPUT_PULLUP);
  Pcf->pinMode(P4, INPUT_PULLUP);
  Pcf->pinMode(P5, INPUT_PULLUP);
  Pcf->pinMode(P6, INPUT_PULLUP);
  Pcf->pinMode(P7, INPUT_PULLUP);
  Pcf->begin();
} // End of "PreparePcfInputs"


//////////////////////////////////////////////////////////////////////////////////////////////
// Debug texts to serial port (only if debug is active)
void DebugLn(String Msg)
{
  #ifdef DEBUG
  Serial.println(Msg);
  #endif // DEBUG
} // End of "DebugLn"


void DebugTxt(String Msg)
{
  #ifdef DEBUG
  Serial.print(Msg);
  #endif // DEBUG
} // End of "DebugTxt"


void DebugInit()
{
  #ifdef DEBUG
  Serial.begin(9600);
  delay(1000);
  // Display title
  Serial.println("");
  Serial.println("=== Control Panel - Black Otters ===");
  Serial.println("========== By Nem' & Rac' ==========");
  #endif // DEBUG
} // End of "DebugInit"
