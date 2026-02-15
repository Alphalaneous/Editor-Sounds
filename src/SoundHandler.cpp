#include "SoundHandler.hpp"
#include <regex>
#include "CustomKeybinds.hpp"

SoundHandler& SoundHandler::get() {
    static SoundHandler ret;
    return ret;
}

void SoundHandler::registerSound(ZStringView eventName, SoundEvent::OnSoundEvent&& soundEvent, const SoundDefaults& soundDefaults) {
    m_registeredSounds[eventName] = SoundEvent::create(eventName, std::move(soundEvent), soundDefaults);
}

void SoundHandler::registerSound(ZStringView eventName, const SoundDefaults& soundDefaults) {
    m_registeredSounds[eventName] = SoundEvent::create(eventName, nullptr, soundDefaults);
}

void SoundHandler::playSound(ZStringView eventName) {
    if (!m_shouldPlayAudio) return;
    auto sound = &m_registeredSounds[eventName];
    if (std::find(m_queuedSounds.begin(), m_queuedSounds.end(), sound) == m_queuedSounds.end()) {
        if (!sound->m_onEvent || sound->m_onEvent(*this, EditorUI::get())) {
            m_queuedSounds.push_back(sound);
        }
    }
}

void SoundHandler::setup() {
    m_shouldPlayAudio = false;
    m_shouldSkipSelectSound = false;
    m_shouldSkipDeselectSound = false;
    m_shouldSkipPlaceSound = false;
    m_shouldSkipPageSound = false;
    m_fileCache.clear();
    m_registeredSounds.clear();
    m_queuedSounds.clear();
    m_delayedSchedules.clear();
}

void SoundHandler::queue(std::function<void()>&& method) {
    m_delayedSchedules.push_back(std::move(method));
}

void SoundHandler::setEnabled(bool audioEnabled) {
    queue([this, audioEnabled] {
        m_shouldPlayAudio = audioEnabled;
    });
}

void SoundHandler::skipDeselectSound() {
    m_shouldSkipSelectSound = true;
}

Result<SoundEvent&> SoundHandler::getSoundByKey(enumKeyCodes keyCode) {
    for (auto& [_, sound] : m_registeredSounds) {
        for (auto& bindData : sound.m_soundDefaults.keys) {
            if (bindData.getByKey(keyCode)) {
                return geode::Ok(sound);
            }
        }
    }
    
    return geode::Err("No sound by keyCode {}", static_cast<int>(keyCode));
}

const std::vector<std::filesystem::path>& SoundHandler::getFiles(const std::filesystem::path& dir) {
    auto it = m_fileCache.find(dir);
    if (it != m_fileCache.end()) return it->second;

    auto& list = m_fileCache[dir];
    for (auto& e : std::filesystem::directory_iterator(dir)) {
        if (e.is_regular_file()) list.push_back(e.path());
    }

    return list;
}

void SoundHandler::findVariants(const std::filesystem::path& dir, ZStringView baseName, std::vector<std::filesystem::path>& out) {
    static const std::regex variantRegex{R"((.+)_var_([0-9]+)$)", std::regex::ECMAScript};

    for (const auto& path : getFiles(dir)) {
        const auto extension = geode::utils::string::pathToString(path.extension());
        if (extension != ".ogg" && extension != ".mp3") continue;

        const std::string stem = geode::utils::string::pathToString(path.stem());

        if (stem == baseName) {
            out.push_back(path);
            continue;
        }

        std::smatch m;
        if (std::regex_match(stem, m, variantRegex)) {
            if (m[1].str() == baseName) {
                out.push_back(path);
            }
        }
    }
}

void SoundHandler::update() {
    m_shouldSkipPlaceSound = false;
    m_shouldSkipSelectSound = false;
    m_shouldSkipDeselectSound = false;
    m_shouldSkipPageSound = false;

    if (!m_queuedSounds.empty()) {
        if (m_shouldPlayAudio && !m_queuedSounds.empty() && LevelEditorLayer::get()->m_playbackMode != PlaybackMode::Playing) {
            for (const auto sound : m_queuedSounds) {
                sound->play();
            }
        }
        m_queuedSounds.clear();
    }

    if (!m_delayedSchedules.empty()) {
        for (const auto& schedule : m_delayedSchedules) {
            if (schedule) schedule();
        }
        m_delayedSchedules.clear();
    }
}