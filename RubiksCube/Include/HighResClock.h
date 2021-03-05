/**
 * High resolution clock helper class
 */
#pragma once
#include <chrono>


class HighResClock
{
public:
	// Tick the high resolution clock.
	// Tick the clock before reading the delta time for the first time.
	// Only tick the clock once per frame.
	// Use the Get* functions to return the elapsed time between ticks.
	void Tick();

	// Reset the clock.
	void Reset();

	double GetDeltaNanoseconds() const;
	double GetDeltaMicroseconds() const;
	double GetDeltaMilliseconds() const;
	double GetDeltaSeconds() const;

	double GetTotalNanoseconds() const;
	double GetTotalMicroseconds() const;
	double GetTotalMilliSeconds() const;
	double GetTotalSeconds() const;

private:
	// Initial time point.
	std::chrono::high_resolution_clock::time_point m_T0;
	// Time since last tick.
	std::chrono::high_resolution_clock::duration m_DeltaTime{};
	std::chrono::high_resolution_clock::duration m_TotalTime{};
};

inline void HighResClock::Tick()
{
	const auto t1 = std::chrono::high_resolution_clock::now();
	m_DeltaTime = t1 - m_T0;
	m_TotalTime += m_DeltaTime;
	m_T0 = t1;
}

inline void HighResClock::Reset()
{
	m_T0 = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::high_resolution_clock::duration();
	m_TotalTime = std::chrono::high_resolution_clock::duration();
}

inline double HighResClock::GetDeltaNanoseconds() const
{
	return 1.0 * m_DeltaTime.count();
}

inline double HighResClock::GetDeltaMicroseconds() const
{
	return 1e-3 * m_DeltaTime.count();
}

inline double HighResClock::GetDeltaMilliseconds() const
{
	return 1e-6 * m_DeltaTime.count();
}

inline double HighResClock::GetDeltaSeconds() const
{
	return 1e-9 * m_DeltaTime.count();
}

inline double HighResClock::GetTotalNanoseconds() const
{
	return 1.0 * m_TotalTime.count();
}

inline double HighResClock::GetTotalMicroseconds() const
{
	return 1e-3 * m_TotalTime.count();
}

inline double HighResClock::GetTotalMilliSeconds() const
{
	return 1e-6 * m_TotalTime.count();
}

inline double HighResClock::GetTotalSeconds() const
{
	return 1e-9 * m_TotalTime.count();
}
