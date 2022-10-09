#include <simplecpp>
#include <string>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"

using namespace simplecpp;

main_program {

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  //graphics window created
  
  //Control board
  
  Rectangle controls((PLAY_X_START + WINDOW_X)/2, 1.2*(WINDOW_Y - PLAY_Y_HEIGHT), PLAY_X_WIDTH - 2*PLAY_X_START, 2*(WINDOW_Y - PLAY_Y_HEIGHT));
  //outer box
  
  int X_START = PLAY_X_START;
  int INDX = 20;
  int HALF = (PLAY_X_WIDTH - 2*PLAY_X_START)/2 - 30;
  int Y_START = 0.2*(WINDOW_Y - PLAY_Y_HEIGHT);
  int INDY = 20;
  //spacing constants
  
  Text t (X_START + INDX + 0.5*HALF, Y_START + INDY, "t : to throw the lasso");
  Text l (X_START + 2*INDX + 1.5*HALF, Y_START + INDY, "l: to expand lasso loop");
  
  Text y (X_START + INDX + 0.5*HALF, Y_START + 2*INDY, "y: to yank the lasso");
  Text m (X_START + 2*INDX + 1.5*HALF, Y_START + 2*INDY, "m: to reduce release angle");
  
  Text p (X_START + INDX + 0.5*HALF, Y_START + 3*INDY, "p: to increase release angle");
  Text r (X_START + 2*INDX + 1.5*HALF, Y_START + 3*INDY, "-: to reduce release speed");
  
  Text i (X_START + INDX + 0.5*HALF, Y_START + 4*INDY, "=: to increase release speed");
  Text q (X_START + 2*INDX + 1.5*HALF, Y_START + 4*INDY, "q: to quit the game");
  
  //control information
  
  Text note1 (X_START + HALF, Y_START + 5*INDY, "Black coins are worth 1 point each");
  
  Circle B1 (1.5*X_START + INDX + HALF/4, Y_START + 4.75*INDY, COIN_SIZE);
  Circle B2 (1.5*X_START + HALF/2, Y_START + 4.75*INDY, COIN_SIZE);
  Circle B3 (1.5*X_START + 3*HALF/4 - INDX, Y_START + 4.75*INDY, COIN_SIZE);
  
  B1.setColor(COLOR("gold"));
  B1.setFill(true);
  B2.setColor(COLOR("gold"));
  B2.setFill(true);
  B3.setColor(COLOR("gold"));
  B3.setFill(true);
  
  //black coin details
  
  Text note2 (X_START + HALF, Y_START + 6*INDY, "Blue coins give different bonus points");
  
  Circle bl1 (1.5*X_START + INDX + HALF/4, Y_START + 5.75*INDY, COIN_SIZE);
  Circle bl2 (1.5*X_START + HALF/2, Y_START + 5.75*INDY, COIN_SIZE);
  Circle bl3 (1.5*X_START + 3*HALF/4 - INDX, Y_START + 5.75*INDY, COIN_SIZE);
  
  bl1.setColor(COLOR("blue"));
  bl1.setFill(true);
  bl2.setColor(COLOR("blue"));
  bl2.setFill(true);
  bl3.setColor(COLOR("blue"));
  bl3.setFill(true);
  
  //blue coin details
  
  Text note3 (X_START + HALF, Y_START + 7*INDY, "Red coins deduct points and costs lives");
  
  Circle r1 (1.5*X_START + INDX + HALF/4, Y_START + 6.75*INDY, COIN_SIZE);
  Circle r2 (1.5*X_START + HALF/2, Y_START + 6.75*INDY, COIN_SIZE);
  Circle r3 (1.5*X_START + 3*HALF/4 - INDX, Y_START + 6.75*INDY, COIN_SIZE);
  
  r1.setColor(COLOR("red"));
  r1.setFill(true);
  r2.setColor(COLOR("red"));
  r2.setFill(true);
  r3.setColor(COLOR("red"));
  r3.setFill(true);
  
  //red coin details
  
  Text note4 (X_START + 0.8*HALF, Y_START + 8*INDY, "All types will spawn almost randomly!");
  Text note5 (X_START + 0.8*HALF, Y_START + 9*INDY, "Timing is the key! Save your lives!");
  
  //game designer comments
  
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; 
  //will keep running for infinite time without reporting error
  float currTime = 0; 

  // Draw lasso at start position
  
  double release_speed = INIT_RELEASE_SPEED; 
  //in m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; 
  //in degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);  
  //lasso super-variable defined

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT); 
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue")); 
  //game field borders set

  string msg("Cmd: _"); 
  //will take input
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg); 
  //will show input commands
  
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins()); 
  //go to lasso.h near line 93
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr); 
  //will show score
  
  char livesLeft[256];
  sprintf(livesLeft, "Lives: %d", lasso.getLives());
  Text life(PLAY_X_START+50, PLAY_Y_HEIGHT+80, livesLeft);
  
  //scorecard set
  
  paused = true; rtheta = true; 
  
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  //coin variables initialised
  
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 0);
  Coin c1(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 1);
  Coin c2(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 2);
  
  //all 3 coins super-variable defined 

  // After every COIN_GAP sec, make the coin jump
  
  double last_coin_jump_end = 0;
  double last_c1_jump_end = 0;
  double last_c2_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  char c = '0';
  //control variable for input
  
  for(;;) {
  
    if (lasso.getLives() == 0) {
      Rectangle over (WINDOW_X/2, WINDOW_Y/2, 100, 50);
      Text gameover (WINDOW_X/2, WINDOW_Y/2, "GAME OVER!");
      //game over message
      
      wait(2);
      exit(0);
      //if lives finish off the game quits automatically
    }
      
    if((runTime > 0) && (currTime > runTime)) { break; }
    //loop runs till command 'q' is given

    XEvent e;
    //variable that takes an event as input
    
    bool pendingEv = checkEvent(e);
    //standard function to return true if an event has occured
    
    if (c == charFromEvent(e))
      pendingEv = false;
    //same character will not be processed twice in a row 
    
    if(pendingEv) { 
      
      c = charFromEvent(e);
      //new key pressed is taken in
      
      msg[msg.length()-1] = c;
      //string stores the key pressed in the end
      
      charPressed.setMessage(msg);
      //standard function to read string into the text on scorecard
      
      switch(c) {
      
      case 't':
	lasso.unpause();
	//lasso thrown if it was stationary
	
	break;
	
      case 'y':
	lasso.yank();
	//lasso pulled back and reset
	
	break;
	
      case 'l':
	lasso.loopit();
	//lasso loop expands
	
	lasso.check_for_coin(&coin, 0);
	lasso.check_for_coin(&c1, 1);
	lasso.check_for_coin(&c2, 2);
	//individually checking if any coin was caught within some time
	//after expanding of loop
	
	wait(STEP_TIME*5);
	//coin captured if looped
	break;
	
      case 'm': 
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG); }
	//reduces release angle if lasso is paused
	
	break;
	
      case 'p':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	//increases release angle if lasso is paused
	
	break;
	
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	//decreases release speed if lasso is paused
	
	break;
	
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	//increases release speed if lasso is paused
	
	break;
	
      case 'q':
	exit(0);
	//exits the game if q is pressed
	
      default:
	break;
      }
    
    }
                                                     
    lasso.nextStep(stepTime);
    //lasso motion simulated with time

    coin.nextStep(stepTime);
    c1.nextStep(stepTime);
    c2.nextStep(stepTime);
    //motion of each coin with time
    
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
      //makes coin move again if it has stayed stopped for a while
      
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin(0);
      last_coin_jump_end = currTime;
      //makes coin stop if it reaches the lowest point and toggles nature
      
    }
    
    if(c1.isPaused()) {
      if((currTime-last_c1_jump_end) >= COIN_GAP) {
	c1.unpause();
      }
      //makes c1 move again if it has stayed stopped for a while
      
    }

    if(c1.getYPos() > PLAY_Y_HEIGHT) {
      c1.resetCoin(1);
      last_c1_jump_end = currTime;
      //makes c1 stop if it reaches the lowest point and toggles nature
      
    }
    
    if(c2.isPaused()) {
      if((currTime-last_c2_jump_end) >= COIN_GAP) {
	c2.unpause();
      }
      //makes coin move again if it has stayed stopped for a while
      
    }

    if(c2.getYPos() > PLAY_Y_HEIGHT) {
      c2.resetCoin(2);
      last_c2_jump_end = currTime;
      //makes coin stop if it reaches the lowest point and toggles nature
      
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    //coin score update
    
    sprintf(livesLeft, "Lives: %d", lasso.getLives());
    life.setMessage(livesLeft);
    //lives update

    stepCount++;
    currTime += stepTime;
    wait(stepTime); 
    //preparing for next iteration
    
  } // End for(;;)

  wait(3);
} // End main_program
