/**
 * @file func.c
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief
 * @version 0.1
 * @date 2025-03-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include "func.h"
#include <malloc.h>
#include <stdbool.h>

/**
 * @brief Cria um elemento para a lista de elementos
 *
 * @param value  = Valor do Elemento (ex: antena tipo A, efeito nefasto #).
 * @param position = Posicao na matrix.
 * @return Node*
 */
Node* MakeNode(char value, Vector2 position)
{
    if (!NodeInBounds(position))
        return NULL;

    Node *p = (Node *)malloc(sizeof(Node));

    if (p == NULL)
        return NULL;

    p->value = value;
    p->pos = position;
    p->next = NULL;

    return p;
}

/**
 * @brief
 *
 * @param pos
 * @return true
 * @return false
 */
bool NodeInBounds(Vector2 pos)
{
    return (pos.x >= 0 && pos.y >= 0 && pos.x < SWIDTH && pos.y < SHIGHT);
}

/**
 * @brief
 *
 * @param st
 * @param new
 * @return Node*
 */
Node* InsertNode(Node *dnew, Node *st)
{
    if (st == NULL) return dnew;
    if (dnew == NULL) return st;

    if (!ValidNodePos(dnew, st)) return st;

    dnew->next = st;
    st = dnew;

    st = NoiseCheck(st);

    return st;
}

bool ValidNodePos(Node *dnew, Node *st)
{
    if (FindNodePos(st, dnew->pos) != NULL) return false; // Se encontrar elemento na posicao x,y. nao e valido
    return true;
}

Node* NoiseCheck(Node *st)
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

        for (int y = -NOISERANGE; y <= NOISERANGE; y++)
        {
            for (int x = -NOISERANGE; x <= NOISERANGE; x++)
            {
                
                if (!NodeInBounds((Vector2){cpos.x + x, cpos.y + y}) || (x==0 && y==0)) continue; // Se a posicao esta fora da area ou a posicao e o propio elemento continua
                
                p = FindNodePos(st, (Vector2){cpos.x + x, cpos.y + y});

                if (p == NULL) continue; // Se nao ha elementos na posicao, continua

                if (p->value == current->value) // Se os elementos forem iguais, entao ha efeito nefasto
                { 
                    /*printf("\n {%d, %d}",cpos.x, cpos.y);
                    printf("\n {%d, %d}",x, y);
                    printf("\n {%d, %d}",p->pos.x, p->pos.y);
                    printf("\n {%d, %d}", p->pos.x + x, p->pos.y + y);*/
                    st = InsertNode(MakeNode('#',(Vector2){p->pos.x + x, p->pos.y + y}),st); // adiciona 
                }
            }
        }
        
        current = current->next;
    } 
    
    return st;
}

/**
 * @brief Retorna o elemento, se existe, com a posicao indicada
 *
 * @param st
 * @param npos
 * @return Node*
 */
Node* FindNodePos(Node *st, Vector2 npos)
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
 * @brief Compara a estrutura vetor com x e y iguais
 *
 * @param a
 * @param b
 * @return true
 * @return false
 */
bool Vector2Compare(Vector2 a, Vector2 b)
{
    return (a.x == b.x && a.y == b.y);
}

/**
 * @brief
 *
 * @param st
 */
void DrawMatrix(Node *st)
{
    Node *tp;
    Vector2 tpos;
    for (int y = 0; y < SHIGHT; y++)
    {
        printf("\n");
        for (int x = 0; x < SWIDTH; x++)
        {
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

void ShowList(Node *st){
    Node* current;
    printf("\n List: "); 
    while (current!= NULL)
    {
        printf("%c\t", current->value);
        current = current->next;
    }
    
}

void Pause()
{
    printf("\nPressione qualquer tecla para continuar...");

    getchar();
    getchar();
}
