/**
 * @file func.h
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief Implementação de funções para manipulação de uma lista de elementos representados em matriz.
 * @version 0.10
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
    int x; ///< Coordenada X
    int y; ///< Coordenada Y
} Vector2;

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
    Vector2 pos;       ///< Posição do nó na matriz
    char value;        ///< Valor do nó (ex: antena tipo A, efeito nefasto '#')
    struct Node *next; ///< Ponteiro para o próximo nó na lista
} Node;

/**
 * @brief Representa uma aresta (ligação) entre dois vértices no grafo.
 *
 * Cada aresta possui um peso (distância ou custo), um ponteiro para o vértice de destino
 * e um ponteiro para a próxima aresta na lista de adjacência.
 */
typedef struct Edge
{
    float weight;
    struct Vertex *dest;
    struct Edge *next;
} Edge;

/**
 * @brief Representa um vértice (nó) no grafo.
 *
 * Cada vértice possui um identificador único, uma posição na matriz (Vector2),
 * um valor (carácter representando o tipo, como 'A' ou 'B'), uma lista de arestas (ligações),
 * uma flag de controlo de visitação (seen) e um ponteiro para o próximo vértice.
 */
typedef struct Vertex
{
    int id;
    Vector2 pos;
    char value;
    Edge *edges;
    int seen;
    struct Vertex *next;
} Vertex;

/**
 * @brief Representa a estrutura principal de um grafo.
 *
 * Contém o número total de vértices e um ponteiro para a lista de vértices.
 */
typedef struct Graph
{
    int count;
    Vertex *vertices;
} Graph;

/**
 * @brief Representa um elemento de uma lista auxiliar que contém vértices.
 *
 * Utilizado para armazenar e organizar vértices durante percursos ou agrupamentos.
 */
typedef struct Element
{
    Vertex *item;
    struct Element *next;
} Element;

/**
 * @brief Estrutura auxiliar utilizada para armazenar percursos ou listas ordenadas de vértices.
 *
 * Guarda a lista de elementos visitados e um contador máximo de visitas, utilizado para BFS/DFS.
 */
typedef struct Path
{
    Element *first;
    int max;
} Path;

/**
 * @brief Cria um novo elemento (nó) para a lista.
 *
 * @param value Valor do elemento (ex: antena tipo A, efeito nefasto '#').
 * @param position Posição na matriz.
 * @return Ponteiro para o novo nó criado.
 */
Node *MakeNode(char value, Vector2 position);

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
Node *InsertNode(Node *st, Node *dnew);

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
Node *ClearNoise(Node *st);

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
Node *FreeNodes(Node *head);

/**
 * @brief Procura um nó numa posição específica da matriz.
 *
 * Percorre a lista de nós para encontrar um nó cuja posição coincida com a posição fornecida.
 *
 * @param st Lista de nós a pesquisar.
 * @param npos Posição (Vector2) a procurar.
 * @return Node* Ponteiro para o nó encontrado ou NULL se não existir nenhum nó nessa posição.
 */
Node *FindNodePos(Node *st, Vector2 npos);

/**
 * @brief Inicializa uma estrutura de grafo vazia.
 *
 * Reserva memória para um novo grafo e inicializa o contador e a lista de vértices.
 *
 * @return Graph* Ponteiro para o novo grafo.
 */
Graph *InitGraph();

/**
 * @brief Cria um novo vértice com valor e posição especificados.
 *
 * Gera um identificador único com base no ponteiro de contador fornecido.
 *
 * @param value Valor (tipo) do vértice (ex: 'A', 'B', '#').
 * @param position Posição no espaço bidimensional.
 * @param id Ponteiro para o contador global de identificadores.
 * @return Vertex* Novo vértice criado.
 */
Vertex *MakeVertex(char value, Vector2 position, int *id);

/**
 * @brief Cria uma nova aresta entre dois vértices.
 *
 * Apenas adiciona se a ligação ainda não existir.
 *
 * @param from Vértice de origem.
 * @param dest Vértice de destino.
 * @return true Se a aresta foi criada com sucesso.
 * @return false Se a ligação já existia.
 */
bool InsertEdge(Vertex *from, Vertex *dest);

/**
 * @brief Verifica se uma ligação entre dois vértices já existe.
 *
 * @param from Vértice de origem.
 * @param dest Vértice de destino.
 * @return true Se já existe uma aresta entre os vértices.
 * @return false Caso contrário.
 */
bool IsNewEdge(Vertex *from, Vertex *dest);

/**
 * @brief Insere um vértice na lista de vértices.
 *
 * @param dnew Novo vértice.
 * @param st Lista existente de vértices.
 * @param edge Define se deve adicionar arestas automaticamente para vértices compatíveis.
 * @return Vertex* Lista atualizada.
 */
Vertex *InsertVertex(Vertex *dnew, Vertex *st, bool edge);

/**
 * @brief Remove um vértice e as suas arestas da lista.
 *
 * @param rm Vértice a remover.
 * @param st Lista de vértices.
 * @return Vertex* Lista atualizada.
 */
Vertex *RemoveVertex(Vertex *rm, Vertex *st);

/**
 * @brief Liberta todas as arestas de um vértice.
 *
 * @param old Vértice cujas arestas devem ser removidas.
 * @return true Se as arestas foram removidas com sucesso.
 */
bool ClearEdges(Vertex *old);

/**
 * @brief Remove uma aresta entre dois vértices.
 *
 * @param from Vértice de origem.
 * @param dest Vértice de destino.
 * @return true Se a aresta foi removida com sucesso.
 */
bool RemoveEdge(Vertex *from, Vertex *dest);

/**
 * @brief Encontra uma aresta de destino e os ponteiros anteriores na lista.
 *
 * Auxiliar para remoção de arestas.
 *
 * @param current Ponteiro para a aresta atual.
 * @param previous Ponteiro para a aresta anterior.
 * @param pick Vértice de destino a encontrar.
 * @return true Se a aresta foi encontrada.
 */
bool EdgeFindDest(Edge **current, Edge **previous, Vertex *pick);

/**
 * @brief Adiciona arestas entre um novo vértice e outros na lista, se forem compatíveis.
 *
 * Compatibilidade geralmente depende do valor/resonância da antena.
 *
 * @param new Novo vértice.
 * @param st Lista de vértices existentes.
 * @return true Se pelo menos uma aresta foi adicionada.
 */
bool AddEdges(Vertex *new, Vertex *st);

/**
 * @brief Procura um vértice no grafo a partir de uma posição.
 *
 * @param st Lista de vértices do grafo.
 * @param npos Posição procurada.
 * @return Vertex* Ponteiro para o vértice encontrado, ou NULL se não existir.
 */
Vertex *FindVertexAt(Vertex *st, Vector2 npos);

/**
 * @brief Procura um vértice no grafo com base no seu identificador (ID).
 *
 * Percorre a lista de vértices e retorna o ponteiro correspondente ao ID.
 *
 * @param vertices Lista de vértices do grafo.
 * @param id Identificador único do vértice a encontrar.
 * @return Vertex* Ponteiro para o vértice correspondente, ou NULL se não encontrado.
 */
Vertex *FindVertexById(Vertex *vertices, int id);

/**
 * @brief Liberta todos os vértices e arestas de um grafo.
 *
 * Remove completamente todos os dados da estrutura de grafo e reinicia os contadores.
 *
 * @param gr Ponteiro para o grafo a ser limpo.
 * @return true Se a operação foi concluída com sucesso.
 */
bool FreeGraph(Graph *gr);

/**
 * @brief Limpa o estado de "visitado" (seen) de todos os vértices.
 *
 * Define seen = 0 para todos os vértices, preparando para uma nova pesquisa.
 *
 * @param st Lista de vértices.
 * @return true Se os valores foram reiniciados com sucesso.
 */
bool ClearSeen(Vertex *st);

/**
 * @brief Realiza uma pesquisa em profundidade (DFS) a partir de um vértice inicial.
 *
 * Visita todos os vértices conectados recursivamente e guarda a ordem numa lista.
 *
 * @param gr Grafo onde a pesquisa será realizada.
 * @param start Vértice inicial da pesquisa.
 * @return Element* Lista de elementos visitados pela DFS.
 */
Element *GraphDFS(Graph *gr, Vertex *start);

/**
 * @brief Marca recursivamente os vértices visitados em profundidade (DFS).
 *
 * Utilizado para contagem ou verificação de conectividade.
 *
 * @param current Vértice atual.
 * @param count Ponteiro para o contador de visitas.
 */
void DFSMark(Vertex *current, int *count);

/**
 * @brief Realiza uma pesquisa em largura (BFS) a partir de um vértice inicial.
 *
 * Visita todos os vértices acessíveis por níveis, e armazena os visitados numa lista.
 *
 * @param gr Grafo onde a pesquisa será realizada.
 * @param start Vértice inicial da pesquisa.
 * @return Element* Lista de elementos visitados pela BFS.
 */
Element *GraphBFS(Graph *gr, Vertex *start);

/**
 * @brief Cria uma lista ordenada de elementos com base na ordem de visita (seen).
 *
 * @param gr Grafo com os vértices já marcados.
 * @param max Valor máximo da marcação seen.
 * @return Element* Lista ordenada de vértices pela ordem de visita.
 */
Element *MakeSeenList(Graph *gr, int max);

/**
 * @brief Calcula todos os caminhos possíveis entre dois vértices.
 *
 * Usa backtracking para encontrar todas as combinações possíveis.
 *
 * @param gr Ponteiro para o grafo.
 * @param start Vértice de início.
 * @param end Vértice de destino.
 * @return Element* Lista de caminhos encontrados.
 */
Element *GraphPaths(Graph *gr, Vertex *start, Vertex *end);

/**
 * @brief Caminha recursivamente no grafo entre dois vértices, acumulando o caminho atual.
 *
 * @param current Vértice atual.
 * @param end Vértice de destino.
 * @param head Caminho acumulado.
 * @param gr Ponteiro para o grafo.
 */
void Pathing(Vertex *current, Vertex *end, Path *head, Graph *gr);

/**
 * @brief Adiciona um caminho encontrado à lista principal.
 *
 * @param head Lista principal de caminhos.
 * @param gr Grafo de onde os caminhos provêm.
 * @param max Número máximo de visitas.
 */
void AddPath(Path *head, Graph *gr, int max);

/**
 * @brief Encontra e retorna uma lista de intersecções entre pares de antenas com ressonância A e B.
 *
 * Percorre o grafo e encontra todos os pares de antenas (vértices) em que uma possui valor/resonância A e a outra B.
 * Apenas regista intersecções diretas (arestas existentes). A lista resultante contém os pares encontrados.
 *
 * @param gr Ponteiro para o grafo.
 * @param a Carácter correspondente à primeira frequência (ex: 'A').
 * @param b Carácter correspondente à segunda frequência (ex: 'B').
 * @return Element* Lista de elementos contendo os pares encontrados.
 */
Element *FindPairs(Graph *gr, char a, char b);

/**
 * @brief Cria um novo elemento com base num vértice.
 *
 * @param value Ponteiro para o vértice.
 * @return Element* Novo elemento criado.
 */
Element *MakeElement(Vertex *value);

/**
 * @brief Insere um elemento no início de uma lista.
 *
 * @param dnew Novo elemento.
 * @param st Lista existente.
 * @return Element* Lista atualizada.
 */
Element *InsertElement(Element *dnew, Element *st);

/**
 * @brief Insere um elemento no final da lista.
 *
 * @param new Novo elemento.
 * @param st Lista atual.
 * @return Element* Lista atualizada.
 */
Element *InsertElementAtEnd(Element *new, Element *st);

/**
 * @brief Remove um elemento específico da lista.
 *
 * @param rm Elemento a remover.
 * @param st Lista original.
 * @return Element* Lista atualizada.
 */
Element *RemoveElement(Element *rm, Element *st);

/**
 * @brief Procura um elemento na lista.
 *
 * @param item Ponteiro para o item a procurar.
 * @param st Lista onde será feita a procura.
 * @return Element* Ponteiro para o elemento encontrado ou NULL.
 */
Element *FindElement(void *item, Element *st);

/**
 * @brief Liberta toda a memória alocada por uma lista de elementos.
 *
 * @param head Início da lista.
 * @return Element* NULL, lista vazia.
 */
Element *FreeElements(Element *head);

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

/**
 * @brief Lê um grafo a partir de um ficheiro binário, incluindo os vértices e as arestas com base nas posições.
 *
 * Os vértices são lidos primeiro, seguidos de um marcador (-1), e depois as arestas que ligam posições (Vector2).
 *
 * @param filename Nome do ficheiro a ler.
 * @param gr Ponteiro para o grafo a ser preenchido.
 * @return true Se a leitura for bem-sucedida.
 * @return false Em caso de erro na leitura do ficheiro.
 */
bool ReadGraphFile(const char *filename, Graph *gr);

/**
 * @brief Guarda um grafo num ficheiro binário, incluindo vértices e arestas por posição (Vector2).
 *
 * Os vértices são escritos primeiro, seguidos de um marcador (-1), e depois as arestas como pares de posições com peso.
 *
 * @param filename Nome do ficheiro onde será guardado.
 * @param gr Ponteiro para o grafo a ser guardado.
 * @return true Se a gravação for bem-sucedida.
 * @return false Em caso de erro ao abrir ou escrever no ficheiro.
 */
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
bool SaveList(const char *filename, Node *st);

/**
 * @brief Copia os nós de uma lista para um grafo.
 *
 * Converte cada nó da lista de elementos (Node) para um vértice no grafo, mantendo as posições e valores.
 * Atribui automaticamente arestas entre os vértices conforme necessário.
 *
 * @param st Lista de nós a converter.
 * @param gr Grafo onde os vértices serão inseridos.
 * @return true Se a operação foi concluída com sucesso.
 */
bool CopyListToGraph(Node *st, Graph *gr);

/**
 * @brief Copia os vértices de um grafo para uma lista de nós.
 *
 * Cria uma nova lista de elementos baseada na posição e tipo dos vértices do grafo.
 *
 * @param gr Grafo de origem.
 * @param st Lista de destino, será atualizada.
 * @return Node* Ponteiro para a nova lista resultante.
 */
Node *CopyGraphToList(Graph *gr, Node *st);

#endif
