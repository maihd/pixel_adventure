// Created by Modar Nasser on 22/11/2020.

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/FieldsContainer.hpp"
#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk
{
    class World;

    class Entity : public FieldsContainer
    {
    public:
                                        Entity(const Entity&) = delete;
                                        Entity(Entity&&) = default;
        auto                            operator=(const Entity&) -> Entity& = delete;

        const std::string&              getName() const;
        const IntPoint&                 getSize() const;
        const IntPoint&                 getPosition() const;
        const IntPoint&                 getGridPosition() const;
        const Color&                    getColor() const;
        const FloatPoint&               getPivot() const;

        bool                            hasTile() const;
        const Tileset&                  getTileset() const;
        const IntRect&                  getTextureRect() const;

        bool                            hasIcon() const;
        const Tileset&                  getIconTileset() const;
        IntPoint                        getIconTexturePos() const;

        bool                            hasTag(const std::string& tag);
        const std::vector<std::string>& getTags();

        explicit Entity(const nlohmann::json& j, const World* w);

    private:
        const EntityDef* m_definition = nullptr;

        const IntPoint m_size;
        const IntPoint m_position;
        const IntPoint m_grid_pos;

        const Tileset* m_tileset = nullptr;
        IntRect m_src_rect;
    };

}
