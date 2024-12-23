#include <game_server.hpp>

float MAX_Y_OFFSET = SCREEN_HEIGHT * 0.08;
float ANGLE = 0.0f, SPEED = 0.05f; 
std::map<std::string, bool> collided_map;
CollisionEvent* collision_event;

float tmp_y;
float tmp_x = 5;
int modifier = 1;
GameObjects* loadServerEntities(void)
{
    GameObjects* game_objects = new GameObjects();
    // game_objects->addEntity("platform1", new Rect(0, SCREEN_HEIGHT * 0.7, SCREEN_WIDTH*0.2, SCREEN_HEIGHT * 0.5, false, Color{0, 255, 0}));
    // game_objects->addEntity("platform2", new Rect(SCREEN_WIDTH*0.3, SCREEN_HEIGHT * 0.7, SCREEN_WIDTH*0.1, SCREEN_HEIGHT * 0.5, false, Color{0, 255, 255}));
    // game_objects->addEntity("platform3", new Rect(SCREEN_WIDTH*0.75, SCREEN_HEIGHT * 0.7, SCREEN_WIDTH*0.1, SCREEN_HEIGHT * 0.5, false, Color{255, 255, 0}));
    // game_objects->addEntity("platform4", new Rect(SCREEN_WIDTH * 1.2, SCREEN_HEIGHT * 0.6, LEVEL_WIDTH, SCREEN_HEIGHT * 0.8, false, Color{0, 0, 255}));

    // game_objects->addEntity("pattern1", new Rect(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.6, 60, 60, true, Color{255, 105, 180}, false));
    // game_objects->addEntity("pattern2", new Rect(SCREEN_WIDTH * 0.30, SCREEN_HEIGHT * 0.0, 60, 60, true, Color{15, 10, 200}, false));
    // game_objects->addEntity("pattern3", new Rect(SCREEN_WIDTH * 1.4, SCREEN_HEIGHT * 0.3, 60, 60, true, Color{200, 15, 80}, false));
    // game_objects->addEntity("pattern4", new Rect(SCREEN_WIDTH * 2.9, SCREEN_HEIGHT * 0.3, 60, 60, true, Color{200, 15, 80}, false));
    
    // game_objects->addEntity("mov_platform1", new Rect(SCREEN_WIDTH * 0.10, SCREEN_HEIGHT * 0.6, 60, 10, true, Color{255, 105, 180}, false));
    // game_objects->addEntity("mov_platform2", new Rect(SCREEN_WIDTH * 0.90, SCREEN_HEIGHT * 0.5, 60, 10, true, Color{255, 105, 180}, false));
    // game_objects->addEntity("mov_platform3", new Rect(SCREEN_WIDTH * 1.8, SCREEN_HEIGHT * 0.2, 60, 10, true, Color{255, 105, 180}, false));
    return game_objects;
}

void patternEntity(GameObjects& game_objects)
{
    // Entity* pattern = game_objects.getEntity("pattern1");
    // Entity* platform = game_objects.getEntity("platform3");
    // Entity* pattern1 = game_objects.getEntity("pattern2");
    // Entity* pattern2 = game_objects.getEntity("pattern3");
    // Entity* pattern3 = game_objects.getEntity("pattern4");
        
    // if(pattern->getY()<10)
    // {
    //     tmp_y = 2;
    // }
    // if(detectCollision(pattern,platform))
    // {
    //     tmp_y = -2;
    // }
    
    // game_objects.setEntityVelocityY("pattern1", global_time->getTic()? tmp_y /global_time->getTic() : 0);
    // pattern->move(global_time->getTic(), false);

    // game_objects.setEntityVelocityY("pattern2", global_time->getTic()? -1*(tmp_y /global_time->getTic()) : 0);
    // pattern1->move(global_time->getTic(), false);

    // game_objects.setEntityVelocityX("pattern3", tmp_x);
    // pattern2->move(global_time->getTic(), false);

    // game_objects.setEntityVelocityY("pattern4", global_time->getTic()? -1*(tmp_y /global_time->getTic()) : 0);
    // pattern3->move(global_time->getTic(), false);

    // Entity* mov_platform1 = game_objects.getEntity("mov_platform1");
    // Entity* mov_platform2 = game_objects.getEntity("mov_platform2");
    // Entity* mov_platform3 = game_objects.getEntity("mov_platform3");
    // float curr_x = mov_platform1->getX();
    //  if(curr_x < SCREEN_WIDTH*0.1)
    // {
    //     tmp_x = -1*tmp_x;
    // }
    // if(curr_x > SCREEN_WIDTH*0.8)
    // {
    //     tmp_x = -1*tmp_x;
    // }

    
    // game_objects.setEntityVelocityX("mov_platform1", tmp_x);
    // mov_platform1->move(global_time->getTic(), false);

    // game_objects.setEntityVelocityX("mov_platform2", -1*tmp_x);
    // mov_platform2->move(global_time->getTic(), false);

    // game_objects.setEntityVelocityY("mov_platform3", global_time->getTic()? -1*(tmp_y /global_time->getTic()) : 0);
    // mov_platform3->move(global_time->getTic(), false);

    // for(auto ent: game_objects.getEntities())
    // {
    //     if (ent.first.find("pattern") != 0 && ent.first.find("platform") !=0 && ent.first.find("mov") !=0  )
    //     {
    //         if(detectCollision(pattern,game_objects.getEntity(ent.first)) )
    //         {

    //             collision_event = new CollisionEvent(scoreModifier, &game_objects, "pattern1", ent.first, global_time->getTime(), EventPriority::HIGH);
    //             collision_event->setType("Collision");
    //             event_manager.registerEvent(collision_event);
    //             event_manager.raiseEvent(collision_event);
    //             event_manager.handleEvents();
    //             std::cout << "Detected Collision " << ent.first<< std::endl;
    //         }

    //         else if (detectCollision(pattern1,game_objects.getEntity(ent.first)) )
    //         {
    //             collision_event = new CollisionEvent(scoreModifier,&game_objects, "pattern2", ent.first, global_time->getTime(), EventPriority::HIGH);
    //             collision_event->setType("Collision");
    //             event_manager.registerEvent(collision_event);
    //             event_manager.raiseEvent(collision_event);
    //             event_manager.handleEvents();
    //             std::cout << "Detected Collision " << ent.first<< std::endl;
    //         }

    //         else if (detectCollision(pattern2,game_objects.getEntity(ent.first)) )
    //         {
    //             collision_event = new CollisionEvent(scoreModifier, &game_objects, "pattern3", ent.first, global_time->getTime(), EventPriority::HIGH);
    //             collision_event->setType("Collision");
    //             event_manager.registerEvent(collision_event);
    //             event_manager.raiseEvent(collision_event);
    //             event_manager.handleEvents();
    //             std::cout << "Detected Collision " << ent.first<< std::endl;
    //         }
    //     }
    // }
}

int scoreModifier(int score){
    modifier = score * (1+(score/100));
    return score;
}

int getModifier(){
    int range = 100 - 20 + 1;
    int num = rand() % range + 20;
    return num*modifier;
}