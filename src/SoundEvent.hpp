#pragma once
#include <Geode/Geode.hpp>
#include "CustomKeybinds.hpp"

class SoundHandler;

using namespace geode::prelude;

struct SoundDefaults {
    std::string fallback;
    std::vector<CustomKeyinds::BindData> keys;
};

struct SoundEvent {
    using OnSoundEvent = std::function<bool(SoundHandler& soundHandler, EditorUI* editorUI)>;

    OnSoundEvent m_onEvent;
    SoundDefaults m_soundDefaults;
    std::string m_soundName;
    std::vector<std::filesystem::path> m_foundVariants;

    void play() const;

    static SoundEvent create(std::string soundName, OnSoundEvent onEvent, SoundDefaults defaults);
};