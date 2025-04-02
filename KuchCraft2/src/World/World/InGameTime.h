#pragma once

namespace KuchCraft {

    /// Represents different times of the day in the game world.
    enum class TimeOfDay
    {
        /// Represents dawn time.
        Dawn,

        /// Represents morning time.
        Morning,

        /// Represents noon time.
        Noon,

        /// Represents afternoon time.
        Afternoon,

        /// Represents evening time.
        Evening,

        /// Represents dusk time.
        Dusk,

        /// Represents night time.
        Night
    };

    /// Represents an in-game time structure with days, hours, minutes, and seconds.
    struct Time
    {
        /// Number of seconds (0-59).
        uint32_t Seconds = 0;

        /// Number of minutes (0-59).
        uint32_t Minutes = 0;

        /// Number of hours (0-23).
        uint32_t Hours = 0;

        /// Number of days.
        uint32_t Days = 0;
    };

    /// Manages in-game time progression and provides utilities for handling time-related logic.
	class InGameTime
	{
    public:
        InGameTime() = default;

        /// Constructs an InGameTime object with a specified time.
        /// @param time Initial time to set.
        InGameTime(Time time);

         /// Advances the in-game time by a given delta time.
		 /// @param dt delta time elapsed between frames.
        void operator+=(float dt);

        /// @brief Adds a given time duration to the current in-game time.
        /// @param time The time duration to add.
        void operator+=(const Time& time);

        /// Sets the current in-game time.
        /// @param time The new time to set.
        void SetTime(Time time) { m_Time = time; }

        /// Retrieves the current in-game time.
        /// @return The current in-game time.
        Time GetTime() const { return m_Time; }

        /// Sets the speed factor for time progression.
        /// @param factor The new speed factor.
        void SetSpeedFactor(float factor) { m_SpeedFactor = factor; }

        /// Gets the current speed factor for time progression.
        /// @return The current speed factor.
        float GetSpeedFactor() const { return m_SpeedFactor; }

        /// Determines the current time of day based on the in-game time.
        /// @return The corresponding TimeOfDay value.
        TimeOfDay GetTimeOfDay() const;

        /// Converts a TimeOfDay value to a string representation.
        /// @param timeOfDay The TimeOfDay value to convert.
        /// @return A string representation of the given TimeOfDay.
        static std::string TimeOfDayToString(TimeOfDay timeOfDay);

		/// Converts a string representation to a TimeOfDay value.
        static TimeOfDay StringToTimeOfDay(const std::string& time);

    private:
        /// Stores the current in-game time.
        Time m_Time;

        /// Accumulates fractional time steps.
        float m_AccumulatedTime = 0.0f;

        /// Controls the speed of time progression.
        float m_SpeedFactor = 1.0f;
	};
}


