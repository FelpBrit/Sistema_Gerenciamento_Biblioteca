# =============================================================================
# CMakeLists.txt - Configuração do projeto para CLion
# Sistema de Gerenciamento de Biblioteca em C
# =============================================================================

cmake_minimum_required(VERSION 3.10)
project(Sistema_Gerenciamento_Biblioteca C)

# Define o padrão C99 (compatível com time_t, bool, etc.)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Flags de compilação
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Lista de arquivos fonte (.c)
set(SOURCE_FILES
        main.c
        biblioteca.c
)

# Cria o executável
add_executable(biblioteca ${SOURCE_FILES})

# Mensagem de compilação bem-sucedida
message(STATUS "Configuração do projeto concluída!")
message(STATUS "Arquivos fonte: ${SOURCE_FILES}")
