#include "coin.h"
#include "lasso.h"

void Coin::initCoin(int n) { 
  //drawing coins at start position
  
  coin_start_y = PLAY_Y_HEIGHT;
  //common y position for all coins
  
  coin_start_x = (WINDOW_X)/2;
  //starting x position of first coin
  
  if (n == 1)
    coin_start_x = (2*PLAY_X_START + WINDOW_X)/2;
    //starting x position of second coin
  else if (n == 2)
    coin_start_x = (4*PLAY_X_START + WINDOW_X)/2;
    //starting x position of third coin
    
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("gold"));
  coin_circle.setFill(true);
  //coin made
  
  gold[0] = 0; gold[1] = 0; gold[2] = 0;
  //act as bool variables to check if coin caught is special or not
  
  addPart(&coin_circle); 
  //this function adds address of each new part at the end of a vector
  
}

void Coin::resetCoin(int n) {
  //resets coin after each coin jump interval

  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  //constants related to motion
  
  if (n == 1)
    coin_start_x = (2*PLAY_X_START + WINDOW_X)/2;
    //reset for second coin
  else if (n == 2)
    coin_start_x = (4*PLAY_X_START + WINDOW_X)/2;
    //reset for third coin
    
  for (int i = 0; i <= 2; i = i + 1)
    goldstep[i] = (goldstep[i] + 1) % (3 + i); 
    //counter for changing each coin to special coin after different intervals
     
  if (goldstep[n] == 1) {
    gold[n] = 1; bomb[n] = 0;
    coin_circle.setColor(COLOR("blue"));
    coin_circle.setFill(true);
    //(n+1)th coin is special
  }
  else {
    gold[n] = 0;
    coin_circle.setColor(COLOR("gold"));
    coin_circle.setFill(true);
    //(n+1)th coin is not special 
  }
  
  for (int i = 0; i <= 2; i = i + 1)
    bombstep[i] = (bombstep[i] + 1) % (4 + i); 
    //counter for changing each coin to bomb after different intervals
  
  if (bombstep[n] == 1) {
    bomb[n] = 1; gold[n] = 0;
    coin_circle.setColor(COLOR("red"));
    coin_circle.setFill(true);
    //(n+1)th coin is bomb
  }
  else if (goldstep[n] != 1) {
    bomb[n] = 0;
    coin_circle.setColor(COLOR("gold"));
    coin_circle.setFill(true); 
    //(n+1)th coin is not bomb
  }
  
  //conditions are placed such that
  //in case of a conflict between special and bomb coin
  //bomb will spawn preferentially
   
  coin_ax = 0;    
  coin_ay = COIN_G;
  //reset common for all coins
  
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  //resets the (n+1)th coin to its initial position 
  //with the desired normal/special/bomb nature
  
}
