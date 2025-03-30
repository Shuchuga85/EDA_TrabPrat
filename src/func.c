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
        return NULL;

    p->value = value;
    p->pos = position;
    p->next = NULL;

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

    if (!ValidNodePos(dnew, st))
    {
        if (dnew->value != '#')
        {
            printf("\n Posicao {%d,%d} ocupada! Elemeto %c discartado.", dnew->pos.x, dnew->pos.y, dnew->value);
        }

        free(dnew);
        return st;
    }

    dnew->next = st;
    st = dnew;

    st = NoiseCheckAlt(st);

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

#pragma endregion

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
    Node *current = st;
    printf("\n| Value | Position |");
    while (current != NULL)
    {
        if (filter == '.')
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

#pragma endregion

#pragma region File

/**
 * @brief Lê uma lista de elementos a partir de um ficheiro.
 *
 * @param filename Nome do ficheiro.
 * @return Node* Lista carregada do ficheiro.
 */
Node *ReadListFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file!\n");
        return NULL;
    }

    Node *st = NULL;
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
void SaveList(const char *filename, Node *st)
{
    if (st == NULL)
        return;
    Node *tp;
    Vector2 tpos;
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error opening file!\n");
        return;
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
void Menu(Node *st)
{

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
                st = ReadListFile(SFILE);
            }
            else
            {
                st = ReadListFile(filename);
            }
            break;
        case 7:
            printf("\n Insert file name (default:%s) : ", SFILE);
            scanf("%s", filename);
            if (strcmp(filename, " ") == 0)
            {
                SaveList(SFILE, st);
            }
            else
            {
                SaveList(filename, st);
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
    printf("\n > show [matrix|list|noise]");
    printf("\n > load [filename.txt]");
    printf("\n > save [filename.txt]");
    printf("\n > set scale [x,y]");
    printf("\n > set noiseRange [int]");
    printf("\n > exit\n");
}

/**
 * @brief Interface baseada em comandos para manipulação da lista de elementos.
 *
 * @param st Lista de nós.
 */
void CommandIO(Node *st)
{
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
                    Pause();
                }
                else if (strcmp(arg1, "list") == 0)
                {
                    ShowList(st, '.');
                }
                else if (strcmp(arg1, "noise") == 0)
                {
                    ShowList(st, '#');
                }
                else
                {
                    printf("\nInvalid parameter! (matrix,list).");
                }
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
            if (arg1 != NULL)
            {
                st = ReadListFile(arg1);
            }
            else
            {
                st = ReadListFile(SFILE);
            }
        }
        else if (strcmp(command, "save") == 0)
        {
            char *arg1 = strtok(NULL, " ");
            if (arg1 != NULL)
            {
                SaveList(arg1, st);
            }
            else
            {
                SaveList(SFILE, st);
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
        else if (strcmp(ip, "exit") != 0)
        {
            printf("\n No valid commands detected, try again.");
        }

    } while (strcmp(ip, "exit") != 0);
}

#pragma endregion

/**
 * @brief Pausa a execução do programa até que o usuário pressione uma tecla.
 */
void Pause()
{
    printf("\nPressione qualquer tecla para continuar...");
    getchar();
    getchar();
}
