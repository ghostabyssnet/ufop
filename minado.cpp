#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include "engine.h"

#define INSTRUCOES_0 "X para colocar/tirar bomba, setas para se mover"
#define INSTRUCOES_1 "R resolve o jogo automaticamente, Q sai do jogo"
#define INSTRUCOES_2 "S abre o menu de salvar jogo, L o de carregar jogo"

enum {
    BACKGROUND_FLASH = 1,
    TEXT_FLASH = 2
};

struct entidade {
	char representacao; // representacao visual da entidade
	char valor;
	int cor;
};

struct jogador {
    int x = 0;
    int y = 0;
};

jogador player; // eu sei que e uma ma pratica de programacao usar variaveis globais,
// falei sobre com o puca quarta-feira antes de entregar o trabalho e ficou tudo bem

namespace CampoMinado {
    entidade **Terreno;
	static int bandeiras;
    void moverJogador(int x, int y);
    void colocarJogador(int x, int y);
    void removerJogador(int x, int y);
//	std::vector< std::vector <entidade> > terreno;
}

int main(int argc, char *argv[]) {
    jogador *p = &player;
	// iniciar o curses e setar cores
	initscr();
	start_color();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	init_pair(BACKGROUND_FLASH, COLOR_WHITE, COLOR_BLACK);
	init_pair(TEXT_FLASH, COLOR_GREEN, COLOR_BLACK);
	// iniciar o timer
	int timer = 0;
	Menu::atual = 0;
	std::string linhaTimer; // o timer do jogo e definido aqui
	printw("=== CAMPO MINADO ===");
	if (argc > 1) {
		Menu::carregar_jogo(argv[1]);
	}
	else {
		Menu::trocar(PRINCIPAL);
	}
	// loop principal
	while (1) {
        move(p->x, p->y);
		int movenum;
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // transformando em frames
		if (Engine::gameCounter >= 10) { Engine::gameCounter = 0; }
		if (Engine::gameStates == STATE_MENU) {
			movenum = 7;
		}
		else {
			movenum = Engine::sizeY;
		}
		move(movenum, 0);
		timer++;
		// resetando a linha e imprimindo ela de novo com o timer do programa
		linhaTimer = "TIMER -> ";
		linhaTimer += std::to_string((timer / 10));
		linhaTimer += "s ";
		linhaTimer += std::to_string(Engine::gameCounter);
		linhaTimer += "ms";
		printw(linhaTimer.c_str());
		move(0, 0);
		attron(A_BOLD);
		printw("=== CAMPO MINADO ===");
		attroff(A_BOLD);
		Engine::handleKeyPresses();
		if (Engine::gameStates == STATE_MENU) {
			Menu::animar();
		}
		else {
			if (Engine::gameStates == STATE_GAME) {
				Engine::readScreen();
                move(Engine::sizeY + 1, 0);
                printw(INSTRUCOES_0);
                move(Engine::sizeY + 2, 0);
                printw(INSTRUCOES_1);
                move(Engine::sizeY + 3, 0);
                printw(INSTRUCOES_2);
                move(Engine::sizeY + 4, 0);
                printw("POSICAO ATUAL: ");
                printw("%d", p->x);
                printw("/");
                printw("%d", p->y);
                move(p->x, p->y);
            }
		}
				Engine::gameCounter++;
			}
			endwin();
			return 0;
		}

void CampoMinado::colocarJogador(int x, int y) {
    switch (CampoMinado::Terreno[x][y].valor) {
        case VALOR_NADA:
            CampoMinado::Terreno[x][y].valor = VALOR_J_NADA;
            break;
        case VALOR_BOMBA:
            CampoMinado::Terreno[x][y].valor = VALOR_J_BOMBA;
            break;
        case VALOR_REVELADO:
            CampoMinado::Terreno[x][y].valor = VALOR_J_REVELADO;
            break;
        case VALOR_BANDEIRA:
            CampoMinado::Terreno[x][y].valor = VALOR_J_BANDEIRA;
            break;
    }
}

void CampoMinado::removerJogador(int x, int y) {
    switch (CampoMinado::Terreno[x][y].valor) {
        case VALOR_J_NADA:
            CampoMinado::Terreno[x][y].valor = VALOR_NADA;
            break;
        case VALOR_J_BOMBA:
            CampoMinado::Terreno[x][y].valor = VALOR_BOMBA;
            break;
        case VALOR_J_REVELADO:
            CampoMinado::Terreno[x][y].valor = VALOR_REVELADO;
            break;
        case VALOR_J_BANDEIRA:
            CampoMinado::Terreno[x][y].valor = VALOR_BANDEIRA;
            break;
    }
}

void CampoMinado::moverJogador(int x, int y) {
    jogador *p = &player;
    int xAtual = p->x;
    int yAtual = p->y;
    if (x >= 0 && y >= 0) {
        p->x = x;
        p->y = y + 1;
        CampoMinado::colocarJogador(x, y);
        CampoMinado::removerJogador(xAtual, yAtual);
    }
}

void Engine::handleKeyPresses() {
    jogador *p = &player;
    int tecla;
	tecla = wgetch(stdscr);
	switch(tecla) {
        case 113:
        case 81:
            refresh();
            endwin();
            exit(0);
            break;
		case KEY_UP:
			if (Engine::gameStates == STATE_MENU) {
				if (Menu::opcao > 1) {
					Menu::opcao--;
				}
			}
            else {
                int x = p->x;
                int y = p->y + 1;
                CampoMinado::moverJogador(x, y);
            }
			break;
		case KEY_DOWN:
			if (Engine::gameStates == STATE_MENU) {
				if (Menu::opcao < 5) {
					Menu::opcao++;
				}
			}
            else {
                int x = p->x;
                int y = p->y - 1;
                CampoMinado::moverJogador(x, y);
            }
			break;
		case KEY_RIGHT:
			if (Engine::gameStates == STATE_GAME) {
                int x = p->x + 1;
                int y = p->y;
                CampoMinado::moverJogador(x, y);
			}
			break;
		case KEY_LEFT:
			if (Engine::gameStates == STATE_GAME) {
                int x = p->x - 1;
                int y = p->y;
                CampoMinado::moverJogador(x, y);
			}
			break;
		case 10: // tecla enter do centro do teclado, KEY_ENTER e a do numpad
		case KEY_ENTER:
			if (Engine::gameStates == STATE_MENU) {
				switch(Menu::atual) {
					case PRINCIPAL:
						switch(Menu::opcao) {
							case JOGAR:
								Menu::trocar(JOGAR);
								break;
							case SALVAR:
								Menu::salvar();
								break;
							case CARREGAR:
								Menu::trocar(CARREGAR);
								Menu::carregar();
								break;
							case OPCOES:
								Menu::trocar(OPCOES);
								break;
							case SAIR:
								Menu::sair();
								break;
						}
						break;
					case JOGAR:
						int tamX;
						int tamY;
						int dificuldade;
						if (Menu::opcao == 5) { // caso seja o botao de MENU PRINCIPAL
							Menu::trocar(PRINCIPAL);
						}
						switch(Menu::opcao) {
							case 1:
								tamX = 7;
								tamY = 7;
								dificuldade = 4;
								break;
							case 2:
								tamX = 10;
								tamY = 10;
								dificuldade = 6;
								break;
							case 3:
								tamX = 17;
								tamY = 17;
								dificuldade = 10;
								break;
						}
						if (Menu::opcao > 0 && Menu::opcao <= 3) {
						  Menu::jogar(tamX, tamY, dificuldade);
							Engine::sizeX = tamX;
							Engine::sizeY = tamY;
						}
						break;
					case OPCOES:
						if (Menu::opcao == 4) { // caso seja o botao de MENU PRINCIPAL
							Menu::trocar(PRINCIPAL);
						}
						break;
				}
			}
			break;
	}
}

void Menu::jogar(int x, int y, int dificuldade) {
    jogador *p = &player;
	Engine::gameStates = STATE_GAME;
	printw("Carregando...");
	int minas = dificuldade;
	entidade **terreno;
	terreno = (entidade**)malloc(x * sizeof(entidade*));
	for (int i = 0; i < x; i++) {
		terreno[i] = (entidade*)malloc(y * sizeof(entidade));
	}
	srand(time(NULL));
	clear();
	for (int k = 0; k < x; k++) {
		for (int l = 0; l < y; l++) {
			terreno[k][l].valor = VALOR_NADA;
		}
	}
	while (minas > 0) {
		for (int i = 0; i < x; i++) {
 			for (int j = 0; j < y; j++) {
	   			int random = std::rand(); // rand() retorna entre 0 e MAX
	   			if (minas > 0 && random % dificuldade == 0 && terreno[i][j].valor != VALOR_BOMBA) { // chance de <dificuldade>
				      terreno[i][j].valor = VALOR_BOMBA;
							minas--;
					}
					else {
							terreno[i][j].valor = VALOR_NADA;
					}
					terreno[i][j].representacao = '-';
			}
		}
	}
	Engine::tela = (char**)malloc(x * sizeof(char*));
	for (int i = 0; i < x; i++) {
		Engine::tela[i] = (char*)malloc(y * sizeof(char));
	}
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			Engine::tela[i][j] = terreno[i][j].representacao;
		}
	}
    terreno[0][0].valor = terreno[0][0].valor == VALOR_BOMBA ? VALOR_J_BOMBA : VALOR_J_NADA;
//    terreno[0][0].representacao = '@';
//    Engine::tela[0][0] = terreno[0][0].representacao;
    p->x = 0;
    p->y = 0;
    CampoMinado::Terreno = (entidade**)malloc(x * sizeof(entidade*));
	for (int i = 0; i < x; i++) {
		CampoMinado::Terreno[i] = (entidade*)malloc(y * sizeof(entidade));
	}
    for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			CampoMinado::Terreno[i][j] = terreno[i][j];
		}
	}
    Engine::gameIsReady = 1;
    free(terreno);
}

void Engine::drawTerrain(char** terreno) {
	for (int x = 0; x < Engine::sizeX; x++) {
		for (int y = 0; y < Engine::sizeY; y++) {
			move(x, y);
			printw(&terreno[x][y]);
		}
	}
}

void Engine::readScreen() {
	if (Engine::gameIsReady) {
		Engine::drawTerrain(tela);
	}
}

void Menu::salvar() {
	conteudo[SALVAR] = "-> Salve o jogo quando ele estiver sendo executado!";
	move(SALVAR + 1, 7);
	printw(conteudo[SALVAR].c_str());
}

void Menu::trocar(int tipo) {
	beep();
	Menu::atual = tipo;
	clear();
}

void Menu::sair() {
	beep();
	endwin();
	exit(0);
}

int Menu::carregar() {
	return 0;
}

int Menu::carregar_jogo(std::string filename) {
	std::ifstream file;
	char data[26];
	std::string s;
	file.open(filename, std::ios::in);
	if (!file) {
		return 1;
	}
//	file >> data;
//	Menu::jogar(data[0], data[2]);
	while(file.eof()==0) {
        file >> s;
        std::cout<<s;
	}
	std::cout<<"\n";
	return 0;
}

void Menu::animar() {
	switch(Menu::atual) {
		case PRINCIPAL:
			conteudo[0] = "MENU PRINCIPAL";
			conteudo[1] = "Novo Jogo";
			conteudo[2] = "Salvar";
			conteudo[3] = "Carregar";
			conteudo[4] = "Opcoes";
			conteudo[5] = "Sair";
			break;
		case JOGAR:
			conteudo[0] = "NOVO JOGO";
			conteudo[1] = "Facil           (6x6)";
			conteudo[2] = "Medio           (9x9)";
			conteudo[3] = "Dificil         (16x16)";
			conteudo[4] = "Super Aleatorio (?x?)";
			conteudo[5] = "MENU PRINCIPAL";
			break;
		case SALVAR:
			conteudo[0] = "SALVAR JOGO";
			conteudo[1] = "Digite o nome do arquivo para que o jogo seja salvo... (terminado em .txt)";
			conteudo[2] = "-> ";
			conteudo[3] = "Arquivo do jogo...";
			conteudo[4] = "MENU PRINCIPAL";
			conteudo[5] = "Volte ao menu principal acima...";
			break;
		case CARREGAR:
			conteudo[0] = "CARREGAR JOGO";
			conteudo[1] = "Digite o nome do arquivo para que o jogo seja carregado... (terminado em .txt)";
			conteudo[2] = "-> ";
			conteudo[3] = "Arquivo do jogo...";
			conteudo[4] = "MENU PRINCIPAL";
			conteudo[5] = "Volte ao menu principal acima...";
			break;
		case OPCOES:
			conteudo[0] = "OPCOES";
			conteudo[1] = "Aqui ficariam as opcoes do jogo como audio e etc.";
			conteudo[2] = "Eu queria utilizar OpenAL para fazer audio, mas parece ser uma ma ideia";
			conteudo[3] = "usar libraries externas. Deixo aqui o recado!";
			conteudo[4] = "MENU PRINCIPAL";
			conteudo[5] = "Volte ao menu principal acima...";
			break;
		// ...
	}
	int attr;
	for (int x = 0; x <= 5; x++) {
		if (conteudo[x] == "") { break; }
		move(x + 1, 0);
		if (Menu::opcao == x) {
			if (Engine::gameCounter % 2 == 0) {
				attron(COLOR_PAIR(TEXT_FLASH));
				attron(A_BOLD);
				attr = TEXT_FLASH;
			}
		}
		printw((conteudo[x].c_str()));
		attroff(COLOR_PAIR(attr)); // desliga o attr se nao for hora de animar
		attroff(A_BOLD);
	}
	move(Menu::opcao + 1, 0);
	refresh();
}
