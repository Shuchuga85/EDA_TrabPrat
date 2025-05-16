/**
 * @file interface.h
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief 
 * @version 0.1
 * @date 2025-05-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#define LOG "log.txt"
#define LOG_OLD "log_old.txt"
#define SFILE "file.txt"

 /**
 * @brief Desenha o menu de opções no terminal.
 */
void DrawMenu();

/**
 * @brief Gerencia a interface do utilizador e manipulação da lista de nós.
 *
 * @param st Lista de nós.
 */
void Menu(Node* st, Graph* gr);

/**
 * @brief Exibe os comandos disponíveis para manipulação da lista.
 */
void DrawCommands();

/**
 * @brief Interface baseada em comandos para manipulação da lista de elementos.
 *
 * @param st Lista de nós.
 */
void CommandIO(Node* st, Graph* gr);

bool HasBinExtension(const char *filename);

/**
 * @brief Desenha a matriz com os elementos presentes na lista.
 *
 * @param st Lista de nós.
 */
void DrawMatrix(Node *st);

/**
 * @brief Exibe a lista de nós, com um filtro opcional.
 *
 * @param st Lista de nós.
 * @param filter Caracter usado para filtrar os elementos exibidos.
 */
void ShowList(Node *st, char filter);

void ShowGraph(Graph * gr, char filter);

bool ShowPath(Element *st, Vertex* start, Vertex* end);

bool ShowTraversal(Element *st, const char* label);

/**
 * @brief Pausa a execução do programa até que o usuário pressione uma tecla.
 */
void Pause();

void InitLog();

void Log(const char *format, ...);

#endif 