#include "stdafx.h"
#include "RubixCube.h"
#include <random>
#include <windows.h>
#include "ApplicationEvents.h"
#include "Program.h"
#include "RubixCubeEnums.h"

using namespace MaterialUtilities;
using namespace RubixCubeEnums;

RubixCube::RubixCube(Mesh3D* baseMesh, std::vector<std::shared_ptr<RenderBatch>>& renderBatches,
                     const glm::vec3& center,
                     const float sideLength)
	: Object()
	  , m_pRayCastHandler(std::make_unique<RubixCubeDiskManager>())
	  , m_pRotator(std::make_unique<DiskRotator>(0.4f))
	  , m_center(center)
	  , m_sideLengthCubie(sideLength + RUBIX_CUBIE_GAP)
	  , m_sideLengthCubieFace(m_sideLengthCubie * 3.0f)
	  , m_xCubes(3)
	  , m_yCubes(3)
{
	SetName("RubiksCube");


	Shader normalShader("PlainVertNormalMap.vs", "DefaultDeferredFrag.frag");

	std::vector<ShaderAttribute> attbs;
	attbs.push_back(ShaderAttribute::NORMALS);
	attbs.push_back(ShaderAttribute::TEXCOORDS);
	attbs.push_back(ShaderAttribute::VERTEXCOLOR);
	attbs.push_back(ShaderAttribute::TANGENTSPACE);

	std::vector<UniformAttribute> uniforms;
	uniforms.push_back(UniformAttribute::ALBEDO);
	uniforms.push_back(UniformAttribute::NORMALMAP);

	Properties props(glm::vec3(0,0,0),
	                 glm::vec3(0.5, 0.5, 0),
	                 glm::vec3(1, 1, 1),
	                 0.3f * 128.0f);

	std::shared_ptr<TextureData> tmp = std::make_unique<TextureData>();
	tmp->m_albedo = TextureManager::Get()->GetTextureHandle(
		"Textures/Rubik_cube_white_texture.jpg");
	tmp->m_normalMap = TextureManager::Get()->GetTextureHandle(
		"Textures/7252-normal.jpg");
	
	const auto phong = std::make_shared<Material>(MaterialPreset::Default, normalShader, Program::m_materialIndex++, props, tmp, attbs);

	const auto leftCorner = //Red
		center - glm::vec3(
			(m_xCubes * 0.5f) * m_sideLengthCubie,
			(m_yCubes * 0.5f) * m_sideLengthCubie,
			(m_xCubes * 0.5f) * m_sideLengthCubie
		) + glm::vec3(
			(0.5f) * m_sideLengthCubie,
			(0.5f) * m_sideLengthCubie,
			(0.5f) * m_sideLengthCubie
		);

	for (int z = 0; z < 3; ++z)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				const auto curCenter = leftCorner + glm::vec3(
					x * m_sideLengthCubie,
					y * m_sideLengthCubie,
					z * m_sideLengthCubie
				);
				m_cubies.push_back(
					CreateCubie(curCenter, x, y, z, baseMesh, *phong)
				);
			}
		}
	}

	m_centerPieces = {
		{
			CVec<int>{1, 1, 2},
			CVec<int>{2, 1, 1},
			CVec<int>{1, 1, 0},
			CVec<int>{0, 1, 1},
			CVec<int>{1, 0, 1},
			CVec<int>{1, 2, 1}
		}
	};
	int index = 0;
	for (const auto& cubie : m_cubies)
	{
		m_logicalCube[cubie->GetCenter().x + 1][cubie->GetCenter().y + 1][cubie->GetCenter().z + 1] = index;
		++index;
	}

	const auto batch = std::make_shared<RenderBatch>(phong, DataHolder::Get()->GetCameraShared());
	batch->CollectGeometry();
	renderBatches.push_back(batch);

	HookEvents();

	m_translation = m_center;
	m_animState = AnimatorState::Idle;
}

std::shared_ptr<Cubie> RubixCube::CreateCubie(const glm::vec3& center, const int& x, const int& y, const int& z,
                                              Mesh3D* baseMesh,
                                              const Material& mat)
{
	using namespace RubixCubeEnums;
	std::vector<Color> colors;
	DetermineCubieColor(x, y, z, colors);
	const Mesh3D copy = Mesh3D(*baseMesh);
	const auto cubie = std::make_shared<Cubie>(copy, center);

	cubie->SetParent(this);
	cubie->AddColor(colors);
	GeometryManager::Get()->AddGeometry(cubie, mat);
	cubie->Scale(0.5f);
	return cubie;
}

void RubixCube::DetermineCubieColor(const int& x, const int& y, const int& z,
                                    std::vector<RubixCubeEnums::Color>& colors) const
{
	using namespace RubixCubeEnums;
	if (z == 0)
		colors.push_back(Color::Red);
	if (x == 2)
		colors.push_back(Color::Yellow);
	if (z == 2)
		colors.push_back(Color::White);
	if (x == 0)
		colors.push_back(Color::Blue);
	if (y == 0)
		colors.push_back(Color::Orange);
	if (y == 2)
		colors.push_back(Color::Green);
}

/* ---------- RayCasting ----------
 * Functions to handle Raycasting from the camera against the RubiksCube as a whole
 * ------------------------------
 */

void RubixCube::RayCastAgainst(const RayWithMouse& ray)
{
	const RaySlim rayCubeSpace{
		glm::vec3{
			inverse(GetModel()) * glm::vec4(ray.m_origin.x,
			                                ray.m_origin.y,
			                                ray.m_origin.z,
			                                1)
		},
		ray.m_distance,
		normalize(glm::vec3{
			inverse(GetModel()) * glm::vec4(ray.m_direction.x,
			                                ray.m_direction.y,
			                                ray.m_direction.z,
			                                1)
		})
	};
	// Raycast and determine if and which Cubie we hit

	using namespace RubixCubeStructs;
	using namespace RubixCubeEnums;
	RotatorInitParams params{};
	const auto faceIndex = m_pRayCastHandler->RayCastCube(rayCubeSpace, m_curCubeIndex, this);
	if (faceIndex == 99)
		return;
	m_rotateIndex = m_logicalCube[m_curCubeIndex.x][m_curCubeIndex.y][m_curCubeIndex.z];

	// Determine the appropriate rotation axis and whether they should be invertes
	using namespace glm;
	params.m_xInverted = 1;
	params.m_yInverted = 1;
	if (faceIndex == 0 || faceIndex == 2)
	{
		if(faceIndex == 2)
		{
			params.m_yInverted = -1;
		}
		params.m_yAxis = vec4{0, 1, 0, 1};
		params.m_xAxis = vec4{1, 0, 0, 1};
	}
	if (faceIndex == 1 || faceIndex == 3)
	{
		if(faceIndex == 1)
			params.m_yInverted = -1;

		params.m_xAxis = vec3{0, 0, 1};
		params.m_yAxis = vec3{0, 1, 0};
		
	}
	if (faceIndex == 4 || faceIndex == 5)
	{
		if(faceIndex == 5)
			params.m_xInverted = -1;
		if(faceIndex == 4)
			params.m_yInverted = -1;
		params.m_yAxis = vec3{0, 0, 1};
		params.m_xAxis = vec3{1, 0, 0};
	}
	
	m_animState = AnimatorState::Animating;
	params.m_inverseMatrix = inverse(GetModel());
	m_pRotator->StartRotation(ray.m_screenPos, std::move(params), this);
}



/* ---------- Rotation ----------
 * Functions that handle rotation of the RubiksCube
 * ------------------------------
 */
void RubixCube::RotateDisk(const glm::vec2& mousePos)
{
	if (m_animState == RubixCubeEnums::AnimatorState::Animating)
	{
		m_pRotator->ContinueRotation(mousePos);
		if (m_pRotator->IsFirstRot())
		{
			const auto rotAxis = m_pRotator->GetRotType();

			if (rotAxis == xAxis)
			{
				CollectXTurnIndices(m_curCubeIndex, m_curIndices);
			}
			if (rotAxis == yAxis)
			{
				CollectYTurnIndices(m_curCubeIndex, m_curIndices);
			}
			if (rotAxis == zAxis)
			{
				CollectZTurnIndices(m_curCubeIndex, m_curIndices);
			}

			const auto disc = m_pRotator->GetDisk().get();
			for (auto& index : m_curIndices)
			{
				m_cubies.at(index)->SetParent(disc);
			}
		}
	}
}

void RubixCube::RotateLogicalDisc(const RotationAxis& axis, const RotationType& type, const int& turns)
{
	const auto realTurns = turns % 4;
	if (realTurns > 0)
	{
		const float angle = type == Clockwise ? turns * 90.0f : turns * - 90.0f;
		glm::vec3 ax;
		if (axis == xAxis)
			ax = glm::vec3(0, 1, 0);
		if (axis == yAxis)
			ax = glm::vec3(1, 0, 0);
		if (axis == zAxis)
			ax = glm::vec3(0, 0, 1);
		const auto rotation = angleAxis(glm::radians(angle), ax);
		
		for (const int& i : m_curIndices)
		{
			Cubie* cubie = m_cubies.at(i).get();
			const auto point = cubie->GetTranslation();
			const glm::vec3 pos = rotation * glm::vec4(point.x, point.y, point.z, 1);

			int x = round(pos.x);
			int y = round(pos.y);
			int z = round(pos.z);
			const CVec<int> cen{x, y, z};
			cubie->SetCenter(cen);
			m_logicalCube[cen.x + 1][cen.y + 1][cen.z + 1] = i;
		}
	}
}

void RubixCube::EndDiskRotation(const glm::vec2& mousePos)
{
	if (m_animState == RubixCubeEnums::AnimatorState::Animating)
	{
		RotationType type;
		RotationAxis axis;
		int turns;
		m_pRotator->EndRotation(mousePos, type, axis, turns);

		for (const auto& index : m_curIndices)
		{
			m_cubies.at(index)->AdditiveRotation(m_pRotator->GetDisk()->GetRotation());
			m_cubies.at(index)->SetParent(this);
		}
		RotateLogicalDisc(axis, type, turns);

		// Cleanup
		m_curIndices.clear();
		
		if (turns > 0)
		{
			if (CheckSolved())
				Solved();
		}
		m_animState = AnimatorState::Idle;
	}
}

/* ---------- Solve and Shuffling ----------
 * Functions that check whether the cube is solved and reshuffle it if needed
 * ------------------------------
 */
bool RubixCube::CheckSolved()
{
	for (int f = 0; f < 6; ++f)
	{
		const CVec<int> center = m_centerPieces[f];
		const Cubie* centerCubie = m_cubies.at(m_logicalCube[center.x][center.y][center.z]).get();
		const auto faceColor = centerCubie->GetColors().at(0);
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				const CVec<int> in = GetNextLogicalPos(faceColor, m_centerPieces[f], x, y);

				const auto& colors = m_cubies.at(m_logicalCube[in.x][in.y][in.z])->GetColors();
				
				if (std::none_of(colors.begin(), colors.end(), 
					[&faceColor](const auto& elem) {return elem == faceColor;})
					)
					return false;
			}
		}
	}
	return true;
}

void RubixCube::Solved()
{
	PlaySound(TEXT("Sounds/bell.wav"), nullptr, SND_ASYNC | SND_FILENAME);
	Shuffle();
}

void RubixCube::Shuffle()
{
	using namespace RubixCubeEnums;

	std::random_device s;
	std::default_random_engine gen{s()};
	std::uniform_int_distribution<int> typeRand(0, 1);
	std::uniform_int_distribution<int> axisRand(0, 2);
	for (int i = 0; i < RUBIX_SHUFFLES; ++i)
	{
		const auto rotAxis = static_cast<RotationAxis>(axisRand(gen));
		const auto type = static_cast<RotationType>(typeRand(gen));
		if (rotAxis == xAxis)
		{
			m_curCubeIndex = CVec<int>{0, axisRand(gen), 0};
			CollectXTurnIndices(m_curCubeIndex, m_curIndices);
		}
		if (rotAxis == yAxis)
		{
			m_curCubeIndex = CVec<int>{axisRand(gen), 0, 0};
			CollectYTurnIndices(m_curCubeIndex, m_curIndices);
		}
		if (rotAxis == zAxis)
		{
			m_curCubeIndex = CVec<int>{0, 0, axisRand(gen)};
			CollectZTurnIndices(m_curCubeIndex, m_curIndices);
		}
		
		const float angle = type == Clockwise ? 90.0f : -90.0f;
		glm::vec3 ax;
		if (rotAxis == xAxis)
			ax = glm::vec3(0, 1, 0);
		if (rotAxis == yAxis)
			ax = glm::vec3(1, 0, 0);
		if (rotAxis == zAxis)
			ax = glm::vec3(0, 0, 1);
		
		const auto rotation = angleAxis(glm::radians(angle), ax);
		for (const auto& index : m_curIndices)
		{
			m_cubies.at(index)->AdditiveRotation(rotation);
		}
		
		RotateLogicalDisc(
			rotAxis,
			type,
			1
		);
		m_curIndices.clear();
	}
}

/* ---------- Helper Functions ----------
 * Helper functions specifically for the RubiksCube game
 * ------------------------------
 */
CVec<int> RubixCube::GetNextLogicalPos(const RubixCubeEnums::Color& color, const CVec<int>& in, const int& x,
                                       const int& y) const
{
	using namespace RubixCubeEnums;
	if (color == Color::White || color == Color::Red)
	{
		return {x, y, in.z};
	}
	if (color == Color::Yellow || color == Color::Blue)
	{
		return {in.x, x, y};
	}
	return {x, in.y, y};
}

void RubixCube::CollectXTurnIndices(const CVec<int>& indices, std::vector<int>& rslt) const
{
	const CVec<int> pos{0, indices.y, 0};
	for (int i = 0; i < 3; ++i)
	{
		for (int u = 0; u < 3; ++u)
		{
			rslt.emplace_back(m_logicalCube[pos.x + i][pos.y][pos.z + u]);
		}
	}
}

void RubixCube::CollectYTurnIndices(const CVec<int>& indices, std::vector<int>& rslt) const
{
	const CVec<int> pos{indices.x, 0, 0};
	for (int i = 0; i < 3; ++i)
	{
		for (int u = 0; u < 3; ++u)
		{
			rslt.emplace_back(m_logicalCube[pos.x][pos.y + i][pos.z + u]);
		}
	}
}

void RubixCube::CollectZTurnIndices(const CVec<int>& indices, std::vector<int>& rslt) const
{
	const CVec<int> pos{0, 0, indices.z};
	for (int i = 0; i < 3; ++i)
	{
		for (int u = 0; u < 3; ++u)
		{
			rslt.emplace_back(m_logicalCube[pos.x + i][pos.y + u][pos.z]);
		}
	}
}
using namespace SignalSystem;

void RubixCube::HookEvents()
{
	m_rotatorState = ShouldRespond;
	ApplicationEvents::Get()->onRayCast.connect([this](const RayWithMouse& ray) { RayCastAgainst(ray); });
	ApplicationEvents::Get()
		->onArcBallUpdate.connect([this](const auto& quat) { if(m_rotatorState == ShouldRespond) Rotate(quat); });
	InputSignals::Get()->onMouseLeftDrag.connect([this](auto pos) { RotateDisk(pos); });
	InputSignals::Get()->onMouseStopLeftDrag.connect([this](const glm::vec2& pos) { EndDiskRotation(pos); });
	InputSignals::Get()->onSpacePress.connect([this]() { if(m_animState == AnimatorState::Idle) Shuffle(); });
}
