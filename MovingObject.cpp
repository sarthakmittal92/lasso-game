#include <simplecpp>
#include "MovingObject.h"

using namespace simplecpp;

void MovingObject::nextStep(double t) {

  if(paused) { return; }
  //no movement if paused
  
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  //moves all parts one-by-one
  
  vx += ax*t;
  vy += ay*t;
  //velocity update
  
} // End MovingObject::nextStep()

double MovingObject::getXPos() {

  if (parts.size() > 0) 
    return parts[0]->getX();
  else
    return -1;
  //returns the x coordinate of first part
  
}

double MovingObject::getYPos() {

  if (parts.size() > 0)
    return parts[0]->getY();
  else
    return -1;
  //returns y coordinate of first part
  
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
    //moves all parts to the next position
    
  }
  
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
  //changes values for the next iteration
  
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  //this function has argument as lasso (invoking object of check_for_coin)

  double xpos = m->getXPos();
  double ypos = m->getYPos();
  //lasso position determined
  
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  //coin (invoking object of getAttachedTo) positioned onto the lasso
  
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
  //coin will move along with lasso
  
}
