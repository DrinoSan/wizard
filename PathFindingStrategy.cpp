#include "PathFindingStrategy.h"
#include "Globals.h"
#include "NpcEnemy.h"
#include "Player.h"
#include "World.h"

#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

// ----------------------------------------------------------------------------
struct Node
{
   int   x;
   int   y;
   float g;
   float h;
   int   parentIdx;   // index in nodes vector
};

// ----------------------------------------------------------------------------
static inline int toIndex( int x, int y, int cols )
{
   return y * cols + x;
}

// ----------------------------------------------------------------------------
// Octile heuristic for 8-direction movement (diagonals allowed)
static inline float heuristic( int x1, int y1, int x2, int y2 )
{
   int         dx = std::abs( x1 - x2 );
   int         dy = std::abs( y1 - y2 );
   const float D  = 1.0f;
   const float D2 = std::sqrt( 2.0f );
   int         mn = std::min( dx, dy );
   int         mx = std::max( dx, dy );
   return D * ( mx ) + ( D2 - D ) * mn;
}

// ----------------------------------------------------------------------------
void A_StarStrategy_t::findPath( NpcEnemy_t& npc, const World_t& world,
                                 const Player_t& player ) const
{
   if ( world.worldMap.empty() )
      return;

   // Determine grid dimensions by counting tiles on the first row
   int   cols      = 0;
   float firstRowY = world.worldMap[ 0 ].tileDest.y;
   for ( const auto& t : world.worldMap )
   {
      if ( t.tileDest.y == firstRowY )
         ++cols;
      else
         break;
   }
   if ( cols <= 0 )
      return;
   int rows = static_cast<int>( world.worldMap.size() / cols );

   float tileW = world.worldMap[ 0 ].tileDest.width;
   float tileH = world.worldMap[ 0 ].tileDest.height;

   // Convert world positions to tile coordinates (clamp to grid)
   auto posToTile = [ & ]( const Vector2& pos )
   {
      int tx = static_cast<int>( pos.x / tileW );
      int ty = static_cast<int>( pos.y / tileH );
      if ( tx < 0 )
         tx = 0;
      if ( tx >= cols )
         tx = cols - 1;
      if ( ty < 0 )
         ty = 0;
      if ( ty >= rows )
         ty = rows - 1;
      return std::pair<int, int>( tx, ty );
   };

   auto [ startX, startY ] = posToTile( npc.playerPosition );
   auto [ goalX, goalY ]   = posToTile( player.playerPosition );

   if ( startX == goalX && startY == goalY )
      return;

   int gridSize = cols * rows;

   std::vector<float> gScore( gridSize,
                              std::numeric_limits<float>::infinity() );
   std::vector<int>   parent( gridSize, -1 );
   std::vector<char>  closed( gridSize, 0 );

   // Min-heap by f = g + h
   struct PQItem
   {
      int   idx;
      float f;
   };
   struct Cmp
   {
      bool operator()( PQItem const& a, PQItem const& b ) const
      {
         return a.f > b.f;
      }
   };
   std::priority_queue<PQItem, std::vector<PQItem>, Cmp> open;

   int startIdx       = toIndex( startX, startY, cols );
   gScore[ startIdx ] = 0.0f;
   open.push( { startIdx, heuristic( startX, startY, goalX, goalY ) } );

   // 8-directional neighbors (including diagonals)
   const int dir[ 8 ][ 2 ] = { { 1, 0 }, { -1, 0 }, { 0, 1 },  { 0, -1 },
                               { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

   bool found   = false;
   int  goalIdx = toIndex( goalX, goalY, cols );

   // Obstacle dilation disabled for now (padding = 0). If NPC is not
   // passing narrow corridors correctly, consider enabling padding based
   // on NPC hitbox size. Using padding=0 avoids over-blocking in current
   // setup.
   int padding = 0;

   auto isBlocked = [ & ]( int tx, int ty )
   {
      // return true if any tile within padding square is COLLISION
      int x0 = std::max( 0, tx - padding );
      int x1 = std::min( cols - 1, tx + padding );
      int y0 = std::max( 0, ty - padding );
      int y1 = std::min( rows - 1, ty + padding );
      for ( int yy = y0; yy <= y1; ++yy )
      {
         for ( int xx = x0; xx <= x1; ++xx )
         {
            int i = toIndex( xx, yy, cols );
            if ( world.worldMap[ i ].tileType == TileType_t::COLLISION )
               return true;
         }
      }
      return false;
   };

   while ( !open.empty() )
   {
      auto cur = open.top();
      open.pop();
      int curIdx = cur.idx;
      if ( closed[ curIdx ] )
         continue;
      closed[ curIdx ] = 1;

      if ( curIdx == goalIdx )
      {
         found = true;
         break;
      }

      int cx = curIdx % cols;
      int cy = curIdx / cols;

      for ( auto& d : dir )
      {
         int nx = cx + d[ 0 ];
         int ny = cy + d[ 1 ];
         if ( nx < 0 || nx >= cols || ny < 0 || ny >= rows )
            continue;
         int nIdx = toIndex( nx, ny, cols );

         // Skip blocked tiles (dilated by NPC size)
         if ( isBlocked( nx, ny ) )
            continue;

         // If moving diagonally, prevent corner cutting: don't allow diagonal
         // movement if either adjacent cardinal tiles are blocked.
         if ( std::abs( d[ 0 ] ) + std::abs( d[ 1 ] ) == 2 )   // diagonal
         {
            int idx1 = toIndex( cx + d[ 0 ], cy, cols );
            int idx2 = toIndex( cx, cy + d[ 1 ], cols );
            if ( world.worldMap[ idx1 ].tileType == TileType_t::COLLISION ||
                 world.worldMap[ idx2 ].tileType == TileType_t::COLLISION )
               continue;
         }

         // cost between neighbours: 1 for orthogonal, sqrt(2) for diagonal
         float moveCost   = ( std::abs( d[ 0 ] ) + std::abs( d[ 1 ] ) == 2 )
                                ? std::sqrt( 2.0f )
                                : 1.0f;
         float tentativeG = gScore[ curIdx ] + moveCost;
         if ( tentativeG < gScore[ nIdx ] )
         {
            gScore[ nIdx ] = tentativeG;
            parent[ nIdx ] = curIdx;
            float f        = tentativeG + heuristic( nx, ny, goalX, goalY );
            open.push( { nIdx, f } );
         }
      }
   }

   if ( !found )
      return;

   // Reconstruct path (as tile indices) from goal to start
   std::vector<int> pathIdx;
   int              cur = goalIdx;
   while ( cur != -1 )
   {
      pathIdx.push_back( cur );
      cur = parent[ cur ];
   }
   std::reverse( pathIdx.begin(), pathIdx.end() );
   // Store the path in the NPC so it can follow it across frames
   npc.pathIndices = std::move( pathIdx );
   npc.pathCursor  = 0;

   // Also immediately set the npc velocity toward the next path node so
   // the NPC starts moving without waiting for the manager to compute
   // the direction. This helps the NPC react faster to newly computed
   // paths.
   if ( npc.pathIndices.size() >= 2 )
   {
      int   nextIdx = npc.pathIndices[ 1 ];
      float targetX = world.worldMap[ nextIdx ].tileDest.x +
                      world.worldMap[ nextIdx ].tileDest.width * 0.5f;
      float targetY = world.worldMap[ nextIdx ].tileDest.y +
                      world.worldMap[ nextIdx ].tileDest.height * 0.5f;
      Vector2 dirVec = { targetX - npc.playerPosition.x,
                         targetY - npc.playerPosition.y };
      float   len    = std::sqrt( dirVec.x * dirVec.x + dirVec.y * dirVec.y );
      if ( len > 0.0001f )
      {
         dirVec.x /= len;
         dirVec.y /= len;
         // TODO: add NPC speed variable
         npc.velocity.x = dirVec.x * ( PLAYER_MOVEMENT_SPEED - .5f );
         npc.velocity.y = dirVec.y * ( PLAYER_MOVEMENT_SPEED - .5f );
      }
   }
}
