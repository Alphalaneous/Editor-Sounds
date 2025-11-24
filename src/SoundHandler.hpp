#pragma once
#include "SoundEvent.hpp"

struct SoundHandler {

    bool m_shouldPlayAudio = false;
    bool m_shouldSkipSelectSound = false;
    bool m_shouldSkipDeselectSound = false;
    bool m_shouldSkipPlaceSound = false;
    bool m_shouldSkipPageSound = false;

    std::unordered_map<std::string, SoundEvent> m_registeredSounds;
    std::vector<SoundEvent*> m_queuedSounds;
    std::vector<std::function<void()>> m_delayedSchedules;
    std::unordered_map<std::filesystem::path, std::vector<std::filesystem::path>> m_fileCache;
    static SoundHandler& get();

    void registerSound(std::string eventName, SoundEvent::OnSoundEvent soundEvent, SoundDefaults soundDefaults = {});
    void registerSound(std::string eventName, SoundDefaults soundDefaults = {});

    void playSound(const std::string& eventName);
    void setup();
    void queue(std::function<void()> method);
    void setEnabled(bool audioEnabled);
    void skipDeselectSound();

    Result<SoundEvent&> getSoundByKey(enumKeyCodes keyCode);

    const std::vector<std::filesystem::path>& getFiles(const std::filesystem::path& dir);
    void findVariants(const std::filesystem::path& dir, std::string_view baseName, std::vector<std::filesystem::path>& out);
    void update();
};