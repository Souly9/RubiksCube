#pragma once
#include "Renderer.h"

class RenderBatch;
class Renderer;

class ForwardRenderer : public Renderer
{
public:

	void Render(RenderBatch* batch) override;
	void RenderInstanced(RenderBatch* batch);
private:
};
