#pragma once

namespace RubixCubeEnums
{
	// Face Colors
	enum class Color : short
	{
		Red = 0,
		Yellow = 1,
		White = 2,
		Blue = 3,
		Orange = 4,
		Green = 5,
		None = 6
	};

	// Internal state machine for animations
	enum class AnimatorState : short
	{
		Idle,
		Animating
	};

	// Logical rotation axis
	enum RotationAxis : short
{
	xAxis = 0,
	yAxis = 1,
	zAxis = 2
};

	// Rotation direction
enum RotationType : short
{
	Clockwise = 0,
	CounterClockwise = 1
};
}

namespace RubixCubeStructs
{
	// Helper struct to hand over parameters
	struct RotatorInitParams
	{
		glm::mat4 m_inverseMatrix;
		glm::vec3 m_xAxis;
		glm::vec3 m_yAxis;
		int m_xInverted;
		int m_yInverted;
	};
}