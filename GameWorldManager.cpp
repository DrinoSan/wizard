// System Headers
#include <algorithm>
#include <cassert>
#include <random>
#include <sstream>

#include "GameWorldManager.h"
#include "imgui.h"
#include "log.h"

//-----------------------------------------------------------------------------
GameWorldManager_t::GameWorldManager_t() : world{ std::make_unique<World_t>() }
{
   world->preapreWorld();
   // Camera setup
   camera.offset   = ( Vector2 ){ ( float ) screenWidth / 2.0f,
                                  ( float ) screenHeight / 2.0f };
   camera.rotation = 0.0f;
   camera.zoom     = 1.0f;
}

//-----------------------------------------------------------------------------
GameWorldManager_t::~GameWorldManager_t() = default;

//-----------------------------------------------------------------------------
void GameWorldManager_t::spawnPlayer()
{
   auto player  = std::make_unique<Player_t>( *world );
   player->type = ENTITY_TYPE::PLAYER;
   // playerEntity = player.get();
   enities.push_back( std::move( player ) );
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::draw() const
{
   BeginMode2D( camera );
   // @TODO: add layers which should be drawn to have a way to know whats in
   // the background and so on
   world->draw();
   for ( const auto& e : enities )
   {
      e->draw();
   }
   EndMode2D();

   if ( isLevelCleared() )
   {
      int32_t screenW = GetScreenWidth();
      int32_t screenH = GetScreenHeight();
      if ( IsWindowFullscreen() )
      {
         int32_t monitor = GetCurrentMonitor();
         screenW         = GetMonitorWidth( monitor );
         screenH         = GetMonitorHeight( monitor );
      }

      // Dark overlay
      DrawRectangle( 0, 0, screenW, screenH, ( Color ){ 0, 0, 0, 128 } );

      const char* msg      = "Level Passed!";
      Vector2     textSize = MeasureTextEx( GetFontDefault(), msg, 48, 1 );
      DrawText( msg, ( screenW - textSize.x ) / 2, screenH / 2 - 100, 48,
                LIME );

      int32_t            countdown = ( int32_t ) ceilf( levelBreakTimer );
      std::ostringstream os;
      os << "Next level in: " << countdown;
      std::string timerText = os.str();
      textSize = MeasureTextEx( GetFontDefault(), timerText.c_str(), 32, 1 );
      DrawText( timerText.c_str(), ( screenW - textSize.x ) / 2, screenH / 2,
                32, WHITE );

      os.clear();
      os << "Level " << getCurrentLevel() << " Complete";
      std::string levelText = os.str();
      textSize = MeasureTextEx( GetFontDefault(), levelText.c_str(), 24, 1 );
      DrawText( levelText.c_str(), ( screenW - textSize.x ) / 2,
                screenH / 2 + 60, 24, GRAY );
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::updateEntities( float dt )
{
   for ( auto& entity : enities )
   {
      entity->update( dt );
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::applyMovement( float dt )
{
   for ( auto& entity : enities )
   {
      // Skip purely static objects I mean why would they even move
      if ( entity->type == ENTITY_TYPE::STATIC )
      {
         continue;
      }

      // Only move if the entity actually wants to move this frame
      // (collision system has already zeroed velocity axes if blocked)
      if ( entity->velocity.x != 0.0f || entity->velocity.y != 0.0f )
      {
         Vector2 motion = Vector2Scale( entity->velocity, dt );

         entity->playerPosition = Vector2Add( entity->playerPosition, motion );

         // Sync hitbox
         entity->hitbox.x = entity->playerPosition.x + entity->hitboxOffset.x;
         entity->hitbox.y = entity->playerPosition.y + entity->hitboxOffset.y;
      }

      // Needed for loggin
      entity->lastVelocity = entity->velocity;
      // IMPORTANT Without this, entities keep sliding forever
      entity->velocity = { 0.0f, 0.0f };
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::updateCamera()
{
   // @TODO: cache player pointer so i dont have to iterate always...
   // Could lead to issues if i add multiplayer on single machine
   /*
   if ( playerEntity != nullptr )
   {
      camera.target =
          playerEntity->playerPosition;
   }
   */

   for ( auto& entity : enities )
   {
      if ( entity->type == ENTITY_TYPE::PLAYER )
      {
         camera.target = entity->playerPosition;
         break;
      }
   }

   camera.offset = ( Vector2 ){ ( float ) GetScreenWidth() / 2.0f,
                                ( float ) GetScreenHeight() / 2.0f };

   float baseZoom = ( float ) GetScreenHeight() /
                    WORLD_HEIGHT;   // WORLD_HEIGTH is number of tiles * size of
                                    // tiles check globals.h

   static float zoomModifier = 1.0f;

   if ( IsKeyDown( KEY_UP ) )
   {
      zoomModifier += 0.5f * GetFrameTime();   // zoom in gradually
   }
   if ( IsKeyDown( KEY_DOWN ) )
   {
      zoomModifier -= 0.5f * GetFrameTime();   // zoom out gradually
   }

   // Lightning zoom
   if ( IsKeyDown( KEY_LEFT_SHIFT ) || IsKeyDown( KEY_RIGHT_SHIFT ) )
   {
      zoomModifier += IsKeyDown( KEY_UP ) ? 1.0f * GetFrameTime() : 0;
      zoomModifier -= IsKeyDown( KEY_DOWN ) ? 1.0f * GetFrameTime() : 0;
   }

   zoomModifier = Clamp( zoomModifier, 0.4f, 3.5f );   // min/max allowed zoom

   camera.zoom = baseZoom * zoomModifier;

   camera.zoom = fmax( camera.zoom, 0.5f );
   camera.zoom = fmin( camera.zoom, 4.0f );
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::update( float dt )
{
   handleInputs();
   executeNpcMovements();
   executeNpcAttacks();
   updateEntities( dt );
   handleCollisions( dt );
   applyMovement( dt );

   if ( !levelCleared )
   {
      // @TODO: Check if this could be done with our event system
      livingEnemies = 0;
      for ( const auto& entity : enities )
      {
         if ( entity->type == ENTITY_TYPE::ENEMY && entity->lifePoints >= 0 )
         {
            ++livingEnemies;
         }
      }

      // Level seems cleared
      if ( livingEnemies == 0 )
      {
         levelCleared    = true;
         levelBreakTimer = LEVEL_BREAK_DURATION;
#ifdef DEBUG
         TraceLog( LOG_INFO, "Level %d cleared!", currentLevel );
#endif
      }
   }
   else
   {
      levelBreakTimer -= dt;
      if ( levelBreakTimer <= 0.0f )
      {
         ++currentLevel;
         levelCleared = false;
         spawnLevelEnemies();
      }
   }

   updateCamera();
   lateUpdate( dt );
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::lateUpdate( float dt )
{
   enities.erase(
       std::remove_if( enities.begin(), enities.end(),
                       []( const auto& entity )
                       {
                          if ( entity->type == ENTITY_TYPE::PLAYER )
                             return false;
                          return entity->state == ENTITY_STATE::DEAD ||
                                 ( entity->type == ENTITY_TYPE::ENEMY &&
                                   entity->lifePoints <= 0 );
                       } ),
       enities.end() );
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::onEvent( Event_t& e )
{
   // EventDispatcher_t dispatcher( e );
   // dispatcher.Dispatch<KeyPressedEvent_t>( BIND_EVENT_FN( onKeyEvent ) );

   for ( auto& obj : enities )
   {
      obj->onEvent( e );
      // If a event is handled the other objects do  not need to check it
      // Example: PlayerMovement via KeyPressedEvent which is only for the
      // player
      if ( e.Handled )
      {
         break;
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::handleCollisions( float dt )
{
   updateCollisionGrid();

   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::STATIC )
      {
         continue;
      }

      // First i want to do the static checks -> object against world map
      resolveCollisionEntityStatic( obj.get(), dt );
      resolveAttackCollisionsWithEntities();

      // Second i want to check all dynamic objects

      // Check if Obj is overlapping with the map
      // Get all tiles next to the obj
      // check if the textures overlap
      // kick it back
      if ( obj->type == ENTITY_TYPE::PLAYER )
      {
         // world->getClosestTiles( static_cast<Player_t*>( obj.get() )->hitbox
         // );
      }
      else if ( obj->type == ENTITY_TYPE::NPC )
      {
         // @TODO: Implement  me
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::resolveAttackCollisionsWithEntities()
{
   for ( auto& [ cellIndex, cell ] : grid.collisionGrid )
   {
      if ( cell.attacks.empty() || cell.entities.empty() )
      {
         continue;
      }

      for ( auto& [ sourceEntity, attack ] : cell.attacks )
      {
         if ( !attack->active )
         {
            continue;
         }

         for ( auto& targetEntity : cell.entities )
         {
            // Need to check again for attacks which can only affect one enemy
            if ( !attack->active )
            {
               continue;
            }

            if ( targetEntity == sourceEntity )
            {
               continue;
            }

            if ( targetEntity->type != ENTITY_TYPE::ENEMY &&
                 targetEntity->type != ENTITY_TYPE::PLAYER )
            {
               continue;
            }

            if ( sourceEntity->type == ENTITY_TYPE::ENEMY &&
                 targetEntity->type == ENTITY_TYPE::ENEMY )
            {
               continue;
            }

            if ( CheckCollisionRecs( attack->hitbox, targetEntity->hitbox ) )
            {
               targetEntity->lifePoints -=
                   sourceEntity->attackPower * attack->damageMultiplier;
               targetEntity->lifePoints = fmax( 0, targetEntity->lifePoints );

               // Checking if attack ends after first hit or if it can hit
               // multiple entites
               if ( attack->attribute == AttackAttribute::SINGLE )
               {
                  attack->active = false;
               }

#ifdef DEBUG
               std::cout << "Fireball hit "
                         << entityTypeToString( targetEntity->type )
                         << " HP: " << targetEntity->lifePoints << "\n";
#endif

               if ( targetEntity->lifePoints <= 0.0f )
               {
                  targetEntity->state = ENTITY_STATE::DEAD;
               }
            }
         }
      }
   }

   for ( auto& entity : enities )
   {
      entity->activeAttacks.erase(
          std::remove_if( entity->activeAttacks.begin(),
                          entity->activeAttacks.end(),
                          []( const Attack_t& a ) { return !a.active; } ),
          entity->activeAttacks.end() );
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::resolveCollisionEntityStatic( Entity_t* e, float dt )
{
   // How far the entity wants to move this frame
   Vector2 desiredMotion = Vector2Scale( e->velocity, dt );

   // Skip if not moving
   if ( desiredMotion.x == 0 && desiredMotion.y == 0 )
   {
      return;
   }

   Rectangle futureHitbox = e->hitbox;

   // Checking around x axis
   futureHitbox.x += desiredMotion.x;

   bool collisionX = false;
   for ( const auto& tile : world->worldMapTilesWithCollision )
   {
      if ( CheckCollisionRecs( futureHitbox, tile.tileDest ) )
      {
         collisionX = true;
         break;
      }
   }

   if ( collisionX )
   {
      e->velocity.x   = 0;
      desiredMotion.x = 0;
      futureHitbox.x  = e->hitbox.x;
   }

   // Checking around y axis
   futureHitbox.y += desiredMotion.y;

   bool collisionY = false;
   for ( const auto& tile : world->worldMapTilesWithCollision )
   {
      if ( CheckCollisionRecs( futureHitbox, tile.tileDest ) )
      {
         collisionY = true;
         break;
      }
   }

   if ( collisionY )
   {
      e->velocity.y   = 0;
      desiredMotion.y = 0;
      futureHitbox.y =
          e->hitbox
              .y;   // Not needed to set but i like if its the same as for X
   }

   // Optional: push-out to prevent getting "sucked" into walls
   // Only needed if floating point couses issues
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::prepareManager()
{
   for ( auto& obj : enities )
   {
      std::cout << "PrepareManager Entity type: "
                << entityTypeToString( obj->type ) << "\n";
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::executeNpcAttacks()
{
   Player_t* player = nullptr;
   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::PLAYER )
      {
         player = static_cast<Player_t*>( obj.get() );
      }
   }

   assert( player != nullptr && "Player needs to be initialized" );

   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::ENEMY )
      {
         auto enemy = static_cast<NpcEnemy_t*>( obj.get() );
         enemy->attack( *player );
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::executeNpcMovements()
{
   // @TODO seprate vectors for player and enemies
   Player_t* player = nullptr;
   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::PLAYER )
      {
         player = static_cast<Player_t*>( obj.get() );
      }
   }

   assert( player != nullptr && "Player needs to be initialized" );

   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::ENEMY )
      {
         auto enemy = static_cast<NpcEnemy_t*>( obj.get() );
         enemy->handleNpcMovement( *world, player );
      }
   }
}

//-----------------------------------------------------------------------------
// I think this is way more complicated than needed
void GameWorldManager_t::handleInputs()
{
   int  keyCode;
   bool keyHandeld{ false };

   // Need to poll for space for attacks
   // if ( IsKeyDown( KEY_SPACE ) )
   //{
   //   KeyPressedEvent_t event( KEY_SPACE );
   //   onEvent( event );
   //}

   for ( auto& key : keysDown )
   {
      if ( key == 0 )
      {
         continue;
      }

      if ( IsKeyDown( key ) )
      {
         KeyPressedEvent_t event( key );
         onEvent( event );
      }
      else
      {
         key = 0;
      }
   }

   // Working the key off
   while ( ( keyCode = GetKeyPressed() ) != 0 )
   {
      KeyPressedEvent_t event( keyCode );
      onEvent( event );
      if ( IsKeyDown( keyCode ) )
      {
         for ( auto& key : keysDown )
         {
            if ( key == keyCode )
            {
               // Key is already in pressedKeys we can skip
               keyHandeld = true;
               break;
            }
         }

         bool keyAdded{ false };
         if ( !keyHandeld )
         {
            for ( auto& key : keysDown )
            {
               if ( key == 0 )
               {
                  key      = keyCode;
                  keyAdded = true;
                  break;
               }
            }

            if ( keyAdded )
               continue;
         }
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::spawnEnemies( int32_t numEnemies_ )
{
   numEnemies = numEnemies_;
   for ( int32_t i = 0; i < numEnemies; ++i )
   {
      // INIT ENEMIES
      auto& enemy = enities.emplace_back( std::make_unique<NpcEnemy_t>(
          std::make_unique<A_StarStrategy_t>(),
          std::make_unique<MeleeAttackStrategy_t>() ) );

      static_cast<NpcEnemy_t*>( enemy.get() )
          ->registerOnEventCallback( [ this ]( Event_t& e )
                                     { this->onEvent( e ); } );
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::updateWorld()
{
   world->preapreWorld();
}

//-----------------------------------------------------------------------------
Player_t* GameWorldManager_t::getPlayer() const
{
   for ( auto& obj : enities )
   {
      if ( obj->type == ENTITY_TYPE::PLAYER )
      {
         return static_cast<Player_t*>( obj.get() );
      }
   }

   assert( false && "Player not initialized" );
   return nullptr;
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::imgui_debug() const
{
   auto* player = getPlayer();
   if ( ImGui::CollapsingHeader( "Player Info" ) )
   {
      ImGui::SeparatorText( "General:" );
      ImGui::BulletText( "State: %s",
                         entityStateToString( player->state ).c_str() );
      ImGui::BulletText( "Position: %.2f, %.2f", player->playerPosition.x,
                         player->playerPosition.y );
      ImGui::BulletText( "Velocity: %.2f, %.2f", player->lastVelocity.x,
                         player->lastVelocity.y );

      ImGui::SeparatorText( "Character:" );
      ImGui::BulletText( "LifePoints: %d", player->lifePoints );
      ImGui::BulletText( "ManaPoints: %d", player->manaPoints );
      ImGui::BulletText( "Stamina: %d", player->stamina );
      ImGui::BulletText( "Armor: %d", player->armor );
      ImGui::BulletText( "AttackRange: %d", player->attackRange );
      ImGui::BulletText( "AttackSpeed: %.2f", player->attackSpeed );
      ImGui::BulletText( "AttackPower: %d", player->attackPower );
   }
   if ( ImGui::CollapsingHeader( "World Info" ) )
   {
      ImGui::SeparatorText( "General:" );
      ImGui::BulletText( "RenderWidth: %d - RenderHeight: %d", GetRenderWidth(),
                         GetRenderHeight() );
      ImGui::BulletText( "GetScreenWidth: %d - GetScreenHeight: %d",
                         GetScreenWidth(), GetScreenHeight() );
      ImGui::SeparatorText( "Enemies:" );
      ImGui::BulletText( "Number of Enemies: %d", NUM_ENEMIES );
      ImGui::SeparatorText( "Level:" );
      ImGui::BulletText(
          "Level: %d | Enemies left: %d | Cleared: %s | Break: %.1f",
          currentLevel, livingEnemies, levelCleared ? "Yes" : "No",
          levelBreakTimer );
   }
   player->drawAttackDebugInfo();
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::updateCollisionGrid()
{
   grid.collisionGrid.clear();

   // Updating grid data
   for ( auto& entity : enities )
   {
      int32_t cellIndex = grid.getCellIndex( entity->playerPosition );
      grid.collisionGrid[ cellIndex ].entities.push_back( entity.get() );

      for ( auto& attack : entity->activeAttacks )
      {
         if ( attack.active )
         {
            int32_t attackCellIndex = grid.getCellIndex( attack.position );
            grid.collisionGrid[ attackCellIndex ].attacks.emplace_back(
                entity.get(), &attack );
         }
      }
   }
}

//-----------------------------------------------------------------------------
void GameWorldManager_t::spawnLevelEnemies()
{
   enities.erase( std::remove_if( enities.begin(), enities.end(),
                                  []( const auto& entity ) {
                                     return entity->type ==
                                                ENTITY_TYPE::ENEMY &&
                                            entity->lifePoints <= 0;
                                  } ),
                  enities.end() );

   // Calculate new enemies
   enemiesInCurrentLevel = 1;// 5 + 3 * ( currentLevel - 1 );

   float rangedRatio{};
   if ( currentLevel >= 4 )
      rangedRatio = 0.3f;
   if ( currentLevel >= 7 )
      rangedRatio = 0.5f;

   // rangedRatio = 1.0f;

   for ( int32_t i = 0; i < enemiesInCurrentLevel; ++i )
   {
      Vector2 spawnPos = getRandomFreeSpawnPosition();

      auto& enemy = enities.emplace_back( std::make_unique<NpcEnemy_t>(
          std::make_unique<A_StarStrategy_t>(),
          std::make_unique<MeleeAttackStrategy_t>() ) );

      auto* npcEnemy = static_cast<NpcEnemy_t*>( enemy.get() );
      npcEnemy->registerOnEventCallback( [ this ]( Event_t& e )
                                         { this->onEvent( e ); } );

      npcEnemy->playerPosition = spawnPos;
      npcEnemy->lifePoints     = 100 + ( currentLevel - 1 ) * 10;
      npcEnemy->attackPower    = 10 + currentLevel;
      npcEnemy->waypointThreshold = FIXED_TILE_SIZE / 3;

      // Raylib function GetRandomValue
      float roll = GetRandomValue( 0, 100 ) / 100.0f;
      if ( roll < rangedRatio )
      {
         npcEnemy->behaviour      = ENEMY_BEHAVIOUR::RANGE;
         npcEnemy->attackRange    = 200.0f;
         npcEnemy->attackStrategy = std::make_unique<RangeAttackStrategy_t>();
      }
      else
      {
         npcEnemy->behaviour   = ENEMY_BEHAVIOUR::MELEE;
         npcEnemy->attackRange = 30.0f;
      }
   }
}

//-----------------------------------------------------------------------------
Vector2 GameWorldManager_t::getRandomFreeSpawnPosition()
{
   std::vector<Vector2> freeTiles;
   for ( size_t i = 0; i < world->tileMapLayout.size(); ++i )
   {
      if ( world->tileMapLayout[ i ].second == TileType_t::NO_COLLISION )
      {
         float x =
             ( i % MAP_TILES_X ) * FIXED_TILE_SIZE + FIXED_TILE_SIZE / 2.0f;
         float y =
             ( i / MAP_TILES_X ) * FIXED_TILE_SIZE + FIXED_TILE_SIZE / 2.0f;
         freeTiles.push_back( { x, y } );
      }
   }

   if ( freeTiles.empty() )
      return { WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f };

   std::random_device              rd;
   std::mt19937                    gen( rd() );
   std::uniform_int_distribution<> dist( 0, freeTiles.size() - 1 );
   return freeTiles[ dist( gen ) ];
}
