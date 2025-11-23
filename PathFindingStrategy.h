#pragma once

//-----------------------------------------------------------------------------
// Forward declaration
class NpcEnemy_t;
class World_t;
class Player_t;

//-----------------------------------------------------------------------------
class PathFindingStrategy_t
{
 public:
    PathFindingStrategy_t()          = default;
    virtual ~PathFindingStrategy_t() = default;

    // PathFinding virtual to be used as strategy pattern
    virtual void findPath( NpcEnemy_t& npc, const World_t& world,
                                       const Player_t& player ) const = 0;
};

//-----------------------------------------------------------------------------
class A_StarStrategy_t : public PathFindingStrategy_t
{
 public:
    virtual ~A_StarStrategy_t() {}

    void findPath( NpcEnemy_t& npc, const World_t& world,
                           const Player_t& player ) const override;
};
