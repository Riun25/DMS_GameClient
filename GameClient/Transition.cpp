#include "Transition.h"

Transition::Transition()
{
}

bool Transition::IsCanTransitionTo(State _newStateEnum)
{
	/// 우리 게임은 그냥 Dead일 때만 다른 상태로 전환 불가인 조건만 존재함
	if (_newStateEnum == State::DEAD)
	{
		return false;  // Dead 상태에서 다른 상태로 전환 불가
	}

	// 다른 상태 전환에 대한 로직 추가할 것
	return true;
}

bool Transition::IsTransit(State& _currentStateEnum, State& _newStateEnum)
{
	// 만약 받아온 상태가 다르면 true를 반환한다.
	return _currentStateEnum != _newStateEnum;
}
