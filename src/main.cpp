#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
//#include <Geode/modify/FMODAudioEngine.hpp>

using namespace geode::prelude;

bool initFinished = false;
bool playSelectSound = true;
std::map<std::string, bool> existingSounds;



void playSoundIfExists(std::string path, float pitch){

	if(!initFinished) return;

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

float randomFloat(float min, float max) {
    float random = ((float) rand()) / (float) RAND_MAX;

    float range = max - min;  
    return (random*range) + min;
}

class $modify(LevelEditorLayer){

	GameObject* createObject(int p0, cocos2d::CCPoint p1, bool p2){
		auto ret = LevelEditorLayer::createObject(p0, p1, p2);

		//float randomPitch = randomFloat(0.95, 1.05);

		playSoundIfExists("place.ogg"_spr, 1);
		return ret;
	}
};

class $modify(EditorUI) {

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
		EditorUI::onDeleteSelected(sender);
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

	cocos2d::CCPoint moveForCommand(EditCommand command){
		auto ret = EditorUI::moveForCommand(command);
		playSoundIfExists("move.ogg"_spr);
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

	void updateZoom(float p0){
		EditorUI::updateZoom(p0);

	}

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

