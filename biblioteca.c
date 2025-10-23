/**
 * =============================================================================
 * SISTEMA DE GERENCIAMENTO DE BIBLIOTECA
 * =============================================================================
 * Arquivo: biblioteca.c
 * Descrição: Implementação de todas as funções das estruturas de dados
 * Autores: [INSIRA NOMES DA EQUIPE]
 * Data: Outubro 2025
 * =============================================================================
 */

#include "biblioteca.h"

// =============================================================================
// FUNÇÕES AUXILIARES
// =============================================================================

/**
 * Converte uma string para minúsculas (para comparações case-insensitive)
 */
void para_minusculo(char* dest, const char* src) {
    int i = 0;
    while (src[i]) {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

/**
 * Formata um timestamp para string legível (DD/MM/YYYY HH:MM:SS)
 */
void formatar_data(time_t timestamp, char* buffer, size_t tamanho) {
    struct tm* info = localtime(&timestamp);
    strftime(buffer, tamanho, "%d/%m/%Y %H:%M:%S", info);
}

/**
 * Limpa o buffer de entrada (stdin) para evitar lixo
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// =============================================================================
// INICIALIZAÇÃO E LIBERAÇÃO DO SISTEMA COMPLETO
// =============================================================================

/**
 * Inicializa todas as estruturas da biblioteca
 */
Biblioteca* inicializar_biblioteca() {
    // Aloca memória para a estrutura principal
    Biblioteca* bib = (Biblioteca*)malloc(sizeof(Biblioteca));

    if (bib == NULL) {
        printf("Erro: Falha ao alocar memória para a biblioteca!\n");
        return NULL;
    }

    // Inicializa cada estrutura de dados
    bib->catalogo = criar_lista_livros();
    bib->fila_espera = criar_fila_espera();
    bib->historico = criar_pilha_historico();

    // Verifica se todas as estruturas foram criadas com sucesso
    if (bib->catalogo == NULL || bib->fila_espera == NULL || bib->historico == NULL) {
        printf("Erro: Falha ao inicializar as estruturas de dados!\n");
        liberar_biblioteca(bib);
        return NULL;
    }

    return bib;
}

/**
 * Libera toda a memória alocada pelo sistema
 */
void liberar_biblioteca(Biblioteca* bib) {
    if (bib == NULL) return;

    // Libera cada estrutura de dados
    if (bib->catalogo != NULL) {
        liberar_lista_livros(bib->catalogo);
    }

    if (bib->fila_espera != NULL) {
        liberar_fila_espera(bib->fila_espera);
    }

    if (bib->historico != NULL) {
        liberar_pilha_historico(bib->historico);
    }

    // Libera a estrutura principal
    free(bib);

    printf("\n[SISTEMA] Memória liberada com sucesso!\n");
}

// =============================================================================
// FUNÇÕES DA LISTA ENCADEADA (CATÁLOGO DE LIVROS)
// =============================================================================

/**
 * Cria uma nova lista de livros vazia
 */
ListaLivros* criar_lista_livros() {
    ListaLivros* lista = (ListaLivros*)malloc(sizeof(ListaLivros));

    if (lista == NULL) {
        return NULL;
    }

    lista->cabeca = NULL;
    lista->total = 0;

    return lista;
}

/**
 * Adiciona um novo livro ao final do catálogo
 */
bool adicionar_livro(ListaLivros* lista, Livro livro) {
    // Verifica se a lista é válida
    if (lista == NULL) {
        return false;
    }

    // Verifica se já existe um livro com o mesmo título
    if (buscar_por_titulo(lista, livro.titulo) != NULL) {
        printf("Erro: Já existe um livro com este título no catálogo!\n");
        return false;
    }

    // Cria um novo nó
    NoLivro* novo = (NoLivro*)malloc(sizeof(NoLivro));
    if (novo == NULL) {
        printf("Erro: Falha ao alocar memória para o livro!\n");
        return false;
    }

    // Copia os dados do livro
    novo->dados = livro;
    novo->proximo = NULL;

    // Insere no final da lista
    if (lista->cabeca == NULL) {
        // Lista vazia - primeiro elemento
        lista->cabeca = novo;
    } else {
        // Percorre até o último nó
        NoLivro* atual = lista->cabeca;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }

    lista->total++;
    return true;
}

/**
 * Busca um livro pelo título (case-insensitive)
 */
NoLivro* buscar_por_titulo(ListaLivros* lista, const char* titulo) {
    if (lista == NULL || titulo == NULL) {
        return NULL;
    }

    // Converte o título buscado para minúsculas
    char titulo_busca[MAX_TITULO];
    para_minusculo(titulo_busca, titulo);

    // Percorre a lista
    NoLivro* atual = lista->cabeca;
    while (atual != NULL) {
        // Converte o título do livro atual para minúsculas
        char titulo_atual[MAX_TITULO];
        para_minusculo(titulo_atual, atual->dados.titulo);

        // Compara os títulos
        if (strcmp(titulo_atual, titulo_busca) == 0) {
            return atual;
        }

        atual = atual->proximo;
    }

    return NULL; // Não encontrado
}

/**
 * Busca livros por autor (comparação parcial e case-insensitive)
 */
int buscar_por_autor(ListaLivros* lista, const char* autor) {
    if (lista == NULL || autor == NULL) {
        return 0;
    }

    // Converte o autor buscado para minúsculas
    char autor_busca[MAX_AUTOR];
    para_minusculo(autor_busca, autor);

    int encontrados = 0;
    NoLivro* atual = lista->cabeca;

    printf("\n=== LIVROS DO AUTOR '%s' ===\n", autor);

    // Percorre a lista
    while (atual != NULL) {
        // Converte o autor do livro atual para minúsculas
        char autor_atual[MAX_AUTOR];
        para_minusculo(autor_atual, atual->dados.autor);

        // Verifica se o autor buscado está contido no autor do livro
        if (strstr(autor_atual, autor_busca) != NULL) {
            encontrados++;

            printf("\n[%d] Título: %s\n", encontrados, atual->dados.titulo);
            printf("    Autor: %s\n", atual->dados.autor);
            printf("    Ano: %d\n", atual->dados.ano_publicacao);
            printf("    ISBN: %s\n", strlen(atual->dados.isbn) > 0 ? atual->dados.isbn : "N/A");
            printf("    Status: %s\n", atual->dados.status ? "Disponível" : "Emprestado");

            if (!atual->dados.status) {
                char data_str[30];
                formatar_data(atual->dados.data_emprestimo, data_str, sizeof(data_str));
                printf("    Emprestado para: %s (em %s)\n",
                       atual->dados.nome_leitor_atual, data_str);
            }
        }

        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Nenhum livro encontrado para o autor '%s'.\n", autor);
    } else {
        printf("\nTotal de livros encontrados: %d\n", encontrados);
    }

    return encontrados;
}

/**
 * Remove um livro do catálogo pelo título
 */
bool remover_livro(ListaLivros* lista, const char* titulo) {
    if (lista == NULL || titulo == NULL || lista->cabeca == NULL) {
        return false;
    }

    // Converte o título para minúsculas
    char titulo_busca[MAX_TITULO];
    para_minusculo(titulo_busca, titulo);

    NoLivro* atual = lista->cabeca;
    NoLivro* anterior = NULL;

    // Procura o livro na lista
    while (atual != NULL) {
        char titulo_atual[MAX_TITULO];
        para_minusculo(titulo_atual, atual->dados.titulo);

        if (strcmp(titulo_atual, titulo_busca) == 0) {
            // Livro encontrado - remove
            if (anterior == NULL) {
                // Remove o primeiro nó
                lista->cabeca = atual->proximo;
            } else {
                // Remove um nó do meio ou fim
                anterior->proximo = atual->proximo;
            }

            free(atual);
            lista->total--;
            return true;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    return false; // Não encontrado
}

/**
 * Lista todos os livros do catálogo
 */
void listar_todos_livros(ListaLivros* lista) {
    if (lista == NULL || lista->cabeca == NULL) {
        printf("\nO catálogo está vazio!\n");
        return;
    }

    printf("\n=== CATÁLOGO COMPLETO ===\n");
    printf("Total de livros: %d\n", lista->total);

    NoLivro* atual = lista->cabeca;
    int contador = 1;

    while (atual != NULL) {
        printf("\n[%d] Título: %s\n", contador, atual->dados.titulo);
        printf("    Autor: %s\n", atual->dados.autor);
        printf("    Ano: %d\n", atual->dados.ano_publicacao);
        printf("    ISBN: %s\n", strlen(atual->dados.isbn) > 0 ? atual->dados.isbn : "N/A");
        printf("    Status: %s\n", atual->dados.status ? "Disponível" : "Emprestado");

        if (!atual->dados.status) {
            char data_str[30];
            formatar_data(atual->dados.data_emprestimo, data_str, sizeof(data_str));
            printf("    Emprestado para: %s (em %s)\n",
                   atual->dados.nome_leitor_atual, data_str);
        }

        atual = atual->proximo;
        contador++;
    }
}

/**
 * Lista apenas os livros com status "disponível"
 */
void listar_livros_disponiveis(ListaLivros* lista) {
    if (lista == NULL || lista->cabeca == NULL) {
        printf("\nO catálogo está vazio!\n");
        return;
    }

    printf("\n=== LIVROS DISPONÍVEIS ===\n");

    NoLivro* atual = lista->cabeca;
    int contador = 0;

    while (atual != NULL) {
        if (atual->dados.status) { // Se disponível
            contador++;
            printf("\n[%d] Título: %s\n", contador, atual->dados.titulo);
            printf("    Autor: %s\n", atual->dados.autor);
            printf("    Ano: %d\n", atual->dados.ano_publicacao);
            printf("    ISBN: %s\n", strlen(atual->dados.isbn) > 0 ? atual->dados.isbn : "N/A");
        }

        atual = atual->proximo;
    }

    if (contador == 0) {
        printf("Não há livros disponíveis no momento.\n");
    } else {
        printf("\nTotal de livros disponíveis: %d\n", contador);
    }
}

/**
 * Lista apenas os livros com status "emprestado"
 */
void listar_livros_emprestados(ListaLivros* lista) {
    if (lista == NULL || lista->cabeca == NULL) {
        printf("\nO catálogo está vazio!\n");
        return;
    }

    printf("\n=== LIVROS EMPRESTADOS ===\n");

    NoLivro* atual = lista->cabeca;
    int contador = 0;

    while (atual != NULL) {
        if (!atual->dados.status) { // Se emprestado
            contador++;
            char data_str[30];
            formatar_data(atual->dados.data_emprestimo, data_str, sizeof(data_str));

            printf("\n[%d] Título: %s\n", contador, atual->dados.titulo);
            printf("    Autor: %s\n", atual->dados.autor);
            printf("    Emprestado para: %s\n", atual->dados.nome_leitor_atual);
            printf("    Data do empréstimo: %s\n", data_str);
        }

        atual = atual->proximo;
    }

    if (contador == 0) {
        printf("Não há livros emprestados no momento.\n");
    } else {
        printf("\nTotal de livros emprestados: %d\n", contador);
    }
}

/**
 * Libera toda a memória da lista de livros
 */
void liberar_lista_livros(ListaLivros* lista) {
    if (lista == NULL) return;

    NoLivro* atual = lista->cabeca;
    NoLivro* proximo;

    // Percorre a lista liberando cada nó
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    free(lista);
}

// =============================================================================
// FUNÇÕES DA FILA (LISTA DE ESPERA)
// =============================================================================

/**
 * Cria uma nova fila de espera vazia
 */
FilaEspera* criar_fila_espera() {
    FilaEspera* fila = (FilaEspera*)malloc(sizeof(FilaEspera));

    if (fila == NULL) {
        return NULL;
    }

    fila->frente = NULL;
    fila->tras = NULL;
    fila->total = 0;

    return fila;
}

/**
 * Adiciona um leitor à fila de espera (final da fila - FIFO)
 */
bool enfileirar(FilaEspera* fila, const char* nome_leitor, const char* titulo_livro) {
    if (fila == NULL || nome_leitor == NULL || titulo_livro == NULL) {
        return false;
    }

    // Cria um novo nó
    NoFila* novo = (NoFila*)malloc(sizeof(NoFila));
    if (novo == NULL) {
        printf("Erro: Falha ao alocar memória para a solicitação!\n");
        return false;
    }

    // Preenche os dados da solicitação
    strcpy(novo->dados.nome_leitor, nome_leitor);
    strcpy(novo->dados.titulo_livro, titulo_livro);
    novo->dados.data_solicitacao = time(NULL);
    novo->proximo = NULL;

    // Insere no final da fila
    if (fila->tras == NULL) {
        // Fila vazia - primeiro elemento
        fila->frente = novo;
        fila->tras = novo;
    } else {
        // Adiciona no final
        fila->tras->proximo = novo;
        fila->tras = novo;
    }

    fila->total++;
    return true;
}

/**
 * Remove o próximo leitor da fila para um livro específico
 */
bool desenfileirar_especifico(FilaEspera* fila, const char* titulo_livro, char* nome_leitor_saida) {
    if (fila == NULL || titulo_livro == NULL || fila->frente == NULL) {
        return false;
    }

    // Converte o título para minúsculas
    char titulo_busca[MAX_TITULO];
    para_minusculo(titulo_busca, titulo_livro);

    NoFila* atual = fila->frente;
    NoFila* anterior = NULL;

    // Procura o primeiro leitor aguardando este livro
    while (atual != NULL) {
        char titulo_atual[MAX_TITULO];
        para_minusculo(titulo_atual, atual->dados.titulo_livro);

        if (strcmp(titulo_atual, titulo_busca) == 0) {
            // Encontrou - copia o nome do leitor
            strcpy(nome_leitor_saida, atual->dados.nome_leitor);

            // Remove da fila
            if (anterior == NULL) {
                // Remove da frente
                fila->frente = atual->proximo;
                if (fila->frente == NULL) {
                    fila->tras = NULL; // Fila ficou vazia
                }
            } else {
                // Remove do meio
                anterior->proximo = atual->proximo;
                if (atual == fila->tras) {
                    fila->tras = anterior; // Era o último
                }
            }

            free(atual);
            fila->total--;
            return true;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    return false; // Não encontrado
}

/**
 * Retorna a posição de um leitor na fila para um dado livro
 */
int consultar_posicao(FilaEspera* fila, const char* nome_leitor, const char* titulo_livro) {
    if (fila == NULL || nome_leitor == NULL || titulo_livro == NULL) {
        return 0;
    }

    // Converte para minúsculas
    char titulo_busca[MAX_TITULO];
    char nome_busca[MAX_NOME_LEITOR];
    para_minusculo(titulo_busca, titulo_livro);
    para_minusculo(nome_busca, nome_leitor);

    NoFila* atual = fila->frente;
    int posicao = 0;

    while (atual != NULL) {
        char titulo_atual[MAX_TITULO];
        char nome_atual[MAX_NOME_LEITOR];
        para_minusculo(titulo_atual, atual->dados.titulo_livro);
        para_minusculo(nome_atual, atual->dados.nome_leitor);

        // Conta apenas para este livro específico
        if (strcmp(titulo_atual, titulo_busca) == 0) {
            posicao++;

            // Verifica se é o leitor procurado
            if (strcmp(nome_atual, nome_busca) == 0) {
                return posicao;
            }
        }

        atual = atual->proximo;
    }

    return 0; // Não encontrado
}

/**
 * Lista todos os leitores na fila para um livro específico
 */
int listar_fila_livro(FilaEspera* fila, const char* titulo_livro) {
    if (fila == NULL || titulo_livro == NULL || fila->frente == NULL) {
        printf("\nA fila de espera está vazia!\n");
        return 0;
    }

    // Converte o título para minúsculas
    char titulo_busca[MAX_TITULO];
    para_minusculo(titulo_busca, titulo_livro);

    printf("\n=== FILA DE ESPERA PARA: %s ===\n", titulo_livro);

    NoFila* atual = fila->frente;
    int posicao = 0;

    while (atual != NULL) {
        char titulo_atual[MAX_TITULO];
        para_minusculo(titulo_atual, atual->dados.titulo_livro);

        if (strcmp(titulo_atual, titulo_busca) == 0) {
            posicao++;
            char data_str[30];
            formatar_data(atual->dados.data_solicitacao, data_str, sizeof(data_str));

            printf("[Posição %d] %s - Solicitado em: %s\n",
                   posicao, atual->dados.nome_leitor, data_str);
        }

        atual = atual->proximo;
    }

    if (posicao == 0) {
        printf("Não há leitores aguardando por este livro.\n");
    } else {
        printf("\nTotal de leitores na fila: %d\n", posicao);
    }

    return posicao;
}

/**
 * Lista todas as solicitações em todas as filas de espera
 */
void listar_todas_filas(FilaEspera* fila) {
    if (fila == NULL || fila->frente == NULL) {
        printf("\nNão há solicitações na fila de espera!\n");
        return;
    }

    printf("\n=== TODAS AS SOLICITAÇÕES EM ESPERA ===\n");
    printf("Total de solicitações: %d\n", fila->total);

    NoFila* atual = fila->frente;
    int contador = 1;

    while (atual != NULL) {
        char data_str[30];
        formatar_data(atual->dados.data_solicitacao, data_str, sizeof(data_str));

        printf("\n[%d] Leitor: %s\n", contador, atual->dados.nome_leitor);
        printf("    Livro: %s\n", atual->dados.titulo_livro);
        printf("    Solicitado em: %s\n", data_str);

        atual = atual->proximo;
        contador++;
    }
}

/**
 * Libera toda a memória da fila de espera
 */
void liberar_fila_espera(FilaEspera* fila) {
    if (fila == NULL) return;

    NoFila* atual = fila->frente;
    NoFila* proximo;

    // Percorre a fila liberando cada nó
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    free(fila);
}

// =============================================================================
// FUNÇÕES DA PILHA (HISTÓRICO DE OPERAÇÕES)
// =============================================================================

/**
 * Cria uma nova pilha de histórico vazia
 */
PilhaHistorico* criar_pilha_historico() {
    PilhaHistorico* pilha = (PilhaHistorico*)malloc(sizeof(PilhaHistorico));

    if (pilha == NULL) {
        return NULL;
    }

    pilha->topo = NULL;
    pilha->total = 0;

    return pilha;
}

/**
 * Adiciona uma nova operação ao topo da pilha (LIFO)
 */
bool empilhar(PilhaHistorico* pilha, const char* tipo_operacao,
              const char* titulo_livro, const char* nome_leitor) {
    if (pilha == NULL || tipo_operacao == NULL || titulo_livro == NULL || nome_leitor == NULL) {
        return false;
    }

    // Cria um novo nó
    NoPilha* novo = (NoPilha*)malloc(sizeof(NoPilha));
    if (novo == NULL) {
        printf("Erro: Falha ao alocar memória para o histórico!\n");
        return false;
    }

    // Preenche os dados da operação
    strcpy(novo->dados.tipo_operacao, tipo_operacao);
    strcpy(novo->dados.titulo_livro, titulo_livro);
    strcpy(novo->dados.nome_leitor, nome_leitor);
    novo->dados.data_operacao = time(NULL);

    // Insere no topo da pilha
    novo->proximo = pilha->topo;
    pilha->topo = novo;

    pilha->total++;
    return true;
}

/**
 * Exibe as operações mais recentes do histórico
 */
void exibir_historico(PilhaHistorico* pilha, int limite) {
    if (pilha == NULL || pilha->topo == NULL) {
        printf("\nO histórico está vazio!\n");
        return;
    }

    printf("\n=== HISTÓRICO DE OPERAÇÕES ===\n");

    if (limite == 0 || limite > pilha->total) {
        limite = pilha->total;
        printf("Total de operações: %d\n", pilha->total);
    } else {
        printf("Exibindo as %d operações mais recentes (total: %d)\n", limite, pilha->total);
    }

    NoPilha* atual = pilha->topo;
    int contador = 1;

    while (atual != NULL && contador <= limite) {
        char data_str[30];
        formatar_data(atual->dados.data_operacao, data_str, sizeof(data_str));

        printf("\n[%d] Operação: %s\n", contador, atual->dados.tipo_operacao);
        printf("    Livro: %s\n", atual->dados.titulo_livro);
        printf("    Leitor: %s\n", atual->dados.nome_leitor);
        printf("    Data/Hora: %s\n", data_str);

        atual = atual->proximo;
        contador++;
    }
}

/**
 * Exibe todo o histórico de operações para um livro específico
 */
int historico_livro(PilhaHistorico* pilha, const char* titulo_livro) {
    if (pilha == NULL || titulo_livro == NULL || pilha->topo == NULL) {
        printf("\nO histórico está vazio!\n");
        return 0;
    }

    // Converte o título para minúsculas
    char titulo_busca[MAX_TITULO];
    para_minusculo(titulo_busca, titulo_livro);

    printf("\n=== HISTÓRICO DO LIVRO: %s ===\n", titulo_livro);

    NoPilha* atual = pilha->topo;
    int encontrados = 0;

    while (atual != NULL) {
        char titulo_atual[MAX_TITULO];
        para_minusculo(titulo_atual, atual->dados.titulo_livro);

        if (strcmp(titulo_atual, titulo_busca) == 0) {
            encontrados++;
            char data_str[30];
            formatar_data(atual->dados.data_operacao, data_str, sizeof(data_str));

            printf("\n[%d] Operação: %s\n", encontrados, atual->dados.tipo_operacao);
            printf("    Leitor: %s\n", atual->dados.nome_leitor);
            printf("    Data/Hora: %s\n", data_str);
        }

        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Não há operações registradas para este livro.\n");
    } else {
        printf("\nTotal de operações encontradas: %d\n", encontrados);
    }

    return encontrados;
}

/**
 * Libera toda a memória da pilha de histórico
 */
void liberar_pilha_historico(PilhaHistorico* pilha) {
    if (pilha == NULL) return;

    NoPilha* atual = pilha->topo;
    NoPilha* proximo;

    // Percorre a pilha liberando cada nó
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    free(pilha);
}

// =============================================================================
// FUNÇÕES DE ALTO NÍVEL (LÓGICA DO SISTEMA)
// =============================================================================

/**
 * Realiza o empréstimo de um livro
 */
int emprestar_livro(Biblioteca* bib, const char* titulo, const char* nome_leitor) {
    if (bib == NULL || titulo == NULL || nome_leitor == NULL) {
        return 1;
    }

    // Busca o livro no catálogo
    NoLivro* no_livro = buscar_por_titulo(bib->catalogo, titulo);

    if (no_livro == NULL) {
        printf("\nErro: Livro '%s' não encontrado no catálogo!\n", titulo);
        return 1; // Livro não encontrado
    }

    // Verifica se o livro está disponível
    if (no_livro->dados.status) {
        // Livro disponível - realiza o empréstimo
        no_livro->dados.status = false;
        strcpy(no_livro->dados.nome_leitor_atual, nome_leitor);
        no_livro->dados.data_emprestimo = time(NULL);

        // Registra no histórico
        empilhar(bib->historico, "EMPRESTIMO", titulo, nome_leitor);

        char data_str[30];
        formatar_data(no_livro->dados.data_emprestimo, data_str, sizeof(data_str));

        printf("\n✓ Empréstimo realizado com sucesso!\n");
        printf("  Livro: %s\n", no_livro->dados.titulo);
        printf("  Leitor: %s\n", nome_leitor);
        printf("  Data: %s\n", data_str);

        return 0; // Sucesso
    } else {
        // Livro já emprestado - adiciona à fila de espera
        enfileirar(bib->fila_espera, nome_leitor, titulo);

        int posicao = consultar_posicao(bib->fila_espera, nome_leitor, titulo);

        printf("\n⚠ Livro '%s' já está emprestado!\n", titulo);
        printf("  Emprestado para: %s\n", no_livro->dados.nome_leitor_atual);
        printf("  Você foi adicionado à fila de espera.\n");
        printf("  Sua posição na fila: %d\n", posicao);

        return 2; // Livro já emprestado
    }
}

/**
 * Realiza a devolução de um livro
 */
int devolver_livro(Biblioteca* bib, const char* titulo) {
    if (bib == NULL || titulo == NULL) {
        return 1;
    }

    // Busca o livro no catálogo
    NoLivro* no_livro = buscar_por_titulo(bib->catalogo, titulo);

    if (no_livro == NULL) {
        printf("\nErro: Livro '%s' não encontrado no catálogo!\n", titulo);
        return 1; // Livro não encontrado
    }

    // Verifica se o livro está emprestado
    if (!no_livro->dados.status) {
        // Salva o nome do leitor antes de limpar
        char leitor_anterior[MAX_NOME_LEITOR];
        strcpy(leitor_anterior, no_livro->dados.nome_leitor_atual);

        // Marca como disponível
        no_livro->dados.status = true;
        strcpy(no_livro->dados.nome_leitor_atual, "");
        no_livro->dados.data_emprestimo = 0;

        // Registra no histórico
        empilhar(bib->historico, "DEVOLUCAO", titulo, leitor_anterior);

        printf("\n✓ Devolução realizada com sucesso!\n");
        printf("  Livro: %s\n", no_livro->dados.titulo);
        printf("  Devolvido por: %s\n", leitor_anterior);

        // Verifica se há alguém na fila de espera
        char proximo_leitor[MAX_NOME_LEITOR];
        if (desenfileirar_especifico(bib->fila_espera, titulo, proximo_leitor)) {
            printf("\n📢 NOTIFICAÇÃO:\n");
            printf("  O leitor '%s' estava aguardando este livro.\n", proximo_leitor);
            printf("  Por favor, notifique-o que o livro está disponível!\n");
        }

        return 0; // Sucesso
    } else {
        printf("\nErro: O livro '%s' já está disponível (não estava emprestado)!\n", titulo);
        return 2; // Livro já disponível
    }
}

/**
 * Exibe um relatório completo do sistema
 */
void relatorio_sistema(Biblioteca* bib) {
    if (bib == NULL) {
        printf("\nErro: Sistema não inicializado!\n");
        return;
    }

    // Conta livros disponíveis e emprestados
    int disponiveis = 0;
    int emprestados = 0;

    NoLivro* atual = bib->catalogo->cabeca;
    while (atual != NULL) {
        if (atual->dados.status) {
            disponiveis++;
        } else {
            emprestados++;
        }
        atual = atual->proximo;
    }

    // Exibe o relatório
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║       RELATÓRIO DO SISTEMA DE BIBLIOTECA              ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║ Total de livros no catálogo:        %-5d            ║\n", bib->catalogo->total);
    printf("║ Livros disponíveis:                 %-5d            ║\n", disponiveis);
    printf("║ Livros emprestados:                 %-5d            ║\n", emprestados);
    printf("║ Leitores na fila de espera:         %-5d            ║\n", bib->fila_espera->total);
    printf("║ Operações registradas no histórico: %-5d            ║\n", bib->historico->total);
    printf("╚════════════════════════════════════════════════════════╝\n");
}