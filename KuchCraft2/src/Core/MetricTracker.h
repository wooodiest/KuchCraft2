#pragma once

#ifdef  INCLUDE_IMGUI
#include <imgui.h>
#endif

namespace KuchCraft {

	/// A class for tracking and visualizing a single performance metric using a fixed-size circular buffer
	template<typename T, size_t N = 100>
	class MetricTracker
	{
	public:
		MetricTracker()
			: m_CurrentIndex(0), m_Count(0), m_CurrentValue(T())
		{
			std::fill(m_Data.begin(), m_Data.end(), T());
		}

		MetricTracker(T initialValue)
			: m_CurrentIndex(0), m_Count(0), m_CurrentValue(initialValue)
		{
			std::fill(m_Data.begin(), m_Data.end(), initialValue);
		}

		~MetricTracker() = default;

		/// Adds a new value to the metric's history
		void AddValue(T value)
		{
			m_CurrentValue = value;
			m_Data[m_CurrentIndex] = value;
			m_CurrentIndex = (m_CurrentIndex + 1) % N;

			if (m_Count < N)
				m_Count++;
		}

		/// Retrieves the current value of the metric
		T GetCurrentValue() const { return m_CurrentValue; }

		/// Retrieves the maximum value in the history
		T GetMaxValue() const
		{
			return m_Count == 0 ? T() : *std::max_element(m_Data.begin(), m_Data.begin() + m_Count);
		}

		void RenderImGui(const char* label)
		{
#ifdef  INCLUDE_IMGUI

			if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
				ImGui::Text("%s: %d", label, static_cast<int>(m_CurrentValue));
			else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>)
				ImGui::Text("%s: %u", label, static_cast<unsigned int>(m_CurrentValue));
			else if constexpr (std::is_floating_point_v<T>)
				ImGui::Text("%s: %.2f", label, static_cast<float>(m_CurrentValue));
			else
				ImGui::Text("%s: (unsupported type)", label);

			if (m_Count > 0)
			{
				/// Prepare ImGui draw list
				ImVec2 canvasSize = ImVec2(ImGui::GetContentRegionAvail().x, 70);
				ImGui::BeginChild(("##AreaGraph" + std::string(label)).c_str(), canvasSize, true);
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 canvasPos = ImGui::GetCursorScreenPos();

				/// Find maximum value for normalization
				float maxValue = static_cast<float>(GetMaxValue());
				if (maxValue == 0.0f) 
					maxValue = 1.0f; /// Avoid division by zero

				/// Define graph dimensions
				float step = canvasSize.x / static_cast<float>(N - 1);
				float baseY = canvasPos.y + canvasSize.y;

				/// Draw area graph, starting from the most recent value
				for (size_t i = 0; i < m_Count - 1; i++)
				{
					size_t index1 = (m_CurrentIndex + N - i - 1) % N;
					size_t index2 = (m_CurrentIndex + N - i - 2) % N;

					float x1 = canvasPos.x + (N - i - 1) * step;
					float x2 = canvasPos.x + (N - i - 2) * step;

					float y1 = baseY - (static_cast<float>(m_Data[index1]) / maxValue) * canvasSize.y;
					float y2 = baseY - (static_cast<float>(m_Data[index2]) / maxValue) * canvasSize.y;

					/// Draw filled area under the line
					drawList->AddRectFilledMultiColor(
						ImVec2(x1, baseY), ImVec2(x2, y1),
						IM_COL32(100, 200, 255, 100), IM_COL32(100, 200, 255, 100),
						IM_COL32(50, 100, 200, 50), IM_COL32(50, 100, 200, 50)
					);

					/// Draw the line on top
					drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(50, 150, 255, 255), 2.0f);
				}

				ImGui::EndChild();
			}
#endif
		}

	private:
		/// Fixed-size history buffer for the metric
		std::array<T, N> m_Data{};

		/// Current write index in the circular buffer
		size_t m_CurrentIndex = 0;

		/// Number of valid values in the buffer
		size_t m_Count = 0;

		/// Current value of the metric
		T m_CurrentValue = T();
	};

}