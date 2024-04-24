//#####################################################################
// Main
// CS3451 Computer Graphics Starter Code
// Contact: Bo Zhu (bo.zhu@gatech.edu)
//#####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Common.h"
#include "OpenGLMesh.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class NoiseDriver : public OpenGLViewer
{
	std::vector<OpenGLTriangleMesh*> mesh_object_array;		////mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
        draw_axes = false;
		startTime = clock();
		OpenGLViewer::Initialize();
		opengl_window->camera_distance = 12.f;
		opengl_window->camera_target = Vector3f(2.5, 2.5, 0);
	}

    void Create_Background(const OpenGLColor color1 = OpenGLColor::Black(), const OpenGLColor color2 = OpenGLColor(.01f, .01f, .2f, 1.f)) 
    {
        auto bg = Add_Interactive_Object<OpenGLBackground>();
        bg->Set_Color(color1, color2);
        bg->Initialize();
    }

	////This function adds a mesh object from an obj file
	OpenGLTriangleMesh* Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		std::cout<<"load tri_mesh from obj file, #vtx: "<<mesh_obj->mesh.Vertices().size()<<", #ele: "<<mesh_obj->mesh.Elements().size()<<std::endl;		

		mesh_object_array.push_back(mesh_obj);
		return mesh_obj;
	}

	virtual void Initialize_Data()
	{
        Create_Background(OpenGLColor(0.1f, 0.1f, 0.1f, 1.f), OpenGLColor(0.1f, 0.1f, .4f, 1.f)); // add background

		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a6_vert.vert", "a6_frag.frag", "perlin.glsl", "a6_shader");
		////add the plane mesh object
		auto obj = Add_Obj_Mesh_Object("plane.obj");
		obj->Set_Ka(Vector3f(0.1f, 0.1f, 0.1f));
        obj->Set_Kd(Vector3f(0.7f, 0.7f, 0.7f));
        obj->Set_Ks(Vector3f(1, 1, 1));
        obj->Set_Shininess(128.f);
		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a6_shader"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::Texture);
		obj->Set_Data_Refreshed();
		obj->Initialize();
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		for (auto& mesh_obj : mesh_object_array) {
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	NoiseDriver driver;
	driver.Initialize();
	driver.Run();	
}

#endif
