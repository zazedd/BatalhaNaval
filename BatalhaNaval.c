/**
 * @autores:
 * Nome: Leonardo Mendes Santos                   Número de Aluno: 48708
 * Nome: António Pedro Pais da Cruz               Número de Aluno: 47995
 * Nome: Rúben José Ferreira Carvalho             Número de Aluno: 46180
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B 6
#define N 8
#define M 8

/**Representa uma coordenada*/
typedef struct
{
    int x;
    int y;
} Position;

/**Associa cada coordenada do barco com um estado**/
typedef struct
{
    int afloat;   // 0 indica que está afundada, 1 indica que não
    Position pos; // Coordenada
} StateCoord;

/**Representa um barco**/
typedef struct
{
    int afloat;          // Posições que ainda não foram afundadas
    int tSize;           // Tamanho do tipo do barco
    StateCoord coord[5]; // O barco maior tem 5 coordenadas, usando o tSize garantimos que acedemos apenas às existentes
    char type;           // Caracter que representa o tipo do barco
} Boat;

/**Representa um tabuleiro**/
typedef struct
{
    int numBoatsAfloat; // Número de barcos não afundados
    int numBoats;       // Número de barcos que foram colocados
    Boat boats[B];      // Array de barcos. Cada barco é do tipo Boat.
    char board[N][M];   // Array que contém a informação de cada posição do tabuleiro
} Board;

Boat b; // usado para inserir os barcos
void wait();

/**
 * NOTA IMPORTANTE:
 * Não pode alterar nenhuma das assinaturas das funções abaixo.
 * Este ficheiro tem de ser considerado em conjunção com o enunciado do TP2.
 **/

/**
 * Function: init_board
 *
 * Inicializa um Board.
 *
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * b: apontador para o Board que vai ser inicializado
 *
 **/
void init_board(int n, int m, Board *b)
{
    b->numBoatsAfloat = 0;
    b->numBoats = 0;

    // Inicializa o array board
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            b->board[i][j] = ' ';
        }
    }
}

/**
 * Function: print_board
 *
 * Mostra no ecrã o estado atual do tabuleiro.
 * Se a flag é 0 (zero) não mostra os barcos, se for 1, mostra.
 * Quando a flag é 1, para indicar a posição de cada barco, é colocada em todas
 * as posições do barco a letra que representa o seu tipo.
 *
 * n: número de linhas do tabuleiro
 * m: número de colunas do tabuleiro
 * board: estado do tabuleiro
 * flag: indicador do modo de visualização do tabuleiro -- se for 0 (zero) não
 *       mostra os barcos; se for diferente de 0 (zero) mostra.
 */

void print_board(int n, int m, char board[n][m], int flag)
{
    printf("+");
    for (int j = 0; j < n; j++)
    {
        printf("---+");
    }
    printf("\n");

    for (int i = 0; i < m; i++)
    {
        printf("|");
        if (flag) // Mostra tudo
        {
            for (int j = 0; j < n; j++)
            {
                printf(" %c |", board[i][j]);
            }
            printf("\n");
        }
        else // Esconde os barcos
        {
            for (int j = 0; j < n; j++)
            {
                if (board[i][j] == 'P' || board[i][j] == 'N' || board[i][j] == 'C' || board[i][j] == 'S')
                { // substituir os barcos por espaços, deixa o resto como está
                    printf("   |");
                }
                else
                {
                    printf(" %c |", board[i][j]);
                }
            }
            printf("\n");
        }

        printf("+");
        for (int j = 0; j < n; j++)
        {
            printf("---+");
        }
        printf("\n");
    }
}

/**
 * Function: typeToSize
 *
 * Dado o tipo do barco devolve o seu tamanho.
 *
 * type: tipo do barco ('P', 'N', 'C', ou 'S')
 *
 * return
 *  -1 se o tipo de barco for inválido
 *  caso contrário, devolve o tamanho do barco
 **/
int typeToSize(char type)
{
    switch (type)
    {
    case 'P':
        return 5;

    case 'N':
        return 4;

    case 'C':
        return 3;

    case 'S':
        return 2;

    default:
        return -1;
    }
}

char indiceToType(int indice)
{
    switch (indice)
    {
    case 0:
        return 'P';

    case 1:
        return 'N';

    case 2:
        return 'C';

    case 3:
        return 'C';

    case 4:
        return 'S';

    case 5:
        return 'S';

    default:
        return '0';
    }
}

/**
 * Function: init_boat
 *
 * Dado um apontador para um barco, o tipo, a posição inicial e a direcção,
 * inicializa o barco com todas as sua posições, tamanho e tipo.
 * Por exemplo, se o tipo do barco é 'C' (contratorpedeiro),
 * a posição inicial (1,1), e a direcção 'H', então o barco fica inicializado
 * com os seguintes valores
 *
 * afloat = 3;
 * tSize = 3;
 * coord = [(1,(1,1)),(1,(1,2)),(1,(1,3))]; Onde (s,(x,y)) representa um elemento do tipo StateCoord
 * type = 'C';
 *
 * b: barco que vai ser inicializado
 * type: tipo do barco
 * xy: posição inicial do barco
 * dir: direcção do barco
 *
 **/
void init_boat(Boat *b, char type, Position xy, char dir)
{

    b->tSize = typeToSize(type);
    b->afloat = b->tSize; // iguala o numero de posicoes vivas ao numero total de posicoes, pois o barco acabou de ser inicializado

    b->type = type;

    // insere as coordenadas iniciais dadas pelo user no struct coord do barco e define essa posicao como viva
    b->coord[0].afloat = 1;
    b->coord[0].pos.x = xy.x;
    b->coord[0].pos.y = xy.y;

    // o resto das posicoes sao iniciadas aqui, dependendo da direção do barco
    if (dir == 'H')
    {
        for (int i = 1; i < b->tSize; i++)
        {
            b->coord[i].afloat = 1;
            b->coord[i].pos.x = xy.x;                      // como é horizontal, a coordenada x é sempre igual
            b->coord[i].pos.y = b->coord[i - 1].pos.y + 1; // e coordenada y vai aumentando até chegarmos a ultima posição
        }
    }
    else if (dir == 'V')
    {
        for (int j = 1; j < b->tSize; j++)
        {
            b->coord[j].afloat = 1;
            b->coord[j].pos.y = xy.y;                      // como é vertical, a coordenada y é sempre igual
            b->coord[j].pos.x = b->coord[j - 1].pos.x + 1; // e coordenada x vai aumentando até chegarmos a ultima posição
        }
    }
}

/**
 * Function: check_free
 *
 * Verifica se estão livres todas as posições que serão ocupadas por um
 * barco no tabuleiro.
 *
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * boat: barco a colocar no tabuleiro
 * board: estado atual do tabuleiro
 *
 * returns:
 *  1 se todas as posições estão livres
 *  0 caso contrário
 *
 **/
int check_free(int n, int m, Boat *boat, char board[n][m])
{

    int posLivres = 0, posX, posY;
    for (int i = 0; i < boat->tSize; i++)
    {
        // este passo é apenas para readability
        posX = boat->coord[i].pos.x;
        posY = boat->coord[i].pos.y;

        if (board[posX][posY] == ' ') // se houver um espaço na posicao quer dizer que esta esta livre
        {
            posLivres++;
        }
        else
        {
            return 0;
        }
    }
    if (posLivres == boat->tSize) // se o numero de pos livres == ao tamanho do barco, isso significa que o barco pode ser inserido
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 * Function: place_boat
 *
 * Cria e coloca, se tal for possível, um barco no tabuleiro.
 *
 * x1: coordenada x inicial do barco
 * y1: coordenada y inicial do barco
 * dir: dirrecção do barco ('H' ou 'V')
 * type: o tipo de barco
 * board: estado atual do tabuleiro
 *
 * returns:
 * 0 se o barco for colocado com sucesso.
 * -1 se alguma das posições já está ocupada.
 * -2 se as coordenadas forem inválidas.
 * -3 se a direcção for inválida.
 * -4 se o tipo de barco for inválido.
 **/
int place_boat(int x1, int y1, char dir, char type, Board *board)
{
    int indiceBarcos = 0, tamanhoBarco, isFree, checkIfInside, posX, posY;
    indiceBarcos = board->numBoats; // para readability

    if (dir != 'H' && dir != 'V')
        return -3; // direção inválida

    tamanhoBarco = typeToSize(type);

    checkIfInside = (dir == 'H') ? tamanhoBarco + y1 : tamanhoBarco + x1; // se este numero, dependente da direção, fôr maior que N ou M então o barco está fora da zona de jogo

    if (tamanhoBarco == -1 || indiceBarcos > 5) // tamanho/tipo invalido
    {
        return -4;
    }
    else if (x1 >= N || y1 >= M || x1 < 0 || y1 < 0 || checkIfInside > N) // coordenadas invalidas
    {
        return -2;
    }
    else // podemos começar a inseri-lo pois tudo o resto já está em ordem (menos se a posição está livre)
    {
        board->boats[indiceBarcos] = b;
        board->boats[indiceBarcos].coord[0].pos.x = x1;
        board->boats[indiceBarcos].coord[0].pos.y = y1;

        init_boat(&board->boats[indiceBarcos], type, board->boats[indiceBarcos].coord[0].pos, dir);

        isFree = check_free(N, M, &board->boats[indiceBarcos], board->board);

        if (isFree == 0)
            return -1; // posicao ocupada

        for (int i = 0; i < board->boats[indiceBarcos].tSize; i++) // var i -> iterar pelos barcos
        {
            posX = board->boats[indiceBarcos].coord[i].pos.x; // readability
            posY = board->boats[indiceBarcos].coord[i].pos.y;

            board->board[posX][posY] = type; // inserir o barco no tabuleiro
        }

        board->numBoats++;
        board->numBoatsAfloat++;

        return 0;
    }

    return 1;
}

/*
 * @brief
 *
 *
 *
 * Verifica se ao atacar a posição (x,y) algum barco é afundado.
 *
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 *
 * returns:
 *   'F' se não for afundado nenhum barco.
 *   Se afundar um barco devolve a letra que representa o seu tipo: 'P', 'N', 'C' ou 'S'
 *   'I' se a coordenada for inválida.
 *
 **/
char check_sink(int x, int y, Board *board)
{
    int posMorta = 0;

    if (x < 0 || y < 0 || x >= N || y >= M) // coordenada invalida
    {
        return 'I';
    }
    else
    {
        for (int i = 0; i < board->numBoats; i++) // var i -> iterar pelos barcos
        {
            for (int j = 0; j < board->boats[i].tSize; j++) // var j -> iterar as coordenadas
            {
                if (board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) // achar qual é o barco que a posição atingiu
                {
                    for (int k = 0; k < board->boats[i].tSize; k++)
                    {
                        if (board->boats[i].coord[k].afloat == 0) // fazer um loop sobre as posicoes todas do barco para saber quantas estão mortas
                        {
                            posMorta++;
                        }
                        else
                            return 'F';
                    }

                    if (posMorta == board->boats[i].tSize) // se elas estiverem todas mortas:
                    {
                        board->boats[i].afloat = 0;  // o barco afunda
                        return board->boats[i].type; // e devolvemos a letra correspondente ao barco em questão
                    }
                    else
                        return 'F';
                }
            }
        }
    }

    return '0';
}

/**
 * Function: target
 *
 * Ataca a coordenada (x,y) no tabuleiro.
 * Se a coordenada (x,y) é parte de um barco, então coloca um '*' nessa
 * posição '*'; caso contrário coloca a letra 'F' nessa posição; caso afunde
 * um barco coloca em todas as posições desse barco a letra 'A'.
 *
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 *
 * returns:
 *   0 se a posicao já foi atacada anteriormente x
 *   1 se acerta numa coordenada de um barco mas sem o afundar
 *   -1 se nao tiver sucesso x
 *   -2 se a coordenada for inválida. x
 *   caso afunde um barco, devolve o número correspondente ao tamanho do barco
 *   que afundou (valor de 2 a 5)
 *
 **/
int target(int x, int y, Board *board)
{
    int posX, posY;

    if (x < 0 || y < 0 || x >= N || y >= M) // coordenada invalida
    {
        return -2;
    }
    else if (board->board[x][y] == ' ') // falhou
    {
        board->board[x][y] = 'F';
        return -1;
    }
    else if (board->board[x][y] == '*' || board->board[x][y] == 'A' || board->board[x][y] == 'F') // posicao já atacada
    {
        return 0;
    }
    else // a partir daqui temos a certeza de que se trata de um barco
    {
        for (int i = 0; i < board->numBoats; i++) // var i -> iterar pelos barcos
        {
            for (int j = 0; j < board->boats[i].tSize; j++) // var j -> iterar as coordenadas
            {
                if (board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) // se acertou num barco:
                {
                    board->boats[i].coord[j].afloat = 0; // marcamos a posição como morta
                    board->board[x][y] = '*';

                    if (check_sink(x, y, board) == board->boats[i].type) // se afundou
                    {
                        for (int k = 0; k < board->boats[i].tSize; k++)
                        {
                            posX = board->boats[i].coord[k].pos.x;
                            posY = board->boats[i].coord[k].pos.y; // readability

                            board->board[posX][posY] = 'A'; // marcamos todas as suas posições com 'A'
                        }
                        board->numBoatsAfloat--;                 // tiramos um barco vivo do tabuleiro
                        return typeToSize(board->boats[i].type); // e devolvemos o numero correspondente ao tipo do barco
                    }
                }
            }
        }

        return 1; // acertou
    }

    return -3;
}

/**
 * Function: removeBreakline
 *
 * Função para remover o "\n" duma string
 *
 * str: apontador para a string
 *
 **/
void removeBreakline(char *str)
{
    int pos = strcspn(str, "\n");
    str[pos] = 0;
}

/**
 * Function: swapPlayer
 *
 * Esta função é responsável pela troca dos nomes dos
 * jogadores quando estes pretendem jogar novamente.
 * O jogador que atacava passa a ser o defensor e vice-versa.
 *
 * nome1: pointer para um dos nomes que queremos trocar
 * nome2: pointer para um dos nomes que queremos trocar
 *
 **/
void swapPlayer(char *nome1, char *nome2)
{
    char temp[100];
    strcpy(temp, nome1);
    strcpy(nome1, nome2);
    strcpy(nome2, temp);
}

 /**
 * Function: lerOrientacao()
 *
 * Função responsável por ler a orientação e assegurar que 
 * esta está dentro dos conformes (é so uma letra).
 * 
 * returns:
 *   ch[0]: a orientação quando esta consiste apenas de uma letra 
 *
 **/
char lerOrientacao()
{
    char ch[50;
    fgets(ch, 50, stdin);
    removeBreakline(ch);

    while (strlen(ch) != 1)
    {
        printf("A orientação introduzida não é válida! Tente de novo. (H, V) ");
        fgets(ch, 50, stdin);
        removeBreakline(ch);
    }

    return ch[0];
}

/**
 * Function: wait
 *
 * Função responsável por esperar por input do utilizador aquando à contínuação do programa.
 * Se o utilizador der "ENTER", o programa continua normalmente.
 * Se o utilizador escrever '0', programa sai como esperado.
 * Outro tipo de input não esperado não é aceite e o utilizador é remetido a
 * inserir outro caracter.
 *
 **/
void wait()
{
    char input;
    printf("\nPara prosseguir pressione ENTER, ou para sair do programa, 0 seguido de ENTER.\n");

    do
    {
        input = getchar();
        switch (input)
        {
        case '0':
            exit(EXIT_SUCCESS);

        case '\n':
            break;

        default:
            getchar(); // consumir paragrafo
            printf("Input inválido, tente novamente.\n");
            input = '\0';
            break;
        }

    } while (input == '\0');
}

/**
 * Function: waitAttacking
 *
 * Função responsável por esperar por input do utilizador aquando à contínuação do programa quando este está a atacar.
 * Se o utilizador der "ENTER", o programa continua normalmente.
 * Se o utilizador escrever '0', programa sai como esperado.
 * Se o utilizador escrever '1', é lhe mostrado o tabuleiro com os barcos e ele perde automaticamente (feito na main).
 * Outro tipo de input não esperado não é aceite e o utilizador é remetido a
 * inserir outro caracter.
 *
 * returns:
 *   0 se o utilizador continuar o programa normalmente
 *   1 se o utilizador quiser desistir
 **/
int waitAttacking()
{
    char input;
    printf("\nPara prosseguir com o ataque pressione ENTER, 1 seguido de ENTER para mostrar os barcos (IRÁ PERDER AUTOMATICAMENTE!), 0 seguido de ENTER para sair do programa.\n");

    input = getchar();
    do
    {
        switch (input)
        {
        case '0':
            exit(EXIT_SUCCESS);

        case '\n':
            return 0;

        case '1':
            return 1;

        default:
            getchar(); // consumir paragrafo
            printf("Input inválido, tente novamente.\n");
            input = getchar();
            break;
        }

    } while (input != '0' || input != '\n' || input != '1');

    return 0;
}

int main(void)
{
    char jogador1[100], jogador2[100], playAgain, orientacao, tipo;
    int score[2], ataques = 40, desistencia = 0, check, option = 0;

    Board brd;
    Position xy;

    while (option != 1) // Menu inicial:
    {
        printf("Por favor, introduza qual a opção que pretende que o programa execute:\n");
        printf("1.\tIniciar Jogo\n");
        printf("2.\tMenu de ajuda\n");
        printf("0.\tSair\n");
        scanf("%d", &option);
        getchar();

        switch (option)
        {
        case 0:
            printf("Adeus! Obrigado por jogar!\n");
            return 0;

        case 1:
            break;

        case 2:
            printf("Inseriu o menu de ajuda!");
            wait(0, 0);
            break;

        default:
            printf("Opção Inválida! Por favor, tente de novo.\n\n");
            break;
        }
    }

    printf("\nQual é o nome do jogador Defensor? ");
    fgets(jogador2, 100, stdin);
    removeBreakline(jogador2);

    printf("Qual é o nome do jogador Atacante? ");
    fgets(jogador1, 100, stdin);
    removeBreakline(jogador1);

    do
    {
        init_board(N, M, &brd);

        while (brd.numBoats < B) // colocação dos barcos
        {
            if (brd.numBoats > 0)
            {
                wait();
            }

            printf("\nVez do jogador: %s\n", jogador2);
            printf("Faltam inserir os barcos: ");

            for (int i = 0; i < B - brd.numBoats; i++) // var i -> iterar pelos barcos
            {
                if (i != (B-1) - brd.numBoats)
                {
                    printf("%c, ", indiceToType(brd.numBoats + i)); // print ao tipo com virgula, pois nao é o ultimo da lista
                }
                else
                {
                    printf("%c\n", indiceToType(brd.numBoats + i)); // print ao tipo sem virgula, pois é o ultimo da lista
                }
            }

            tipo = indiceToType(brd.numBoats);
            printf("A inserir o barco (%c):\n", tipo);

            printf("Qual é a orientação do barco? (H, V)? ");
            orientacao = lerOrientacao();

            printf("Qual é a coordenada x? ");
            scanf("%d", &xy.x);

            printf("Qual é a coordenada y? ");
            scanf("%d", &xy.y);
            getchar(); // consumir paragrafo

            check = place_boat(xy.x, xy.y, orientacao, tipo, &brd); // função responsável pela colocação dos barcos

            if (check == 0)
            {
                printf("\nNúmero de barcos: %d\n", brd.numBoats);
                print_board(N, M, brd.board, 1);
            }
            else
            {
                switch (check) // dar informação relevante do erro ocorrido ao colocar o barco
                {
                case -1:
                    printf("\nA posição está ocupada. Tente de novo.\n");
                    break;

                case -2:
                    printf("\nAs coordenadas são inválidas. Tente de novo.\n");
                    break;

                case -3:
                    printf("\nA direção é inválida. Tente de novo.\n");
                    break;

                case -4:
                    printf("\nO tamanho do barco (tipo do barco) é inválido. Tente de novo.\n");
                    break;

                default:
                    break;
                }
            }
        } // acaba aqui o while da colocação dos barcos

        while (ataques > 0) // logica dos ataques
        {
            if (!desistencia) // se ainda nao desistiu:
            {   
                if (waitAttacking()) // esperamos por input, se o if for true, o utilizador desistiu.
                {
                    // se o utilizador desistir:

                    getchar(); // consumir paragrafo
                    printf("O JOGADOR \"%s\" GANHOU!\n\n", jogador2);
                    print_board(N, M, brd.board, 1);
                    desistencia = 1;
                    wait();
                }
            }
            else // so é executado se o utilizador desistiu
            {
                wait();
            }

            printf("Vez do jogador: %s\n", jogador1);
            printf("Ataques restantes = %d\n", ataques);

            printf("Faltam afundar os barcos: ");

            for (int i = 0; i < brd.numBoats; i++) // var i -> iterar pelos barcos
            {
                if (brd.boats[i].afloat != 0) // se o barco nao estiver afundado
                {
                    if (i != brd.numBoats - 1) // e se nao for o ultimo da lista
                    {
                        printf("%c, ", brd.boats[i].type); // print ao tipo desse barco (com virgula pois ha mais barcos a seguir)
                    }
                    else
                    {
                        printf("%c\n", brd.boats[i].type); // print ao tipo desse barco (sem virgula pois é o ultimo da lista)
                    }
                }
            }

            print_board(N, M, brd.board, 0);

            printf("Qual é a coordenada a atacar? (x) ");
            scanf("%d", &xy.x);
            getchar(); // consumir paragrafo

            printf("Qual é a coordenada a atacar? (y) ");
            scanf("%d", &xy.y);
            getchar(); // consumir paragrafo

            check = target(xy.x, xy.y, &brd); // funcao responsavel por atacar

            switch (check) // dar informação ao utilizador conforme o output da função target(), e retirar um ataque se este foi válido
            {
            case 5:
                printf("\nAfundou um Porta-Aviões!\n");
                ataques--;
                break;

            case 4:
                printf("\nAfundou um Navio-Tanque!\n");
                ataques--;
                break;

            case 3:
                printf("\nAfundou um Contratorpedeiro!\n");
                ataques--;
                break;

            case 2:
                printf("\nAfundou um Submarino!\n");
                ataques--;
                break;

            case 1:
                printf("\nAcertou num barco!\n");
                ataques--;
                break;

            case 0:
                printf("\nA posição já foi atacada. Tente de novo.\n");
                break;

            case -1:
                printf("\nFalhou!\n");
                ataques--;
                break;

            case -2:
                printf("\nAs coordenadas são inválidas. Tente de novo.\n");
                break;

            default:
                break;
            }

            if (brd.numBoatsAfloat == 0) // se todos os barcos estiverem mortos, então o jogo acabou
            {
                break;
            }
        } // acaba aqui o while ataques

        if (brd.numBoatsAfloat != 0 || desistencia == 1) // mensagem de winner
        {
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", jogador2);
            print_board(N, M, brd.board, 0);
        }
        else
        {
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", jogador1);
            print_board(N, M, brd.board, 0);
        }

        printf("\nVão pretender jogar de novo? (Y/n) ");
        do // certificar-nos que o utilizador so escolhe 'y' ou 'n'
        {
            getchar(); // consumir paragrafo
            playAgain = getchar();

            if (playAgain == 'Y' || playAgain == 'y')
            {
                swapPlayer(jogador1, jogador2); // troca dos nomes, pois o jogo vai recomeçar
            }
            else if (playAgain == 'n' || playAgain == 'N')
            {
                printf("Obrigado por jogar!\n");
                return 0;
            }
            else
            {
                printf("Input inválido! Tente de novo.");
                playAgain = '\0';
            }

        } while (playAgain == '\0');

    } while (playAgain == 'y' || playAgain == 'Y'); // se a resposta for y, o jogo repete

    return 0;
}
