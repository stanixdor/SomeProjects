#include "ImGuiUpdate.h"
#include "GameObject\GameObject.h"
#include "imgui\imgui.h"
#include "GameObject\GameObjectManager.h"
#include "Engine\Graphics\GraphicsComponents\Renderable.h"
#include "Engine\Graphics\GraphicsSystem.h"
#include "Engine\Physics\RigidBody.h"
#include "Engine\Physics\CollisionSystem.h"
#include "Engine\Graphics\GraphicsComponents\AnimationComponent.h"
#include "Game\Components\FakeTongue.h"
#include "Game\Components\Pipe.h"
#include "Platform\Input\Input.h"
#include "Game\Components\Test.h"
#include "Engine\Graphics\GraphicsComponents\LineRenderer.h"
#include <cmath>
#include <string>
#include "imgui\imgui_internal.h"
#include "Components/Graphics/ParallaxComponent.h"
#include "Physics\Collider.h"
#include "Platform/Resource Manager/ResourceManager.h"
#include "EngineDebug\ConsolePrinting.h"
#include "Logic\Level.h"
#include "Components\PropulsionObject.h"
#include "Components\ClimbableObject.h"
#include "Components\ZClimbable.h"
#include "Components\Tongue.h"
#include "Levels\GenericLevel.h"
#include "Graphics\GraphicsSystem.h"
#include "Game\EnemiesStates\SheepStates.h"
#include "Game\EnemiesStates\WolfStates.h"
#include "Components\SteamComponent.h"
#include "Game\Logic\StateMachines\SlothStateMachine.h"
#include "Components\TestPatherino.h"
#include "Components/FlyComponent.h"
#include "Components/HeadComponent.h"
#include "Components/CamouflageComponent.h"
#include "Components\Rabbit.h"
#include "Components\Checkpoint.h"
#include "Components\Laser.h"
#include "Components\Laser2.h"
#include "Components\DetectorComponent.h"
#include "Components\ActivableComponent.h"
#include "Components\ActivObject.h"
#include "Platform\Input\Button.h"
#include "Components\CollectibleComponent.h"
#include "Components\Door.h"
#include "Graphics\GraphicsComponents\FontRenderable.h"
#include "Components\Graphics\CameraZoom.h"


bool thomasMode = false;
namespace EngineImGUI
{
	
	int CustomButton(const char* x, int i, ImVec2 y, ImVec4 a, ImVec4 b, ImVec4 c);

	static bool show_test_window = false;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static bool jokin_window_open = true;
	ImGuiWindowFlags ObjectEditorFlags = 0;
	Graphics::LineRenderer *lineRend;

	std::vector<GameObject*> selectedObjects;
	bool anyObjectSelected;
	bool showSelectedObjectsProperties = true;
	bool selecting;
	enum class GMode { Translate, Scale, Rotate, Selection };
	GMode gizmoMode;
	glm::vec2 selectedStartingPoint;
	glm::vec2 selectedEndingPoint;
	glm::vec2 PreviousMousePos;
	glm::vec2 originalStartingPos;
	glm::vec2 originalEndingPos;

	float thomasModeColor = 0;
	std::unique_ptr<GameObject> cursorGizmo;
	std::unique_ptr<GameObject> line;
	GMode cursorMode;
	Transform2D* cursorTransform;
	Renderable* renderable;
	glm::vec2 mousePos;    //this two are for dragging the camera with middle button
	glm::vec2 mousePosObjective; //this two are for dragging the camera with middle button
	GameObject* camera;

	FRC::Timer automaticSaveTimer;
	bool deselectedAnObject = false;
	bool isCameraDragging = false;
	std::string lastSave;
	Texture* selectedTexture = nullptr;
	int counter = 0;

	std::vector<glm::vec4> colorOfSelection;

	namespace windowConfig
	{
		bool lockObjectsInSpace = true;
		bool lockProperties = true;
		bool lockArchetypes = true;
		bool lockEditor = true;
		bool hideConsole = false;
		ImVec2 propertiesWindowPosition = { 1200, 20 };
		ImVec2 propertiesWindowSize;

	}
	namespace globals
	{
		GameObject * pickedObject;
		int selectedCounter = 0;
		ImVec4 inportantText = ImVec4(0.0f, 0.6f, 0.6f, 1.0f);
		ImVec4 white = ImVec4(0.0f, 0.6f, 0.6f, 1.0f);
	}
	
	// automatice cogpath function for editor:
	void CogPathToGui(CogPath & p)
	{
		for (const auto& obj : objectManager.m_gameObjects)
		{
			if (ImGui::Selectable(("SELECT  " + obj->m_name).c_str()))
			{
				p.set_id(obj->m_guid);
			}
		}
	}

	void initialize()
	{
		automaticSaveTimer.Start();
		//ImGui::SetColorEditOptions(ImGuiColorEditFlags_::ImGuiColorEditFlags_NoPicker);
		show_test_window = true;

		static bool no_titlebar = false;
		static bool no_border = true;
		static bool no_resize = false;
		static bool no_move = false;
		static bool no_scrollbar = false;
		static bool no_collapse = true;
		static bool no_menu = false;

		if (no_titlebar)  ObjectEditorFlags |= ImGuiWindowFlags_NoTitleBar;
		if (!no_border)   ObjectEditorFlags |= ImGuiWindowFlags_ShowBorders;
		if (no_resize)    ObjectEditorFlags |= ImGuiWindowFlags_NoResize;
		if (no_move)      ObjectEditorFlags |= ImGuiWindowFlags_NoMove;
		if (no_scrollbar) ObjectEditorFlags |= ImGuiWindowFlags_NoScrollbar;
		if (no_collapse)  ObjectEditorFlags |= ImGuiWindowFlags_NoCollapse;
		if (!no_menu)     ObjectEditorFlags |= ImGuiWindowFlags_MenuBar;

		anyObjectSelected = false;
		selecting = false;
		gizmoMode = GMode::Selection;
		selectedStartingPoint = { 0,0 };
		selectedEndingPoint = { 0,0 };
		PreviousMousePos = { 0,0 };



		line = std::make_unique<GameObject>("Do not touch");
		lineRend = line->AddComponent<Graphics::LineRenderer>();
		lineRend->pTransform->mZorder = 2.0f;

		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(lineRend);
		cursorGizmo = std::make_unique<GameObject>("Mouse Cursor");
		cursorGizmo->EditableByGizmos = false;
		renderable = cursorGizmo->AddComponent<Graphics::Renderable>();
		renderable->pTexture = ResourceManager::textures["Select.png"].get();
		cursorTransform = cursorGizmo->FindComponentByType<Transform2D>();
		cursorTransform->SetScale({ 20, 20 });
		cursorTransform->SetPos({ 0,0 });
		cursorTransform->mZorder = 2.f;
		renderable->pMesh = (GraphicsSystem::Instance()->pMesh);
		renderable->pShader = GraphicsSystem::Instance()->mPrograms[0];
		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(renderable);
		cursorMode = GMode::Selection;

		camera = objectManager.FindObjectByName("camera");
	}

	void Update()
	{

		if (!camera)
			camera = objectManager.FindObjectByName("camera");

		if (!playMode)
			editorUpdate();

		if (thomasMode)
			thomasModeUpdate();
		

		
		//ImGui::End();

		/*if (playMode)
		cursorGizmo->m_transform->SetScale({ 0,0 });*/
	}

	void mainMenu()
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("New Level"))
				{
					static char buf[32] = "";
					std::string folder = "Data/Levels/";
					ImGui::InputText("Enter Name", buf, IM_ARRAYSIZE(buf));
					if (ImGui::MenuItem("Create"))
					{
						/*std::string levelName = buf;
						levelName += ".level";
						objectManager.Destroy();
						objectManager.LoadFromFile("Standard Level", folder);
						objectManager.SaveToFile(levelName, folder);*/
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Save Level", "CTRL+S"))
				{
					static char buf[32] = "";
					std::string folder = "Data/Levels/";
					ImGui::InputText("Enter Name", buf, IM_ARRAYSIZE(buf));
					if (ImGui::MenuItem("Save"))
					{
						std::string levelName = buf;
						levelName += ".level";
						lastSave = levelName;
						objectManager.SaveToFile(levelName, folder);
						automaticSaveTimer.Reset();

					}
					/*std::string folder = "Data/Levels/";
					std::string LevelName = "TestLevel";
					objectManager.SaveToFile(LevelName + ".level", folder);*/

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Load Level", "Ctrl+L"))
				{
					/*std::string folder = "Data/Levels/";
					std::string LevelName = "TestLevel";
					objectManager.LoadFromFile(LevelName + ".level", folder);*/
					for (const auto& level : ResourceManager::levels)
						if (ImGui::MenuItem(("LOAD  " + level.first).c_str()))
						{
							ResetLevel(level.first, "Data/Levels/");
							EngineImGUI::initialize();
							globals::pickedObject = nullptr;
							selectedObjects.clear();
						}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Refresh Resources"))
				{
					ResourceManager::RefreshResources();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::BeginMenu("Lock/Unlock Windows"))
				{
					if (ImGui::MenuItem("Lock Object Creation Window", windowConfig::lockEditor ? "Locked" : "Unlocked"))
						windowConfig::lockEditor = !windowConfig::lockEditor;

					if (ImGui::MenuItem("Lock Objects In Level Window", windowConfig::lockObjectsInSpace ? "Locked" : "Unlocked"))
						windowConfig::lockObjectsInSpace = !windowConfig::lockObjectsInSpace;

					if (ImGui::MenuItem("Lock Properties Window", windowConfig::lockProperties ? "Locked" : "Unlocked"))
						windowConfig::lockProperties = !windowConfig::lockProperties;

					if (ImGui::MenuItem("Lock Archetypes Window", windowConfig::lockArchetypes ? "Locked" : "Unlocked"))
						windowConfig::lockArchetypes = !windowConfig::lockArchetypes;

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Hide Console"))
				{
					windowConfig::hideConsole = !windowConfig::hideConsole;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void objectEditor()
	{

		unsigned windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (windowConfig::lockEditor)
		{
			windowFlags |= ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoResize;
		}

		//security check
		if (ImGui::Begin("Editor", NULL, windowFlags))
		{
			if (ImGui::BeginMenu("Object creation"))
			{
				if (ImGui::MenuItem("Create GameObject", "Ctrl+C"))
				{
					globals::pickedObject = nullptr;
					globals::pickedObject = objectManager.AddGameObject("NewObject");
					globals::pickedObject->m_transform->SetPos(glm::vec2(0, 200));
					globals::pickedObject->m_transform->SetScale(glm::vec2(50, 50));

					/*auto* rend = object->AddComponent<Graphics::Renderable>();
					rend->pMesh = GraphicsSystem::Instance()->pMesh;
					rend->pShader = GraphicsSystem::Instance()->mPrograms[0];
					GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);*/
				}
				if (ImGui::MenuItem("Create GameObject from archetype"))
				{
					for (const auto& archetype : ResourceManager::archetypes)
						if (ImGui::BeginMenu(archetype.first.c_str()))
						{
							if (ImGui::MenuItem("LOAD"))
							{
								auto* gameObject = objectManager.AddGameObject(archetype.first, false);
								gameObject->from_json(archetype.second);
								ImGui::EndMenu();
							}		
						}
				}
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Add Components"))
			{
				if (globals::pickedObject == nullptr)
				{
					ImGui::EndMenu();
					ImGui::End();
					return;
				}
				if (ImGui::MenuItem("Renderable"))
				{
					auto* rend = globals::pickedObject->AddComponent<Graphics::Renderable>();
					GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
				}
				if (ImGui::MenuItem("Animation"))
				{
					auto* render = globals::pickedObject->AddComponent<Graphics::AnimComp>();
					//SpriteAnimationData * IdleAnim_data3 =  new SpriteAnimationData("Idle", ResourceManager::textures["agachar.png"].get(), 7, 2, 2.0f);
					//render->mAnimData = IdleAnim_data3;
					GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(render);
				}
				if (ImGui::MenuItem("FakeTongue"))
				{
					globals::pickedObject->AddComponent<FakeTongue>();
				}
				if (ImGui::MenuItem("Rigidbody"))
				{
					globals::pickedObject->AddComponent<RigidBody>();
				}
				if (ImGui::MenuItem("Propulsion"))
				{
					globals::pickedObject->AddComponent<PropulsionObject>();
				}
				if (ImGui::MenuItem("Climbable wall"))
				{
					globals::pickedObject->AddComponent<ClimbableObject>();
				}
				if (ImGui::MenuItem("Pipe"))
				{
					globals::pickedObject->AddComponent<Pipe>();
				}
				if (ImGui::MenuItem("ZClimb"))
				{
					globals::pickedObject->AddComponent<ZClimbableObject>();
				}
				if (ImGui::MenuItem("Laser"))
				{
					globals::pickedObject->AddComponent<Laser>();
				}
				if (ImGui::MenuItem("Laser2"))
				{
					globals::pickedObject->AddComponent<Laser2>();
				}
				if (ImGui::MenuItem("Collider"))
				{
					globals::pickedObject->AddComponent<Collider>();
				}
				if (ImGui::MenuItem("Checkpoint"))
				{
					globals::pickedObject->AddComponent<Checkpoint>();
				}
				if (ImGui::MenuItem("PlayerStateMachine"))
				{
					globals::pickedObject->AddComponent<PlayerStateMachine>();
				}
				if (ImGui::MenuItem("SheepStateMachine"))
				{
					globals::pickedObject->AddComponent<SheepStateMachine>();
				}
				if (ImGui::MenuItem("WolfStateMachine"))
				{
					globals::pickedObject->AddComponent<WolfStateMachine>();
				}
				if (ImGui::MenuItem("Steam"))
				{
					globals::pickedObject->AddComponent<Steam>();
				}
				if (ImGui::MenuItem("Door"))
				{
					globals::pickedObject->AddComponent<Door>();
				}
				if (ImGui::MenuItem("TestPatherino"))
				{
					globals::pickedObject->AddComponent<TestPatherino>();
				}
				if (ImGui::MenuItem("Rabbit"))
				{
					globals::pickedObject->AddComponent<Rabbit>();
				}
				if (ImGui::MenuItem("FlyComponent"))
				{
					globals::pickedObject->AddComponent<FlyComponent>();
				}
				if (ImGui::MenuItem("HeadComponent"))
				{
					globals::pickedObject->AddComponent<HeadComponent>();
				}
				if (ImGui::MenuItem("CamouflageComponent"))
				{
					globals::pickedObject->AddComponent<CamouflageComponent>();
				}
				if (ImGui::MenuItem("Detector"))
				{
					globals::pickedObject->AddComponent<Detector>();
				}
				if (ImGui::MenuItem("SlothStateMachine"))
				{
					globals::pickedObject->AddComponent<SlothStateMachine>();
				}
				if (ImGui::MenuItem("Activable"))
				{
					globals::pickedObject->AddComponent<Activable>();
				}
				if (ImGui::MenuItem("Parallax"))
				{
					globals::pickedObject->AddComponent<ParallaxComponent>();
				}
				if (ImGui::MenuItem("Button"))
				{
					globals::pickedObject->AddComponent<Button>();
				}
				if (ImGui::MenuItem("CollectibleComponent"))
				{
					globals::pickedObject->AddComponent<CollectibleComponent>();
				}
				if (ImGui::MenuItem("Font Renderer"))
				{
					globals::pickedObject->AddComponent<Graphics::FontRenderer>();
				}
				if (ImGui::MenuItem("Camera Zoom"))
				{
					globals::pickedObject->AddComponent<CamZoom>();
				}
				//if (ImGui::MenuItem("Collider"))
				//{
				//	globals::pickedObject->AddComponent<Collider>();
				//}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Remove Selected Object"))
			{
				if (ImGui::Button("Remove Selected Object"))
				{
					if (globals::pickedObject != nullptr && globals::pickedObject->m_name != "camera")
					{
						globals::pickedObject->Destroy();
						globals::pickedObject = nullptr;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::End();
		}
	}


	void objectsInSpace()
	{
		ImGui::SetNextWindowPos(ImVec2(1650, 700), ImGuiCond_FirstUseEver);
		unsigned windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (windowConfig::lockObjectsInSpace)
		{
			windowFlags |= ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoResize;
		}
		if (ImGui::Begin("Objects", NULL, windowFlags))
		{
			int objectCounter = 1;
			static ImGuiTextFilter objectFilter;

			objectFilter.Draw();
			for (const auto& gameObject : objectManager.m_gameObjects)
			{
				
				if (objectFilter.PassFilter(gameObject->m_name.c_str()))
				{

					//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 1.f, 0.f));
					//ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.f, 1.f));
					if (gameObject.get() != nullptr)
					{
						if (ImGui::Selectable((gameObject->m_name + "##" + std::to_string(objectCounter)).c_str(), globals::selectedCounter == objectCounter))
						{
							globals::pickedObject = nullptr;
							if (globals::selectedCounter == objectCounter)

							{
								globals::selectedCounter = 0;
								globals::pickedObject = nullptr;
							}
							else
							{
								selectedObjects.clear();
								globals::selectedCounter = objectCounter;
								globals::pickedObject = gameObject.get();
								selectedObjects.push_back(globals::pickedObject);
							}
						}
						objectCounter++;
					}
				}
			}


			ImGui::End();
		}
	}

	void showSelectedObject()
	{

		unsigned windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (windowConfig::lockProperties)
		{
			windowFlags |= ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoResize;
			ImGui::SetNextWindowPos(windowConfig::propertiesWindowPosition);
			ImGui::SetNextWindowSize(windowConfig::propertiesWindowSize);
		}

		//ImGui::SetNextWindowSize({ 500,600 });
		if (globals::pickedObject != nullptr)
		{
			if (ImGui::Begin(("properties of " + globals::pickedObject->m_name).c_str(), NULL, windowFlags))
			{
				if (!windowConfig::lockProperties)
				{
					windowConfig::propertiesWindowPosition = ImGui::GetWindowPos();
					windowConfig::propertiesWindowSize = ImGui::GetWindowSize();
				}
				if (ImGui::Button("Save as archetype"))
				{
					globals::pickedObject->save_archetype(globals::pickedObject->m_name + ".archetype", "Data/Archetypes/");
				}

				char *changedName = new char[50];

				char temp[50];

				strcpy_s(changedName, sizeof(temp), globals::pickedObject->m_name.c_str());

				if (ImGui::InputText("Name: ", changedName, IM_ARRAYSIZE(temp), 0 | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					windowConfig::propertiesWindowPosition = ImGui::GetWindowPos();
					windowConfig::propertiesWindowSize = ImGui::GetWindowSize();
					windowConfig::lockProperties = true;
					globals::pickedObject->m_name = changedName;
				}
				//NO LEAKERINO IN THE CODERINO
				delete[] changedName;
				//remove warning
				ENGINE_UNUSED(temp);

				ImGui::CollapsingHeader("Object Mode: ");
				{
					if (ImGui::Selectable("Default"))
						globals::pickedObject->mode = GameObjectMode::Default;

					if (ImGui::Selectable("Menu"))
						globals::pickedObject->mode = GameObjectMode::Menu;

					if (ImGui::Selectable("HUD"))
						globals::pickedObject->mode = GameObjectMode::HUD;
				}
				int componentCount = 0;
				for (auto& currentComponent : globals::pickedObject->m_components)
				{
					componentCount++;
					std::string componentName = currentComponent->m_name;
					componentName += "##";
					componentName += std::to_string(componentCount);
					
					if (currentComponent->m_name == "Transform")
					{
						if (ImGui::CollapsingHeader(componentName.c_str()))
						{
							ImGui::TreePush(componentName.c_str());
							ImGui::Unindent();

							Transform2D* transform = static_cast<Transform2D*>(currentComponent.get());

							static int clicked = 0;
							if (ImGui::Button("Bring object here"))
							{
								transform->SetPos(glm::vec2{ GraphicsSystem::Instance()->mSpriteLayer[0]->mCameraLayer->CameraPos().x, GraphicsSystem::Instance()->mSpriteLayer[0]->mCameraLayer->CameraPos().y });
							}

							//X Y and Z values
							float vec4a[3] = { transform->GetPos().x, transform->GetPos().y, transform->mZorder };
							ImGui::SliderFloat3("Position (X, Y, Z)", vec4a, -100000.0f, 100000.0f);
							transform->SetPos(glm::vec2{ vec4a[0], vec4a[1] });
							transform->mZorder = vec4a[2];

							float vec3a[2] = { transform->GetScale().x, transform->GetScale().y };
							ImGui::SliderFloat2("Scale", vec3a, -100000.0f, 100000.0f);
							transform->SetScale({ vec3a[0], vec3a[1] });
							
							float angleRad = transform->GetRotation();
							if(ImGui::SliderAngle("Rotation", &angleRad))
							transform->SetAngle(angleRad);

							float angle = glm::degrees(transform->GetRotation());
							if(ImGui::InputFloat("Fixed rotation", &angle))
							transform->SetAngle(glm::radians(angle));


							ImGui::Indent();
							ImGui::TreePop();
						}
						continue;
					}

					if (currentComponent->m_name == "Line Renderer")
					{
						if(ImGui::CollapsingHeader(componentName.c_str()))
						{

						}
						continue;
					}
					if (ImGui::CollapsingHeader(componentName.c_str(), &(currentComponent->m_alive)))
					{
						ImGui::TreePush(componentName.c_str());
						ImGui::Unindent();

						if (currentComponent->m_name == "Child Transform")
						{
							//X Y and Z values
							Transform2D* transform = static_cast<Transform2D*>(currentComponent.get());
							float vec4a[3] = { transform->GetPos().x, transform->GetPos().y, transform->mZorder };
							ImGui::InputFloat3("Position (X, Y, Z)", vec4a);
							transform->SetPos(glm::vec2{ vec4a[0], vec4a[1] });
							transform->mZorder = vec4a[2];


							float vec3a[2] = { transform->GetScale().x, transform->GetScale().y };
							ImGui::InputFloat2("Scale", vec3a);
							transform->SetScale({ vec3a[0], vec3a[1] });

							float angle = transform->GetRotation();
							ImGui::SliderAngle("Rotation", &angle);
							transform->SetAngle(angle);
						}

						//rigidbody would go here
						else if (currentComponent->m_name == "RigidBody")
						{
							RigidBody* body = static_cast<RigidBody*>(currentComponent.get());

							float velocity[2] = { body->mVelocity.x, body->mVelocity.y };
							ImGui::InputFloat2("Velocity", velocity);
							body->mVelocity.x = velocity[0];
							body->mVelocity.y = velocity[1];

							float acceleration[2] = { body->mAcceleration.x, body->mAcceleration.y };
							ImGui::InputFloat2("Accelleration", velocity);
							body->mAcceleration.x = acceleration[0];
							body->mAcceleration.y = acceleration[1];

							float gravity[2] = { body->mGravity.x, body->mGravity.y };
							ImGui::InputFloat2("Gravity", gravity);
							body->mGravity.x = gravity[0];
							body->mGravity.y = gravity[1];

							float inverseMass = body->mInvMass;
							ImGui::InputFloat("Inverse Mass", &inverseMass);
							body->mInvMass = inverseMass;

							float drag = body->mDrag;
							ImGui::InputFloat("Drag Forces", &drag);
							body->mDrag = drag;

							float rest = body->mRestitution;
							ImGui::SliderFloat("Force Restitution (not working atm)", &rest, 0.0f, 1.0f);
							if (ImGui::IsItemHovered())
								ImGui::SetTooltip("Not working right now");
							body->mRestitution = rest;

							ImGui::Checkbox("Affected by gravity", &body->mAffectedByGravity);

							ImGui::Checkbox("Static", &body->isStatic);

							ImGui::Checkbox("Bouncyness", &body->isBouncy);
						}

						else if ((currentComponent->m_name == "Collider"))
						{
							//X Y and Z values

							Collider* collider = static_cast<Collider*>(currentComponent.get());
							float offset[2] = { collider->colliderTransform.mPosition.x,collider->colliderTransform.mPosition.y };
							ImGui::InputFloat2("Position (X, Y)", offset);

							collider->colliderTransform.SetPos( (glm::vec2{ offset[0], offset[1] }) );

							float offsetScale[2] = { collider->colliderTransform.mScale.x,collider->colliderTransform.mScale.y };
							ImGui::InputFloat2("Scale Modifier", offsetScale);

							ImGui::Checkbox("Is Ghost", &collider->isGhost);

							collider->colliderTransform.SetScale((glm::vec2{ offsetScale[0], offsetScale[1] }));

							if (ImGui::CollapsingHeader("Collision Groups"))
							{
								ImGui::TextColored(globals::white, "Actual collision group "); ImGui::SameLine();
								ImGui::TextColored(globals::inportantText, (collider->collisionGroupName).c_str());
								for (auto& currentCollisionGroup : CollisionGroup::collisionGroups)
								{
									if (ImGui::Selectable(("SELECT  " + currentCollisionGroup.first).c_str()))
									{
										globals::pickedObject->FindComponentByType<Collider>()->addCollisionGroup(currentCollisionGroup.second);
									}
								}
							}
						}
						else if (currentComponent->m_name == "Renderable")
						{
							auto *rend = globals::pickedObject->FindComponentByType<Graphics::Renderable>();
							static ImGuiTextFilter filter;
							if (ImGui::CollapsingHeader("Textures"))
							{
								ImGui::TextColored(globals::white, "Actual texture "); ImGui::SameLine();
								ImGui::TextColored(globals::inportantText, (rend->mTextureName).c_str());
								filter.Draw();
								for (const auto& texture : ResourceManager::textures) {
									if (filter.PassFilter(texture.first.c_str())) {
										if (ImGui::Selectable((texture.first).c_str()))
										{
											rend->pTexture = texture.second.get();
											rend->mTextureName = texture.first;
											rend->setScaleOfTransform(static_cast<GLfloat>(rend->pTexture->GetWidth()), static_cast<GLfloat>(rend->pTexture->GetHeight()));
										}
									}
								}
							}
							else
							{
								filter.Clear();
							}
							//color
							glm::vec4 color = rend->getColor();
							GLfloat ObjectColor[4] = { color.x, color.y, color.z, color.w };
							ImGui::ColorEdit4("Color", ObjectColor, 0);
							rend->setColor(glm::vec4(ObjectColor[0], ObjectColor[1], ObjectColor[2], ObjectColor[3]));
							rend->setOriginalColor(glm::vec4(ObjectColor[0], ObjectColor[1], ObjectColor[2], ObjectColor[3]));
						}
						else if (currentComponent->m_name == "Animation Component")
						{
							auto * anim = globals::pickedObject->FindComponentByType<Graphics::AnimComp>();
							static ImGuiTextFilter filterAnim;
							if (ImGui::CollapsingHeader("Textures"))
							{
								ImGui::TextColored(globals::white, "Actual texture "); ImGui::SameLine();
								ImGui::TextColored(globals::inportantText, (anim->mTextureName).c_str());
								filterAnim.Draw();
								for (const auto& texture : ResourceManager::textures) {
									if (filterAnim.PassFilter(texture.first.c_str())) {
										if (ImGui::Selectable((texture.first).c_str()))
										{
											anim->pTexture = texture.second.get();
											//globals::pickedObject->FindComponentByType<Graphics::AnimComp>()->mAnimData->mAtlas = texture.second.get();
											anim->mTextureName = texture.first;
											anim->mAtlasName = texture.first;
											anim->setScaleOfTransform(static_cast<GLfloat>(anim->pTexture->GetWidth()), static_cast<GLfloat>(anim->pTexture->GetHeight()));
										}
									}
								}
							}
							else
							{
								filterAnim.Clear();
							}
							//color
							glm::vec4 color = anim->getColor();
							GLfloat ObjectColor[4] = { color.x, color.y, color.z, color.w };
							ImGui::SliderFloat4("Color", ObjectColor, 0.0f, 1.0f);
							anim->setColor(glm::vec4(ObjectColor[0], ObjectColor[1], ObjectColor[2], ObjectColor[3]));
							anim->setOriginalColor(glm::vec4(ObjectColor[0], ObjectColor[1], ObjectColor[2], ObjectColor[3]));

							ImGui::SliderInt("Rows", &anim->CompRows, 1, 20);
							ImGui::SliderInt("Columns", &anim->CompColumns, 1, 20);
							anim->CompSprites = anim->CompRows * anim->CompColumns;
							anim->CompDelays.resize(anim->CompRows * anim->CompColumns);


							unsigned int size = static_cast<unsigned int>(anim->CompDelays.size());

							float allDelays = 0;
							if(ImGui::InputFloat("General delay: ", &allDelays))
							{
								for (unsigned int i = 0; i < size; i++)
								{
									anim->CompDelays[i] = allDelays;
								}
							}
							for (unsigned int i = 0; i < size; i++)
							{
								float result = anim->CompDelays[i];
								ImGui::SliderFloat(("Delay frame" + std::to_string(i)).c_str(), &result, 0, 5);
								anim->CompDelays[i] = result;
							}

						}
						else if (currentComponent->m_name == "Font Renderer")
						{
							auto * font = globals::pickedObject->FindComponentByType<Graphics::FontRenderer>();


							if (ImGui::TreeNode("Multi-line Text Input"))
							{
								char *text = new char[500];

								char tem2p[500];

								strcpy_s(text, sizeof(tem2p), font->getText().c_str());

								ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
								ImGui::PopStyleVar();
								ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(tem2p), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);

								font->setText(text);
								delete[] text;

								ENGINE_UNUSED(tem2p);

								ImGui::TreePop();
							}
							glm::vec4 color = font->getColor();
							GLfloat ObjectColor[4] = { color.x, color.y, color.z, color.w };
							if (ImGui::ColorEdit4("Color of Font", ObjectColor, 0))
							{
								font->originalColor = glm::vec4(ObjectColor[0], ObjectColor[1], ObjectColor[2], ObjectColor[3]);
								font->setColor(font->originalColor);
							}
						}
						else if (currentComponent->m_name == "SheepStateMachine")
						{
							auto * sheep = globals::pickedObject->FindComponentByType<SheepStateMachine>();
							Transform2D* transform = globals::pickedObject->FindComponentByType<Transform2D>();

							if (ImGui::Button("Init position here?"))
							{
								sheep->mInitGen.x = transform->GetPos().x;
								sheep->mInitGen.y = transform->GetPos().y;
							}

							ImGui::InputFloat("DetectionDistance", &sheep->mDetectionDis);
							ImGui::InputFloat("TimeForIdle", &sheep->mTimeForIdle);
							ImGui::InputFloat("TimeForPatrol", &sheep->mTimeForPatrol);
							ImGui::InputFloat("TimeForDetection", &sheep->mTimeForDetect);
							ImGui::InputFloat("MovingRange", &sheep->mMaxMovingRange);
							ImGui::InputFloat("Velocity", &sheep->mVelocity);

							if (ImGui::CollapsingHeader("Player pointer"))
							{
								CogPathToGui(reinterpret_cast<SheepStateMachine*>(currentComponent.get())->cg1);
							}
						}
						else if (currentComponent->m_name == "WolfStateMachine")
						{
							auto * wolf = globals::pickedObject->FindComponentByType<WolfStateMachine>();

							ImGui::InputFloat("DetectionDistance", &wolf->mDetectionDis);
							ImGui::InputFloat("TimeForIdle", &wolf->mTimeForIdle);
							ImGui::InputFloat("TimeForPatrol", &wolf->mTimeForPatrol);
							ImGui::InputFloat("TimeForDisconcert", &wolf->mTimeDisconcerted);
							ImGui::InputFloat("MovingRange", &wolf->mMaxMovingRange);
							ImGui::InputFloat("Velocity", &wolf->mVelocity);
							ImGui::InputFloat("JumpHeight", &wolf->mJumpHeight);

							float vec3a[2] = { wolf->mInitGen.x, wolf->mInitGen.y };
							ImGui::InputFloat2("InitPos", vec3a);
							wolf->mInitGen.x = vec3a[0];
							wolf->mInitGen.y = vec3a[1];

							if (ImGui::CollapsingHeader("Player pointer"))
							{
								CogPathToGui(reinterpret_cast<WolfStateMachine*>(currentComponent.get())->cg1);
							}
						}
						else if (currentComponent->m_name == "FlyComponent")
						{
							auto * fly = globals::pickedObject->FindComponentByType<FlyComponent>();
							Transform2D* transform = globals::pickedObject->FindComponentByType<Transform2D>();

							if (ImGui::Button("Init position here?"))
							{
								fly->InitialPosition.x = transform->GetPos().x;
								fly->InitialPosition.y = transform->GetPos().y;
							}
						}
						else if (currentComponent->m_name == "TestPatherino")
						{
							if (ImGui::CollapsingHeader("Select target object"))
							{
								CogPathToGui(reinterpret_cast<TestPatherino*>(currentComponent.get())->dummyPath);
							}
						}
						else if (currentComponent->m_name == "Detector")
						{
							if (ImGui::CollapsingHeader("Select target object"))
							{
								auto * actComp = globals::pickedObject->FindComponentByType<Detector>();
								CogPath & p = actComp->pathDetector;
								char buffer[256];
								GameObject * curr_obj = p.FindObject();
								if (curr_obj)
									sprintf_s(buffer, "Currently pointing: %s##%d", curr_obj->m_name.c_str());
								else
									sprintf_s(buffer, "Currently pointing: null##%d");

								// List of all objects with an ActivObject component to point at
								if (ImGui::TreeNode(buffer))
								{
									for (const auto & obj : objectManager.m_gameObjects)
									{
										if (obj->FindComponentByType<SlothStateMachine>() && ImGui::Selectable(("SELECT  " + obj->m_name).c_str()))
										{
											p.set_id(obj->m_guid);
											//actComp->pathDetector.set_id(globals::pickedObject->m_guid);
										}
									}

									ImGui::TreePop();
								}

								//CogPathToGui(reinterpret_cast<Detector*>(currentComponent.get())->pathDetector);
							}
						}
						else if (currentComponent->m_name == "Pipe System")
						{
							auto * pipe = globals::pickedObject->FindComponentByType<Pipe>();
							CogPath & p = pipe->exitCogPath;
							// Message with the name of the object is currently pointing at (or null if no object)
							char buffer[256];
							GameObject * curr_obj = p.FindObject();
							if (curr_obj)
								sprintf_s(buffer, "Currently pointing: %s##%d", curr_obj->m_name.c_str());
							else
								sprintf_s(buffer, "Currently pointing: null##%d");

							// List of all objects with an ActivObject component to point at
							if (ImGui::TreeNode(buffer))
							{
								for (const auto & obj : objectManager.m_gameObjects)
								{
									if (obj->FindComponentByType<Pipe>() && ImGui::Selectable(("SELECT  " + obj->m_name).c_str()))
									{
										p.set_id(obj->m_guid);
										pipe->exitPipe = obj.get();
										obj->FindComponentByType<Pipe>()->exitCogPath.set_id(globals::pickedObject->m_guid);
										obj->FindComponentByType<Pipe>()->exitPipe = globals::pickedObject;
									}
								}

								ImGui::TreePop();
							}
						}
						else if (currentComponent->m_name == "Activable")
						{
							auto * actComp = globals::pickedObject->FindComponentByType<Activable>();

							ImGui::Checkbox("Use Timer", &actComp->mbUseTimer);
							ImGui::SameLine();
							ImGui::InputFloat("Timer Duration", &actComp->mTimerDuration);
							ImGui::SameLine();
							ImGui::Checkbox("Repeat", &actComp->mbTimerRepeat);

							// Button to add and remove items from the vector
							int size = static_cast<int>(actComp->m_activableCogPaths.size());
							if (ImGui::InputInt("Number of paths", &size))
							{
								// Clamp size at 0 because we can't allocate 
								if (size < 0)
									size = 0;
								// Resize to new size
								actComp->m_activableCogPaths.resize(size);
							}
							// For each path in the vector
							for (size_t i = 0; i < actComp->m_activableCogPaths.size(); ++i)
							{
								CogPath & p = actComp->m_activableCogPaths[i];
								// Message with the name of the object is currently pointing at (or null if no object)
								char buffer[256];
								GameObject * curr_obj = p.FindObject();
								if (curr_obj)
									sprintf_s(buffer, "Currently pointing: %s##%d", curr_obj->m_name.c_str(), (int)i);
								else
									sprintf_s(buffer, "Currently pointing: null##%d", (int)i);
								
								// List of all objects with an ActivObject component to point at
								if (ImGui::TreeNode(buffer))
								{
									for (const auto & obj : objectManager.m_gameObjects)
									{
										if (obj->FindComponentByType<ActivObject>() && ImGui::Selectable(("SELECT  " + obj->m_name).c_str()))
											p.set_id(obj->m_guid);
									}

									ImGui::TreePop();
								}

								/*
								if (ImGui::CollapsingHeader("Select target object"))
								{
									auto * actComp = globals::pickedObject->FindComponentByType<Detector>();
									CogPath & p = actComp->pathDetector;
									char buffer[256];
									GameObject * curr_obj = p.FindObject();
									if (curr_obj)
										sprintf_s(buffer, "Currently pointing: %s##%d", curr_obj->m_name.c_str());
									else
										sprintf_s(buffer, "Currently pointing: null##%d");

									// List of all objects with an ActivObject component to point at
									if (ImGui::TreeNode(buffer))
									{
										for (const auto & obj : objectManager.m_gameObjects)
										{
											if (obj->FindComponentByType<SlothStateMachine>() && ImGui::Selectable(("SELECT  " + obj->m_name).c_str()))
											{
												p.set_id(obj->m_guid);
												//actComp->pathDetector.set_id(globals::pickedObject->m_guid);
											}
										}

										ImGui::TreePop();
									}
								*/

								/*
								for (const auto & obj : objectManager.m_gameObjects)
								{
									if (ImGui::Selectable(("SELECT  " + obj->m_name).c_str()))
									{
										p.set_id(obj->m_guid);
									}
								}
								*/
							}
						}
						else if (currentComponent->m_name == "Parallax")
						{
							ParallaxComponent* parallax = static_cast<ParallaxComponent*>(currentComponent.get());
							float distance = parallax->minDistanceToActivate;
							ImGui::InputFloat("Minimum distance to activate", &distance);
							parallax->minDistanceToActivate = distance;

							float modifier = parallax->effectModifier;
							ImGui::InputFloat("Effect Modifier", &modifier);
							parallax->effectModifier = modifier;
						}
						else if (currentComponent->m_name == "Camera Zoom")
						{
							CamZoom* zoom = static_cast<CamZoom*>(currentComponent.get());

							glm::vec2 pos = zoom->end_pos;
							ImGui::InputFloat("Ending x: ", &pos.x);
							ImGui::InputFloat("Ending y: ", &pos.y);
							zoom->end_pos = pos;
						}
						else if (currentComponent->m_name == "Button Component")
						{

							static ImGuiTextFilter filter;
							Button* button = static_cast<Button*>(currentComponent.get());
							if (ImGui::CollapsingHeader("Levels"))
							{
								filter.Draw();
								
								for (const auto& level : ResourceManager::levels) {
									if (filter.PassFilter(level.first.c_str())) {
										if (ImGui::Selectable((level.first).c_str()))
										{
											button->levelName = level.first;
										}
									}
								}
							}

							float amount = button->expansionLimits;
							ImGui::InputFloat("Minimum distance to activate", &amount);
							button->expansionLimits = amount;

							float speed = button->expansionSpeed;
							ImGui::InputFloat("Speed", &speed);
							button->expansionSpeed = speed;
							
							//original color
							glm::vec4 color = button->originalColor;
							GLfloat ObjectColor[4] = { color.x, color.y, color.z, color.w };
							ImGui::ColorEdit4("Default Color", ObjectColor, 0);
							button->originalColor = (glm::vec4(ObjectColor[0], ObjectColor[1], ObjectColor[2], ObjectColor[3]));

							glm::vec4 hovered = button->buttonHoveredColor;
							GLfloat ObjectHoveredColor[4] = { hovered.x, hovered.y, hovered.z, hovered.w };
							ImGui::ColorEdit4("Button Hovered Color", ObjectHoveredColor, 0);
							button->buttonHoveredColor = (glm::vec4(ObjectHoveredColor[0], ObjectHoveredColor[1], ObjectHoveredColor[2], ObjectHoveredColor[3]));

							glm::vec4 pressedColor = button->buttonClickedColor;
							GLfloat ObjectClickedColor[4] = { pressedColor.x, pressedColor.y, pressedColor.z, pressedColor.w };
							ImGui::ColorEdit4("Button Clicked Color", ObjectClickedColor, 0);
							button->buttonClickedColor = (glm::vec4(ObjectClickedColor[0], ObjectClickedColor[1], ObjectClickedColor[2], ObjectClickedColor[3]));

							//Button* button = static_cast<Button*>(currentComponent.get());
							//char *newLevelName = new char[50];
							//
							//char tempBuffer[50];
							//
							//strcpy_s(changedName, sizeof(tempBuffer), button->levelName.c_str());
							//
							//if (ImGui::InputText("Name: ", newLevelName, IM_ARRAYSIZE(tempBuffer), 0 | ImGuiInputTextFlags_EnterReturnsTrue))
							//	button->levelName = changedName;
							//
							////NO LEAKERINO IN THE CODERINO
							//delete[] newLevelName;
							////remove warning
							//ENGINE_UNUSED(tempBuffer);
						}
						else if (currentComponent->m_name == "player state machine")
						{
							Transform2D* transform = globals::pickedObject->FindComponentByType<Transform2D>();

							PlayerStateMachine * playerStateMachine = static_cast<PlayerStateMachine *>(currentComponent.get());
							if (ImGui::Button("Init position here?"))
							{
								playerStateMachine->InitialPosition = transform->GetPos();
							}
						}
						ImGui::Indent();
						ImGui::TreePop();
					}

				}
				ImGui::End();
			}

		}
		else if (ImGui::Begin("Properties", NULL, windowFlags))
		{
			windowConfig::propertiesWindowPosition = ImGui::GetWindowPos();
			windowConfig::propertiesWindowSize = ImGui::GetWindowSize();

			ImGui::End();
		}
	}
	//
	void GizmoUpdate()
	{
		glm::vec2 currentMousePos = Input::Instance()->ScreenToWorld();

		if (ImGui::GetCurrentContext()->HoveredWindow)
		{
			if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
			{
				originalStartingPos = currentMousePos;
				PreviousMousePos = currentMousePos;

			}
			return;
		}

		if (Input::Instance()->KeyTriggered('T'))
			gizmoMode = GMode::Selection;
		else if (Input::Instance()->KeyTriggered('Q'))
			gizmoMode = GMode::Translate;
		else if (Input::Instance()->KeyTriggered('S'))
			gizmoMode = GMode::Scale;
		else if (Input::Instance()->KeyTriggered('R'))
			gizmoMode = GMode::Rotate;

		lineRend->Flush();

		switch (gizmoMode)
		{
		case GMode::Selection:
		{
			if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
			{
				bool collidedWithAnything = false;
				for (auto& object : objectManager.m_gameObjects)
				{
					if (object->EditableByGizmos)
					{
						if (StaticRectToStaticRect(Input::Instance()->ScreenToWorld(), 1, 1, object->m_transform->GetPos(), object->m_transform->GetScale().x, object->m_transform->GetScale().y))
						{
							auto* temp = object->FindComponentByType<Graphics::LineRenderer>();
							if (temp != nullptr)
								temp->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
							selectedObjects.push_back(object.get());

							collidedWithAnything = true;
						}
					}
				}
				if (!collidedWithAnything)
				{
					globals::pickedObject = nullptr;
					deselectedAnObject = true;
				}
			}
			if (Input::Instance()->MousePressed(Input::eMouseLeft))
			{
				if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
				{
					selectedStartingPoint = Input::Instance()->ScreenToWorld();
				}
				selecting = true;
				selectedEndingPoint = Input::Instance()->ScreenToWorld();
				lineRend->DrawRect(selectedStartingPoint, selectedEndingPoint);
			}
			if (!Input::Instance()->MousePressed(Input::eMouseLeft) && selecting)
			{

				deselectedAnObject = false;
				selecting = false;
				selectedEndingPoint = Input::Instance()->ScreenToWorld();

				removeSelectedObjectsBorders();

				selectedObjects.clear();
				auto mousePosition = glm::vec2((selectedStartingPoint + selectedEndingPoint) / 2.f);
				auto width = abs(selectedStartingPoint.x - selectedEndingPoint.x);
				auto height = abs(selectedStartingPoint.y - selectedEndingPoint.y);
				for (auto& object : objectManager.m_gameObjects)
				{
					if (object->EditableByGizmos)
					{
						if (StaticRectToStaticRect(mousePosition, width, height, object->m_transform->GetPos(), object->m_transform->GetScale().x, object->m_transform->GetScale().y))
						{
							auto* temp = object->FindComponentByType<Graphics::LineRenderer>();

							if (temp != nullptr)
								temp->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
							selectedObjects.push_back(object.get());
						}
					}
				}


			}
			if (!Input::Instance()->MousePressed(Input::eMouseLeft) && !Input::Instance()->MouseTriggered(Input::eMouseLeft))
			{
				selectedStartingPoint = Input::Instance()->ScreenToWorld();
			}
			break;
		}
		case GMode::Translate:
		{

			if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
			{
				originalStartingPos = currentMousePos;
			}

			if (Input::Instance()->MousePressed(Input::eMouseLeft))
			{
				glm::vec2 deltaMovement = currentMousePos - PreviousMousePos;
				//glm::vec2 xAxis = glm::vec2()
				float distanceX = glm::distance(glm::vec2(currentMousePos.x, 0), glm::vec2(originalStartingPos.x, 0));
				float distanceY = glm::distance(glm::vec2(0, currentMousePos.y), glm::vec2(0, originalStartingPos.y));

				if (Input::Instance()->KeyPressed(Input::eShift))
				{
					if (distanceX >= distanceY)
						deltaMovement.y = 0;

					else if (distanceX < distanceY)
						deltaMovement.x = 0;
				}

				if (globals::pickedObject != nullptr)
				{
					globals::pickedObject->m_transform->SetPos(globals::pickedObject->m_transform->GetPos() + deltaMovement);
					break;
				}
				for (auto& object : selectedObjects)
				{
					object->m_transform->SetPos(object->m_transform->GetPos() + deltaMovement);
				}
			}
			break;
		}
		case GMode::Scale:
		{
			if (Input::Instance()->MousePressed(Input::eMouseLeft))
			{
				float deltaScale = (currentMousePos.x - PreviousMousePos.x) * 0.001f;
				for (auto& object : selectedObjects)
				{
					object->m_transform->SetScale(object->m_transform->GetScale() + (object->m_transform->GetScale() * deltaScale));
				}
			}
			break;
		}
		case GMode::Rotate:
		{
			if (Input::Instance()->MousePressed(Input::eMouseLeft))
			{
				float deltaRotate = (currentMousePos.x - PreviousMousePos.x) * 0.001f;
				for (auto& object : selectedObjects)
				{
					object->m_transform->SetAngle(object->m_transform->GetRotation() + deltaRotate);
				}
			}
			break;
		}
		}
		PreviousMousePos = currentMousePos;



	}

	void GizmoCursorUpdate()
	{
		if (!camera)
			camera = objectManager.FindObjectByName("camera");
		if (Input::Instance()->KeyTriggered('L'))
		{
			for (auto& obj : objectManager.m_gameObjects)
				if (obj.get()->m_name == "background")
					obj.get()->Destroy();
		}
		if (!isCameraDragging)
		{
			if (Input::Instance()->MouseTriggered(Input::eMouseMiddle))
			{
				isCameraDragging = true;
				mousePosObjective = Input::Instance()->ScreenToWorld();

			}
			else
			{
				mousePos = camera->m_transform->GetPos();
				mousePosObjective = camera->m_transform->GetPos();
			}
		}
		else if (isCameraDragging)
		{
			mousePos = Input::Instance()->ScreenToWorld();
			camera->m_transform->translate(mousePosObjective - mousePos);

			if (!Input::Instance()->MousePressed(Input::eMouseMiddle))
				isCameraDragging = false;
		}


		static bool changed = false;

		if (cursorMode != gizmoMode && !changed)
		{
			cursorGizmo->m_transform->SetScale({ lerp(cursorGizmo->m_transform->GetScale().x,0.f,0.4f), lerp(cursorGizmo->m_transform->GetScale().y,0.f,0.4f) });
			if (cursorGizmo->m_transform->GetScale().x < 0.2f)
			{
				changed = true;
				cursorGizmo->m_transform->SetAngle(0.f);
				if (gizmoMode == GMode::Selection)
				{
					renderable->pTexture = ResourceManager::textures["Select.png"].get();
				}
				else if (gizmoMode == GMode::Translate)
				{
					renderable->pTexture = ResourceManager::textures["Translate.png"].get();
				}
				else if (gizmoMode == GMode::Scale)
				{
					renderable->pTexture = ResourceManager::textures["Scale.png"].get();
				}
				else if (gizmoMode == GMode::Rotate)
				{
					renderable->pTexture = ResourceManager::textures["Rotate.png"].get();
				}
			}
		}
		else if (cursorMode != gizmoMode)
		{
			cursorGizmo->m_transform->SetScale({ lerp(cursorGizmo->m_transform->GetScale().x,20.f,0.4f), lerp(cursorGizmo->m_transform->GetScale().y,20.f,0.4f) });
			if (cursorGizmo->m_transform->GetScale().x > 19.9f)
			{
				cursorMode = gizmoMode;
				changed = false;
			}
		}
		POINT pos;
		pos.x = static_cast<LONG>(Input::Instance()->GetMouseScreen().x + 18);
		pos.y = static_cast<LONG>(Input::Instance()->GetMouseScreen().y - 12);
		cursorGizmo->m_transform->SetPos(Input::Instance()->ScreenToWorld(pos));

	}

	void ShowListOfSelectedObjects()
	{
		if (selectedObjects.size() == 1)
		{
			if (deselectedAnObject)
			{
				globals::pickedObject = nullptr;
				return;
			}
			if (selectedObjects[0] != nullptr && selectedObjects[0]->m_alive == true)
				globals::pickedObject = selectedObjects[0];
			return;
		}
		else
		{
			if (selectedObjects.size() == 0)
				return;

			if (ImGui::Begin("Objects Currently selected", NULL, ImGuiWindowFlags_NoCollapse | ImGuiCond_FirstUseEver))
			{
				for (size_t i = 0; i < selectedObjects.size(); i++)
				{
					GameObject* object = selectedObjects[i];

					std::string name = object->m_name + "##" + std::to_string(i);
					if (ImGui::Selectable(name.c_str()))
					{
						globals::pickedObject = object;
						removeSelectedObjectsBorders();
						auto* temp = globals::pickedObject->FindComponentByType<Graphics::LineRenderer>();

						if (temp != nullptr)
							temp->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

						selectedObjects.clear();
					}
				}
				ImGui::End();
			}
		}
	}


	void DeleteSelectedObjects()
	{
		if (Input::Instance()->KeyTriggered(Input::eDelete))
		{
			for (auto& object : selectedObjects)
			{
				object->Destroy();
			}
			if (globals::pickedObject != nullptr)
			{
				globals::pickedObject->Destroy();
				globals::pickedObject = nullptr;
			}
			selectedObjects.clear();
		}

	}
	void Cloning()
	{
		if (Input::Instance()->KeyPressed(Input::eShift))
		{
			if (Input::Instance()->KeyTriggered('D'))
			{
				if (selectedObjects.size() != 0)
				{
					for (const auto& object : selectedObjects)
						object->Clone();
					return;
				}
				if (globals::pickedObject != nullptr)
					globals::pickedObject->Clone();
			}
			if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
			{
				std::vector<GameObject*> tempBuffer;
				for (const auto& object : selectedObjects)
					tempBuffer.push_back(object->Clone());
				selectedObjects.clear();
				selectedObjects = tempBuffer;
				gizmoMode = GMode::Translate;
			}
		}
	}
	void Console()
	{
		ImGui::SetNextWindowPos(ImVec2(220, 830));
		ImGui::SetNextWindowSize({ 1700,250 });
		if (ImGui::Begin("Console", 0, ObjectEditorFlags))
		{
			ImGui::Text("%s", Console::textBuffer.str().c_str());
			ImGui::End();
		}
	}
	void ResourceManagerWindow()
	{
		unsigned windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (windowConfig::lockArchetypes)
		{
			windowFlags |= ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoResize;
		}
		if (ImGui::Begin("Archetypes", NULL, windowFlags))
		{
			for (const auto& archetype : ResourceManager::archetypes)
			{
				unsigned characterCountUntilNameExtension = static_cast<unsigned>(archetype.first.find(".archetype"));
				std::string archetypeName = archetype.first.substr(0, characterCountUntilNameExtension);

				if (ImGui::Selectable(archetypeName.c_str()))
				{
					auto* gameObject = objectManager.AddGameObject(archetypeName, false);
					gameObject->from_json(archetype.second);
					gameObject->m_guid = (size_t)gameObject;
				}
			}
			ImGui::End();
		}
	}
	float lerp(float v0, float v1, float t)
	{
		return (1 - t) * v0 + t * v1;
	}
	void ImGuiLateUpdate()
	{
		if (globals::pickedObject != nullptr)
			if (globals::pickedObject->m_alive == false)
				globals::pickedObject = nullptr;
	}

	void cameraStuff()
	{
		if (ImGui::GetCurrentContext()->HoveredWindow)
			return;
		if (Input::Instance()->KeyPressed('Z') || Input::Instance()->GetMouseWheel() < -0.5)
		{
			GraphicsSystem::Instance()->UpdateCamerasZoom(1.1f);
		}
		if (Input::Instance()->KeyPressed('X') || Input::Instance()->GetMouseWheel() > 0.5f)
		{
			GraphicsSystem::Instance()->UpdateCamerasZoom(0.9f);
		}
	}
	void drawBorders()
	{
		for (auto& object : objectManager.m_gameObjects)
		{
			if (object->EditableByGizmos)
			{
				glm::vec2 point0 = glm::vec2(object->m_transform->GetPos());
				point0 += (object->m_transform->GetScale() / 2.f);
				//point0 *= getTransform()->GetRotation();

				glm::vec2 point1 = glm::vec2(object->m_transform->GetPos());
				point1 -= (object->m_transform->GetScale() / 2.f);
				//point1 *= getTransform()->GetRotation();

				if (object->FindComponentByType<Graphics::LineRenderer>())
					object->FindComponentByType<Graphics::LineRenderer>()->DrawRect(point0, point1);
			}
		}
	}
	void removeSelectedObjectsBorders()
	{
		for (unsigned i = 0; i < selectedObjects.size(); i++)
		{
			if (selectedObjects[i] == nullptr)
				continue;
			if (selectedObjects[i]->FindComponentByType<Graphics::LineRenderer>() == nullptr)
				continue;
			auto* temp = selectedObjects[i]->FindComponentByType<Graphics::LineRenderer>();
			if (temp != nullptr)
			{
				temp->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	void EditorShortcuts()
	{
		if (Input::Instance()->KeyPressed(Input::eControl) && Input::Instance()->KeyTriggered('S'))
		{
			std::time_t now = std::time(NULL);
			std::tm * ptm = std::localtime(&now);
			char buffer[32];
			// Format: Mo, 15.06.2009 20:20:00
			std::strftime(buffer, 32, "%a %d %m %Y %H %M %S", ptm);

			std::string folder = "Data/Levels/";

			std::string levelName = "LazySave" + std::string(buffer);
			levelName += ".level";
			objectManager.SaveToFile(levelName, folder);

			automaticSaveTimer.Reset();

		}

		if (Input::Instance()->KeyPressed(Input::Keys::eEsc))
		{
			selectedObjects.clear();
		}

		if (Input::Instance()->KeyPressed(Input::eControl) && Input::Instance()->KeyTriggered('N'))
		{
			globals::pickedObject = nullptr;
			globals::pickedObject = objectManager.AddGameObject("QuickObject");
			globals::pickedObject->m_transform->SetPos(glm::vec2(0, 200));
			globals::pickedObject->m_transform->SetScale(glm::vec2(50, 50));

			//also add it a renderable component
			auto* rend = globals::pickedObject->AddComponent<Graphics::Renderable>();
			GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
		}

		if (Input::Instance()->MousePressed(Input::eMouseRight))
		{
			glm::vec2 currentMousePos = Input::Instance()->ScreenToWorld();

			if (ImGui::GetCurrentContext()->HoveredWindow)
			{
				if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
				{
					originalStartingPos = currentMousePos;
					PreviousMousePos = currentMousePos;

				}
				return;
			}

			if (globals::pickedObject != nullptr)
			{
				globals::pickedObject->m_transform->SetPos(Input::Instance()->ScreenToWorld());
			}
		}
	}
	void editorUpdate()
	{


		//IMGUI DEMO
		//ImGui::ShowTestWindow();	//TEST WINDOW

		static float f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// 2. Show another simple window, this time using an explicit Begin/End pair
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			ImGui::End();
		}
		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(1200, 20)/*, ImGuiCond_Once, ImVec2(0.5, 0.5)*/);
			ImGui::ShowTestWindow(&show_test_window);
		}


		EditorShortcuts();
		objectEditor();
		objectsInSpace();
		
		ShowListOfSelectedObjects();
		GizmoUpdate();
		DeleteSelectedObjects();
		GizmoCursorUpdate();
		Cloning();
		cameraStuff();
		if(!windowConfig::hideConsole)
			Console();

		ResourceManagerWindow();
		mainMenu();
		showSelectedObject();
		autosave();
	}
	void autosave()
	{
		//if you havent saved for 5 minutes
		if (automaticSaveTimer.GetTimeSinceStart() > 60 * 5)
		{
			automaticSaveTimer.Reset();
			std::time_t now = std::time(NULL);
			std::tm * ptm = std::localtime(&now);
			char buffer[32];
			// Format: Mo, 15.06.2009 20:20:00
			std::strftime(buffer, 32, "%a %d %m %Y %H %M %S", ptm);

			std::string folder = "Data/Levels/";

			std::string levelName = "Autosave" + std::string(buffer);
			levelName += ".level";
			objectManager.SaveToFile(levelName, folder);
		}
	}

	namespace ThomasModeConfig
	{
		bool rainbow = true;
	}

	void thomasModeUpdate()
	{
		using namespace ThomasModeConfig;

		bool pushedColors = rainbow;
		if (rainbow)
		{
			ImVec4 color;
			ImGui::ColorConvertHSVtoRGB(thomasModeColor / 255, 1, 1, color.x, color.y, color.z);
			color.w = 170;

			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, color);
			ImGui::PushStyleColor(ImGuiCol_TitleBg, color);
		}
		if(ImGui::Begin("Thomas Mode", NULL))
		{
			thomasModeColor+= 1;

			if (thomasModeColor > 255)
				thomasModeColor = 0;

			ImGui::Checkbox("Rainbow Mode", &rainbow);


			if(ImGui::Button("Create checkpoint at current position"))
			{
				Checkpoint::respawnPosition = objectManager.FindObjectByName("player")->m_transform->GetPos();
			}

			if (ImGui::Button("Go to last checkpoint"))
			{
				objectManager.FindObjectByName("player")->m_transform->SetPos(Checkpoint::respawnPosition);
			}

			ImGui::Checkbox("Teleport Mode", &PlayerStateMachine::teleportMode);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("While active, control + right clicking will teleport DIBO to the mouse position");
				ImGui::EndTooltip();
			}

			ImGui::End();
		}
		if(pushedColors)
			ImGui::PopStyleColor(2);
		
	}
}