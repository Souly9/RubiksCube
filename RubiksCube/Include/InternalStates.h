#pragma once

enum RenderBatchState : short
{
	NoUpdate,
	TextureUpdate
};

enum MouseRotation : int
{
	ShouldRespond,
	ShouldIgnore
};