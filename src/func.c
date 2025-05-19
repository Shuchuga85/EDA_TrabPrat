/**
 * @file func.c
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief Implementação de funções para manipulação de uma lista de elementos representados em matriz.
 * @version 0.10
 * @date 2025-03-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "interface.h"
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#include <math.h>

Vector2 sSize = {SWIDTH, SHEIGHT};
int noiseRange = NOISERANGE;

#pragma region Node

/**
 * @brief Cria um novo elemento (nó) para a lista.
 *
 * @param value Valor do elemento (ex: antena tipo A, efeito nefasto '#').
 * @param position Posição na matriz.
 * @return Ponteiro para o novo nó criado.
 */
Node *MakeNode(char value, Vector2 position)
{
    if (!NodeInBounds(position))
    {
        if (value != '#')
        {
            printf("\n Posicao {%d,%d} fora da matrix! Elemeto discartado.", position.x, position.y);
        }

        return NULL;
    }

    Node *p = (Node *)malloc(sizeof(Node));

    if (p == NULL)
    {
        Log("[MakeNode] Failed to allocate memory.");
        return NULL;
    }

    p->value = value;
    p->pos = position;
    p->next = NULL;

    if (value != '#')
    {
        Log("[MakeNode] Successfully made a Node : (%c, {%d, %d})", value, position.x, position.y);
    }

    return p;
}

/**
 * @brief Verifica se a posição está dentro dos limites da matriz.
 *
 * @param pos Posição a ser verificada.
 * @return true se estiver dentro dos limites, false caso contrário.
 */
bool NodeInBounds(Vector2 pos)
{
    return (pos.x >= 0 && pos.y >= 0 && pos.x < sSize.x && pos.y < sSize.y);
}

/**
 * @brief Insere um novo nó na lista, verificando se a posição está disponível.
 *
 * @param dnew Novo nó a ser inserido.
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista atualizada.
 */
Node *InsertNode(Node *dnew, Node *st)
{
    if (st == NULL)
        return dnew;
    if (dnew == NULL)
        return st;

    Node *tmp = NULL;
    if ((tmp = FindNodePos(st, dnew->pos)) != NULL)
    {

        if (dnew->value != '#')
        {
            if (AskReplace(tmp->value,dnew->pos))
            {
                free(dnew);
                return st;
            }
            else
            {
                st = RemoveNode(tmp, st);
            }
        }
        else
        {
            free(dnew);
            return st;
        }
    }

    dnew->next = st;
    st = dnew;

    st = NoiseCheckAlt(st);

    Log("[InsertNode] Successfully inserted node in list");

    return st;
}

/**
 * @brief Remove um nó da lista.
 *
 * @param rm Ponteiro para o nó a ser removido.
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista atualizada.
 */
Node *RemoveNode(Node *rm, Node *st)
{
    if (rm == NULL)
        return st;
    if (st == NULL)
        return st;

    Node *current = st;
    Node *aux;

    if (current == rm)
    {
        st = current->next;
        free(rm);
        st = ClearNoise(st);
        st = NoiseCheck(st);
        return st;
    }

    while (current->next != rm)
    {
        current = current->next;
    }

    aux = rm->next;

    free(rm);
    current->next = aux;
    st = ClearNoise(st);
    st = NoiseCheck(st);
    return st;
}

/**
 * @brief Limpa elementos de ruído ('#') da lista.
 *
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista sem elementos de ruído.
 */
Node *ClearNoise(Node *st)
{
    if (st == NULL)
        return st;

    Node *current = st;
    Node *aux;

    if (current->value == '#')
    {
        aux = current;
        current = current->next;
        st = current;
        free(aux);
    }

    while (current->next != NULL)
    {
        if (current->next->value == '#')
        {
            aux = current->next;
            current->next = aux->next;
            free(aux);
            continue;
        }
        current = current->next;
    }
    return st;
}

/**
 * @brief Verifica se a posição de um novo nó é válida (sem sobreposição).
 *
 * @param dnew Novo nó.
 * @param st Ponteiro para a lista.
 * @return true se a posição for válida, false caso contrário.
 */
bool ValidNodePos(Node *dnew, Node *st)
{
    if (FindNodePos(st, dnew->pos) != NULL)
        return false; // Se encontrar elemento na posicao x,y. e falso
    return true;
}

/**
 * @brief Verifica e aplica a regra de efeito nefasto aos elementos na lista.
 *
 * @param st Lista de nós.
 * @return Node* Lista atualizada após a verificação do efeito nefasto.
 */
Node *NoiseCheck(Node *st)
{
    Node *current = st;
    Node *p = NULL;
    Vector2 cpos;

    while (current != NULL)
    {
        if (current->value == '#')
        {
            current = current->next;
            continue;
        }

        cpos.x = current->pos.x;
        cpos.y = current->pos.y;

        for (int y = -noiseRange; y <= noiseRange; y++)
        {
            for (int x = -noiseRange; x <= noiseRange; x++)
            {

                if (!NodeInBounds((Vector2){cpos.x + x, cpos.y + y}) || (x == 0 && y == 0))
                    continue; // Se a posicao esta fora da area ou a posicao e o propio elemento continua

                p = FindNodePos(st, (Vector2){cpos.x + x, cpos.y + y});

                if (p == NULL)
                    continue; // Se nao ha elementos na posicao, continua

                if (p->value == current->value) // Se os elementos forem iguais, entao ha efeito nefasto
                {
                    st = InsertNode(MakeNode('#', (Vector2){p->pos.x + x, p->pos.y + y}), st); // adiciona
                }
            }
        }

        current = current->next;
    }
    Log("[NoiseCheck] Successfully calculated noise");

    return st;
}

/**
 * @brief Versão alternativa da verificação de ruído que compara todos os elementos entre si.
 *
 * @param st Lista de nós.
 * @return Node* Lista atualizada após a verificação do efeito nefasto.
 */
Node *NoiseCheckAlt(Node *st)
{
    Node *current = st;
    Node *p = NULL;
    Node *start = st;
    Vector2 diff, npos;

    if (st == NULL)
        return NULL;

    while (current != NULL)
    {
        if (current->value == '#')
        {
            current = current->next;
            continue;
        }

        p = st;

        while (p != NULL)
        {

            if (Vector2Compare(p->pos, current->pos))
            {
                p = p->next;
                continue;
            }

            if (current->value == p->value)
            {
                diff = Vector2Subtract(p->pos, current->pos);
                npos = Vector2Add(p->pos, diff);

                start = InsertNode(MakeNode('#', npos), start);
            }

            p = p->next;
        }
        current = current->next;
    }
    Log("[NoiseCheckAlt] Successfully calculated noise");

    st = start;
    return start;
}

/**
 * @brief Procura um nó na lista pela sua posição.
 *
 * @param st Ponteiro para a lista.
 * @param npos Posição do nó a ser encontrado.
 * @return Ponteiro para o nó encontrado, ou NULL se não existir.
 */
Node *FindNodePos(Node *st, Vector2 npos)
{
    Node *current = st;

    while (current != NULL)
    {
        if (Vector2Compare(current->pos, npos) == true)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/**
 * @brief Procura um nó em uma posição específica.
 *
 * @param st Lista de nós.
 * @param npos Posição a ser pesquisada.
 * @return Node* Ponteiro para o nó encontrado ou NULL se não existir.
 */
Node *FreeNodes(Node *st)
{
    if (st == NULL)
        return st;

    Node *current = st;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    st = NULL;
    return st;
}

#pragma endregion

#pragma region Graph

/**
 * @brief Inicializa uma estrutura de grafo vazia.
 *
 * Reserva memória para um novo grafo e inicializa o contador e a lista de vértices.
 *
 * @return Graph* Ponteiro para o novo grafo.
 */
Graph *InitGraph()
{
    Graph *gr = (Graph *)malloc(sizeof(Graph));
    gr->count = 0;
    gr->vertices = NULL;
    return gr;
}

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
Vertex *MakeVertex(char value, Vector2 position, int *id)
{

    if (!NodeInBounds(position))
    {
        if (value != '#')
        {
            printf("\n Posicao {%d,%d} fora da matrix! Elemeto discartado.", position.x, position.y);
        }

        return NULL;
    }

    Vertex *v = (Vertex *)malloc(sizeof(Vertex));

    if (v == NULL)
    {
        Log("[MakeVertex] Failed to allocate memory.");
        return NULL;
    }

    v->value = value;
    v->id = *id;
    *id = *id + 1;
    v->pos = position;
    v->edges = NULL;
    v->next = NULL;
    Log("[MakeVertex] Successfully made a Vertex : (%c, %d, {%d, %d})", value, *id, position.x, position.y);
    return v;
}

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
bool InsertEdge(Vertex *from, Vertex *dest)
{

    if (from == NULL || dest == NULL || !IsNewEdge(from, dest))
    {
        return false;
    }

    float weight = sqrtf(pow(dest->pos.x - from->pos.x, 2) + pow(dest->pos.y - from->pos.y, 2));

    Edge *newdest = (Edge *)malloc(sizeof(Edge));
    if (newdest == NULL)
    {
        Log("[MakeEdge] Failed to allocate memory.");
        return false;
    }
    newdest->weight = weight;
    newdest->dest = dest;
    newdest->next = from->edges;
    from->edges = newdest;

    Edge *newfrom = (Edge *)malloc(sizeof(Edge));
    if (newfrom == NULL)
    {
        Log("[MakeEdge] Failed to allocate memory.");
        return false;
    }
    newfrom->weight = weight;
    newfrom->dest = from;
    newfrom->next = dest->edges;
    dest->edges = newfrom;

    return true;
}

/**
 * @brief Verifica se uma ligação entre dois vértices já existe.
 *
 * @param from Vértice de origem.
 * @param dest Vértice de destino.
 * @return true Se já existe uma aresta entre os vértices.
 * @return false Caso contrário.
 */
bool IsNewEdge(Vertex *from, Vertex *dest)
{
    Edge *current = from->edges;
    while (current)
    {
        if (current->dest == dest)
        {
            return false;
        }
        current = current->next;
    }
    return true;
}

/**
 * @brief Insere um vértice na lista de vértices.
 *
 * @param dnew Novo vértice.
 * @param st Lista existente de vértices.
 * @param edge Define se deve adicionar arestas automaticamente para vértices compatíveis.
 * @return Vertex* Lista atualizada.
 */
Vertex *InsertVertex(Vertex *dnew, Vertex *st, bool edge)
{
    if (st == NULL)
        return dnew;
    if (dnew == NULL)
        return st;

    Vertex *tmp = NULL;
    if ((tmp = FindVertexAt(st, dnew->pos)) != NULL)
    {
        if (dnew->value != '#')
        {
            if (AskReplace(tmp->value, dnew->pos ))
            {
                free(dnew);
                return st;
            }
            else
            {
                st = RemoveVertex(tmp, st);
            }
        }
        else
        {
            free(dnew);
            return st;
        }
    }

    dnew->next = st;
    st = dnew;
    
    if (edge) AddEdges(dnew, st);

    Log("[InsertVertex] Successfully inserted vertex in list");
    return st;
}

/**
 * @brief Remove um vértice e as suas arestas da lista.
 *
 * @param rm Vértice a remover.
 * @param st Lista de vértices.
 * @return Vertex* Lista atualizada.
 */
Vertex *RemoveVertex(Vertex *rm, Vertex *st)
{
    if (rm == NULL)
        return st;
    if (st == NULL)
        return st;

    Vertex *current = st;
    Vertex *aux;

    ClearEdges(rm);

    if (current == rm)
    {
        st = current->next;
        free(rm);
        return st;
    }

    while (current->next != rm)
    {
        current = current->next;
    }

    aux = rm->next;
    free(rm);
    current->next = aux;
    return st;
}

/**
 * @brief Liberta todas as arestas de um vértice.
 *
 * @param old Vértice cujas arestas devem ser removidas.
 * @return true Se as arestas foram removidas com sucesso.
 */
bool ClearEdges(Vertex *old)
{
    if (old == NULL)
        return 0;
    Edge *cedge = old->edges;
    Edge *next = NULL;
    Edge *current = NULL;
    Edge *previous = NULL;
    while (cedge)
    {
        // dest edges to find the node about to be removed
        current = cedge->dest->edges;
        previous = NULL;

        if (EdgeFindDest(&current, &previous, old) == true)
        {
            if (previous == NULL)
            {
                Log("[ClearEdges] previous is null");

                cedge->dest->edges = current->next; // first
            }
            else
            {
                Log("[ClearEdges] previous is not null");

                previous->next = current->next; // in the middle
            }
            free(current);
        }

        Log("[ClearEdges] removing cedge");

        next = cedge->next;
        free(cedge);
        cedge = next;
    }

    Log("[AddEdges] Successfully Removed edges for vertex : %c {%d,%d}", old->value, old->pos.x, old->pos.y);
    return 1;
}

/**
 * @brief Remove uma aresta entre dois vértices.
 *
 * @param from Vértice de origem.
 * @param dest Vértice de destino.
 * @return true Se a aresta foi removida com sucesso.
 */
bool RemoveEdge(Vertex *from, Vertex *dest)
{
    if (from == NULL || dest == NULL || IsNewEdge(from, dest))
    {
        return false;
    }

    Edge *current = from->edges;
    Edge *previous = NULL;
    while (current)
    {
        if (current->dest == dest)
            break;

        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return false;

    if (previous == NULL)
        from->edges = current->next;
    else
        previous->next = current->next;

    free(current);
    RemoveEdge(dest, from);
    return true;
}

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
bool EdgeFindDest(Edge **current, Edge **previous, Vertex *pick)
{
    if (*current == NULL)
    {
        Log("[EdgeFindDest] Failed to look for edgedest (current = null)");
    }

    Log("[EdgeFindDest] Looking for edgedest");
    while (*current && (*current)->dest != pick)
    {
        *previous = *current;
        *current = (*current)->next;
    }

    if (*current == NULL)
    {
        return false;
    }

    return true;
}

/**
 * @brief Adiciona arestas entre um novo vértice e outros na lista, se forem compatíveis.
 *
 * Compatibilidade geralmente depende do valor/resonância da antena.
 *
 * @param new Novo vértice.
 * @param st Lista de vértices existentes.
 * @return true Se pelo menos uma aresta foi adicionada.
 */
bool AddEdges(Vertex *new, Vertex *st)
{
    if (new == NULL || st == NULL)
        return 0;

    Vertex *current = st;
    while (current)
    {
        if (current != new && current->value == new->value)
        {
            InsertEdge(new, current);
        }
        current = current->next;
    }
    Log("[AddEdges] Successfully added edges for vertex : %c {%d,%d}", new->value, new->pos.x, new->pos.y);
    return 1;
}

/**
 * @brief Procura um vértice no grafo a partir de uma posição.
 *
 * @param st Lista de vértices do grafo.
 * @param npos Posição procurada.
 * @return Vertex* Ponteiro para o vértice encontrado, ou NULL se não existir.
 */
Vertex *FindVertexAt(Vertex *st, Vector2 npos)
{
    Vertex *current = st;

    while (current != NULL)
    {
        if (Vector2Compare(current->pos, npos) == true)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/**
 * @brief Procura um vértice no grafo com base no seu identificador (ID).
 *
 * Percorre a lista de vértices e retorna o ponteiro correspondente ao ID.
 *
 * @param vertices Lista de vértices do grafo.
 * @param id Identificador único do vértice a encontrar.
 * @return Vertex* Ponteiro para o vértice correspondente, ou NULL se não encontrado.
 */
Vertex *FindVertexById(Vertex *vertices, int id)
{
    while (vertices)
    {
        if (vertices->id == id)
            return vertices;
        vertices = vertices->next;
    }
    return NULL;
}

/**
 * @brief Liberta todos os vértices e arestas de um grafo.
 *
 * Remove completamente todos os dados da estrutura de grafo e reinicia os contadores.
 *
 * @param gr Ponteiro para o grafo a ser limpo.
 * @return true Se a operação foi concluída com sucesso.
 */
bool FreeGraph(Graph *gr)
{
    if (gr == NULL) return false;

    if (gr->vertices == NULL) return true;

    Vertex *current = gr->vertices;
    Vertex *next;

    while (current != NULL)
    {
        next = current->next;
        gr->vertices = RemoveVertex(current, gr->vertices);
        current = next;
    }

    gr->count = 0;
    gr->vertices = NULL;
    return true;
}

#pragma region Search Alg

/**
 * @brief Limpa o estado de "visitado" (seen) de todos os vértices.
 *
 * Define seen = 0 para todos os vértices, preparando para uma nova pesquisa.
 *
 * @param st Lista de vértices.
 * @return true Se os valores foram reiniciados com sucesso.
 */
bool ClearSeen(Vertex *st)
{
    if (st == NULL)
        return false;

    Vertex *current = st;
    while (current)
    {
        current->seen = 0;
        current = current->next;
    }
    return true;
}

/**
 * @brief Realiza uma pesquisa em profundidade (DFS) a partir de um vértice inicial.
 *
 * Visita todos os vértices conectados recursivamente e guarda a ordem numa lista.
 *
 * @param gr Grafo onde a pesquisa será realizada.
 * @param start Vértice inicial da pesquisa.
 * @return Element* Lista de elementos visitados pela DFS.
 */
Element *GraphDFS(Graph *gr, Vertex *start)
{
    if (!gr || !start)
        return NULL;
    ClearSeen(gr->vertices);

    int count = 1;
    DFSMark(start, &count);

    return MakeSeenList(gr, count);
}

/**
 * @brief Marca recursivamente os vértices visitados em profundidade (DFS).
 *
 * Utilizado para contagem ou verificação de conectividade.
 *
 * @param current Vértice atual.
 * @param count Ponteiro para o contador de visitas.
 */
void DFSMark(Vertex *current, int *count)
{
    current->seen = (*count)++;

    Edge *e = current->edges;
    while (e)
    {
        if (!e->dest->seen)
        {
            DFSMark(e->dest, count);
        }
        e = e->next;
    }
}

/**
 * @brief Realiza uma pesquisa em largura (BFS) a partir de um vértice inicial.
 *
 * Visita todos os vértices acessíveis por níveis, e armazena os visitados numa lista.
 *
 * @param gr Grafo onde a pesquisa será realizada.
 * @param start Vértice inicial da pesquisa.
 * @return Element* Lista de elementos visitados pela BFS.
 */
Element *GraphBFS(Graph *gr, Vertex *start)
{
    if (!gr || !start)
        return NULL;
    ClearSeen(gr->vertices);

    Path *queue = (Path *)malloc(sizeof(Path));
    queue->first = NULL;
    queue->max = 1;
    queue->first = InsertElementAtEnd(MakeElement(start), queue->first);

    start->seen = queue->max++;

    Element *c = queue->first;
    while (c)
    {
        Vertex *current = c->item;

        Edge *cedge = current->edges;
        while (cedge)
        {

            if (cedge->dest->seen == 0)
            {
                cedge->dest->seen = queue->max++;
                queue->first = InsertElementAtEnd(MakeElement(cedge->dest), queue->first);
            }
            cedge = cedge->next;
        }

        queue->first = RemoveElement(c, queue->first);
        c = queue->first;
    }

    int i = queue->max;
    FreeElements(queue->first);
    free(queue);
    return MakeSeenList(gr, i);
}

/**
 * @brief Cria uma lista ordenada de elementos com base na ordem de visita (seen).
 *
 * @param gr Grafo com os vértices já marcados.
 * @param max Valor máximo da marcação seen.
 * @return Element* Lista ordenada de vértices pela ordem de visita.
 */
Element *MakeSeenList(Graph *gr, int max)
{
    Element *seenst = NULL;
    for (int i = 1; i < max; i++)
    {
        Vertex *current = gr->vertices;
        while (current)
        {
            if (current->seen == i)
            {
                seenst = InsertElementAtEnd(MakeElement(current), seenst);
                break;
            }
            current = current->next;
        }
    }
    return seenst;
}

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
Element *GraphPaths(Graph *gr, Vertex *start, Vertex *end)
{
    if (!gr || !start || !end)
        return false;
    ClearSeen(gr->vertices);

    Path *path = (Path *)malloc(sizeof(Path));
    path->first = NULL;
    path->max = 1;

    Pathing(start, end, path, gr);
    Element *first = path->first;
    free(path);
    return first;
}

/**
 * @brief Caminha recursivamente no grafo entre dois vértices, acumulando o caminho atual.
 *
 * @param current Vértice atual.
 * @param end Vértice de destino.
 * @param head Caminho acumulado.
 * @param gr Ponteiro para o grafo.
 */
void Pathing(Vertex *current, Vertex *end, Path *path, Graph *gr)
{
    current->seen = path->max;
    path->max++;
    if (current == end)
    {
        AddPath(path, gr, path->max);
    }
    else
    {
        Edge *e = current->edges;
        while (e)
        {
            if (!e->dest->seen)
                Pathing(e->dest, end, path, gr);
            e = e->next;
        }
    }
    path->max--;
    current->seen = 0;
}

/**
 * @brief Adiciona um caminho encontrado à lista principal.
 *
 * @param head Lista principal de caminhos.
 * @param gr Grafo de onde os caminhos provêm.
 * @param max Número máximo de visitas.
 */
void AddPath(Path *path, Graph *gr, int max)
{
    for (int i = 1; i < max; i++)
    {
        Vertex *current = gr->vertices;
        while (current)
        {
            if (current->seen == i)
            {
                path->first = InsertElementAtEnd(MakeElement(current), path->first);
                break;
            }
            current = current->next;
        }
    }
}

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
Element *FindPairs(Graph *gr, char a, char b)
{
    if (!gr)
        return NULL;

    Element *result = NULL;
    Vertex *v1 = gr->vertices;

    while (v1)
    {
        if (v1->value == a)
        {
            Vertex *v2 = gr->vertices;
            while (v2)
            {
                if ((v1->value == a && v2->value == b))
                {

                    result = InsertElementAtEnd(MakeElement(v1), result);
                    result = InsertElementAtEnd(MakeElement(v2), result);
                }
                v2 = v2->next;
            }
        }
        v1 = v1->next;
    }

    return result;
}

#pragma endregion

#pragma endregion

#pragma region Element


/**
 * @brief Cria um novo elemento com base num vértice.
 *
 * @param value Ponteiro para o vértice.
 * @return Element* Novo elemento criado.
 */
Element *MakeElement(Vertex *value)
{
    Element *e = (Element *)malloc(sizeof(Element));

    if (e == NULL)
    {
        Log("[MakeElement] Failed to allocate memory.");
        return NULL;
    }
    e->item = value;
    e->next = NULL;

    Log("[MakeElement] Successfully made a Element");
    return e;
}

/**
 * @brief Insere um elemento no início de uma lista.
 *
 * @param dnew Novo elemento.
 * @param st Lista existente.
 * @return Element* Lista atualizada.
 */
Element *InsertElement(Element *dnew, Element *st)
{
    if (st == NULL)
        return dnew;
    if (dnew == NULL)
        return st;

    /*Element *tmp = NULL;
    if ((tmp = FindElement(dnew->item, st)) != NULL)
    {
        return st;
    }*/

    dnew->next = st;
    st = dnew;

    Log("[InsertElement] Successfully inserted element");

    return st;
}

/**
 * @brief Insere um elemento no final da lista.
 *
 * @param new Novo elemento.
 * @param st Lista atual.
 * @return Element* Lista atualizada.
 */
Element *InsertElementAtEnd(Element *new, Element *st)
{

    Element *current = st;
    if (current == NULL)
        return new;
    while (current->next)
        current = current->next;
    current->next = new;

    return st;
}

/**
 * @brief Remove um elemento específico da lista.
 *
 * @param rm Elemento a remover.
 * @param st Lista original.
 * @return Element* Lista atualizada.
 */
Element *RemoveElement(Element *rm, Element *st)
{
    if (rm == NULL)
        return st;
    if (st == NULL)
        return st;

    Element *current = st;
    Element *aux;

    if (current == rm)
    {
        st = current->next;
        free(rm);
        return st;
    }

    while (current->next != rm)
    {
        current = current->next;
    }

    aux = rm->next;

    free(rm);
    current->next = aux;
    return st;
}

/**
 * @brief Procura um elemento na lista.
 *
 * @param item Ponteiro para o item a procurar.
 * @param st Lista onde será feita a procura.
 * @return Element* Ponteiro para o elemento encontrado ou NULL.
 */
Element *FindElement(void *item, Element *st)
{
    Element *current = st;

    while (current != NULL)
    {
        if (current->item == item)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Liberta toda a memória alocada por uma lista de elementos.
 *
 * @param head Início da lista.
 * @return Element* NULL, lista vazia.
 */
Element *FreeElements(Element *path)
{
    if (path == NULL)
        return path;

    Element *current = path;
    Element *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    path = NULL;
    return path;
}

#pragma endregion

#pragma region Vector2

/**
 * @brief Compara se dois vetores possuem as mesmas coordenadas.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return true Se forem iguais.
 * @return false Se forem diferentes.
 */
bool Vector2Compare(Vector2 a, Vector2 b)
{
    return (a.x == b.x && a.y == b.y);
}
/**
 * @brief Subtrai um vetor do outro.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return Vector2 Resultado da subtração.
 */
Vector2 Vector2Subtract(Vector2 a, Vector2 b)
{
    Vector2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return ((Vector2){a.x - b.x, a.y - b.y});
}
/**
 * @brief Soma dois vetores.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return Vector2 Resultado da soma.
 */
Vector2 Vector2Add(Vector2 a, Vector2 b)
{
    Vector2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

#pragma endregion

#pragma region File

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
bool ReadGraphFile(const char *filename, Graph *gr)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        Log("[ReadGraphFile] Failed to open file: %s", filename);
        return false;
    }

    FreeGraph(gr);

    fread(&sSize, sizeof(Vector2), 1, file);

    char ch;
    Vector2 pos;
    long lastPos = ftell(file);

    while (fread(&ch, sizeof(char), 1, file) == 1 &&
           fread(&pos, sizeof(Vector2), 1, file) == 1)
    {
        if ((int)ch == -1)
        {
            break;
        }

        gr->vertices = InsertVertex(MakeVertex(ch, pos, &gr->count), gr->vertices, false);
        lastPos = ftell(file);
    }

    fseek(file, lastPos, SEEK_SET);
    int marker;
    fread(&marker, sizeof(int), 1, file);
    if (marker != -1)
    {
        fclose(file);
        Log("[ReadGraphFile] Missing edge marker after vertices.");
        return false;
    }

    Vector2 from_pos, to_pos;

    while (fread(&from_pos, sizeof(Vector2), 1, file) == 1 &&
           fread(&to_pos, sizeof(Vector2), 1, file) == 1)
    {
        Vertex *from = FindVertexAt(gr->vertices, from_pos);
        Vertex *to = FindVertexAt(gr->vertices, to_pos);
        if (from && to)
        {
            InsertEdge(from, to);
        }
    }

    fclose(file);
    return true;
}

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
bool SaveGraphFile(const char *filename, Graph *gr)
{
    if (!gr || !gr->vertices)
        return false;

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        Log("[SaveGraphFile] Failed to open file: %s", filename);
        return false;
    }

    fwrite(&sSize, sizeof(Vector2), 1, file);

    Vertex *v = gr->vertices;
    while (v)
    {
        fwrite(&v->value, sizeof(char), 1, file);
        fwrite(&v->pos, sizeof(Vector2), 1, file);
        v = v->next;
    }

    int marker = -1;
    fwrite(&marker, sizeof(int), 1, file);

    v = gr->vertices;
    while (v)
    {
        Edge *e = v->edges;
        while (e)
        {
            fwrite(&v->pos, sizeof(Vector2), 1, file);
            fwrite(&e->dest->pos, sizeof(Vector2), 1, file);
            e = e->next;
        }
        v = v->next;
    }

    fclose(file);
    return true;
}


/**
 * @brief Lê uma lista de elementos a partir de um ficheiro.
 *
 * @param filename Nome do ficheiro.
 * @return Node* Lista carregada do ficheiro.
 */
Node *ReadListFile(const char *filename, Node *st)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        Log("[ReadListFile] Tried and failed to open file : %s", filename);
        return NULL;
    }

    FreeNodes(st);
    st = NULL;
    int y = 0;
    int x = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {

        if (ch == ' ')
            continue;

        if (ch != '.' && ch != ' ' && ch != '\n' && ch != '#')
        {
            //printf("\n (%d, %d), %c", x, y, ch);
            st = InsertNode(MakeNode(ch, (Vector2){x, y}), st);
            x++;
        }
        else if (ch == '.')
        {
            x++;
        }

        if (x > sSize.x)
        {
            sSize.x = x;
        }

        if (ch == '\n')
        {
            x = 0;
            y++;
            if (y + 1 > sSize.y)
            {
                sSize.y = y + 1;
            }
        }
    }

    fclose(file);

    return st;
}

/**
 * @brief Guarda a lista de elementos num ficheiro.
 *
 * @param filename Nome do ficheiro onde os dados serão armazenados.
 * @param st Lista de nós a ser salva.
 */
bool SaveList(const char *filename, Node *st)
{
    if (st == NULL)
        return false;
    Node *tp;
    Vector2 tpos;
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        Log("[SaveList] Tried and failed to open file : %s", filename);
        return false;
    }

    for (int y = 0; y < sSize.y; y++)
    {
        if (y != 0)
            fprintf(file, "\n");

        for (int x = 0; x < sSize.x; x++)
        {
            tp = FindNodePos(st, (Vector2){x, y});
            if (tp != NULL && tp->value != '#')
            {
                fprintf(file, "%c", tp->value);
            }
            else
            {
                fprintf(file, ".");
            }
        }
    }
    fclose(file);

    Log("[SaveList] Saved List successfully : %s", filename);
    return true;
}

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
bool CopyListToGraph(Node *st, Graph *gr)
{
    if (st == NULL)
    {
        Log("[CopyListToGraph] Missing list, *st is null");
        return false;
    }
    FreeGraph(gr);
    Node *current = st;
    while (current != NULL)
    {
        if (current->value != '#')
        {
            gr->vertices = InsertVertex(MakeVertex(current->value, current->pos, &gr->count), gr->vertices, true);
            current = current->next;
            continue;
        }
        current = current->next;
    }
}

/**
 * @brief Copia os vértices de um grafo para uma lista de nós.
 *
 * Cria uma nova lista de elementos baseada na posição e tipo dos vértices do grafo.
 *
 * @param gr Grafo de origem.
 * @param st Lista de destino, será atualizada.
 * @return Node* Ponteiro para a nova lista resultante.
 */
Node *CopyGraphToList(Graph *gr, Node *st)
{
    if (gr->vertices == NULL)
    {
        Log("[CopyGraphToList] Missing vertices, *gr->vertices is null");
        return NULL;
    }
    FreeNodes(st);
    st = NULL;
    Vertex *current = gr->vertices;
    while (current != NULL)
    {
        st = InsertNode(MakeNode(current->value, current->pos), st);

        current = current->next;
    }
    return st;
}

#pragma endregion
