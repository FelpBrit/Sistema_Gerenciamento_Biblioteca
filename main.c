#include "biblioteca.h"
#include <locale.h>

// =============================================================================
// PROTÓTIPOS DAS FUNÇÕES DO MENU
// ============================================================================

void exibir_menu_principal();
void menu_cadastrar_livro(Biblioteca* bib);
void menu_emprestar_livro(Biblioteca* bib);
void menu_devolver_livro(Biblioteca* bib);
void menu_consultar_livros(Biblioteca* bib);
void menu_fila_espera(Biblioteca* bib);
void menu_historico(Biblioteca* bib);
void menu_remover_livro(Biblioteca* bib);
void pausar();

// =============================================================================
// FUNÇÃO PRINCIPAL
// =============================================================================

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(65001);

    // Inicializa o sistema
    printf("    SISTEMA DE GERENCIAMENTO DE BIBLIOTECA EM C         \n");
    printf("    Estruturas de Dados: Lista, Fila e Pilha            \n");
    printf("Inicializando sistema...\n");

Biblioteca* biblioteca = inicializar_biblioteca();
    if (biblioteca == NULL) {
        printf("Erro fatal: Não foi possível inicializar o sistema!\n");
        return 1;
    }

    printf("Sistema inicializado com sucesso!\n");
    pausar();

    // Loop principal do menu
    int opcao;

    do {
        exibir_menu_principal();
        printf("Digite sua opção: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("\nEntrada inválida! Digite um número.\n");
            pausar();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                menu_cadastrar_livro(biblioteca);
                break;

            case 2:
                menu_emprestar_livro(biblioteca);
                break;

            case 3:
                menu_devolver_livro(biblioteca);
                break;

            case 4:
                menu_consultar_livros(biblioteca);
                break;

            case 5:
                menu_fila_espera(biblioteca);
                break;

            case 6:
                menu_historico(biblioteca);
                break;

            case 7:
                relatorio_sistema(biblioteca);
                pausar();
                break;

            case 8:
                menu_remover_livro(biblioteca);
                break;

            case 9:
                printf("\nEncerrando sistema...\n");
                liberar_biblioteca(biblioteca);
                printf("Até logo!\n");
                break;

            default:
                printf("\nOpção inválida! Escolha entre 1 e 9.\n");
                pausar();
        }

    } while (opcao != 9);

    return 0;
}

// =============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DO MENU
// =============================================================================

/**
 * Exibe o menu principal
 */
void exibir_menu_principal(){
    printf("\n");
    printf("            MENU PRINCIPAL\n");
    printf("    1. Cadastrar Novo Livro                               \n");
    printf("    2. Emprestar Livro                                    \n");
    printf("    3. Devolver Livro                                     \n");
    printf("    4. Consultar Livros                                   \n");
    printf("    5. Ver Fila de Espera                                 \n");
    printf("    6. Ver Histórico                                      \n");
    printf("    7. Relatório do Sistema                               \n");
    printf("    8. Remover Livro                                      \n");
    printf("    9. Sair                                               \n");

}

/**
 * Menu: Cadastrar novo livro
 */
void menu_cadastrar_livro(Biblioteca* bib) {
    printf("\n=== CADASTRAR NOVO LIVRO ===\n");

Livro novo_livro;
    // Solicita o título
    printf("Título do livro: ");
    fgets(novo_livro.titulo, MAX_TITULO, stdin);
    novo_livro.titulo[strcspn(novo_livro.titulo, "\n")] = '\0'; // Remove o \n

    if (strlen(novo_livro.titulo) == 0) {
        printf("Erro: O título não pode estar vazio!\n");
        pausar();
        return;
    }

    // Solicita o autor
    printf("Autor do livro: ");
    fgets(novo_livro.autor, MAX_AUTOR, stdin);
    novo_livro.autor[strcspn(novo_livro.autor, "\n")] = '\0';

    if (strlen(novo_livro.autor) == 0) {
        printf("Erro: O autor não pode estar vazio!\n");
        pausar();
        return;
    }

    // Solicita o ano de publicação
    printf("Ano de publicação: ");
    if (scanf("%d", &novo_livro.ano_publicacao) != 1) {
        limpar_buffer();
        printf("Erro: Ano inválido!\n");
        pausar();
        return;
    }
    limpar_buffer();

    if (novo_livro.ano_publicacao < 1000 || novo_livro.ano_publicacao > 2025) {
        printf("Erro: Ano inválido! Digite um ano entre 1000 e 2025.\n");
        pausar();
        return;
    }

    // Solicita o ISBN (opcional)
    printf("ISBN (opcional, pressione ENTER para pular): ");
    fgets(novo_livro.isbn, MAX_ISBN, stdin);
    novo_livro.isbn[strcspn(novo_livro.isbn, "\n")] = '\0';

    // Define status inicial como disponível
    novo_livro.status = true;
    strcpy(novo_livro.nome_leitor_atual, "");
    novo_livro.data_emprestimo = 0;

    // Adiciona o livro ao catálogo
    if (adicionar_livro(bib->catalogo, novo_livro)) {
        printf("\n✓ Livro cadastrado com sucesso!\n");
        printf("  Total de livros no catálogo: %d\n", bib->catalogo->total);
    } else {
        printf("\nErro ao cadastrar o livro!\n");
    }

    pausar();
}

/**
 * Menu: Emprestar livro
 */
void menu_emprestar_livro(Biblioteca* bib) {
    printf("\n=== EMPRESTAR LIVRO ===\n");

    if (bib->catalogo->total == 0) {
        printf("O catálogo está vazio! Cadastre livros primeiro.\n");
        pausar();
        return;
    }

    char titulo[MAX_TITULO];
    char nome_leitor[MAX_NOME_LEITOR];

    // Solicita o título do livro
    printf("Título do livro: ");
    fgets(titulo, MAX_TITULO, stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    if (strlen(titulo) == 0) {
        printf("Erro: O título não pode estar vazio!\n");
        pausar();
        return;
    }

    // Solicita o nome do leitor
    printf("Nome do leitor: ");
    fgets(nome_leitor, MAX_NOME_LEITOR, stdin);
    nome_leitor[strcspn(nome_leitor, "\n")] = '\0';

    if (strlen(nome_leitor) == 0) {
        printf("Erro: O nome do leitor não pode estar vazio!\n");
        pausar();
        return;
    }

    // Realiza o empréstimo
    int resultado = emprestar_livro(bib, titulo, nome_leitor);

    pausar();
}

/**
 * Menu: Devolver livro
 */
void menu_devolver_livro(Biblioteca* bib) {
    printf("\n=== DEVOLVER LIVRO ===\n");

    if (bib->catalogo->total == 0) {
        printf("O catálogo está vazio!\n");
        pausar();
        return;
    }

    char titulo[MAX_TITULO];

    // Solicita o título do livro
    printf("Título do livro a ser devolvido: ");
    fgets(titulo, MAX_TITULO, stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    if (strlen(titulo) == 0) {
        printf("Erro: O título não pode estar vazio!\n");
        pausar();
        return;
    }

    // Realiza a devolução
    int resultado = devolver_livro(bib, titulo);

    pausar();
}

/**
 * Menu: Consultar livros (submenu)
 */
void menu_consultar_livros(Biblioteca* bib) {
    int opcao;

    do {
        printf("\n");

        printf("            CONSULTAR LIVROS - SUBMENU                \n");

        printf("    1. Listar todos os livros                             \n");
        printf("    2. Buscar livro por título                            \n");
        printf("    3. Buscar livros por autor                            \n");
        printf("    4. Listar apenas livros disponíveis                   \n");
        printf("    5. Listar apenas livros emprestados                   \n");
        printf("    6. Voltar ao menu principal                           \n");
        printf("Digite sua opção: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("\nEntrada inválida!\n");
            pausar();
            continue;
        }
        limpar_buffer();

        char busca[MAX_TITULO];
        NoLivro* resultado;

        switch (opcao) {
            case 1:
                listar_todos_livros(bib->catalogo);
                pausar();
                break;

            case 2:
                printf("\nTítulo do livro: ");
                fgets(busca, MAX_TITULO, stdin);
                busca[strcspn(busca, "\n")] = '\0';

                resultado = buscar_por_titulo(bib->catalogo, busca);

                if (resultado != NULL) {
                    printf("\n=== LIVRO ENCONTRADO ===\n");
                    printf("Título: %s\n", resultado->dados.titulo);
                    printf("Autor: %s\n", resultado->dados.autor);
                    printf("Ano: %d\n", resultado->dados.ano_publicacao);
                    printf("ISBN: %s\n", strlen(resultado->dados.isbn) > 0 ? resultado->dados.isbn : "N/A");
                    printf("Status: %s\n", resultado->dados.status ? "Disponível" : "Emprestado");

                    if (!resultado->dados.status) {
                        char data_str[30];
                        formatar_data(resultado->dados.data_emprestimo, data_str, sizeof(data_str));
                        printf("Emprestado para: %s (em %s)\n",
                               resultado->dados.nome_leitor_atual, data_str);
                    }
                } else {
                    printf("\nLivro não encontrado!\n");
                }
                pausar();
                break;

            case 3:
                printf("\nNome do autor (ou parte dele): ");
                fgets(busca, MAX_AUTOR, stdin);
                busca[strcspn(busca, "\n")] = '\0';

                buscar_por_autor(bib->catalogo, busca);
                pausar();
                break;

            case 4:
                listar_livros_disponiveis(bib->catalogo);
                pausar();
                break;

            case 5:
                listar_livros_emprestados(bib->catalogo);
                pausar();
                break;

            case 6:
                // Volta ao menu principal
                break;

            default:
                printf("\nOpção inválida! Escolha entre 1 e 6.\n");
                pausar();
        }

    } while (opcao != 6);
}

/**
 * Menu: Ver fila de espera
 */
void menu_fila_espera(Biblioteca* bib) {
    int opcao;

    do {
        printf("\n");
        printf("           FILA DE ESPERA - SUBMENU                    \n");
        printf("    1. Ver fila de um livro específico                    \n");
        printf("    2. Ver todas as solicitações em espera                \n");
        printf("    3. Consultar minha posição na fila                    \n");
        printf("    4. Voltar ao menu principal                           \n");
        printf("Digite sua opção: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("\nEntrada inválida!\n");
            pausar();
            continue;
        }
        limpar_buffer();

        char titulo[MAX_TITULO];
        char nome[MAX_NOME_LEITOR];
        int posicao;

        switch (opcao) {
            case 1:
                printf("\nTítulo do livro: ");
                fgets(titulo, MAX_TITULO, stdin);
                titulo[strcspn(titulo, "\n")] = '\0';

                listar_fila_livro(bib->fila_espera, titulo);
                pausar();
                break;

            case 2:
                listar_todas_filas(bib->fila_espera);
                pausar();
                break;

            case 3:
                printf("\nSeu nome: ");
                fgets(nome, MAX_NOME_LEITOR, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Título do livro: ");
                fgets(titulo, MAX_TITULO, stdin);
                titulo[strcspn(titulo, "\n")] = '\0';

                posicao = consultar_posicao(bib->fila_espera, nome, titulo);

                if (posicao > 0) {
                    printf("\n✓ Você está na posição %d da fila para o livro '%s'.\n",
                           posicao, titulo);
                } else {
                    printf("\nVocê não está na fila para este livro.\n");
                }
                pausar();
                break;

            case 4:
                // Volta ao menu principal
                break;

            default:
                printf("\nOpção inválida! Escolha entre 1 e 4.\n");
                pausar();
        }

    } while (opcao != 4);
}

/**
 * Menu: Ver histórico
 */
void menu_historico(Biblioteca* bib) {
    int opcao;

    do {
        printf("\n");
        printf("        HISTÓRICO - SUBMENU                      \n");
        printf("    1. Ver histórico geral (todas as operações)           \n");
        printf("    2. Ver histórico geral (últimas N operações)          \n");
        printf("    3. Ver histórico de um livro específico               \n");
        printf("    4. Voltar ao menu principal                           \n");

        printf("Digite sua opção: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("\nEntrada inválida!\n");
            pausar();
            continue;
        }
        limpar_buffer();

        int limite;
        char titulo[MAX_TITULO];

        switch (opcao) {
            case 1:
                exibir_historico(bib->historico, 0); // 0 = todas
                pausar();
                break;

            case 2:
                printf("\nQuantas operações deseja visualizar? ");
                if (scanf("%d", &limite) != 1 || limite < 1) {
                    limpar_buffer();
                    printf("Número inválido!\n");
                    pausar();
                    break;
                }
                limpar_buffer();

                exibir_historico(bib->historico, limite);
                pausar();
                break;

            case 3:
                printf("\nTítulo do livro: ");
                fgets(titulo, MAX_TITULO, stdin);
                titulo[strcspn(titulo, "\n")] = '\0';

                historico_livro(bib->historico, titulo);
                pausar();
                break;

            case 4:
                // Volta ao menu principal
                break;

            default:
                printf("\nOpção inválida! Escolha entre 1 e 4.\n");
                pausar();
        }

    } while (opcao != 4);
}

/**
 * Menu: Remover livro (BÔNUS)
 */
void menu_remover_livro(Biblioteca* bib) {
    printf("\n=== REMOVER LIVRO (FUNCIONALIDADE BÔNUS) ===\n");

    if (bib->catalogo->total == 0) {
        printf("O catálogo está vazio!\n");
        pausar();
        return;
    }

    char titulo[MAX_TITULO];
    char confirmacao;

    // Solicita o título do livro
    printf("Título do livro a ser removido: ");
    fgets(titulo, MAX_TITULO, stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    if (strlen(titulo) == 0) {
        printf("Erro: O título não pode estar vazio!\n");
        pausar();
        return;
    }

// Busca o livro
NoLivro* livro = buscar_por_titulo(bib->catalogo, titulo);

    if (livro == NULL) {
        printf("\nLivro '%s' não encontrado no catálogo!\n", titulo);
        pausar();
        return;
    }

    // Exibe informações do livro
    printf("\n=== LIVRO ENCONTRADO ===\n");
    printf("Título: %s\n", livro->dados.titulo);
    printf("Autor: %s\n", livro->dados.autor);
    printf("Ano: %d\n", livro->dados.ano_publicacao);
    printf("Status: %s\n", livro->dados.status ? "Disponível" : "Emprestado");

    // Pede confirmação
    printf("\nATENÇÃO: Esta operação é IRREVERSÍVEL!\n");
    printf("Tem certeza que deseja remover este livro? (S/N): ");
    scanf(" %c", &confirmacao);
    limpar_buffer();

    if (confirmacao == 'S' || confirmacao == 's') {
        if (remover_livro(bib->catalogo, titulo)) {
            printf("\n✓ Livro removido com sucesso!\n");
            printf("  Total de livros no catálogo: %d\n", bib->catalogo->total);
        } else {
            printf("\nErro ao remover o livro!\n");
        }
    } else {
        printf("\nOperação cancelada.\n");
    }

    pausar();
}

/**
 * Função auxiliar: Pausa a execução até o usuário pressionar ENTER
 */
void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}