#ifndef __COIN_H__
#define __COIN_H__

#include "MovingObject.h"

class Coin : public MovingObject
//class MovingObject inheriting public members of class Coin
  
{

  double coin_start_x;
  double coin_start_y;
  //initial position of each coin
  
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  //other coin variables
  
  int goldstep[3] = {0, 0, 0};
  int bombstep[3] = {2, 2, 2};
  //step variables to account for special and bomb coins

  Circle coin_circle;
  //coin circle

 public:
  Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, int i) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
 
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    //coin motion variables
    
    initCoin(i); 
    //(i+1)th coin gets drawn
    
  }

  void initCoin (int n);
  void resetCoin (int n);
  //coin functions
  
  int gold[3] = {0, 0, 0};
  int bomb[3] = {0, 0, 0};
  //act as bool variables to toggle special and bomb nature

}; // End class Coin

#endif
