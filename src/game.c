#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realSize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realSize + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realSize);
    mem->size += realSize;
    mem->memory[mem->size] = 0;

    printf("[DEBUG] Dados recebidos (%zu bytes): %s\n", realSize, (char *)contents); // debug
    return realSize;
}

void enviar_para_ia(const char *pergunta) {
    printf("[DEBUG] Função enviar_para_ia chamada com pergunta: %s\n", pergunta);

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Erro ao inicializar CURL\n");
        return;
    }

    struct MemoryStruct resposta;
    resposta.memory = malloc(1);
    resposta.size = 0;

    const char *url = "http://localhost:5000/gerar";

    char json[512];
    snprintf(json, sizeof(json), "{\"pergunta\": \"%s\"}", pergunta);
    printf("[DEBUG] JSON a ser enviado: %s\n", json);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resposta);

    printf("[DEBUG] Enviando requisição...\n");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Erro ao realizar requisição: %s\n", curl_easy_strerror(res));
    } else {
        printf("[DEBUG] Requisição concluída com sucesso.\n");
        printf("Resposta da IA: %s\n", resposta.memory);
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(resposta.memory);
}
