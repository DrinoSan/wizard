#pragma once

//-----------------------------------------------------------------------------
// Forward declaration
class NpcEnemy_t;
class Player_t;

//-----------------------------------------------------------------------------
class AttackStrategy_t
{
 public:
   AttackStrategy_t()          = default;
   virtual ~AttackStrategy_t() = default;

   virtual void attack( NpcEnemy_t& npc, Player_t& player ) const = 0;
};

//-----------------------------------------------------------------------------
class MeleeAttackStrategy_t : public AttackStrategy_t
{
 public:
   virtual ~MeleeAttackStrategy_t() {}

   void attack( NpcEnemy_t& npc, Player_t& player ) const override;
};

//-----------------------------------------------------------------------------
class RangeAttackStrategy_t : public AttackStrategy_t
{
 public:
   virtual ~RangeAttackStrategy_t() {}

   void attack( NpcEnemy_t& npc, Player_t& player ) const override;
};
