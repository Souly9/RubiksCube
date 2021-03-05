#include "stdafx.h"
#include "GeometryManager.h"

static std::unique_ptr<GeometryManager> m_pSingleton;

GeometryManager::GeometryManager()
{
}

GeometryManager* GeometryManager::Get()
{
	if (!m_pSingleton)
	{
		m_pSingleton = std::make_unique<GeometryManager>();
	}
	return m_pSingleton.get();
}

void GeometryManager::AddGeometry(const std::shared_ptr<Mesh3D> geom, const Material& mat)
{
	const auto iter = m_materialMap.find(mat.GetID());
	if (iter == m_materialMap.end())
	{
		m_materialMap[mat.GetID()] = true;
		std::vector<std::shared_ptr<Mesh3D>> vec;
		m_geometryMap[mat.GetID()] = vec;
		vec.push_back(geom);
	}
	m_geometryMap[mat.GetID()].push_back(geom);
}

std::vector<std::shared_ptr<Mesh3D>> GeometryManager::GetGeometryForMaterial(Material mat)
{
	auto iter = m_materialMap.find(mat.GetID());
	if (iter == m_materialMap.end())
	{
		m_materialMap[mat.GetID()] = true;
		std::vector<std::shared_ptr<Mesh3D>> vec;
		m_geometryMap[mat.GetID()] = vec;
	}
	return m_geometryMap[mat.GetID()];
}
