/**
 * @file interface.c
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief Interface do utilizador e visualização de estruturas.
 * @version 0.10
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "interface.h"
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

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
                    printf("\nRead list successfully!\n");
                else
                    printf("\nFailed to read list!\n");

                CopyListToGraph(st, gr);
            }
            else
            {
                st = ReadListFile(filename, st);
                if (st != NULL)
                    printf("\nRead list successfully!\n");
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
                    printf("\nSaved list successfully!\n");
                else
                    printf("\nFailed to save list!\n");
            }
            else
            {
                if (SaveList(filename, st))
                    printf("\nSaved list successfully!\n");
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
    printf("\n > add [char_freq] [int_x,int_y]");
    printf("\n > remove [int_x,int_y]");
    printf("\n > show [matrix|list|noise|graph]");
    printf("\n > load [filename.txt/bin]");
    printf("\n > save [filename.txt/bin]");
    printf("\n > set scale [x,y]");
    printf("\n > set noiseRange [int]");
    printf("\n > edge [add|remove] [from_vertex_id] [to_vertex_id]");
    printf("\n > graph [dfs|bfs] [start_vertex_id]");
    printf("\n > paths [start_vertex_id] [end_vertex_id]");
    printf("\n > pair [char_a] [char_b]");
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
                gr->vertices = InsertVertex(MakeVertex(value, (Vector2){pos.x, pos.y}, &gr->count), gr->vertices, true);
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
                    printf("\nRead graph successfully!\n");
                else
                    printf("\nFailed to read graph!\n");
                st = CopyGraphToList(gr, st);
            }
            else
            {
                if ((st = ReadListFile(arg1, st)) != NULL)
                    printf("\nRead list successfully!\n");
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
                    printf("\nSaved graph successfully!\n");
                else
                    printf("\nFailed to save graph!\n");
            }
            else
            {
                if (SaveList(arg1, st))
                    printf("\nSaved list successfully!\n");
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
        else if (strcmp(ip, "edge") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            if (arg1 != NULL)
            {
                if (strcmp(arg1, "add") == 0)
                {
                    char *from_id_str = strtok(NULL, " ");
                    char *to_id_str = strtok(NULL, " ");
                    if (!from_id_str || !to_id_str)
                    {
                        printf("\nMissing 'from' or 'to' vertex id.\n");
                        continue;
                    }
                    int from_id = atoi(from_id_str);
                    int to_id = atoi(to_id_str);
                    Vertex *from = FindVertexById(gr->vertices, from_id);
                    Vertex *to = FindVertexById(gr->vertices, to_id);

                    if (!from || !to)
                    {
                        printf("\n 'from' or 'to'vertex not found!\n");
                        continue;
                    }

                    if (InsertEdge(from, to))
                        printf("\nAdded edge successfully!");
                    else
                        printf("\nFailed to add edge!");
                }
                else if (strcmp(arg1, "remove") == 0)
                {
                    char *from_id_str = strtok(NULL, " ");
                    char *to_id_str = strtok(NULL, " ");
                    if (!from_id_str || !to_id_str)
                    {
                        printf("\nMissing 'from' or 'to' vertex id.\n");
                        continue;
                    }
                    int from_id = atoi(from_id_str);
                    int to_id = atoi(to_id_str);
                    Vertex *from = FindVertexById(gr->vertices, from_id);
                    Vertex *to = FindVertexById(gr->vertices, to_id);

                    if (!from || !to)
                    {
                        printf("\n 'from' or 'to'vertex not found!\n");
                        continue;
                    }

                    if (RemoveEdge(from, to))
                        printf("\nRemoved edge successfully!");
                    else
                        printf("\nFailed to remove edge!");
                }
                else
                {
                    printf("\nInvalid parameter! (add, remove).");
                }
                Pause();
            }
        }
        else if (strcmp(command, "graph") == 0)
        {
            char *subcmd = strtok(NULL, " ");
            if (subcmd == NULL)
            {
                printf("\nMissing subcommand for graph! Use: dfs, bfs, paths, intersections.\n");
                continue;
            }

            if (strcmp(subcmd, "dfs") == 0)
            {
                char *start_id_str = strtok(NULL, " ");
                if (start_id_str == NULL)
                {
                    printf("\nMissing start vertex id for dfs.\n");
                    continue;
                }
                int start_id = atoi(start_id_str);
                Vertex *start = FindVertexById(gr->vertices, start_id);
                if (!start)
                {
                    printf("\nStart vertex not found!\n");
                    continue;
                }
                // DFS(gr, start);
                ShowTraversal(GraphDFS(gr, start), "DFS");
                Pause();
            }
            else if (strcmp(subcmd, "bfs") == 0)
            {
                char *start_id_str = strtok(NULL, " ");
                if (start_id_str == NULL)
                {
                    printf("\nMissing start vertex id for bfs.\n");
                    continue;
                }
                int start_id = atoi(start_id_str);
                Vertex *start = FindVertexById(gr->vertices, start_id);
                if (!start)
                {
                    printf("\nStart vertex not found!\n");
                    continue;
                }
                // BFS(gr, start);
                ShowTraversal(GraphBFS(gr, start), "BFS");
                Pause();
            }
            else if (strcmp(subcmd, "paths") == 0)
            {
                char *start_id_str = strtok(NULL, " ");
                char *end_id_str = strtok(NULL, " ");
                if (!start_id_str || !end_id_str)
                {
                    printf("\nMissing start or end vertex id for paths.\n");
                    continue;
                }
                int start_id = atoi(start_id_str);
                int end_id = atoi(end_id_str);
                Vertex *start = FindVertexById(gr->vertices, start_id);
                Vertex *end = FindVertexById(gr->vertices, end_id);
                if (!start || !end)
                {
                    printf("\nStart or end vertex not found!\n");
                    continue;
                }
                // FindPaths(gr, start, end);
                // GraphPaths(gr, start, end);
                ShowPath(GraphPaths(gr, start, end), start, end);
                Pause();
            }
            else if (strcmp(subcmd, "intersections") == 0)
            {
                /*FindIntersections(gr);
                Pause();*/
            }
            else
            {
                printf("\nUnknown graph subcommand: %s\n", subcmd);
            }
        }
        else if (strcmp(ip, "paths") == 0)
        {
            char *start_id_str = strtok(NULL, " ");
            char *end_id_str = strtok(NULL, " ");
            if (!start_id_str || !end_id_str)
            {
                printf("\nMissing start or end vertex id for paths.\n");
                continue;
            }
            int start_id = atoi(start_id_str);
            int end_id = atoi(end_id_str);
            Vertex *start = FindVertexById(gr->vertices, start_id);
            Vertex *end = FindVertexById(gr->vertices, end_id);
            if (!start || !end)
            {
                printf("\nStart or end vertex not found!\n");
                continue;
            }

            ShowPath(GraphPaths(gr, start, end), start, end);
            Pause();
        }
        else if (strcmp(ip, "pair") == 0)
        {
            char *freqA_str = strtok(NULL, " ");
            char *freqB_str = strtok(NULL, " ");
            if (!freqA_str || !freqB_str || strlen(freqA_str) != 1 || strlen(freqB_str) != 1)
            {
                printf("\nInvalid resonance: <char A> <char B>\n");
                continue;
            }

            char freqA = freqA_str[0];
            char freqB = freqB_str[0];

            Element *abPairs = FindPairs(gr, freqA, freqB);
            ShowResonancePairs(abPairs);
            Pause();
        }
        else if (strcmp(ip, "clear") == 0)
        {

            FreeGraph(gr);
            st = FreeNodes(st);
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

bool AskReplace(char v, Vector2 pos)
{
    getchar();
    if (v == '#')
    {
        printf("\n Position {%d,%d} have noise! Want to insert in noise? [y,n]", pos.x, pos.y);
    }
    else
    {
        printf("\n Position {%d,%d} occupied! Want to replace %c with new one? [y,n]", pos.x, pos.y, v);
    }
    char choice;
    scanf("%c", &choice);
    return (choice != 'y' && choice != 'Y');
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
    if (current != NULL)
    {
        printf("\n| Val | id | Position | Edges");
    }
    else
    {
        printf("\nThe Graph is empty!");
    }

    while (current != NULL)
    {
        if (filter == '.' || filter == ' ')
        {
            printf("\n|  %c  | ", current->value);
            printf("%-2d | ", current->id);
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

bool ShowPath(Element *st, Vertex *start, Vertex *end)
{
    if (st == NULL)
    {
        Log("[ShowPath] Missing elements, *st is null");
        return false;
    }

    printf("\n Paths from  %c*%d {%-2d,%-2d} to %c*%d {%-2d,%-2d} \n",
           start->value, start->id, start->pos.x, start->pos.y,
           end->value, end->id, end->pos.x, end->pos.y);

    Element *current = st;
    printf("\nPath: ");
    while (current)
    {
        if (current->item == end)
        {
            printf("%c*%d", current->item->value, current->item->id);
            if (current->next != NULL)
            {
                printf("\nPath: ");
            }
        }
        else
        {
            printf("%c*%d -> ", current->item->value, current->item->id);
        }
        current = current->next;
    }
    FreeElements(st);
    return true;
}

bool ShowTraversal(Element *st, const char *label)
{
    if (!st || !label)
    {
        Log("[ShowTraversal] Missing parameters.");
        return false;
    }

    printf("\n [%s Traversal] Starting from %c*%d {%2d,%2d} \n",
           label,
           st->item->value, st->item->id, st->item->pos.x, st->item->pos.y);

    printf("\nTraversal: ");
    Element *current = st;
    while (current)
    {
        printf("%c*%d", current->item->value, current->item->id);
        if (current->next)
            printf(" -> ");
        current = current->next;
    }
    printf("\n");

    FreeElements(st);
    return true;
}

void ShowResonancePairs(Element *list)
{
    if (!list)
    {
        printf("[ResonancePairs] Empty list.\n");
        return;
    }
    bool once = true;
    Element *current = list;
    while (current && current->next)
    {
        Vertex *a = current->item;
        Vertex *b = current->next->item;

        if (once)
        {
            printf("\nPairs with frequency: %c - %c:\n", a->value, b->value);
            once = false;
        }

        printf("Pair: %c*%d (%d,%d) <-> %c*%d (%d,%d)\n",
               a->value, a->id, a->pos.x, a->pos.y,
               b->value, b->id, b->pos.x, b->pos.y);

        current = current->next->next;
    }

    FreeElements(list);
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