// System Header
#include "stdio.h"

// Project Header
#include "Event.h"
#include "../EntityDef.h"

//-----------------------------------------------------------------------------
EventDispatcher_t event_dispatcher_stack_create( Event_t* e )
{
   EventDispatcher_t ed = { .event = e };
   return ed;
}

//-----------------------------------------------------------------------------
bool event_dispatcher_dispatch( EventDispatcher_t* dispatcher, Entity_t* entity,
                                EventHandlerFn handler, EventType_t event_type )
{
   if ( !dispatcher || !handler || !entity )
   {
      return false;
   }

   if ( dispatcher->event->event_type != event_type )
   {
      return false;
   }

   printf("[EventSystem] Dispatching event type %s\n",  event_type_to_string(dispatcher->event->event_type ) );

   handler( entity, dispatcher->event );

   return true;
}

//-----------------------------------------------------------------------------
KeyPressedEvent_t key_pressed_event_stack_create( int key_code,
                                                  int key_repeat_count )
{
   Event_t           e  = { .event_type = EventType_t_KeyPressed, .handeld = false };
   KeyPressedEvent_t ke = { .header           = e,
                            .key_code         = key_code,
                            .key_repeat_count = key_repeat_count };

   return ke;
}

//-----------------------------------------------------------------------------
int key_pressed_event_get_key_code( KeyPressedEvent_t* e )
{
   return e->key_code;
}
//-----------------------------------------------------------------------------
void key_pressed_event_print( KeyPressedEvent_t* e )
{
   printf( "KeyPressedEvent_t: %d ( %d ) repeats\n", e->key_code,
           e->key_repeat_count );
}


//-----------------------------------------------------------------------------
// Helper
//-----------------------------------------------------------------------------
const char* event_type_to_string( EventType_t type )
{
   switch ( type )
   {
   case EventType_t_None:
      return "None";
   case EventType_t_WindowClose:
      return "WindowClose";
   case EventType_t_WindowResize:
      return "WindowResize";
   case EventType_t_WindowFoxus:
      return "WindowFocus";
   case EventType_t_WindowLostFocus:
      return "WindowLostFocus";
   case EventType_t_WindowMoved:
      return "WindowMoved";

   case EventType_t_AppTick:
      return "AppTick";
   case EventType_t_AppUpdate:
      return "AppUpdate";
   case EventType_t_AppRenderer:
      return "AppRenderer";

   case EventType_t_KeyPressed:
      return "KeyPressed";
   case EventType_t_KeyReleased:
      return "KeyReleased";
   case EventType_t_KeyTyped:
      return "KeyTyped";

   case EventType_t_MouseButtonPressed:
      return "MouseButtonPressed";
   case EventType_t_MouseButtonReleased:
      return "MouseButtonReleased";
   case EventType_t_MouseMoved:
      return "MouseMoved";
   case EventType_t_MouseScrolled:
      return "MouseScrolled";

   default:
      return "UNKNOWN_EVENT";
   }
}
