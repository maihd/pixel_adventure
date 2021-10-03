// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/Entity.hpp"

namespace ldtk {

    class World;
    class Level;

    class Layer
    {
    public:
        const Level* level = nullptr;

    public:
                                    Layer(const Layer&) = delete;
                                    Layer(Layer&&) = default;
        Layer&                      operator=(const Layer&) = delete;

        const LayerType&            getType() const;
        const std::string&          getName() const;

        int                         getCellSize() const;
        const IntPoint&             getGridSize() const;

        const IntPoint&             getOffset() const;
        void                        setOffset(const IntPoint& offset) const;

        float                       getOpacity() const;
        void                        setOpacity(float opacity) const;

        bool                        hasTileset() const;
        const Tileset*              getTileset() const;

        const std::vector<Tile>&    allTiles() const;
        const Tile&                 getTile(int grid_x, int grid_y) const;

        const IntGridValue&         getIntGridVal(int grid_x, int grid_y) const;

        bool                        hasEntity(const std::string& entity_name) const;
        const std::vector<Entity>&  allEntities() const;
        auto getEntitiesByName(const std::string& entity_name) const -> const std::vector<std::reference_wrapper<Entity>>&;
        auto getEntitiesByTag(const std::string& tag) const -> const std::vector<std::reference_wrapper<Entity>>&;

        Layer(const nlohmann::json& j, const World* w, const Level* l);

    private:
        void updateTileVertices(const Tile& tile) const;
        void updateTileVerticesPos(const Tile& tile) const;
        void updateTileVerticesTex(const Tile& tile) const;
        void updateTileVerticesCol(const Tile& tile) const;

        const LayerDef* m_definition = nullptr;

        bool m_visible;
        mutable IntPoint m_total_offset;
        mutable float m_opacity;
        const IntPoint m_grid_size;

        std::vector<Tile> m_tiles;
        std::vector<Entity> m_entities;
        mutable std::unordered_map<std::string, std::vector<std::reference_wrapper<Entity>>> m_entities_by_name;
        mutable std::unordered_map<std::string, std::vector<std::reference_wrapper<Entity>>> m_entities_by_tag;
        std::map<int, const IntGridValue*> m_intgrid;
        std::map<int, Tile*> m_tiles_map;
    };

}
