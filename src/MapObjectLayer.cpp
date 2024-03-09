#include "MapObjectLayer.hpp"

MapObjectLayer::MapObjectLayer(const tmx::Map& map, std::size_t idx) {
    const auto& layers = map.getLayers();
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cout << "Map is not orthogonal - nothing will be drawn" << std::endl;
    }
    else if (idx >= layers.size())
    {
        std::cout << "Layer index " << idx << " is out of range, layer count is " << layers.size() << std::endl;
    }
    else if (layers[idx]->getType() != tmx::Layer::Type::Object)
    {
        std::cout << "layer " << idx << " is not a Object layer..." << std::endl;
    }
    else
    {
        m_name = layers[idx]->getName();

        const auto& objectLayer = layers[idx]->getLayerAs<tmx::ObjectGroup>();
        loadObjects(objectLayer);
    }
}

void MapObjectLayer::loadObjects(const tmx::ObjectGroup& layer) {
    const auto& objects = layer.getObjects();
    m_objects.reserve(objects.size());
    for (const auto& object : objects) {
        if (object.getType() == "AABB") {
            m_objects.emplace_back(object.getAABB());
        }
        else if (object.getType() == "Coord") {
            m_coords.emplace_back(object.getPosition().x, object.getPosition().y);
        }
    }
}