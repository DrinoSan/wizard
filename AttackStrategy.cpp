#include "AttackStrategy.h"
#include "NpcEnemy.h"
#include "Player.h"
#include "raylib.h"
#include "raymath.h"

//-----------------------------------------------------------------------------
void MeleeAttackStrategy_t::attack( NpcEnemy_t& npc, Player_t& player ) const
{
   // Getting vector from Npc to Player
   Vector2 toPlayer{ player.playerPosition.x - npc.playerPosition.x,
                     player.playerPosition.y - npc.playerPosition.y };
   float   distToPlayer = Vector2Length( toPlayer );

   if ( distToPlayer <= 30.0f )
   {
      npc.meleeDamageTimer += GetFrameTime();
      if ( npc.meleeDamageTimer < npc.MELEE_DAMAGE_RATE )
      {
         return;
      }

      // Because its a melee attack i can already here subtract the points of
      // the player
      npc.state = ENTITY_STATE::ATTACKING;
      player.lifePoints -= npc.attackPower;
      player.lifePoints = fmax( 0, player.lifePoints );

      npc.meleeDamageTimer = 0;
      TraceLog( LOG_INFO, "Melee hit! Player HP: %d", player.lifePoints );
   }
}

//-----------------------------------------------------------------------------
void RangeAttackStrategy_t::attack( NpcEnemy_t& npc, Player_t& player ) const
{
   float desiredDist{ 200.0f };
   // Getting vector from Npc to Player
   Vector2 toPlayer{ player.playerPosition.x - npc.playerPosition.x,
                     player.playerPosition.y - npc.playerPosition.y };
   float   distToPlayer = Vector2Length( toPlayer );

   if ( distToPlayer <= desiredDist )
   {
      // ATTACKKKK
      npc.attackCooldown -= GetFrameTime();
      if ( npc.attackCooldown <= 0.0f )
      {
         Vector2 attackDir      = Vector2Normalize( toPlayer );
         Vector2 attackVelocity = Vector2Scale( attackDir, 150.0f );

         Attack_t attack;
         attack.type     = AttackType::FIRE;
         attack.position = npc.playerPosition;
         attack.velocity = attackVelocity;
         attack.active   = true;
         attack.rotation =
             Vector2Angle( attackDir, { 1, 0 } ) * RAD2DEG *
             -1.0f;   // -1 noetig, da initial das sprite nach rechts zeigt
         attack.maxFrames = 8;
         attack.sourceRec = { 0, 0, 72, 72 };

         npc.activeAttacks.push_back( attack );
         npc.attackCooldown = npc.RANGED_FIRE_RATE;

         TraceLog( LOG_INFO, "Ranged enemy fired!" );
      }
   }
}
