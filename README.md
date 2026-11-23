# Resident Evil - Trabalho Pratico de Programacao de Computadores II (2026/2)

## Dependencias
- gcc, make
- raylib (Linux: `sudo apt install libraylib-dev`; Windows: w64devkit + raylib oficial)

## Compilacao e execucao
```
make
./resident      # execute na pasta onde estao mapa1.txt, mapa2.txt...
```

## Controles
- Setas ou WASD: mover (prioridade fixa quando varias direcoes: TODO documentar)
- Espaco: coletar / entregar
- TAB: menu (N novo jogo, C carregar, S salvar, Q sair, V voltar)

## Organizacao dos arquivos
- `src/tipos.h`  structs e constantes
- `src/mapa.*`   leitura dos mapas e alocacao dinamica
- `src/render.*` desenho com raylib
- `src/main.c`   loop principal
- `mapa1.txt`    mapa de teste

## Integrantes
- TODO
