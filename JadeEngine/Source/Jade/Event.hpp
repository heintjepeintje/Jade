#pragma once

#include "./Core.hpp"
#include "./System/Input.hpp"

#define JD_DECL_EVENT_TYPE(x) virtual EventType GetEventType() const override { return x; } \
								static EventType GetEventTypeStatic() { return x; }

namespace Jade {
	
	enum struct EventType {
		None = 0,
		WindowClose, WindowResize, WindowMove, WindowFocus,
		KeyPress, KeyRelease, CharacterType,
		MouseMove, MouseButtonPress, MouseButtonRelease
	};
	
	class Event {
	public:
		Event() = default;
		virtual ~Event() = default;
		
		virtual EventType GetEventType() const = 0;
		static EventType GetEventTypeStatic() { return EventType::None; }
		
		inline bool IsHandled() const { return m_IsHandled; }	
	private:
		mutable bool m_IsHandled = false;
	};
	
	class EventDispatcher {
	public:
		EventDispatcher(const Event &event) : m_Event(event) { }
		~EventDispatcher() = default;
		
		template<typename _Type>
		void Dispatch(bool(*callback)(const _Type &)) {
			if (m_Event.IsHandled()) return;
			if (m_Event.GetEventType() != _Type::GetEventTypeStatic()) return;
			callback((const _Type &)m_Event);
		}
		
	private:
		const Event &m_Event;	
	};
	
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() = default;
		
		JD_DECL_EVENT_TYPE(EventType::WindowClose)
	};
	
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent() = delete;
		WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) { }
		~WindowResizeEvent() = default;
		
		inline uint32_t GetWindowWidth() const { return m_Width; }
		inline uint32_t GetWindowHeight() const { return m_Height; }
		
		JD_DECL_EVENT_TYPE(EventType::WindowResize)
	private:
		uint32_t m_Width, m_Height;	
	};
	
	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent() = delete;
		WindowMoveEvent(uint32_t x, uint32_t y) : m_X(x), m_Y(y) { }
		~WindowMoveEvent() = default;
		
		inline uint32_t GetWindowX() const { return m_X; }
		inline uint32_t GetWindowY() const { return m_Y; }
		
		JD_DECL_EVENT_TYPE(EventType::WindowMove)
	private:
		uint32_t m_X, m_Y;
	};
	
	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() = delete;
		WindowFocusEvent(bool isFocused) : m_IsFocused(isFocused) { }
		~WindowFocusEvent() = default;
		
		inline bool IsFocused() const { return m_IsFocused; }
		
		JD_DECL_EVENT_TYPE(EventType::WindowFocus)	
	private:
		bool m_IsFocused;
	};
	
	class KeyPressEvent : public Event {
	public:
		KeyPressEvent() = delete;
		KeyPressEvent(KeyCode key) : m_Key(key) { }
		~KeyPressEvent() = default;
		
		inline KeyCode GetKeyCode() const { return m_Key; }
		
		JD_DECL_EVENT_TYPE(EventType::KeyPress)
	private:
		KeyCode m_Key;
	};
	
	class KeyReleaseEvent : public Event {
	public:
		KeyReleaseEvent() = delete;
		KeyReleaseEvent(KeyCode key) : m_Key(key) { }
		~KeyReleaseEvent() = default;
		
		inline KeyCode GetKeyCode() const { return m_Key; }
		
		JD_DECL_EVENT_TYPE(EventType::KeyRelease)
	private:
		KeyCode m_Key;
	};
	
	class CharacterTypeEvent : public Event {
	public:
		CharacterTypeEvent() = delete;
		CharacterTypeEvent(char character) : m_Character(character) { }
		~CharacterTypeEvent() = default;
		
		inline char GetCharacter() const { return m_Character; }
		
		JD_DECL_EVENT_TYPE(EventType::CharacterType)
	private:
		char m_Character;
	};
	
	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent() = delete;
		MouseMoveEvent(int32_t x, int32_t y) : m_X(x), m_Y(y) { }
		~MouseMoveEvent() = default;
		
		inline int32_t GetMouseX() const { return m_X; }
		inline int32_t GetMouseY() const { return m_Y; }
		
		JD_DECL_EVENT_TYPE(EventType::MouseMove)
		
	private:
		int32_t m_X, m_Y;	
	};
	
	class MouseButtonPressEvent : public Event {
	public:
		MouseButtonPressEvent() = delete;
		MouseButtonPressEvent(MouseButton button) : m_Button(button) { }
		~MouseButtonPressEvent() = default;
		
		inline MouseButton GetButton() const { return m_Button; }
		
		JD_DECL_EVENT_TYPE(EventType::MouseButtonPress)
	private:
		MouseButton m_Button;	
	};
	
	class MouseButtonReleaseEvent : public Event {
	public:
		MouseButtonReleaseEvent() = delete;
		MouseButtonReleaseEvent(MouseButton button) : m_Button(button) { }
		~MouseButtonReleaseEvent() = default;
		
		inline MouseButton GetButton() const { return m_Button; }
		
		JD_DECL_EVENT_TYPE(EventType::MouseButtonRelease)
	private:
		MouseButton m_Button;	
	};
	
}