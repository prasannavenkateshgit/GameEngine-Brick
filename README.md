File Structure: 

Main contains the main update loop and calls other functions to handle engine subsystems.

structs contains the application struct, which includes the SDL Renderer and SDL Window pointers.

init initializes the SDL components

input handles HID Input

draw has the functions to apply the window color, clear the renderer, and render the updated scene.

networking has the functions used by clients to handle P2P functionalities. This previously used to be the file which had client-server functions

message_server and message_publisher have the classes used by the server to establish a connection with the clients and publish the updates to other clients 

game has the individual implementations of each of our games. The game.cpp file in the base game engine only has the base function definitions with no logic and thus when the base game engine is compiled it simply creates the SDL window.

### Gravity (Using Gravity for Game Entities)
To apply gravity to the entities in your game, follow these steps:

1. Import `physics.hpp`.
2. Use the global variable `physics`.

3. Change the Gravity Value  
To change the gravity value, use the `setGravity(g)` method, where `g` is the value of gravity you want to set.  
**Example:** 
```cpp
physics->setGravity(9.8f);
```

4. Apply gravity effect  
To apply the gravity effect to your entities, use `applyGravity(e)`, where `e` is the Game Entity/object to which you want to add the gravity effect.  
**Example:**
```cpp
physics->applyGravity(e);
```

### Timeline
To use a timeline object create an object of `Timeline` either anchoring from any existing object or allowing it to rely on system time. You can also set the global pointer `global_time` to be used by other files across the game engine. Use the `getTic` function to get the current tic being used, and the `getTime` to get the elapsed time so far. Call the `pause` or `unpause` to pause or unpause the game respectively. 

### GameObjects
This is a thread-safe class that will contain all the objects defined by the server and the clients. Any object created should be added to the game objects instance by calling `addEntity`. To change any part of any entity we recommend using one of the `set` methods from this class. This would ensure that the changes are handled in a thread-safe manner. 

### Game server
The game server will contain the objects and function definitions that need to be handled by the server. All objects defined here also need to be added to the game objects instance of the server. A timeline object must also be assigned to the `global_time` pointer which will be the anchor for other clients. The server by default uses 5555 and 5556 ports for networking with the clients.

### Event Management
We have 4 Events:
- CollisionEvent
- InputEvent
- DisconnectionEvent
- DeathZoneEvent

We advise the game developers to use the Collision and InputEvent and provide custom callbacks to their functions which will be invoked internally by the `event_manager`.


## Running a game
```
make clean; make
```
### Server
```
./main server
```
### Clients
```
./main client localhost localhost 5557 # or any other port
```
