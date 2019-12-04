#include <curses.h>
#include <iostream>
#include <fstream>
#ifndef MINADO_ENGINE
#define MINADO_ENGINE
enum {
	PRINCIPAL = 0,
	JOGAR = 1,
	SALVAR = 2,
	CARREGAR = 3,
	OPCOES = 4,
	SAIR = 5
};

enum {
	STATE_MENU = 0,
	STATE_GAME = 1
};

enum {
	VALOR_NADA = 0,
	VALOR_BOMBA = 1,
	VALOR_REVELADO = 2,
	VALOR_BANDEIRA = 3,
	VALOR_J_NADA = 4,
	VALOR_J_BOMBA = 5,
	VALOR_J_REVELADO = 6,
	VALOR_J_BANDEIRA = 7
};

namespace Engine {
//	char **terreno;  alocar um vetor[x][y] de memoria
	int gameIsReady = 0;
	void drawTerrain(char** terreno);
	void readScreen();
	int sizeX = 1;
	int sizeY = 1;
	int gameStates = STATE_MENU;
	int gameCounter = 0;
	char** tela;
	void handleKeyPresses();
}

namespace Menu {
	int atual = 0;
	int opcao = 1;
	std::string conteudo[5];
	void trocar(int tipo);
	void animar();
	void jogar(int x, int y, int dificuldade);
	void salvar();
	void opcoes();
	int carregar();
	int carregar_jogo(std::string file);
	void sair();
}
#endif
