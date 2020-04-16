// @author: scott
// @brief: DaSE Software Engneering Class Project 1

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

// add int32 format
#include <stdint.h>

#include <math.h>

#include <string>


// convert string to integer
// if cannot convert, return 0
int32_t  str_to_int32(std::string str) {
  int32_t  val = (int32_t)atoi(str.c_str());
  return val;
}

// convert string to double
// if cannot convert, return 0
double str_to_double(std::string str) {
  double integer = 0;  // integer part of double number 
  double decimal = 0;  // decimal part of double number
  int str_len = (int)strlen(str.c_str());
  int point = str_len;
  
  for (int i = 0; i < str_len; i++) {
    if (str[i] >= '0' && str[i] <= '9')
      continue;
    else if (str[i] == '.'){
        if (point != str_len)     // had assigned point position
            return 0;
        else
            point = i;
    }
    else                        // if exist other charactors
      return 0;
  }

  integer = (double)str_to_int32(str.substr(0, point));
  if (point < str_len - 1)
    decimal = (double)str_to_int32(str.substr(point + 1, str_len - point - 1));

  return integer + decimal / pow(10, str_len - point - 1);
}


int main() {
  std::string str;
  str.resize(100);
  scanf("%s", &str[0]);

  double val_double = str_to_double(str);
  int32_t  val_int32 = str_to_int32(str);
  
  printf("convert to double: %f\n",val_double);
  printf("convert to int32: %d\n",val_int32);
  return 0;
}
