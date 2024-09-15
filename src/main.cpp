#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/BoomScrollLayer.hpp>

using namespace geode::prelude;

int random(int min, int max) {
    static bool first = true;
    if (first) {  
        srand(time(nullptr)); 
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

class $modify(MyEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::moveForCommand", INT_MIN);
    }

    struct Fields {
        bool m_playtesting = false;
        int m_lastPos = 0;
        int m_lastObjectCount = 0;
        bool m_playSelectSound = true;
        bool m_initFinished = false;
        bool m_ignoreSound = false;

        std::map<std::string, bool> m_existingSounds;
    };

    void playSoundIfExists(std::string path, float pitch) {

        if (!m_fields->m_initFinished || m_fields->m_ignoreSound) return;

        bool exists = false;

        if (m_fields->m_existingSounds[path]) {
            exists = true;
        }
        else {
            auto soundFile = std::filesystem::path(path);
            exists = !soundFile.empty();
        }

        if (exists) {
            m_fields->m_existingSounds[path] = true;

            FMODAudioEngine::sharedEngine()->m_channelGroup2->setPaused(false);
            FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel->setPaused(false);
            FMODAudioEngine::sharedEngine()->m_globalChannel->setPaused(false);
            FMODAudioEngine::sharedEngine()->m_system->update();
            FMODAudioEngine::sharedEngine()->playEffectAdvanced(path, 1, 0, 1, pitch, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
        }
    }

    void playSoundIfExists(std::string path) {
        playSoundIfExists(path, 1);
    }

    #ifdef GEODE_IS_ANDROID
    void selectAllWithDirection(bool p0) {
        EditorUI::selectAllWithDirection(p0);
        playSoundIfExists("select.ogg"_spr);
    }

    void selectAll() {
        EditorUI::selectAll();
        playSoundIfExists("select.ogg"_spr);
    }
    #endif

    void checkForObjectChange(float dt) {
        
        int last = m_fields->m_lastObjectCount;
        int cur = m_editorLayer->m_objectCount;

        if (last < cur) { // object placed
            int rand = random(1, 3);
            std::string formatted = fmt::format("place_{}.ogg"_spr, rand);
            playSoundIfExists(formatted);
        }

        if (last > cur) { // object deleted
            playSoundIfExists("delete.ogg"_spr);
        }

        m_fields->m_lastObjectCount = m_editorLayer->m_objectCount;
    }

    void onCreateObject(int p0) {
        m_fields->m_playSelectSound = false;
        EditorUI::onCreateObject(p0);
        m_fields->m_playSelectSound = true;
    }

    bool init(LevelEditorLayer* editorLayer) {

        m_fields->m_existingSounds.clear();
        if (!EditorUI::init(editorLayer)) return false;

        m_fields->m_initFinished = true;
        m_fields->m_lastObjectCount = m_editorLayer->m_objectCount;

        schedule(schedule_selector(MyEditorUI::checkForObjectChange));

        return true;
    }

    void sliderChanged(cocos2d::CCObject* p0){
        EditorUI::sliderChanged(p0);

        Slider* slider = this->m_positionSlider;

        int currentPos = floor(slider->getThumb()->getValue()*100);

        if (currentPos % 7 == 0) {
            if (m_fields->m_lastPos != currentPos) {
                playSoundIfExists("sliderTick.ogg"_spr);
                m_fields->m_lastPos = currentPos;
            }
        }
    }

    void selectObject(GameObject* p0, bool p1) {
        EditorUI::selectObject(p0, p1);
        if (m_fields->m_playSelectSound) {
            playSoundIfExists("select.ogg"_spr);
        }
    }

    void selectObjects(cocos2d::CCArray* p0, bool p1) {
        m_fields->m_playSelectSound = false;
        EditorUI::selectObjects(p0, p1);
        m_fields->m_playSelectSound = true;
        playSoundIfExists("select.ogg"_spr);
    }

    void onDeselectAll(cocos2d::CCObject* sender) {
        EditorUI::onDeselectAll(sender);
        playSoundIfExists("deselect.ogg"_spr);
    }

    void doCopyObjects(bool p0) {
        EditorUI::doCopyObjects(p0);
        playSoundIfExists("copy.ogg"_spr);
    }

    void doPasteObjects(bool p0) {
        EditorUI::doPasteObjects(p0);
        playSoundIfExists("paste.ogg"_spr);
    }

    cocos2d::CCPoint moveForCommand(EditCommand command) {
        auto ret = EditorUI::moveForCommand(command);

        switch (command) {
            case EditCommand::Down:
            case EditCommand::Up:
            case EditCommand::Left:
            case EditCommand::Right:
                playSoundIfExists("move_1.ogg"_spr);
                break;
            case EditCommand::HalfDown:
            case EditCommand::HalfUp:
            case EditCommand::HalfLeft:
            case EditCommand::HalfRight:
                playSoundIfExists("move_2.ogg"_spr);
                break;
            case EditCommand::SmallDown:
            case EditCommand::SmallUp:
            case EditCommand::SmallLeft:
            case EditCommand::SmallRight:
                playSoundIfExists("move_3.ogg"_spr);
                break;
            case EditCommand::TinyDown:
            case EditCommand::TinyUp:
            case EditCommand::TinyLeft:
            case EditCommand::TinyRight:
                playSoundIfExists("move_4.ogg"_spr);
                break;
            case EditCommand::BigDown:
            case EditCommand::BigUp:
            case EditCommand::BigLeft:
            case EditCommand::BigRight:
                playSoundIfExists("move_5.ogg"_spr);
                break;
        }

        return ret;
    }

    void transformObjectCall(EditCommand command) {
        EditorUI::transformObjectCall(command);
        playSoundIfExists("rotate.ogg"_spr);
    }

    void onDuplicate(cocos2d::CCObject* sender) {
        EditorUI::onDuplicate(sender);
        playSoundIfExists("duplicate.ogg"_spr);
    }

    void undoLastAction(cocos2d::CCObject* p0) {
        m_fields->m_ignoreSound = true;
        EditorUI::undoLastAction(p0);
        m_fields->m_ignoreSound = false;
        playSoundIfExists("undo.ogg"_spr);
    }

    void redoLastAction(cocos2d::CCObject* p0) {
        m_fields->m_ignoreSound = true;
        EditorUI::redoLastAction(p0);
        m_fields->m_ignoreSound = false;
        playSoundIfExists("redo.ogg"_spr);
    }

    void onCreateButton(cocos2d::CCObject* sender) {
        EditorUI::onCreateButton(sender);
        playSoundIfExists("objectButton.ogg"_spr);
    }

    void onSelectBuildTab(cocos2d::CCObject* sender) {
        EditorUI::onSelectBuildTab(sender);
        playSoundIfExists("switchTab.ogg"_spr);
    }

    void toggleMode(cocos2d::CCObject* sender) {
        EditorUI::toggleMode(sender);
        playSoundIfExists("button.ogg"_spr);
    }

    void toggleSwipe(cocos2d::CCObject* p0) {
        EditorUI::toggleSwipe(p0);
        playSoundIfExists("button.ogg"_spr);
    }
    
    void toggleEnableRotate(cocos2d::CCObject* p0) {
        EditorUI::toggleEnableRotate(p0);
        playSoundIfExists("button.ogg"_spr);
    }
   
    void toggleFreeMove(cocos2d::CCObject* p0) {
        EditorUI::toggleFreeMove(p0);
        playSoundIfExists("button.ogg"_spr);
    }

    void toggleSnap(cocos2d::CCObject* p0) {
        EditorUI::toggleSnap(p0);
        playSoundIfExists("button.ogg"_spr);
    }

    void onGroupSticky(cocos2d::CCObject* sender) {
        EditorUI::onGroupSticky(sender);
        playSoundIfExists("link.ogg"_spr);
    }

    void onUngroupSticky(cocos2d::CCObject* sender) {
        EditorUI::onUngroupSticky(sender);
        playSoundIfExists("unlink.ogg"_spr);
    }

    void onGoToBaseLayer(cocos2d::CCObject* sender) {
        EditorUI::onGoToBaseLayer(sender);
        playSoundIfExists("switchPage.ogg"_spr);
    }

    void onGroupDown(cocos2d::CCObject* sender) {
        EditorUI::onGroupDown(sender);
        playSoundIfExists("switchPage.ogg"_spr);
    }

    void onGroupUp(cocos2d::CCObject* sender) {
        EditorUI::onGroupUp(sender);
        playSoundIfExists("switchPage.ogg"_spr);
    }

    void onPlaytest(cocos2d::CCObject* sender) {
        EditorUI::onPlaytest(sender);
        m_fields->m_playtesting = true;
    }

    void onStopPlaytest(cocos2d::CCObject* sender) {
        EditorUI::onStopPlaytest(sender);
        m_fields->m_playtesting = false;
    }

    void keyDown(cocos2d::enumKeyCodes keycode) {
        EditorUI::keyDown(keycode);
        if (keycode == 39 || keycode == 37 && !m_fields->m_playtesting) {
            playSoundIfExists("switchPage.ogg"_spr);
        }
    }

    #ifndef GEODE_IS_ANDROID
    void onLockLayer(cocos2d::CCObject* sender) {
        EditorUI::onLockLayer(sender);
        LevelEditorLayer* levelEditorLayer = this->m_editorLayer;
    
        if (levelEditorLayer->m_currentLayer != -1) {

            gd::vector<bool> lockedLayers = levelEditorLayer->m_lockedLayers;
            if (lockedLayers[levelEditorLayer->m_currentLayer]) {
                playSoundIfExists("lock.ogg"_spr);
            }
            else {
                playSoundIfExists("unlock.ogg"_spr);
            }
        }
    }
    #endif

    #ifdef GEODE_IS_ANDROID
    void zoomGameLayer(bool in) {
        EditorUI::zoomGameLayer(p0);
        if (in) {
            playSoundIfExists("zoomIn.ogg"_spr);
        }
        else {
            playSoundIfExists("zoomOut.ogg"_spr);
        }
    }
    #endif

    #ifdef GEODE_IS_WINDOWS
    void zoomIn(cocos2d::CCObject* p0) {
        EditorUI::zoomIn(p0);
        playSoundIfExists("zoomIn.ogg"_spr);
    }
    
    void zoomOut(cocos2d::CCObject* p0) {
        EditorUI::zoomOut(p0);
        playSoundIfExists("zoomOut.ogg"_spr);
    }
    #endif
};

class $modify(MyEditButtonBar, EditButtonBar) {

    void loadFromItems(CCArray* items, int c, int r, bool unkBool) {

        EditButtonBar::loadFromItems(items, c, r, unkBool);

        if (auto menu = getChildOfType<CCMenu>(this, 0)) {
            CCMenuItemSpriteExtra* prevButton = getChildOfType<CCMenuItemSpriteExtra>(menu, 0);
            CCMenuItemSpriteExtra* nextButton = getChildOfType<CCMenuItemSpriteExtra>(menu, 1);
            prevButton->m_pfnSelector = menu_selector(MyEditButtonBar::onLeft_h);
            nextButton->m_pfnSelector = menu_selector(MyEditButtonBar::onRight_h);
        }
    }

    void onLeft_h(CCObject* obj) {
        EditButtonBar::onLeft(obj);
        if (EditorUI* editorUI = EditorUI::get()) {
            static_cast<MyEditorUI*>(editorUI)->playSoundIfExists("switchPage.ogg"_spr);
        }
    }

    void onRight_h(CCObject* obj) {
        EditButtonBar::onRight(obj);
        if (EditorUI* editorUI = EditorUI::get()) {
            static_cast<MyEditorUI*>(editorUI)->playSoundIfExists("switchPage.ogg"_spr);
        }
    }
};

#ifndef GEODE_IS_ANDROID
class $modify(EditorPauseLayer) {

    void onSelectAll(cocos2d::CCObject* sender) {
        EditorPauseLayer::onSelectAll(sender);
        if (EditorUI* editorUI = EditorUI::get()) {
            static_cast<MyEditorUI*>(editorUI)->playSoundIfExists("select.ogg"_spr);
        }
    }

    void onSelectAllLeft(cocos2d::CCObject* sender) {
        EditorPauseLayer::onSelectAllLeft(sender);
        if (EditorUI* editorUI = EditorUI::get()) {
            static_cast<MyEditorUI*>(editorUI)->playSoundIfExists("select.ogg"_spr);
        }
    }

    void onSelectAllRight(cocos2d::CCObject* sender) {
        EditorPauseLayer::onSelectAllRight(sender);
        if (EditorUI* editorUI = EditorUI::get()) {
            static_cast<MyEditorUI*>(editorUI)->playSoundIfExists("select.ogg"_spr);
        }
    }
};

#endif