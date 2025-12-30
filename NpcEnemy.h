#pragma once
// System Headers
#include <cstdint>
#include <functional>
#include <vector>

// Raylib Headers
#include "raylib.h"
#include "raymath.h"

// Project Headers
#include "AttackStrategy.h"
#include "Entity.h"
#include "Globals.h"
#include "Graphic.h"
#include "PathFindingStrategy.h"
#include "Player.h"
#include "events/KeyEvent.h"

class Grid_t;

//-----------------------------------------------------------------------------
enum class ENEMY_BEHAVIOUR
{
   MELEE,
   RANGE,
};

//-----------------------------------------------------------------------------
class NpcEnemy_t : public Graphic_t, public Entity_t
{
 public:
   explicit NpcEnemy_t( std::unique_ptr<PathFindingStrategy_t> pathFinding,
                        std::unique_ptr<AttackStrategy_t> attackStrategy_ );
   explicit NpcEnemy_t( Vector2 pos );

   virtual ~NpcEnemy_t() = default;

   // Movements
   // Moves Player and texture rectangle
   void goRight( float movement = PLAYER_MOVEMENT_SPEED );
   void goLeft( float movement = PLAYER_MOVEMENT_SPEED );
   void goUp( float movement = PLAYER_MOVEMENT_SPEED );
   void goDown( float movement = PLAYER_MOVEMENT_SPEED );

   void draw() override;
   void updateAttacks( float dt );
   bool handleMovement( KeyPressedEvent_t& e );
   bool handleNpcMovement( World_t& world, Player_t* player, Grid_t& grid );
   void onEvent( Event_t& e ) override;
   void registerOnEventCallback( std::function<void( Event_t& )> callback );
   std::function<void( Event_t& )> onEventCallback;

   // PathFinding for npc
   void pathFindingStrategy( const class World_t& world,
                             const Player_t&      player ) const
   {
      path->findPath( const_cast<NpcEnemy_t&>( *this ), world, player );
   }

   // Method for A* group handling
   Vector2 calculateSeekForce( Vector2& target );
   Vector2 calculateSeparationForce( Grid_t& grid );

   void attack( Player_t& player ) const
   {
      attackStrategy->attack( const_cast<NpcEnemy_t&>( *this ), player );
   }

   // Cached path produced by the strategy as tile indices (world.worldMap
   // indexes)
   std::vector<int> pathIndices;
   int              pathCursor{ 0 };

   // Entity specifics
   void update( float dt ) override;

   std::unique_ptr<PathFindingStrategy_t> path;
   std::unique_ptr<AttackStrategy_t>      attackStrategy;

   // Atack specifics
   ENEMY_BEHAVIOUR behaviour = ENEMY_BEHAVIOUR::MELEE;
   float           attackCooldown{ 0.0f };     // Pause between attacks
   float           meleeDamageTimer{ 0.0f };   // Damage per second on contact

   const float MELEE_DAMAGE_RATE{ 1.0f };
   const float RANGED_FIRE_RATE{ 2.5f };

   // Steering behaviors
   Vector2 steeringForce    = { 0, 0 };
   float   maxSpeed         = 80.0f;
   float   maxSteeringForce = 10.0f;

   // Separation
   const float SEPARATION_RADIUS = 60.0f;
   const float SEPARATION_WEIGHT = 1.5f;
};
