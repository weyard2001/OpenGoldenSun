template <typename EventType, typename T>
void EventBus::listen(T* instance, void (T::*callbackFunction)(EventType&)) {
	auto funcHandlers = m_listeners[typeid(EventType)].get();

	if (!funcHandlers) {
		m_listeners[typeid(EventType)] = std::make_unique<FunctionHandlers>();
	}

	auto listener = std::make_unique<FunctionHandler<T, EventType>>(instance, callbackFunction);
	m_listeners[typeid(EventType)]->push_back(std::move(listener));
}

template <typename EventType, typename... Args>
void EventBus::emit(Args&&... args) {
	auto funcHandlers = m_listeners[typeid(EventType)].get();

	if (funcHandlers) {
		for (auto it = funcHandlers->begin(); it != funcHandlers->end(); ++it) {
			EventType event(std::forward<Args>(args)...);
			it->get()->execute(event);
		}
	}
}