#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>

#include "Physics/AABB.hpp"

class MapObjectLayer final {
public:
	using Ptr = std::unique_ptr<MapObjectLayer>;

	MapObjectLayer(const tmx::Map& map, std::size_t idx);
	~MapObjectLayer() = default;
	MapObjectLayer(const MapObjectLayer&) = delete;
	MapObjectLayer& operator=(const MapObjectLayer&) = delete;

	const std::string& getName() const { return m_name; }
	const std::vector<AABB>& getObjects() const { return m_objects; }
	const std::vector<sf::Vector2f>& getCoords() const { return m_coords; }

private:
	std::string m_name;

	std::vector<AABB> m_objects;
	std::vector<sf::Vector2f> m_coords;

	void loadObjects(const tmx::ObjectGroup& layer);
};