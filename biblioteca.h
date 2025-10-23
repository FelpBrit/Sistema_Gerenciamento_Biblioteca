/**
 * =============================================================================
 * SISTEMA DE GERENCIAMENTO DE BIBLIOTECA
 * =============================================================================
 * Arquivo: biblioteca.h
 * Descrição: Header com todas as estruturas de dados e declarações de funções
 * Autores: [INSIRA NOMES DA EQUIPE]
 * Data: Outubro 2025
 * =============================================================================
 */

#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// =============================================================================
// CONSTANTES DO SISTEMA
// =============================================================================

#define MAX_TITULO 100      // Tamanho máximo para título do livro
#define MAX_AUTOR 100       // Tamanho máximo para nome do autor
#define MAX_ISBN 20         // Tamanho máximo para ISBN
#define MAX_NOME_LEITOR 100 // Tamanho máximo para nome do leitor

// =============================================================================
// ESTRUTURA 1: LISTA ENCADEADA (CATÁLOGO DE LIVROS)
// =============================================================================

/**
 * Estrutura que representa um LIVRO no catálogo
 */
typedef struct {
    char titulo[MAX_TITULO];              // Título do livro
    char autor[MAX_AUTOR];                // Nome do autor
    int ano_publicacao;                   // Ano de publicação
    char isbn[MAX_ISBN];                  // ISBN (opcional)
    bool status;                          // true = disponível, false = emprestado
    char nome_leitor_atual[MAX_NOME_LEITOR]; // Nome do leitor (se emprestado)
    time_t data_emprestimo;               // Data do empréstimo (timestamp)
} Livro;

/**
 * Nó da Lista Encadeada de Livros
 */
typedef struct NoLivro {
    Livro dados;                // Dados do livro
    struct NoLivro* proximo;    // Ponteiro para o próximo nó
} NoLivro;

/**
 * Estrutura da Lista Encadeada (Catálogo)
 */
typedef struct {
    NoLivro* cabeca;    // Ponteiro para o primeiro livro
    int total;          // Total de livros no catálogo
} ListaLivros;

// =============================================================================
// ESTRUTURA 2: FILA (LISTA DE ESPERA)
// =============================================================================

/**
 * Estrutura que representa uma SOLICITAÇÃO na fila de espera
 */
typedef struct {
    char nome_leitor[MAX_NOME_LEITOR];  // Nome do leitor que está aguardando
    char titulo_livro[MAX_TITULO];      // Título do livro desejado
    time_t data_solicitacao;            // Data da solicitação (timestamp)
} Solicitacao;

/**
 * Nó da Fila de Espera
 */
typedef struct NoFila {
    Solicitacao dados;          // Dados da solicitação
    struct NoFila* proximo;     // Ponteiro para o próximo nó
} NoFila;

/**
 * Estrutura da Fila (Lista de Espera)
 * Princípio: FIFO (First In, First Out)
 */
typedef struct {
    NoFila* frente;     // Ponteiro para o início da fila
    NoFila* tras;       // Ponteiro para o final da fila
    int total;          // Total de solicitações na fila
} FilaEspera;

// =============================================================================
// ESTRUTURA 3: PILHA (HISTÓRICO DE OPERAÇÕES)
// =============================================================================

/**
 * Estrutura que representa uma OPERAÇÃO no histórico
 */
typedef struct {
    char tipo_operacao[20];             // "EMPRESTIMO" ou "DEVOLUCAO"
    char titulo_livro[MAX_TITULO];      // Título do livro
    char nome_leitor[MAX_NOME_LEITOR];  // Nome do leitor
    time_t data_operacao;               // Data/hora da operação (timestamp)
} Operacao;

/**
 * Nó da Pilha de Histórico
 */
typedef struct NoPilha {
    Operacao dados;             // Dados da operação
    struct NoPilha* proximo;    // Ponteiro para o próximo nó (abaixo)
} NoPilha;

/**
 * Estrutura da Pilha (Histórico)
 * Princípio: LIFO (Last In, First Out)
 */
typedef struct {
    NoPilha* topo;      // Ponteiro para o topo da pilha
    int total;          // Total de operações registradas
} PilhaHistorico;

// =============================================================================
// ESTRUTURA PRINCIPAL: SISTEMA DA BIBLIOTECA
// =============================================================================

/**
 * Estrutura que integra todas as estruturas de dados
 */
typedef struct {
    ListaLivros* catalogo;      // Catálogo de livros (lista encadeada)
    FilaEspera* fila_espera;    // Fila de espera (fila)
    PilhaHistorico* historico;  // Histórico de operações (pilha)
} Biblioteca;

// =============================================================================
// FUNÇÕES DE INICIALIZAÇÃO E LIBERAÇÃO DE MEMÓRIA
// =============================================================================

/**
 * Inicializa o sistema completo da biblioteca
 * Retorna: Ponteiro para a estrutura Biblioteca criada
 */
Biblioteca* inicializar_biblioteca();

/**
 * Libera toda a memória alocada pelo sistema
 * Parâmetros:
 *   - bib: Ponteiro para a estrutura Biblioteca
 */
void liberar_biblioteca(Biblioteca* bib);

// =============================================================================
// FUNÇÕES DA LISTA ENCADEADA (CATÁLOGO DE LIVROS)
// =============================================================================

/**
 * Cria uma nova lista de livros vazia
 * Retorna: Ponteiro para a lista criada
 */
ListaLivros* criar_lista_livros();

/**
 * Adiciona um novo livro ao final do catálogo
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 *   - livro: Estrutura Livro com os dados a serem inseridos
 * Retorna: true se adicionado com sucesso, false caso contrário
 */
bool adicionar_livro(ListaLivros* lista, Livro livro);

/**
 * Busca um livro pelo título (comparação case-insensitive)
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 *   - titulo: String com o título a ser buscado
 * Retorna: Ponteiro para o nó do livro encontrado, ou NULL se não encontrado
 */
NoLivro* buscar_por_titulo(ListaLivros* lista, const char* titulo);

/**
 * Busca livros por autor (comparação parcial e case-insensitive)
 * Exibe todos os livros do autor encontrado
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 *   - autor: String com o nome do autor a ser buscado
 * Retorna: Número de livros encontrados
 */
int buscar_por_autor(ListaLivros* lista, const char* autor);

/**
 * Remove um livro do catálogo pelo título
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 *   - titulo: String com o título do livro a ser removido
 * Retorna: true se removido com sucesso, false caso contrário
 */
bool remover_livro(ListaLivros* lista, const char* titulo);

/**
 * Lista todos os livros do catálogo
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 */
void listar_todos_livros(ListaLivros* lista);

/**
 * Lista apenas os livros com status "disponível"
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 */
void listar_livros_disponiveis(ListaLivros* lista);

/**
 * Lista apenas os livros com status "emprestado"
 * Parâmetros:
 *   - lista: Ponteiro para a lista de livros
 */
void listar_livros_emprestados(ListaLivros* lista);

/**
 * Libera toda a memória da lista de livros
 * Parâmetros:
 *   - lista: Ponteiro para a lista a ser liberada
 */
void liberar_lista_livros(ListaLivros* lista);

// =============================================================================
// FUNÇÕES DA FILA (LISTA DE ESPERA)
// =============================================================================

/**
 * Cria uma nova fila de espera vazia
 * Retorna: Ponteiro para a fila criada
 */
FilaEspera* criar_fila_espera();

/**
 * Adiciona um leitor à fila de espera para um livro
 * Parâmetros:
 *   - fila: Ponteiro para a fila de espera
 *   - nome_leitor: Nome do leitor que está solicitando
 *   - titulo_livro: Título do livro desejado
 * Retorna: true se enfileirado com sucesso, false caso contrário
 */
bool enfileirar(FilaEspera* fila, const char* nome_leitor, const char* titulo_livro);

/**
 * Remove o próximo leitor da fila que aguarda por um livro específico
 * Parâmetros:
 *   - fila: Ponteiro para a fila de espera
 *   - titulo_livro: Título do livro
 *   - nome_leitor_saida: Buffer para armazenar o nome do leitor removido
 * Retorna: true se desenfileirado com sucesso, false se não há ninguém na fila
 */
bool desenfileirar_especifico(FilaEspera* fila, const char* titulo_livro, char* nome_leitor_saida);

/**
 * Retorna a posição de um leitor na fila para um dado livro
 * Parâmetros:
 *   - fila: Ponteiro para a fila de espera
 *   - nome_leitor: Nome do leitor
 *   - titulo_livro: Título do livro
 * Retorna: Posição na fila (1 = primeiro), ou 0 se não encontrado
 */
int consultar_posicao(FilaEspera* fila, const char* nome_leitor, const char* titulo_livro);

/**
 * Lista todos os leitores na fila para um livro específico
 * Parâmetros:
 *   - fila: Ponteiro para a fila de espera
 *   - titulo_livro: Título do livro
 * Retorna: Número de leitores na fila para este livro
 */
int listar_fila_livro(FilaEspera* fila, const char* titulo_livro);

/**
 * Lista todas as solicitações em todas as filas de espera
 * Parâmetros:
 *   - fila: Ponteiro para a fila de espera
 */
void listar_todas_filas(FilaEspera* fila);

/**
 * Libera toda a memória da fila de espera
 * Parâmetros:
 *   - fila: Ponteiro para a fila a ser liberada
 */
void liberar_fila_espera(FilaEspera* fila);

// =============================================================================
// FUNÇÕES DA PILHA (HISTÓRICO DE OPERAÇÕES)
// =============================================================================

/**
 * Cria uma nova pilha de histórico vazia
 * Retorna: Ponteiro para a pilha criada
 */
PilhaHistorico* criar_pilha_historico();

/**
 * Adiciona uma nova operação ao topo da pilha
 * Parâmetros:
 *   - pilha: Ponteiro para a pilha de histórico
 *   - tipo_operacao: "EMPRESTIMO" ou "DEVOLUCAO"
 *   - titulo_livro: Título do livro
 *   - nome_leitor: Nome do leitor
 * Retorna: true se empilhado com sucesso, false caso contrário
 */
bool empilhar(PilhaHistorico* pilha, const char* tipo_operacao,
              const char* titulo_livro, const char* nome_leitor);

/**
 * Exibe as operações mais recentes do histórico
 * Parâmetros:
 *   - pilha: Ponteiro para a pilha de histórico
 *   - limite: Número máximo de operações a exibir (0 = todas)
 */
void exibir_historico(PilhaHistorico* pilha, int limite);

/**
 * Exibe todo o histórico de operações para um livro específico
 * Parâmetros:
 *   - pilha: Ponteiro para a pilha de histórico
 *   - titulo_livro: Título do livro
 * Retorna: Número de operações encontradas para este livro
 */
int historico_livro(PilhaHistorico* pilha, const char* titulo_livro);

/**
 * Libera toda a memória da pilha de histórico
 * Parâmetros:
 *   - pilha: Ponteiro para a pilha a ser liberada
 */
void liberar_pilha_historico(PilhaHistorico* pilha);

// =============================================================================
// FUNÇÕES DE ALTO NÍVEL (LÓGICA DO SISTEMA)
// =============================================================================

/**
 * Realiza o empréstimo de um livro
 * Parâmetros:
 *   - bib: Ponteiro para a estrutura Biblioteca
 *   - titulo: Título do livro a ser emprestado
 *   - nome_leitor: Nome do leitor
 * Retorna:
 *   0 = Empréstimo realizado com sucesso
 *   1 = Livro não encontrado
 *   2 = Livro já emprestado (leitor adicionado à fila)
 */
int emprestar_livro(Biblioteca* bib, const char* titulo, const char* nome_leitor);

/**
 * Realiza a devolução de um livro
 * Parâmetros:
 *   - bib: Ponteiro para a estrutura Biblioteca
 *   - titulo: Título do livro a ser devolvido
 * Retorna:
 *   0 = Devolução realizada com sucesso
 *   1 = Livro não encontrado
 *   2 = Livro já está disponível (não estava emprestado)
 */
int devolver_livro(Biblioteca* bib, const char* titulo);

/**
 * Exibe um relatório completo do sistema
 * Parâmetros:
 *   - bib: Ponteiro para a estrutura Biblioteca
 */
void relatorio_sistema(Biblioteca* bib);

// =============================================================================
// FUNÇÕES AUXILIARES
// =============================================================================

/**
 * Converte uma string para minúsculas (case-insensitive)
 * Parâmetros:
 *   - dest: Buffer de destino
 *   - src: String original
 */
void para_minusculo(char* dest, const char* src);

/**
 * Formata um timestamp para string legível
 * Parâmetros:
 *   - timestamp: Valor time_t
 *   - buffer: Buffer para armazenar a string formatada
 *   - tamanho: Tamanho do buffer
 */
void formatar_data(time_t timestamp, char* buffer, size_t tamanho);

/**
 * Limpa o buffer de entrada (stdin)
 */
void limpar_buffer();

#endif // BIBLIOTECA_H