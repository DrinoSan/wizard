#pragma once

#include "stdbool.h"

#define BIT( x ) ( 1 << x )

// Forward dec
// struct Entity_s;
// typedef struct Entity_s Entity_t;
typedef struct Entity Entity_t;

typedef enum
{
   EventType_t_None = 0,
   // Window
   EventType_t_WindowClose,
   EventType_t_WindowResize,
   EventType_t_WindowFoxus,
   EventType_t_WindowLostFocus,
   EventType_t_WindowMoved,
   // App
   EventType_t_AppTick,
   EventType_t_AppUpdate,
   EventType_t_AppRenderer,
   // Key
   EventType_t_KeyPressed,
   EventType_t_KeyReleased,
   EventType_t_KeyTyped,
   // Mouse
   EventType_t_MouseButtonPressed,
   EventType_t_MouseButtonReleased,
   EventType_t_MouseMoved,
   EventType_t_MouseScrolled
} EventType_t;

typedef enum
{
   EventCategory_t_None                     = 0,
   EventCategory_t_EventCategoryApplication = BIT( 0 ),
   EventCategory_t_EventCategoryInput       = BIT( 1 ),
   EventCategory_t_EventCategoryKeyboard    = BIT( 2 ),
   EventCategory_t_EventCategoryMouse       = BIT( 3 ),
   EventCategory_t_EventCategoryMouseButton = BIT( 4 ),
} EventCategory_t;

//-----------------------------------------------------------------------------
struct Event
{
   EventType_t event_type;
   bool        handeld;
};

typedef struct Event Event_t;

//-----------------------------------------------------------------------------
typedef struct
{
   Event_t header;
   int     key_code;
   int     key_repeat_count;
} KeyPressedEvent_t;

KeyPressedEvent_t key_pressed_event_stack_create( int key_code,
                                                  int key_repeat_count );
int               key_pressed_event_get_key_code( KeyPressedEvent_t* e );
void              key_pressed_event_print( KeyPressedEvent_t* e );

//-----------------------------------------------------------------------------
typedef struct
{
   Event_t* event;
} EventDispatcher_t;

typedef bool ( *EventHandlerFn )( Entity_t*, Event_t* );

EventDispatcher_t event_dispatcher_stack_create( Event_t* e );
bool event_dispatcher_dispatch( EventDispatcher_t* dispatcher, Entity_t* entity,
                                EventHandlerFn handler,
                                EventType_t    event_type );

//-----------------------------------------------------------------------------
// Helper
//-----------------------------------------------------------------------------
const char* event_type_to_string( EventType_t type );

