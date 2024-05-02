#include "Common.h"
#include "OpenGLCommon.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLBgEffect.h"
#include "OpenGLMesh.h"
#include "OpenGLViewer.h"
#include "OpenGLWindow.h"
#include "TinyObjLoader.h"
#include "OpenGLSkybox.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>


#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class MyDriver : public OpenGLViewer
{
    std::vector<OpenGLTriangleMesh *> mesh_object_array;
    OpenGLBgEffect *bgEffect = nullptr;
    OpenGLSkybox *skybox = nullptr;
    clock_t startTime;

    OpenGLScreenCover *screen_cover = nullptr;




public:
    virtual void Initialize()
    {
        draw_axes = false;
        startTime = clock();
        OpenGLViewer::Initialize();
    }


    /*Joanna Zolnik's Technical Algorithm
    In this algorithm, I use procedural generation to cover the terrain in grass.
    I've decided to use randomized instancing for the x and z coordinates in the 
    procedural generation.*/
    
    virtual void generateGrass() {
        float numGrass = 300;
        float xPosition = 3.8;
        float yPosition = -3.4;
        float zPosition = -5.0;

        std::srand(std::time(nullptr));

        for (int i = 0; i < numGrass; i++) {
            
            xPosition = static_cast<float>(std::rand()) / RAND_MAX * 31.0 - 14.0;
            yPosition = -4.0;
            zPosition = static_cast<float>(std::rand()) / RAND_MAX * 30.0 - 31.0;
            
            {
                // create object by reading an obj mesh
                auto grass = Add_Obj_Mesh_Object("obj/grass.obj");

                // set object's transform
                Matrix4f t;
                t << 2.5, 0, 0, xPosition,
                    0, 2.5, 0, yPosition,
                    0, 0, 2.5, zPosition,
                    0, 0, 0, 1;

                grass->Set_Model_Matrix(t);

                // set object's material
                grass->Set_Ka(Vector3f(0.1, 0.2, 0.2));
                grass->Set_Kd(Vector3f(0.6, 0.8, 0.7));
                grass->Set_Ks(Vector3f(2, 3, 2));
                grass->Set_Shininess(140);

                // bind texture to object
                grass->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("grass_color"));

                // bind shader to object
                grass->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
            }
        }

    }




    virtual void Initialize_Data() {
        // Load all the shaders you need for the scene 
        // In the function call of Add_Shader_From_File(), we specify three names: 
        // (1) vertex shader file name
        // (2) fragment shader file name
        // (3) shader name used in the shader library
        // When we bind a shader to an object, we implement it as follows:
        // object->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shader_name"));
        // Here "shader_name" needs to be one of the shader names you created previously with Add_Shader_From_File()


        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/basic.frag", "basic");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/environment.frag", "environment");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/stars.vert", "shaders/stars.frag", "stars");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/alphablend.frag", "blend");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/billboard.vert", "shaders/alphablend.frag", "billboard");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/terrain.vert", "shaders/terrain.frag", "terrain");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/skybox.vert", "shaders/skybox.frag", "skybox");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/fireflies.vert", "shaders/fireflies.frag", "fireflies");
        //OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/bird.vert", "shaders/bird.frag", "bird");
        // Load all the textures you need for the scene
        // In the function call of Add_Shader_From_File(), we specify two names:
        // (1) the texture's file name
        // (2) the texture used in the texture library
        // When we bind a texture to an object, we implement it as follows:
        // object->Add_Texture("tex_sampler", OpenGLTextureLibrary::Get_Texture("tex_name"));
        // Here "tex_sampler" is the name of the texture sampler2D you used in your shader, and
        // "tex_name" needs to be one of the texture names you created previously with Add_Texture_From_File()

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/earth_color.png", "sphere_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/earth_normal.png", "sphere_normal");

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bunny_color.jpg", "bunny_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bunny_normal.png", "bunny_normal");

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/window.png", "window_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/buzz_color.png", "buzz_color");
        
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/star.png", "star_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/cloud.png", "cloud_color");

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/tree.jpg", "tree_color");


        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/deer.jpg", "deer_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/raccoon.jpg", "raccoon_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/fox.png", "fox_color");


        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bird.jpg", "bird_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bird_normal.jpg", "bird_normal");

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bluebird_color.jpg", "bluebird_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bluebird_normal.jpg", "bluebird_normal");

        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/grass_color.png", "grass_color");

        
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/walker_color.png", "walker_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/walker_normal.png", "walker_normal");


        //LIGHTING
        // Add all the lights you need for the scene (no more than 4 lights)
        // The four parameters are position, ambient, diffuse, and specular.

        // The lights you declared here will be synchronized to all shaders in uniform lights.
        // You may access these lights using lt[0].pos, lt[1].amb, lt[1].dif, etc.
        // You can also create your own lights by directly declaring them in a shader without using Add_Light().
        // Here we declared three default lights for you. Feel free to add/delete/change them at your will.

        // opengl_window->Add_Light(Vector3f(-2, 20, -15), Vector3f(1, 1, 1), Vector3f(1, 1, 1), Vector3f(1, 1, 1)); 
        // opengl_window->Add_Light(Vector3f(-10, 20, -5), Vector3f(1, 1, 1), Vector3f(1, 1, 1), Vector3f(1, 1, 1));
        // opengl_window->Add_Light(Vector3f(10, 20, 10), Vector3f(1, 1, 1), Vector3f(1, 1, 1), Vector3f(1, 1, 1));
        // opengl_window->Add_Light(Vector3f(5, 20, 20), Vector3f(1, 1, 1), Vector3f(1, 1, 1), Vector3f(1, 1, 1));
        
        // Soft overhead light settings
        Vector3f lightColor(0.8f, 0.8f, 0.8f); // Soft white color for the light
        Vector3f ambientColor(0.5f, 0.5f, 0.5f); // Ambient color to fill in shadows
        Vector3f varied(1.0f, 0.09f, 0.032f); // varied for softness

        // Add soft overhead lights
        opengl_window->Add_Light(Vector3f(-2, 20, -15), lightColor, ambientColor, varied);
        opengl_window->Add_Light(Vector3f(-10, 20, -5), lightColor, ambientColor, varied);
        opengl_window->Add_Light(Vector3f(10, 20, 10), lightColor, ambientColor, varied);
        opengl_window->Add_Light(Vector3f(5, 20, 20), lightColor, ambientColor, varied);

       
        // Add the background / environment
        // Here we provide you with four default options to create the background of your scene:
        // (1) Gradient color (like A1 and A2; if you want a simple background, use this one)
        // (2) Programmable Canvas (like A7 and A8; if you consider implementing noise or particles for the background, use this one)
        // (3) Sky box (cubemap; if you want to load six background images for a skybox, use this one)
        // (4) Sky sphere (if you want to implement a sky sphere, enlarge the size of the sphere to make it colver the entire scene and update its shaders for texture colors)
        // By default, Option (2) (Buzz stars) is turned on, and all the other three are commented out.
        
        // Background Option (1): Gradient color
        //navy gradient with stars
        {
            auto bg = Add_Interactive_Object<OpenGLBackground>();
            //bg->Set_Color(OpenGLColor(0.0, 0.0, 0.2, 1.0), OpenGLColor(0.05, 0.05, 0.25, 1.0));
            bg->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("stars"));
            bg->Initialize();
        }

        // fireflies
        {
            bgEffect = Add_Interactive_Object<OpenGLBgEffect>();
            bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("fireflies"));
            //bgEffect->Add_Texture("tex_buzz", OpenGLTextureLibrary::Get_Texture("buzz_color")); // bgEffect can also Add_Texture
            //bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("blend"));
            bgEffect->Initialize();
        }

        // bird
        // {
            
        //     bgEffect = Add_Interactive_Object<OpenGLBgEffect>();
        //     bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("bird"));
        //     //bgEffect->Add_Texture("tex_buzz", OpenGLTextureLibrary::Get_Texture("buzz_color")); // bgEffect can also Add_Texture
        //     //bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("blend"));
        //     bgEffect->Initialize();
        // // screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
        // // Set_Polygon_Mode(screen_cover, PolygonMode::Fill);

        // // screen_cover->Set_Data_Refreshed();
        // // screen_cover->Initialize();
        // // screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("bird"));
        // // screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("blend"));

        // }
        

        // Background Option (2): Programmable Canvas
        // By default, we load a GT buzz + a number of stars
        /*{
            bgEffect = Add_Interactive_Object<OpenGLBgEffect>();
            bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("stars"));
            //bgEffect->Add_Texture("tex_buzz", OpenGLTextureLibrary::Get_Texture("buzz_color")); // bgEffect can also Add_Texture
            bgEffect->Initialize();
        }*/
        

        // Background Option (3): Sky box
        // Here we provide a default implementation of a sky box; customize it for your own sky box
        // {
        //     // from https://www.humus.name/index.php?page=Textures
        //     const std::vector<std::string> cubemap_files{
        //         "cubemap/negx.jpg",     // + X
        //         "cubemap/negy.jpg",     // - X
        //         "cubemap/negz.jpg",     // + Y
        //         "cubemap/posx.jpg",     // - Y
        //         "cubemap/posy.jpg",     // + Z
        //         "cubemap/posz.jpg",     // - Z 
        //     };
        //     OpenGLTextureLibrary::Instance()->Add_CubeMap_From_Files(cubemap_files, "cube_map");

        //     skybox = Add_Interactive_Object<OpenGLSkybox>();
        //     skybox->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));
        //     skybox->Initialize();
        // }



      

        // screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
            
        // screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("fireflies"));
        // screen_cover->Initialize();
        // //screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("blend"));
        // }
        

        // Background Option (4): Sky sphere
        // Here we provide a default implementation of a textured sphere; customize it for your own sky sphere
        // {
        //     // create object by reading an obj mesh
        //     auto sphere = Add_Obj_Mesh_Object("obj/sphere.obj");

        //     // set object's transform
        //     Matrix4f t;
        //     t << 1, 0, 0, -1.5,
        //         0, 1, 0, -1,
        //         0, 0, 1, 0.5,
        //         0, 0, 0, 1;
        //     sphere->Set_Model_Matrix(t);

        //     // set object's material
        //     sphere->Set_Ka(Vector3f(0.1, 0.1, 0.1));
        //     sphere->Set_Kd(Vector3f(0.7, 0.7, 0.7));
        //     sphere->Set_Ks(Vector3f(2, 2, 2));
        //     sphere->Set_Shininess(128);

        //     // bind texture to object
        //     sphere->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("sphere_color"));
        //     sphere->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("sphere_normal"));

        //     // bind shader to object
        //     sphere->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        // }

        
        generateGrass();


        // Here we load a bunny object with the basic shader to show how to add an object into the scene
        {
            // create object by reading an obj mesh
            auto bunny = Add_Obj_Mesh_Object("obj/bunny.obj");

            // set object's transform
            Matrix4f t;
            t << .3, 0, 0, 3.8,
                0, .3, 0, -3.4,
                0, 0, .3, -15.0,
                0, 0, 0, 1;
            bunny->Set_Model_Matrix(t);

            // set object's material
            bunny->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            bunny->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            bunny->Set_Ks(Vector3f(2, 2, 2));
            bunny->Set_Shininess(128);

            // bind texture to object
            bunny->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("bunny_color"));
            bunny->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bunny_normal"));

            // bind shader to object
            bunny->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }



        {
            // create object by reading an obj mesh
            auto deer = Add_Obj_Mesh_Object("obj/deer.obj");

            // set object's transform

            Matrix4f t;
            Matrix4f r;
            Matrix4f k;
            
            float angle = -79.0f; // Angle in degrees
            float angleRad = radians(angle);

            float z_angle = 210.0f; // Angle in degrees
            float z_angleRad = radians(z_angle);


            //rotate deer around the z axis
            k << cos(z_angleRad), -sin(z_angleRad), 0.0, 0.0,
                sin(z_angleRad), cos(z_angleRad), 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;


            //rotate deer around the x axis
            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;


            t << .088, 0, 0, 1.5,
                0, .088, 0, -3.8,
                0, 0, .088, -17.0,
                0, 0, 0, 1;


            deer->Set_Model_Matrix(t * r * k);

            // set object's material
            deer->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            deer->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            deer->Set_Ks(Vector3f(2, 2, 2));
            deer->Set_Shininess(128);

            // bind texture to object
            deer->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("deer_color"));
            //deer->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bunny_normal"));

            // bind shader to object
            deer->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        


        {
            // create object by reading an obj mesh
            auto raccoon = Add_Obj_Mesh_Object("obj/raccoon.obj");

            // set object's transform

            Matrix4f t;
            Matrix4f r;
            Matrix4f k;
            
            float angle = -79.0f; // Angle in degrees
            float angleRad = radians(angle);

            float z_angle = 40.0f; // Angle in degrees
            float z_angleRad = radians(z_angle);


            //rotate deer around the z axis
            k << cos(z_angleRad), -sin(z_angleRad), 0.0, 0.0,
                sin(z_angleRad), cos(z_angleRad), 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;


            //rotate deer around the x axis
            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;


            t << .037, 0, 0, -3.5,
                0, .037, 0, -3.9,
                0, 0, .037, -16.0,
                0, 0, 0, 1;


            raccoon->Set_Model_Matrix(t * r * k);

            // set object's material
            raccoon->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            raccoon->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            raccoon->Set_Ks(Vector3f(2, 2, 2));
            raccoon->Set_Shininess(128);

            // bind texture to object
            raccoon->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("raccoon_color"));
            //deer->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bunny_normal"));

            // bind shader to object
            raccoon->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        /*{
            // create object by reading an obj mesh
            auto fox = Add_Obj_Mesh_Object("obj/fox.obj");

            // set object's transform

            Matrix4f t;
            Matrix4f r;
            Matrix4f k;
            
            float angle = -79.0f; // Angle in degrees
            float angleRad = radians(angle);

            float z_angle = 40.0f; // Angle in degrees
            float z_angleRad = radians(z_angle);


            //rotate deer around the z axis
            k << cos(z_angleRad), -sin(z_angleRad), 0.0, 0.0,
                sin(z_angleRad), cos(z_angleRad), 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;


            //rotate deer around the x axis
            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;


            t << .37, 0, 0, -4.5,
                0, .37, 0, -2.4,
                0, 0, .37, -8.0,
                0, 0, 0, 1;


            fox->Set_Model_Matrix(t * r * k);

            // set object's material
            fox->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            fox->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            fox->Set_Ks(Vector3f(2, 2, 2));
            fox->Set_Shininess(128);

            // bind texture to object
            fox->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("fox_color"));
            fox->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bunny_normal"));

            // bind shader to object
            fox->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }*/


        //yellow bird
        {
            // create object by reading an obj mesh
            auto bird = Add_Obj_Mesh_Object("obj/bird.obj");

            // set object's transform

            Matrix4f t;
            Matrix4f r;
            Matrix4f k;
            
            float angle = -79.0f; // Angle in degrees
            float angleRad = radians(angle);

            float z_angle = 40.0f; // Angle in degrees
            float z_angleRad = radians(z_angle);


            //rotate deer around the z axis
            k << cos(z_angleRad), -sin(z_angleRad), 0.0, 0.0,
                sin(z_angleRad), cos(z_angleRad), 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;


            //rotate deer around the x axis
            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;


            t << .055, 0, 0, 2.5 ,
                0, .055, 0, -0.916 ,
                0, 0, .055, -17.09,
                0, 0, 0, 1;


            bird->Set_Model_Matrix(t * r * k);

            // set object's material
            bird->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            bird->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            bird->Set_Ks(Vector3f(2, 2, 2));
            bird->Set_Shininess(128);

            // bind texture to object
            bird->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("bird_color"));
            bird->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bird_normal"));

            // bind shader to object
            bird->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }



        //blue bird
        {
            // create object by reading an obj mesh
            auto bluebird = Add_Obj_Mesh_Object("obj/bluebird.obj");

            // set object's transform

            Matrix4f t;
            Matrix4f r;
            Matrix4f k;
            
            float angle = -79.0f; // Angle in degrees
            float angleRad = radians(angle);

            float z_angle = 180.0f; // Angle in degrees
            float z_angleRad = radians(z_angle);


            //rotate deer around the z axis
            k << cos(z_angleRad), -sin(z_angleRad), 0.0, 0.0,
                sin(z_angleRad), cos(z_angleRad), 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;


            //rotate deer around the x axis
            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;


            t << .055, 0, 0, 1.80,
                0, .055, 0, -1.000,
                0, 0, .055, -16.55,
                0, 0, 0, 1;


            bluebird->Set_Model_Matrix(t * r * k);

            // set object's material
            bluebird->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            bluebird->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            bluebird->Set_Ks(Vector3f(2, 2, 2));
            bluebird->Set_Shininess(128);

            // bind texture to object
            bluebird->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("bluebird_color"));
            bluebird->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("bluebird_normal"));

            // bind shader to object
            bluebird->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        //walker
        {
            // create object by reading an obj mesh
            auto man = Add_Obj_Mesh_Object("obj/walker.obj");

            // set object's transform

            Matrix4f t;
            Matrix4f r;
            Matrix4f k;
            Matrix4f w;
            
            float angle = 0.0f; // Angle in degrees
            float angleRad = radians(angle);

            float z_angle = 0.0f; // Angle in degrees
            float z_angleRad = radians(z_angle);

            float y_angle = 90.0f; // Angle in degrees
            float y_angleRad = radians(y_angle);

            //rotate deer around the z axis
            k << cos(z_angleRad), -sin(z_angleRad), 0.0, 0.0,
                sin(z_angleRad), cos(z_angleRad), 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0;

            //rotate deer around the x axis
            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;

            //rotate deer around the y axis
            w << cos(y_angleRad), 0, sin(y_angleRad), 0,
                0, 1, 0, 0,
                -sin(y_angleRad), 0, cos(y_angleRad), 0,
                0, 0, 0, 1;

            t << .025, 0, 0, -1.5,
                0, .025, 0, -3.816,
                0, 0, .025, -12.85,
                0, 0, 0, 1;

            man->Set_Model_Matrix(t * r * k * w);

            // set object's material
            man->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            man->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            man->Set_Ks(Vector3f(2, 2, 2));
            man->Set_Shininess(128);

            // bind texture to object
            man->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("walker_color"));
            man->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("walker_normal"));

            // bind shader to object
            man->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        {
            // create object by reading an obj mesh
            auto pinetree = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .03, 0, 0, -6.0,
                0, .03, 0, -4.6,//-2.5
                0, 0, .03, -24,
                0, 0, 0, 1;

            pinetree->Set_Model_Matrix(t * r);
            // set object's material
            pinetree->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree->Set_Ks(Vector3f(2, 2, 2));
            pinetree->Set_Shininess(128);
            // bind texture to object
            pinetree->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }



        {
            // create object by reading an obj mesh
            auto pinetree1 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .03, 0, 0, 22.0,
                0, .03, 0, -4.8,
                0, 0, .03, -24,
                0, 0, 0, 1;

            pinetree1->Set_Model_Matrix(t * r);
            // set object's material
            pinetree1->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree1->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree1->Set_Ks(Vector3f(2, 2, 2));
            pinetree1->Set_Shininess(128);
            // bind texture to object
            pinetree1->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree1->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        {
            // create object by reading an obj mesh
            auto pinetree2 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .028, 0, 0, 15.0,
                0, .028, 0, -4.7,
                0, 0, .028, -24,
                0, 0, 0, 1;

            pinetree2->Set_Model_Matrix(t * r);
            // set object's material
            pinetree2->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree2->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree2->Set_Ks(Vector3f(2, 2, 2));
            pinetree2->Set_Shininess(128);
            // bind texture to object
            pinetree2->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree2->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }



        {
            // create object by reading an obj mesh
            auto pinetree3 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .020, 0, 0, 14.0,
                0, .020, 0, -4.6,
                0, 0, .020, -16,
                0, 0, 0, 1;

            pinetree3->Set_Model_Matrix(t * r);
            // set object's material
            pinetree3->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree3->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree3->Set_Ks(Vector3f(2, 2, 2));
            pinetree3->Set_Shininess(128);
            // bind texture to object
            pinetree3->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree3->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        {
            // create object by reading an obj mesh
            auto pinetree4 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .020, 0, 0, -3.0,
                0, .020, 0, -4.5,
                0, 0, .020, -20,
                0, 0, 0, 1;

            pinetree4->Set_Model_Matrix(t * r);
            // set object's material
            pinetree4->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree4->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree4->Set_Ks(Vector3f(2, 2, 2));
            pinetree4->Set_Shininess(128);
            // bind texture to object
            pinetree4->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree4->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        {
            // create object by reading an obj mesh
            auto pinetree5 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .022, 0, 0, -7.0,
                0, .022, 0, -4.5,
                0, 0, .022, -16,
                0, 0, 0, 1;

            pinetree5->Set_Model_Matrix(t * r);
            // set object's material
            pinetree5->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree5->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree5->Set_Ks(Vector3f(2, 2, 2));
            pinetree5->Set_Shininess(128);
            // bind texture to object
            pinetree5->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree5->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }




        {
            // create object by reading an obj mesh
            auto pinetree5 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .014, 0, 0, -5.0,
                0, .014, 0, -4.5,
                0, 0, .014, -14,
                0, 0, 0, 1;

            pinetree5->Set_Model_Matrix(t * r);
            // set object's material
            pinetree5->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree5->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree5->Set_Ks(Vector3f(2, 2, 2));
            pinetree5->Set_Shininess(128);
            // bind texture to object
            pinetree5->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree5->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        {
            // create object by reading an obj mesh
            auto pinetree5 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .024, 0, 0, 5.0,
                0, .024, 0, -4.5,
                0, 0, .024, -25,
                0, 0, 0, 1;

            pinetree5->Set_Model_Matrix(t * r);
            // set object's material
            pinetree5->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree5->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree5->Set_Ks(Vector3f(2, 2, 2));
            pinetree5->Set_Shininess(128);
            // bind texture to object
            pinetree5->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree5->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        {
            // create object by reading an obj mesh
            auto pinetree5 = Add_Obj_Mesh_Object("obj/tree.obj");
            // set object's transform
            Matrix4f t;
            Matrix4f r;

            float angle = -85.0f; // Angle in degrees
            float angleRad = radians(angle);
            

            r << 1.0, 0.0, 0.0, 0.0,
                0.0, cos(angleRad),  -sin(angleRad),  0.0,
                0.0, sin(angleRad),   cos(angleRad),  0.0,
                0.0, 0.0, 0.0, 1.0;
            
            t << .018, 0, 0, 8.0,
                0, .018, 0, -4.5,
                0, 0, .018, -21,
                0, 0, 0, 1;

            pinetree5->Set_Model_Matrix(t * r);
            // set object's material
            pinetree5->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            pinetree5->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            pinetree5->Set_Ks(Vector3f(2, 2, 2));
            pinetree5->Set_Shininess(128);
            // bind texture to object
            pinetree5->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("tree_color"));
            // bind shader to object
            pinetree5->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }





        // Here we show an example of adding a mesh with noise-terrain (A6)
        {
            // create object by reading an obj mesh
            auto terrain = Add_Obj_Mesh_Object("obj/plane.obj");

            // set object's transform
            Matrix4f r, s, t;

            
            float angle = -90.0f; // Angle in degrees
            float angleRad = radians(angle); // Convert angle to radians

            r << 1,          0,              0,  0,
                0,  cos(angleRad), -sin(angleRad),  0,
                0,  sin(angleRad),  cos(angleRad),  0,
                0,          0,              0,  1;

            /*r << 1, 0, 0, 0,
                0, 0.5, 0.67, 0,
                0, -0.67, 0.5, 0,
                0, 0, 0, 1*/

            //adjust terrain size using s matrix
            s << 6.5, 0, 0, 0,
                0, 6.5, 0, 0,
                0, 0, 6.5, 0,
                0, 0, 0, 1;

            //adjust terrain position
            t << 1, 0, 0, -14.5, //sideways
                 0, 1, 0, -4,//up
                 0, 0, 1, 0,
                 0, 0, 0, 1,

                 
            terrain->Set_Model_Matrix(t * s * r);

            // set object's material
            terrain->Set_Ka(Vector3f(0.1f, 0.1f, 0.1f));
            terrain->Set_Kd(Vector3f(0.7f, 0.7f, 0.7f));
            terrain->Set_Ks(Vector3f(1, 1, 1));
            terrain->Set_Shininess(150.f);

            // bind shader to object (we do not bind texture for this object because we create noise for texture)
            terrain->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("terrain"));
        }


        // Here we show an example of adding a transparent object with alpha blending
        // This example will be useful if you implement objects such as tree leaves, grass blades, flower pedals, etc.
        // Alpha blending will be turned on automatically if your texture has the alpha channel
        /*{
            // create object by reading an obj mesh
            auto sqad = Add_Obj_Mesh_Object("obj/sqad.obj");

            // set object's transform
            Matrix4f t;
            t << 1, 0, 0, -0.5,
                0, 1, 0, 0,
                0, 0, 1, 1.5,
                0, 0, 0, 1;
            sqad->Set_Model_Matrix(t);

            // bind texture to object
            sqad->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("window_color"));

            // bind shader to object
            sqad->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("blend"));
        }*/


        // Here we show an example of adding a billboard particle with a star shape using alpha blending
        // The billboard is rendered with its texture and is always facing the camera.
        // This example will be useful if you plan to implement a CPU-based particle system.
        /*{
            // create object by reading an obj mesh
            auto sqad = Add_Obj_Mesh_Object("obj/sqad.obj");

            // set object's transform
            Matrix4f t;
            t << 1, 0, 0, 3,//sideways
                 0, 1, 0, 1.5,//up
                 0, 0, 1, 1,
                 0, 0, 0, 1;
            sqad->Set_Model_Matrix(t);

            // bind texture to object
            sqad->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("star_color"));

            // bind shader to object
            sqad->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("billboard"));
        }*/


        // Here we show an example of shading (ray-tracing) a sphere with environment mapping
        /*{
            // create object by reading an obj mesh
            auto sphere2 = Add_Obj_Mesh_Object("obj/sphere.obj");

            // set object's transform
            Matrix4f t;
            t << .6, 0, 0, 0,
                0, .6, 0, -.5,
                0, 0, .6, 1,
                0, 0, 0, 1;
            sphere2->Set_Model_Matrix(t);

            // bind shader to object
            sphere2->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("environment")); // bind shader to object
        }*/
        


        // This for-loop updates the rendering model for each object on the list
        for (auto &mesh_obj : mesh_object_array){
            Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
            Set_Shading_Mode(mesh_obj, ShadingMode::TexAlpha);
            mesh_obj->Set_Data_Refreshed();
            mesh_obj->Initialize();
        }
        Toggle_Play();
    }

    OpenGLTriangleMesh *Add_Obj_Mesh_Object(std::string obj_file_name)
    {
        auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        Array<std::shared_ptr<TriangleMesh<3>>> meshes;
        // Obj::Read_From_Obj_File(obj_file_name, meshes);
        Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);

        mesh_obj->mesh = *meshes[0];
        std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

        mesh_object_array.push_back(mesh_obj);
        return mesh_obj;
    }

    // Go to next frame
    virtual void Toggle_Next_Frame()
    {
        for (auto &mesh_obj : mesh_object_array)
            mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);

        if (bgEffect){
            bgEffect->setResolution((float)Win_Width(), (float)Win_Height());
            bgEffect->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
            bgEffect->setFrame(frame++);
        } else if (screen_cover) {
            //screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
            screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
            screen_cover->setFrame(frame++);
        }

        OpenGLViewer::Toggle_Next_Frame();
    }

    virtual void Run()
    {
        OpenGLViewer::Run();
    }
};

int main(int argc, char *argv[])
{
    MyDriver driver;
    driver.Initialize();
    driver.Run();
}

#endif