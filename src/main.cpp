#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
//#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/EditButtonBar.hpp>

using namespace geode::prelude;

bool initFinished = false;
bool playSelectSound = true;
bool playDeleteSound = true;
bool playAnySound = true;
std::map<std::string, bool> existingSounds;



void playSoundIfExists(std::string path, float pitch){

	if(!initFinished) return;

	if(!playAnySound) return;

	bool exists = false;

	if(existingSounds[path]){
		exists = true;
	}
	else{
		auto soundFile = ghc::filesystem::path(path);
		exists = !soundFile.empty();
	}

	if (exists){
		existingSounds[path] = true;

#ifndef GEODE_IS_ANDROID
		FMODAudioEngine::sharedEngine()->m_currentSoundChannel->setPaused(false);
		FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel->setPaused(false);
		FMODAudioEngine::sharedEngine()->m_globalChannel->setPaused(false);
		FMODAudioEngine::sharedEngine()->m_system->update();
#endif

		FMODAudioEngine::sharedEngine()->playEffectAdvanced(path, 1, 0, 1, pitch, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
	}
}

void playSoundIfExists(std::string path){
	playSoundIfExists(path, 1);
}

/*class $modify(FMODAudioEngine){
	void playEffectAdvanced(gd::string p0, float p1, float p2, float p3, float p4, bool p5, bool p6, int p7, int p8, int p9, int p10, bool p11, int p12, bool p13, bool p14, int p15, int p16, float p17, int p18){
		log::info("p0 {}", p0); //path
		log::info("p1 {}", p1); //speed
		log::info("p2 {}", p2); //? 0
		log::info("p3 {}", p3); //volume
		log::info("p4 {}", p4); //pitch
		log::info("p5 {}", p5); //fft enabled
		log::info("p6 {}", p6); //reverb enabled
		log::info("p7 {}", p7); //start
		log::info("p8 {}", p8); //end
		log::info("p9 {}", p9); //fade in
		log::info("p10 {}", p10); //fade out
		log::info("p11 {}", p11); //loop enabled
		log::info("p12 {}", p12); //? 0
		log::info("p13 {}", p13); //override
		log::info("p14 {}", p14); //? true
		log::info("p15 {}", p15); //? 0
		log::info("p16 {}", p16); //unique ID
		log::info("p17 {}", p17); //min interval
		log::info("p18 {}", p18); //sfx group
		FMODAudioEngine::playEffectAdvanced(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18);
	}
};*/

int random(int min, int max){
   static bool first = true;
   if (first) {  
      srand(time(nullptr)); 
      first = false;
   }
   return min + rand() % ((max + 1) - min);
}

class $modify(LevelEditorLayer){

	GameObject* createObject(int p0, cocos2d::CCPoint p1, bool p2){
		auto ret = LevelEditorLayer::createObject(p0, p1, p2);

		int rand = random(1, 3);

		std::string formatted = fmt::format("place_{}.ogg", rand);

		std::string path = std::string(geode::Mod::get()->expandSpriteName(formatted.c_str()));

		playSoundIfExists(path);
		return ret;
	}

	void removeObject(GameObject* p0, bool p1){
		LevelEditorLayer::removeObject(p0, p1);
		if(playDeleteSound && initFinished){
			playSoundIfExists("delete.ogg"_spr);
		}
	}
};

class $modify(EditButtonBar){

	void onLeft(cocos2d::CCObject* sender){
		EditButtonBar::onLeft(sender);
		playSoundIfExists("switchPage.ogg"_spr);
	}

	void onRight(cocos2d::CCObject* sender){
		EditButtonBar::onRight(sender);
		playSoundIfExists("switchPage.ogg"_spr);
	}
};

class $modify(EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::moveForCommand", INT_MIN);
    }

    struct Fields {
        bool m_playtesting = false;
        int m_lastPos = 0;
    };

	void onCreateObject(int p0){
		playSelectSound = false;
		EditorUI::onCreateObject(p0);
		playSelectSound = true;
	}

	bool init(LevelEditorLayer* editorLayer){
		initFinished = false;

		existingSounds.clear();

		if(!EditorUI::init(editorLayer)) return false;

		initFinished = true;

		return true;
	}

	void sliderChanged(cocos2d::CCObject* p0){
		EditorUI::sliderChanged(p0);

		Slider* slider = this->m_positionSlider;

		int currentPos = floor(slider->getThumb()->getValue()*100);

		if(currentPos % 7 == 0){
			if(m_fields->m_lastPos != currentPos){
				playSoundIfExists("sliderTick.ogg"_spr);
				m_fields->m_lastPos = currentPos;
			}
		}
	}

    void selectObject(GameObject* p0, bool p1){
		EditorUI::selectObject(p0, p1);
		if(playSelectSound){
			playSoundIfExists("select.ogg"_spr);
		}
	}

    void selectObjects(cocos2d::CCArray* p0, bool p1){
		playSelectSound = false;
		EditorUI::selectObjects(p0, p1);
		playSelectSound = true;
		
		playSoundIfExists("select.ogg"_spr);
	}

    void selectAll(){
		EditorUI::selectAll();
		playSoundIfExists("select.ogg"_spr);
	}
  
    void selectAllWithDirection(bool p0){
		EditorUI::selectAllWithDirection(p0);
		playSoundIfExists("select.ogg"_spr);
	}

	void onDeselectAll(cocos2d::CCObject* sender){
		EditorUI::onDeselectAll(sender);
		playSoundIfExists("deselect.ogg"_spr);
	}

	void deleteObject(GameObject* p0, bool p1){
		EditorUI::deleteObject(p0, p1);
		playSoundIfExists("delete.ogg"_spr);
	}

	void onDeleteSelected(cocos2d::CCObject* sender){
		playDeleteSound = false;
		EditorUI::onDeleteSelected(sender);
		playDeleteSound = true;
		playSoundIfExists("delete.ogg"_spr);
	}

	void onDeleteSelectedType(cocos2d::CCObject* sender){
		EditorUI::onDeleteSelectedType(sender);
		playSoundIfExists("delete.ogg"_spr);
	}

	void doCopyObjects(bool p0){
		EditorUI::doCopyObjects(p0);
		playSoundIfExists("copy.ogg"_spr);
	}

	void doPasteObjects(bool p0){
		EditorUI::doPasteObjects(p0);
		playSoundIfExists("paste.ogg"_spr);
	}

	void onNewCustomItem(cocos2d::CCObject* sender){
		playAnySound = false;
		EditorUI::onNewCustomItem(sender);
		playAnySound = true;
	}

	void onDeleteCustomItem(cocos2d::CCObject* sender){
		playAnySound = false;
		EditorUI::onDeleteCustomItem(sender);
		playAnySound = true;
	}

    void orderDownCustomItem(cocos2d::CCObject* sender){
		playAnySound = false;
		EditorUI::orderDownCustomItem(sender);
		playAnySound = true;
	}

    void orderUpCustomItem(cocos2d::CCObject* sender){
		playAnySound = false;
		EditorUI::orderUpCustomItem(sender);
		playAnySound = true;
	}

	void reloadCustomItems(){
		playAnySound = false;
		EditorUI::reloadCustomItems();
		playAnySound = true;
	}

	cocos2d::CCPoint moveForCommand(EditCommand command){
		auto ret = EditorUI::moveForCommand(command);

		switch(command){
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

	void transformObjectCall(EditCommand command){
		EditorUI::transformObjectCall(command);
		playSoundIfExists("rotate.ogg"_spr);
	}

	void onDuplicate(cocos2d::CCObject* sender){
		EditorUI::onDuplicate(sender);
		playSoundIfExists("duplicate.ogg"_spr);
	}

    void undoLastAction(cocos2d::CCObject* p0){
		EditorUI::undoLastAction(p0);
		playSoundIfExists("undo.ogg"_spr);
	}

	void redoLastAction(cocos2d::CCObject* p0){
		EditorUI::redoLastAction(p0);
		playSoundIfExists("redo.ogg"_spr);
	}

	void onCreateButton(cocos2d::CCObject* sender){
		EditorUI::onCreateButton(sender);
		playSoundIfExists("objectButton.ogg"_spr);
	}

	void onSelectBuildTab(cocos2d::CCObject* sender){
		EditorUI::onSelectBuildTab(sender);
		playSoundIfExists("switchTab.ogg"_spr);
	}

	void toggleMode(cocos2d::CCObject* sender){
		EditorUI::toggleMode(sender);
		playSoundIfExists("button.ogg"_spr);
	}

	void toggleSwipe(cocos2d::CCObject* p0){
		EditorUI::toggleSwipe(p0);
		playSoundIfExists("button.ogg"_spr);
	}
    
    void toggleEnableRotate(cocos2d::CCObject* p0){
		EditorUI::toggleEnableRotate(p0);
		playSoundIfExists("button.ogg"_spr);
	}
   
    void toggleFreeMove(cocos2d::CCObject* p0){
		EditorUI::toggleFreeMove(p0);
		playSoundIfExists("button.ogg"_spr);
	}

	void toggleSnap(cocos2d::CCObject* p0){
		EditorUI::toggleSnap(p0);
		playSoundIfExists("button.ogg"_spr);
	}

	void onGroupSticky(cocos2d::CCObject* sender){
		EditorUI::onGroupSticky(sender);
		playSoundIfExists("link.ogg"_spr);
	}

	void onUngroupSticky(cocos2d::CCObject* sender){
		EditorUI::onUngroupSticky(sender);
		playSoundIfExists("unlink.ogg"_spr);
	}

    void onGoToBaseLayer(cocos2d::CCObject* sender){
		EditorUI::onGoToBaseLayer(sender);
		playSoundIfExists("switchPage.ogg"_spr);
	}

    void onGroupDown(cocos2d::CCObject* sender){
		EditorUI::onGroupDown(sender);
		playSoundIfExists("switchPage.ogg"_spr);
	}

	void onGroupUp(cocos2d::CCObject* sender){
		EditorUI::onGroupUp(sender);
		playSoundIfExists("switchPage.ogg"_spr);
	}

	void onPlaytest(cocos2d::CCObject* sender){
		EditorUI::onPlaytest(sender);
		m_fields->m_playtesting = true;
	}

	void onStopPlaytest(cocos2d::CCObject* sender){
		EditorUI::onStopPlaytest(sender);
		m_fields->m_playtesting = false;
	}


	void keyDown(cocos2d::enumKeyCodes keycode){
		EditorUI::keyDown(keycode);
		if(keycode != 13 && keycode != 113 && keycode != 114 && keycode != 115 && keycode != 116 && keycode != 117){
			if(keycode == 39 && !m_fields->m_playtesting){
				playSoundIfExists("switchPage.ogg"_spr);
			}
		}
	}

#ifndef GEODE_IS_ANDROID
	void onLockLayer(cocos2d::CCObject* sender){
		EditorUI::onLockLayer(sender);
		LevelEditorLayer* levelEditorLayer = this->m_editorLayer;
		
		if(levelEditorLayer->m_currentLayer != -1){

			gd::vector<bool> lockedLayers = levelEditorLayer->m_lockedLayers;

			if(lockedLayers[levelEditorLayer->m_currentLayer]){
				playSoundIfExists("lock.ogg"_spr);
			}
			else{
				playSoundIfExists("unlock.ogg"_spr);
			}
		}
	}
#endif
#ifdef GEODE_IS_ANDROID
    void zoomGameLayer(bool p0){
		EditorUI::zoomGameLayer(p0);

		if(p0){
			playSoundIfExists("zoomIn.ogg"_spr);
		}
		else{
			playSoundIfExists("zoomOut.ogg"_spr);
		}
	}
#endif
#ifdef GEODE_IS_WINDOWS
	void zoomIn(cocos2d::CCObject* p0){
		EditorUI::zoomIn(p0);
		playSoundIfExists("zoomIn.ogg"_spr);
	}

    void zoomOut(cocos2d::CCObject* p0){
		EditorUI::zoomOut(p0);
		playSoundIfExists("zoomOut.ogg"_spr);
	}
#endif
};

