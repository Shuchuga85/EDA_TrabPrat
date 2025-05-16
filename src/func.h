/**
 * @file func.h
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief 
 * @version 0.1
 * @date 2025-03-18
 * 
 * @copyright Copyright (c) 2025
 * 
*/
#ifndef FUNC_H
#define FUNC_H

#define SWIDTH 12
#define SHEIGHT 12
#define NOISERANGE 4
#define MAXINPUT 100

#include <stdbool.h>

/**
 * @brief Representa um vetor bidimensional com coordenadas inteiras.
 *
 * A estrutura Vector2 é usada para armazenar posições em uma matriz,
 * com valores inteiros para coordenadas X e Y.
 */
typedef struct Vector2
{
    int x;  ///< Coordenada X
    int y;  ///< Coordenada Y
}Vector2;

extern Vector2 sSize;
extern int noiseRange;

/**
 * @brief Representa um nó de uma lista ligada de elementos na matriz.
 *
 * Cada nó contém um valor (por exemplo, um tipo de antena ou um efeito nefasto),
 * uma posição na matriz representada por um Vector2, e um ponteiro para o próximo nó da lista.
 */
typedef struct Node
{
    Vector2 pos;        ///< Posição do nó na matriz
    char value;         ///< Valor do nó (ex: antena tipo A, efeito nefasto '#')
    struct Node* next;  ///< Ponteiro para o próximo nó na lista
}Node;

// Aresta (ligação entre antenas com mesma frequência)
typedef struct Edge {
    float weight; 
    struct Vertex* dest;
    struct Edge* next;
} Edge;

// Vértice (antena)
typedef struct Vertex {
    int id;
    Vector2 pos;
    char value;
    Edge* edges;
    int seen;
    struct Vertex* next;
} Vertex;

// Grafo
typedef struct Graph{
    int count;
    Vertex* vertices;
} Graph;

//Elemento
typedef struct Element{
    Vertex* item;
    struct Element* next;
}Element;

//Primeiro elemento
typedef struct Path{
    Element* first;
    int max;
}Path;




/**
 * @brief Cria um novo elemento (nó) para a lista.
 *
 * @param value Valor do elemento (ex: antena tipo A, efeito nefasto '#').
 * @param position Posição na matriz.
 * @return Ponteiro para o novo nó criado.
 */
Node* MakeNode(char value, Vector2 position);

/**
 * @brief Verifica se a posição está dentro dos limites da matriz.
 *
 * @param pos Posição a ser verificada.
 * @return true se estiver dentro dos limites, false caso contrário.
 */
bool NodeInBounds(Vector2 pos);

/**
 * @brief Insere um novo nó na lista, verificando se a posição está disponível.
 *
 * @param dnew Novo nó a ser inserido.
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista atualizada.
 */
Node* InsertNode(Node* st ,Node* dnew);

/**
 * @brief Remove um nó da lista.
 *
 * @param rm Ponteiro para o nó a ser removido.
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista atualizada.
 */
Node *RemoveNode(Node *rm, Node *st);

/**
 * @brief Limpa elementos de ruído ('#') da lista.
 *
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista sem elementos de ruído.
 */
Node* ClearNoise(Node* st);

/**
 * @brief Verifica se a posição de um novo nó é válida (sem sobreposição).
 *
 * @param dnew Novo nó.
 * @param st Ponteiro para a lista.
 * @return true se a posição for válida, false caso contrário.
 */
bool ValidNodePos(Node *dnew, Node *st);

/**
 * @brief Verifica e aplica a regra de efeito nefasto aos elementos na lista.
 *
 * @param st Lista de nós.
 * @return Node* Lista atualizada após a verificação do efeito nefasto.
 */
Node *NoiseCheck(Node *st);

/**
 * @brief Versão alternativa da verificação de ruído que compara todos os elementos entre si.
 *
 * @param st Lista de nós.
 * @return Node* Lista atualizada após a verificação do efeito nefasto.
 */
Node *NoiseCheckAlt(Node *st);

/**
 * @brief Procura um nó em uma posição específica.
 *
 * @param st Lista de nós.
 * @param npos Posição a ser pesquisada.
 * @return Node* Ponteiro para o nó encontrado ou NULL se não existir.
 */
Node* FreeNodes(Node *head);

Node* FindNodePos(Node* st, Vector2 npos);

Graph* InitGraph();

Vertex* MakeVertex(char value, Vector2 position, int *id);

bool InsertEdge(Vertex* from, Vertex* dest);

bool IsNewEdge(Vertex* from, Vertex* dest);

Vertex *InsertVertex(Vertex *dnew, Vertex *st);

Vertex *RemoveVertex(Vertex *rm, Vertex *st);

bool ClearEdges(Vertex* old);

bool RemoveEdge(Vertex *from, Vertex *dest);

bool EdgeFindDest(Edge** current, Edge** previous, Vertex* pick);

bool AddEdges(Vertex* new, Vertex* st);

Vertex *FindVertexAt(Vertex *st, Vector2 npos);

Vertex* FindVertexById(Vertex *vertices, int id);

bool FreeGraph(Graph *gr);

//search
bool ClearSeen(Vertex *st);

Element* GraphDFS(Graph* gr, Vertex* start);

void DFSVisit(Vertex* current, Path* path);

Element* GraphBFS(Graph* gr, Vertex* start);

Element* MakeSeenList(Graph* gr, int max);

void PrintPath(Vertex *path[], int pathLen);

Element* GraphPaths(Graph *gr, Vertex *start, Vertex *end);

void Pathing(Vertex *current, Vertex *end, Path* head,Graph *gr);

void AddPath(Path* head, Graph *gr, int max);

/*
void FindPathsUtil(Vertex *current, Vertex *end, bool visited[], Vertex *path[], int pathLen);

void FindPaths(Graph *gr, Vertex *start, Vertex *end);
*/
void FindIntersections(Graph *gr);

//elements

Element *MakeElement(Vertex* value);

Element *InsertElement(Element *dnew, Element *st);

Element *InsertElementAtEnd(Element* new, Element* st);

Element *RemoveElement(Element *rm, Element *st);

Element *FindElement(void* item,Element *st);

Element* FreeElements(Element *head);

/**
 * @brief Compara se dois vetores possuem as mesmas coordenadas.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return true Se forem iguais.
 * @return false Se forem diferentes.
 */
bool Vector2Compare(Vector2 a, Vector2 b);

/**
 * @brief Subtrai um vetor do outro.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return Vector2 Resultado da subtração.
 */
Vector2 Vector2Subtract(Vector2 a, Vector2 b);

/**
 * @brief Soma dois vetores.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return Vector2 Resultado da soma.
 */
Vector2 Vector2Add(Vector2 a, Vector2 b);


bool ReadGraphFile(const char *filename, Graph *gr);

bool SaveGraphFile(const char *filename, Graph *gr);

/**
 * @brief Lê uma lista de elementos a partir de um ficheiro.
 *
 * @param filename Nome do ficheiro.
 * @return Node* Lista carregada do ficheiro.
 */
Node *ReadListFile(const char *filename, Node *st);

/**
 * @brief Guarda a lista de elementos num ficheiro.
 *
 * @param filename Nome do ficheiro onde os dados serão armazenados.
 * @param st Lista de nós a ser salva.
 */
bool SaveList(const char* filename, Node* st);


bool CopyListToGraph(Node* st, Graph* gr);

Node* CopyGraphToList(Graph* gr, Node* st);

#endif 
