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
#include <vart/meshobject.h>
#include <iostream>
#include <string.h>

using namespace std;

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : redRadians(0), greenRadians(0) {}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            redRadians += 0.001;
            redRotPtr->MakeZRotation(redRadians);
            greenRadians += 0.001;
            greenRotPtr->MakeYRotation(greenRadians);
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
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects

    VART::MeshObject piramede;

    VART::Point4D v0(0,0,0);
    VART::Point4D v1(2,0,0);
    VART::Point4D v2(2,0,2);
    VART::Point4D v3(0,0,2);
    VART::Point4D v4(1,2,1);
    vector<VART::Point4D> vet;
    vet.push_back(v0);
    vet.push_back(v1);
    vet.push_back(v2);
    vet.push_back(v3);
    vet.push_back(v4);
    piramede.SetVertices(vet);
    string face1="1 4 0";
    string face2="2 4 1";
    string face3="2 4 3";
    string face4="0 4 3";
    string face5="2 1 0 3";
    cout << "AQUI" << endl;

    piramede.AddFace(face1.c_str());
    cout << "AQUq" << endl;
    piramede.AddFace(face2.c_str());
    cout<<"aui1"<<endl;
    piramede.AddFace(face3.c_str());
    cout<<"aqi2"<<endl;
    piramede.AddFace(face4.c_str());
    cout<<"aqi3"<<endl;
    piramede.AddFace(face5.c_str());
    cout<<"aqi4"<<endl;
    piramede.SetMaterial(VART::Material::PLASTIC_RED());
    cout<<"aqui5"<<endl;

    piramede.ComputeVertexNormals();
    cout<<"aqui6";
    // Initialize scene objects
    

    // Build up the scene
    

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    scene.AddObject(&piramede);
    // Set up the Idle Handler
    

    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
   
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
