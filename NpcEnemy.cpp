#include <iostream>

#include "NpcEnemy.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
NpcEnemy_t::NpcEnemy_t()
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

   type          = ENTITY_TYPE::ENEMY;
   hitbox        = { playerPosition.x + 10, playerPosition.y, 20, 40 };
   currentFrame  = 0;
   framesCounter = 0;
   framesSpeed   = 8;   // Number of spritesheet frames shown by second
}

//-----------------------------------------------------------------------------
NpcEnemy_t::NpcEnemy_t( Vector2 pos )
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

   type          = ENTITY_TYPE::ENEMY;
   hitbox        = { playerPosition.x + 10, playerPosition.y, 20, 40 };
   currentFrame  = 0;
   framesCounter = 0;
   framesSpeed   = 8;   // Number of spritesheet frames shown by second
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::goRight( float movement )
{
   // playerPosition.x += movement;
   //velocity.x += movement;

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
void NpcEnemy_t::goLeft( float movement )
{
   // playerPosition.x -= movement;
   //velocity.x -= movement;

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
void NpcEnemy_t::goUp( float movement )
{
   // playerPosition.y -= movement;
   //velocity.y -= movement;

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
void NpcEnemy_t::goDown( float movement )
{
   // playerPosition.y += movement;
   //velocity.y += movement;

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
void NpcEnemy_t::draw()
{
   // DrawTextureRec( playerTexture, frameRec, playerPosition,
   // WHITE );   // Draw part of the texture
   DrawTexturePro( playerTexture, frameRec,
                   { playerPosition.x, playerPosition.y, 40, 40 }, { 0, 0 }, 0,
                   WHITE );
#ifdef DEBUG
   DrawRectangleLines( playerPosition.x + 10, playerPosition.y, 20, 40, RED );
#endif
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::onEvent( Event_t& e )
{
   EventDispatcher_t dispatcher( e );
   dispatcher.Dispatch<KeyPressedEvent_t>(
       BIND_EVENT_FN( NpcEnemy_t::handleMovement ) );
}

//-----------------------------------------------------------------------------
bool NpcEnemy_t::handleMovement( KeyPressedEvent_t& e )
{
   if ( e.getKeyCode() == KEY_D )
   {
      goRight();
   }
   if ( e.getKeyCode() == KEY_A )
   {
      goLeft();
   }
   if ( e.getKeyCode() == KEY_W )
   {
      goUp();
   }
   if ( e.getKeyCode() == KEY_S )
   {
      goDown();
   }

   return true;
}

//-----------------------------------------------------------------------------
bool NpcEnemy_t::handleNpcMovement( Player_t* player )
{
   ++framesCounter;

   // Get Richtungsvektor
   // Get normilized vektor
   // scale it for velocity
   auto directionVec = Vector2Subtract( player->playerPosition, playerPosition );
   auto normalizedVec = Vector2Normalize( directionVec );

   velocity.x += normalizedVec.x * 1.5;
   velocity.y += normalizedVec.y * 1.5;

   if ( velocity.x > 0 )
   {
      goRight();
   }
   if ( velocity.x < 0 )
   {
      goLeft();
   }
   if ( velocity.y < 0 )
   {
      goUp();
   }
   if ( velocity.y > 0 )
   {
      goDown();
   }
   //switch ( distrib( gen ) )
   //{
   //case 0:
   //{
   //   goRight();
   //   break;
   //}
   //case 1:
   //{
   //   goLeft();
   //   break;
   //}
   //case 2:
   //{
   //   goUp();
   //   break;
   //}
   //case 3:
   //{
   //   goDown();
   //   break;
   //}
   //}

   return true;
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::update()
{
   playerPosition.x += velocity.x;
   playerPosition.y += velocity.y;

   // Need to update the hitbox on each update
   hitbox = { playerPosition.x + 10, playerPosition.y, 20, 40 };
   draw();

   // Important to reset otherwise we become buz lightyear
   velocity.x = 0;
   velocity.y = 0;
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::registerOnEventCallback(
    std::function<void( Event_t& )> callback )
{
   onEventCallback = std::move( callback );
}
