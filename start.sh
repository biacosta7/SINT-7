#!/bin/bash

# Roda setup
make setup

# Roda o servidor em background (para não travar o script)
make run-server &

# Dá um tempinho para o servidor iniciar
sleep 8

# Força recompilação do jogo
make -B sint7

# Espera o servidor finalizar (opcional, se quiser manter o script aberto)
wait
