/**
 * @file main.c
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief 
 * @version 0.10
 * @date 2025-03-18
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#include <stdio.h>
#include "func.h"
#include "interface.h"


int main(){

    InitLog();

    Node* st = NULL;
    Graph* gr = InitGraph();
    
    CommandIO(st, gr);

    /*
    int op = 0;
    printf("\n 1 - Options Menu");
    printf("\n 2 - Command Line");
    printf("\n 0 - Exit");
    printf("\n Option > ");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
        Menu(st, gr);
        break;
    case 2:
        CommandIO(st, gr);
        break;
    default:
        break;
    }

    */


      /*st = InsertNode(MakeNode('A',(Vector2){7,6}),NULL);
    st = InsertNode(MakeNode('A',(Vector2){5,7}),st);
    st = InsertNode(MakeNode('O',(Vector2){13,7}),st);
    st = InsertNode(MakeNode('O',(Vector2){13,9}),st);*/

    /*
    st = ReadListFile("file.txt");
    
    SaveList("file.txt",st);

    DrawMatrix(st);
    //ShowList(st);
    */
    return 0;
}