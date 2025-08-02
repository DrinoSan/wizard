#pragma once

// System Header
#include <cstdint>

// Project Header
#include "events/Event.h"
#include "events/KeyEvent.h"

enum OBJECT_TYPE
{
   PLAYER,
   STATIC,
};

class Graphic_t
{
 public:
   Graphic_t( OBJECT_TYPE type_ ) : type{ type_ } {}
   virtual ~Graphic_t();

   virtual void draw()                                 = 0;
   virtual bool handleMovement( KeyPressedEvent_t& e ) = 0;
   virtual void onEvent( Event_t& e )                  = 0;

   OBJECT_TYPE type;
};
