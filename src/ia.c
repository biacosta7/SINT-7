#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

#include <unistd.h>
#include <limits.h>


int processar_ia() {
    FILE *pipe;
    char buffer[BUFFER_SIZE];

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("[DEBUG] Diretório atual: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    printf("Executando IA...\n");

    pipe = popen("python3 ia/server.py", "r");
    if (!pipe) {
        fprintf(stderr, "Erro ao executar script da IA.\n");
        return 1;
    }

    // Agora sim: leia a saída ANTES de fechar o pipe
    while (fgets(buffer, BUFFER_SIZE, pipe)) {
        printf("%s", buffer);
    }

    int status = pclose(pipe);
    if (status == -1) {
        fprintf(stderr, "Erro ao fechar o processo da IA.\n");
        
        return 1;
    }

    // DEBUG - mostra arquivos do include
    system("ls -l include/fragmentos.h");

    printf("[DEBUG] Tentando abrir include/fragmentos.h...\n");
    FILE *header = fopen("include/fragmentos.h", "r");
    if (!header) {
        char cwd2[PATH_MAX];
        if (getcwd(cwd2, sizeof(cwd2)) != NULL) {
            fprintf(stderr, "[DEBUG] Diretório atual: %s\n", cwd2);
        } else {
            perror("getcwd() error");
        }

        fprintf(stderr, "Erro: fragmentos.h não foi gerado.\n");
        return 1;
    }
    printf("[DEBUG] Arquivo fragmentos.h aberto com sucesso.\n");
    fclose(header);




    printf("Arquivo 'fragmentos.h' carregado com sucesso.\n");
    return 0;
}
