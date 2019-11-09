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
                //cout<<"foi0"<<endl;
                return NULL;
            }
        }
        else{
            t=numerador[i]/denominador[i];
            if(denominador[i]>0){
                if(t>fim){
                    //cout<<"foi1"<<endl;
                    return NULL;
                }
               if(t>inicio){
                   cout<<"foi2"<<endl;
                    inicio=t;
                }
            }
            else{
                if(t<inicio){
                   // cout<<"foi3"<<endl;
                    return NULL;
                }
                if(t<fim){
                    cout<<"foi4"<<endl;
                    fim=t;
                }
            }
        }  
       // cout<<"valor de t: "<<t<<endl;  
   }
   //cout<<"valor de t: "<<t<<" "<<"valor inicio e fim: "<<inicio<<" "<<fim<<endl;
   if(inicio==0 and fim==1){
       cout<<"foiteste"<<endl;
       linha->mP0=l.mP0;
       linha->mP1=l.mP1;
       return linha;
   }
   else if(inicio!=0 and fim!=1){
       linha->mP0=l.mP0+(l.mP1-l.mP0)*inicio;
       linha->mP1=l.mP0+(l.mP1-l.mP0)*fim;
   }
   
   else if(inicio!=0){
       
        ponto=l.mP0+(l.mP1-l.mP0)*inicio;
        linha->mP0=ponto;
        linha->mP1=l.mP1;
   }
   else if(fim!=1){
       cout<<"aqui";
        ponto=l.mP0+(l.mP1-l.mP0)*fim;
        linha->mP0=l.mP0;
        linha->mP1=ponto;
   }
   cout<<endl;
   cout<<endl;
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


