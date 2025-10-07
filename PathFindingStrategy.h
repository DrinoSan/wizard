#include "NpcEnemy.h"

//-----------------------------------------------------------------------------
// Forward declaration
class NpcEnemy_t;

//-----------------------------------------------------------------------------
class PathFindingStrategy_t
{
 public:
   PathFindingStrategy_t()          = default;
   virtual ~PathFindingStrategy_t() = default;

   // PathFinding virtual to be used as strategy pattern
   virtual void findPath( const NpcEnemy_t& npc /*...*/ ) const = 0;
};

//-----------------------------------------------------------------------------
class A_StarStrategy_t : public PathFindingStrategy_t
{
 public:
   virtual ~A_StarStrategy_t() {}

   inline void findPath( const NpcEnemy_t& npc /*...*/ ) const override
   {
      std::cout << "Find path via A*" << std::endl;
   }
};
