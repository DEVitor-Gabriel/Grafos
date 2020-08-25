#include <iostream>
#include<math.h>
#include<bits/stdc++.h>

#define INFINITO INT_MAX
#define INF 1

using namespace std;
template<class type>
class no
{
public:
    type dado; // dado que será colocado em uma estrutura
    no * next; // ponteiro que apontta para o proximo
    no * previous;//ponteiro que aponta para o anterior
    static no<type> *montano(type *dado);
    static void desmontaNo(no<type> *sair, type *dado);
    no();
};
template<class type>
no<type> *no<type>::montano(type *dado){
    try{
        no<type> *retornar = new no<type>();
        retornar->dado = *dado;
        return retornar;
    }catch(std::bad_alloc &erro){
        throw erro;
    }
    catch(...){
        throw std::string("Erro desconhecido!\n");
    }
}

template<class type>
void no<type>::desmontaNo(no<type> *sair, type *dado){
    *dado =sair->dado;
    delete sair;
}
template<class type>
no<type>::no(){
    previous = NULL;
    next = NULL;
}

template<class type>
class List;
template<class type>
class iterato // iterator para ajudar a percorer a lista
{
private:
    friend class List<type>;
    no<type> *atual;
public:
    bool operator !=(no<type> * cmp){
        return atual!=cmp;
    }
    void operator=(no<type> *pt){
        atual=pt;
    }
    void operator=(iterato &pt){
        atual=pt.atual;
    }
    type operator*(){
        return atual->dado;
    }
    bool operator==(no<type> *cmp){
        return cmp==atual;
    }
    void operator++(){
        atual = atual->next;
    }
    void operator+(int index){
        for(int i =0;i<index;i++)atual = atual->next;
    }
    void operator--(){
        atual=atual->previous;
    }
    iterato(no<type> *at){atual=at;}
    iterato(){atual=NULL;}

};
template<class type>
class List{
protected:
    no<type> *begin; // inicio da lista
    no<type> *end; // final da lista
    int amount; // quantidade
public:
    typedef iterato<type> iterator;
    type pop_backType();
    type front()const{return begin->dado;}
    type back()const{return end->dado;}
    void push_back(type &dado);///Insere elemento no final da lista
    void push_front( type & dado);// insere elemento no começo da lista
    no<type> *Begin(){return begin;}// retorna o começo da lista
    no<type> *End(){return end;}//retorna o final da lista
    type &operator[](int index); // para inseriri um elemento em uma posição
    int size()const{int i;no<type> * aux = begin;for(i=0;aux;i++,aux=aux->next);return i;}// tamanho da lista
    bool empty(){return amount==0;}// verificar se a lisra é vazia
    void pop_back();//retirar do final
    void pop_front();/// retirar do inicio
    no<type> *seach(iterator &it);//procucar com iterator
    bool seach(const type &value); // procurar com valor
    void insert(iterator &it,type); // inserir em uma possição dada
    void priority_queue(type dado);
    void clear();
    List();
    ~List();
};
template<class type>
type List<type>::pop_backType(){
    if(!amount)throw std::string("Lista vazia!\n");
    no<type> *aux=end;
    end = aux->previous;
    end->next=NULL;
    type dado;
    amount--;
    no<type>::desmontaNo(aux,&dado);
    return dado;
}
template<class type>
void List<type>::insert(iterator &it, type d){
    try{
        no<type> *pt=no<type>::montano(&d);

        if(it.atual==begin){ // caso seja o inicio  faz se um push_front , insere na frente
            push_front(d);
            return;
        }
        if(it.atual==end){ // caso seja o final iserir no final
            push_back(d);
            return;
        }

        pt->next = it.atual->next; //inserir o elemento na posicao que o iterator passou
        pt->previous = it.atual;
        it.atual->next = pt;
        pt->next->previous = pt;
        amount++;
    }catch(std::bad_alloc & erro){
        throw erro;
    }catch(...){
        throw std::string("Erro desconhecido!\n");
    }
}

template<class type>
void List<type>::priority_queue(type dado){
    if(empty())return push_back(dado);
    no<type> *aux;int i;
    if(dado<begin->dado)return push_front(dado);
    for(i=0,aux = begin;aux&&dado>aux->dado;aux=aux->next,i++);
    if(!aux)return push_back(dado);
    iterator it= aux->previous;
    if(it==begin){
        no<type> *pt=no<type>::montano(&dado);
        pt->next = it.atual->next; //inserir o elemento na posicao que o iterator passou
        pt->previous = it.atual;
        it.atual->next = pt;
        pt->next->previous = pt;
        amount++;
        return;
    }
    insert(it,dado);
}
template<class type>
void List<type>::clear(){
    no<type> *aux ;
    type aux1;
    while(begin){
        aux = begin;
        begin = aux->next;
        no<type>::desmontaNo(aux,&aux1);
        amount--;
    }
}
template <class type>
List<type>::~List() {//destrutor
    clear();
}
template<class type>
bool List<type>::seach(const type &value){
    int i;
    no<type> *aux=begin;
    for(i=0;i<amount&&aux->dado!=value;aux=aux->next,i++);
    return i<amount;
}

template <class type>
no<type> *List<type>::seach(iterator &it){ // proxurar um elemento na lista e retornar o iterator , caso não ache retorna NULL
    int i;
    no<type> *aux=begin;
    for(i =0;i<amount&&aux!=it.atual;i++,aux->next);
    if(i>= amount)return NULL;
    return aux;
}
template<class type>
void List<type>::pop_front(){
    if(!amount)throw std::string("Lista vazia!\n");
    no<type> *aux=begin;
    type dado;
    begin = aux->next;
    if(begin==NULL){end=begin=NULL;amount =0;}
    else{
        begin->previous=NULL;
        amount--;
        no<type>::desmontaNo(aux,&dado);
    }
}
template<class type>
void List<type>::pop_back(){
    if(!amount)throw std::string("Lista vazia!\n");
    no<type> *aux=end;
    end = aux->previous;
    end->next=NULL;
    type dado;
    amount--;
    no<type>::desmontaNo(aux,&dado);
}

template <class type>
type &List<type>::operator [](int index)
{
    if(index<0||index>amount)throw std::string("Posição invalida!\n");
    no<type> *aux=begin;
    for(int i=0;i<index;i++)
        aux=aux->next;
    return aux->dado;
}
template<class type>
List<type>::List() // contrutor para começar com a lista vazia , apontando o final e o começo para null
{
    begin = NULL;//começado a lista vazia , apontando para nulo
    end = NULL;
    amount = 0;
}
template<class type>
void List<type>::push_back(type &dado) // inserir novo elemento no final da lista
{
    try{
        no<type> * novo = no<type>::montano(&dado);
        if(!amount){// se a lista não conter nehum elemento
            begin=novo;
            end=novo;  // inicio e começo aponta para o elemento
        }
        else{//chegou aqui existe um ou mais elementos

            end->next =novo;//fazendo o ultimo apontar para o novo ultimo
            novo->previous = end; // fazendo o novo ultimo apontar para o final antingo
            end = novo; // colocando o final para apontar para o elemento que entrou
        }
        amount++;//incrementando a quantidade de elementos
    }catch(std::bad_alloc &erro){
        throw erro;
    }
    catch(...){
        throw std::string("Erro desconhecido!\n");
    }

}
template<class type>
void List<type>::push_front(type &dado)//inserir elemento no inicio da lista
{
    try{
        no<type> * novo = no<type>::montano(&dado);
        if(!amount){//a lista não possui nenhum elemento quantidade =0
            begin = novo;
            end = novo;
        }
        else{//chegou aqui quer dizer que possui pelo menos um elemento
            begin->previous = novo;//o anterior da lista aponta para o novo elemento
            novo->next = begin;// o proximo do novo elemento aponta para o inicio da lista
            begin = novo;// begin recebe o novo inicio
        }
        amount++;
    }catch(std::bad_alloc &erro){
        throw erro;
    }catch(...){
        throw std::string("Erro desconhecido!\n");
    }
}

using namespace std;
// inline é para deixar a chamada na função mais rapida
template<class type1,class type2>
class myPair
{
public:
    type1 first;
    type2 second;
    myPair<type1,type2> &operator=(const myPair<type1,type2> &);//atribuição
    bool operator ==(const myPair<type1,type2> &);//overloading sing equal
    bool operator !=(const myPair<type1,type2> &); // diference
    bool operator > (const myPair<type1,type2> &); // bigger
    bool operator >=(const myPair<type1,type2> &); // bigger equal
    bool operator < (const myPair<type1,type2> &);
    bool operator <=(const myPair<type1,type2> &);
    friend  ostream &operator<<(ostream & os,myPair<type1,type2> &__p){
        os<<"[|P| "<<__p.second<<" ,|V| "<<__p.first<<" ]";
        return os;}


    //myPair();
};
template<class type1, class type2>
inline myPair<type1,type2> &myPair<type1,type2>::operator=(const myPair<type1, type2> &__p){//atribuição de um myPair

    first = __p.first;
    second = __p.second;
    return *this;

}

template<class type1, class type2>
inline bool myPair<type1,type2>::operator ==(const myPair<type1, type2> &__p){ // verificar se são iguais
    return (__p.first==first&&second==__p.second);
}

template<class type1, class type2>
inline bool myPair<type1,type2>::operator !=(const myPair<type1, type2> &__p){
    return !(this==__p);
}

template<class type1, class type2>
inline bool myPair<type1,type2>::operator >(const myPair<type1, type2> &__x){ //!(__x.first>first) verificando se os dois são iguais
    return (first>__x.first||(!(__x.first>first)&&__x.second<second)); // se O primiro são iguais então compara o segundo
}

template<class type1, class type2>
inline bool myPair<type1,type2>::operator >=(const myPair<type1, type2> &__p){ // verificando se e maior ou igual
    return (__p>this||__p==this);
}

template<class type1, class type2>
inline bool myPair<type1,type2>::operator <(const myPair<type1, type2> &__p){//verificar se e menor ,se o primeiro for igual compara o segundo
    return  first<__p.first||(!(__p.first<first)&&second<__p.second);
}

template<class type1, class type2>
inline bool myPair<type1,type2>::operator <=(const myPair<type1, type2> &__p){
    return (this<__p||this==__p);
}


class Aresta{
public:
    Aresta(){}
    myPair<int,int> __celula;
    int vertice;
    void setCelula(myPair<int,int> & setar){__celula = setar;}
    void setVertice(int & vertex){this->vertice=vertex;}
    friend istream &operator>>(istream & is,Aresta & nova){
        do{
            cout<<"DIGITE O VERTICE DE ORIGEM : ";
            is>>nova.vertice;
            cout<<"DIGITE O VERTICE DE DESTINO : ";
            cin>>nova.__celula.first;
            cout<<"DIGITE O PESO DO VERTICE DE DESTINO : ";
            is>>nova.__celula.second;
        }while(nova.vertice<0||nova.__celula.first<0||nova.__celula.second<0);
        return is;
    }
    bool operator < (Aresta & aresta2)const{
        return __celula.second<aresta2.__celula.second;
    }

};

typedef myPair<int,int> celula;
#define INF 1
class Grafo
{
private:
    List<celula> **G;
    int amount;
    void dfs(int vertice, bool *vet);
    void bfs(int &__x, int *vet);

public:
    void imprimir();//imprime o grafo
    void insertGrafo(Aresta &vertice);//vertex vetice que ela esta ligada;
    bool buscaEmProfundidade(int &vertex); // função de busca DFS
    void imprimirvertice(int &vertex); // imprimir um vertice
    bool verificaAresta(Aresta &);//verifica se existe uma aresta
    void imprimirVerciceAdj(int &index);//imprimir vertices adijacentes de um determinado vertice
    bool buscaEmLargura(int &__x);
    int MenorCaminho(int origem,int destino);
    void caminho(myPair<int,int> * aux);
    void imprimir(myPair<int,int> * aux);
    int buscar(int * subset,int v){if(subset[v]==-1)return v;return buscar(subset,subset[v]);}

    Grafo(int tam);
};
void Grafo::dfs(int vertice, bool *vet) {// busca em profundidade , vet e o vetor para verificar se foi vizidado
    vet[vertice] = true; // marcar que foi vistado

    for(int i=0;i<G[vertice]->size();i++){
        celula ii =G[vertice]->operator [](i);
        if(!vet[ii.first]){cout<<"\nVisitando : "<<ii<<endl;dfs(ii.first,vet);}
    }
}

void Grafo::bfs(int &__x, int *vet){
    celula ii; //  celula auxiliar para pegar o vertice que estamos
    List<int> fila;// fila para usar para percorrer
    fila.push_back(__x); // enquanto tiver elemento
    vet[__x]=0;//vetor de distancias começando desse vertice
    while(!fila.empty()){
        int __u=fila.front();// retira e retorna o elemento
        fila.pop_front();
        for(int i =0; i < G[__u]->size();i++){
            ii = G[__u]->operator [](i); // pegando a celula que estamos
            if(vet[ii.first]==-1){//verificando se ela já foi visitada
                fila.push_back(ii.first); // colocando na fila porque não foi visitado
                vet[ii.first] = vet[__u]+1;// marcando como visitado
                cout<<"VISITANDO : ";
                cout<<ii.first;
                puts("");

            }
        }
    }
}
void Grafo::imprimir(){
    puts("");
    celula aux;
    for(int i=0;i<amount;i++){
        cout<<" ["<<i<<" ] ->";
        for(int j=0;j<G[i]->size();j++){
            aux = G[i]->operator [](j);
            cout<<aux.first<<" -> ";
        }puts("NULL");
    }
}

void Grafo::insertGrafo(Aresta & vertice){
    try{
        if(vertice.vertice<0||vertice.vertice>amount)throw string("Posição de grafo invalida!\n");
        G[vertice.vertice]->push_back(vertice.__celula); // colocando a celula no grafo
    }catch(bad_alloc &erro){
        throw erro;
    }
}

bool Grafo::buscaEmProfundidade(int &vertex){
    if(vertex<0||vertex>amount) throw "Não existe o vertice";
    bool dfs_num[amount];// vetor para marcar o vertice que foi vizidado
    memset(dfs_num,false,sizeof(bool)*amount); // enchendo o vetor com falso
    cout<<"Visitando : "<<vertex<<endl;
    dfs(vertex,dfs_num);// vertex e o o vertice que deve começar
    int i;
   // for(i=0;i<amount;i++)cout<<"  "<<dfs_num[i];
    puts("");
    for(i =0;i<amount;i++){if(!dfs_num[i]){cout<<"Visistando :"<<i<<endl,dfs(i,dfs_num);}};
    return true;
}

void Grafo::imprimirvertice(int &vertex){
    if(vertex<0||vertex>amount) throw ("Não existe o vertice");
    puts("");//pular de linha
    cout<<" ["<<vertex<<"] ->";
    for(int i =0;i<G[vertex]->size();i++){
        celula ii = G[vertex]->operator [](i);
        cout<<ii.first<<" ->";
    }puts("NULL");
}

bool Grafo::verificaAresta(Aresta & verificar){ // vertex é a aresta de origem e celula(destino,peso) a de destino
    for(int i=0;i<G[verificar.vertice]->size();i++){//vertice de origem é uma lista e celula um elemento
        if(verificar.__celula==G[verificar.vertice]->operator[](i))return true;
    }return false;
}

void Grafo::imprimirVerciceAdj(int &index){
    celula ii;
    puts("");//Função Pular de linha
    cout<<"[ "<<index<<" ] -> ";
    for(int i=0;i<G[index]->size();i++){
        ii = G[index]->operator [](i);
        cout<<"[ "<<ii.first<<" ] -> ";
    }puts("NULL");
}

bool Grafo::buscaEmLargura(int &__x){
    int i;
    if(__x<0||__x>amount) throw ("Não existe o vertice");
    int bfs_num[amount];// vetor para marcar o vertice que foi vizidado
    cout<<"PARTINDO VERTICE : "<<__x<<endl;
    memset(bfs_num,-1,sizeof(int)*amount); // enchendo o vetor com falso
    bfs(__x,bfs_num);
    for(i =0;i<amount;i++)cout<<" "<<bfs_num[i];
    puts("");
    for(i =0;i<amount;i++){if(bfs_num[i]==-1)cout<<"VISITANDO : "<<i<<endl,bfs(i,bfs_num);}
    return true;

}


Grafo::Grafo(int tam){
    if(tam<=0)throw std::string("Tamanho não suportado!\n"); // casoo tamanho inválido
    try{
        G= new List<celula>*[tam];// alocando um vetor de ponteiros para lista de celula
        for(int i =0;i<tam;i++){
            G[i]= new List<celula>();// alocando uma lista para cada posição
        }
        amount = tam;
    }catch(std::bad_alloc &erro){//caso falte memoria
        throw erro;
    }catch(string &erro){
        throw erro;
    }catch(...){
        throw std::string("Erro desconhecido!\n");
    }

}

class Public_Static_Void_Main
{
public:
    Public_Static_Void_Main();
    void Public_Static_Void_MainC();

};
Public_Static_Void_Main::Public_Static_Void_Main(){
}

void Public_Static_Void_Main::Public_Static_Void_MainC()
{
    int tam_Grafo,qtd_aresta,i;
    Aresta inserir;
    cout<<"Digite o tamanho do grafo : ";
    cin>>tam_Grafo;
    cout<<"Digite a quantidade de aresta do grafo : ";
    cin>>qtd_aresta;
    Grafo grafo(tam_Grafo);
    for(i=0;i<qtd_aresta;i++){
        cin>>inserir;
        grafo.insertGrafo(inserir);
    }
    do{
        cout<<"\n\n0 Para sair \n"
              "1 Para imprimir o Grafo\n"
              "2 Para Busca em profundidade DFS\n"
              "3 Para Busca em Largura DFS\n\n";
        cin>>i;
        switch (i) {
        case 0 : continue;
        case 1:
            grafo.imprimir();
            puts("");
            break;
        case 2:////////////////////////////////////////////////
            try{
            int vertex=-1; // vertece que começara a busca
            cout<<"Digite O vertice de partida";
            cin>>vertex;

            grafo.buscaEmProfundidade(vertex);
            puts("");
        }catch(const char *erro){cerr<<erro<<endl;}
            catch(string & erro){cerr<<erro<<endl;}
            catch(...){cout<<"ERRO DESCONHECIDO!\n";}
            break;
        case 3:
            try{
            int vertex=-1; // vertece que começara a busca
            cout<<"Digite O vertice de partida";
            cin>>vertex;
            grafo.buscaEmLargura(vertex);

        }catch(const char *erro){cerr<<erro<<endl;}
            catch(string & erro){cerr<<erro<<endl;}
            catch(...){cout<<"ERRO DESCONHECIDO!\n";}
            break;

        default:
            break;
        }


    }while(i>0);

}

int main()
{
    Public_Static_Void_Main b;
    b.Public_Static_Void_MainC();
}

