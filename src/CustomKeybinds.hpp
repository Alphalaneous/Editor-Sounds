#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace CustomKeyinds {

    enum class Modifier : unsigned int {
        None    = 0b0000,
        Control = 0b0001,
        Shift   = 0b0010,
        Alt     = 0b0100,
        Command = 0b1000,
    };

    inline bool operator&(unsigned int a, Modifier b) {
        return (a & static_cast<unsigned int>(b)) != 0;
    }

    struct Bind {
        enumKeyCodes key;
        bool shift;
        bool control;
        bool alt;
        bool command;
    };

    struct BindData {
        std::string id;

        std::vector<Bind> binds;

        bool hasRepeat;
        int rate;
        int delay;

        float last = 0.f;
        float lastTime;
        float firstPress = true;

        bool canFireAgain() {
            if (firstPress) {
                lastTime = CCDirector::get()->m_fAccumDt;
                last += delay / 1000.f;
                return true;
            }
            if (!hasRepeat) return false;

            auto now = CCDirector::get()->m_fAccumDt;
            auto dt = now - lastTime;
            lastTime = now;

            last -= dt;

            if (last < 0.f) {
                last += rate / 1000.f;
                return true;
            }

            return false;
        }

        void fire(std::function<void()> onFire) {
            if (canFireAgain()) {
                onFire();
            }
            firstPress = false;
        }

        void reset() {
            firstPress = true;
            last = 0.f;
        }

        void parse() {
            auto ck = Loader::get()->getLoadedMod("geode.custom-keybinds");
            if (!ck) return;

            binds.clear();

            auto value = ck->getSavedValue<matjson::Value>(id);
            auto bindsRes = value["binds"].asArray();
            if (!bindsRes) return;
            auto bindsArr = bindsRes.unwrap();

            for (const auto& bindObj : bindsArr) {
                Bind bind;
            
                bind.key = static_cast<enumKeyCodes>(bindObj["key"].asInt().unwrapOr(0));
                unsigned int modifiers = bindObj["modifiers"].asUInt().unwrapOr(0);

                bind.control = modifiers & Modifier::Control;
                bind.shift   = modifiers & Modifier::Shift;
                bind.alt     = modifiers & Modifier::Alt;
                bind.command = modifiers & Modifier::Command;
                binds.push_back(bind);
            }

            auto repeat = value["repeat"];
            if (repeat.isObject()) {
                hasRepeat = repeat["enabled"].asBool().unwrapOrDefault();
                rate = repeat["rate"].asInt().unwrapOrDefault();
                delay = repeat["delay"].asInt().unwrapOrDefault();
            }
        }

        Result<Bind&> getByKey(enumKeyCodes keyCode) {
            for (auto& bindObj : binds) {
                if (bindObj.key == keyCode) return geode::Ok(bindObj);
            }
            return geode::Err("Bind for key {} not found.", static_cast<int>(keyCode));
        }
    };

    static bool isValid(CustomKeyinds::Bind bind) {
        auto keyboard = CCKeyboardDispatcher::get();

        bool altPressed     = keyboard->getAltKeyPressed();
        bool controlPressed = keyboard->getControlKeyPressed();
        bool shiftPressed   = keyboard->getShiftKeyPressed();
        bool commandPressed = keyboard->getCommandKeyPressed();

        if (!bind.alt && !bind.control && !bind.shift && !bind.command) {
            if (altPressed || controlPressed || shiftPressed || commandPressed) {
                return false;
            }
            return true;
        }

        bool isPressed =
            (!bind.alt     || altPressed) &&
            (!bind.control || controlPressed) &&
            (!bind.shift   || shiftPressed) &&
            (!bind.command || commandPressed);
        if (isPressed) {
            return true;
        }
        return false;
    }
}