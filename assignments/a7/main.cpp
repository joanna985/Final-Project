// #####################################################################
//  Main
//  CS3451 Computer Graphics Starter Code
//  Contact: Bo Zhu (bo.zhu@gatech.edu)
// #####################################################################
#include <iostream>
#include <random>

#include "OpenGLCommon.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLMesh.h"
#include "OpenGLParticles.h"
#include "OpenGLViewer.h"
#include "OpenGLWindow.h"

class RayTracingDriver : public OpenGLViewer 
{
    OpenGLScreenCover *screen_cover = nullptr;
    clock_t startTime;
    int frame;

public:
    virtual void Initialize() 
    {
        draw_bk = false;
        draw_axes = false;
        startTime = clock();
        frame = 1;
        OpenGLViewer::Initialize();
        Disable_Resize_Window(); // Changing window size would cause trouble in progressive rendering
    }

    //// Initialize the screen covering mesh and shaders
    virtual void Initialize_Data() 
    {
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("common.vert", "ray_tracing.frag", "rt");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("common.vert", "basic_frag.frag", "screen");
        screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
        Set_Polygon_Mode(screen_cover, PolygonMode::Fill);
        Uniform_Update();
        // Add texture
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("floor.jpg", "floor_color");
        screen_cover->Add_Texture("floor_color", OpenGLTextureLibrary::Get_Texture("floor_color"));
        
        screen_cover->use_tex = true;
        screen_cover->Set_Data_Refreshed();
        screen_cover->Initialize();
        screen_cover->Add_Buffer();
		screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("screen"));
        screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("rt"));
        // Temporal Anti-aliasing
        Toggle_Play();
    }

    //// Update the uniformed variables used in shader
    void Uniform_Update() 
    {
        // screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
        screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
        screen_cover->setFrame(frame++);
    }

    //// Go to next frame
    virtual void Toggle_Next_Frame() 
    {
        Uniform_Update();
        OpenGLViewer::Toggle_Next_Frame();
    }

    ////Keyboard interaction
    virtual void Initialize_Common_Callback_Keys() 
    {
        OpenGLViewer::Initialize_Common_Callback_Keys();
        Bind_Callback_Key('r', &Keyboard_Event_R_Func, "Restart");
    }

    virtual void Keyboard_Event_R() 
    {
        std::cout << "Restart" << std::endl;
        startTime = clock();
        frame = 1;
    }

    Define_Function_Object(RayTracingDriver, Keyboard_Event_R);
    virtual void Run() 
    {
        OpenGLViewer::Run();
    }
};

int main(int argc, char *argv[]) 
{
    RayTracingDriver driver;
    driver.Initialize();
    driver.Run();
}
