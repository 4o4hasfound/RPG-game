# RPG Game
#### Video Demo:  https://youtu.be/wcIo7IRTxW4
## Description
This is a RPG game that is made in c++ using the SFML graphics library.  
You can run around controlling the character, kill enemies, with attacks and dashes.  
You can also explore the map with a variaty of different treasure chests hidden in it.  

## Control
Press "a" to move leftward  
Press "d" to move rightward  
Press "w" to jump  
Press/Hold left click to attack  
Press right click to dash(use skill)  

## Gameplay
The game uses velocity to update the position, and the velocity will decrease over time, so you will not immediate stop when you release a/d key. If you touch any enemy with your dash(skill), you will deal some damage to them, shock them(causing them to stop moving and attacking), and deal huge knockback. And if you happen to kill them using the dash(skill), the cooldown of it will be recharge immediatly.

## Design Architectures
### State
This game uses a stack based state system, is stacks multiple states on top of each other, and the main loop only updates the top most state. You can push and pop states as you wish, even the state the program current is in. This let add functionality like text popup or main menu without too much of a hassel. To add a custom state, all you have to do is to inherit from the State class, fill in all the virtual functions(onCreate, onDestroy, update, draw, reset, etc.), add in some of your own variables and logics, and there you have a custom state, and all is left is to push your state into the StateManager with pushState function inside StateManager. You can also use the popState and topState to access the states inside StateManager.

### Physics
The main part of the physics solver lies the PhysicsWorld class, responsible for all collision tests and gravitational effects. Within PhysicsWorld, all bodies are intelligently organized within an automatic growing grid, offering flexibility in map customization. Upon adding a body, collision resolution is handled, and will trigger the onCollide virtual function within the Body class. This allows for diverse interactions among entities such as characters and enemies, with customizable body types including Character, Enemy, Static, and Static_Uncollidable. Body class also store other necessary information, like AABB and bodyData(all the messy data in a character or in a enemy, like health, velocity, and other flags). All of which are inheritted to Character, Enemy, Area that trigger Text Popups, Area that trigger the end of the game, and much more. And in which, Character and Enemy were then inheritted to the character and enemy that appears on the screen, with each of it containing different behaviors.  Because the PhysicsWorld uses a grid system, the collision detection phase is roughly optimized, and won't add too much time overhead if you add more obstacles. You can also query objects, using AABB, Line, or Circle, and you can also specify the bodyType you want to query using the template argument. It will then return a vector of Body* for you to work with.

### Animation
All of the animations in this game is managed using AnimationManager, which contains a variaty of things. First of all, it contains a unordered_map with AnimationType as key and Animation as the value. AnimationType an enum containing many animation type, such as Idle, Walk, Attack, Hit, Death and much more. Animation is a class to load spritesheet and output a frame based on the progress time. Every time it updates, it add the delta time to its progress, which it will later use to determine the current frame(By dividing the progress by the frameDuration). It can also set a start and end frame, it can even load spritesheets of different directions, like UpDown and LeftRight. All of those will be passed in through the constructor. And back to the AnimationManager, it provides a versatile way of combining animation with the program itself. You can set a callback function for each frame, and it will be called everytime a animation has finished playing. You can also set a per frame callback, with the progress time and total duration as the parameter. And beside all of these, you can also set a keyframe callback. Keyframe callback makes animation controlling become very easy. You can easily set the attack frames based on the animation where the attacking will only happened at one or two specific frame in the animation. Finally you can set a update function, which will be called every frame. It will return a animationType, and if it is different from the current animationType, the current animation will be reset and the new one will start to run. And if the two animationType are the same, then nothing will be done. This way transitioning between animations require not more than a few lines of code.

## ScreenShots
![image](https://github.com/4o4hasfound/RPG-game/assets/138118407/df8b5836-976b-4910-a8df-c7617f2dc1df)
![image](https://github.com/4o4hasfound/RPG-game/assets/138118407/21e7c89f-4763-463a-aa50-1669027f9376)
![image](https://github.com/4o4hasfound/RPG-game/assets/138118407/760441db-cb95-47e8-80cd-dab87406213c)
![image](https://github.com/4o4hasfound/RPG-game/assets/138118407/37e46903-a48a-4b3a-91ac-f7a0a8b6d0f2)
![image](https://github.com/4o4hasfound/RPG-game/assets/138118407/14b86147-d77f-444c-a267-4b5a0a298db5)