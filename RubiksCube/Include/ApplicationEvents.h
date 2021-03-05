#pragma once
#include "RayStructs.h"

namespace SignalSystem
{
	class ApplicationEvents
	{
		static std::unique_ptr<ApplicationEvents> Events;

	public:
		boost::signals2::signal<void(float)> onUpdate;
		boost::signals2::signal<void(const glm::vec2&)> onScreenResize;
		
		boost::signals2::signal<void(const RayWithMouse&)> onRayCast;
		boost::signals2::signal<void(const glm::quat&)> onArcBallUpdate;
		boost::signals2::signal<void(const glm::quat&)> onRotCamera;
		static ApplicationEvents* Get()
		{
			if (Events)
				return Events.get();
			Events = std::make_unique<ApplicationEvents>();
			return Events.get();
		}
	};
}
