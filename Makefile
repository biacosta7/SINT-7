export GOOGLE_API_KEY=AIzaSyA0rF-Fhy6SsPowYL0dule0fFz_UtAEhkQ

setup: 
	@echo "Criando ambiente virtual..."
	python3 -m venv venv
	@echo "Instalando dependências..."
	venv/bin/pip install google-generativeai
	@echo "Ambiente pronto."

run-server:
	@echo "Iniciando servidor..."
	@venv/bin/python ia/server.py

sint7: src/*.c
	@echo "Compilando jogo..."
	@{ \
		echo "Tentando com paths do Homebrew..."; \
		gcc src/*.c -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib -o sint7 \
			-lraylib -lm -ldl -lpthread; \
	} || { \
		gcc src/*.c -Iinclude -o sint7 \
			-lraylib -framework OpenGL -framework Cocoa \
			-framework IOKit -framework CoreVideo; \
	} || { \
		echo "Erro: Falha na compilação do projeto."; \
		exit 1; \
	}

start:
	@./start.sh

all: setup sint7
