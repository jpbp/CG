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
   Line *linha = new Line;
   Point ponto; 
   double t;
   double inicio=0;
   double fim=1;
   double numerador[]={mXmin - l.mP0.mX, l.mP0.mX - mXmax, l.mP0.mY - mYmax, mYmin-l.mP0.mY};
   double denominador[]={l.mP1.mX - l.mP0.mX, l.mP0.mX - l.mP1.mX, l.mP0.mY - l.mP1.mY, l.mP1.mY-l.mP0.mY};
   for (size_t i = 0; i < 4; i++)
   {
        if(denominador[i]==0){
            if(numerador[i]>0){
                return NULL;
            }
        }
        else{
            t=numerador[i]/denominador[i];
            if(denominador[i]>0){
                if(t>fim){
                    return NULL;
                }
               if(t>inicio){
                    inicio=t;
                }
            }
            else{
                if(t<inicio){
                    return NULL;
                }
                if(t<fim){
                    fim=t;
                }
            }
        }    
   }
   if(inicio==0 and fim==1){
       linha->mP0=l.mP0;
       linha->mP1=l.mP1;
       return linha;
   }
   if(inicio!=0){
       cout<<"valor de t: "<<t<<" "<<"valor inicio e fim: "<<inicio<<" "<<fim<<endl;
        ponto=l.mP0+(l.mP1-l.mP0)*inicio;
        linha->mP0=ponto;
        linha->mP1=l.mP1;
   }
   if(fim!=1){
       cout<<"valor de t: "<<t<<" "<<"valor inicio e fim: "<<inicio<<" "<<fim<<endl;
        ponto=l.mP0+(l.mP1-l.mP0)*fim;
        //linha->mP0=l.mP0;
        linha->mP1=ponto;
   }
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


