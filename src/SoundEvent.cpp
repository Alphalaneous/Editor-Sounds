#include "SoundEvent.hpp"
#include "SoundHandler.hpp"
#include "Utils.hpp"
#include <filesystem>

void SoundEvent::play() const {
    if (m_foundVariants.empty()) return;

    auto path = m_foundVariants[EditorSounds::utils::random(0, m_foundVariants.size()-1)];

    auto fmod = FMODAudioEngine::get();
    fmod->m_globalChannel->setPaused(false);
    fmod->playEffectAsync(utils::string::pathToString(path));
}

SoundEvent SoundEvent::create(std::string soundName, OnSoundEvent onEvent, SoundDefaults defaults) {
    SoundEvent event;
    event.m_soundName = soundName;
    event.m_onEvent = onEvent;
    event.m_soundDefaults = defaults;
    for (auto& bindData : event.m_soundDefaults.keys) {
        bindData.parse();
    }

    std::filesystem::path soundPath;

    if (Mod::get()->getSettingValue<bool>("enable-custom-sounds")) {
        soundPath = Mod::get()->getSettingValue<std::filesystem::path>("custom-sound-path");
    }
    else {
        soundPath = Mod::get()->getResourcesDir();
    }

    SoundHandler::get().findVariants(soundPath, event.m_soundName, event.m_foundVariants);

    if (event.m_foundVariants.empty()) {
        SoundHandler::get().findVariants(soundPath, event.m_soundDefaults.fallback, event.m_foundVariants);
    }
    return event;
}