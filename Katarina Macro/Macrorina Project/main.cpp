#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../Input/InputManager.hh"
#include "../Keyboard/KeyboardUtilities.hh"

#include "cryptopp/default.h"
using CryptoPP::DefaultEncryptorWithMAC;
using CryptoPP::DefaultDecryptorWithMAC;

#include "cryptopp/filters.h"
using CryptoPP::StringSource;
using CryptoPP::StringSink;

#include "cryptopp/files.h"
using CryptoPP::FileSource;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <Windows.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

std::string GetActiveWindowTitle()
{
    char wnd_title[256];
    HWND hwnd = GetForegroundWindow(); // get handle of currently active window
    GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
    return wnd_title;
}
void MacroExecution(const char* E2WQR, const char* QE2WR, const char* ignite, const char* gunblade, int delay,
                    const bool useUlt, const bool useGun, const bool useIgn,
                    const char* ultToggle, const char* gunToggle, const char* ignToggle,
                    bool& useUltimate, bool& useGunblade, bool& useIgnite)
{
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    std::string lolWindowName = "League of Legends (TM) Client";
    static bool active = true;
    if (GetActiveWindowTitle() != lolWindowName)
        return;

    if (InputManager::IsKeyPressed(ultToggle[0]))
        useUltimate = !useUltimate;

    if (InputManager::IsKeyPressed(gunToggle[0]))
        useGunblade = !useGunblade;

    if (InputManager::IsKeyPressed(ignToggle[0]))
        useIgnite = !useIgnite;

    if (InputManager::IsKeyPressed(13))
    {
        active = !active;
    }
    if (active)
    {
        if (InputManager::IsKeyPressed(E2WQR[0]))
        {
            KeyboardUtilities::PressKey('E');
            sleep_for(milliseconds(delay));
            if (useGun)
            {
                KeyboardUtilities::PressKey(gunblade[0]);
                sleep_for(milliseconds(delay));
            }
            if (useIgn)
            {
                KeyboardUtilities::PressKey(ignite[0]);
                sleep_for(milliseconds(delay));
            }
            KeyboardUtilities::PressKey('W');
            sleep_for(milliseconds(delay));
            KeyboardUtilities::PressKey('Q');
            sleep_for(milliseconds(delay));
            if (useUlt)
                KeyboardUtilities::PressKey('R');
        }
        else if (InputManager::IsKeyPressed(QE2WR[0]))
        {
            KeyboardUtilities::PressKey('Q');
            sleep_for(milliseconds(150) + milliseconds(delay));
            KeyboardUtilities::PressKey('E');
            sleep_for(milliseconds(delay));
            if (useGun)
            {
                KeyboardUtilities::PressKey(gunblade[0]);
                sleep_for(milliseconds(delay));
            }
            if (useIgn)
            {
                KeyboardUtilities::PressKey(ignite[0]);
                sleep_for(milliseconds(delay));
            }
            KeyboardUtilities::PressKey('W');
            sleep_for(milliseconds(delay));
            if (useUlt)
                KeyboardUtilities::PressKey('R');
        }
    }
}


void EncryptF(const char* file)
{
    std::string encrypted;
    FileSource fs1(file, true,
        new DefaultEncryptorWithMAC(
            (byte*)std::string("pastelito2003").data(), std::string("pastelito2003").size(),
            new HexEncoder(
                new StringSink(encrypted)
            )
        )
    );

    std::ofstream f;
    f.open(file, std::ofstream::trunc);
    if (f.is_open())
    {
        f << encrypted;
    }
    f.close();
}
std::string DecryptF(const char* file)
{
    std::string encrypted, decrypted;

    std::ifstream f;
    f.open(file);
    if (f.is_open())
    {
        std::ostringstream ss;
        ss << f.rdbuf();
        encrypted = ss.str();
    }
    f.close();
    
    
    StringSource ss1(encrypted, true,
        new HexDecoder(
            new DefaultDecryptorWithMAC(
                (byte*)std::string("pastelito2003").data(), std::string("pastelito2003").size(),
                new StringSink(decrypted)
            )
        )
    );
    return decrypted;
}
std::string GetIP()
{
    system("curl ifconfig.co > tempip");

    std::string ip;

    std::ifstream f;
    f.open("tempip");
    if (f.is_open())
    {
        std::ostringstream ss;
        ss << f.rdbuf();
        ip = ss.str();
    }

    f.close();

    std::remove("tempip");

    return ip;
}
bool Logging()
{
    if (!std::experimental::filesystem::exists("key"))
        return false;

    std::string deS = DecryptF("Key");
    std::string line;
    std::stringstream ss;
    ss << deS;

    std::string ip = GetIP();

    unsigned counter = 0;
    while (std::getline(ss, line))
    {
        
        if (counter == 0)
        {
            std::string s1 = "curl https://lolautosmite.000webhostapp.com/Macrorina/";
            std::string s2 = line;
            std::string s3 = "/key -o tempkey -s";
            std::string sFinal = s1+ s2 +s3;
            system(sFinal.c_str());
            if (std::experimental::filesystem::file_size("tempkey") > 3000)
            {
                std::remove("tempkey");
                return false;
            }
        }
        if (counter == 2)
        {
            
            std::string deS2 = DecryptF("tempkey");
            std::remove("tempkey");
            std::string line2;
            std::stringstream ss2;
            ss2 << deS2;
            unsigned counter2 = 0;
            while (std::getline(ss2, line2))
            {
                if (counter2 == 2)
                {
                    if (ip == std::string(line2 + "\n"))
                        return true;
                    else
                        return false;
                }
                ++counter2;
            }
        }
        ++counter;
    }

    return false;
}

int main(int, char**)
{

    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    if (!Logging())
    {
        MessageBox(NULL, "Loggin failed", "Error",0);
        return -1;
    }

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(700, 420, "Macrorina 1.0", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    InputManager::Initialize();

    // Our state
    /*bool show_demo_window = true;
    bool show_another_window = false;*/
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int delay = 0;
    bool useIgnite = true;
    bool useUltimate = true;
    bool useGunblade = true;
    char ultToggle[2]{ 0 };
    char gunToggle[2]{ 0 };
    char ignToggle[2]{ 0 };
    char ignite[2]{0};
    char gunblade[2]{ 0 };
    char E2WQR[2]{0};
    char QE2WR[2]{ 0 };

    std::string line;
    std::ifstream myfile("config");
    if (myfile.is_open())
    {
        unsigned counter = 0;
        while (std::getline(myfile, line))
        {
            if (line.size() < 1)
            {
                ++counter;
                continue;
            }
            std::stringstream ss(line);

            switch (counter)
            {
            case 0:
            {
                ss >> delay;
                break;
            }
            case 1:
            {
                if(line != std::string("NULL"))
                    ss >> gunblade[0];
                break;
            }
            case 2:
            {
                if (line != std::string("NULL"))
                    ss >> ignite[0];
                break;
            }
            case 3:
            {
                if (line != std::string("NULL"))
                {
                    if (line == std::string(" "))
                        E2WQR[0] = 32;
                    else
                        ss >> E2WQR[0];
                }
                    
                break;
            }
            case 4:
            {
                if (line != std::string("NULL"))
                    ss >> QE2WR[0];
                break;
            }
            case 5:
            {
                if (line != std::string("NULL"))
                    ss >> ultToggle[0];
                break;
            }
            case 6:
            {
                if (line != std::string("NULL"))
                    ss >> gunToggle[0];
                break;
            }
            case 7:
            {
                if (line != std::string("NULL"))
                    ss >> ignToggle[0];
                break;
            }
            case 8:
            {
                ss >> useUltimate;
                break;
            }
            case 9:
            {
                ss >> useGunblade;
                break;
            }
            case 10:
            {
                ss >> useIgnite;
                break;
            }
            default:
                break;
            }

            ++counter;
        }
        myfile.close();
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        InputManager::Update();
        MacroExecution(E2WQR,QE2WR,ignite,gunblade,delay,useUltimate,useGunblade,useIgnite,ultToggle,gunToggle,ignToggle,useUltimate,useGunblade,useIgnite);

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {

            ImGui::SetNextWindowSize({ 700,430 });
            ImGui::SetNextWindowPos({ 0,0 });
            ImGui::Begin("Macrorina", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
                                                    ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);               


            ImGui::Text("Humanizer (delay between actions in ms)");            

            ImGui::SliderInt("", &delay, 0, 150);
            ImGui::NewLine();
            ImGui::Text("Key Bindings (Use upper case letters in order to make the binding work properly)");
            ImGui::NewLine();
            ImGui::InputText("Gunblade", gunblade, sizeof(gunblade));
            ImGui::InputText("Ignite", ignite, sizeof(ignite));
            ImGui::InputText("E->Gunblade->W->Q->R Combo", E2WQR, sizeof(E2WQR));
            ImGui::InputText("Q->E->Gunblade->W->R Combo", QE2WR, sizeof(QE2WR));
            ImGui::InputText("Use Ultimate toggle", ultToggle, sizeof(ultToggle));
            ImGui::InputText("Use Gunblade toggle", gunToggle, sizeof(gunToggle));
            ImGui::InputText("Use Ignite toggle", ignToggle, sizeof(ignToggle));
            ImGui::NewLine();
            ImGui::Text("Options");
            ImGui::NewLine();
            ImGui::Checkbox("Use Ultimate", &useUltimate);
            ImGui::Checkbox("Use Gunblade", &useGunblade);
            ImGui::Checkbox("Use Ignite", &useIgnite);

            if (ImGui::Button("Apply changes"))
            {
                std::ofstream file;
                file.open("config", std::ofstream::trunc);
                if (file.is_open())
                {
                    file << delay << "\n";
                    if(gunblade[0])
                        file << gunblade[0] << "\n";
                    else 
                        file << "NULL" << "\n";
                    if (ignite[0])
                        file << ignite[0] << "\n";
                    else
                        file << "NULL" << "\n";
                    if (E2WQR[0])
                        file << E2WQR[0] << "\n";
                    else
                        file << "NULL" << "\n";
                    if (QE2WR[0])
                        file << QE2WR[0] << "\n";
                    else
                        file << "NULL" << "\n";
                    if (ultToggle[0])
                        file << ultToggle[0] << "\n";
                    else
                        file << "NULL" << "\n";
                    if (gunToggle[0])
                        file << gunToggle[0] << "\n";
                    else
                        file << "NULL" << "\n";
                    if (ignToggle[0])
                        file << ignToggle[0] << "\n";
                    else
                        file << "NULL" << "\n";

                    file << useUltimate << "\n";
                    file << useGunblade << "\n";
                    file << useIgnite << "\n";

                }
                file.close();
            }

            ImGui::End();
        }


        InputManager::EndUpdate();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
