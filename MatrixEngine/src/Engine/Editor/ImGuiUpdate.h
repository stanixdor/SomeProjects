#pragma once

extern bool thomasMode;

class GameObject;
namespace EngineImGUI
{
	void GizmoUpdate();
	void GizmoCursorUpdate();
	void DeleteSelectedObjects();
	void Cloning();
	void Console();
	void ResourceManagerWindow();
	float lerp(float v0, float v1, float t);
    void initialize();
	void Update();
	void mainMenu();
    void objectEditor();
	void objectsInSpace();
	void showSelectedObject();
    void ShowListOfSelectedObjects();
	void ImGuiLateUpdate();
	void cameraStuff();
	void drawBorders();
	void removeSelectedObjectsBorders();
	void EditorShortcuts();
	void editorUpdate();
	void autosave();

	void thomasModeUpdate();
}

struct EditorHacks
{
	static float Length;
};

