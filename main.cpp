#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <limits>
#include <vector>
#include <algorithm>
#include <string>
#include <ifstream>
#include <fstream>
#include <stdlib.h>

using namespace std;

void leerimagen(const char* nombre){
  ifstream file(nombre);
  char imageName[200];
  vector<pair<vector<string>,vector<int> > image;
  if(file.is_open()){
    while(file.getline(imageName,200,'\t')){
      image.push_back(make_pair(imageName,200));
    }
    cout<<image[0]<<endl;    
  }
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
        for( ; it!= m_Aristas.end(); ++it){
          if(it->m_pIni->m_Dato == m_Dato){
              os<<m_Dato<<"->"<< it->m_pDes->m_Dato  <<" [label=\""<< it->m_Dato<<"\" ]"<<endl;
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
    	if(find_vertex(v1) != 0)return;
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

        file<<"digraph {"<<endl;

          file<<"edge [dir=none];"<<endl;

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
             cout<<"entra"<<p->m_Dato<<endl;
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

  cout<<"Hola"<<endl;
  //leerimagen("salidaHaar.txt");
Grafo<int,int>  G;

    G.Add_Vertex(0);
    G.Add_Vertex(1);
    G.Add_Vertex(2);
    G.Add_Vertex(3);
    G.Add_Vertex(4);
    G.Add_Vertex(5);
    G.Add_Vertex(6);
    G.Add_Vertex(7);
    G.Add_Vertex(8);
    G.Add_Arista(0,1,4);
    G.Add_Arista(0,7,8);
    G.Add_Arista(1,2,8);
    G.Add_Arista(1,7,11);
    G.Add_Arista(2,8,2);
    G.Add_Arista(2,3,7);
    G.Add_Arista(3,4,9);
    G.Add_Arista(3,5,10);
    G.Add_Arista(5,2,4);
    G.Add_Arista(5,4,10);
    G.Add_Arista(6,5,2);
    G.Add_Arista(6,8,6);
    G.Add_Arista(7,6,1);
    G.Add_Arista(7,8,7);

    G.printDot("normal.dot");
    G.Prim().printDot("prim.dot");
    system("dot -Tpng normal.dot -o normal.png");
    system("dot -Tpng prim.dot -o prim.png");
    //G.Prim().print();
    //G.profundidad();
    //G.amplitud();
    	system("read");
    cout << "Hello world!" << endl;
    return 0;
}