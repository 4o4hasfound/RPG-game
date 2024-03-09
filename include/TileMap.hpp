#pragma once
#include <string>
#include <vector>
#include <tmxlite/Map.hpp>
#include <SFML/Graphics.hpp>

#include "MapTileLayer.hpp"
#include "MapObjectLayer.hpp"

class TileMap {
public:
	TileMap() = default;
	explicit TileMap(const std::string& filename);

	void update(sf::Time dt);
	void draw(sf::RenderTarget& window);

	const std::vector<AABB>& getLayerObjects(const std::string& layerName) const;
	const std::vector<sf::Vector2f>& getLayerCoords(const std::string& layerName) const;
private:
	tmx::Map m_map;
	std::vector<MapTileLayer::Ptr> m_tileLayer;
	std::vector<MapObjectLayer::Ptr> m_objectLayer;
};