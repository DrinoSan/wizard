#pragma once

#define BIT( x ) ( 1 << x )

typedef enum
{
   None = 0,
   // Window
   WindowClose,
   WindowResize,
   WindowFoxus,
   WindowLostFocus,
   WindowMoved,
   // App
   AppTick,
   AppUpdate,
   AppRenderer,
   // Key
   KeyPressed,
   KeyReleased,
   KeyTyped,
   // Mouse
   MouseButtonPressed,
   MouseButtonReleased,
   MouseMoved,
   MouseScrolled
} EventType_t;

typedef enum
{
   None                     = 0,
   EventCategoryApplication = BIT( 0 ),
   EventCategoryInput       = BIT( 1 ),
   EventCategoryKeyboard    = BIT( 2 ),
   EventCategoryMouse       = BIT( 3 ),
   EventCategoryMouseButton = BIT( 4 ),
} EventCategory_t;

//#define EVENT_CLASS_TYPE( type )                                               \
//   static EventType_t GetStaticType()                                          \
//   {                                                                           \
//      return EventType_t::type;                                                \
//   }                                                                           \
//   virtual EventType_t GetEventType() const override                           \
//   {                                                                           \
//      return GetStaticType();                                                  \
//   }                                                                           \
//   virtual const char* GetName() const override                                \
//   {                                                                           \
//      return #type;                                                            \
//   }
//
//#define EVENT_CLASS_CATEGORY( category )                                       \
//   virtual int GetCategoryFlags() const override                               \
//   {                                                                           \
//      return category;                                                         \
//   }

//class Event_t
//{
// public:
//   bool Handled = false;
//
//   virtual EventType_t GetEventType() const     = 0;
//   virtual const char* GetName() const          = 0;
//   virtual int         GetCategoryFlags() const = 0;
//   virtual std::string ToString() const { return GetName(); }
//
//   inline bool IsInCategory( EventCategory_t category )
//   {
//      return GetCategoryFlags() & category;
//   }
//};

//-----------------------------------------------------------------------------
typedef struct
{
   EventType_t event_type;
} Event_t;

//-----------------------------------------------------------------------------
typedef struct
{
   Event_t header;
   int key_code;
} KeyPressedEvent_t;

KeyPressedEvent_t key_pressed_event_stack_create( int key_code );
int key_pressed_event_get_key_code( KeyPressedEvent_t* e );
const char* key_pressed_event_to_string( KeyPressedEvent_t* e );

//-----------------------------------------------------------------------------
typedef struct
{
   Event_t event;
} EventDispatcher_t;

//class KeyPressedEvent_t : public KeyEvent_t
//{
// public:
//   KeyPressedEvent_t( int keycode, int repeatCount = 0 )
//       : KeyEvent_t( keycode ), m_RepeatCount( repeatCount )
//   {
//   }
//
//   inline int getRepeatCount() const { return m_RepeatCount; }
//
//   std::string ToString() const override
//   {
//      std::stringstream ss;
//      ss << "KeyPressedEvent_t: " << keyCode << " (" << m_RepeatCount
//         << " repeats)";
//      return ss.str();
//   }
//
//   EVENT_CLASS_TYPE( KeyPressed )
//
// private:
//   int m_RepeatCount;
//};
class EventDispatcher_t
{
 public:
   EventDispatcher_t( Event_t& event_ ) : event( event_ ) {}

   // F will be deduced by the compiler
   template <typename T, typename F>
   bool Dispatch( const F& func )
   {
		std::cout << "Inside Event Dispatcher Event: " << event.ToString() << "\n";
      if ( event.GetEventType() == T::GetStaticType() )
      {
         event.Handled = func( static_cast<T&>( event ) );

         return true;
      }

      return false;
   }

 private:
   Event_t& event;
};

inline std::ostream& operator<<( std::ostream& os, const Event_t& e )
{
   return os << e.ToString();
}
