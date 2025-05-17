/**
 * @file interface.h
 * @author Vitor Rezende (a31521@alunos.ipca.pt)
 * @brief Interface do utilizador e visualização de estruturas.
 * @version 0.10
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
 * @brief Gerencia o menu interativo de manipulação da lista de nós e do grafo.
 *
 * @param st Lista de nós.
 * @param gr Estrutura de grafo.
 */
void Menu(Node* st, Graph* gr);

/**
 * @brief Exibe os comandos disponíveis para manipulação da lista.
 */
void DrawCommands();

/**
 * @brief Interface de comandos para o utilizador controlar as operações da lista e do grafo.
 *
 * @param st Lista de nós.
 * @param gr Estrutura de grafo.
 */
void CommandIO(Node* st, Graph* gr);

/**
 * @brief Verifica se o nome de ficheiro termina com a extensão '.bin'.
 *
 * @param filename Nome do ficheiro.
 * @return true Se a extensão for '.bin'.
 * @return false Caso contrário.
 */
bool HasBinExtension(const char *filename);

/**
 * @brief Solicita ao utilizador confirmação para substituir um elemento existente na mesma posição.
 *
 * @param v Valor do novo elemento.
 * @param pos Posição onde o novo elemento será inserido.
 * @return true Se o utilizador confirmar a substituição.
 * @return false Caso contrário.
 */
bool AskReplace(char v, Vector2 pos);

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

/**
 * @brief Exibe todos os vértices do grafo no terminal, com filtro por tipo de antena.
 *
 * @param gr Grafo contendo os vértices.
 * @param filter Carácter para filtrar (ex: 'A', 'B'). Use '*' para mostrar todos.
 */
void ShowGraph(Graph * gr, char filter);

/**
 * @brief Exibe um percurso (caminho) entre dois vértices, se existir.
 *
 * @param st Lista de elementos contendo o percurso.
 * @param start Vértice de início.
 * @param end Vértice de fim.
 * @return true Se o percurso for exibido com sucesso.
 * @return false Se o percurso for inválido ou vazio.
 */
bool ShowPath(Element *st, Vertex* start, Vertex* end);

/**
 * @brief Exibe uma travessia (BFS ou DFS) completa a partir de um vértice.
 *
 * @param st Lista de elementos visitados.
 * @param label Título da travessia (ex: "DFS", "BFS").
 * @return true Se a travessia for válida e exibida.
 * @return false Se a travessia estiver vazia.
 */
bool ShowTraversal(Element *st, const char* label);

/**
 * @brief Exibe os pares de antenas com ressonância A-B ou B-A encontrados na travessia.
 *
 * @param list Lista de elementos resultante de uma travessia.
 */
void ShowResonancePairs(Element* list);

/**
 * @brief Pausa a execução do programa até que o utilizador pressione ENTER.
 */
void Pause();

/**
 * @brief Inicializa o sistema de registo (log), criando ou recriando o ficheiro de log.
 */
void InitLog();

/**
 * @brief Regista mensagens formatadas no ficheiro de log.
 *
 * @param format Texto formatado (igual ao printf).
 * @param ... Argumentos adicionais.
 */
void Log(const char *format, ...);

#endif 