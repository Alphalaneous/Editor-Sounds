#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

bool initFinished = false;
bool playSelectSound = true;
std::map<std::string, bool> existingSounds;

void playSoundIfExists(std::string path){

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
			
        	FMODAudioEngine::sharedEngine()->playEffectAsync(path);
		}
	}

class $modify(LevelEditorLayer){

	GameObject* createObject(int p0, cocos2d::CCPoint p1, bool p2){
		auto ret = LevelEditorLayer::createObject(p0, p1, p2);
		playSoundIfExists("place.ogg"_spr);
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

#ifndef GEODE_IS_ANDROID

	void rotationforCommand(EditCommand command){
		EditorUI::rotationforCommand(command);
		playSoundIfExists("rotate.ogg"_spr);
	}

#endif

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

    void zoomIn(cocos2d::CCObject* p0){
		EditorUI::zoomIn(p0);
		playSoundIfExists("zoomIn.ogg"_spr);
	}

    void zoomOut(cocos2d::CCObject* p0){
		EditorUI::zoomOut(p0);
		playSoundIfExists("zoomOut.ogg"_spr);
	}

};

