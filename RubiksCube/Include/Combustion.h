#pragma once

class Program;

class Combustion : public Program
{
public:
	Combustion(const char* windowName);
	~Combustion() = default;

	Combustion(const Combustion& other) = delete;
	Combustion* operator=(const Combustion& other) = delete;

	void Run() override;

protected:
	void Update() override;
	void Render() override;

	// Used to configure GL States for the program
	void SetupGLStates() override;

	// Used to load models
	void LoadModels() override;
};
