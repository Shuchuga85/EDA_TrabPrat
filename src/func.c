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
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
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

Node* FreeList(Node *head)
{
    if (head == NULL)
        return head;

    Node *current = head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    return head;
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

    RemoveEdges(rm);

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

bool RemoveEdges(Vertex *old)
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
                Log("[RemoveEdges] previous is null");

                cedge->dest->edges = current->next; // first
            }
            else
            {
                Log("[RemoveEdges] previous is not null");

                previous->next = current->next; // in the middle
            }
            free(current);
        }

        Log("[RemoveEdges] removing cedge");

        next = cedge->next;
        free(cedge);
        cedge = next;
    }

    Log("[AddEdges] Successfully Removed edges for vertex : %c {%d,%d}", old->value, old->pos.x, old->pos.y);
    return 1;
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

#pragma region Output

/**
 * @brief Desenha a matriz com os elementos presentes na lista.
 *
 * @param st Lista de nós.
 */
void DrawMatrix(Node *st)
{
    if (st == NULL)
    {
        Log("[DrawMatrix] Missing list, *st is null");
    }

    Node *tp;
    Vector2 tpos;

    for (int y = 0; y < sSize.y; y++)
    {
        printf("\n");
        for (int x = 0; x < sSize.x; x++)
        {
            if (st == NULL)
            {
                printf(". ");
                continue;
            }

            tp = FindNodePos(st, (Vector2){x, y});
            if (tp != NULL)
            {
                printf("%c", tp->value);
            }
            else
            {
                printf(".");
            }
            printf(" ");
        }
    }
}

/**
 * @brief Exibe a lista de nós, com um filtro opcional.
 *
 * @param st Lista de nós.
 * @param filter Caracter usado para filtrar os elementos exibidos.
 */
void ShowList(Node *st, char filter)
{
    if (st == NULL)
    {
        Log("[ShowList] Missing list, *st is null");
        printf("\n List is empty!");
        return;
    }

    Node *current = st;
    printf("\n| Value | Position |");
    while (current != NULL)
    {
        if (filter == '.' || filter == ' ')
        {
            printf("\n|   %c   | ", current->value);
            printf("{%-2d,%-2d}  |", current->pos.x, current->pos.y);
            // printf("%c\t", current->value);
        }
        else
        {
            if (current->value == filter)
            {
                printf("\n|   %c   | ", current->value);
                printf("{%-2d,%-2d}  |", current->pos.x, current->pos.y);
            }
        }

        current = current->next;
    }
}

void ShowGraph(Graph *gr, char filter)
{
    if (gr == NULL)
    {
        Log("[ShowGraph] Missing graph, *gr is null");
        printf("\n Graph is empty!");
        return;
    }

    Vertex *current = gr->vertices;
    Edge *cedge = NULL;
    printf("\n| Value | Position | Edges");
    while (current != NULL)
    {
        if (filter == '.' || filter == ' ')
        {
            printf("\n|  %c*%d  | ", current->value, current->id);
            printf("{%-2d,%-2d}  | ", current->pos.x, current->pos.y);
            cedge = current->edges;
            while (cedge != NULL)
            {
                printf(" %c*%d (w:%.2f) -", cedge->dest->value, cedge->dest->id, cedge->weight);
                cedge = cedge->next;
            }
        }
        else
        {
            if (current->value == filter)
            {
                printf("\n|  %c*%d  | ", current->value, current->id);
                printf("{%-2d,%-2d}  |", current->pos.x, current->pos.y);
            }
            cedge = current->edges;
            while (cedge != NULL)
            {
                printf(" %c*%d (w:%.2f) -", cedge->dest->value, cedge->dest->id, cedge->weight);
                cedge = cedge->next;
            }
        }
        current = current->next;
    }
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

    FreeList(st);
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
    FreeList(st);
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

#pragma region Interface

/**
 * @brief Desenha o menu de opções no terminal.
 */
void DrawMenu()
{
    printf("\n\n ..... Main Menu .....");
    printf("\n 1 -> Add element;");
    printf("\n 2 -> Remove element;");
    printf("\n 3 -> Show matrix;");
    printf("\n 4 -> Show list;");
    printf("\n 5 -> Show noise;");
    printf("\n 6 -> Load file;");
    printf("\n 7 -> Save file;");
    printf("\n 8 -> Settings;");
    printf("\n 0 -> Exit;\n");
}

/**
 * @brief Gerencia a interface do utilizador e manipulação da lista de nós.
 *
 * @param st Lista de nós.
 */
void Menu(Node *st, Graph *gr)
{
    Log("Starting menu...");
    int op = 0;
    Vector2 pos;
    char value;
    int range;
    char filename[MAXINPUT];
    do
    {
        DrawMenu();
        printf("\nChoose Option : ");
        scanf("%d", &op);

        switch (op)
        {
        case 0:
            break;
        case 1:
            printf("\n Adding element:");
            getchar();
            printf("\n Insert value (type:char) : ");
            scanf("%c", &value);
            printf("\n Insert coordinates (type:vector2int) x,y : ");
            scanf("%d,%d", &pos.x, &pos.y);
            st = InsertNode(MakeNode(value, (Vector2){pos.x, pos.y}), st);
            break;
        case 2:
            getchar();
            printf("\n Removing element:");
            printf("\n Insert coordinates (type:vector2int) x,y : ");
            scanf("%d,%d", &pos.x, &pos.y);
            st = RemoveNode(FindNodePos(st, pos), st);
            break;
        case 3:
            DrawMatrix(st);
            Pause();
            break;
        case 4:
            ShowList(st, '.');
            break;
        case 5:
            ShowList(st, '#');
            break;
        case 6:
            printf("\n Insert file name (default:%s) : ", SFILE);
            scanf("%s", filename);
            if (strcmp(filename, " ") == 0)
            {
                st = ReadListFile(SFILE, st);
                if (st != NULL)
                    printf("\nRead list successfuly!\n");
                else
                    printf("\nFailed to read list!\n");

                CopyListToGraph(st, gr);
            }
            else
            {
                st = ReadListFile(filename, st);
                if (st != NULL)
                    printf("\nRead list successfuly!\n");
                else
                    printf("\nFailed to read list!\n");
                CopyListToGraph(st, gr);
            }
            break;
        case 7:
            printf("\n Insert file name (default:%s) : ", SFILE);
            scanf("%s", filename);
            if (strcmp(filename, " ") == 0)
            {
                if (SaveList(SFILE, st))
                    printf("\nSaved list successfuly!\n");
                else
                    printf("\nFailed to save list!\n");
            }
            else
            {
                if (SaveList(filename, st))
                    printf("\nSaved list successfuly!\n");
                else
                    printf("\nFailed to save list!\n");
            }
            break;
        case 8:
            getchar();
            printf("\n a -> Change Scale (current: {%d, %d})", sSize.x, sSize.y);
            printf("\n b -> Change Noise range (current: %d)", noiseRange);
            printf("\n Choose option: ");
            int o = getchar();
            if (o == 'a')
            {
                printf("\n Insert scale (type:vector2int) x,y: ");
                scanf("%d,%d", &pos.x, &pos.y);
                sSize.x = pos.x;
                sSize.y = pos.y;
            }
            else if (0 == 'b')
            {
                printf("\n Insert scale (type:vector2int) x,y: ");
                scanf("%d", range);
                noiseRange = range;
                st = ClearNoise(st);
                st = NoiseCheck(st);
            }
            else
            {
                printf("\n Invalid option, try again.");
            }
            break;
        default:
            printf("\n Invalid option, try again.");
            break;
        }

    } while (op != 0);
}

/**
 * @brief Exibe os comandos disponíveis para manipulação da lista.
 */
void DrawCommands()
{
    printf("\n\n ..... Commands .....");
    printf("\n > add [char] [x,y]");
    printf("\n > remove [x,y]");
    printf("\n > show [matrix|list|noise|graph]");
    printf("\n > load [filename.txt]");
    printf("\n > save [filename.txt]");
    printf("\n > set scale [x,y]");
    printf("\n > set noiseRange [int]");
    printf("\n > clear");
    printf("\n > exit\n");
}

/**
 * @brief Interface baseada em comandos para manipulação da lista de elementos.
 *
 * @param st Lista de nós.
 */
void CommandIO(Node *st, Graph *gr)
{
    Log("Starting commandIO...");

    char ip[MAXINPUT];
    Vector2 pos;
    char value;
    do
    {
        DrawCommands();
        printf("\nEnter command > ");

        fgets(ip, MAXINPUT, stdin);

        ip[strcspn(ip, "\n")] = 0;

        char *command = strtok(ip, " ");

        if (command == NULL)
        {
            printf("\n No commands detected, try again.");
            continue;
        }

        if (strcmp(command, "add") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            char *arg2 = strtok(NULL, ", ");
            char *arg3 = strtok(NULL, ", ");

            if (arg1 != NULL && arg2 != NULL && arg3 != NULL)
            {
                value = arg1[0];
                pos.x = atoi(arg2);
                pos.y = atoi(arg3);
                st = InsertNode(MakeNode(value, (Vector2){pos.x, pos.y}), st);
                gr->vertices = InsertVertex(MakeVertex(value, (Vector2){pos.x, pos.y}, &gr->count), gr->vertices);
            }
            else
            {
                printf("\nMissing parameter! [char] [x,y].");
            }
        }
        else if (strcmp(command, "remove") == 0)
        {
            char *arg1 = strtok(NULL, ", ");
            char *arg2 = strtok(NULL, ", ");
            if (arg1 != NULL && arg2 != NULL)
            {
                pos.x = atoi(arg1);
                pos.y = atoi(arg2);
                st = RemoveNode(FindNodePos(st, pos), st);
                gr->vertices = RemoveVertex(FindVertexAt(gr->vertices, pos), gr->vertices);
            }
            else
            {
                printf("\nMissing parameter! [x,y].");
            }
        }
        else if (strcmp(command, "show") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            if (arg1 != NULL)
            {
                if (strcmp(arg1, "matrix") == 0)
                {
                    DrawMatrix(st);
                }
                else if (strcmp(arg1, "list") == 0)
                {
                    ShowList(st, '.');
                }
                else if (strcmp(arg1, "noise") == 0)
                {
                    ShowList(st, '#');
                }
                else if (strcmp(arg1, "graph") == 0)
                {
                    ShowGraph(gr, ' ');
                }
                else
                {
                    printf("\nInvalid parameter! (matrix,list).");
                }
                Pause();
            }
            else
            {
                // printf("\nMissing parameter! (matrix,list).");
                DrawMatrix(st);
                Pause();
            }
        }
        else if (strcmp(command, "load") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            arg1 = (arg1 == NULL) ? SFILE : arg1;

            if (HasBinExtension(arg1))
            {
                if (ReadGraphFile(arg1, gr))
                    printf("\nRead graph successfuly!\n");
                else
                    printf("\nFailed to read graph!\n");
                st = CopyGraphToList(gr, st);
            }
            else
            {
                if ((st=ReadListFile(arg1, st)) != NULL)
                    printf("\nRead list successfuly!\n");
                else
                    printf("\nFailed to read list!\n");
                CopyListToGraph(st, gr);
            }
        }
        else if (strcmp(command, "save") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            arg1 = (arg1 == NULL) ? SFILE : arg1;

            if (HasBinExtension(arg1))
            {
                if (SaveGraphFile(arg1, gr))
                    printf("\nSaved graph successfuly!\n");
                else
                    printf("\nFailed to save graph!\n");
            }
            else
            {
                if (SaveList(arg1, st))
                    printf("\nSaved list successfuly!\n");
                else
                    printf("\nFailed to save list!\n");
            }
        }
        else if (strcmp(command, "set") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            if (arg1 != NULL)
            {
                if (strcmp(arg1, "scale") == 0)
                {
                    char *arg2 = strtok(NULL, ", ");
                    char *arg3 = strtok(NULL, ", ");
                    if (arg2 != NULL && arg3 != NULL)
                    {
                        sSize.x = atoi(arg2);
                        sSize.y = atoi(arg3);
                        printf("\n Matrix limits set to : {%d, %d}", sSize.x, sSize.y);
                    }
                    else
                    {
                        printf("\n Matrix limits set to : {%d, %d}", sSize.x, sSize.y);
                    }
                }
                else if (strcmp(arg1, "noiseRange") == 0)
                {
                    char *arg2 = strtok(NULL, " ");
                    if (arg2 != NULL)
                    {
                        noiseRange = atoi(arg2);
                        st = ClearNoise(st);
                        st = NoiseCheck(st);
                        printf("\n Noise range set to : %d", noiseRange);
                    }
                    else
                    {
                        printf("\n Noise range set to : %d", noiseRange);
                    }
                }
                else
                {
                    printf("\nInvalid parameter! (scale, noiseRange).");
                }
            }
        }
        else if (strcmp(ip, "clear") == 0)
        {
            FreeGraph(gr);
            st = FreeList(st);
        }
        else if (strcmp(ip, "exit") != 0)
        {
            printf("\n No valid commands detected, try again.");
        }

    } while (strcmp(ip, "exit") != 0);
}

bool HasBinExtension(const char *filename)
{
    if (filename == NULL)
        return false;

    const char *ext = strrchr(filename, '.'); // procura o último ponto
    return (ext != NULL && strcmp(ext, ".bin") == 0);
}

#pragma endregion

#pragma region Debug

/**
 * @brief Pausa a execução do programa até que o usuário pressione uma tecla.
 */
void Pause()
{
    printf("\n\nPressione qualquer tecla para continuar...");
    getchar();
    getchar();
}

void InitLog()
{
    remove(LOG_OLD);

    rename(LOG, LOG_OLD);

    FILE *logFile = fopen(LOG, "w");
    if (logFile)
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(logFile, "[%02d:%02d:%02d] Log initialized\n",
                t->tm_hour, t->tm_min, t->tm_sec);
        fclose(logFile);
    }
}

void Log(const char *format, ...)
{
    FILE *logFile = fopen(LOG, "a");
    if (!logFile)
        return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(logFile, "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);

    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fprintf(logFile, "\n");
    fclose(logFile);
}

#pragma endregion