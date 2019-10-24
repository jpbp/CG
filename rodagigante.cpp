// main.cpp - Wavefront OBJ viewer application

// V-ART example: Load and Display a OBJ file.

// This application shows how to create an object from a Wavefront OBJ file.

// Made from picker example, keeps the piking code.

// Changelog
// Oct 19, 2012 - Bruno de Oliveira Schneider
// - Adapted to new keyboard handler methods.
// Sep 26, 2012 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/box.h>
#include <vart/cylinder.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>

using namespace std;
float conversor(float rad){
    return (rad/180)*M_PI;
}

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : redRadians(0), greenRadians(0) {}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            
            
            int grau=0;
            for (size_t i = 0; i < 10; i++)
            {
                float y=-70*sin(conversor(grau+redRadians));
                float z=70*cos(conversor(grau+redRadians));
                redRotPtr[i]->MakeTranslation(0,y,z);
                grau+=36;    
            }
            redRadians += 0.1;
            greenRadians += 0.1;
            greenRotPtr->MakeXRotation(conversor(greenRadians));
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* redRotPtr[10];
        VART::Transform* greenRotPtr;
    private:
        float redRadians;
        float greenRadians;
};
// Define the keyboard handler
class KeyboardHandler : public VART::ViewerGlutOGL::KbHandler
{
    public:
        KeyboardHandler() {
        }
        virtual void OnKeyDown(int key) {
            switch (key)
            {
                case 'j':
                    break;
                case 'l':
                    break;
                case 'i':
                    break;
                case 'k':
                    break;
            }
        }
    private:
};

class ClickHandlerClass : public VART::MouseControl::ClickHandler
{
    public:
        ClickHandlerClass() {
        }
        virtual ~ClickHandlerClass() {};
        virtual void OnClick() {
            if (mouseCtrlPtr->LastClickIsDown()) {
                VART::MouseControl::ButtonID lastButton = mouseCtrlPtr->GetLastClickButton();

                if (lastButton == VART::MouseControl::LEFT) {
                    scenePtr->Pick(mouseCtrlPtr->GetClickXPosition(),
                                   mouseCtrlPtr->GetClickYPosition(), &pickList);
                    if (!pickList.empty()) {
                        cout << pickList.size() << " selected objects: ";
                        while (!pickList.empty()) {
                            cout << pickList.front()->GetDescription() << ":\n";
#ifdef DEBUG
                            VART::MeshObject* ptObj = dynamic_cast<VART::MeshObject*>(pickList.front());
                            if (ptObj)
                                cout << *ptObj << "\n";
#endif
                            pickList.pop_front();
                        }
                        cout << endl;
                    }
                }
            }
        }
        list<VART::GraphicObj*> pickList;
        VART::Scene* scenePtr;
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT

    if (argc < 2) {
        cerr << "Fatal error: No filename given.\nUsage: " << argv[0] << " filename\n"
             << "Where 'filename' is a Wavefront OBJ file.\n";
        return 1;
    }

    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    KeyboardHandler kbh; // create a keyboard handler
    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(200,0,50),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    camera.SetFarPlaneDistance(200);
    // create some objects
    list<VART::MeshObject*> objects;
    ClickHandlerClass clickHandler;
    MyIHClass idh;
    clickHandler.scenePtr = &scene;

    // Initialize scene objects
    VART::MeshObject::ReadFromOBJ(argv[1], &objects);
    VART::MeshObject wheel;
    VART::MeshObject support;
    VART::Transform rotWheel;
    VART::Transform TransChair[10];

    
    int grau;

    // Build up the scene
    list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
        if((*iter)->GetDescription()=="chair"){
            for (size_t i = 0; i < 10; i++)
            {
                
                TransChair[i].AddChild(*(dynamic_cast<VART::MeshObject*>((*iter)->Copy())));
                scene.AddObject(&TransChair[i]);
                idh.redRotPtr[i]=&TransChair[i];
            }
        }
        else if((*iter)->GetDescription()=="wheel"){
            wheel=*(*iter);
            rotWheel.AddChild(wheel);
        }
        else if((*iter)->GetDescription()=="support"){
            support=*(*iter);
        }
    }
    scene.AddObject(&rotWheel);
    scene.AddObject(&support);
    idh.greenRotPtr=&rotWheel;
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    //scene.MakeCameraViewAll();

    // Set up the viewer
    viewer.SetTitle("V-ART OBJ Viewer");
    viewer.SetScene(scene); // attach the scene
    viewer.SetKbHandler(&kbh); // attach the keyboard handler
    viewer.SetClickHandler(&clickHandler);
    viewer.SetIdleHandler(&idh);
    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
