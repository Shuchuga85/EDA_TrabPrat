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
#define SWIDTH 30
#define SHEIGHT 15
#define NOISERANGE 3
#define MAXINPUT 100

#include <stdbool.h>

typedef struct Vector2
{
    int x;
    int y;
}Vector2;

typedef struct Node
{
    Vector2 pos;
    char value;
    struct Node* next;    
}Node;

Node* MakeNode(char value, Vector2 position);

bool NodeInBounds(Vector2 pos);

Node* InsertNode(Node* st ,Node* dnew);

Node *RemoveNode(Node *rm, Node *st);

Node* ClearNoise(Node* st);

bool ValidNodePos(Node *dnew, Node *st);

Node *NoiseCheck(Node *st);

Node* FindNodePos(Node* st, Vector2 npos);

bool Vector2Compare(Vector2 a, Vector2 b);

void DrawMatrix(Node *st);

Node* ReadListFile(const char* filename);

void SaveList(const char* filename, Node* st);

void DrawMenu();

void Menu(Node* st);

void DrawCommands();

void CommandIO(Node* st);

void ShowList(Node *st);

void Pause();

