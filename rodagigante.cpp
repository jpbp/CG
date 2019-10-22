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
#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std;

float conversor(float grau){
    float radiano=(grau/180)*M_PI;
    return radiano;
}

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : redRadians(0) {}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
            redRadians += 1;
            int cont=0;
            for (int i=0; i<10; i++){
                float z=-70*cos(conversor(redRadians+cont));
                float y=70*sin(conversor(redRadians+cont));
                redRotPtr[i]->MakeTranslation(0,y,z);
                cont+=36;
            }
           
            rotation->MakeXRotation(conversor(redRadians));
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform *redRotPtr[10];
        VART::Transform *rotation;
        
    private:
        float redRadians;
        
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
    MyIHClass idleHandler;

    KeyboardHandler kbh; // create a keyboard handler
    // create a camera (scene observer)

    VART::Camera camera(VART::Point4D(1,0,0),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    camera.SetFarPlaneDistance(200); //arruma a distancia da camera
    camera.MoveForward(-190);
    // create some objects
    
    list<VART::MeshObject*> objects;
    ClickHandlerClass clickHandler;
    clickHandler.scenePtr = &scene; 

    // Initialize scene objects
    VART::MeshObject::ReadFromOBJ(argv[1], &objects);
    


    //objetos

    //vetor de cadeiras
    VART::MeshObject *chairs[10]; 
    VART::Transform testeTrans[10];
    VART::Transform rotation;
    for (int n=0; n < 10; n++){
        chairs[n]=NULL;
        testeTrans[n].MakeIdentity();
       
    }
    VART::MeshObject *support=NULL;
    VART::MeshObject *wheel=NULL;
    // Build up the scene
    list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
        if((*iter)->GetDescription()=="chair"){
          
            for (int n=0; n < 10; n++){
                chairs[n]=dynamic_cast<VART::MeshObject *>((*iter)->Copy()); //copia das cadeiras
                if(n%2==0){
                    chairs[n]->SetMaterial(VART::Material::PLASTIC_RED());
                }
                else if (n%3==0){
                    chairs[n]->SetMaterial(VART::Material::PLASTIC_BLUE());
                }
                //scene.AddObject(chairs[n]);
                testeTrans[n].AddChild(*(dynamic_cast<VART::SceneNode*>(chairs[n])));
                scene.AddObject(&(testeTrans[n]));
            }
        }
        else if((*iter)->GetDescription()=="support"){
            support=*iter;
           // support->ApplyTransform(redux);
            scene.AddObject(support);
            cout<<support->GetDescription()<<endl;
        }
        else if((*iter)->GetDescription()=="wheel"){
            wheel=*iter;
           // wheel->ApplyTransform(redux);
            rotation.MakeIdentity();
            rotation.AddChild(*(dynamic_cast<VART::SceneNode*>(wheel)));
            scene.AddObject(&rotation);
            cout<<wheel->GetDescription()<<endl;
        }
    }

    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    //scene.MakeCameraViewAll();
    for (int i=0; i<10; i++){
        idleHandler.redRotPtr[i]=&(testeTrans[i]);
    }
    idleHandler.rotation=&rotation;
    
    // Set up the viewer
    viewer.SetTitle("V-ART OBJ Viewer");
    viewer.SetScene(scene); // attach the scene
    viewer.SetKbHandler(&kbh); // attach the keyboard handler
    viewer.SetClickHandler(&clickHandler);
    viewer.SetIdleHandler(&idleHandler);
   
    // Run application
    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
