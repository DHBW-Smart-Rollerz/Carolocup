﻿#include "aktorik.h"

Servo servo;
Servo motor;  

std_msgs::Bool rc_msg;
std_msgs::Int16 drive_mode_msg;

std_msgs::Int16 test_msg;

ros::Subscriber<std_msgs::Float32> sub_servo("ctl_servoAngle", servo_cb);
ros::Subscriber<std_msgs::Int16> sub_motor("ctl_motorRpm", motor_cb);      
ros::Subscriber<std_msgs::UInt8> sub_light_l("trj_flashLeft", lichtLinks_cb);
ros::Subscriber<std_msgs::UInt8> sub_light_r("trj_flashRight", lichtRechts_cb);
ros::Subscriber<std_msgs::UInt8> sub_light_b("trj_brakeLight", lichtBremse_cb);
ros::Subscriber<std_msgs::UInt8> sub_light_rem("trj_remoteLight", lichtRemote_cb);

ros::Publisher rc_pub("akt_rc", &rc_msg);
ros::Publisher drive_mode_pub("akt_autoDriveMode", &drive_mode_msg);

ros::Publisher test_pub("test_rc", &test_msg);

bool rc_mode = false;

int test_value = 0;

int16_t analogvalue_motor_rcmode; //eingelesener Pin - Wert am Tiefpass vom Motor
int16_t analogvalue_rcmode;   //eingelesener Pin an Channel 4, um zu schauen ob im RC-Mode
int16_t drive_mode;
int motor_zaehler = 0;
float wert_servo;
float lenkwinkel_servosize;
float lenkwinkel_grad;
int16_t motor_uebertragung_RC_mode;
int16_t motor_uebertragung;

short state_light_r = 0;
short state_light_l = 0;
short state_light_b = 0;
short state_light_rem = 0;

uint16_t previousMillis = 0;
boolean blinkstate = true;

int arr[] = {0, 0, 0, 0, 0, 0};

void init_aktorik(ros::NodeHandle *aktorik_node)
{  
  aktorik_node->subscribe(sub_servo);  //Zuweisung Servo Subscriber zum Aktorik - Knoten
  aktorik_node->subscribe(sub_motor);  //Zuweisung Motor Subscriber zum Aktorik - Knoten
  aktorik_node->subscribe(sub_light_l);
  aktorik_node->subscribe(sub_light_r);
  aktorik_node->subscribe(sub_light_b);
  aktorik_node->subscribe(sub_light_rem);

  aktorik_node->advertise(rc_pub);
  aktorik_node->advertise(drive_mode_pub);
  aktorik_node->advertise(test_pub);
  
  motor.attach(6); //Motor an Pin zuweisen
  servo.attach(5); //Servo an Pin zuweisen

  //Setzen der Lichter als Ausgang
  pinMode(frontlicht, OUTPUT);              
  pinMode(blinker_links, OUTPUT);         
  pinMode(blinker_rechts, OUTPUT);        
  pinMode(bremslicht, OUTPUT);              
  pinMode(rueckfahrlicht, OUTPUT);          
  pinMode(blaues_licht, OUTPUT);
  
  pinMode(tiefpass_pwm_motor_voltage, INPUT);
  pinMode(tiefpass_rcmode_voltage, INPUT);

  pinMode(drive_mode_off, INPUT);
  pinMode(drive_mode_freeDrive, INPUT);
  pinMode(drive_mode_obstacleAvoidance, INPUT);

  //Alle Lichter ausschalten
  digitalWrite(blinker_links, LOW);            
  digitalWrite(blinker_rechts, LOW); 
  digitalWrite(bremslicht,LOW); 
  digitalWrite(rueckfahrlicht, LOW);
  digitalWrite(blaues_licht, LOW);
  digitalWrite(frontlicht,HIGH);
}

bool aktorik()
{  
  int voltage_rcmode;
  analogvalue_rcmode = analogRead(tiefpass_rcmode_voltage_nr);    //Einlesen des Pins vom Tiefpass vom channel 4
  
  if (analogvalue_rcmode > rcmode_schwellenwert)
  {                                    //RC-Mode
    delay(5);
    if(analogvalue_rcmode > rcmode_schwellenwert){
      state_light_rem = 2;
   
      rc_mode = true;
      motor_bewegung_RC_mode();
    }
  }
  
  else 
  {                                     //autonomer Betrieb
    state_light_rem = 0;                //blaue LED ausschalten
    
    rc_mode = false;
  }

  uint16_t currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    set_led_states();
    blinkstate = !blinkstate;
  }


  if(digitalRead(drive_mode_off)==HIGH)
  {
    drive_mode = 0;
  }
  
  if(digitalRead(drive_mode_freeDrive)==HIGH)
  {
    drive_mode = 1;
  }

  if(digitalRead(drive_mode_obstacleAvoidance)==HIGH)
  {
    drive_mode = 2;
  }
  
  return rc_mode;
}

void servo_cb(const std_msgs::Float32& cmd_msg)//Callback - Funktion für Servoaufruf
{
  if (rc_mode == false)
  {   
    servo_bewegung(cmd_msg.data);
  }
}

void motor_cb(const std_msgs::Int16& cmd_msg)//Callback - Funktion für Motoraufruf
{ 
  if (rc_mode == false)
  {    
    motor_bewegung(cmd_msg.data);
  }
}

void lichtLinks_cb(const std_msgs::UInt8& light_state)
{
  state_light_l = light_state.data;
}

void lichtRechts_cb(const std_msgs::UInt8& light_state)
{
  state_light_r = light_state.data;
}

void lichtBremse_cb(const std_msgs::UInt8& light_state)
{
  state_light_b = light_state.data;
}

void lichtRemote_cb(const std_msgs::UInt8& light_state)
{
  state_light_rem = light_state.data;
}

void servo_bewegung(float lenkwinkel_bogenmass)
{
  lenkwinkel_grad = 20*(lenkwinkel_bogenmass/pi)*180;
  if(lenkwinkel_grad<=0)
  {
    lenkwinkel_servosize = lenkwinkel_grad + 91.5;
  }
  else
  {
    lenkwinkel_servosize = lenkwinkel_grad + 91.5;
  } 
  servo.write(lenkwinkel_servosize); //Servo fährt in die ensprechende Stellung
}

void motor_bewegung(int16_t motor_drehzahl)
{
  test_value = 1;
  test_publish();
  
  //motor.attach(6);
  if(motor_drehzahl < 0)//rückwärts
  {     
    test_value = 2;
    test_publish();
    motor_uebertragung = (int) 90 +(0.234 * motor_drehzahl);
  }
  else//vorwärts
  {  
    test_value = 3;
    test_publish();
    motor_uebertragung = (int) (0.236 * motor_drehzahl) + 93; //96 raus!!!! 
  }
  /* 
   * Übergangsbereich von 91 bis 95 wird nicht betrachtet
   */
 motor.write(motor_uebertragung);
}

void motor_bewegung_RC_mode()
{
  test_value = 4;
  test_publish();
  
    analogvalue_motor_rcmode = analogRead(tiefpass_pwm_motor_voltage_nr);
    arr[motor_zaehler]=analogvalue_motor_rcmode;
    
    float dummy = (arr[0]+arr[1]+arr[2]+arr[3]+arr[4]+arr[5])/6.0;
    
    if(dummy < 34)
    {
      motor_uebertragung_RC_mode = 87;
    }
    else if(dummy > 45)
    {
      motor_uebertragung_RC_mode = 100;
    }
    else 
    {
       motor_uebertragung_RC_mode = 93;
    }
    
    motor.write(motor_uebertragung_RC_mode);
    
    motor_zaehler++;
    
    if(motor_zaehler > 5)
    {
        motor_zaehler = 0;
    }
  /*
    if (analogvalue_motor_rcmode < tiefpass_untere_spannung) //rückwarts
    {       
        //motor_uebertragung_RC_mode = (analogvalue_motor_rcmode * 3.3333) - 9.99;
        motor_uebertragung_RC_mode = 87;
    }
    else if (analogvalue_motor_rcmode > tiefpass_obere_spannung)//vorwärts
    {   
        //motor_uebertragung_RC_mode = (analogvalue_motor_rcmode * 2.222) + 36.06;
        motor_uebertragung_RC_mode = 100;
    }
    else
    {
      //motor.detach(6);
      motor_uebertragung_RC_mode = 93;
    }*/
   // motor.write(motor_uebertragung_RC_mode); 
}

void set_led_states()
{
  set_output(state_light_l, blinker_links);
  set_output(state_light_r, blinker_rechts);
  set_output(state_light_b, bremslicht);
  set_output(state_light_rem, blaues_licht);
}

void set_output(short state, short port)
{
  switch(state)
  {
    case 0: digitalWrite(port, LOW);
      break;
    case 1: digitalWrite(port, HIGH);
      break;
    case 2: 
      digitalWrite(port, blinkstate);
  }
}

void rc_publish ()
{ 
  rc_msg.data = rc_mode;
  rc_pub.publish(&rc_msg);
}

void drive_mode_publish ()
{ 
  drive_mode_msg.data = drive_mode;
  drive_mode_pub.publish(&drive_mode_msg);
}

void test_publish ()
{ 
  test_msg.data = test_value;
  test_pub.publish(&test_msg);
}