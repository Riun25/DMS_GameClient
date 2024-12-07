#pragma once
#include "pch.h"

template <typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T;
		}
		return m_pInstance;
	}

	void Finalize()
	{
		delete m_pInstance;
	}


protected:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	virtual ~Singleton() = default;

 	static T* m_pInstance;
};

 template <typename T>
T* Singleton<T>::m_pInstance = nullptr;