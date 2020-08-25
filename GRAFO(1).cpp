#include <iostream>

#define MAXNUMVERTICES 100
#define MAXNUMARESTAS 4500
#define TRUE 1
#define FALSE 0
#define MAXTAM (MAXNUMVERTICES + MAXNUMARESTAS * 2)

typedef int TipoValorVertice;
typedef int TipoPeso;
typedef int TipoTam;
typedef struct TipoGrafo {
TipoTam Cab[MAXTAM + 1];
TipoTam Prox[MAXTAM + 1];
TipoTam Peso[MAXTAM + 1];
TipoTam ProxDisponivel;
char NumVertices;
short NumArestas;
} TipoGrafo;

typedef short TipoApontador;

void FGVazio(TipoGrafo *Grafo)
	{
		short i ;
		for ( i = 0; i <= Grafo->NumVertices; i++)
		{ 
			Grafo->Prox[ i ] = 0; Grafo->Cab[ i ] = i ;
			Grafo->ProxDisponivel = Grafo->NumVertices;
		}
	}

void InsereAresta(TipoValorVertice *V1, TipoValorVertice *V2, TipoPeso *Peso, TipoGrafo *Grafo)
	{
		short Pos;
		Pos = Grafo->ProxDisponivel;
		if (Grafo->ProxDisponivel == MAXTAM)
			{
				printf ( "nao ha espaco disponivel para a aresta\n" ) ; return;
			}
		Grafo->ProxDisponivel++;
		Grafo->Prox[Grafo->Cab[*V1]] = Pos;
		Grafo->Cab[Pos] = *V2; Grafo->Cab[*V1] = Pos;
		Grafo->Prox[Pos] = 0; Grafo->Peso[Pos] = *Peso;
	}

short ExisteAresta(TipoValorVertice Vertice1 , TipoValorVertice Vertice2 , TipoGrafo *Grafo)
	{
		TipoApontador Aux;
		short EncontrouAresta = FALSE;
		Aux = Grafo->Prox[Vertice1 ] ;
		while (Aux != 0 && EncontrouAresta == FALSE)
			{
				if ( Vertice2 == Grafo->Cab[Aux])
					EncontrouAresta = TRUE;
				Aux = Grafo->Prox[Aux] ;
			}
		return EncontrouAresta;
	}

/* Operadores para obter a lista de adjacentes */
short ListaAdjVazia(TipoValorVertice *Vertice , TipoGrafo *Grafo)
	{
		return (Grafo->Prox[*Vertice ] == 0); 
	}
	
TipoApontador PrimeiroListaAdj(TipoValorVertice *Vertice , TipoGrafo *Grafo)
	{
		return (Grafo->Prox[*Vertice ] ) ; 
	}
	
void ProxAdj(TipoValorVertice *Vertice , TipoGrafo *Grafo, TipoValorVertice *Adj , TipoPeso *Peso, TipoApontador *Prox, short *FimListaAdj)
	{ /* Retorna Adj apontado por Prox */
		*Adj = Grafo->Cab[*Prox ] ; *Peso = Grafo->Peso[*Prox] ;
		*Prox = Grafo->Prox[*Prox] ;
		if (*Prox == 0) *FimListaAdj = TRUE;
	}

void RetiraAresta(TipoValorVertice *V1, TipoValorVertice *V2, TipoPeso *Peso, TipoGrafo *Grafo)
	{
		TipoApontador Aux, AuxAnterior ; short EncontrouAresta = FALSE;
		AuxAnterior = *V1; Aux = Grafo->Prox[*V1] ;
		while (Aux != 0 && EncontrouAresta == FALSE)
			{
				if (*V2 == Grafo->Cab[Aux] ) EncontrouAresta = TRUE;
					else
						{
							AuxAnterior = Aux; Aux = Grafo->Prox[Aux] ;
						}
			}
			if (EncontrouAresta) /* Apenas marca como retirado */
				{
					Grafo->Cab[Aux] = MAXNUMVERTICES + MAXNUMARESTAS * 2;
				}
				else printf ( "Aresta nao existe \n" );
	}

void LiberaGrafo(TipoGrafo *Grafo)
	{ /* Nao faz nada no caso de posicoes contiguas */
	}
	
void ImprimeGrafo(TipoGrafo *Grafo)
	{
		short i , forlim ;
		printf ( " Cab Prox Peso\n" );
		forlim = Grafo->NumVertices + Grafo->NumArestas * 2;
		for ( i = 0; i <= forlim - 1; i++)
		printf ( "%2d%4d%4d%4d\n" , i , Grafo->Cab[ i ] ,
		Grafo->Prox[ i ] , Grafo->Peso[ i ] ) ;
	}
	
	
		
void VisitaDfs(TipoValorVertice u, TipoGrafo *Grafo, TipoValorTempo* Tempo, TipoValorTempo* d, TipoValorTempo* t , TipoCor* Cor, short* Antecessor)
	{
		char FimListaAdj ; TipoValorAresta Peso; TipoApontador Aux;
		TipoValorVertice v; Cor[u] = cinza; (*Tempo)++; d[u] = (*Tempo);
		printf ( "Visita%2d Tempo descoberta:%2d cinza\n" , u, d[u]) ; getchar ();
		if ( ! ListaAdjVazia(&u, Grafo))
			{
				Aux = PrimeiroListaAdj(&u, Grafo) ; FimListaAdj = FALSE;
				while ( ! FimListaAdj)
					{
						ProxAdj(&u, &v, &Peso, &Aux, &FimListaAdj);
						if (Cor[v] == branco)
							{
								Antecessor[v] = u; VisitaDfs(v, Grafo, Tempo, d, t , Cor, Antecessor);
							}
					}
			}
		Cor[u] = preto ; (*Tempo)++; t [u] = (*Tempo);
		printf ( "Visita%2d Tempo termino:%2d preto\n" , u, t [u] ) ; getchar ();
	}


void BuscaEmProfundidade(TipoGrafo *Grafo)
	{
		TipoValorVertice x;
		TipoValorTempo Tempo;
		TipoValorTempo d[MAXNUMVERTICES + 1] , t [MAXNUMVERTICES + 1];
		TipoCor Cor[MAXNUMVERTICES+1];
		short Antecessor[MAXNUMVERTICES+1];
		Tempo = 0;
		for (x = 0; x <= Grafo->NumVertices - 1; x++)
			{
				Cor[x] = branco;
				Antecessor[x] = -1;
			}
		for (x = 0; x <= Grafo->NumVertices - 1; x++)
			{
				if (Cor[x] == branco)
				VisitaDfs(x, Grafo, &Tempo, d, t , Cor, Antecessor);
			}
	}


void BuscaEmLargura(TipoGrafo *Grafo)
	{
		TipoValorVertice x;
		int Dist [MaxNumvertices + 1];
		TipoCor Cor[MaxNumvertices + 1];
		int Antecessor[MaxNumvertices + 1];
		for (x = 0; x <= Grafo -> NumVertices - 1; x++)
			{
				Cor[x] = branco; Dist [x] = Infinito ; Antecessor[x] = -1;
			}
		for (x = 0; x <= Grafo -> NumVertices - 1; x++)
			{
				if (Cor[x] == branco)
					VisitaBfs (x, Grafo, Dist , Cor, Antecessor);
			}
	}

/* * Entram aqui os operadores FFVazia, Vazia, Enfileira e Desenfileira do * */
/* * do Programa 3.18 ou do Programa 3.20, dependendo da implementação * */
/* * da busca em largura usar arranjos ou apontadores, respectivamente * */

void VisitaBfs(TipoValorVertice u, TipoGrafo *Grafo, int *Dist , TipoCor *Cor, int *Antecessor)
	{
		TipoValorVertice v; Apontador Aux; short FimListaAdj;
		TipoPeso Peso; TipoItem Item ; TipoFila Fila ;
		Cor[u] = cinza ; Dist [u] = 0;
		FFVazia(&Fila );
		Item. Vertice = u; Item.Peso = 0;
		Enfileira (Item, &Fila );
		printf ( "Visita origem%2d cor : cinza F: " , u);
		ImprimeFila( Fila ) ;
		getchar ();
	}


while ( ! FilaVazia( Fila ))
	{
		Desenfileira(&Fila , &Item);
		u = Item. Vertice ;
		if ( ! ListaAdjVazia(&u, Grafo))
			{
				Aux = PrimeiroListaAdj(&u, Grafo);
				FimListaAdj = FALSE;
				while (FimListaAdj == FALSE)
					{
						ProxAdj(&u, &v, &Peso, &Aux, &FimListaAdj);
						if (Cor[v] != branco) continue;
						Cor[v] = cinza ; Dist [v] = Dist [u] + 1;
						Antecessor[v] = u; Item. Vertice = v;
						Item.Peso = Peso; Enfileira (Item, &Fila );
					}
			}
		Cor[u] = preto;
		printf ( "Visita %2d Dist %2d cor : preto F: " , u, Dist [u] ) ;
		ImprimeFila( Fila ) ; getchar ();
	}
 /* VisitaBfs */


int main()
{
    TipoGrafo obj;

    obj.menu();
    return 0;
}
