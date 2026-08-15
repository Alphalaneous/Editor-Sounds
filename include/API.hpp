#pragma once

#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/utils/ZStringView.hpp>

#define MY_MOD_ID "alphalaneous.editorsounds"

namespace alpha::editor_sounds {
    inline bool isLoaded() {
        return geode::Loader::get()->getLoadedMod("alphalaneous.editorsounds");
    }

    inline void playSound(geode::ZStringView ID)
    GEODE_EVENT_EXPORT_NORES(&playSound, (ID));

    inline void assignToMenuItem(cocos2d::CCMenuItem* item, geode::ZStringView ID)
    GEODE_EVENT_EXPORT_NORES(&assignToMenuItem, (item, ID));
}