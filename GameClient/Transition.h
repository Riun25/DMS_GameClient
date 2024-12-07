#pragma once
#include <functional>
#include "StateEnum.h"
/// FSM의 상태전환과 전환 가능 여부를 확인하는 클래스
class Transition
{
public:
	Transition();
	~Transition() {};

	// 전환 가능 여부 : 전환이 일어나지 않을수도 있다 <- 쉽게 말해 단방향
	bool IsCanTransitionTo(State _newStateEnum);
	// 전환 발생 여부 : 전환이 무조건 일어나는 것을 전제한다.
	/// 이 놈이 매우매우 더러운 함수가 될 예정이다. 어떻게 해야 가장 깔끔하고 직관적이 될까.
	//bool IsTransitionNeeded(State& _currentStateEnum, State& _newStateEnum);
	bool IsTransit(State& _currentStateEnum, State& _newStateEnum);
};

