#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct HijackCallback : public CCObject {

    using Hijack = std::function<void(std::function<void(CCObject* sender)> orig, CCObject* sender)>;

    Hijack m_method;
    SEL_MenuHandler m_selector;

    static HijackCallback* create(Hijack method, SEL_MenuHandler originalSelector);
    static void set(CCMenuItem* btn, Hijack method);

    bool init(Hijack method, SEL_MenuHandler originalSelector);
    void callback(CCObject* sender);
};