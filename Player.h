#pragma once
#include <cstdint>

#include "Graphic.h"
#include "raylib.h"

class Player_t : public Graphic_t
{
 public:
   Player_t();
   Player_t( Vector2 pos );

   virtual ~Player_t() = default;

   // Movements
   // Moves Player and texture rectangle
   void goRight( float movement = 2.0f );
   void goLeft( float movement = 2.0f );
   void goUp( float movement = 2.0f );
   void goDown( float movement = 2.0f );

   // Moves player but no animation movement
   void moveRight( float movement = 2.0f );
   void moveLeft( float movement = 2.0f );
   void moveUp( float movement = 2.0f );
   void moveDown( float movement = 2.0f );

   int32_t   ANIMATION_WALK_UP_Y;
   int32_t   ANIMATION_WALK_LEFT_Y;
   int32_t   ANIMATION_WALK_DOWN_Y;
   int32_t   ANIMATION_WALK_RIGHT_Y;
   Rectangle frameRec;
   Texture2D playerTexture;
   Vector2   playerPosition;
   int       currentFrame  = 0;
   int       framesCounter = 0;
   int       framesSpeed = 8;   // Number of spritesheet frames shown by second

   void draw() override;
   void handleMovement() override;

   const int32_t screenWidth  = 800;
   const int32_t screenHeight = 450;
};
