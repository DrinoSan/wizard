#include <iostream>

#include "NpcEnemy.h"
#include "World.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
NpcEnemy_t::NpcEnemy_t( std::unique_ptr<PathFindingStrategy_t> pathFinding,
                        std::unique_ptr<AttackStrategy_t>      attackStrategy_ )
    : path{ std::move( pathFinding ) },
      attackStrategy{ std::move( attackStrategy_ ) }
{
   attackTextures[ static_cast<std::size_t>( AttackType::LIGHTNING ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::FIRE ) ] = LoadTexture(
       "resources/attackSprites/magicAttackSprites/10FireBall/Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::WATER ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::EARTH ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );

   activeAttacks.reserve( 32 );

   playerPosition = { ( float ) screenWidth / 2 + 20,
                      ( float ) screenHeight / 2 + 20 };

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

   attackStrategy = std::make_unique<MeleeAttackStrategy_t>();

   attackTextures[ static_cast<std::size_t>( AttackType::LIGHTNING ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::FIRE ) ] = LoadTexture(
       "resources/attackSprites/magicAttackSprites/10FireBall/Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::WATER ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );
   attackTextures[ static_cast<std::size_t>( AttackType::EARTH ) ] =
       LoadTexture( "resources/attackSprites/magicAttackSprites/10FireBall/"
                    "Fire-ball.png" );

   activeAttacks.reserve( 32 );

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
   // velocity.x += movement;

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
   // velocity.x -= movement;

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
   // velocity.y -= movement;

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
   // velocity.y += movement;

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
void NpcEnemy_t::updateAttacks( float dt )
{
   for ( auto& atk : activeAttacks )
   {
      if ( atk.active )
      {
         atk.position.x += atk.velocity.x * GetFrameTime();
         atk.position.y += atk.velocity.y * GetFrameTime();
         atk.hitbox.x = atk.position.x;
         atk.hitbox.y = atk.position.y;
         atk.timer += GetFrameTime();

         if ( atk.timer >= 0.1f )
         {   // 10 FPS anim
            atk.currentFrame = ( atk.currentFrame + 1 ) % atk.maxFrames;
            atk.sourceRec.x  = atk.currentFrame * 72;
            atk.sourceRec.y  = 0;
            atk.timer        = 0;
         }

         // Deactivate off-screen or on-hit
         if ( atk.position.x > GetRenderWidth() + 100 ||
              atk.position.x < 0 - 100 ||
              atk.position.y > GetRenderHeight() + 100 ||
              atk.position.y < 0 - 100 )
         {
            atk.active = false;
         }
      }
   }
}
//-----------------------------------------------------------------------------
void NpcEnemy_t::draw()
{
   // DrawTextureRec( playerTexture, frameRec, playerPosition,
   // WHITE );   // Draw part of the texture
   Color tint = WHITE;
   if ( behaviour == ENEMY_BEHAVIOUR::RANGE )
      tint = ORANGE;

   DrawTexturePro( playerTexture, frameRec,
                   { playerPosition.x, playerPosition.y, 40, 40 }, { 0, 0 }, 0,
                   tint );

   for ( const auto& atk : activeAttacks )
   {
      if ( atk.active )
      {
         DrawTexturePro( getAttackTexture( atk.type ), atk.sourceRec,
                         { atk.position.x, atk.position.y, 72.0f, 72.0f },
                         { 36.0f, 48.0f }, atk.rotation, WHITE );
#ifdef DEBUG
         DrawRectangleLines( atk.position.x, atk.position.y, 72, 72, RED );
#endif
      }
   }

#ifdef DEBUG
   DrawRectangleLines( playerPosition.x + 10, playerPosition.y, 20, 40, RED );

   // Draw direction
   // This is not drwan because velocity is 0 at this point, i reset velocity in
   // updat
   DrawLine( playerPosition.x, playerPosition.y,
             playerPosition.x + velocity.x * 50,
             playerPosition.y + velocity.y * 50, RED );

#endif
   drawLifeBar();
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
   return false;
}

//-----------------------------------------------------------------------------
bool NpcEnemy_t::handleNpcMovement( World_t& world, Player_t* player )
{
   ++framesCounter;

   // First we check if the attackDistance to the player is reached if yes we
   // attack otherwise we ask pathStrategy for the next path
   if ( lifePoints <= 0 )
   {
      state = ENTITY_STATE::DEAD;
      return false;
   }

   state = ENTITY_STATE::ACTIVE;

   // Getting vector from Npc to Player
   Vector2 toPlayer{ player->playerPosition.x - playerPosition.x,
                     player->playerPosition.y - playerPosition.y };
   float   distToPlayer = Vector2Length( toPlayer );

   bool boost{ false };
   bool shouldAttack{ false };

   if ( distToPlayer <= attackRange )
   {
      shouldAttack = true;
   }

   if ( distToPlayer >= 150 )
   {
      boost = true;
   }

   lastPlayerDist = distToPlayer;

   if ( shouldAttack )
   {
      return true;
   }

   repathTimer += GetFrameTime();
   bool repathNow =
       pathIndices.empty() ||
       ( pathCursor >= static_cast<int32_t>( pathIndices.size() ) ) ||
       ( repathTimer >= repathInterval );

   if ( repathNow )
   {
      // Compute fresh path (overwrites pathIndices, resets cursor=1)
      pathFindingStrategy( world, *player );
   }

   velocity = { 0.0f, 0.0f };   // Default idle
   if ( !pathIndices.empty() &&
        pathCursor < static_cast<int32_t>( pathIndices.size() ) )
   {
      // Advance through waypoints
      bool advanced = true;
      while ( advanced &&
              pathCursor < static_cast<int32_t>( pathIndices.size() ) )
      {
         int32_t nextIdx = pathIndices[ pathCursor ];
         Vector2 target  = { world.worldMap[ nextIdx ].tileDest.x +
                                 world.worldMap[ nextIdx ].tileDest.width * 0.5f,
                             world.worldMap[ nextIdx ].tileDest.y +
                                 world.worldMap[ nextIdx ].tileDest.height *
                                     0.5f };
         Vector2 dirVec  = Vector2Subtract( target, playerPosition );
         float   dist    = Vector2Length( dirVec );

         if ( dist <= waypointThreshold )
         {
            pathCursor++;
         }
         else
         {
            advanced = false;
            if ( dist > 0.001f )
            {
               dirVec      = Vector2Normalize( dirVec );
               float speed = PLAYER_MOVEMENT_SPEED - 0.5f;
               if ( boost )
               {
                  speed *= 1.2f;
               }

               velocity = Vector2Scale( dirVec, speed );
            }
         }
      }

      if ( pathCursor >= static_cast<int32_t>( pathIndices.size() ) )
         needsRepath = true;
   }

   // Update sprite animation based on velocity
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

   return true;
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::update( float dt )
{
   playerPosition.x += velocity.x;
   playerPosition.y += velocity.y;

   // Need to update the hitbox on each update
   hitbox = { playerPosition.x + 10, playerPosition.y, 20, 40 };

   updateAttacks( dt );
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::registerOnEventCallback(
    std::function<void( Event_t& )> callback )
{
   onEventCallback = std::move( callback );
}
