#pragma once
#include <boost/signals2/signal.hpp>

namespace SignalSystem
{
	class InputSignals
	{
		static std::unique_ptr<InputSignals> Events;
	public:
		boost::signals2::signal<void(const glm::vec2&)> onMouseRightClick;
		boost::signals2::signal<void(const glm::vec2&)> onMouseDrag;
		boost::signals2::signal<void(const glm::vec2&)> onMouseRightReleased;
		boost::signals2::signal<void()> onMouseStartRightDrag;
		boost::signals2::signal<void()> onMouseStopRightDrag;
		boost::signals2::signal<void(const glm::vec2&)> onMouseLeftDrag;
		boost::signals2::signal<void(const glm::vec2&)> onMouseStopLeftDrag;
		boost::signals2::signal<void(float)> onMouseScroll;
		boost::signals2::signal<void(const glm::vec2&)> onMouseLeftClick;

		boost::signals2::signal<void()> onSpacePress;
		
		static InputSignals* Get()
		{
			if (Events)
				return Events.get();
			Events = std::make_unique<InputSignals>();
			return Events.get();
		}
	};
}
