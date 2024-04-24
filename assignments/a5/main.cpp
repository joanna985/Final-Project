// #####################################################################
//  Main
//  Dartmouth COSC 77/177 Computer Graphics, starter code
//  Contact: Bo Zhu (bo.zhu@dartmouth.edu)
// #####################################################################
#include "Common.h"
#include "OpenGLCommon.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLMesh.h"
#include "OpenGLViewer.h"
#include "OpenGLWindow.h"
#include "TinyObjLoader.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
#include <string>

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

enum class TexType:int{Color=0, Normal=1};

class ShaderDriver : public OpenGLViewer {
    std::vector<OpenGLTriangleMesh *> mesh_object_array;
    clock_t startTime;

public:
    void Create_Bunny_Scene()
    {
        Create_Background(OpenGLColor(0.1f, 0.1f, 0.1f, 1.f), OpenGLColor(0.1f, 0.1f, .3f, 1.f)); // add background

        //// initialize sphere
        {
            //// initiailize mesh
            auto sphere = Add_Obj_Mesh_Object("sphere.obj");
            sphere->name = "sphere"; //// Must set name for the object

            //// initialize transform
            Matrix4f t;
            t << 1, 0, 0, -1.5,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
            sphere->Set_Model_Matrix(t);

            //// initialize material
            sphere->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            sphere->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            sphere->Set_Ks(Vector3f(2, 2, 2));
            sphere->Set_Shininess(128);

            //// initialize texture
            Add_Textture_For_Mesh_Object(sphere, "earth_color.png", TexType::Color);
            Add_Textture_For_Mesh_Object(sphere, "earth_normal.png", TexType::Normal);
        }
        
        //// initialize bunny
        {
            //// initialize mesh
            auto bunny = Add_Obj_Mesh_Object("bunny.obj");
            bunny->name = "bunny"; //// Must set name for the object

            //// initialize transform
            Matrix4f t;
            t << 1, 0, 0, 1.5,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
            bunny->Set_Model_Matrix(t);

            //// initialize material
            bunny->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            bunny->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            bunny->Set_Ks(Vector3f(2., 2., 2.));
            bunny->Set_Shininess(128.);

            //// initialize texture
            Add_Textture_For_Mesh_Object(bunny, "bunny_color.jpg", TexType::Color);
            Add_Textture_For_Mesh_Object(bunny, "bunny_normal.png", TexType::Normal);
        }
    }

    void Create_Old_Object_Scene()
    {
        Create_Background(OpenGLColor(0.1f, 0.1f, 0.1f, 1.f), OpenGLColor(0.1f, 0.1f, .3f, 1.f));   //// add background

        //// Step 5: Add your customized mesh objects and specify their transform, material, and texture properties by mimicking Create_Bunny_Scene() 
        /* Your implementation starts */

        /* Your implementation ends */
    }

    virtual void Initialize_Data() 
    {
        Create_Bunny_Scene();           //// TODO: comment out this line for your customized scene
        // Create_Old_Object_Scene();   //// TODO: uncomment this line for your customized scene

        ////initialize shader
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a5_vert.vert", "a5_frag.frag", "a5_shader");
        ////bind the shader with each mesh object in the object array
        for (auto& mesh_obj : mesh_object_array) {
            mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a5_shader"));
            std::cout << "mesh_obj->name: " << mesh_obj->name << std::endl;
            mesh_obj->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture(mesh_obj->name + "_color"));
            mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture(mesh_obj->name + "_normal"));
            Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
            Set_Shading_Mode(mesh_obj, ShadingMode::Texture);
            mesh_obj->Set_Data_Refreshed();
            mesh_obj->Initialize();
        }
    }

    virtual void Initialize() 
    {
        draw_axes = false;
        startTime = clock();
        OpenGLViewer::Initialize();
    }

    void Create_Background(const OpenGLColor color1 = OpenGLColor::Black(), const OpenGLColor color2 = OpenGLColor(.01f, .01f, .2f, 1.f)) 
    {
        auto bg = Add_Interactive_Object<OpenGLBackground>();
        bg->Set_Color(color1, color2);
        bg->Initialize();
    }

    OpenGLTriangleMesh *Add_Obj_Mesh_Object(std::string obj_file_name) 
    {
        auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        Array<std::shared_ptr<TriangleMesh<3>>> meshes;
        // Obj::Read_From_Obj_File(obj_file_name, meshes);
        Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes); // load an obj mesh with discrete triangles

        mesh_obj->mesh = *meshes[0];
        std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

        mesh_object_array.push_back(mesh_obj);
        return mesh_obj;
    }

    void Add_Textture_For_Mesh_Object(OpenGLTriangleMesh *obj ,const std::string &texture_file_name, TexType type) 
    {
        if (type == TexType::Color)
            OpenGLTextureLibrary::Instance()->Add_Texture_From_File(texture_file_name, obj->name + "_color");
        else if (type == TexType::Normal)
            OpenGLTextureLibrary::Instance()->Add_Texture_From_File(texture_file_name, obj->name + "_normal");
    }

    //// Go to next frame
    virtual void Toggle_Next_Frame() 
    {
        for (auto &mesh_obj : mesh_object_array)
            mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
        OpenGLViewer::Toggle_Next_Frame();
    }

    virtual void Run() 
    {
        OpenGLViewer::Run();
    }
};

int main(int argc, char *argv[]) 
{
    ShaderDriver driver;
    driver.Initialize();
    driver.Run();
}

#endif