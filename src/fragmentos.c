#include "fragmentos.h"

NodeFragmento *fragmentosColetados = NULL;
bool fragmentoFoiAtivado = false;
int countFragCarregado = 0;

FragmentoMemoria fragmentosObrigatorios[TOTAL_FRAGMENTOS_OBRIGATORIOS] = {
    { true, false, "\"O padrão era sempre primo. Ela dizia: 2, 3, 5...\"", 1, ENIGMA, 550, 350 },
    { true, false, "\"A senha era simples: 0101, como sempre.\"", 2, ENIGMA, 2030, 330 },
    { false, false, "\"O módulo de cálculo priorizava a eficiência.\nO módulo de empatia... falhava com frequência,\nmas nos fazia sorrir.\"", 3, ENIGMA, 4140, 330 },
    { false, false, "\"Eu nasci do silêncio. Depois me conectaram.\nO mundo doeu. Então me calaram.\"", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

//TO-DO: pegar onde a camera tá e colocar o conteudo do fragmento no top centro
void init_fragmento(int fase){
    fragmentoObrigatorioAtual = fragmentosObrigatorios[fase-1];
}

void update_fragmento() {
    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        if (fragmentosObrigatorios[i].fase == player.faseAtual) {
            fragmentoObrigatorioAtual = fragmentosObrigatorios[i];

            if(i>=countFragCarregado){
                char path[64];
                sprintf(path, "assets/fragmentos/background-frag/%03d.png", player.faseAtual);
                fragmentosObrigatorios[i].texture = LoadTexture(path);

                char trigger_path[64];
                sprintf(trigger_path, "assets/fragmentos/trigger-frag/%03d.png", player.faseAtual);
                fragmentosObrigatorios[i].trigger = LoadTexture(trigger_path);

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
    novo->next = fragmentosColetados;
    fragmentosColetados = novo;
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
        DrawText("(F) para interagir", fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30, 20, GREEN);
        
        if (IsKeyPressed(KEY_F)) { // agora só no momento do clique
            if (!fragmentoObrigatorioAtual.foiColetado) {
                fragmentosObrigatorios[player.faseAtual - 1].foiColetado = true;
                fragmentoObrigatorioAtual = fragmentosObrigatorios[player.faseAtual - 1];
                adicionar_fragmento(fragmentoObrigatorioAtual);
                printar_fragmentos();
            }
            fragmentoFoiAtivado = !fragmentoFoiAtivado;
            return true;
        }
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

}

void draw_fragmento_trigger(){
    Vector2 position = {fragmentoObrigatorioAtual.x, fragmentoObrigatorioAtual.y};
    float scale = 3.0f;
    DrawTextureEx(fragmentoObrigatorioAtual.trigger, position, 0.0f, scale, WHITE);
}

void free_fragmento_resources() {
    NodeFragmento *atual = fragmentosColetados;
    while (atual != NULL) {
        NodeFragmento *temp = atual;
        atual = atual->next;
        free(temp);
    }
    fragmentosColetados = NULL;

    for (int i = 0; i < TOTAL_FRAGMENTOS_OBRIGATORIOS; i++) {
        UnloadTexture(fragmentosObrigatorios[i].texture);
        UnloadTexture(fragmentosObrigatorios[i].trigger);
    }
}



