#pragma once

#include <sstream>
#include <iostream>


#define BIT( x ) ( 1 << x )

enum class EventType_t
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
};

enum EventCategory_t
{
   None                     = 0,
   EventCategoryApplication = BIT( 0 ),
   EventCategoryInput       = BIT( 1 ),
   EventCategoryKeyboard    = BIT( 2 ),
   EventCategoryMouse       = BIT( 3 ),
   EventCategoryMouseButton = BIT( 4 ),
};

#define EVENT_CLASS_TYPE( type )                                               \
   static EventType_t GetStaticType()                                          \
   {                                                                           \
      return EventType_t::type;                                                \
   }                                                                           \
   virtual EventType_t GetEventType() const override                           \
   {                                                                           \
      return GetStaticType();                                                  \
   }                                                                           \
   virtual const char* GetName() const override                                \
   {                                                                           \
      return #type;                                                            \
   }

#define EVENT_CLASS_CATEGORY( category )                                       \
   virtual int GetCategoryFlags() const override                               \
   {                                                                           \
      return category;                                                         \
   }

class Event_t
{
 public:
   bool Handled = false;

   virtual EventType_t GetEventType() const     = 0;
   virtual const char* GetName() const          = 0;
   virtual int         GetCategoryFlags() const = 0;
   virtual std::string ToString() const { return GetName(); }

   inline bool IsInCategory( EventCategory_t category )
   {
      return GetCategoryFlags() & category;
   }
};

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
