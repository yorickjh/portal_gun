#include "l74digit.h"

int16_t l74_values[4]={OFF,OFF,OFF,OFF};
bool l74_blinks[4]={false,false,false,false};

static uint8_t l74_digits[4]={A0, A1, A2, A3}; //A3,A2,A1,A0};
static uint8_t l74_segments[7]={5,6,7,8,9,10,11};
bool l74_blink_show=true;
static uint32_t l74_blink_timer=0;
static uint16_t l74_blink_duration=333;

static void l74_set_value(int16_t value)
{
 /* switch(value)
  {
    case 0:*/
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],HIGH);
     /* break;
    case 1:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],HIGH);
      break;
    case 2:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 3:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 4:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 5:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 6:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 7:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],HIGH);
      break;
    case 8:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 9:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 10:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 11:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 12:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],HIGH);
      break;
    case 13:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],LOW);
      digitalWrite(l74_segments[2],LOW);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 14:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case 15:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],LOW);
      break;
    case TOP:
      digitalWrite(l74_segments[0],LOW);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],HIGH);
      break;
    case MIDDLE:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],LOW);
      break;
    case BOTTOM:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],LOW);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],HIGH);
      break;
    case LEFT:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],LOW);
      digitalWrite(l74_segments[5],LOW);
      digitalWrite(l74_segments[6],HIGH);
      break;
    case OFF:
      digitalWrite(l74_segments[0],HIGH);
      digitalWrite(l74_segments[1],HIGH);
      digitalWrite(l74_segments[2],HIGH);
      digitalWrite(l74_segments[3],HIGH);
      digitalWrite(l74_segments[4],HIGH);
      digitalWrite(l74_segments[5],HIGH);
      digitalWrite(l74_segments[6],HIGH);
      break;
  }*/
}

void l74_initialize()
{
  for(uint16_t ii=0;ii<4;++ii)
  {
    pinMode(l74_digits[ii],OUTPUT);
    digitalWrite(l74_digits[ii],LOW);
  }

  for(uint16_t ii=0;ii<7;++ii)
  {
    pinMode(l74_segments[ii],OUTPUT);
    digitalWrite(l74_segments[ii],LOW);
  }
}

void l74_number_rollover_dependent()
{
  for(int16_t ii=3;ii>=0;--ii)
  {
    if(l74_values[ii]>15)
    {
      l74_values[ii]=0;

      if(ii>0)
        ++l74_values[ii-1];
    }

    if(l74_values[ii]<0)
    {
      l74_values[ii]=15;

      if(ii>0)
        --l74_values[2];
    }
  }
}

void l74_number_rollover_independent()
{
  for(uint16_t ii=0;ii<4;++ii)
  {
    if(l74_values[ii]>15)
      l74_values[ii]=0;
    if(l74_values[ii]<0)
      l74_values[ii]=15;
  }
}

void l74_update()
{
  for(uint16_t ii=0;ii<4;++ii)
    digitalWrite(l74_digits[ii],LOW);

  for(uint16_t ii=0;ii<4;++ii)
  {
    if(millis()>l74_blink_timer)
    {
      l74_blink_timer=millis()+l74_blink_duration;
      l74_blink_show=!l74_blink_show;
    }

    if((l74_blinks[ii]&&l74_blink_show)||!l74_blinks[ii])
      l74_set_value(l74_values[ii]);
    else
      l74_set_value(OFF);

    digitalWrite(l74_digits[ii],HIGH);
    delay(5);
    digitalWrite(l74_digits[ii],LOW);
  }
}

