#pragma once
#include "pch.h"

// 이벤트 데이터 타입 정의 std::any_cast를 통해 특정 타입으로 캐스팅 가능
// std::optional을 통해 데이터가 없는 경우도 처리할수 있습니다.
using EventData = std::optional<std::any>;

/// <summary>
/// 이벤트 이름과 이벤트 데이터를 가지고 있는 이벤트 클래스입니다.
/// </summary>
class Event
{
public:
	Event() = default;	// 디폴트 생성자

	// 이벤트의 이름과 이벤트 데이터를 매개변수로 받는 생성자, data의 디폴트값은 null입니다.
	Event(const std::string& _name, EventData _data = std::nullopt)
		:m_name(_name), m_data(_data)
	{
	}

	template <typename T>
	std::optional<T> GetDataAs() const
	{
		if (m_data->has_value())
		{
			try
			{
				return std::any_cast<T>(m_data.value());
			}
			catch (const std::bad_any_cast& e)
			{
				DLOG(LOG_WARN, std::string("Bad any cast: ") + std::string(e.what()));
			}
		}
		return std::nullopt;
	}	

	std::string m_name;	// 이벤트 이름

private:
	EventData m_data = std::nullopt;	// 이벤트 처리에 사용되는 데이터

	friend class EventManager;
	friend class EventListener;

};
