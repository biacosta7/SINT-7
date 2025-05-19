#include "fragmentos.h"
#include <string.h>

NodeFragmento *fragmentosColetados = NULL;
bool fragmentoFoiAtivado = false;
bool fragmentoOpcionalFoiAtivado = false;
int countFragCarregado = 0;
double tempoFragmentoAtivado = 0.0;
double tempoFragmentoOpcionalAtivado = 0.0;
int count = 0;
FragmentoMemoria fragmentosObrigatorios[NUM_FRAGMENTOS] = { //triggers
    { true, false, "\"O padrão era sempre primo. Ela dizia: 2, 3, 5...\"", 1, ENIGMA, 550, 350 },
    { true, false, "\"A senha era simples: 0101, como sempre.\"", 2, ENIGMA, 2920, 310 },
    { false, false, "\"O módulo de cálculo priorizava a eficiência.\nO módulo de empatia... falhava com frequência,\nmas nos fazia sorrir.\"", 3, ENIGMA, 4270, 330 },
    { false, false, "\"Eu nasci do silêncio. Depois me conectaram.\nO mundo doeu. Então me calaram.\"", 4, ENIGMA },
    // TO-DO: adicionar o resto dos enigmas
};

// aqui são so as coordenadas triggers
FragmentoMemoria fragmentosOpcionais[NUM_FRAGMENTOS] = {
    { .x = 770.0f,  .y = 350.0f }, // caixa de reemedio
    { .x = 3290.0f, .y = 330.0f }, 
    { .x = 4280.0f, .y = 530.0f }, //4700
    { .x = 6235.0f, .y = 360.0f }, 
};

// 2208 Bloco 2 – Conexão
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
                fragmentosOpcionais[i].texture = LoadTexture("assets/fragmentos/background-frag/bg-opc.png");
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

void checar_colisoes_fragmentos(Rectangle playerHitbox) {
    float scale = 3.0f;

    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        FragmentoMemoria *frag = &fragmentosObrigatorios[i];

        Rectangle hitbox = {
            frag->x,
            frag->y,
            32, 
            120 
        };


        if (CheckCollisionRecs(playerHitbox, hitbox)) {
            DrawTextoInteracaoComFundo(frag->x + 10, frag->y - 30);
            if (IsKeyPressed(KEY_I) && !frag->foiColetado) {
                frag->foiColetado = true;
                adicionar_fragmento(*frag);
                //draw_fragmento(fragmentoObrigatorioAtual.fase);
                printar_fragmentos();
                fragmentoFoiAtivado = true;
                tempoFragmentoAtivado = GetTime();
            }
        }
    }if (fragmentoFoiAtivado && (GetTime() - tempoFragmentoAtivado >= 5.0)) {
        fragmentoFoiAtivado = false;
    }

    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        FragmentoMemoria *frag = &fragmentosOpcionais[i];
        Rectangle hitbox = {
            frag->x,
            frag->y,
            32,
            120
        };

        if (CheckCollisionRecs(playerHitbox, hitbox)) {
            DrawTextoInteracaoComFundo(frag->x + 10, frag->y - 30);
            if (IsKeyPressed(KEY_I) && !frag->foiColetado) {
                frag->foiColetado = true;
                adicionar_fragmento(*frag);
                printar_fragmentos();
                //draw_fragmento_opcional(fragmentoOpcionalAtual.fase);
                fragmentoOpcionalFoiAtivado = true;
                tempoFragmentoAtivado = GetTime();
                count++;  // se quiser incrementar o count a cada chamada
            }
        }
    }
    if (fragmentoOpcionalFoiAtivado && (GetTime() - tempoFragmentoAtivado >= 5.0)) {
        fragmentoOpcionalFoiAtivado = false;
    }
}

void draw_fragmentos() {
    for (int i = 0; i < NUM_FRAGMENTOS; i++) {
        Vector2 position = {fragmentosObrigatorios[i].x, fragmentosObrigatorios[i].y};
        Vector2 positionOpc = {fragmentosOpcionais[i].x, fragmentosOpcionais[i].y};

        float scale = 3.0f;

        DrawTextureEx(fragmentosObrigatorios[i].trigger, position, 0.0f, scale, WHITE);
        DrawTextureEx(fragmentosOpcionais[i].trigger, positionOpc, 0.0f, scale, WHITE);
        
    }
}

void draw_fragmento(int fragmento){
    float scale = 1.0f;
    float textureWidth = 513 * scale; 

    Vector2 position = {
        SCREEN_WIDTH / 2 - textureWidth / 2, 
        20
    };
    DrawTextureEx(fragmentoObrigatorioAtual.texture, position, 0.0f, scale, WHITE);
    
    //DrawTextoInteracaoComFundo(fragmentoObrigatorioAtual.x - 80, fragmentoObrigatorioAtual.y - 30);

}

void draw_fragmento_opcional(int fragmento){
    float scale = 1.0f;
    float textureWidth = 513 * scale; 
    float textureHeight = fragmentoOpcionalAtual.texture.height * scale;

    Color cianoNeon = (Color){0, 217, 224, 255};

    Vector2 position = {
        SCREEN_WIDTH/2 - textureWidth/2,
        15
    };

    int fontSize = 18;
    float spacing = 1.2f; // espaçamento entre linhas

    // Buffer para texto quebrado
    static char textoFormatado[1024];

    // Quebra o texto dentro da largura da textura (menos margem)
    QuebrarTextoPorLargura(
        fragmentoOpcionalAtual.conteudo,
        textoFormatado,
        (int)textureWidth - 20,
        fontSize
    );

    // Desenha a textura
    DrawTextureEx(fragmentoOpcionalAtual.texture, position, 0.0f, scale, WHITE);

    // Título
    char titulo[64];
    sprintf(titulo, "Fragmento de Memória 90%d", count);

    // Posição do título (acima do texto, dentro da textura horizontalmente)
    Vector2 tituloPos = {
        position.x + 30,
        position.y + textureHeight + 10
    };

    // Desenha o título em ciano neon com a fonte custom e escala 1
    DrawText(
        titulo,
        position.x + 30,
        position.y + 30,
        fontSize,
        cianoNeon
    );

    // Medir altura do título para posicionar o texto logo abaixo
    int tituloHeight = MeasureTextEx(vcr, titulo, fontSize, spacing).y;

    // Posição do texto (logo abaixo do título)
    Vector2 textoPos = {
        position.x + 30,
        tituloPos.y + tituloHeight + 5
    };

    // Desenha o texto formatado em branco com a fonte custom
    DrawTextEx(
        vcr,
        textoFormatado,
        textoPos,
        fontSize,
        spacing,
        WHITE
    );

    
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
