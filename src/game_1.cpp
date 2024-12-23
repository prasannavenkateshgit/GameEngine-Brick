#include "game.hpp"

std::string selfID;
ReplayEvent* replay_event;
PlaybackEvent* playback_event;
const int ROW = 5;
const int COL = 7;
const int BALL_SPEED = 8;
const int SPACING = 16;
const int SIZE = 16;
const int PI = 3.14;
bool bricks[ROW*COL];
SDL_Rect brick;
int lives;
float velY=BALL_SPEED/2;
float velX=0;

void setBricks(int i) {
 brick.w=(SCREEN_WIDTH-(SPACING*COL))/COL;
 brick.h=22;
 brick.x=(((i%COL)+1)*SPACING)+((i%COL)*brick.w)-(SPACING/2);
 brick.y=brick.h*3+(((i%ROW)+1)*SPACING)+((i%ROW)*brick.h)-(SPACING/2);
}

void resetBricks(GameObjects* game_objects) {
    for(int i=0; i<COL*ROW; i++){
        Color c;
        if(i%2==0)c={0,255,255};
        else c={255,0,255};
        setBricks(i);
        SDL_Rect* brick2 = &brick;
        game_objects->addEntity("brick"+std::to_string(i), new Rect(brick2->x,brick2->y,brick2->w,brick2->h,true,c,false));
    }
    cout<<"RESET BRICKS"<<endl;
    for(int i=0; i<COL*ROW; i++) bricks[i]=1;
    lives=2;
    float paddle_w = game_objects->getEntity(selfID)->getW();
    float paddle_x = (SCREEN_WIDTH/2)-(paddle_w/2);
    game_objects->getEntity(selfID)->setX(paddle_x);
    float paddle_y = game_objects->getEntity(selfID)->getY();
    float paddle_h = game_objects->getEntity(selfID)->getH();
    game_objects->getEntity("ball")->setY(paddle_y-(paddle_h*4));
    game_objects->getEntity("ball")->setX(SCREEN_WIDTH/2-(SIZE/2));
    game_objects->getEntity("ball")->setVelocityY(BALL_SPEED/2);
    game_objects->getEntity("ball")->setVelocityX(0);
}

void update(GameObjects* game_objects) {
 if(lives<=0) resetBricks(game_objects);
 SDL_Rect ball;
 ball.x=game_objects->getEntity("ball")->getX();
 ball.y=game_objects->getEntity("ball")->getY();
 ball.w=game_objects->getEntity("ball")->getW();
 ball.h=game_objects->getEntity("ball")->getH();
 SDL_Rect paddle;
 paddle.x=game_objects->getEntity(selfID)->getX();
 paddle.y=game_objects->getEntity(selfID)->getY();
 paddle.w=game_objects->getEntity(selfID)->getW();
 paddle.h=game_objects->getEntity(selfID)->getH();
 if(SDL_HasIntersection(&ball, &paddle)) {
  double rel=(paddle.x+(paddle.w/2))-(ball.x+(SIZE/2));
  double norm=rel/(paddle.w/2);
  double bounce = norm* (5*PI/12);
  velY=-BALL_SPEED*cos(bounce);
  velX=BALL_SPEED*-sin(bounce);
//   game_objects->setEntityVelocityX("ball", velX);
//   game_objects->setEntityVelocityY("ball", velY);
 }
 if(ball.y<=0){
  velY=-velY;
  //game_objects->getEntity("ball")->setVelocityY(velY);
  game_objects->setEntityVelocityY("ball", velY);
 }
 if(ball.y+SIZE>=SCREEN_HEIGHT) 
 {
    velY=-velY;
    lives--;
    //game_objects->getEntity("ball")->setVelocityY(velY);
    game_objects->setEntityVelocityY("ball", velY);
 }
 if(ball.x<=0 || ball.x+SIZE>=SCREEN_WIDTH){
  velX=-velX;
  //game_objects->getEntity("ball")->setVelocityX(velX);
  game_objects->setEntityVelocityX("ball", velX);
 }
 ball.x+=velX;
 ball.y+=velY;
 game_objects->getEntity("ball")->setX(ball.x);
 game_objects->getEntity("ball")->setY(ball.y);
 game_objects->setEntityVelocityX("ball", velX);
 game_objects->setEntityVelocityY("ball", velY);
 if(paddle.x<0) {
  paddle.x=0;
  game_objects->getEntity(selfID)->setX(paddle.x);
 }
 if(paddle.x+paddle.w>SCREEN_WIDTH){
  paddle.x=SCREEN_WIDTH-paddle.w;
  game_objects->getEntity(selfID)->setX(paddle.x);
 }

 bool reset=1;
 for(int i=0; i<COL*ROW; i++) {
  setBricks(i);
  if(SDL_HasIntersection(&ball, &brick) && bricks[i]) {
   bricks[i]=0;
    game_objects->removeEntity("brick"+std::to_string(i));
   if(ball.x >= brick.x) 
   {    velX=-velX; 
        ball.x-=20;
        game_objects->getEntity("ball")->setX(ball.x);
        //game_objects->getEntity("ball")->setVelocityX(velX);
        game_objects->setEntityVelocityX("ball", velX);
   }
   if(ball.x <= brick.x) 
   {
        velX=-velX;
        ball.x+=20;
        game_objects->getEntity("ball")->setX(ball.x);
        //game_objects->getEntity("ball")->setVelocityX(velX);
        game_objects->setEntityVelocityX("ball", velX);
    }
   if(ball.y <= brick.y) 
   {
        velY=-velY;
        ball.y-=20;
        game_objects->getEntity("ball")->setY(ball.y);
        //game_objects->getEntity("ball")->setVelocityY(velY);
        game_objects->setEntityVelocityY("ball", velY);
   }
   if(ball.y >= brick.y) 
   {
        velY=-velY;
        ball.y+=20;
        game_objects->getEntity("ball")->setY(ball.y);
        //game_objects->getEntity("ball")->setVelocityY(velY);
        game_objects->setEntityVelocityY("ball", velY);
    }
  }
  if(bricks[i]) reset=0;
 }
 if(reset) resetBricks(game_objects);
}

GameObjects* loadEntities(std::string clientID, Color color)
{
    global_time = new Timeline();
    physics = new Physics(0);
    selfID = clientID;
    GameObjects* game_objects = new GameObjects();
    //game_objects->addEntity(clientID, new Rect(SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.7 - 50, 20, 30, true, color, true));
    game_objects->addEntity(selfID, new Rect((SCREEN_WIDTH/2)-(SCREEN_WIDTH/4),SCREEN_HEIGHT-48,SCREEN_WIDTH/4,12,true,{255,255,255},false));
    float paddle_y = SCREEN_HEIGHT-48;
    float paddle_h = 12;
    game_objects->addEntity("ball", new Rect(SCREEN_WIDTH/2-8,paddle_y-(paddle_h*4),16,16,false,{255,255,255},false));
    // for(int i=0; i<COL*ROW; i++){
    //     Color c;
    //     if(i%2==0)c={0,255,0};
    //     else c={255,0,0};
    //     setBricks(i);
    //     SDL_Rect* brick2 = &brick;
    //     game_objects->addEntity("brick"+std::to_string(i), new Rect(brick2->x,brick2->y,brick2->w,brick2->h,true,c,false));
    // }
    input_event = new InputEvent(moveEntity, game_objects, global_time->getTime(), EventPriority::HIGH);
    input_event->setType("Input");
    event_manager.registerEvent(input_event);
    // replay_event = new ReplayEvent(std::string("game_1"), global_time->getTime(), EventPriority::LOW);
    // replay_event->setType("replay");
    // event_manager.registerEvent(replay_event);
    // playback_event = new PlaybackEvent(std::string("game_1"), global_time->getTime(), EventPriority::HIGH);
    // playback_event->setType("playback");
    // event_manager.registerEvent(playback_event);
    return game_objects;
}

void moveEntity(GameObjects* game_objects, Uint8 *keys)
{
    int SPEED=15;
    if(keys[SDL_SCANCODE_RIGHT])
    {
        float x = game_objects->getEntity(selfID)->getX();
        game_objects->getEntity(selfID)->setX(x+SPEED);
    }
    else if(keys[SDL_SCANCODE_LEFT])
    {
        float x = game_objects->getEntity(selfID)->getX();
        game_objects->getEntity(selfID)->setX(x-SPEED);
    }
    // if (keys[SDL_SCANCODE_T])
    // {
    //     isProportional = !isProportional;
    //     if(isProportional)
    //     {
    //         std::cout<<"Object sizes are set to proportional"<<std::endl;
    //     }
    //     else
    //     {
    //         std::cout<<"Object sizes are set to constant"<<std::endl;
    //     }
    // }
    // if (keys[SDL_SCANCODE_R])
    // {
    //     std::cout<<"RECORDING"<<std::endl;
    //     event_manager.raiseEvent(replay_event);
    //     // recording = !recording;
    // }
    // if (keys[SDL_SCANCODE_Y])
    // {
    //     std::cout<<"PLAYBACK"<<std::endl;
    //     event_manager.raiseEvent(playback_event);
    //     // playback = !playback;
    // }
    // if(keys[SDL_SCANCODE_KP_PLUS])
    // {
    //     float tic = global_time->getTic();
    //     global_time->changeTic(tic + 0.1);
    //     std::cout<<"Tic set to "<<global_time->getTic()<<std::endl;
    // }
    // else if(keys[SDL_SCANCODE_KP_MINUS])
    // {
    //     float tic = global_time->getTic();
    //     global_time->changeTic(tic - 0.1);
    //     std::cout<<"Tic set to "<<global_time->getTic()<<std::endl;
    // }
    // if(keys[SDL_SCANCODE_P])
    // {
    //     if(global_time->isPaused())
    //     {
    //         global_time->unpause();
    //         std::cout<<"Game Unpaused"<<std::endl;
    //     }
    //     else{
    //         global_time->pause();
    //         std::cout<<"Game Paused"<<std::endl;
    //     }
    // }
    // float vel_x = 0.0, vel_y = 0.0;
    // if(keys[SDL_SCANCODE_RIGHT])
    // {
    //     if(keys[SDL_SCANCODE_LSHIFT])
    //     {
    //         vel_x = global_time->getTic()? 20.0 / global_time->getTic() : 0;
    //     }
    //     else
    //     {
    //         vel_x = global_time->getTic()? 10.0 / global_time->getTic() : 0;
    //     }
    // }
    // else if(keys[SDL_SCANCODE_LEFT])
    // {
    //     if(keys[SDL_SCANCODE_LSHIFT])
    //     {
    //         vel_x = global_time->getTic()? -20.0 / global_time->getTic() : 0;
    //     }
    //     else
    //     {
    //         vel_x = global_time->getTic()? -10.0 / global_time->getTic() : 0;
    //     }
    // }
    // if (keys[SDL_SCANCODE_SPACE])
    // {
    //     vel_y = global_time->getTic()? -10.0 / global_time->getTic() : 0;
    // }
    // game_objects->setEntityVelocityX(selfID, vel_x);
    // if(vel_y)
    // {
    //     game_objects->setEntityVelocityY(selfID, vel_y);
    // }
}


void handleDzone(GameObjects* game_objs)
{
    // Coord spawn_point = {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.2};
    // SDL_Rect dzone1 = { static_cast<int>(SCREEN_WIDTH * 0.25), static_cast<int>(SCREEN_HEIGHT * 0.7), static_cast<int>(SCREEN_WIDTH * 0.2), static_cast<int>(SCREEN_HEIGHT * 0.3)};
    // SDL_Rect dzone2 = { static_cast<int>(SCREEN_WIDTH * 0.55), static_cast<int>(SCREEN_HEIGHT * 0.7), static_cast<int>(SCREEN_WIDTH * 0.4), static_cast<int>(SCREEN_HEIGHT * 0.3)};
    // SDL_Rect dzone3 = { static_cast<int>(SCREEN_WIDTH * 0.95), static_cast<int>(SCREEN_HEIGHT * 0.7), static_cast<int>(SCREEN_WIDTH * 0.6), static_cast<int>(SCREEN_HEIGHT * 0.3)};
    // Rect* playerE = dynamic_cast<Rect*>(game_objs->getEntity(selfID));

    // DzoneHandler& dzHandler = DzoneHandler::getInstance();
    // dzHandler.addDeathZone(dzone1, spawn_point);
    // dzHandler.addDeathZone(dzone2, spawn_point);
    // dzHandler.addDeathZone(dzone3, spawn_point);
    // dzHandler.startCollisionChecking(playerE);
}