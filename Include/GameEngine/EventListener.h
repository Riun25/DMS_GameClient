#pragma once
#include "pch.h"

class Event;
class EventManager;
using EventHandler = std::function<void(const Event&)>;
using HandlerID = uint16_t;

/// <summary>
/// 이벤트 리스너 클래스입니다.
/// 이 클래스를 상속받으면 이벤트를 등록할 수 있습니다.
/// 씬 오브젝트, 월드 오브젝트 등이 상속받아서 사용합니다.
/// </summary>
class EventListener
{
public:
	EventListener() = delete;
	EventListener(EventManager* _eventManager);
	virtual ~EventListener();

protected:
	HandlerID RegisterImmediateEvent(const std::string& _eventName, EventHandler _handler);
	HandlerID RegisterDelayedEvent(const std::string& _eventName, EventHandler _handler);
	void UnregisterEvent(const std::string& _eventName);

protected:
	EventManager* mpEventManager;	// 이벤트 매니저
	std::unordered_map<std::string, HandlerID> mRegisteredImmediateHandlers;	// 즉각적인 이벤트들의 id 맵
	std::unordered_map<std::string, HandlerID> mRegisteredDelayedHandlers;		// 지연된 이벤트들의 id 맵

	friend class EventManager;
};

// 즉각적인 이벤트 등록 매크로
#define REGISTER_IMMEDIATE_EVENT(EVENT_NAME, HANDLER) \
    mRegisteredImmediateHandlers[EVENT_NAME] = RegisterImmediateEvent(EVENT_NAME, [this](const Event& _event) { HANDLER(_event); })

// 지연된 이벤트 등록 매크로
#define REGISTER_DELAYED_EVENT(EVENT_NAME, HANDLER) \
    mRegisteredDelayedHandlers[EVENT_NAME] = RegisterDelayedEvent(EVENT_NAME, [this](const Event& _event) { HANDLER(_event); })

// 이벤트 등록 해제 매크로
#define UNREGISTER_EVENT(EVENT_NAME) \
    UnregisterEvent(EVENT_NAME)

