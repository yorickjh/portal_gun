#include "RotaryEncoder.h"
#include "l74digit.h"
#include <Adafruit_NeoPixel.h>

uint16_t lights_pin=1;
Adafruit_NeoPixel pixels=Adafruit_NeoPixel(4,lights_pin,NEO_GRB+NEO_KHZ800);
uint32_t crystal_color=Adafruit_NeoPixel::Color(100,255,0);
uint32_t deflector_color=Adafruit_NeoPixel::Color(100,255,0);

uint16_t button_pin=13;
bool button_down=false;
uint16_t button_off_time=5000;
uint16_t button_activate_time=1000;
uint16_t button_pressed_time=100;
uint16_t button_shoot_time=50;
uint32_t button_down_time=0;
uint32_t button_up_time=0;
uint32_t button_down_start_time=0;
uint32_t button_up_start_time=0;
uint16_t encoder_a_pin=A4;
uint16_t encoder_b_pin=A5;
int16_t encoder_pos=0;
RotaryEncoder encoder(encoder_a_pin,encoder_b_pin);
int16_t cursor_pos=3;

uint16_t speaker_pin=3;

uint16_t alive_pin=12;
uint16_t kill_timeout=30000;
uint32_t kill_timer=millis()+kill_timeout;

bool can_shoot=false;

enum mode_t
{
  NONE,
  EDIT,
  DIGIT,
  SHOOT
};

mode_t mode=NONE;

void power_on()
{
  pinMode(alive_pin,OUTPUT);
  digitalWrite(alive_pin,HIGH);
}

void power_off()
{
  pinMode(alive_pin,OUTPUT);
  digitalWrite(alive_pin,LOW);

  crystal_off();
  deflector_off();

  enter_none_mode();

  while(true)
  {}
}

void button_setup()
{
  pinMode(button_pin,INPUT);
}

void speaker_setup()
{
  pinMode(speaker_pin,OUTPUT);
}

void lights_setup()
{
  pixels.begin();
}

void lcd_setup()
{
  l74_initialize();
}

void button_update()
{
  bool value=digitalRead(button_pin);
  
  if(value&&!button_down)
    button_down_start_time=millis();

  if(!value&&button_down)
    button_up_start_time=millis();
  
  if(button_down)
    button_down_time=millis()-button_down_start_time;
  
  if(!button_down)
    button_up_time=millis()-button_up_start_time;

  button_down=value;
}

void button_reset()
{
  button_down_time=0;
  button_up_time=0;
  button_down_start_time=0;
  button_up_start_time=0;
  button_down=false;
}

void portal_delay(uint32_t ms)
{
  uint32_t timer=millis()+ms;

  while(millis()<timer)
  {
    l74_update();
    encoder.tick();
    button_update();
  }
}

void crystal_on()
{
  pixels.setPixelColor(0,crystal_color);
  pixels.show();
}

void crystal_off()
{
  pixels.setPixelColor(0,pixels.Color(0,0,0));
  pixels.show();
}

void deflector_on()
{ 
  for(uint16_t ii=1;ii<4;++ii)
    pixels.setPixelColor(ii,deflector_color);
  pixels.show();
}

void deflector_off()
{ 
  for(uint16_t ii=1;ii<4;++ii)
    pixels.setPixelColor(ii,pixels.Color(0,0,0));
  pixels.show();
}

void bios_show_value(uint16_t value,uint16_t duration)
{
  uint32_t timer;

  for(uint16_t ii=0;ii<4;++ii)
    l74_values[ii]=value;

  portal_delay(duration);
}

void bios_show_values(uint16_t values[4],uint16_t duration)
{
  uint32_t timer;

  for(uint16_t ii=0;ii<4;++ii)
    l74_values[ii]=values[ii];

  portal_delay(duration);
}

void bios_show_vertical_swipe_down(uint16_t duration,uint16_t times)
{
  for(uint16_t ii=0;ii<times;++ii)
  {
    bios_show_value(TOP,duration);
    bios_show_value(MIDDLE,duration);
    bios_show_value(BOTTOM,duration);
  }
}

void bios_show_vertical_swipe_up(uint16_t duration,uint16_t times)
{
  for(uint16_t ii=0;ii<times;++ii)
  {
    bios_show_value(BOTTOM,duration);
    bios_show_value(MIDDLE,duration);
    bios_show_value(TOP,duration);
  }
}

void bios_show_horizontal_swipe_right(uint16_t duration,uint16_t times)
{
  for(uint16_t ii=0;ii<times;++ii)
  {
    uint16_t values[4]={LEFT,OFF,OFF,OFF};
    bios_show_values(values,duration);
    values[0]=1;
    bios_show_values(values,duration);
    values[0]=OFF;
    values[1]=LEFT;
    bios_show_values(values,duration);
    values[1]=1;
    bios_show_values(values,duration);
    values[1]=OFF;
    values[2]=LEFT;
    bios_show_values(values,duration);
    values[2]=1;
    bios_show_values(values,duration);
    values[2]=OFF;
    values[3]=LEFT;
    bios_show_values(values,duration);
    values[3]=1;
    bios_show_values(values,duration);
  }
}

void bios_show_horizontal_swipe_left(uint16_t duration,uint16_t times)
{
  for(uint16_t ii=0;ii<times;++ii)
  {
    uint16_t values[4]={OFF,OFF,OFF,1};
    bios_show_values(values,duration);
    values[3]=LEFT;
    bios_show_values(values,duration);
    values[3]=OFF;
    values[2]=1;
    bios_show_values(values,duration);
    values[2]=LEFT;
    bios_show_values(values,duration);
    values[2]=OFF;
    values[1]=1;
    bios_show_values(values,duration);
    values[1]=LEFT;
    bios_show_values(values,duration);
    values[1]=OFF;
    values[0]=1;
    bios_show_values(values,duration);
    values[0]=LEFT;
    bios_show_values(values,duration);
  }
}

void bios_show()
{
  uint16_t vertical_duration=100;
  uint16_t horizontal_duration=75;

  bios_show_vertical_swipe_down(vertical_duration,3);
  
  for(uint16_t ii=0;ii<3;++ii)
  {
    bios_show_horizontal_swipe_right(horizontal_duration,1);
    bios_show_horizontal_swipe_left(horizontal_duration,1);
  }
  
  bios_show_vertical_swipe_up(vertical_duration,3);

  l74_values[0]=12;
  l74_values[1]=1;
  l74_values[2]=3;
  l74_values[3]=2;
  l74_update();
}

void set_cursor_pos(int16_t pos)
{
  cursor_pos=pos;

  while(cursor_pos<0)
    cursor_pos+=4;
  while(cursor_pos>=4)
    cursor_pos-=4;

  for(uint16_t ii=0;ii<4;++ii)
    l74_blinks[ii]=false;

  l74_blinks[cursor_pos]=true;
}

void speaker_shoot()
{
  for(int16_t ii=150;ii>=0;--ii)
  {
    analogWrite(speaker_pin,ii);
    portal_delay(ii/20);
  }
}

void shoot()
{
  deflector_on();
  speaker_shoot();
  deflector_off();
  kill_timer=millis()+kill_timeout;
}

void enter_none_mode()
{
  kill_timer=millis()+kill_timeout;
  mode=NONE;
  bios_show_value(OFF,100);
  button_reset();
}

void enter_edit_mode()
{
  kill_timer=millis()+kill_timeout;
  mode=EDIT;
  set_cursor_pos(cursor_pos);
  button_reset();
}

void enter_digit_mode()
{
  kill_timer=millis()+kill_timeout;
  mode=DIGIT;
  set_cursor_pos(cursor_pos);
  button_reset();
}

void enter_shoot_mode()
{
  kill_timer=millis()+kill_timeout;
  mode=SHOOT;
  cursor_pos=3;
  for(uint16_t ii=0;ii<4;++ii)
    l74_blinks[ii]=false;
  button_reset();
  can_shoot=false;
}

void update_edit_mode()
{
  button_update();
  
  encoder.tick();
  int16_t new_encoder_pos=encoder.getPosition();

  if(encoder_pos!=new_encoder_pos)
  {
    kill_timer=millis()+kill_timeout;

    bool negative=(new_encoder_pos<encoder_pos);

    if(negative)
      set_cursor_pos(cursor_pos-1);
    else
      set_cursor_pos(cursor_pos+1);
  }

  encoder_pos=new_encoder_pos;
  
  if(button_down_time>button_off_time)
    power_off();

  if(button_down&&button_down_time>button_activate_time)
    enter_shoot_mode();
  else if(!button_down&&button_down_time>button_pressed_time)
    enter_digit_mode();
  else if(!button_down&&button_down_time>0)
    button_reset();

  portal_delay(80);

  l74_update();
}

void update_digit_mode()
{
  button_update();

  encoder.tick();
  int16_t new_encoder_pos=encoder.getPosition();

  if(encoder_pos!=new_encoder_pos)
  {
    kill_timer=millis()+kill_timeout;

    bool negative=(new_encoder_pos<encoder_pos);

    if(negative)
      --l74_values[cursor_pos];
    else
      ++l74_values[cursor_pos];

    l74_number_rollover_independent();
  }

  encoder_pos=new_encoder_pos;

  if(button_down&&button_down_time>button_activate_time)
    enter_shoot_mode();
  else if(!button_down&&button_down_time>button_pressed_time)
    enter_edit_mode();
  else if(!button_down&&button_down_time>0)
    button_reset();

  portal_delay(80);

  l74_update();
}

void update_shoot_mode()
{
  button_update();
  
  if(button_down&&button_down_time>button_activate_time)
    enter_digit_mode();
  else if(!button_down&&button_down_time>button_shoot_time)
  {
    shoot();
    button_reset();
  }
  else if(!button_down&&button_down_time>0)
    button_reset();

   portal_delay(80);

   l74_update();
}

void setup()
{
  power_on();
  
  button_setup();
  speaker_setup();
  lights_setup();
  lcd_setup();

  crystal_on();

  enter_none_mode();
  bios_show();
  portal_delay(1000);
  set_cursor_pos(3);
  enter_edit_mode();
}

void loop()
{
  l74_values[0] = 5;
  while(true)
  {
    l74_update();
   /* if(mode==EDIT)
      update_edit_mode();
    else if(mode==DIGIT)
      update_digit_mode();
    else if(mode==SHOOT)
      update_shoot_mode();

    if(millis()>kill_timer)
      power_off();*/
  }

  power_off();
}

