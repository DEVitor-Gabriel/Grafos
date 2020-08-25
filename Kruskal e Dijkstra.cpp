/*

			ALGORITMOS DE KRUSKAL E DIJKSTRA
	
	
*/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <sys/types.h>
#include <iostream>
#include <time.h>

using namespace std;

#define MaxDim  100
#define Rand_Max  100000


int find( int );
void ffunion( int , int );
void seedRandom(unsigned int );
double randomn(void );
double seconds(void );

void gera_grafo(int , double);
void constroi_lista(int );
void Dijkstra(int );
void Kruskal(void  );
void print_listas_AGM_B(int , int , int* );

// Variáveis Globais

int MAdj[MaxDim][MaxDim];
int Dist[MaxDim][MaxDim];

int LisAdjP[MaxDim][MaxDim];
int LisAdjN[MaxDim][MaxDim];
int CardP[MaxDim];
int CardN[MaxDim];

int M_arcos; // Numero de Arcos no Grafo
int I_arco[MaxDim*MaxDim]; //  I_arco[k] Inicio(Origem) do Arco k
int J_arco[MaxDim*MaxDim]; //  J_arco[k] Fim(Destino) do Arco k

int Hq, Tq;
int Dim;

int Q[MaxDim];
int mark[MaxDim];

int DistInfinita;

int AGM_LisAdjP[MaxDim][MaxDim];
int AGM_CardP[MaxDim];

// Programa Principal

int main()
{
	int v,a;
	float  faux;
	double dens;
	
	cout<<"Digite o numero de vertices no grafo: ";
	cin>>Dim;
	
	cout<<"Digite a densidade o grafo: ";
	cin>>faux;

	dens = faux;

	// Geração do Grafo

	gera_grafo(Dim, dens);

	constroi_lista (Dim);
	
	cout<<"Digite o numero do vertice para saber seu alcance: ";
	cin>>v;
	
	cout<<"\n\nKRUSKAL : \n";

    Kruskal();
    
    cout<<"\n\n\nDIJKSTRA : \n\n";
    
    Dijkstra(v);

	return(0);
}

// Algoritmo de Dijkstra

void Dijkstra(int s)
{
	int v,w,k,l, vmin, dmin;
	int d_temp[MaxDim];
	int pred[MaxDim];
	int pos_prox[MaxDim];


    // Inicializacao: d_temp contem um estimativa pessimista
	// da distancia de s a v

	for (v=1; v<=Dim; v++)
	{
		d_temp[v] = DistInfinita;
		pos_prox[v] = 0;
		pred[v] = -1;
	}

    // Caso Base - conhece-se o vértice mais próximo de s
	// (ele mesmo).

	d_temp[s] = 0;
	pred[s] = 0;

	k = 1;
	pos_prox[s] = k;


	// Agora pode-se atualizar as estimativas pessimistas
	// das distancias.

	for(l=1; l<=CardP[s]; l++)
	{
		v = LisAdjP[s][l];
		if (d_temp[v] > d_temp[s]+Dist[s][v])
		{
           d_temp[v] = d_temp[s]+Dist[s][v];
		   pred[v] = s;
		}
	}


    // Passo Indutivo
	// Conhece-se os k vértices mais próximos de s
	// Então conhece-se os k+1 vertices mais próximos de s

    //  O k+1-ésimo mais próximo é o que possui o menor d_temp

    for (k=2; k<=Dim; k++)
	{
       // Encontra o k-ésimo vértice mais próximo de s

	   vmin = 0;
	   dmin = DistInfinita;
       for (v=1; v<=Dim; v++)
	   {
		   if ((d_temp[v] < dmin) && (pos_prox[v] == 0))
		   {
               vmin = v;
               dmin = d_temp[v];
		   }
	   }

		cout<<"Vmin "<<vmin<<" DistMin: "<<dmin<<"\n";
	   	pos_prox[vmin] = k;


	  // Atualiza-se as estimativas pessimistas
	  // das distancias.

   	  for(l=1; l<=CardP[vmin]; l++)
	  {
	 	 v = LisAdjP[vmin][l];
		 if ( (pos_prox[v] == 0) && (d_temp[v] > d_temp[vmin] + Dist[vmin][v])) //d_temp[vmin]+Dist[s][v])
		 {
            d_temp[v] = d_temp[vmin] + Dist[vmin][v]; //d_temp[vmin]+Dist[s][v];
		    pred[v] = vmin;
		 }
	  }

	}

	// Distancias e caminhos mais curtos
	cout<<"\nDIJKSTRA -- CMC a partir de "<<s<<"\n\n";
	for (v=1; v<=Dim; v++)
	{
		cout<<" "<<v<<" D = "<<d_temp[v]<<" "<<pos_prox[v]<<"-esimo mais proximo Caminho: ";
		w = v;
		while (pred[w] > 0)
		{
			cout<<" "<<pred[w]<<" ";
			w = pred[w];
		}
		cout<<"\n\n";
	}

	for (v=1; v<=Dim; v++)
	{
			cout<<"Aresta: "<<v<<" "<<pred[v]<<" "<<Dist[v][pred[v]]<<" \n";
    }

}

//====================================================


int pont[MaxDim];
int rank[MaxDim];

int find(int v)
{
    if ( pont[v] != v)
    {
         return( find(pont[v]) );
    }
    return(v);
}

void ffunion( int c1, int c2 )
{
   if (rank[c1] < rank[c2])
   {
       pont[c1] = c2;
   }
   else
   {
       pont[c2] = c1;
       if ( rank[c1] == rank[c2] )
       {
            rank[c1]++;
       }
   }
}


void Kruskal( )
{
	int v,w,k, ll;
	int v1, v2, w1, w2, c1, c2;
	int temp;
	int pred[MaxDim];
	int ind_sort[MaxDim*MaxDim];

    // Inicializacao: Caso Base - conhece o CMC de s
	// a todos os outros vertices utilizando no minimo ZERO
	// arcos.
    // Init Union and Find  ponteiros e ranks

	for (v=1; v<=Dim; v++)
	{
		pont[v] = v;
		rank[v] = 0;
	}

    //  Encontra ordem crescente das arestas

    for (k=1; k<=M_arcos; k++)
	{
        ind_sort[k] = k;
    }
    for (ll=1; ll<=M_arcos; ll++)
	{
		for(k=1; k<=M_arcos-1; k++)
		{
            v1 = I_arco[ind_sort[k]];
            w1 = J_arco[ind_sort[k]];
            v2 = I_arco[ind_sort[k+1]];
            w2 = J_arco[ind_sort[k+1]];
			if( Dist[v1][w1] > Dist[v2][w2] )
			{
                temp = ind_sort[k];
                ind_sort[k] = ind_sort[k+1];
                ind_sort[k+1] = temp;
			}
		}
	}

	// Distancias e caminhos mais curtos

    for (k=1; k<=M_arcos - 1; k++)
	{
         v = I_arco[ind_sort[k]];
         w = J_arco[ind_sort[k]];
         cout<<" \nA "<<v<<"  "<<w<<"  "<<Dist[v][w]<<"\n";
         c1 = find(v);
         c2 = find(w);

         if (c1 != c2)
         {
                cout<<" \nAresta "<<v<<"  "<<w<<"  "<<Dist[v][w]<<"\n";
                ffunion(c1,c2);
         }
     }

}

//  Gera Grafo

void gera_grafo(int n, double dens)
{
	int i,j;

	M_arcos = 0;
	DistInfinita = 1;
	cout<<"\nGerando Grafo Aleatorio de Densidade: "<<dens<<" #Vertices : "<<n<<"\n\n";
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
		  if(i != j)
		  {
            if(randomn() < dens)
			{
				M_arcos++;
				I_arco[M_arcos] = i;
				J_arco[M_arcos] = j;
				MAdj[i][j]=1;
				Dist[i][j]=  600 - (int)(randomn() * 1000.);
		        DistInfinita += Dist[i][j];
			}
			else
			{
				MAdj[i][j]=0;
			}
          }
          else
          {
			MAdj[i][j]=0;
          }
		}
	}

	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			if(MAdj[i][j]==0)
			{
				Dist[i][j]=DistInfinita;
			}
		}
	}

}

// Constroi Lista de Adjacência

void  constroi_lista(int n)
{
	int i,j,p;

	for (i=1; i<=n; i++)
	{
		CardP[i] = 0;
		CardN[i] = 0;
	}

	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			if (MAdj[i][j] == 1)
			{
				p = CardP[i] + 1;
				LisAdjP[i][p] = j;
				CardP[i] = p;

				p = CardN[j] + 1;
				LisAdjN[j][p] = i;
				CardN[j] = p;
			}
		}
	}

}

// System dependent routines
// File: system.cpp


void seedRandom(unsigned int seed)
// seed for random number generator.
{
  srand(seed);
  return;
}

double randomn(void)
// random number between 0.0 and 1.0 (uncluded).
{
  double rrr;

  rrr = (double) rand() / (double) RAND_MAX;
  return rrr;
}

double seconds()
// cpu time in seconds since start of run.
{
  double secs;

  secs = (double)(clock() / 1000.0);
  return(secs);
}

