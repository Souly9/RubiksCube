#include "stdafx.h"
#include "RubixCubeApp.h"

int main()
{
	std::unique_ptr<RubixCubeApp> prog = std::make_unique<RubixCubeApp>("RubixCube");
	prog->Run();
	return 1;
}
