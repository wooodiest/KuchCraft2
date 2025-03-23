#include "kcpch.h"
#include "InGameTime.h"

#include "Core/Config.h"

namespace KuchCraft {

	InGameTime::InGameTime(Time time)
		: m_Time(time)
	{
	}

	void InGameTime::operator+=(float dt)
	{
		constexpr uint32_t minutes_in_day = 1440;

		m_AccumulatedTime += dt * m_SpeedFactor * minutes_in_day / ApplicationConfig::GetWorldData().DurationOfDayInMinutes;

		uint32_t addedSeconds = static_cast<uint32_t>(m_AccumulatedTime);

		m_AccumulatedTime -= static_cast<float>(addedSeconds);

		uint32_t totalSeconds = m_Time.Seconds + addedSeconds;
		m_Time.Seconds = totalSeconds % 60;

		uint32_t totalMinutes = m_Time.Minutes + totalSeconds / 60;
		m_Time.Minutes = totalMinutes % 60;

		uint32_t totalHours = m_Time.Hours + totalMinutes / 60;
		m_Time.Hours = totalHours % 24;

		m_Time.Days += totalHours / 24;
	}

	void InGameTime::operator+=(const Time& time)
	{
		uint32_t totalSeconds = m_Time.Seconds + time.Seconds;
		m_Time.Seconds = totalSeconds % 60;

		uint32_t totalMinutes = m_Time.Minutes + time.Minutes + totalSeconds / 60;
		m_Time.Minutes = totalMinutes % 60;

		uint32_t totalHours = m_Time.Hours + time.Hours + totalMinutes / 60;
		m_Time.Hours = totalHours % 24;

		m_Time.Days += time.Days + totalHours / 24;
	}

	TimeOfDay InGameTime::GetTimeOfDay() const
	{
		if (m_Time.Hours >= 4 && m_Time.Hours < 6)
			return TimeOfDay::Dawn;
		else if (m_Time.Hours >= 6 && m_Time.Hours < 10)
			return TimeOfDay::Morning;
		else if (m_Time.Hours >= 10 && m_Time.Hours < 12)
			return TimeOfDay::Noon;
		else if (m_Time.Hours >= 12 && m_Time.Hours < 14)
			return TimeOfDay::Afternoon;
		else if (m_Time.Hours >= 14 && m_Time.Hours < 17)
			return TimeOfDay::Afternoon;
		else if (m_Time.Hours >= 17 && m_Time.Hours < 20)
			return TimeOfDay::Evening;
		else if (m_Time.Hours >= 20 && m_Time.Hours < 22)
			return TimeOfDay::Dusk;
		else
			return TimeOfDay::Night;
	}

	std::string InGameTime::TimeOfDayToString(TimeOfDay timeOfDay)
	{
		switch (timeOfDay)
		{
			case TimeOfDay::Dawn:      return "Dawn";
			case TimeOfDay::Morning:   return "Morning";
			case TimeOfDay::Noon:      return "Noon";
			case TimeOfDay::Afternoon: return "Afternoon";
			case TimeOfDay::Evening:   return "Evening";
			case TimeOfDay::Dusk:      return "Dusk";
			case TimeOfDay::Night:     return "Night";
			default: return "Unknown";
		}
	}

}