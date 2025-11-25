#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include "CustomKeybinds.hpp"
#include "Geode/loader/Log.hpp"
#include "SoundHandler.hpp"
#include "HijackCallback.hpp"

using namespace geode::prelude;

bool areObjectsSelected(EditorUI* editorUI) {
    if (!editorUI) return false;
    return editorUI->m_selectedObject || (editorUI->m_selectedObjects && editorUI->m_selectedObjects->count() > 0);
}

void initializeSounds() {
    auto& sh = SoundHandler::get();

    sh.registerSound("select", [](auto& sh, auto editorUI) {
        if (sh.m_shouldSkipSelectSound) return false;
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("deselect", [](auto& sh, auto editorUI) {
        if (sh.m_shouldSkipDeselectSound || !areObjectsSelected(editorUI)) return false;
        sh.m_shouldSkipSelectSound = true;
        return true;
    });

    sh.registerSound("place-object", [](auto& sh, auto editorUI) {
        sh.m_shouldSkipSelectSound = true;
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("delete", [](auto& sh, auto editorUI) {
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("copy", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    });

    sh.registerSound("paste", [](auto& sh, auto editorUI) {
        sh.m_shouldSkipPlaceSound = true;
        sh.m_shouldSkipSelectSound = true;
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("undo", [](auto& sh, auto editorUI) {
        if (editorUI->m_editorLayer->m_undoObjects->count() == 0) return false;
        sh.m_shouldSkipPlaceSound = true;
        sh.m_shouldSkipSelectSound = true;
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("redo", [](auto& sh, auto editorUI) {
        if (editorUI->m_editorLayer->m_redoObjects->count() == 0) return false;
        sh.m_shouldSkipPlaceSound = true;
        sh.m_shouldSkipSelectSound = true;
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("move-down", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move"
    });

    sh.registerSound("move-up", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move"
    });

    sh.registerSound("move-left", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move"
    });

    sh.registerSound("move-right", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move"
    });

    sh.registerSound("move-half-down", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-half"
    });

    sh.registerSound("move-half-up", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-half"
    });

    sh.registerSound("move-half-left", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-half"
    });

    sh.registerSound("move-half-right", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-half"
    });

    sh.registerSound("move-small-down", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-small"
    });

    sh.registerSound("move-small-up", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-small"
    });

    sh.registerSound("move-small-left", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-small"
    });

    sh.registerSound("move-small-right", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-small"
    });

    sh.registerSound("move-tiny-down", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-tiny"
    });

    sh.registerSound("move-tiny-up", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-tiny"
    });

    sh.registerSound("move-tiny-left", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-tiny"
    });

    sh.registerSound("move-tiny-right", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-tiny"
    });

    sh.registerSound("move-big-down", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-big"
    });

    sh.registerSound("move-big-up", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-big"
    });

    sh.registerSound("move-big-left", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-big"
    });

    sh.registerSound("move-big-right", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "move-big"
    });

    sh.registerSound("rotate-clockwise", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "rotate"
    });

    sh.registerSound("rotate-counter-clockwise", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "rotate"
    });

    sh.registerSound("rotate-45-clockwise", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "rotate"
    });

        sh.registerSound("rotate-45-counter-clockwise", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "rotate"
    });

    sh.registerSound("flip-x", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "rotate"
    });

    sh.registerSound("flip-y", [](auto& sh, auto editorUI) {
        return areObjectsSelected(editorUI);
    }, {
        .fallback = "rotate"
    });

    sh.registerSound("duplicate", [](auto& sh, auto editorUI) {
        if (!areObjectsSelected(editorUI)) return false;
        sh.m_shouldSkipPlaceSound = true;
        sh.m_shouldSkipSelectSound = true;
        sh.m_shouldSkipDeselectSound = true;
        return true;
    });

    sh.registerSound("toolbar-categories", [](auto& sh, auto editorUI) {
        sh.m_shouldSkipDeselectSound = true;
        return true;
    }, {
        .fallback = "generic-button",
        .keys = {
            {
                .id = "robtop.geometry-dash/build-mode",
                .binds = {
                    { 
                        .key = enumKeyCodes::KEY_One 
                    }
                }
            }, 
            {
                .id = "robtop.geometry-dash/edit-mode",
                .binds = {
                    {
                        .key = enumKeyCodes::KEY_Two 
                    }
                }
            }, 
            {
                .id = "robtop.geometry-dash/delete-mode",
                .binds = { 
                    {
                        .key = enumKeyCodes::KEY_Three 
                    }
                }
            },
            {
                .id = "hjfod.betteredit/view-mode"
            }
        }
    });

    sh.registerSound("toolbar-toggles", {
        .fallback = "generic-button",
        .keys = {
            {
                .id = "robtop.geometry-dash/toggle-rotate",
                .binds = { 
                    {
                        .key = enumKeyCodes::KEY_R 
                    }
                }
            }, 
            {
                .id = "robtop.geometry-dash/toggle-free-move",
                .binds = { 
                    {
                        .key = enumKeyCodes::KEY_F 
                    }
                }
            }, 
            {
                .id = "robtop.geometry-dash/toggle-swipe",
                .binds = { 
                    {
                        .key = enumKeyCodes::KEY_T
                    }
                }
            }, 
            {
                .id = "robtop.geometry-dash/toggle-snap",
                .binds = { 
                    {
                        .key = enumKeyCodes::KEY_G
                    }
                }
            }
        }
    });

    sh.registerSound("next-page", [](auto& sh, auto editorUI) {
        sh.queue([] {
            SoundHandler::get().m_shouldSkipPageSound = true;
        });
        return !sh.m_shouldSkipPageSound;
    });

    sh.registerSound("prev-page", [](auto& sh, auto editorUI) {
        sh.queue([] {
            SoundHandler::get().m_shouldSkipPageSound = true;
        });
        return !sh.m_shouldSkipPageSound;
    });

    sh.registerSound("base-layer", {
        .fallback = "prev-page"
    });

    sh.registerSound("next-free-layer", {
        .fallback = "next-page"
    });

    sh.registerSound("prev-layer", {
        .fallback = "prev-page",
        .keys = {
            {
                .id = "robtop.geometry-dash/prev-layer",
                .binds = {
                    {
                        .key = enumKeyCodes::KEY_Left
                    }
                },
                .hasRepeat = true,
                .rate = 50,
                .delay = 500
            }
        }
    });

    sh.registerSound("next-layer", {
        .fallback = "next-page",
        .keys = {
            {
                .id = "robtop.geometry-dash/next-layer",
                .binds = {
                    {
                        .key = enumKeyCodes::KEY_Right
                    }
                },
                .hasRepeat = true,
                .rate = 50,
                .delay = 500
            }
        }
    });

    sh.registerSound("tabs-prev-page", {
        .fallback = "prev-page"
    });

    sh.registerSound("tabs-next-page", {
        .fallback = "next-page"
    });

    sh.registerSound("switch-tab");
    sh.registerSound("zoom-in");
    sh.registerSound("zoom-out");
    sh.registerSound("lock-layer");
    sh.registerSound("unlock-layer");
    sh.registerSound("object-button");
    sh.registerSound("link");
    sh.registerSound("unlink");
    sh.registerSound("toggle-link");
    sh.registerSound("slider-tick");
    sh.registerSound("grid-increase");
    sh.registerSound("grid-decrease");
    sh.registerSound("toggle-ui");
}

class $modify(MyEditorPauseLayer, EditorPauseLayer) {
    void onResume(CCObject* sender) {
        for (auto& [_, sound] : SoundHandler::get().m_registeredSounds) {
            for (auto& bindData : sound.m_soundDefaults.keys) {
                bindData.parse();
            }
        }
        EditorPauseLayer::onResume(sender);
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

    bool init(LevelEditorLayer* editorLayer) {
        SoundHandler::get().setup();
        initializeSounds();

        if (!EditorUI::init(editorLayer)) return false;

        setupButtons();
        schedule(schedule_selector(MyEditorUI::updateSounds));
        return true;
    }

    void setupButtons() {
        if (auto gridControlsMenu = getChildByID("hjfod.betteredit/grid-size-controls")) {
            if (auto btn1 = gridControlsMenu->getChildByType<CCMenuItem*>(0)) {
                HijackCallback::set(btn1, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("grid-increase");
                    orig(sender);
                });
            }
            if (auto btn2 = gridControlsMenu->getChildByType<CCMenuItem*>(1)) {
                HijackCallback::set(btn2, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("grid-decrease");
                    orig(sender);
                });
            }
        }

        if (auto hideUIbtn = typeinfo_cast<CCMenuItem*>(querySelector("> undo-menu > hjfod.betteredit/hide-ui-toggle"))) {
            HijackCallback::set(hideUIbtn, [](auto orig, auto sender) {
                SoundHandler::get().playSound("toggle-ui");
                orig(sender);
            });
        }

        if (auto nextFreeLayerBtn = typeinfo_cast<CCMenuItem*>(querySelector("> layer-menu > hjfod.betteredit/next-free-layer-button"))) {
            HijackCallback::set(nextFreeLayerBtn, [](auto orig, auto sender) {
                SoundHandler::get().playSound("next-free-layer");
                orig(sender);
            });
        }

        if (auto linkMenu = getChildByID("link-menu")) {
            if (auto btn = static_cast<CCMenuItem*>(linkMenu->getChildByID("alphalaneous.tinker/link-controls-toggle"))) {
                HijackCallback::set(btn, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("toggle-link");
                    orig(sender);
                });
            }
            if (auto btn = static_cast<CCMenuItem*>(linkMenu->getChildByID("alphalaneous.improvedlink/link-controls-toggle"))) {
                HijackCallback::set(btn, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("toggle-link");
                    orig(sender);
                });
            }
        }

        if (auto buildTabsNavMenu = getChildByID("build-tabs-menu-navigation-menu")) {
            if (auto nextBtn = static_cast<CCMenuItem*>(buildTabsNavMenu->getChildByID("next-button"))) {
                HijackCallback::set(nextBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("tabs-next-page");
                    orig(sender);
                });
            }
            if (auto prevBtn = static_cast<CCMenuItem*>(buildTabsNavMenu->getChildByID("prev-button"))) {
                HijackCallback::set(prevBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("tabs-prev-page");
                    orig(sender);
                });
            }
        }

        if (auto zoomMenu = getChildByID("zoom-menu")) {
            if (auto zoomInBtn = static_cast<CCMenuItem*>(zoomMenu->getChildByID("zoom-in-button"))) {
                HijackCallback::set(zoomInBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("zoom-in");
                    orig(sender);
                });
            }
            if (auto zoomOutBtn = static_cast<CCMenuItem*>(zoomMenu->getChildByID("zoom-out-button"))) {
                HijackCallback::set(zoomOutBtn, [](auto orig, auto sender) {
                    SoundHandler::get().playSound("zoom-out");
                    orig(sender);
                });
            }
        }

        if (auto toolbarMenu = getChildByID("toolbar-categories-menu")) {
            for (auto child : toolbarMenu->getChildrenExt()) {
                if (auto btn = typeinfo_cast<CCMenuItem*>(child)) {
                    HijackCallback::set(btn, [](auto orig, auto sender) {
                        SoundHandler::get().playSound("toolbar-categories");
                        orig(sender);
                    });
                }
            }
        }

        if (auto togglesMenu = getChildByID("toolbar-toggles-menu")) {
            for (auto child : togglesMenu->getChildrenExt()) {
                if (auto btn = typeinfo_cast<CCMenuItem*>(child)) {
                    HijackCallback::set(btn, [](auto orig, auto sender) {
                        SoundHandler::get().playSound("toolbar-toggles");
                        orig(sender);
                    });
                }
            }
        }
    }

    void setupCreateMenu() {
        EditorUI::setupCreateMenu();
        SoundHandler::get().setEnabled(true);
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
            SoundHandler::get().playSound("slider-tick");
        }

        m_fields->m_lastPos = xPos;
    }

    void selectObject(GameObject* p0, bool p1) {
        SoundHandler::get().playSound("select");
        EditorUI::selectObject(p0, p1);
    }

    void selectObjects(CCArray* p0, bool p1) {
        SoundHandler::get().playSound("select");
        EditorUI::selectObjects(p0, p1);
    }

    void deselectAll() {
        SoundHandler::get().playSound("deselect");
        EditorUI::deselectAll();
    }

    void deselectObject(GameObject* object) {
        SoundHandler::get().playSound("deselect");
        EditorUI::deselectObject(object);
    }

    void doCopyObjects(bool p0) {
        SoundHandler::get().playSound("copy");
        EditorUI::doCopyObjects(p0);
    }

    cocos2d::CCArray* pasteObjects(gd::string p0, bool p1, bool p2) {
        if (GameManager::get()->m_editorClipboard == p0) SoundHandler::get().playSound("paste");
        else SoundHandler::get().playSound("place-object");

        return EditorUI::pasteObjects(p0, p1, p2);
    }

    CCPoint moveForCommand(EditCommand command) {
        auto ret = EditorUI::moveForCommand(command);

        switch (command) {
            case EditCommand::Down:
                SoundHandler::get().playSound("move-down");
                break;
            case EditCommand::Up:
                SoundHandler::get().playSound("move-up");
                break;
            case EditCommand::Left:
                SoundHandler::get().playSound("move-left");
                break;
            case EditCommand::Right:
                SoundHandler::get().playSound("move-right");
                break;
            case EditCommand::HalfDown:
                SoundHandler::get().playSound("move-half-down");
                break;
            case EditCommand::HalfUp:
                SoundHandler::get().playSound("move-half-up");
                break;
            case EditCommand::HalfLeft:
                SoundHandler::get().playSound("move-half-left");
                break;
            case EditCommand::HalfRight:
                SoundHandler::get().playSound("move-half-right");
                break;
            case EditCommand::SmallDown:
                SoundHandler::get().playSound("move-small-down");
                break;
            case EditCommand::SmallUp:
                SoundHandler::get().playSound("move-small-up");
                break;
            case EditCommand::SmallLeft:
                SoundHandler::get().playSound("move-small-left");
                break;
            case EditCommand::SmallRight:
                SoundHandler::get().playSound("move-small-right");
                break;
            case EditCommand::TinyDown:
                SoundHandler::get().playSound("move-tiny-down");
                break;
            case EditCommand::TinyUp:
                SoundHandler::get().playSound("move-tiny-up");
                break;
            case EditCommand::TinyLeft:
                SoundHandler::get().playSound("move-tiny-left");
                break;
            case EditCommand::TinyRight:
                SoundHandler::get().playSound("move-tiny-right");
                break;
            case EditCommand::BigDown:
                SoundHandler::get().playSound("move-big-down");
                break;
            case EditCommand::BigUp:
                SoundHandler::get().playSound("move-big-up");
                break;
            case EditCommand::BigLeft:
                SoundHandler::get().playSound("move-big-left");
                break;
            case EditCommand::BigRight:
                SoundHandler::get().playSound("move-big-right");
                break;
            default: 
                SoundHandler::get().playSound("move");
            break;
        }
        return ret;
    }

    void transformObjectCall(EditCommand command) {
        switch (command) {
            case EditCommand::RotateCW:
                SoundHandler::get().playSound("rotate-clockwise");
                break;
            case EditCommand::RotateCCW:
                SoundHandler::get().playSound("rotate-counter-clockwise");
                break;
            case EditCommand::RotateCW45:
                SoundHandler::get().playSound("rotate-45-clockwise");
                break;
            case EditCommand::RotateCCW45:
                SoundHandler::get().playSound("rotate-45-counter-clockwise");
                break;
            case EditCommand::FlipX:
                SoundHandler::get().playSound("flip-x");
                break;
            case EditCommand::FlipY:
                SoundHandler::get().playSound("flip-y");
                break;
        default:
            SoundHandler::get().playSound("rotate");
            break;
        }

        EditorUI::transformObjectCall(command);
    }

    void onDuplicate(CCObject* sender) {
        SoundHandler::get().playSound("duplicate");
        EditorUI::onDuplicate(sender);
    }

    void undoLastAction(CCObject* p0) {
        SoundHandler::get().playSound("undo");
        EditorUI::undoLastAction(p0);
    }

    void redoLastAction(CCObject* p0) {
        SoundHandler::get().playSound("redo");
        EditorUI::redoLastAction(p0);
    }

    void onCreateButton(CCObject* sender) {
        SoundHandler::get().playSound("object-button");
        EditorUI::onCreateButton(sender);
    }

    void selectBuildTab(int tab) {
        SoundHandler::get().playSound("switch-tab");
        EditorUI::selectBuildTab(tab);
    }

    void onGroupSticky(CCObject* sender) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        if (btn && btn->m_animationEnabled) SoundHandler::get().playSound("link");
        EditorUI::onGroupSticky(sender);
    }

    void onUngroupSticky(CCObject* sender) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        if (btn && btn->m_animationEnabled) SoundHandler::get().playSound("unlink");
        EditorUI::onUngroupSticky(sender);
    }

    void onGoToBaseLayer(CCObject* sender) {
        SoundHandler::get().playSound("base-layer");
        EditorUI::onGoToBaseLayer(sender);
    }

    void onGroupDown(CCObject* sender) {
        SoundHandler::get().playSound("prev-layer");
        EditorUI::onGroupDown(sender);
    }

    void onGroupUp(CCObject* sender) {
        SoundHandler::get().playSound("next-layer");
        EditorUI::onGroupUp(sender);
    }

    void keyDown(enumKeyCodes keycode) {
        EditorUI::keyDown(keycode);

        if (auto soundRes = SoundHandler::get().getSoundByKey(keycode)) {
            auto& sound = soundRes.unwrap();
            for (auto& bindData : sound.m_soundDefaults.keys) {
                auto bindRes = bindData.getByKey(keycode);
                if (!bindRes) continue;

                auto& bind = bindRes.unwrap();

                if (bind.key != keycode) continue;
                if (CustomKeyinds::isValid(bind)) {
                    bindData.fire([&] {
                        sound.play();
                    });
                }
            }
        }
    }

    void keyUp(enumKeyCodes keycode) {
        EditorUI::keyUp(keycode);
        for (auto& [_, sound] : SoundHandler::get().m_registeredSounds) {
            for (auto& bindData : sound.m_soundDefaults.keys) {
                if (!bindData.getByKey(keycode)) continue;
                bindData.reset();
            }
        }
    }

    #ifndef GEODE_IS_ANDROID
    void onLockLayer(CCObject* sender) {
        EditorUI::onLockLayer(sender);
        if (m_editorLayer->m_currentLayer == -1) return;
        
        if (m_editorLayer->m_lockedLayers[m_editorLayer->m_currentLayer]) {
            SoundHandler::get().playSound("lock-layer");
        }
        else {
            SoundHandler::get().playSound("unlock-layer");
        }
    }
    #endif

    void onDelete(CCObject* sender) {
        auto prevCount = m_editorLayer->m_objectCount;
        EditorUI::onDelete(sender);
        if (prevCount > m_editorLayer->m_objectCount) {
            SoundHandler::get().playSound("delete");
        }
    }

    void onDeleteSelected(CCObject* sender) {
        if (areObjectsSelected(this)) SoundHandler::get().playSound("delete");
        EditorUI::onDeleteSelected(sender);
    }

    void onDeleteSelectedType(CCObject* sender) {
        if (areObjectsSelected(this)) SoundHandler::get().playSound("delete");
        EditorUI::onDeleteSelectedType(sender);
    }

    void onDeleteStartPos(CCObject* sender) {
        if (areObjectsSelected(this)) SoundHandler::get().playSound("delete");
        EditorUI::onDeleteStartPos(sender);
    }
};

class $modify(MyLevelEditorLayer, LevelEditorLayer) {

    void removeObject(GameObject* p0, bool p1) {
        if (areObjectsSelected(m_editorUI)) SoundHandler::get().playSound("delete");
        LevelEditorLayer::removeObject(p0, p1);
    }

    GameObject* createObject(int p0, CCPoint p1, bool p2) {
        SoundHandler::get().playSound("place-object");
        return LevelEditorLayer::createObject(p0, p1, p2);
    }
};

class $modify(MyEditButtonBar, EditButtonBar) {

    void loadFromItems(cocos2d::CCArray* objects, int rows, int columns, bool keepPage) {
        EditButtonBar::loadFromItems(objects, rows, columns, keepPage);

        SoundHandler::get().queue([self = Ref(this)] {
            for (auto child : self->getChildrenExt()) {
                if (auto menu = typeinfo_cast<CCMenu*>(child)) {
                    if (auto prev = menu->getChildByType<CCMenuItemSpriteExtra*>(0)) {
                        HijackCallback::set(prev, [](auto orig, auto sender) {
                            SoundHandler::get().playSound("prev-page");
                            orig(sender);
                        });
                    }
                    if (auto next = menu->getChildByType<CCMenuItemSpriteExtra*>(1)) {
                        HijackCallback::set(next, [](auto orig, auto sender) {
                            SoundHandler::get().playSound("next-page");
                            orig(sender);
                        });
                    }
                }
            }
        });
    };
};
