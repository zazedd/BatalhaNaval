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

Boat p, n, c1, c2, s1, s2, err;
int wait(int isAttacking, int gaveUp);

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

Boat indiceToBoat(int indice)
{
    switch (indice)
    {
    case 0:
        return p;

    case 1:
        return n;

    case 2:
        return c1;

    case 3:
        return c2;

    case 4:
        return s1;

    case 5:
        return s2;

    default:
        return err;
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
            b->coord[i].pos.x = xy.x;
            b->coord[i].pos.y = b->coord[i - 1].pos.y + 1;
        }
    }
    else if (dir == 'V')
    {
        for (int j = 1; j < b->tSize; j++)
        {
            b->coord[j].afloat = 1;
            b->coord[j].pos.y = xy.y;
            b->coord[j].pos.x = b->coord[j - 1].pos.x + 1;
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
    if (posLivres == boat->tSize) // se o numero de pos livres fôr = ao tamanho do barco, isso significa que o barco pode ser inserido
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

    checkIfInside = (dir == 'H') ? tamanhoBarco + y1 : tamanhoBarco + x1; // se este numero, dependente da direção, fôr maior que 8 (N e M) então o barco está fora da zona de jogo

    if (tamanhoBarco == -1) // tamanho/tipo invalido
    {
        return -4;
    }
    else if (x1 >= N || y1 >= M || x1 < 0 || y1 < 0 || checkIfInside > N) // coordenadas invalidas
    {
        return -2;
    }
    else if (checkIfInside <= N) // se o barco estiver dentro do tabuleiro, podemos inseri-lo pois tudo o resto já está em ordem
    {
        board->boats[indiceBarcos] = indiceToBoat(indiceBarcos);
        board->boats[indiceBarcos].coord[0].pos.x = x1;
        board->boats[indiceBarcos].coord[0].pos.y = y1;

        init_boat(&board->boats[indiceBarcos], type, board->boats[indiceBarcos].coord[0].pos, dir);

        isFree = check_free(8, 8, &board->boats[indiceBarcos], board->board);

        if (isFree == 0)
            return -1; // posicao ocupada

        for (int i = 0; i < board->boats[indiceBarcos].tSize; i++)
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
        for (int i = 0; i < board->numBoats; i++) // iterar pelos barcos
        {
            for (int j = 0; j < board->boats[i].tSize; j++) // iterar as coordenadas
            {
                if (board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) // achar qual é o barco que a posição atingiu
                {
                    for (int k = 0; k < board->boats[i].tSize; k++)
                    {
                        if (board->boats[i].coord[k].afloat == 0) // fazer um loop sobre as posicoes todas para saber se estao todas mortas
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
    else if (board->board[x][y] == '*' || board->board[x][y] == 'A' || board->board[x][y] == 'F') // posicao ja atacada
    {
        return 0;
    }
    else // a partir daqui temos a certeza de que se trata de um barco
    {
        for (int i = 0; i < board->numBoats; i++)
        {
            for (int j = 0; j < board->boats[i].tSize; j++)
            {
                if (board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) // marcar a posicao como morta
                {
                    board->boats[i].coord[j].afloat = 0;
                    board->board[x][y] = '*';

                    if (check_sink(x, y, board) == board->boats[i].type) // afundou
                    {
                        for (int k = 0; k < board->boats[i].tSize; k++)
                        {
                            posX = board->boats[i].coord[k].pos.x;
                            posY = board->boats[i].coord[k].pos.y;

                            board->board[posX][posY] = 'A';
                        }
                        board->numBoatsAfloat--;
                        return typeToSize(board->boats[i].type);
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
 * Função para remover o "\n" do nome dos jogadores
 *
 * str: apontador para o array dos nomes
 *
 **/
void removeBreakline(char *str)
{
    int pos = strcspn(str, "\n");
    str[pos] = 0;
}

/**
 * Function: wait
 *
 * Função para esperar por input do utilizador aquando à contínuação do programa
 * Se estivermos a colocar os barcos, ou o utilizador que ataca já tiver desistido,
 * não precisamos de perguntar ao utilizador se ele quer ver a posição inicial dos
 * barcos no tabuleiro.
 * Se o utilizador escrever '0', saímos do programa como esperado.
 *
 * isAttacking: 1 se estivermos a atacar, 0 se estivermos a colocar os barcos
 * gaveUp: estado da desistência do utilizador que ataca
 *
 *
 * returns:
 *   0 se o utilizador quiser continuar o programa
 *   1 se o utilizador quiser ver os barcos na sua posição inicial
 *   (dá exit quando o input é 0)
 *
 **/
int wait(int isAttacking, int gaveUp)
{
    char input;
    if (isAttacking && !gaveUp)
    {
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
    }
    else
    {
        printf("\nPara prosseguir pressione ENTER, ou para sair do programa, 0 seguido de ENTER.\n");

        input = getchar();
        do
        {
            switch (input)
            {
            case '0':
                exit(EXIT_SUCCESS);

            case '\n':
                return 0;

            default:
                getchar(); // consumir paragrafo
                printf("Input inválido, tente novamente.\n");
                input = getchar();
                break;
            }

        } while (input != '0' || input != '\n' || input != '1');
    }

    return 0;
}

int main(void)
{
    char orientacao, nomeAtacante[100], nomeDefensor[100], playAgain;
    int indiceBarcos = 0, ataques = 40, desistencia = 0, check;

    Board brd;
    Position xy;

    init_board(N, M, &brd);

    printf("Qual é o nome do jogador Defensor? ");
    fgets(nomeDefensor, 100, stdin);
    removeBreakline(nomeDefensor);

    printf("Qual é o nome do jogador Atacante? ");
    fgets(nomeAtacante, 100, stdin);
    removeBreakline(nomeAtacante);

    do
    {
        while (indiceBarcos < 6) // comeca no 0, sao 6 barcos
        {
            if (indiceBarcos > 0)
            {
                wait(0, desistencia);
            }
            printf("\nVez do jogador: %s\n", nomeDefensor);

            printf("A inserir o barco (%c):\n", indiceToType(indiceBarcos));

            printf("Qual é a orientação do barco? (H, V)? ");
            orientacao = getchar();
            getchar(); // consumir paragrafo

            printf("Qual é a coordenada x? ");
            scanf("%d", &xy.x);

            printf("Qual é a coordenada y? ");
            scanf("%d", &xy.y);
            getchar(); // consumir paragrafo

            check = place_boat(xy.x, xy.y, orientacao, indiceToType(indiceBarcos), &brd);

            if (!check)
            {
                printf("\nNúmero de barcos: %d\n", indiceBarcos + 1);
                print_board(N, M, brd.board, 1);
                indiceBarcos++;
            }
            else
            {
                switch (check)
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
        }

        while (ataques > 0)
        {
            if (wait(1, desistencia))
            {
                getchar(); // consumir paragrafo

                printf("O JOGADOR \"%s\" GANHOU!\n\n", nomeDefensor);
                print_board(N, M, brd.board, 1);
                desistencia = 1;
                wait(0, desistencia);
            }
            printf("Vez do jogador: %s\n", nomeAtacante);
            printf("Ataques restantes = %d\n", ataques);

            print_board(N, M, brd.board, 0);

            printf("Qual é a coordenada a atacar? (x) ");
            scanf("%d", &xy.x);
            getchar(); // consumir paragrafo

            printf("Qual é a coordenada a atacar? (y) ");
            scanf("%d", &xy.y);
            getchar(); // consumir paragrafo

            check = target(xy.x, xy.y, &brd); // funcao responsavel por atacar

            switch (check)
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
                ataques--;
                printf("Acertou num barco!\n");
                break;

            case 0:
                printf("A posição já foi atacada. Tente de novo.\n");
                break;

            case -1:
                ataques--;
                printf("Falhou!\n");
                break;

            case -2:
                printf("As coordenadas são inválidas. Tente de novo.\n");
                break;

            default:
                break;
            }

            if (brd.numBoatsAfloat == 0)
            {
                break;
            }
        }

        if (brd.numBoatsAfloat != 0 || desistencia == 1)
        {
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", nomeDefensor);
            print_board(N, M, brd.board, 0);
        }
        else
        {
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", nomeAtacante);
            print_board(N, M, brd.board, 0);
        }

        printf("\nVão pretender jogar de novo? (Y/n) ");
        char playAgain = getchar();

    } while ();

    // P - 0
    // N - 1
    // C1 - 2
    // C2 - 3
    // S1 - 4
    // S2 - 5

    return 0;
}
