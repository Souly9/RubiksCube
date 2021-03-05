#pragma once
class Material;

class GeometryManager
{
public:
	GeometryManager();
	static GeometryManager* Get();

	void AddGeometry(std::shared_ptr<Mesh3D> geom, const Material& mat);

	std::vector<std::shared_ptr<Mesh3D>> GetGeometryForMaterial(Material mat);

private:
	GeometryManager(GeometryManager& mang) = delete;

	std::map<uint32_t, bool> m_materialMap;
	std::map<uint32_t, std::vector<std::shared_ptr<Mesh3D>>> m_geometryMap;
};
