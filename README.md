# Trabalho Programacao de Computadores II (2026/2)

## Dependencias
- gcc, make
- raylib (Linux: `sudo apt install libraylib-dev`; Windows: w64devkit + raylib oficial)
- Windows: extrair o w64devkit (github.com/skeeto/w64devkit) e colocar `C:\w64devkit\bin`
  no PATH; extrair `raylib-X.Y_win64_mingw-w64.zip` (github.com/raysan5/raylib/releases)
  em `C:\raylib` (com as pastas `include` e `lib`)

## Compilacao e execucao
O Makefile detecta o sistema (Linux ou Windows) sozinho.
```
make          # compila (gera resident no Linux, resident.exe no Windows)
make run      # compila e executa
make clean    # apaga o executavel e o save.bin
```
Se a raylib estiver em outra pasta no Windows: `make RAYLIB_DIR=D:/caminho/raylib`.

Execute a partir da pasta do projeto: os arquivos `mapa1.txt`, `mapa2.txt`, ...
precisam estar no diretorio de execucao.

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
