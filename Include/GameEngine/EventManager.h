#pragma once
#include "IManager.h"

class Event;

// 이벤트핸들러 함수객체입니다.
using EventHandler = std::function<void(const Event&)>;
// 같은 이름의 이벤트를 구별하기 위한 uid입니다.
using HandlerID = uint16_t;

/// <summary>
/// 이벤트 등록, 해제, 실행 등 이벤트 루프를 관리하는 매니저 클래스입니다.
/// 즉각적인 이벤트는 바로 처리하고,
/// 지연된 이벤트는 이벤트 큐에 보관 후 Update()함수에서 순차적으로 실행합니다.
/// 2024.05.16 _Doyo;
/// </summary>
class EventManager
	: public IManager
{
public:
	EventManager() = default;
	~EventManager() = default;

	virtual bool Initialize() override;
	virtual void FixedUpdate(float _fixedDTime) override {}
	virtual void Update(float _dTime) override;
	virtual void LateUpdate(float _dTime) override;
	virtual void Finalize() override;

	HandlerID RegisterImmediateEvent(const std::string& _name, EventHandler _handler);
	HandlerID RegisterDelayedEvent(const std::string& _name, EventHandler _handler);
	void UnregisterEvent(const std::string& _name, HandlerID _handlerId);
	void TriggerEvent(const Event& _event);
	void PushDelayedEvent(const Event& _event, int _delayFrames);

private:
	void TriggerDelayedEvent(const Event& _event);
	void ProcessImmediateEvents();

private:
	std::unordered_map<std::string, std::unordered_map<HandlerID, EventHandler>> mImmediateHandlers;	// 즉각적인 이벤트 핸들러 맵
	std::unordered_map<std::string, std::unordered_map<HandlerID, EventHandler>> mDelayedHandlers;		// 지연된 이벤트 핸들러 맵
	HandlerID m_lastHandlerId = 0;	// 마지막 핸들러 ID

	struct DelayedEvent
	{
		DelayedEvent(std::shared_ptr<Event> e, int frame);

		bool operator<(const DelayedEvent& _other) const;

		std::shared_ptr<Event> mEvent;
		int mExecuteFrame;
	};

	std::priority_queue<DelayedEvent> mDelayedEventQueue;		// 지연된 이벤트 우선순위 큐
	std::queue<std::shared_ptr<Event>> mImmediateEventQueue;	// 즉각적인 이벤트 큐

// 	std::mutex mEventMutex;

	int mCurrentFrame = 0; // 현재 프레임 번호
};