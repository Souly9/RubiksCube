#pragma once

class Scene;

class Renderer
{
public:
	virtual void Render(RenderBatch* batch);
protected:
	std::unique_ptr<Scene> m_currentScene;
};
