#include <iostream>

#include "Grid.h"
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

   if ( spawnDelayTimer > 0 )
   {
      tint = BLUE;
   }

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
bool NpcEnemy_t::handleNpcMovement( World_t& world, Player_t* player,
                                    Grid_t& grid )
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

   if ( behaviour == ENEMY_BEHAVIOUR::MELEE )
   {
      boost = false;
      if ( distToPlayer <= 30.0f )
      {
         // Attack possible
         return true;
      }

      if ( distToPlayer >= 150 )
      {
         boost = true;
      }
   }
   else if ( behaviour == ENEMY_BEHAVIOUR::RANGE )
   {
      float desiredDist{ 200.0f };
      if ( distToPlayer <= desiredDist )
      {
         // Attack possible
         return true;
      }
   }

   // Recompute path if needed
   if ( pathIndices.empty() || pathCursor >= pathIndices.size() ||
        Vector2Distance( playerPosition, player->playerPosition ) > 100.0f )
   {
      pathFindingStrategy( world, *player );
      pathCursor = 0;
   }

   // Get current target waypoint
   Vector2 targetWaypoint = player->playerPosition;   // fallback

   size_t nextIndex = pathCursor + 1;
   if ( nextIndex < pathIndices.size() )
   {
      int32_t tileIdx  = pathIndices[ nextIndex ];
      targetWaypoint.x = world.worldMap[ tileIdx ].tileDest.x +
                         world.worldMap[ tileIdx ].tileDest.width / 2.0f;
      targetWaypoint.y = world.worldMap[ tileIdx ].tileDest.y +
                         world.worldMap[ tileIdx ].tileDest.height / 2.0f;
   }

   size_t currentIndex = pathCursor;
   if ( currentIndex < pathIndices.size() )
   {
      int32_t currentTileIdx  = pathIndices[ currentIndex ];
      Vector2 currentWaypoint = {
          world.worldMap[ currentTileIdx ].tileDest.x +
              world.worldMap[ currentTileIdx ].tileDest.width / 2.0f,
          world.worldMap[ currentTileIdx ].tileDest.y +
              world.worldMap[ currentTileIdx ].tileDest.height / 2.0f };

      float distToCurrent = Vector2Distance( playerPosition, currentWaypoint );
      if ( distToCurrent < 16.0f )
      {
         pathCursor++;   // Now heading to nextIndex + 1
      }
   }

   // === STEERING ===
   Vector2 desired       = Vector2Subtract( targetWaypoint, playerPosition );
   float   desiredLength = Vector2Length( desired );

   if ( desiredLength > 0.001f )   // Avoid division by zero
   {
      desired = Vector2Normalize( desired );
      desired = Vector2Scale( desired, maxSpeed );
   }
   else
   {
      desired = { 0, 0 };   // Already at waypoint
   }

   Vector2 pathForce = Vector2Subtract( desired, velocity );
   pathForce = Vector2ClampMagnitude( pathForce, maxSteeringForce * 2.0f );

   Vector2 separation = calculateSeparationForce( grid );

   Vector2 totalForce = Vector2Add( pathForce, separation );

   velocity = Vector2Add( velocity, totalForce );
   velocity = Vector2ClampMagnitude( velocity, maxSpeed );

   if ( boost )
   {
      velocity = Vector2Scale( velocity, 2.0f );
      velocity = Vector2ClampMagnitude(
          velocity, maxSpeed * 2.0f );   // Cap boosted speed
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
   playerPosition.x += velocity.x * dt;
   playerPosition.y += velocity.y * dt;

   // Need to update the hitbox on each update
   hitbox = { playerPosition.x + 10, playerPosition.y, 20, 40 };
   // draw();

   updateAttacks( dt );

   // velocity.x = 0;
   // velocity.y = 0;
}

//-----------------------------------------------------------------------------
void NpcEnemy_t::registerOnEventCallback(
    std::function<void( Event_t& )> callback )
{
   onEventCallback = std::move( callback );
}

//-----------------------------------------------------------------------------
Vector2 NpcEnemy_t::calculateSeekForce( Vector2& target )
{
   Vector2 desired =
       Vector2Normalize( Vector2Subtract( target, playerPosition ) );
   desired       = Vector2Scale( desired, maxSpeed );
   Vector2 steer = Vector2Subtract( desired, velocity );
   return Vector2ClampMagnitude( steer, maxSteeringForce );
}

//-----------------------------------------------------------------------------
Vector2 NpcEnemy_t::calculateSeparationForce( Grid_t& grid )
{
   Vector2 separation    = { 0, 0 };
   int32_t neighborCount = 0;

   // Use grid to find nearby enemies (fast!)
   int32_t cellIndex      = grid.getCellIndex( playerPosition );
   auto    checkNeighbors = [ & ]( int32_t idx )
   {
      auto it = grid.collisionGrid.find( idx );
      if ( it == grid.collisionGrid.end() )
         return;

      for ( auto* other : it->second.entities )
      {
         if ( other == this || other->type != ENTITY_TYPE::ENEMY )
            continue;

         float dist = Vector2Distance( playerPosition, other->playerPosition );
         if ( dist < SEPARATION_RADIUS && dist > 0 )
         {
            Vector2 diff =
                Vector2Subtract( playerPosition, other->playerPosition );
            diff = Vector2Normalize( diff );
            diff =
                Vector2Scale( diff, 1.0f / dist );   // Stronger push if closer
            separation = Vector2Add( separation, diff );
            neighborCount++;
         }
      }
   };

   checkNeighbors( cellIndex );

   if ( neighborCount > 0 )
   {
      separation = Vector2Scale( separation, 1.0f / neighborCount );
      separation = Vector2ClampMagnitude( separation, maxSteeringForce );
   }

   return Vector2Scale( separation, SEPARATION_WEIGHT );
}
