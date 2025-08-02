#pragma once

#include "Event.h"

class KeyEvent_t : public Event_t
{
 public:
   inline int getKeyCode() const { return keyCode; }

   EVENT_CLASS_CATEGORY( EventCategoryKeyboard | EventCategoryInput )
 protected:
   KeyEvent_t( int keycode_ ) : keyCode( keycode_ ) {}

   int keyCode;
};

class KeyPressedEvent_t : public KeyEvent_t
{
 public:
   KeyPressedEvent_t( int keycode, int repeatCount = 0 )
       : KeyEvent_t( keycode ), m_RepeatCount( repeatCount )
   {
   }

   inline int getRepeatCount() const { return m_RepeatCount; }

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "KeyPressedEvent_t: " << keyCode << " (" << m_RepeatCount
         << " repeats)";
      return ss.str();
   }

   EVENT_CLASS_TYPE( KeyPressed )

 private:
   int m_RepeatCount;
};

class KeyReleasedEvent_t : public KeyEvent_t
{
 public:
   KeyReleasedEvent_t( int keycode ) : KeyEvent_t( keycode ) {}

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "KeyReleasedEvent_t: " << keyCode;
      return ss.str();
   }

   EVENT_CLASS_TYPE( KeyReleased )
};

class KeyTypedEvent_t : public KeyEvent_t
{
 public:
   KeyTypedEvent_t( int keycode ) : KeyEvent_t( keycode ) {}

   std::string ToString() const override
   {
      std::stringstream ss;
      ss << "KeyTypedEvent_t: " << keyCode;
      return ss.str();
   }

   EVENT_CLASS_TYPE( KeyTyped )
};
