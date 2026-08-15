#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/API.hpp"
#include "SoundHandler.hpp"
#include "HijackCallback.hpp"

namespace alpha::editor_sounds {

    void playSound(geode::ZStringView ID) {
        SoundHandler::get().playSound(ID);
    }
    
    void assignToMenuItem(cocos2d::CCMenuItem* item, geode::ZStringView ID) {
        HijackCallback::set(item, [ID = std::move(ID)](auto orig, auto sender) {
            SoundHandler::get().playSound(ID);
            orig(sender);
        });
    }

}