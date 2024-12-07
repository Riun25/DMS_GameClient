#pragma once
#include "pch.h"

template <typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (mpInstance == nullptr)
		{
			mpInstance = new T;
		}
		return mpInstance;
	}

	void Finalize()
	{
		delete mpInstance;
	}


protected:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	virtual ~Singleton() = default;

 	static T* mpInstance;
};

 template <typename T>
T* Singleton<T>::mpInstance = nullptr;