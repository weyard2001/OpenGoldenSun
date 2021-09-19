#pragma once

#include "gse/events/Event.hpp"

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <list>

namespace gse {
	class IFunctionHandler {
	public:
		IFunctionHandler() = default;
		virtual ~IFunctionHandler() = default;

		void execute(Event& event) {
			call(event);
		}

	private:
		virtual void call(Event& event) = 0;
	};

	template <typename T, typename EventType>
	class FunctionHandler : public IFunctionHandler {
	public:
		typedef void (T::* CallbackFunction)(EventType&);

		FunctionHandler(T* instance, CallbackFunction callbackFunction)
			: m_instance(instance),
			m_callbackFunction(callbackFunction) {

		}

	private:
		T* m_instance;
		CallbackFunction m_callbackFunction;

		void call(Event& event) override {
			(m_instance->*m_callbackFunction)(static_cast<EventType&>(event));	// EventBus::listen initializes all FunctionHandler instances, so no need to dynamic_cast.
		}
	};

	class EventBus final {
	public:
		using FunctionHandlers = std::list<std::unique_ptr<IFunctionHandler>>;

		EventBus() = default;
		~EventBus() = default;

		// Pass in a System so it can listen for specific Events.
		template <typename EventType, typename T>
		void listen(T* instance, void (T::* callbackFunction)(EventType&));

		// Emit an Event to all Systems listening for it.
		template <typename EventType, typename... Args>
		void emit(Args&&... args);

	private:
		std::unordered_map<std::type_index, std::unique_ptr<FunctionHandlers>> m_listeners;
	};

#include "EventBus.inl"
}