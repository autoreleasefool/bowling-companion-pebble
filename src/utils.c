#include <pebble.h>
#include "utils.h"

char *itoa(int num)
{
  static char buff[20] = {};
  int i = 0, temp_num = num, length = 0;
  char *string = buff;
  if(num >= 0) {
    // count how many characters in the number
    while(temp_num) {
      temp_num /= 10;
      length++;
    }
    // assign the number to the buffer starting at the end of the 
    // number and going to the begining since we are doing the
    // integer to character conversion on the last number in the
    // sequence
    for(i = 0; i < length; i++) {
      buff[(length-1)-i] = '0' + (num % 10);
      num /= 10;
    }
    buff[i] = '\0'; // can't forget the null byte to properly end our string
  } else {
    return "Unsupported Number";
  }
  return string;
}

int get_value_of_frame(int frame, int ball, bool pin_state[]) {
  int frameValue = 0;
  for (int i = 0; i < 5; i++)
  {
    if (pin_state[frame * 15 + ball * 5 + i])
    {
      switch (i)
      {
        case 0:
        case 4:
          frameValue += 2;
          break;
        case 1:
        case 3:
          frameValue += 3;
          break;
        case 2:
          frameValue += 5;
          break;
      }
    }
  }
  return frameValue;
}

int get_value_of_frame_diff(int frame1, int ball1, int frame2, int ball2, bool pin_state[]) {
  int frameValue = 0;
  for (int i = 0; i < 5; i++)
  {
    if (pin_state[frame2 * 15 + ball2 * 5 + i] && !pin_state[frame1 * 15 + ball1 * 5 + i])
    {
      switch (i)
      {
        case 0:
        case 4:
          frameValue += 2;
          break;
        case 1:
        case 3:
          frameValue += 3;
          break;
        case 2:
          frameValue += 5;
          break;
      }
    }
  }
  return frameValue;
}

bool is_frame_strike(int frame, int ball, bool pin_state[]) {
  int pins = frame * 15 + ball * 5;
  return (pin_state[pins] && pin_state[pins + 1] && pin_state[pins + 2] && pin_state[pins + 3] && pin_state[pins + 4]);
}