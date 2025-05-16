/**
 * @file func.c
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief Implementação de funções para manipulação de uma lista de elementos representados em matriz.
 * @version 0.1
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

Vector2 sSize = { SWIDTH, SHEIGHT };
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

    Log("[MakeNode] Successfully made a Node : (%c, {%d, %d})", value, position.x, position.y);

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
            char choice = 'y';

            if (tmp->value == '#')
            {
                printf("\n Position {%d,%d} have noise! Want to insert in noise? [y,n]", dnew->pos.x, dnew->pos.y);
            }
            else
            {
                printf("\n Position {%d,%d} occupied! Want to replace %c with new one? [y,n]", dnew->pos.x, dnew->pos.y, tmp->value);
            }
            scanf("%c", &choice);

            if (choice != 'y')
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

Node* FreeNodes(Node *st)
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

Graph *InitGraph()
{
    Graph *gr = (Graph *)malloc(sizeof(Graph));
    gr->count = 0;
    gr->vertices = NULL;
    return gr;
}

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

Vertex *InsertVertex(Vertex *dnew, Vertex *st)
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
            char choice = 'y';
            getchar();
            if (tmp->value == '#')
            {
                printf("\n Position {%d,%d} have noise! Want to insert in noise? [y,n]", dnew->pos.x, dnew->pos.y);
            }
            else
            {
                printf("\n Position {%d,%d} occupied! Want to replace %c with new one? [y,n]", dnew->pos.x, dnew->pos.y, tmp->value);
            }
            scanf("%c", &choice);
            if (choice != 'y')
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

    AddEdges(dnew, st);

    Log("[InsertVertex] Successfully inserted vertex in list");
    return st;
}

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

bool RemoveEdge(Vertex *from, Vertex *dest){
 if (from == NULL || dest == NULL || IsNewEdge(from, dest))
    {
        return false;
    }

    Edge* current = from->edges;
    Edge* previous = NULL;
    while (current)
    {
        if (current->dest == dest) break;
        
        previous = current;
        current = current->next;
    }

    if (current == NULL) return false;
    

    if (previous == NULL)
        from->edges = current->next;
    else
        previous->next = current->next; 
    

    free(current);
    RemoveEdge(dest, from);
    return true;
}


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

Vertex* FindVertexById(Vertex *vertices, int id)
{
    while(vertices)
    {
        if(vertices->id == id) return vertices;
        vertices = vertices->next;
    }
    return NULL;
}

bool FreeGraph(Graph *gr)
{
    if (gr->vertices == NULL)
        return true;

    Vertex *current = gr->vertices;
    Vertex *next;

    while (current != NULL)
    {
        next = current->next;
        gr->vertices = RemoveVertex(current, gr->vertices);
        current = next;
    }
    return true;
}
#pragma region Search

bool ClearSeen(Vertex *st){
    if (st == NULL) return false;
    
    Vertex* current = st;
    while (current)
    {
        current->seen = 0;
        current= current->next;
    }
    return true;
}

Element* GraphDFS(Graph* gr, Vertex* start) {
    if (!gr || !start) return NULL;
    ClearSeen(gr->vertices);

    int count = 1;
    DFSMark(start, &count);

    return MakeSeenList(gr, count);
}

void DFSMark(Vertex* current, int* count) {
    current->seen = (*count)++;
    
    Edge* e = current->edges;
    while (e) {
        if (!e->dest->seen) {
            DFSMark(e->dest, count);
        }
        e = e->next;
    }
}


/*
Element* GraphDFS(Graph* gr, Vertex* start) {
    if (!gr || !start) return NULL;
    ClearSeen(gr->vertices);

    Path* path = (Path*)malloc(sizeof(Path));
    path->first = NULL;
    path->max = 1;

    DFSVisit(start, path);
    Element* first = path->first;
    free(path);
    return first;
}

void DFSVisit(Vertex* current, Path* path) {
    current->seen = path->max;
    path->max++;

    path->first = InsertElementAtEnd(MakeElement(current), path->first);

    Edge* e = current->edges;
    while (e) {
        if (!e->dest->seen) {
            DFSVisit(e->dest, path);
        }
        e = e->next;
    }
}
*/
Element* GraphBFS(Graph* gr, Vertex* start) {
    if (!gr || !start) return NULL;
    ClearSeen(gr->vertices);

    Path* queue = (Path*)malloc(sizeof(Path));
    queue->first = NULL;
    queue->max = 1;
    queue->first = InsertElementAtEnd(MakeElement(start), queue->first);;

    start->seen = queue->max++;
    
    Element* c = queue->first;
    while (c) {
        Vertex* current = c->item;

        Edge* cedge = current->edges;
        while (cedge) {

            if (cedge->dest->seen == 0) {
                cedge->dest->seen = queue->max++;
                queue->first = InsertElementAtEnd(MakeElement(cedge->dest), queue->first);
            }
            cedge = cedge->next;
        }

        queue->first = RemoveElement(c,queue->first);
        c = queue->first;
    }
    
    int i = queue->max;
    FreeElements(queue->first);
    free(queue);
    return MakeSeenList(gr, i);
}

Element* MakeSeenList(Graph* gr, int max){
    Element* seenst = NULL;
    for (int i = 1; i < max; i++) {
        Vertex* current = gr->vertices;
        while (current) {
            if (current->seen == i) {
                seenst = InsertElementAtEnd(MakeElement(current), seenst);
                break; 
            }
            current = current->next;
        }
    }
    return seenst;
}

Element* GraphPaths(Graph *gr, Vertex *start, Vertex *end){
    if (!gr || !start || !end) return false;
    ClearSeen(gr->vertices);

    Path* path = (Path*)malloc(sizeof(Path));
    path->first = NULL;
    path->max = 1;

    Pathing(start, end, path, gr);
    Element* first = path->first;
    free(path);
    return first;
}

void Pathing(Vertex *current, Vertex *end, Path* path, Graph *gr)
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

void AddPath(Path* path, Graph *gr, int max) {
    for (int i = 1; i < max; i++) {
        Vertex* current = gr->vertices;
        while (current) {
            if (current->seen == i) {
                path->first = InsertElementAtEnd(MakeElement(current), path->first);
                break; 
            }
            current = current->next;
        }
    }
}

void FindIntersections(Graph *gr)
{
    if (!gr) return;

    printf("Vertices with intersections (degree > 2):\n");

    Vertex *v = gr->vertices;
    while (v)
    {
        int edgeCount = 0;
        Edge *e = v->edges;
        while (e)
        {
            edgeCount++;
            e = e->next;
        }

        if (edgeCount > 2)
        {
            printf("Vertex %c*%d at {%d,%d} with %d edges\n", v->value, v->id, v->pos.x, v->pos.y, edgeCount);
        }

        v = v->next;
    }
}


#pragma endregion


#pragma endregion

#pragma region Element

Element *MakeElement(Vertex* value)
{
    Element *e = (Element*)malloc(sizeof(Element));

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

Element *InsertElementAtEnd(Element* new, Element* st) {

    Element* current = st;
    if (current == NULL) return new;
    while (current->next) current = current->next;
    current->next = new;

    return st;
}

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


Element *FindElement(void* item,Element *st)
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

Element* FreeElements(Element *path)
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

bool ReadGraphFile(const char *filename, Graph *gr)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        Log("[ReadGraphFile] Tried and failed to open file : %s", filename);
        return false;
    }

    FreeGraph(gr);
    gr->vertices = NULL;

    fread(&sSize, sizeof(Vector2), 1, file);

    char ch;
    Vector2 pos;

    while (fread(&ch, sizeof(char), 1, file) == 1 && fread(&pos, sizeof(Vector2), 1, file) == 1)
    {
        gr->vertices = InsertVertex(MakeVertex(ch, pos, &gr->count), gr->vertices);
    }

    fclose(file);
    return true;
}

bool SaveGraphFile(const char *filename, Graph *gr)
{
    if (gr == NULL || gr->vertices == NULL)
        return false;

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        Log("[SaveGraphFile] Tried and failed to open file : %s", filename);
        return false;
    }

    fwrite(&sSize, sizeof(Vector2), 1, file);

    Vertex *current = gr->vertices;

    while (current != NULL)
    {
        fwrite(&current->value, sizeof(char), 1, file);
        fwrite(&current->pos, sizeof(Vector2), 1, file);
        current = current->next;
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
            printf("\n (%d, %d), %c", x, y, ch);
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
    return true;
    fclose(file);
}

bool CopyListToGraph(Node *st, Graph *gr)
{
    if (st == NULL)
    {
        Log("[CopyListToGraph] Missing list, *st is null");
        return false;
    }
    FreeGraph(gr);
    gr->vertices = NULL;
    Node *current = st;
    while (current != NULL)
    {
        if (current->value != '#')
        {
            gr->vertices = InsertVertex(MakeVertex(current->value, current->pos, &gr->count), gr->vertices);
            current = current->next;
            continue;
        }
        current = current->next;
    }
}

Node* CopyGraphToList(Graph *gr, Node *st)
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

