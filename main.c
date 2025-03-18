/**
 * @file main.c
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

int main(){

    Node* st;
    
    st = InsertNode(MakeNode('A',(Vector2){7,6}),NULL);
    st = InsertNode(MakeNode('A',(Vector2){5,7}),st);
    st = InsertNode(MakeNode('O',(Vector2){13,7}),st);
    st = InsertNode(MakeNode('O',(Vector2){13,9}),st);


    DrawMatrix(st);
    //ShowList(st);

    Pause();
    return 0;
}