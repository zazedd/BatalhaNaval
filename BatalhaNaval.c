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

/**Representa um jogador. O seu nome, sua ocupação dentro do jogo e o seu numero de vitórias.**/
typedef struct
{
    char nome[100]; // Array que guarda o nome do jogador.
    int occupation; // Quando 1, o jogador ataca, quando 0 o jogador defende.
    int victories;  // Vitorias de cada jogador
} Player;

Boat b; // usado para inserir os barcos, é arbitrario e apenas representa a struct
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
                {
                    printf("   |"); // substituir os barcos por espaços na hora do printf
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
 * returns
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

/**
 * Function: indiceToType
 *
 * Dado o indice do barco devolve o seu tipo.
 *
 * indice: indice do barco
 * ('P' -> 0, 'N' -> 1, 'C1' -> 2, 'C2' -> 3, 'S' -> 4, 'S' -> 5)
 *
 * returns
 *  O tipo do barco em função do indice.
 **/
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

    // as posicoes sao iniciadas aqui, dependendo da direção do barco
    if (dir == 'H' || dir == 'h')
    {
        for (int i = 0; i < b->tSize; i++)
        {
            b->coord[i].afloat = 1;
            b->coord[i].pos.x = xy.x;     // como é horizontal, a coordenada x é sempre igual
            b->coord[i].pos.y = xy.y + i; // e coordenada y vai aumentando até chegarmos a ultima posição
        }
    }
    else if (dir == 'V' || dir == 'v')
    {
        for (int j = 0; j < b->tSize; j++)
        {
            b->coord[j].afloat = 1;
            b->coord[j].pos.x = xy.x + j; // como é vertical, a coordenada x vai aumentando até chegarmos a ultima posição
            b->coord[j].pos.y = xy.y;     // a coordenada y é sempre igual
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
        else // case contrario
        {
            return 0;
        }
    }
    if (posLivres == boat->tSize) // se o numero de pos livres == ao tamanho do barco, isso significa que o barco pode ser inserido
    {
        return 1;
    }
    else // case contrario
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
int place_boat(int x1, int y1, int dir, char type, Board *board)
{
    int indiceBarcos = 0, tamanhoBarco, isFree, checkIfInside, posX, posY;
    indiceBarcos = board->numBoats; // para readability

    if (dir != 'H' && dir != 'V' && dir != 'h' && dir != 'v')
        return -3; // direção inválida

    tamanhoBarco = typeToSize(type);

    checkIfInside = (dir == 'H' || dir == 'h') ? tamanhoBarco + y1 : tamanhoBarco + x1; // se este numero, dependente da direção, fôr maior que N ou M então o barco está fora da zona de jogo

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

        // temos todas as condicoes reunidas para meter o barco no tabuleiro
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

/**
 * Function: check_sink
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
 *   0 se a posicao já foi atacada anteriormente
 *   1 se acerta numa coordenada de um barco mas sem o afundar
 *   -1 se nao tiver sucesso
 *   -2 se a coordenada for inválida.
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

                    if (check_sink(x, y, board) == board->boats[i].type) // se afundou:
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
 * Function: clearConsole
 *
 * Usa um regex para dar clear à consola.
 *
 **/
void clearConsole()
{
    printf("\e[1;1H\e[2J");
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
    int pos = strcspn(str, "\n"); // acha a posição no array correspondente ao "\n"
    str[pos] = '\0';              // elimina-a
}

/**
 * Function: swapRole
 *
 * Esta função é responsável pela troca dos nomes das ocupações dos
 * jogadores quando estes pretendem jogar novamente.
 * O jogador que atacava passa a ser o defensor e vice-versa.
 *
 * role1: pointer para uma das ocupações que queremos trocar
 * role2: pointer para uma das ocupações que queremos trocar
 *
 **/
void swapRole(int *role1, int *role2)
{
    int temp = *role1;
    *role1 = *role2;
    *role2 = temp;
}

/**
 * Function: lerOrientacao()
 *
 * Função responsável por ler a orientação e assegurar que
 * esta está dentro dos conformes (é so uma letra).
 * Nao impomos uma restricao quanto a letra porque temos fé de que o código
 * que já implementamos nas outras funções fará o seu trabalho
 *
 * returns:
 *   ch[0]: a orientação quando esta consiste apenas de uma letra
 *
 **/
char lerOrientacao()
{
    char ch[50];
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
 * program exits:
 *  se o utilizador escrever '0'.
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
 * Função responsável por esperar por input do utilizador aquando à contínuação do programa quando ele está a atacar.
 * Se o utilizador der "ENTER", o programa continua normalmente.
 * Se o utilizador escrever '0', programa sai como esperado.
 * Se o utilizador escrever '1', é lhe mostrado o tabuleiro com os barcos e ele perde automaticamente (feito na main).
 * Outro tipo de input não esperado não é aceite e o utilizador é remetido a
 * inserir outro caracter.
 *
 * returns:
 *   0 se o utilizador continuar o programa normalmente
 *   1 se o utilizador quiser desistir
 *
 * program exits:
 *  se o utilizador escrever '0'.
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

/**
 * Function: boatPlacingLogic
 *
 * Função responsável pela lógica de inserir os barcos no tabuleiro.
 * Esta função tem varias utilidades:
 * - Informar os jogadores de quem joga de momento.
 * - Dizer quantos barcos estão no tabuleiro e quais faltam colocar.
 * - Pedir ao utilizador para os inserir, e então realizar a insersão (place_boat).
 * - Avisar o utilizador caso ele tenha feito um erro ao inserir os barcos.
 *
 * name: nome do jogador que defende
 * pos: struct posição para as coordenadas
 * board: posição atual do tabuleiro
 *
 **/
void boatPlacingLogic(char *name, Position pos, Board *board)
{
    char tipo, orientacao;
    int check;

    printf("Vez do jogador: %s\n", name);

    printf("\nNúmero de barcos: %d\n", board->numBoats);
    printf("Faltam inserir os barcos: ");
    for (int i = 0; i < B - board->numBoats; i++) // var i -> iterar pelos barcos
    {
        if (i != (B - 1) - board->numBoats)
        {
            printf("%c, ", indiceToType(board->numBoats + i)); // print ao tipo com virgula, pois nao é o ultimo da lista
        }
        else
        {
            printf("%c\n", indiceToType(board->numBoats + i)); // print ao tipo sem virgula, pois é o ultimo da lista
        }
    }

    print_board(N, M, board->board, 1);
    tipo = indiceToType(board->numBoats);
    printf("A inserir o barco (%c):\n", tipo);

    printf("Qual é a orientação do barco? (H, V)? ");
    orientacao = lerOrientacao();

    printf("Qual é a coordenada x? ");
    scanf("%d", &pos.x);

    printf("Qual é a coordenada y? ");
    scanf("%d", &pos.y);
    getchar(); // consumir paragrafo

    check = place_boat(pos.x, pos.y, orientacao, tipo, board); // função responsável pela colocação dos barcos

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

    if (board->numBoats > 0)
    {
        wait();
    }

    clearConsole();
}

/**
 * Function: attackLogic
 *
 * Função responsável pela lógica de atacar os barcos no tabuleiro.
 * Esta função tem varias utilidades:
 * - Informar os jogadores de quem joga de momento.
 * - Dizer quantos ataques o utilizador ainda dispõe.
 * - Dizer quais barcos faltam afundar.
 * - Pedir ao utilizador as coordenadas do ataque, e então realizá-lo (target).
 * - Avisar o utilizador caso ele tenha feito um erro ao inserir as coordenadas.
 *
 * gaveUp: estado da desistencia do jogador que ataca
 * attacks: numero de ataques restantes
 * name1: nome do jogador que defende
 * name2: nome do jogador que ataca
 * pos: struct posição para as coordenadas
 * board: posição atual do tabuleiro
 *
 **/
void attackLogic(int gaveUp, int *attacks, char *name1, char *name2, Position pos, Board *board)
{
    int check;

    printf("Vez do jogador: %s\n", name2);

    printf("\nAtaques restantes = %d\n", *attacks);
    printf("Faltam afundar os barcos: ");
    for (int i = 0; i < board->numBoats; i++) // var i -> iterar pelos barcos
    {
        if (board->boats[i].afloat != 0) // se o barco nao estiver afundado
        {
            if (i != board->numBoats - 1 && board->numBoatsAfloat != 1) // e se nao for o ultimo da lista
            {
                printf("%c, ", board->boats[i].type); // print ao tipo desse barco (com virgula pois ha mais barcos a seguir)
            }
            else
            {
                printf("%c\n", board->boats[i].type); // print ao tipo desse barco (sem virgula pois é o ultimo da lista)
            }
        }
    }

    print_board(N, M, board->board, 0);

    printf("Qual é a coordenada a atacar? (x) ");
    scanf("%d", &pos.x);
    getchar(); // consumir paragrafo

    printf("Qual é a coordenada a atacar? (y) ");
    scanf("%d", &pos.y);
    getchar(); // consumir paragrafo

    check = target(pos.x, pos.y, board); // funcao responsavel por atacar

    switch (check) // dar informação ao utilizador conforme o output da função target(), e retirar um ataque se este foi válido
    {
    case 5:
        printf("\nAfundou um Porta-Aviões!\n");
        *attacks -= 1;
        break;

    case 4:
        printf("\nAfundou um Navio-Tanque!\n");
        *attacks -= 1;
        break;

    case 3:
        printf("\nAfundou um Contratorpedeiro!\n");
        *attacks -= 1;
        break;

    case 2:
        printf("\nAfundou um Submarino!\n");
        *attacks -= 1;
        break;

    case 1:
        printf("\nAcertou num barco!\n");
        *attacks -= 1;
        break;

    case 0:
        printf("\nA posição já foi atacada. Tente de novo.\n");
        break;

    case -1:
        printf("\nFalhou!\n");
        *attacks -= 1;
        break;

    case -2:
        printf("\nAs coordenadas são inválidas. Tente de novo.\n");
        break;

    default:
        break;
    }

    if (!gaveUp) // se ainda nao desistiu:
    {
        if (waitAttacking()) // esperamos por input para continuacao do programa
        {
            // se o utilizador desistir:

            getchar(); // consumir paragrafo
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", name1);
            print_board(N, M, board->board, 1);
            gaveUp = 1;
            wait();
        }
    }
    else // so é executado se o utilizador já tinha desistido
    {
        wait();
    }

    clearConsole();
}

/**
 * Main function
 *
 * Função responsável pelo fluxo do programa.
 * Contém o menu inicial, o loop da colocacao dos barcos e dos ataques,
 *
 **/
int main(void)
{
    char *nomeAtacante, *nomeDefensor, playAgain;
    int ataques = 40, desistencia = 0, option = -1;

    Board brd;
    Position xy;
    Player j1, j2;
    j1.victories = j2.victories = 0;

    while (option == -1) // Menu inicial
    {
        printf("Bem vindo ao jogo da Batalha naval!\n");
        printf("\n1.\tIniciar Jogo\n");
        printf("2.\tMenu de ajuda\n");
        printf("0.\tSair\n\n");
        printf("Por favor, introduza qual a opção que pretende que o programa execute: ");
        scanf("%d", &option);
        getchar();
        switch (option)
        {
        case 0:
            printf("Aguardamos por si num próximo jogo.\n");
            return 0;
        case 1:
            printf("\nVamos dar início ao seu jogo!\n\n");
            break;
        case 2:
            clearConsole();
            printf("Inseriu o menu de ajuda!\n\n");
            printf("As regras do jogo serão as seguintes:\n- Existirão dois jogadores, um deles fazendo a colocação dos barcos e o outro posteriormente irá tentar afundá-los.\n- Existirá um total de seis barcos, com tamanhos diversos.\n");
            printf("- O tabuleiro é %dx%d. Os ataques podem ser realizados inserindo coordenadas de (0,0) a (%d,%d), sendo a coordenada x as linhas, e y as colunas.\n", N, M, N-1, M-1);
            printf("- A colocação dos barcos  poderá ser feita na horizontal, colocando H, e na vertical, colocando V.\n");
            printf("- O jogador atacante terá um total de 40 jogadas para tentar afundar todos os barcos. Se, na sua jogada acertar num barco, no tabuleiro aparecerá '*' na posição atacada. Se afundar o barco, no seu tabuleiro aparecerá as posições do barco substituídas por um 'A'.\n");
            wait();
            clearConsole();
            option = -1;
            break;
        default:
            printf("Opção Inválida! Por favor selecione outra!\n");
            getchar();
            option = -1;
            break;
        }
    }

    clearConsole();
    option = -1;

    printf("Antes de começarem o jogo, por favor jogadores, introduzam os seus nomes.\n");

    printf("Primeiro Jogador: ");
    fgets(j1.nome, 100, stdin);
    removeBreakline(j1.nome);

    printf("Segundo Jogador: ");
    fgets(j2.nome, 100, stdin);
    removeBreakline(j2.nome);

    printf("\n%s, quer colocar os barcos ou atacá-los?\nIntroduza 0 para defender e 1 para atacar: ", j1.nome);
    while (option == -1)
    {
        scanf("%d", &option);
        getchar(); // consumir paragrafo

        switch (option)
        {
        case 1: // j1 ataca
            printf("\nO jogador %s irá atacar!\n", j1.nome);
            printf("O jogador %s irá defender!\n", j2.nome);

            j1.occupation = 1;
            j2.occupation = 0;
            break;
        case 0: // j1 defende
            printf("\nO jogador %s irá atacar!\n", j2.nome);
            printf("O jogador %s irá defender!\n", j1.nome);

            j1.occupation = 0;
            j2.occupation = 1;
            break;
        default:
            printf("Assim não vamos lá... Introduza 1 se quiser atacar e 0 se quiser defender!");
            option = -1;
            break;
        }
    }

    do
    {
        init_board(N, M, &brd);

        clearConsole();

        if (j1.occupation == 0) // se j1 defende
        {
            nomeDefensor = j1.nome;
            nomeAtacante = j2.nome;
        }
        else if (j1.occupation == 1) // se j1 ataca
        {
            nomeDefensor = j2.nome;
            nomeAtacante = j1.nome;
        }

        while (brd.numBoats < B) // colocação dos barcos
        {
            boatPlacingLogic(nomeDefensor, xy, &brd);
        }

        clearConsole();

        while (ataques > 0) // logica dos ataques
        {
            attackLogic(desistencia, &ataques, nomeDefensor, nomeAtacante, xy, &brd);

            if (brd.numBoatsAfloat == 0) // se todos os barcos estiverem mortos, então o jogo acabou
            {
                break;
            }
        }

        if (brd.numBoatsAfloat != 0 || desistencia == 1) // mensagem de winner
        {
            if (j1.occupation == 0) // se era o j1 um a defender da mos lhe um ponto
            {
                j1.victories++;
            }
            else // caso contrario o ponto é do j2
            {
                j2.victories++;
            }

            clearConsole();
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", nomeDefensor);
            print_board(N, M, brd.board, 0);
        }
        else
        {
            if (j1.occupation == 1) // se era o j1 um a atacar da mos lhe um ponto
            {
                j1.victories++;
            }
            else // caso contrario o ponto é do j2
            {
                j2.victories++;
            }

            clearConsole();
            printf("\nO JOGADOR \"%s\" GANHOU!\n\n", nomeAtacante);
            print_board(N, M, brd.board, 0);
        }

        printf("\nO score é agora: %s \\ %d - %d / %s\n", j1.nome, j1.victories, j2.victories, j2.nome);

        printf("\nVão pretender jogar de novo? (Y/n) ");
        do
        {
            playAgain = getchar();

            if (playAgain == 'Y' || playAgain == 'y') // preparar o jogo para jogar outra vez
            {
                printf("\nTROCA DE JOGADORES!, %s agora vai defender!\n", nomeAtacante);
                getchar(); // consumir paragrafo
                ataques = 40;
                swapRole(&j1.occupation, &j2.occupation); // troca dos nomes, pois o jogo vai recomeçar
                wait();
            }
            else if (playAgain == 'n' || playAgain == 'N') // sair do programa
            {
                printf("Obrigado por jogar!\n");
                return 0;
            }
            else if (playAgain == '\n') // por vezes entra um '\n' por alguma razao
            {
                playAgain = '\0'; // reset playAgain
            }
            else // input inválido
            {
                printf("Input inválido! Tente de novo.");
                getchar();        // consumir paragrafo
                playAgain = '\0'; // reset playAgain
            }

        } while (playAgain == '\0'); // certificar-nos que o utilizador so escolhe 'y' ou 'n'

    } while (playAgain == 'y' || playAgain == 'Y'); // se a resposta for y, o jogo repete

    return 0;
}
