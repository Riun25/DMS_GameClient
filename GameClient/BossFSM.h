//#pragma once
//#include <any>
//#include <optional>
//#include <functional>
//#include <unordered_map>
//#include "StateEnum.h"
//#include "IState.h"
//#include "IdleState.h"
//#include "MoveState.h"
//#include "AttackState.h"
//#include "DeadState.h"
//#include "Transition.h"
//
///// 2024.8.8의 FSM 구조를 임시로 옮겨둠.
//// FSM은 2가지 구조가 있음
//// 1. 공용 FSM을 씀 -> State를 엔티티 별로 별도로 사용함, 메모리 최적화 but 독립적 작용 어려움
//// 2. 개별 FSM을 씀 -> 일반적인 구조 : 보스전에서만 사용할 FSM을 만들 예정.
//class BossFSM
//{
//public:
//	BossFSM(ClientAnimator* _animator);
//	~BossFSM();
//
//public:
//	IState* currentSM; // 현재 상태 머신 -> FSM에서 현재 어떤 상태인지 추적
//	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> mFuncMap; // 함수를 담는 그릇이다. 이것은 최종적으로 아예 바깥으로 빠져서 돌려쓸 가능성이 있다.
//
//private:
//	Transition* mpTransition; // transition은 가벼운 스위치 역할을 한다.
//
//public: //원랜 프리빗
//	std::unordered_map<State, IState*> mStatePool; // 생성한 state를 저장하는 pool이다.
//
//private:
//	void Initialize(ClientAnimator* _animator); // state들을 생성 - fsm이 살아있는 동안은 돌려쓴다
//	void MakeFunction(); // 기본 fsm 함수를 생성한다. 추가적인 함수는 알아서 그때 클라이언트 쪽에서 추가하자.
//	IState* GetState(State _state); // 생성한 state들을 StatePool에서 찾아서 가져옴
//	void ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum); // state가 변경 가능한지 확인한다.
//
//public:
//	void Update(float _dTime, std::shared_ptr<Entity> _entity); // 업데이트 [엔티티]
//};
//
