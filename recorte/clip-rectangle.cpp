#include "clip-rectangle.h"
#include <iostream>
#include <cmath>

using namespace std;

ClipRectangle::ClipRectangle (double xmin, double xmax, double ymin, double ymax)
    : mXmin(xmin), mXmax(xmax), mYmin(ymin), mYmax(ymax), mInitialized(true)
{}

void ClipRectangle::Set(double xmin, double xmax, double ymin, double ymax)
{
    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;
    mInitialized = true;
}

// Retorna um ponteiro para uma linha recem alocada inteiramente dentro da area de recorte
// ou NULL se l for completamente descartada.
// A linha alocada aqui será desalocada na função CleanUp().
Line* ClipRectangle::Clip(const Line& l)
{
    Point newPoint;
    Line *linha= new Line;
    double inicio=0;
    double fim =1;
    double numerador;
    double denominador;
    double t=0;
    cout<<l<<endl;
    for (size_t i = 0; i < 4; i++)
    {
        if(i==0){ // lado esquerdo 
            numerador=mXmin-l.mP0.mX;
            denominador= l.mP1.mX-l.mP0.mX;
            if(denominador==0){// segmento paralelo a area de recorte
                if(numerador>0){ // fora da area de recorte
                    // 
                return NULL;
                }
            }
            else{
                t=numerador/denominador;
                cout<<denominador<<endl;
                if(denominador>0){
                    if(t>fim){
                        
                        return NULL;
                    }
                    else if(t>inicio){
                        inicio=t;
                    }
                }
                else{
                    if(t<inicio){
                        return NULL;
                    }
                    else if(t>fim){
                        fim=t;
                    }
                }
            }
        }
                
        
        
    }
    if(inicio!=0){
            newPoint= l.mP0+(l.mP1-l.mP0)*inicio;
            linha->mP0=newPoint;
            linha->mP1=l.mP1;
        }
        if(fim!=1){
            newPoint= l.mP0+(l.mP1-l.mP0)*fim;
            linha->mP0=l.mP0;
            linha->mP1=newPoint;
        }
    cout<<linha->mP0.mX;
    cout<<linha->mP0.mY;
    return linha;
}

void ClipRectangle::Read(const string& prompt)
{
    cout << prompt;
    cin >> mXmin >> mXmax >> mYmin >> mYmax;
    if (not mInitialized)
        mInitialized = true;
}

ostream& operator << (ostream& os, const ClipRectangle& r)
{
    return os << "(Clipping Rectangle: " << r.mXmin << ", " << r.mXmax << ", " 
              << r.mYmin << ", " << r.mYmax << ")";
}


