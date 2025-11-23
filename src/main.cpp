#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <random>

using namespace geode::prelude;

static int random(int lower, int upper) {
    if (lower > upper) std::swap(lower, upper);
    
    static std::random_device rd; 
    static std::mt19937 gen(rd()); 

    std::uniform_int_distribution<> dist(lower, upper);
    return dist(gen);
}

struct Sound {
    std::string path;
    float pitch;
};

inline bool operator<(const Sound& a, const Sound& b) {
    return a.path < b.path;
}

struct HijackCallback : public CCObject {

    using Hijack = std::function<void(std::function<void(CCObject* sender)> orig, CCObject* sender)>;

    Hijack m_method;
    SEL_MenuHandler m_selector;

    static HijackCallback* create(Hijack method, SEL_MenuHandler originalSelector) {
        auto ret = new HijackCallback();
        if (ret->init(method, originalSelector)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init(Hijack method, SEL_MenuHandler originalSelector) {
        m_method = method;
        m_selector = originalSelector;
        return true;
    }

    void callback(CCObject* sender) {
        auto btn = static_cast<CCMenuItem*>(sender);
        auto hijack = static_cast<HijackCallback*>(btn->getUserObject("hijack"_spr));
        
        if (hijack->m_method) hijack->m_method([btn, hijack] (CCObject* sender) {
            (btn->m_pListener->*hijack->m_selector)(sender);
        }, sender);
    }

    static void set(CCMenuItem* btn, Hijack method) {
        if (btn->getUserObject("hijack"_spr)) return;

        auto hijack = HijackCallback::create(method, btn->m_pfnSelector);
        btn->setUserObject("hijack"_spr, hijack);
        btn->m_pfnSelector = menu_selector(HijackCallback::callback);
    }
};

struct SoundHandler {

    bool m_shouldPlayAudio = false;
    bool m_shouldSkipSelectSound = false;
    bool m_shouldSkipDeselectSound = false;
    bool m_shouldSkipPlaceSound = false;
    bool m_shouldSkipZoomSound = false;
    bool m_shouldSkipPageSound = false;

    std::unordered_map<std::string, bool> m_existingSounds;
    std::set<Sound> m_queuedSounds;

    static SoundHandler& get() {
        static SoundHandler ret;
        return ret;
    }

    void playSound(const std::string& path, float pitch = 1) {
        if (!m_shouldPlayAudio) return;
        bool exists = m_existingSounds[path];
        if (!exists) exists = !std::filesystem::path(path).empty();
        
        if (exists) {
            m_existingSounds[path] = true;
            m_queuedSounds.insert(Sound{path, pitch});
        }
    }

    void setEnabled(bool audioEnabled) {
        m_shouldPlayAudio = audioEnabled;
    }

    void skipZoomSound() {
        m_shouldSkipZoomSound = true;
    }

    void playSelect() {
        if (m_shouldSkipSelectSound) return;
        m_shouldSkipDeselectSound = true;
        playSound("select.ogg"_spr);
    }

    void playDeselect() {
        if (m_shouldSkipDeselectSound) return;
        m_shouldSkipSelectSound = true;
        playSound("deselect.ogg"_spr);
    }

    void playPlace() {
        m_shouldSkipSelectSound = true;
        m_shouldSkipDeselectSound = true;
        playSound(fmt::format("place_{}.ogg"_spr, random(1, 3)));
    }

    void playDelete() {
        m_shouldSkipDeselectSound = true;
        playSound("delete.ogg"_spr);
    }

    void playCopy() {
        playSound("copy.ogg"_spr);
    }

    void playPaste() {
        m_shouldSkipPlaceSound = true;
        m_shouldSkipSelectSound = true;
        m_shouldSkipDeselectSound = true;
        playSound("paste.ogg"_spr);
    }

    void playUndo() {
        m_shouldSkipPlaceSound = true;
        m_shouldSkipSelectSound = true;
        m_shouldSkipDeselectSound = true;
        playSound("undo.ogg"_spr);
    }

    void playRedo() {
        m_shouldSkipPlaceSound = true;
        m_shouldSkipSelectSound = true;
        m_shouldSkipDeselectSound = true;
        playSound("redo.ogg"_spr);
    }

    void playMove(EditCommand command) {
        switch (command) {
            case EditCommand::Down:
            case EditCommand::Up:
            case EditCommand::Left:
            case EditCommand::Right:
                playSound("move_1.ogg"_spr);
                break;
            case EditCommand::HalfDown:
            case EditCommand::HalfUp:
            case EditCommand::HalfLeft:
            case EditCommand::HalfRight:
                playSound("move_2.ogg"_spr);
                break;
            case EditCommand::SmallDown:
            case EditCommand::SmallUp:
            case EditCommand::SmallLeft:
            case EditCommand::SmallRight:
                playSound("move_3.ogg"_spr);
                break;
            case EditCommand::TinyDown:
            case EditCommand::TinyUp:
            case EditCommand::TinyLeft:
            case EditCommand::TinyRight:
                playSound("move_4.ogg"_spr);
                break;
            case EditCommand::BigDown:
            case EditCommand::BigUp:
            case EditCommand::BigLeft:
            case EditCommand::BigRight:
                playSound("move_5.ogg"_spr);
                break;
            default: 
                playSound("move_2.ogg"_spr);
            break;
        }
    }

    void playRotate() {
        playSound("rotate.ogg"_spr);
    }

    void playDuplicate() {
        m_shouldSkipPlaceSound = true;
        m_shouldSkipSelectSound = true;
        m_shouldSkipDeselectSound = true;
        playSound("duplicate.ogg"_spr);
    }

    void playObjectButton() {
        playSound("objectButton.ogg"_spr);
    }

    void playSwitchTab() {
        playSound("switchTab.ogg"_spr);
    }

    void playGenericButton() {
        m_shouldSkipDeselectSound = true;
        playSound("button.ogg"_spr);
    }

    void playLink() {
        playSound("link.ogg"_spr);
    }

    void playUnlink() {
        playSound("unlink.ogg"_spr);
    }

    void playToggleLink() {
        playSound("toggleLink.ogg"_spr);
    }

    void playNextPage() {
        if (m_shouldSkipPageSound) return;
        playSound("nextPage.ogg"_spr);
        m_shouldSkipPageSound = true;
    }

    void playPrevPage() {
        if (m_shouldSkipPageSound) return;
        playSound("prevPage.ogg"_spr);
        m_shouldSkipPageSound = true;
    }

    void playLockLayer() {
        playSound("lock.ogg"_spr);
    }

    void playUnlockLayer() {
        playSound("unlock.ogg"_spr);
    }

    void playZoomIn() {
        if (m_shouldSkipZoomSound) return;
        playSound("zoomIn.ogg"_spr);
    }
    
    void playZoomOut() {
        if (m_shouldSkipZoomSound) return;
        playSound("zoomOut.ogg"_spr);
    }

    void playSliderTick() {
        playSound("sliderTick.ogg"_spr);
    }

    void playGridIncrease() {
        playSound("gridIncrease.ogg"_spr);
    }

    void playGridDecrease() {
        playSound("gridDecrease.ogg"_spr);
    }

    void playGenericToggle() {
        playSound("toggle.ogg"_spr);
    }

    void update() {
        m_shouldSkipZoomSound = false;
        m_shouldSkipPlaceSound = false;
        m_shouldSkipSelectSound = false;
        m_shouldSkipDeselectSound = false;
        m_shouldSkipPageSound = false;

        if (!m_shouldPlayAudio || LevelEditorLayer::get()->m_playbackMode == PlaybackMode::Playing) return m_queuedSounds.clear();
        if (m_queuedSounds.empty()) return;

        for (const auto& sound : m_queuedSounds) {
            auto fmod = FMODAudioEngine::get();
            fmod->m_globalChannel->setPaused(false);
            fmod->playEffectAdvanced(sound.path, 1, 0, 1, sound.pitch, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
        }

        m_queuedSounds.clear();
    }
};

class $modify(MyEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriorityPre("EditorUI::moveForCommand", Priority::EarlyPre);
        (void) self.setHookPriorityPre("EditorUI::scrollWheel", Priority::EarlyPre);
        (void) self.setHookPriorityPost("EditorUI::init", Priority::VeryLate + 1);
    }

    struct Fields {
        int m_lastPos = 0;
    };

    bool areObjectsSelected() {
        return m_selectedObject || (m_selectedObjects && m_selectedObjects->count() > 0);
    }

    bool init(LevelEditorLayer* editorLayer) {
        SoundHandler::get().setEnabled(false);
        if (!EditorUI::init(editorLayer)) return false;
        setupButtons();
        schedule(schedule_selector(MyEditorUI::updateSounds));
        return true;
    }

    void setupButtons() {
        if (auto gridControlsMenu = getChildByID("hjfod.betteredit/grid-size-controls")) {
            if (auto btn1 = gridControlsMenu->getChildByType<CCMenuItem*>(0)) {
                HijackCallback::set(btn1, [](auto orig, auto sender) {
                    SoundHandler::get().playGridIncrease();
                    orig(sender);
                });
            }
            if (auto btn2 = gridControlsMenu->getChildByType<CCMenuItem*>(1)) {
                HijackCallback::set(btn2, [](auto orig, auto sender) {
                    SoundHandler::get().playGridDecrease();
                    orig(sender);
                });
            }
        }

        if (auto hideUIbtn = typeinfo_cast<CCMenuItem*>(querySelector("> undo-menu > hjfod.betteredit/hide-ui-toggle"))) {
            HijackCallback::set(hideUIbtn, [](auto orig, auto sender) {
                SoundHandler::get().playGenericToggle();
                orig(sender);
            });
        }

        if (auto nextFreeLayerBtn = typeinfo_cast<CCMenuItem*>(querySelector("> layer-menu > hjfod.betteredit/next-free-layer-button"))) {
            HijackCallback::set(nextFreeLayerBtn, [](auto orig, auto sender) {
                SoundHandler::get().playNextPage();
                orig(sender);
            });
        }

        if (auto linkToggle = typeinfo_cast<CCMenuItem*>(querySelector("> link-menu > alphalaneous.tinker/link-controls-toggle"))) {
            HijackCallback::set(linkToggle, [](auto orig, auto sender) {
                SoundHandler::get().playToggleLink();
                orig(sender);
            });
        }

        if (auto buildTabsNavMenu = getChildByID("build-tabs-menu-navigation-menu")) {
            if (auto nextBtn = static_cast<CCMenuItem*>(buildTabsNavMenu->getChildByID("next-button"))) {
                HijackCallback::set(nextBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playNextPage();
                    orig(sender);
                });
            }
            if (auto prevBtn = static_cast<CCMenuItem*>(buildTabsNavMenu->getChildByID("prev-button"))) {
                HijackCallback::set(prevBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playPrevPage();
                    orig(sender);
                });
            }
        }

        if (auto zoomMenu = getChildByID("zoom-menu")) {
            if (auto zoomInBtn = static_cast<CCMenuItem*>(zoomMenu->getChildByID("zoom-in-button"))) {
                HijackCallback::set(zoomInBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playZoomIn();
                    orig(sender);
                });
            }
            if (auto zoomOutBtn = static_cast<CCMenuItem*>(zoomMenu->getChildByID("zoom-out-button"))) {
                HijackCallback::set(zoomOutBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playZoomOut();
                    orig(sender);
                });
            }
        }

        if (auto toolbarMenu = getChildByID("toolbar-categories-menu")) {
            for (auto child : toolbarMenu->getChildrenExt()) {
                if (auto btn = typeinfo_cast<CCMenuItem*>(child)) {
                    HijackCallback::set(btn, [](auto orig, auto sender) {
                        SoundHandler::get().playGenericButton();
                        orig(sender);
                    });
                }
            }
        }

        if (auto togglesMenu = getChildByID("toolbar-toggles-menu")) {
            for (auto child : togglesMenu->getChildrenExt()) {
                if (auto btn = typeinfo_cast<CCMenuItem*>(child)) {
                    HijackCallback::set(btn, [](auto orig, auto sender) {
                        SoundHandler::get().playGenericButton();
                        orig(sender);
                    });
                }
            }
        }
    }

    void setupCreateMenu() {
        EditorUI::setupCreateMenu();
        queueInMainThread([] {
            SoundHandler::get().setEnabled(true);
        });
    }

    void updateSounds(float dt) {
        SoundHandler::get().update();
    }

    void sliderChanged(CCObject* p0) {
        EditorUI::sliderChanged(p0);

        auto objectLayer = m_editorLayer->m_objectLayer;

        int xPos = std::abs(objectLayer->getPositionX()) / objectLayer->getScale();
        int last = m_fields->m_lastPos;

        constexpr int DISTANCE = 1500;

        int stepOld = last / DISTANCE;
        int stepNew = xPos / DISTANCE;

        if (stepNew != stepOld) {
            SoundHandler::get().playSliderTick();
        }

        m_fields->m_lastPos = xPos;
    }

    void selectObject(GameObject* p0, bool p1) {
        SoundHandler::get().playSelect();
        EditorUI::selectObject(p0, p1);
    }

    void selectObjects(CCArray* p0, bool p1) {
        SoundHandler::get().playSelect();
        EditorUI::selectObjects(p0, p1);
    }

    void deselectAll() {
        if (areObjectsSelected()) SoundHandler::get().playDeselect();
        EditorUI::deselectAll();
    }

    void deselectObject(GameObject* object) {
        if (areObjectsSelected()) SoundHandler::get().playDeselect();
        EditorUI::deselectObject(object);
    }

    void doCopyObjects(bool p0) {
        if (areObjectsSelected()) SoundHandler::get().playCopy();
        EditorUI::doCopyObjects(p0);
    }

    void doPasteObjects(bool p0) {
        if (areObjectsSelected()) SoundHandler::get().playPaste();
        EditorUI::doPasteObjects(p0);
    }

    CCPoint moveForCommand(EditCommand command) {
        auto ret = EditorUI::moveForCommand(command);
        if (areObjectsSelected()) SoundHandler::get().playMove(command);
        return ret;
    }

    void transformObjectCall(EditCommand command) {
        if (areObjectsSelected()) SoundHandler::get().playRotate();
        EditorUI::transformObjectCall(command);
    }

    void onDuplicate(CCObject* sender) {
        if (areObjectsSelected()) SoundHandler::get().playDuplicate();
        EditorUI::onDuplicate(sender);
    }

    void undoLastAction(CCObject* p0) {
        if (m_editorLayer->m_undoObjects->count() > 0) SoundHandler::get().playUndo();
        EditorUI::undoLastAction(p0);
    }

    void redoLastAction(CCObject* p0) {
        if (m_editorLayer->m_redoObjects->count() > 0) SoundHandler::get().playRedo();
        EditorUI::redoLastAction(p0);
    }

    void onCreateButton(CCObject* sender) {
        SoundHandler::get().playObjectButton();
        EditorUI::onCreateButton(sender);
    }

    void onSelectBuildTab(CCObject* sender) {
        SoundHandler::get().playSwitchTab();
        EditorUI::onSelectBuildTab(sender);
    }

    void onGroupSticky(CCObject* sender) {
        if (areObjectsSelected()) SoundHandler::get().playLink();
        EditorUI::onGroupSticky(sender);
    }

    void onUngroupSticky(CCObject* sender) {
        if (areObjectsSelected()) SoundHandler::get().playUnlink();
        EditorUI::onUngroupSticky(sender);
    }

    void onGoToBaseLayer(CCObject* sender) {
        SoundHandler::get().playPrevPage();
        EditorUI::onGoToBaseLayer(sender);
    }

    void onGroupDown(CCObject* sender) {
        SoundHandler::get().playPrevPage();
        EditorUI::onGroupDown(sender);
    }

    void onGroupUp(CCObject* sender) {
        SoundHandler::get().playNextPage();
        EditorUI::onGroupUp(sender);
    }

    void keyDown(enumKeyCodes keycode) {
        EditorUI::keyDown(keycode);
        if (keycode == enumKeyCodes::KEY_Left) {
            SoundHandler::get().playPrevPage();
        }
        if (keycode == enumKeyCodes::KEY_Right) {
            SoundHandler::get().playNextPage();
        }
    }

    #ifndef GEODE_IS_ANDROID
    void onLockLayer(CCObject* sender) {
        EditorUI::onLockLayer(sender);
        if (m_editorLayer->m_currentLayer == -1) return;
        
        if (m_editorLayer->m_lockedLayers[m_editorLayer->m_currentLayer]) {
            SoundHandler::get().playLockLayer();
        }
        else {
            SoundHandler::get().playUnlockLayer();
        }
    }
    #endif

    void scrollWheel(float p0, float p1) {
        SoundHandler::get().skipZoomSound();
        EditorUI::scrollWheel(p0, p1);
    }

    void onStopPlaytest(CCObject* sender) {
        SoundHandler::get().skipZoomSound();
        EditorUI::onStopPlaytest(sender);
    }

    void onDelete(CCObject* sender) {
        if (areObjectsSelected()) SoundHandler::get().playDelete();
        EditorUI::onDelete(sender);
    }

    void onDeleteSelected(CCObject* sender) {
        if (areObjectsSelected()) SoundHandler::get().playDelete();
        EditorUI::onDeleteSelected(sender);
    }

    void onDeleteSelectedType(CCObject* sender) {
        SoundHandler::get().playDelete();
        EditorUI::onDeleteSelectedType(sender);
    }

    void onDeleteStartPos(CCObject* sender) {
        SoundHandler::get().playDelete();
        EditorUI::onDeleteStartPos(sender);
    }
};

class $modify(MyLevelEditorLayer, LevelEditorLayer) {

    void removeObject(GameObject* p0, bool p1) {
        SoundHandler::get().playDelete();
        LevelEditorLayer::removeObject(p0, p1);
    }

    GameObject* createObject(int p0, CCPoint p1, bool p2) {
        SoundHandler::get().playPlace();
        return LevelEditorLayer::createObject(p0, p1, p2);
    }
};

class $modify(MyEditButtonBar, EditButtonBar) {

    void loadFromItems(cocos2d::CCArray* objects, int rows, int columns, bool keepPage) {
        EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
        
        queueInMainThread([self = Ref(this)] {
            for (auto child : self->getChildrenExt()) {
                if (auto menu = typeinfo_cast<CCMenu*>(child)) {
                    if (auto prev = menu->getChildByType<CCMenuItemSpriteExtra*>(0)) {
                        HijackCallback::set(prev, [](auto orig, auto sender) {
                            SoundHandler::get().playPrevPage();
                            orig(sender);
                        });
                    }
                    if (auto next = menu->getChildByType<CCMenuItemSpriteExtra*>(1)) {
                        HijackCallback::set(next, [](auto orig, auto sender) {
                            SoundHandler::get().playNextPage();
                            orig(sender);
                        });
                    }
                }
            }
        });
    };
};