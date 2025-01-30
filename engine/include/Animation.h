#pragma once

#include "EngineDefines.h"
#include "TextureAtlas.h"
#include "Sprite.h"

namespace shak
{
    class Animation : public shak::Sprite
    {
    public:
        Animation();
        ~Animation();
    private:
        std::shared_ptr<shak::TextureAtlas> m_atlas;
    };
}