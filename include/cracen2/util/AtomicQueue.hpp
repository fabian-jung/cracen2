#pragma once

#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <boost/optional.hpp>

namespace cracen2 {

namespace util {

template <class Type>
class AtomicQueue {
private:
	std::mutex mutex;
	std::condition_variable poped, pushed;

	std::queue<Type> data;
	const unsigned int maxSize;

public:
	AtomicQueue(unsigned int maxSize) :
		maxSize(maxSize)
	{}

	AtomicQueue(const AtomicQueue& other) = delete;
	AtomicQueue& operator=(const AtomicQueue& other) = delete;

	AtomicQueue(AtomicQueue&& other) = default;
	AtomicQueue& operator=(AtomicQueue&& other) = default;

	void push(const Type& value) {
		std::unique_lock<std::mutex> lock(mutex);
		poped.wait(lock, [this]() -> bool {return data.size() < maxSize;});
		data.push(value);
		pushed.notify_one();
	}

	void push(Type&& value) {
		std::unique_lock<std::mutex> lock(mutex);
		poped.wait(lock, [this]() -> bool {return data.size() < maxSize;});
		data.push(std::move(value));
		pushed.notify_one();
	}

	Type pop() {
		std::unique_lock<std::mutex> lock(mutex);
		pushed.wait(lock, [this]() -> bool {return data.size() > 0;});
		Type result = std::move(data.front());
		data.pop();
		poped.notify_one();
		return result;
	}

	boost::optional<Type> tryPop(std::chrono::milliseconds timeout) {
		std::unique_lock<std::mutex> lock(mutex);
		if(pushed.wait_for(lock, timeout, [this]() -> bool {return data.size() > 0;})) {
			Type result = std::move(data.front());
			data.pop();
			poped.notify_one();
			return result;
		} else {
			return boost::none;
		}
	}

	size_t size() {
		return data.size();
	};
};

}

}

