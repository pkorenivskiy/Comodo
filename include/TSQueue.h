#pragma	once

#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#define	QUEUE_TIME_WAIT	6 //00

template <typename T>
class TSQueue
{
public:
	void push(const T& data)
	{
		std::lock_guard<std::mutex> l(m_);

		q_.push(data);
		c_.notify_one();
	}

	size_t size()
	{
		std::lock_guard<std::mutex> l(m_);
		return q_.size();
	}

	T pop(int nTimeOut = QUEUE_TIME_WAIT)
	{
		std::unique_lock<std::mutex> l(m_);
		if (q_.size() == 0)
		{
			if (c_.wait_for(l, std::chrono::seconds(nTimeOut)) == std::cv_status::timeout)
			{
				return nullptr;
			}
		}

		if (bStop_ == true || q_.size() == 0)
			return nullptr;

		T res = q_.front();
		q_.pop();

		return res;
	}

	void stop()
	{
		bStop_ = true;
		c_.notify_one();
	}
private:
	std::queue<T> q_;
	std::mutex m_;
	std::condition_variable c_;
	bool bStop_;
};

