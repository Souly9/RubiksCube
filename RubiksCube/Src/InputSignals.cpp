#include "stdafx.h"
#include <InputSignals.h>

std::unique_ptr<SignalSystem::InputSignals> SignalSystem::InputSignals::Events = std::make_unique<InputSignals>();
std::unique_ptr<SignalSystem::ApplicationEvents> SignalSystem::ApplicationEvents::Events = std::make_unique<
	ApplicationEvents>();
