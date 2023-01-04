#pragma once
#include <chrono>
#include <type_traits>
#include <concepts>


template<typename T>
concept isInteger =
	std::is_same_v<T, int> ||
	std::is_same_v<T, long> ||
	std::is_same_v<T, long long>;

template<typename T>
concept isFloating = std::is_floating_point_v<T>;

template<typename T>
concept isArithmetic = isInteger<T> || isFloating<T>;

// Creates a simple timer.
// Requires a [int, long, long long, or any floating-point]
// Rep - numeric type that represents number of clock ticks
// Period - time interval of one tick
template <isArithmetic Rep, typename Period = std::ratio<1>>
class Timer
{
public:
	
	// Begins the time count.
	Timer() : m_Beginning{ clock_type::now() }
	{}

	// Resets the timer to zero seconds
	void Reset() noexcept
	{
		m_Beginning = clock_type::now();
	}

	// Returns the value of timer in seconds
	Rep Elapsed() const
	{
		return std::chrono::duration_cast<duration_type>(clock_type::now() - m_Beginning).count();
	}

private:
	using clock_type = std::chrono::steady_clock;
	using duration_type = std::chrono::duration<Rep, Period>;

	std::chrono::time_point<clock_type> m_Beginning;
};
