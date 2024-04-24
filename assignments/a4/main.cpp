// #####################################################################
//  Main
//  CS3451 Computer Graphics Starter Code
//  Contact: Bo Zhu (bo.zhu@gatech.edu)
// #####################################################################
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

#include "OpenGLCommon.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLMesh.h"
#include "OpenGLViewer.h"
#include "OpenGLWindow.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class ShaderDriver : public OpenGLViewer {
    std::vector<OpenGLTriangleMesh*> mesh_object_array;
    clock_t startTime;

public:
    void Create_Bunny_Scene()
    {
        Create_Background(OpenGLColor(0.1f, 0.1f, 0.1f, 1.f), OpenGLColor(0.1f, 0.1f, .3f, 1.f));   //// add background

        auto bunny = Add_Obj_Mesh_Object("bunny.obj");
        //// set transform
        Matrix4f t3;
        t3 << 1., 0., 0., 0.,
            0., 1., 0., 0.,
            0., 0., 1., 0.,
            0., 0., 0., 1.;
        bunny->Set_Model_Matrix(t3);
        //// set material properties
        bunny->Set_Ka(Vector3f(0.f, 0.9f, 0.f));
        bunny->Set_Kd(Vector3f(0.f, 0.9f, 0.f));
        bunny->Set_Ks(Vector3f(5.f, 5.f, 5.f));
        bunny->Set_Shininess(128.f);

        auto sphere1 = Add_Sphere_Object(0.4);
        //// set transform
        Matrix4f t1;
        t1 << 1., 0., 0., 0.,
            0., 1., 0., -1.,
            0., 0., 1., 1.5,
            0., 0., 0., 1.;
        sphere1->Set_Model_Matrix(t1);
        //// set material properties
        sphere1->Set_Ka(Vector3f(0.9f, 0.9f, 0.f));
        sphere1->Set_Kd(Vector3f(0.9f, 0.9f, 0.f));
        sphere1->Set_Ks(Vector3f(2.f, 2.f, 2.f));
        sphere1->Set_Shininess(32.f);

        auto sphere2 = Add_Sphere_Object(0.4);
        //// set transform
        Matrix4f t2;
        t2 << 0.75, 0., 0., 1.,
            0., 0.75, 0., -1.,
            0., 0., 0.75, 1.,
            0., 0., 0., 1.;
        sphere2->Set_Model_Matrix(t2);
        //// set material properties
        sphere2->Set_Ka(Vector3f(1.f, 0.f, 0.7f));
        sphere2->Set_Kd(Vector3f(1.f, 0.f, 0.7f));
        sphere2->Set_Ks(Vector3f(2.f, 2.f, 2.f));
        sphere2->Set_Shininess(128.f);
    }

    void Create_Shining_Scene()
    {
        Create_Background(OpenGLColor(0.1f, 0.1f, 0.1f, 1.f), OpenGLColor(0.1f, 0.1f, .3f, 1.f));   //// add background

        //// Step 7: Add your customized mesh objects and specify their transform and material properties by mimicking Create_Bunny_Scene() 
        /* Your implementation starts */

        /* Your implementation ends */
    }

    //// Step 7: Comment out Create_Bunny_Scene() and uncomment Create_Shining_Scene() for your customized scene.
    virtual void Initialize_Data()
    {
        Create_Bunny_Scene();               //// TODO: comment out this line for your customized scene
        //Create_Shining_Scene();           //// TODO: uncomment this line for your customized scene

        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a4_vert.vert", "a4_frag.frag", "a4_shader");
        for (auto& mesh_obj : mesh_object_array) {
            mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a4_shader"));
            Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
            Set_Shading_Mode(mesh_obj, ShadingMode::Phong);
            mesh_obj->Set_Data_Refreshed();
            mesh_obj->Initialize();
        }
    }

    virtual void Initialize() {
        draw_axes = false;
        startTime = clock();
        OpenGLViewer::Initialize();
    }

    void Create_Background(const OpenGLColor color1 = OpenGLColor::Black(), const OpenGLColor color2 = OpenGLColor(.01f, .01f, .2f, 1.f)) {
        auto bg = Add_Interactive_Object<OpenGLBackground>();
        bg->Set_Color(color1, color2);
        bg->Initialize();
    }

    ////This function adds a mesh object from an obj file
    OpenGLTriangleMesh* Add_Obj_Mesh_Object(std::string obj_file_name) {
        auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        Array<std::shared_ptr<TriangleMesh<3>>> meshes;
        Obj::Read_From_Obj_File(obj_file_name, meshes);
        mesh_obj->mesh = *meshes[0];
        std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

        mesh_object_array.push_back(mesh_obj);
        return mesh_obj;
    }

    OpenGLTriangleMesh* Add_Sphere_Object(const double radius = 1.)
    {
        auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        Initialize_Sphere_Mesh(radius, &mesh_obj->mesh, 3);

        mesh_object_array.push_back(mesh_obj);
        return mesh_obj;
    }

    //// Go to next frame
    virtual void Toggle_Next_Frame() {
        for (auto& mesh_obj : mesh_object_array) {
            mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
        }
        OpenGLViewer::Toggle_Next_Frame();
    }

    virtual void Run() {
        OpenGLViewer::Run();
    }
};

int main(int argc, char* argv[]) {
    ShaderDriver driver;
    driver.Initialize();
    driver.Run();
}

#endif