#include "Player.h"
#include "events/KeyEvent.h"
#include "raymath.h"
#include <iostream>

//-----------------------------------------------------------------------------
Player_t::Player_t() : Player_t( {  ( float ) screenWidth / 2 + 30,
                      ( float ) screenHeight / 2 + 30  } )
{}

//-----------------------------------------------------------------------------
Player_t::Player_t( Vector2 pos )
{
   playerPosition = { pos.x, pos.y };

   playerTexture         = LoadTexture( "spritesheets/wizard01.png" );
   ANIMATION_WALK_UP_Y   = static_cast<float>( playerTexture.height / 54 ) * 8;
   ANIMATION_WALK_LEFT_Y = static_cast<float>( playerTexture.height / 54 ) * 9;
   ANIMATION_WALK_DOWN_Y = static_cast<float>( playerTexture.height / 54 ) * 10;
   ANIMATION_WALK_RIGHT_Y =
       static_cast<float>( playerTexture.height / 54 ) * 11;

   ANIMATION_IDLE_UP_Y    = static_cast<float>( playerTexture.height / 54 ) * 0;
   ANIMATION_IDLE_LEFT_Y  = static_cast<float>( playerTexture.height / 54 ) * 1;
   ANIMATION_IDLE_DOWN_Y  = static_cast<float>( playerTexture.height / 54 ) * 2;
   ANIMATION_IDLE_RIGHT_Y = static_cast<float>( playerTexture.height / 54 ) * 3;

   frameRec = { 0.0f, 0.0f, ( float ) playerTexture.width / 13,
                static_cast<float>( playerTexture.height / 54 ) };

   frameWidth = playerTexture.width / 13;

   type   = ENTITY_TYPE::PLAYER;
   hitbox = { playerPosition.x + 10, playerPosition.y, 20, 40 };
}

//-----------------------------------------------------------------------------
void Player_t::goRight( float movement )
{
   // playerPosition.x += movement;
   velocity.x += movement;

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
   // playerPosition.x -= movement;
   velocity.x -= movement;

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
   // playerPosition.y -= movement;
   velocity.y -= movement;

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
   // playerPosition.y += movement;
   velocity.y += movement;

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
void Player_t::onEvent( Event_t& e )
{
   EventDispatcher_t dispatcher( e );
   dispatcher.Dispatch<KeyPressedEvent_t>(
       BIND_EVENT_FN( Player_t::handleKeyEvent ) );
}

//-----------------------------------------------------------------------------
bool Player_t::handleKeyEvent( KeyPressedEvent_t& e )
{
   std::cout << "[PLAYER] : Currently just logging [KeyPressedEvent] : "
             << e.ToString();
   return false;
}

//-----------------------------------------------------------------------------
bool Player_t::handleMovement( float dt )
{
   Vector2 input = { 0 };
   if ( IsKeyDown( KEY_D ) )
      input.x += 1;
   if ( IsKeyDown( KEY_A ) )
      input.x -= 1;
   if ( IsKeyDown( KEY_S ) )
      input.y += 1;
   if ( IsKeyDown( KEY_W ) )
      input.y -= 1;

   if ( Vector2Length( input ) > 0.1f )
   {
      input = Vector2Normalize( input );
      playerPosition.x += input.x * SPEED * dt;
      playerPosition.y += input.y * SPEED * dt;

      // Animation (frame-independent!)
      animTimer += dt;
      if ( animTimer >= 1.0f / framesSpeed )
      {
         animTimer    = 0;
         currentFrame = ( currentFrame + 1 ) % 9;
         frameRec.x   = currentFrame * frameWidth;
      }

      // Set correct row
      if ( fabsf( input.x ) > fabsf( input.y ) )
         frameRec.y =
             input.x > 0 ? ANIMATION_WALK_RIGHT_Y : ANIMATION_WALK_LEFT_Y;
      else
         frameRec.y = input.y > 0 ? ANIMATION_WALK_DOWN_Y : ANIMATION_WALK_UP_Y;
   }
   else
   {
      // Animation (frame-independent!)
      animTimer += dt;
      if ( animTimer >= 1.0f / framesSpeed )
      {
         animTimer    = 0;
         currentFrame = ( currentFrame + 1 ) % 7;
         frameRec.x   = currentFrame * frameWidth;
      }
      // Idle
      // currentFrame = 0;
      // frameRec.x   = 0;
      frameRec.y = ANIMATION_IDLE_DOWN_Y;
   }

   // Update hitbox
   hitbox.x = playerPosition.x + 10;
   hitbox.y = playerPosition.y;

   return true;
}

//-----------------------------------------------------------------------------
void Player_t::updateAnimation( float dt )
{
   // Only animate if moving
   if ( velocity.x != 0.0f || velocity.y != 0.0f )
   {
      animTimer += dt;

      // How long each frame should be shown
      float frameDuration = 1.0f / framesSpeed;

      // Advance frame when time is up
      if ( animTimer >= frameDuration )
      {
         animTimer -= frameDuration;   // subtract = smoother than reset
         currentFrame = ( currentFrame + 1 ) % 9;   // 9 frames per walk cycle
         frameRec.x   = ( float ) currentFrame * frameWidth;
      }

      if ( fabsf( velocity.x ) > fabsf( velocity.y ) )
      {
         frameRec.y = ( velocity.x > 0.0f ) ? ANIMATION_WALK_RIGHT_Y
                                            : ANIMATION_WALK_LEFT_Y;
      }
      else
      {
         frameRec.y = ( velocity.y > 0.0f ) ? ANIMATION_WALK_DOWN_Y
                                            : ANIMATION_WALK_UP_Y;
      }

      // Remember direction for idle pose
      lastDirection = Vector2Normalize( velocity );
   }
   else
   {
      animTimer += dt;
      // How long each frame should be shown
      float frameDuration = 1.0f / framesSpeed;

      if ( animTimer >= frameDuration )
      {
         // 7 Is the amount of sprites i have for the current animation
         currentFrame = ( currentFrame + 1 ) % 7;
         frameRec.x   = ( float ) currentFrame * frameWidth;
         frameRec.y   = ANIMATION_IDLE_DOWN_Y;
         animTimer    -= frameDuration;
      }
   }
}

//-----------------------------------------------------------------------------
void Player_t::update( float dt )
{
   Vector2 input{ 0, 0 };
   if ( IsKeyDown( KEY_D ) )
      input.x += 1;
   if ( IsKeyDown( KEY_A ) )
      input.x -= 1;
   if ( IsKeyDown( KEY_S ) )
      input.y += 1;
   if ( IsKeyDown( KEY_W ) )
      input.y -= 1;

   if ( Vector2Length( input ) > 0.1f )
      velocity = Vector2Scale( Vector2Normalize( input ), SPEED );
   else
      velocity = { 0, 0 };

   updateAnimation( dt );
}
