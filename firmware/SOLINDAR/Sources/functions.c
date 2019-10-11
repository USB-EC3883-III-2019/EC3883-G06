#include "functions.h"



//Motor de paso control
int motor_run(int* dir,int* counter,int* max){
  if(*dir)
    (*counter)++;
  else
    (*counter)--;
  if(*counter>=(*max))
    *dir=!(*dir);
  if((*counter)<=0)
    *dir=!(*dir);
  return (*counter)%8;
}
