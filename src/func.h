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
#define SWIDTH 12
#define SHEIGHT 12
#define NOISERANGE 4
#define MAXINPUT 100
#define SFILE "file.txt"

#include <stdbool.h>

/**
 * @brief Representa um vetor bidimensional com coordenadas inteiras.
 *
 * A estrutura Vector2 é usada para armazenar posições em uma matriz,
 * com valores inteiros para coordenadas X e Y.
 */
typedef struct Vector2
{
    int x;  ///< Coordenada X
    int y;  ///< Coordenada Y
}Vector2;

/**
 * @brief Representa um nó de uma lista ligada de elementos na matriz.
 *
 * Cada nó contém um valor (por exemplo, um tipo de antena ou um efeito nefasto),
 * uma posição na matriz representada por um Vector2, e um ponteiro para o próximo nó da lista.
 */
typedef struct Node
{
    Vector2 pos;        ///< Posição do nó na matriz
    char value;         ///< Valor do nó (ex: antena tipo A, efeito nefasto '#')
    struct Node* next;  ///< Ponteiro para o próximo nó na lista
}Node;

/**
 * @brief Cria um novo elemento (nó) para a lista.
 *
 * @param value Valor do elemento (ex: antena tipo A, efeito nefasto '#').
 * @param position Posição na matriz.
 * @return Ponteiro para o novo nó criado.
 */
Node* MakeNode(char value, Vector2 position);

/**
 * @brief Verifica se a posição está dentro dos limites da matriz.
 *
 * @param pos Posição a ser verificada.
 * @return true se estiver dentro dos limites, false caso contrário.
 */
bool NodeInBounds(Vector2 pos);

/**
 * @brief Insere um novo nó na lista, verificando se a posição está disponível.
 *
 * @param dnew Novo nó a ser inserido.
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista atualizada.
 */
Node* InsertNode(Node* st ,Node* dnew);

/**
 * @brief Remove um nó da lista.
 *
 * @param rm Ponteiro para o nó a ser removido.
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista atualizada.
 */
Node *RemoveNode(Node *rm, Node *st);

/**
 * @brief Limpa elementos de ruído ('#') da lista.
 *
 * @param st Ponteiro para a lista.
 * @return Ponteiro para a lista sem elementos de ruído.
 */
Node* ClearNoise(Node* st);

/**
 * @brief Verifica se a posição de um novo nó é válida (sem sobreposição).
 *
 * @param dnew Novo nó.
 * @param st Ponteiro para a lista.
 * @return true se a posição for válida, false caso contrário.
 */
bool ValidNodePos(Node *dnew, Node *st);

/**
 * @brief Verifica e aplica a regra de efeito nefasto aos elementos na lista.
 *
 * @param st Lista de nós.
 * @return Node* Lista atualizada após a verificação do efeito nefasto.
 */
Node *NoiseCheck(Node *st);

/**
 * @brief Versão alternativa da verificação de ruído que compara todos os elementos entre si.
 *
 * @param st Lista de nós.
 * @return Node* Lista atualizada após a verificação do efeito nefasto.
 */
Node *NoiseCheckAlt(Node *st);

/**
 * @brief Procura um nó em uma posição específica.
 *
 * @param st Lista de nós.
 * @param npos Posição a ser pesquisada.
 * @return Node* Ponteiro para o nó encontrado ou NULL se não existir.
 */
Node* FindNodePos(Node* st, Vector2 npos);

/**
 * @brief Compara se dois vetores possuem as mesmas coordenadas.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return true Se forem iguais.
 * @return false Se forem diferentes.
 */
bool Vector2Compare(Vector2 a, Vector2 b);

/**
 * @brief Subtrai um vetor do outro.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return Vector2 Resultado da subtração.
 */
Vector2 Vector2Subtract(Vector2 a, Vector2 b);

/**
 * @brief Soma dois vetores.
 *
 * @param a Primeiro vetor.
 * @param b Segundo vetor.
 * @return Vector2 Resultado da soma.
 */
Vector2 Vector2Add(Vector2 a, Vector2 b);

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
 * @brief Lê uma lista de elementos a partir de um ficheiro.
 *
 * @param filename Nome do ficheiro.
 * @return Node* Lista carregada do ficheiro.
 */
Node* ReadListFile(const char* filename);

/**
 * @brief Guarda a lista de elementos num ficheiro.
 *
 * @param filename Nome do ficheiro onde os dados serão armazenados.
 * @param st Lista de nós a ser salva.
 */
void SaveList(const char* filename, Node* st);

/**
 * @brief Desenha o menu de opções no terminal.
 */
void DrawMenu();

/**
 * @brief Gerencia a interface do utilizador e manipulação da lista de nós.
 *
 * @param st Lista de nós.
 */
void Menu(Node* st);

/**
 * @brief Exibe os comandos disponíveis para manipulação da lista.
 */
void DrawCommands();

/**
 * @brief Interface baseada em comandos para manipulação da lista de elementos.
 *
 * @param st Lista de nós.
 */
void CommandIO(Node* st);

/**
 * @brief Pausa a execução do programa até que o usuário pressione uma tecla.
 */
void Pause();

