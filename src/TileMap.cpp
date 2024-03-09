#include "TileMap.hpp"

TileMap::TileMap(const std::string& filename) {
	m_map.load(filename);
	const std::vector<tmx::Layer::Ptr>& layers = m_map.getLayers();
	m_tileLayer.reserve(layers.size());
	for (int i = 0; i < layers.size(); ++i) {
		switch (layers[i]->getType()) {
		case tmx::Layer::Type::Tile:
			m_tileLayer.push_back(std::move(std::make_unique<MapTileLayer>(m_map, i)));
			break;
		case tmx::Layer::Type::Object:
			m_objectLayer.push_back(std::move(std::make_unique<MapObjectLayer>(m_map, i)));
			break;
		}
	}
	m_map.~Map();
}

void TileMap::update(sf::Time dt) {
	for (auto& layer : m_tileLayer) {
		layer->update(dt);
	}
}

void TileMap::draw(sf::RenderTarget& window) {
	for (const auto& layer : m_tileLayer) {
		window.draw(*layer.get());
	}
}

const std::vector<AABB>& TileMap::getLayerObjects(const std::string& layerName) const {
	for (const auto& layer : m_objectLayer) {
		if (layer->getName() == layerName) {
			return layer->getObjects();
		}
	}
	return std::vector<AABB>();
}

const std::vector<sf::Vector2f>& TileMap::getLayerCoords(const std::string& layerName) const {
	for (const auto& layer : m_objectLayer) {
		if (layer->getName() == layerName) {
			return layer->getCoords();
		}
	}
	return std::vector<sf::Vector2f>();
}
