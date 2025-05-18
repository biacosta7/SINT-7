#include "fragmentos.h"
#include <string.h>

NodeFragmento *fragmentosColetados = NULL;
bool fragmentoFoiAtivado = false;
bool fragmentoOpcionalFoiAtivado = false;
int countFragCarregado = 0;
double tempoFragmentoAtivado = 0.0;
double tempoFragmentoOpcionalAtivado = 0.0;

FragmentoMemoria fragmentosObrigatorios[NUM_FRAGMENTOS] = {
    { true, false, "\"O padrão era sempre primo. Ela dizia: 2, 3, 5...\"", 1, ENIGMA, 550, 350 },
    { true, false, "\"A senha era simples: 0101, como sempre.\"", 2, ENIGMA, 2030, 330 },
    { false, false, "\"O módulo de cálculo priorizava a eficiência.\nO módulo de empatia... falhava com frequência,\nmas nos fazia sorrir.\"", 3, ENIGMA, 4140, 330 },
    { false, false, "\"Eu nasci do silêncio. Depois me conectaram.\nO mundo doeu. Então me calaram.\"", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

// aqui são so as coordenadas
FragmentoMemoria fragmentosOpcionais[NUM_FRAGMENTOS] = {
    { .x = 670.0f,  .y = 290.0f },
    { .x = 2030.0f, .y = 330.0f },
    { .x = 4140.0f, .y = 330.0f },
    { .x = 5000.0f, .y = 360.0f }, 
};

void init_frag_opcionais() {
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        fragmentosOpcionais[i].conteudo = strdup(fragmentos[i].conteudo);
        fragmentosOpcionais[i].sentimento = fragmentos[i].sentimento;
        fragmentosOpcionais[i].foiColetado = false;
        fragmentosOpcionais[i].ehObrigatorio = false;
        fragmentosOpcionais[i].fase = i;
    }
}

void init_fragmento(int fase){
    fragmentoObrigatorioAtual = fragmentosObrigatorios[fase-1];
    init_frag_opcionais();
    fragmentoOpcionalAtual = fragmentosOpcionais[fase-1];
}

void update_fragmento() {
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        if (fragmentosObrigatorios[i].fase == player.faseAtual) {
            fragmentoObrigatorioAtual = fragmentosObrigatorios[i];
            fragmentoOpcionalAtual = fragmentosOpcionais[i];

            if(i>=countFragCarregado){
                char path[64];
                sprintf(path, "assets/fragmentos/background-frag/obrigatorios/%03d.png", player.faseAtual);
                fragmentosObrigatorios[i].texture = LoadTexture(path);

                char trigger_path[64];
                sprintf(trigger_path, "assets/fragmentos/trigger-frag/obrigatorios/%03d.png", player.faseAtual);
                fragmentosObrigatorios[i].trigger = LoadTexture(trigger_path);

                //opcionais
                fragmentosOpcionais[i].texture = LoadTexture("assets/fragmentos/background-frag/bg-opc.png"); // // Escala do fundo
                int scaleI = 5;
                int margem = 30; // margem interna para o texto

                // Centraliza a textura na tela
                int inventX = (SCREEN_WIDTH - fragmentosOpcionais[i].texture.width * scaleI) / 2;
                int inventY = (SCREEN_HEIGHT - fragmentosOpcionais[i].texture.height * scaleI) / 2;

                // Desenha a textura
                DrawTextureEx(fragmentosOpcionais[i].texture, (Vector2){inventX, inventY}, 0.0f, scaleI, WHITE);

                // Define limites para o texto
                int larguraDisponivel = fragmentosOpcionais[i].texture.width * scaleI - 2 * margem;
                int textoX = inventX + margem + 10;
                int textoY = inventY + margem;
                int linhaAltura = 25;

                char textoFormatado[1024];
                QuebrarTextoPorLargura(fragmentoOpcionalAtual.conteudo, textoFormatado, larguraDisponivel, 20);

                // Desenha o texto dentro da área da textura
                BeginScissorMode(inventX + margem, inventY + margem,
                                larguraDisponivel, fragmentosOpcionais[i].texture.height * scaleI - 2 * margem);

                char *linha = strtok(textoFormatado, "\n");
                while (linha != NULL) {
                    DrawText(linha, textoX, textoY, 20, WHITE);
                    printf("linha: %s\n", linha);
                    textoY += linhaAltura;
                    linha = strtok(NULL, "\n");
                }

                EndScissorMode();




                char trigger_path_opc[64];
                sprintf(trigger_path_opc, "assets/fragmentos/trigger-frag/opcionais/%03d.png", player.faseAtual);
                fragmentosOpcionais[i].trigger = LoadTexture(trigger_path_opc);

                countFragCarregado++;
            }

            break;
        }
    }
}

void adicionar_fragmento(FragmentoMemoria novoFragmento) {
    NodeFragmento *novo = (NodeFragmento *)malloc(sizeof(NodeFragmento));
    if (!novo) return; // Falha de alocação

    novo->fragmento = novoFragmento;
    novo->next = NULL;

    if(fragmentosColetados == NULL){
        fragmentosColetados = novo;
    }
    else{
        NodeFragmento *atual = fragmentosColetados;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

void printar_fragmentos() {
    NodeFragmento *atual = fragmentosColetados;
    int i = 1;
    
    if (!atual) {
        printf("Nenhum fragmento coletado.\n");
        return;
    }

    printf("Fragmentos coletados:\n");
    while (atual != NULL) {
        printf("Fragmento %d:\n", i++);
        printf("  Conteúdo: %s\n", atual->fragmento.conteudo);
        printf("  Fase: %d\n", atual->fragmento.fase);
        printf("  Obrigatório: %s\n", atual->fragmento.ehObrigatorio ? "Sim" : "Não");
        printf("  Foi coletado: %s\n", atual->fragmento.foiColetado ? "Sim" : "Não");
        printf("  Posição: (%.2f, %.2f)\n", atual->fragmento.x, atual->fragmento.y);
        printf("----------------------------------\n");

        atual = atual->next;
    }
}

// Colisões
bool check_colisao_fragmento(Rectangle playerHitbox){

    // HITBOX DO FRAGMENTO
    Rectangle fragmentoHitbox = {
        fragmentoObrigatorioAtual.x,
        fragmentoObrigatorioAtual.y,
        32, //width
        120 //height
    };

    //para ver onde ta a caixa de colisao:
    //DrawRectangle(fragmentoHitbox.x, fragmentoHitbox.y, fragmentoHitbox.width, fragmentoHitbox.height, YELLOW);
    // DrawRectangle(playerHitbox.x, playerHitbox.y, playerHitbox.width, playerHitbox.height, GREEN);

    // colisão com fragmento
    if (CheckCollisionRecs(playerHitbox, fragmentoHitbox)) {
        DrawText("(I) para interagir", fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30, 20, GREEN);
        
        if (IsKeyPressed(KEY_I)) { // agora só no momento do clique
            if (!fragmentoObrigatorioAtual.foiColetado) {
                fragmentosObrigatorios[player.faseAtual - 1].foiColetado = true;
                fragmentoObrigatorioAtual = fragmentosObrigatorios[player.faseAtual - 1];
                adicionar_fragmento(fragmentoObrigatorioAtual);
                printar_fragmentos();
            }
            fragmentoFoiAtivado = true;
            tempoFragmentoAtivado = GetTime();
            return true;
        }
    }
     if (fragmentoFoiAtivado && (GetTime() - tempoFragmentoAtivado >= 5.0)) {
        fragmentoFoiAtivado = false;
    }
    return false;
}

bool check_colisao_fragmento_opcional(Rectangle playerHitbox){

    // HITBOX DO FRAGMENTO
    Rectangle fragmentoHitbox = {
        fragmentoOpcionalAtual.x,
        fragmentoOpcionalAtual.y,
        32, //width
        120 //height
    };

    //para ver onde ta a caixa de colisao:
    DrawRectangle(fragmentoHitbox.x, fragmentoHitbox.y, fragmentoHitbox.width, fragmentoHitbox.height, YELLOW);
    // DrawRectangle(playerHitbox.x, playerHitbox.y, playerHitbox.width, playerHitbox.height, GREEN);

    // colisão com fragmento
    if (CheckCollisionRecs(playerHitbox, fragmentoHitbox)) {
        DrawTextoInteracaoComFundo(fragmentoOpcionalAtual.x + 10, fragmentoOpcionalAtual.y - 30);
        
        if (IsKeyPressed(KEY_I)) { // agora só no momento do clique
            if (!fragmentoOpcionalAtual.foiColetado) {
                fragmentosOpcionais[player.faseAtual - 1].foiColetado = true;
                fragmentoOpcionalAtual = fragmentosOpcionais[player.faseAtual - 1];
                adicionar_fragmento(fragmentoOpcionalAtual);
                printar_fragmentos();
            }
            fragmentoOpcionalFoiAtivado = true;
            tempoFragmentoOpcionalAtivado = GetTime();
            return true;
        }
    }
     if (fragmentoOpcionalFoiAtivado && (GetTime() - tempoFragmentoOpcionalAtivado >= 5.0)) {
        fragmentoOpcionalFoiAtivado = false;
    }
    return false;
}


void draw_fragmento(int fragmento){
    float scale = 1.0f;
    float textureWidth = 513 * scale; 

    Vector2 position = {
        SCREEN_WIDTH / 2 - textureWidth / 2, 
        20
    };
    DrawTextureEx(fragmentoObrigatorioAtual.texture, position, 0.0f, scale, WHITE);
    
    DrawTextoInteracaoComFundo(fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30);

}

void draw_fragmento_opcional(int fragmento){
    float scale = 1.0f;
    float textureWidth = 513 * scale; 

    Vector2 position = {
        SCREEN_WIDTH / 2 - textureWidth / 2, 
        20
    };
    DrawTextureEx(fragmentoOpcionalAtual.texture, position, 0.0f, scale, WHITE);
}

void draw_fragmento_trigger(){
    Vector2 position = {fragmentoObrigatorioAtual.x, fragmentoObrigatorioAtual.y};
    float scale = 3.0f;
    DrawTextureEx(fragmentoObrigatorioAtual.trigger, position, 0.0f, scale, WHITE);
}

void draw_fragmento_opcional_trigger(){
    Vector2 position = {fragmentoOpcionalAtual.x, fragmentoOpcionalAtual.y};
    float scale = 3.0f;
    DrawTextureEx(fragmentoOpcionalAtual.trigger, position, 0.0f, scale, WHITE);
}

void free_fragmento_resources() {
    NodeFragmento *atual = fragmentosColetados;
    while (atual != NULL) {
        NodeFragmento *temp = atual;
        atual = atual->next;
        free(temp);
    }
    fragmentosColetados = NULL;

    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        UnloadTexture(fragmentosObrigatorios[i].texture);
        UnloadTexture(fragmentosObrigatorios[i].trigger);
        UnloadTexture(fragmentosOpcionais[i].texture);
        UnloadTexture(fragmentosOpcionais[i].trigger);
    }
}


void liberar_fragmentos_opcionais() {
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        if (fragmentosOpcionais[i].conteudo != NULL) {
            free(fragmentosOpcionais[i].conteudo);
        }
    }
}
