#pragma once
#include "Program.h"
#include "RubixCube.h"

class Program;

class RubixCubeApp final : public Program
{
public:
	RubixCubeApp(const char* windowName);
	~RubixCubeApp() = default;

	RubixCubeApp(const RubixCubeApp& other) = delete;
	RubixCubeApp* operator=(const RubixCubeApp& other) = delete;

	void Run() override;

protected:
	void Update() override;
	void Render() override;

	// Used to configure GL States for the program
	void SetupGLStates() override;

	// Used to load models
	void LoadModels() override;

private:
	std::unique_ptr<Mesh3D> m_baseCubeMesh;
	std::unique_ptr<RubixCube> m_cube;
};
