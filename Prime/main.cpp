#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <limits>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;


float Euclidiana(vector< pair< string , vector<float> > > &image, int i,int j){
    float resp = 0;
    for (int e = 0; e < 16;e++){
        resp = resp + pow((image[i].second[e] - image[j].second[e]),2);
    }
    return sqrt(resp);
}


vector< pair< string , vector<float> > > leerimagen(const char * nombre){
    ifstream file(nombre);
    char imageName[200];
    vector< pair< string , vector<float> > > image;
    vector<float> caracteristicas;
    if(file.is_open()){
        while(1){
            file>>imageName;
            file.ignore();
            for(int i = 0; i<16;i++){
                float x;
                file>>x;
                caracteristicas.push_back(x);
            }
            image.push_back(pair<string,vector<float> > (imageName,caracteristicas));
            caracteristicas.clear();
            if(file.eof()){
                cout<<"Fin"<<endl;
                break;
            }
        }
        file.close();
    }else{
        cout<<"No se pudo abrir"<<endl;
    }

    return image;

}



template<class V,  class E>
class Vertex;


template<class V,  class E>
class Arista
{
public:
    E               m_Dato;
    Vertex<V,E>  *  m_pIni;
    Vertex<V,E>  *  m_pDes;
public:
    Arista(){};
    Arista(E a, Vertex<V,E> * p =0, Vertex<V,E> * i =0)
    {
        m_Dato = a;
        m_pIni = i;
        m_pDes = p;
    }


    bool operator==(Arista<V,E> & Ar)
    {
        return  m_Dato == Ar.m_Dato;
    }

    bool operator>(const Arista<V,E> & Ar)const
    {
        return  m_Dato > Ar.m_Dato;
    }

    bool operator<(const Arista<V,E> & Ar)const
    {
        return  m_Dato < Ar.m_Dato;
    }

};


template<class V,  class E>
class Vertex
{
public:
    V   m_Dato;
    int tag;
    list< Arista <V,E> >  m_Aristas;
public:
    Vertex(){};
    Vertex(V d)
    {
        tag = 0;
        m_Dato = d;
    }

    void changeTag(int i ){
        tag = i;
    }
    bool operator==(Vertex<V,E> & Ver)
    {
        return  m_Dato == Ver.m_Dato;
    }

    bool operator>(Vertex<V,E> & Ver)
    {
        return  m_Dato > Ver.m_Dato;
    }

    bool operator<(Vertex<V,E> & Ver)
    {
        return  m_Dato < Ver.m_Dato;
    }

    void print()
    {
        typedef typename list< Arista<V,E> >::iterator ITERATOR;
        ITERATOR it = m_Aristas.begin();
        for( ; it!= m_Aristas.end(); ++it)
            cout<<m_Dato << " (" << it->m_Dato << ")" <<it->m_pDes->m_Dato <<endl;

    }

    void printDot(ofstream& os)
    {
        typedef typename list< Arista<V,E> >::iterator ITERATOR;
        ITERATOR it = m_Aristas.begin();
        os<<"\""<<m_Dato<<"\""<<"[image=\""<<m_Dato<<"\", label=\"\"];"<<endl;
        for( ; it!= m_Aristas.end(); ++it){
            if(it->m_pIni->m_Dato == m_Dato){
                os<<"\""<<m_Dato<<"\""<<"->"<<"\""<< it->m_pDes->m_Dato<<"\""  <<"[minlen="<< (int)(it->m_Dato / 1000)<<" label=\""<< it->m_Dato<<"\" ]"<<endl;
                //weight="<< (int)(it->m_Dato / 1000)<<"
            }

        }
    }
};


template<class V, class E>
class Grafo
{
private:
    list< Vertex<V,E> >   m_vertices;
public:
    Grafo(){};
    void Add_Vertex(V v1)
    {
        if(find_vertex(v1) != 0) return;
        m_vertices.push_back(Vertex<V,E>(v1));

    }

    Vertex<V,E> *   find_vertex(V v)
    {
        Vertex<V,E>  tmp(v);
        typedef typename list< Vertex<V,E> >::iterator ITERATOR;
        ITERATOR it = m_vertices.begin();
        for( ; it != m_vertices.end(); ++it)
        {
            if ( tmp == *it)
                return &(*it);
        }

        return 0;

    }

    void print()
    {

        typedef typename list< Vertex<V,E> >::iterator ITERATOR;
        ITERATOR it = m_vertices.begin();
        for( ; it != m_vertices.end(); ++it)
            it->print();
    }

    void printDot(char* p)
    {
        ofstream file(p);

        file<<"digraph {ranksep=3.75;"<<endl;

        file<<"edge [dir=none ];"<<endl;

        typedef typename list< Vertex<V,E> >::iterator ITERATOR;
        ITERATOR it = m_vertices.begin();
        for( ; it != m_vertices.end(); ++it)
            it->printDot(file);

        file<<" }"<<endl;
    }

    void Add_Arista(V v1,  V v2, E ar)
    {
        Vertex<V,E>  * origen = find_vertex(v1);
        Vertex<V,E>  * destino = find_vertex(v2);
        if (origen != 0 && destino != 0)
        {
            Arista<V,E>  Aris(ar,destino,origen);
            origen->m_Aristas.push_back(Aris);
            destino->m_Aristas.push_back(Aris);
        }
    }



    bool formaCiclo(Arista<V,E>  a, int  num, Vertex<V,E> * &p){
        if (a.m_pDes->tag == num  && a.m_pIni->tag == num){
            return true;
        }
        if(a.m_pDes->tag == num) p = a.m_pIni;
        else p = a.m_pDes;
        cout<<a.m_pIni->m_Dato<<"-"<<a.m_pDes->m_Dato<<endl;
        a.m_pDes->changeTag(num);
        a.m_pIni->changeTag(num);
        return false;
    }


    Grafo<V,E>  Prim(){
        typedef typename list< Arista<V,E> >::iterator ITERATORA;
        int cant = 0;
        Grafo<V,E> S;
        Vertex<V,E> *p = &m_vertices.front();
        vector<Vertex<V,E> *> nodos(m_vertices.size());
        nodos[cant] = p;
        S.Add_Vertex(p->m_Dato);
        p->changeTag(1);
        //cout<<S.m_vertices.back().tag<<endl;
        cant++;
        priority_queue<Arista<V,E> ,std::vector<Arista<V,E> >, greater<Arista<V,E> > >minHeap;
        ITERATORA ita = p->m_Aristas.begin();
        for( ; ita!= p->m_Aristas.end(); ++ita){
            minHeap.push(*ita);
        }
        while(cant < m_vertices.size() ){

            if(!S.formaCiclo(minHeap.top(),1, p)){
                //cout<<"entra"<<p->m_Dato<<endl;
                S.Add_Vertex(p->m_Dato);
                //S.m_vertices.back().changeTag(1);
                //cout<<S.m_vertices.back().tag<<endl;
                S.Add_Arista(minHeap.top().m_pIni->m_Dato, minHeap.top().m_pDes->m_Dato, minHeap.top().m_Dato) ;
                cant++;
                minHeap.pop();
                ITERATORA ita = p->m_Aristas.begin();
                for( ; ita!= p->m_Aristas.end(); ++ita){
                    minHeap.push(*ita);
                }
            }else{
                minHeap.pop();
            }
        }
        return S;
    }

};

int main()
{

    vector< pair< string , vector<float> > > img = leerimagen("salidaHaar.txt");
    Grafo<string,float>  G;
    for (int i = 0; i < img.size();i++){
        G.Add_Vertex(img[i].first);
    }

    for (int i = 0; i < img.size();i++){
        for (int j = 0; j < img.size();j++){
            if(i != j) {
                G.Add_Arista(img[i].first,img[j].first,Euclidiana(img,i,j));
            }
        }
    }

/*
    G.Add_Vertex("a0");
    G.Add_Vertex("a1");
    G.Add_Vertex("a2");
    G.Add_Vertex("a3");
    G.Add_Vertex("a4");
    G.Add_Vertex("a5");
    G.Add_Vertex("a6");
    G.Add_Vertex("a7");
    G.Add_Vertex("a8");
    G.Add_Arista("a0","a1",4);
    G.Add_Arista("a0","a7",8);
    G.Add_Arista("a1","a2",8);
    G.Add_Arista("a1","a7",11);
    G.Add_Arista("a2","a8",2);
    G.Add_Arista("a2","a3",7);
    G.Add_Arista("a3","a4",9);
    G.Add_Arista("a3","a5",10);
    G.Add_Arista("a5","a2",4);
    G.Add_Arista("a5","a4",10);
    G.Add_Arista("a6","a5",2);
    G.Add_Arista("a6","a8",6);
    G.Add_Arista("a7","a6",1);
    G.Add_Arista("a7","a8",7);
*/
    G.printDot("normal.dot");
    G.Prim().printDot("prim.dot");
//    cout<<"dibujando1"<<endl;
//    system("dot -Tpng normal.dot -o normal.png");
    cout<<"dibujando2"<<endl;
    system("twopi -Tpng prim.dot -o prim.png");
    //G.Prim().print();
    //G.profundidad();
    //G.amplitud();
    //system("read");
    cout << "Hello world!" << endl;
    return 0;
}
