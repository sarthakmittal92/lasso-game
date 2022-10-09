#include <simplecpp>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"

using namespace simplecpp;

void Lasso::draw_lasso_band() {

  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  //lasso band variables
  
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS); 
  //lasso band drawn
  
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET); 
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  //initial position
  
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  //lasso loop circle
  
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("brown"));
  lasso_loop.setFill(true);
  //lasso inner circle
  
  addPart(&lasso_circle);  
  addPart(&lasso_loop);  
  //lasso parts added into the vector
  
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;
  lives = 5;
  //initial values set

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y); 
  lasso_line.setColor(COLOR("brown"));
  //lasso line drawn

  lasso_band.setColor(COLOR("BlueViolet"));
  draw_lasso_band(); 
  //lasso band drawing

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  //resets lasso to start after yanking
  
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  //loop has been reset
  
  if(the_coin != NULL && (coincaught || c1caught || c2caught)) {
    num_coins++;
    //standard score increment
    
    if (coincaught) {
      if (the_coin->bomb[0] == 1) {
        lives--;
        //life deducted if bomb is caught
        if (num_coins >= 4)
          num_coins = num_coins - 4;
        else
          num_coins = 0;
          //3 points deducted or score reset to 0 
      }
      if (the_coin->gold[0] == 1)
        num_coins = num_coins + 1;
        //1 bonus point for first coin
      the_coin->resetCoin(0);
    }
    
    if (c1caught) {
      if (the_coin->bomb[1] == 1) {
        lives--;
        //life deducted if bomb iscaught
        if (num_coins >= 3) 
          num_coins = num_coins - 3;
        else
          num_coins = 0;
          //2 points deducted or score reset to 0
      }
      if (the_coin->gold[1] == 1)
        num_coins = num_coins + 2;
        //2 bonus points for second coin
      the_coin->resetCoin(1);
    } 
     
    if (c2caught) {
      if (the_coin->bomb[2] == 1) {
        lives--;
        //life deducted if bomb is caught
        if (num_coins >= 2)
          num_coins = num_coins - 2;
        else
          num_coins = 0;
          //1 point deducted or score reset to 0
      }
      if (the_coin->gold[2] == 1)
        num_coins = num_coins + 4;
        //4 bonus points for third coin
      the_coin->resetCoin(2);
    }
      
    coincaught = false; c1caught = false; c2caught = false;
    //declaring coins as released
    //modifying score only if coin was caught just before yanking
    
  }
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } 
  //already looped
  
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
  //lasso loop expands into a circle
  
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  //raises or reduces angle if within the required range
  
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  //lasso reset to new parameters
  
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  //raises or reduces speed if within required range
  
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  //lasso reset to new parameters
  
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  
  MovingObject::nextStep(stepTime);
  //simulates the time dependence
  
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  //if lasso goes beyond the playing arena then it is yanked back
  
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
  //line connecting release spot to the loop
  
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr, int i) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  //lasso position recorded
  
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  //coin position recorded
  
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  //checking looping of coin
  
  if(distance <= LASSO_RADIUS) {
    if (i == 0)
      coincaught = true; 
    else if (i == 1)
      c1caught = true; 
    else if (i == 2)
      c2caught = true;
    //notes which coin got looped
    
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
    //invoking object of check_for_coin (lasso) which gets passed
    //caught coin will go back with lasso
  }
  else if (the_coin == NULL) {
    coincaught = false; c1caught = false; c2caught = false;
  }
    //if no coin was looped
    
} // End Lasso::check_for_coin()
