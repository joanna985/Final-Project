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

class FireWorks : public OpenGLViewer
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
        // Disable_Resize_Window(); // Changing window size would cause trouble in progressive rendering
    }

    //// Initialize the screen covering mesh and shaders
    virtual void Initialize_Data()
    {
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a8_vert.vert", "a8_frag.frag", "firework");

        screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
        Set_Polygon_Mode(screen_cover, PolygonMode::Fill);
        Uniform_Update();

        screen_cover->Set_Data_Refreshed();
        screen_cover->Initialize();
        screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("firework"));
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

    virtual void Run()
    {
        OpenGLViewer::Run();
    }
};

int main(int argc, char *argv[])
{
    FireWorks driver;
    driver.Initialize();
    driver.Run();
}
