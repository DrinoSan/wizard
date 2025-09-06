#pragma once

// System Header
#include <cstdint>

// Project Header
#include "events/Event.h"
#include "events/KeyEvent.h"

//-----------------------------------------------------------------------------
enum OBJECT_TYPE
{
   PLAYER,
   STATIC,
   NPC,
};

//-----------------------------------------------------------------------------
class Graphic_t
{
 public:
   Graphic_t( OBJECT_TYPE type_ ) : type{ type_ } {}
   virtual ~Graphic_t();

   virtual void draw()                                 = 0;

   OBJECT_TYPE type;
};
