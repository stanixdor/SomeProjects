#pragma once

#ifndef IMGUI_API
#define IMGUI_API
#endif

#include "../../Engine/Window/Window.h"
#include "../../Engine/Window/WindowManager.h"
#include "../../src/Engine/Graphics/Camera.h"

//struct GLFWwindow;

IMGUI_API bool        ImGui_Init(Window::Window* window);
IMGUI_API void        ImGui_Shutdown();
IMGUI_API void        ImGui_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplGlfwGL3_CreateDeviceObjects();