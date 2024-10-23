// Jogo_Galo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <string>

using namespace std;

#define	UP 72
#define	DOWN 80
#define ENTER 13
#define ESC 27

#define TAMANHO 3
#define NUM_OPCOES 4

struct JOGADOR
{
	string nome;
	char simbolo;
	int pontuacao;
};

struct JOGO
{
	char tabuleiro[TAMANHO][TAMANHO];
	JOGADOR jogador1;                       //optei assim para poder aceder ao jogador 1 e 2 DA ESTRUTURA JOGADOR só com a estrutura JOGO
	JOGADOR jogador2; 
	int jogadorAtual; //1 para 1. 2 para 2

};

//prototipos funções
char selecionarOpcao(int& opcaoSelecionada, string str1, string str2, string str3, string str4); 
void mostrarMenu(JOGO& jogo);
void mostrarInstrucoes();
void inicializarTabuleiro(JOGO& jogo);
void mostrarTabuleiro(JOGO& jogo);
void jogarMaquina(JOGO& jogo); 
int verificarVencedor(JOGO& jogo);
bool verificarEmpate(JOGO& jogo); 
void gravarPontuacao(const string& nomeFicheiro,int pontuacao);
void vezDeJogar(JOGO& jogo,bool jogarSozinho);
void jogar(JOGO& jogo, bool jogarSozinho);
int lerPontuacao(const string& nomeFicheiro);
void inicializarJogadores(JOGO& jogo, bool jogarSozinho);



int main()
{
    SetConsoleOutputCP(1252);  
    JOGO jogo;

    mostrarMenu(jogo);


}

char selecionarOpcao(int& opcaoSelecionada, string str1, string str2, string str3,string str4) {

    string opcoes[NUM_OPCOES] = { str1,str2,str3,str4 }; 
    char escolha;
    for (int i = 0; i < NUM_OPCOES; i++) {
        if (i == opcaoSelecionada) {
            // Destacar a opção selecionada
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            cout << "\t> " << opcoes[i] << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        else {
            cout << "\t  " << opcoes[i] << endl;
        }
    }
    // Ler uma tecla sem pressionar Enter
    cout << "\n\n\tUtilize as teclas direcionais para se deslocar no menu!";

    escolha = _getch();
    if (escolha == 224) { // Código para teclas especiais (setas)
        escolha = _getch(); // Captura a tecla especial
    }

    switch (escolha) {
    case UP:opcaoSelecionada = (opcaoSelecionada - 1 + NUM_OPCOES) % NUM_OPCOES; // torna o menu ciclico e nunca cai fora dos limites do array
        break;
    case DOWN:opcaoSelecionada = (opcaoSelecionada + 1) % NUM_OPCOES;
        break;
    }

    return escolha;
}
void mostrarMenu(JOGO& jogo) {
    int opcaoSelecionada = 0;

    while (true) {
        system("cls");

        cout << "\n_____________________________________________________________________________________";
        cout << "\n\n\t\t\t\tMENU PRINCIPAL - JOGO DO GALO" << endl;
        cout << "\n_____________________________________________________________________________________\n" << endl;

        char escolha = selecionarOpcao(opcaoSelecionada, "Um Jogador","Dois Jogadores", "Instruções", "Sair");
        if (escolha == ENTER) {
            switch (opcaoSelecionada) {
                case 0: jogar(jogo,true); break;
                case 1: jogar(jogo,false); break;
                case 2: mostrarInstrucoes(); break;
                case 3: exit(0); break;
            }
        }
    }
}
//função para mostrar as intruções
void mostrarInstrucoes() {

    bool sair = false;
    while (!sair) {
        system("cls");

        cout << "\n\n\tESC para sair para o menu Principal!\n" << endl;
        cout << "\n\t1. No jogo do galo o objetivo é fazer 3 em linha" << endl;
        cout << "\n\t2. Cada jogador fica com um simbulo X ou O." << endl;
        cout << "\n\t3. O jogador 1 terá direito a escolher o símbolo (X / O)." << endl;
        cout << "\n\t4. Poderá jogar sozinho ou contra a máquina." << endl;
        cout << "\n\t5. Deverá fazer a escolha com o número da linha (1-3) e o número da coluna (1-3)." << endl;
        

        if (_getch() == ESC)
            sair = true;
    }

}

// Mostrar o tabuleiro
void mostrarTabuleiro(JOGO& jogo) { 

    for (int i = 0; i < TAMANHO; i++) {
        cout << "\t";
        for (int j = 0; j < TAMANHO; j++) {
            cout << jogo.tabuleiro[i][j];
            if (j < TAMANHO - 1) cout << " | ";
        }
        cout << endl;
        cout << "\t";
        if (i < TAMANHO - 1) cout << "--+---+--" << endl;
    }
}

// Inicializar o tabuleiro vazio
void inicializarTabuleiro(JOGO& jogo) {
    for (int i = 0; i < TAMANHO; i++)
        for (int j = 0; j < TAMANHO; j++)
            jogo.tabuleiro[i][j] = ' ';
}
void jogarMaquina(JOGO& jogo) { 
    // O computador faz uma jogada aleatória numa posição vazia
    int linha, coluna;
    bool jogadaValida = false;

    while (!jogadaValida) {
        linha = rand() % TAMANHO;
        coluna = rand() % TAMANHO;

        if (jogo.tabuleiro[linha][coluna] == ' ') {
            jogo.tabuleiro[linha][coluna] = jogo.jogador2.simbolo;  // O computador será sempre o Jogador 2 por minha opção
            jogadaValida = true;
        }
    }
}
//função para ver quem vence
int verificarVencedor(JOGO& jogo) {
    // Verificar linhas
    for (int i = 0; i < TAMANHO; i++)
        if (jogo.tabuleiro[i][0] == jogo.tabuleiro[i][1] && jogo.tabuleiro[i][1] == jogo.tabuleiro[i][2] && jogo.tabuleiro[i][0] != ' ')
            return (jogo.tabuleiro[i][0] == jogo.jogador1.simbolo) ? 1 : 2;

    // Verificar colunas
    for (int i = 0; i < TAMANHO; i++)
        if (jogo.tabuleiro[0][i] == jogo.tabuleiro[1][i] && jogo.tabuleiro[1][i] == jogo.tabuleiro[2][i] && jogo.tabuleiro[0][i] != ' ')
            return (jogo.tabuleiro[0][i] == jogo.jogador1.simbolo) ? 1 : 2;

    // Verificar diagonais --> principal e secundária
    if (jogo.tabuleiro[0][0] == jogo.tabuleiro[1][1] && jogo.tabuleiro[1][1] == jogo.tabuleiro[2][2] && jogo.tabuleiro[0][0] != ' ')
        return (jogo.tabuleiro[0][0] == jogo.jogador1.simbolo) ? 1 : 2;

    if (jogo.tabuleiro[0][2] == jogo.tabuleiro[1][1] && jogo.tabuleiro[1][1] == jogo.tabuleiro[2][0] && jogo.tabuleiro[0][2] != ' ')
        return (jogo.tabuleiro[0][2] == jogo.jogador1.simbolo) ? 1 : 2;

    return 0; //se nem ganhou 1 nem 2
}
// Verificar empate --> só se nenhum ganhar
bool verificarEmpate(JOGO& jogo) {
    for (int i = 0; i < TAMANHO; i++)
        for (int j = 0; j < TAMANHO; j++)
            if (jogo.tabuleiro[i][j] == ' ') //se encontra espaço segue porque ainda não acabou
                return false;
    return true;        //se não ainda ninguem ganhou
}

// Função para processar um turno do jogo alternadamente
void vezDeJogar(JOGO& jogo,bool jogaSozinho) {  
    int linha, coluna;

    if (jogo.jogadorAtual == 1 || !jogaSozinho) {
        JOGADOR& jogadorAtual = (jogo.jogadorAtual == 1) ? jogo.jogador1 : jogo.jogador2;

        cout << "\n\t" << jogadorAtual.nome << " (" << jogadorAtual.simbolo << ") - Escolha linha (1-3) e coluna (1-3): ";
        cin >> linha >> coluna; //verifico a seguir
        linha--; coluna--;  // Ajustar para índices de matriz (0-2)

        // Verificar se a jogada é válida
        if (linha < 0 || linha >= TAMANHO || coluna < 0 || coluna >= TAMANHO || jogo.tabuleiro[linha][coluna] != ' ') {
            cout << "\n\tJogada inválida. Tente novamente." << endl;
            Sleep(500);
            vezDeJogar(jogo,jogaSozinho);
        }
        else {
            jogo.tabuleiro[linha][coluna] = jogadorAtual.simbolo;
        }
    }
    //parte de jogar contra o pc
    else if (jogo.jogadorAtual == 2 && jogaSozinho) {
        cout << "\n\tO computador (" << jogo.jogador2.simbolo << ") está a fazer uma jogada..." << endl;
        Sleep(1000);
        jogarMaquina(jogo);
    }
    
}
void inicializarJogadores(JOGO& jogo, bool jogarZozinho) { 

    system("cls");

    cout << "\n\tDigite o nome do Jogador 1: ";
    getline(cin, jogo.jogador1.nome); 

    // Jogador 1 escolhe o símbolo
    char escolha; 
    do {
        cout << "\n\tEscolha o símbolo que deseja (X/O): ";
        escolha = toupper(_getch()); // Captura a tecla pressionada e converte para maiúscula

        if (escolha == 'X' || escolha == 'O') {
            cout << escolha << endl; // Mostra a escolha feita pelo jogador
        }
    } while (escolha != 'X' && escolha != 'O'); // Repete até o jogador escolher 'X' ou 'O'

    jogo.jogador1.simbolo = escolha;

    jogo.jogador1.pontuacao = lerPontuacao(jogo.jogador1.nome + "_pontuacao.txt");
    cout << "\n\tPontuação inicial de " << jogo.jogador1.nome << ": " << jogo.jogador1.pontuacao << endl;

    // Jogador 2: Computador ou outro jogador   // Jogador 2 recebe o símbolo restante
    if (jogarZozinho) {
        jogo.jogador2.nome = "Computador";
        jogo.jogador2.simbolo = (jogo.jogador1.simbolo == 'X') ? 'O' : 'X';
    }
    else {
        cout << "\n\tDigite o nome do Jogador 2: ";
        getline(cin, jogo.jogador2.nome);
        jogo.jogador2.simbolo = (jogo.jogador1.simbolo == 'X') ? 'O' : 'X';
        jogo.jogador2.pontuacao = lerPontuacao(jogo.jogador2.nome + "_pontuacao.txt");
        cout << "\n\tPontuação inicial de " << jogo.jogador2.nome << ": " << jogo.jogador2.pontuacao << endl;
    }

    cout << "\n\tJogador 2 ficará com o símbolo: " << jogo.jogador2.simbolo << endl;

    // Verificar se a pontuação é válida
    if (jogo.jogador1.pontuacao < 0) jogo.jogador1.pontuacao = 0;
    if (jogo.jogador2.pontuacao < 0) jogo.jogador2.pontuacao = 0;

    jogo.jogadorAtual = 1;  // Começa com o Jogador 1
    cout << "\n________________________________A CARREGAR________________________________________";
    Sleep(3000);

}

// Função principal de jogo
void jogar(JOGO& jogo,bool jogarSozinho) {

    inicializarJogadores(jogo,jogarSozinho);
    do {
        system("cls");
        bool fimJogo = false;
        inicializarTabuleiro(jogo);

        while (!fimJogo) {
            system("cls");
            mostrarTabuleiro(jogo);
            vezDeJogar(jogo,jogarSozinho);

            int resultado = verificarVencedor(jogo);

            //ganhou o 1
            if (resultado == 1) {
                cout << "\n\t" << jogo.jogador1.nome << " ganhou!" << endl;
                Sleep(2000);
                jogo.jogador1.pontuacao += 10;
                gravarPontuacao(jogo.jogador1.nome + "_pontuacao.txt", jogo.jogador1.pontuacao);
                fimJogo = true;
            }
            //ganhou o 2
            else if (resultado == 2) {
                cout << "\n\t" << jogo.jogador2.nome << " ganhou!" << endl;
                Sleep(2000);
                jogo.jogador2.pontuacao += 10;
                gravarPontuacao(jogo.jogador2.nome + "_pontuacao.txt", jogo.jogador2.pontuacao);
                fimJogo = true;
            }
            //empataram
            else if (verificarEmpate(jogo)) {
                cout << "\n\tEmpate!" << endl;
                Sleep(2000);
                fimJogo = true;
            }
            else {
                //continuar a jogar alternadamente se nenhuma das anteriores
                jogo.jogadorAtual = (jogo.jogadorAtual == 1) ? 2 : 1;
            }
        }
        cout << endl;
        mostrarTabuleiro(jogo);
        cout << "\n\tPontuação de " << jogo.jogador1.nome << ": " << jogo.jogador1.pontuacao << endl;
        cout << "\n\tPontuação de " << (jogarSozinho ? "Computador" : jogo.jogador2.nome) << ": " << jogo.jogador2.pontuacao << endl;
        cout << "\n\n\tDeseja jogar mais uma vez? (S/N): ";
        cin.ignore(); 
    } while (toupper(_getch()) == 'S');
   
    
}

//função para guardar pontuação no ficheiro
void gravarPontuacao(const string& nomeFicheiro, int pontuacao) {
    ofstream ficheiro;
    ficheiro.open(nomeFicheiro);
    if (ficheiro.is_open()) {
        ficheiro << pontuacao; // Grava a pontuação
        ficheiro.close();
    }
    else {
        cout << "\n\tErro ao abrir o ficheiro para gravar a pontuação." << endl;
    }
}
// Função para ler pontuação de um ficheiro, se não existir ele cria
int lerPontuacao(const string& nomeFicheiro) { //retorna a pontuação
    ifstream ficheiro;
    ficheiro.open(nomeFicheiro);
    int pontuacao = 0;
    if (ficheiro.is_open()) {
        ficheiro >> pontuacao; // Lê a pontuação
        ficheiro.close();
    }
    else {
        cout << "\n\tFicheiro de pontuação não encontrado " << endl;
        cout << "\n\tA criar ficheiro: " << nomeFicheiro << endl;
        pontuacao = 0;
    }
    return pontuacao; // Retorna 0 se o arquivo não for aberto
}








// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
