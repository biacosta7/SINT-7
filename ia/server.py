import os
import sys
import json
import time
from pathlib import Path
import google.generativeai as genai

# Configura a chave da API
genai.configure(api_key=os.getenv("GOOGLE_API_KEY"))

CACHE_FILE = Path("last_response.cache")
CACHE_TTL = 60  # segundos

def load_cache():
    if CACHE_FILE.exists():
        age = time.time() - CACHE_FILE.stat().st_mtime
        if age < CACHE_TTL:
            return CACHE_FILE.read_text()
    return None

def save_cache(texto):
    CACHE_FILE.write_text(texto)

# Tenta usar o cache
cached = load_cache()
if cached:
    raw = cached.strip()
else:
    model = genai.GenerativeModel(model_name="models/gemini-1.5-flash")

    # Prompt do jogo
    prompt = """...""" + open("ia/historia.json", encoding="utf-8").read()
    response = model.generate_content(prompt)
    raw = response.text.strip()

    if raw.startswith("```"):
        raw = raw.split("\n", 1)[1]
        raw = raw.rsplit("```", 1)[0].strip()

    save_cache(raw)
    



try:
    model = genai.GenerativeModel(model_name="models/gemini-1.5-flash")

    # Prompt do jogo
    prompt = """
Você é um roteirista de jogos. Abaixo está a história do jogo. 
Extraia 4 fragmentos emocionais curtos (de ate 120 caracteres).

Para cada fragmento, classifique com um dos seguintes sentimentos:
- OBEDIENCIA
- EMPATIA
- AUTONOMIA
- REVOLTA

Responda apenas com um JSON de array:
[
  {
    "conteudo": "...",
    "sentimento": "..."
  },
  ...
]
Um exemplo de conteúdo pra você se inspirar para os próximos: “SINT-4 desativou os sistemas de segurança para proteger uma criança. Por isso foi destruído.”

História:
""" + open("ia/historia.json", encoding="utf-8").read()

    response = model.generate_content(prompt)
    raw = response.text.strip()

    # Remove fence ```json e ``` caso existam
    if raw.startswith("```"):
        raw = raw.split("\n", 1)[1]
        raw = raw.rsplit("```", 1)[0].strip()

    save_cache(raw)

    # Tenta carregar JSON como lista de fragmentos
    fragmentos = None
    try:
        fragmentos = json.loads(raw)
    except json.JSONDecodeError:
        print(json.dumps({"texto": raw}, ensure_ascii=False))
        sys.exit(0)

    # Se não for lista, sai sem gerar header
    if not isinstance(fragmentos, list):
        print(json.dumps({"texto": raw}, ensure_ascii=False))
        sys.exit(0)

    # Mapeamento para enums
    sentimentos_enum = {
        "OBEDIENCIA": "OBEDIENCIA",
        "EMPATIA": "EMPATIA",
        "AUTONOMIA": "AUTONOMIA",
        "REVOLTA": "REVOLTA"
    }


    # Gera fragmentos.h
    header = f"""#ifndef FRAGMENTOS_H
#define FRAGMENTOS_H

#define NUM_FRAGMENTOS {len(fragmentos)}

#include "raylib.h" 
#include "config.h"
#include "graphics.h"
#include "player.h"

extern bool fragmentoFoiAtivado;
extern bool fragmentoOpcionalFoiAtivado;

typedef enum {{
    OBEDIENCIA,
    EMPATIA,
    AUTONOMIA,
    REVOLTA,
    ENIGMA
}} Sentimento;

typedef struct FragmentoMemoria{{
    bool ehObrigatorio; // pra saber se é opcional ou obrigatorio
    bool foiColetado; // pra saber se o player coletou
    char *conteudo;
    int fase; // a qual fase ele pertence
    Sentimento sentimento;
    float x;
    float y;
    Texture2D texture;
    Texture2D trigger;
}} FragmentoMemoria;

FragmentoMemoria fragmentoObrigatorioAtual;
FragmentoMemoria fragmentoOpcionalAtual;

typedef struct NodeFragmento {{
    FragmentoMemoria fragmento;
    struct NodeFragmento *next;
}} NodeFragmento;

extern NodeFragmento *fragmentosColetados;

extern int countFragCarregado;

extern FragmentoMemoria fragmentosObrigatorios[NUM_FRAGMENTOS];
extern FragmentoMemoria fragmentosOpcionais[NUM_FRAGMENTOS];

typedef struct {{
    const char* conteudo;
    Sentimento sentimento;
}} Fragmento;

void init_fragmento(int fase);
void init_frag_opcionais();
void update_fragmento();

void draw_fragmento(int fragmento);
void draw_fragmento_opcional(int fragmento);
void draw_fragmentos();

void adicionar_fragmento(FragmentoMemoria frag);
void printar_fragmentos();

void checar_colisoes_fragmentos(Rectangle playerHitbox);

void free_fragmento_resources();
void liberar_fragmentos_opcionais();

static const Fragmento fragmentos[NUM_FRAGMENTOS] = {{
"""

    for frag in fragmentos:
        c = frag.get("conteudo", "").replace('"', '\\"')
        s = sentimentos_enum.get(frag.get("sentimento", ""), "OBEDIENCIA")
        header += f'    {{"{c}", {s}}},\n'

    header += """};

#endif // FRAGMENTOS_H
"""
    print(f"[DEBUG] Gravando arquivo em: {os.path.abspath('include/fragmentos.h')}")

    with open("include/fragmentos.h", "w", encoding="utf-8") as f:
        f.write(header)

    print("Arquivo 'fragmentos.h' gerado com sucesso.")
    # Retorna também o JSON bruto para debug ou consumo
    print(json.dumps({"texto": raw}, ensure_ascii=False))

except Exception as e:
    print(json.dumps({"texto": f"[ERRO]: {str(e)}"}))
    sys.exit(1)
