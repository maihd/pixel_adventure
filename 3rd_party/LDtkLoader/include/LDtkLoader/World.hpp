// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Enum.hpp"

namespace ldtk
{
    class World
    {
    public:
                                    World();
                                    World(const World&) = delete;
                                    World(World&&) = default;
        auto                        operator=(const World&) -> World& = delete;

        void                        loadFromFile(const std::string& filepath);

        const FilePath&             getFilePath() const;

        const FloatPoint&           getDefaultPivot() const;

        int                         getDefaultCellSize() const;

        const Color&                getBgColor() const;

        const WorldLayout&          getLayout() const;

        const LayerDef&             getLayerDef(int id) const;
        const LayerDef&             getLayerDef(const std::string& name) const;

        const EntityDef&            getEntityDef(int id) const;
        const EntityDef&            getEntityDef(const std::string& name) const;

        const std::vector<Tileset>& allTilesets() const;
        const Tileset&              getTileset(int id) const;
        const Tileset&              getTileset(const std::string& name) const;

        const Enum&                 getEnum(int id) const;
        const Enum&                 getEnum(const std::string& name) const;

        const std::vector<Level>&   allLevels() const;
        const Level*                getLevel(int id) const;
        const Level*                getLevel(const std::string& name) const;

    private:
        FilePath                    m_file_path;
        FloatPoint                  m_default_pivot;
        int                         m_default_cell_size = 0;
        Color                       m_background_color;

        WorldLayout                 m_layout = WorldLayout::Free;

        std::vector<Tileset>        m_tilesets;
        std::vector<LayerDef>       m_layers_defs;
        std::vector<EntityDef>      m_entities_defs;
        std::vector<Enum>           m_enums;

        std::vector<Level>          m_levels;
    };
}
