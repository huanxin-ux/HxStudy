#pragma once
#include <iostream>
#include <mutex>

template<class T>
class SingleT
{
protected:
	SingleT() = default;
	SingleT(const SingleT<T>& st) = delete;
	SingleT operator=(const SingleT<T>& st) = delete;
	virtual ~SingleT() = default;
public:
	static std::shared_ptr<T> GetInstance()
	{
		if (ms_single)
		{
			return ms_single;
		}

		std::unique_lock<std::mutex> lock(ms_mutex);
		if (!ms_single)
		{
			ms_single = std::shared_ptr<T>(new T);
		}	
		return ms_single;
	}
private:
	static std::shared_ptr<T> ms_single;
	static std::mutex ms_mutex;
};

template<class T>
std::shared_ptr<T> SingleT<T>::ms_single = nullptr;

template<class T>
std::mutex SingleT<T>::ms_mutex;


