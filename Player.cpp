#include "Player.h"
#include <iostream>

//-----------------------------------------------------------------------------
Player_t::Player_t() : Graphic_t( OBJECT_TYPE::PLAYER )
{
   playerPosition = { ( float ) screenWidth / 2, ( float ) screenHeight / 2 };

   playerTexture         = LoadTexture( "spritesheets/wizard01.png" );
   ANIMATION_WALK_UP_Y   = static_cast<float>( playerTexture.height / 54 ) * 8;
   ANIMATION_WALK_LEFT_Y = static_cast<float>( playerTexture.height / 54 ) * 9;
   ANIMATION_WALK_DOWN_Y = static_cast<float>( playerTexture.height / 54 ) * 10;
   ANIMATION_WALK_RIGHT_Y =
       static_cast<float>( playerTexture.height / 54 ) * 11;

   frameRec = { 0.0f, 0.0f, ( float ) playerTexture.width / 13,
                static_cast<float>( playerTexture.height / 54 ) };

   currentFrame  = 0;
   framesCounter = 0;
   framesSpeed   = 8;   // Number of spritesheet frames shown by second
}

//-----------------------------------------------------------------------------
Player_t::Player_t( Vector2 pos ) : Graphic_t( OBJECT_TYPE::PLAYER )
{
   playerPosition = { pos.x, pos.y };

   playerTexture         = LoadTexture( "spritesheets/wizard01.png" );
   ANIMATION_WALK_UP_Y   = static_cast<float>( playerTexture.height / 54 ) * 8;
   ANIMATION_WALK_LEFT_Y = static_cast<float>( playerTexture.height / 54 ) * 9;
   ANIMATION_WALK_DOWN_Y = static_cast<float>( playerTexture.height / 54 ) * 10;
   ANIMATION_WALK_RIGHT_Y =
       static_cast<float>( playerTexture.height / 54 ) * 11;

   frameRec = { 0.0f, 0.0f, ( float ) playerTexture.width / 13,
                static_cast<float>( playerTexture.height / 54 ) };

   currentFrame  = 0;
   framesCounter = 0;
   framesSpeed   = 8;   // Number of spritesheet frames shown by second
}

//-----------------------------------------------------------------------------
void Player_t::goRight( float movement )
{
   playerPosition.x += movement;

   if ( framesCounter >= ( 60 / framesSpeed ) )
   {
      framesCounter = 0;
      currentFrame++;

      if ( currentFrame > 8 )
         currentFrame = 0;

      frameRec.x = ( float ) currentFrame * ( float ) playerTexture.width / 13;
      frameRec.y = ANIMATION_WALK_RIGHT_Y;
   }
}

//-----------------------------------------------------------------------------
void Player_t::goLeft( float movement )
{
   playerPosition.x -= movement;

   if ( framesCounter >= ( 60 / framesSpeed ) )
   {
      framesCounter = 0;
      currentFrame++;

      if ( currentFrame > 8 )
         currentFrame = 0;

      frameRec.x = ( float ) currentFrame * ( float ) playerTexture.width / 13;
      frameRec.y = ANIMATION_WALK_LEFT_Y;
   }
}

//-----------------------------------------------------------------------------
void Player_t::goUp( float movement )
{
   playerPosition.y -= movement;

   if ( framesCounter >= ( 60 / framesSpeed ) )
   {
      framesCounter = 0;
      currentFrame++;

      if ( currentFrame > 8 )
         currentFrame = 0;

      frameRec.x = ( float ) currentFrame * ( float ) playerTexture.width / 13;
      frameRec.y = ANIMATION_WALK_UP_Y;
   }
}

//-----------------------------------------------------------------------------
void Player_t::goDown( float movement )
{
   playerPosition.y += movement;

   if ( framesCounter >= ( 60 / framesSpeed ) )
   {
      framesCounter = 0;
      currentFrame++;

      if ( currentFrame > 8 )
         currentFrame = 0;

      frameRec.x = ( float ) currentFrame * ( float ) playerTexture.width / 13;
      frameRec.y = ANIMATION_WALK_DOWN_Y;
   }
}

//-----------------------------------------------------------------------------
void Player_t::draw()
{
   //DrawTextureRec( playerTexture, frameRec, playerPosition,
                   //WHITE );   // Draw part of the texture
   DrawTexturePro( playerTexture, frameRec, { playerPosition.x, playerPosition.y, 40, 40 }, { 0, 0 }, 0, WHITE );
}

//-----------------------------------------------------------------------------
void Player_t::handleMovement()
{
   ++framesCounter;
   if ( IsKeyDown( KEY_D ) )
   {
      goRight();
   }
   if ( IsKeyDown( KEY_A ) )
   {
      goLeft();
   }
   if ( IsKeyDown( KEY_W ) )
   {
      goUp();
   }
   if ( IsKeyDown( KEY_S ) )
   {
      goDown();
   }
}
