// main.cpp - transform animation example

// V-ART example: Moving objects with an idle handler

// This application shows how to use ViewerGlutOGL's idle handler to animate objects.

// Changelog
// Jun 17, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h> 

#include <iostream>

using namespace std;
float conversor(float grau){
    float radiano=(grau/180)*M_PI;
    return radiano;
}
class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : redRadians(0), greenRadians(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            float z=10*cos(conversor(redRadians));
            float x=10*sin(conversor(redRadians));
            redRadians += 1;
            greenRadians+=10;
            redRotPtr->MakeTranslation(x,0,z);
            float z1=3*cos(conversor(greenRadians));
            float x1=3*sin(conversor(greenRadians));
            greenRotPtr->MakeTranslation(x1,0,z1);
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* redRotPtr;
        VART::Transform* greenRotPtr;
    private:
        float redRadians;
        float greenRadians;
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT // locacao estática de memória
    static VART::Scene scene; // create a scene conjunto de elementos para formar uma imagem, menos a janela
    static VART::ViewerGlutOGL viewer; // create a viewer (application window) // para formar a imagem observador(janela) luz(enxergar as coisas) objeto
    MyIHClass idleHandler;
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,20),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0)); // 14:30
    //parametros camera 
    //1 (UM PONTO ONDE A CAMERA ESTÁ)
    //2 (OUTRO PONTO ONDE A CAMERA ESTÁ OLHANDO "FOCO")
    //3 (DIREÇÃO PARA CIMA, LADO DE CIMA DA CAMERA)???? 
    // create some objects
    VART::Transform transTerra;
    VART::Transform TransLua;
    VART::Sphere terra;
    VART::Sphere sol;
    VART::Sphere lua;
    VART::Texture textura;
    VART::Texture textura1;
    VART::Material material;
    if(textura.LoadFromFile("sun.gif")){
        material.SetTexture(textura);
        sol.SetMaterial(textura);
    }
    if(textura.LoadFromFile("earth.jpg")){
        material.SetTexture(textura);
        terra.SetMaterial(textura);
    }
    if(textura.LoadFromFile("moon.PNG")){
        material.SetTexture(textura);
        lua.SetMaterial(textura);
    }
    
    //transTerra.MakeTranslation(10,0,0);
    transTerra.AddChild(terra);
    terra.AddChild(TransLua);
    TransLua.AddChild(lua);
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.AddObject(&sol);
    scene.AddObject(&transTerra);
    // Set up the Idle Handler
    
    idleHandler.redRotPtr=&transTerra;
    idleHandler.greenRotPtr=&TransLua;
    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler);

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
