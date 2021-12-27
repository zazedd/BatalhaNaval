/**
 * @autores:
 * Nome: Leonardo Mendes Santos                   Número de Aluno: 48708
 * Nome: António Pedro Pais da Cruz               Número de Aluno: 47995
 * Nome: Rúben José Ferreira Carvalho             Número de Aluno: 46180
 **/ 

#include <stdio.h>

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
    int afloat;        //0 indica que está afundada, 1 indica que não
    Position pos;      //Coordenada
} StateCoord;

/**Representa um barco**/
typedef struct
{
    int afloat;        //Posições que ainda não foram afundadas
    int tSize;         //Tamanho do tipo do barco 
    StateCoord coord[5]; //O barco maior tem 5 coordenadas, usando o tSize garantimos que acedemos apenas às existentes
    char type;         //Caracter que representa o tipo do barco
} Boat;

/**Representa um tabuleiro**/
typedef struct
{
    int numBoatsAfloat; //Número de barcos não afundados
    int numBoats; //Número de barcos que foram colocados
    Boat boats[B]; //Array de barcos. Cada barco é do tipo Boat.
    char board[N][M]; //Array que contém a informação de cada posição do tabuleiro
} Board;


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

    //Inicializa o array board
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
        if (flag) //Mostra tudo
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
                printf("   |");
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
            break;
        case 'N': 
            return 4;
            break;
        case 'C':
            return 3;
            break;
        case 'S':
            return 2;
            break;
        default:
            return -1;
            break;
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
    b->afloat = b->tSize;

    b->type = type;

    b->coord->afloat = 1;
    b->coord[0].pos.x = xy.x;
    b->coord[0].pos.y = xy.y;

    if (dir == 'H')
    {
        for (int i = 1; i < b->tSize; i++)
        {
            b->coord[i].pos.x = xy.x;
            b->coord[i].pos.y = b->coord[i-1].pos.y + 1;
        }
    }
    else if (dir == 'V')
    {
        for (int j = 1; j < b->tSize; j++)
        {
            b->coord[j].pos.y = xy.y;
            b->coord[j].pos.x = b->coord[j-1].pos.x + 1;
        }
    }

    for (int i = 0; i < b->tSize; i++)
    {
        printf("(%d, %d), ", b->coord[i].pos.x, b->coord[i].pos.y);
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
    int controlador = 0;
    for (int i = 0; i < boat->tSize; i++)
    {
       if (board[boat->coord[i].pos.x][boat->coord[i].pos.y] == ' ')
       {
           controlador++;
       }
       else
       {
           return 0;
       }
    }
    if (controlador == boat->tSize)
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
    Position pos;
    int numeroBarcos = 0;
    pos.x = x1; 
    pos.y = y1;
    printf("aaaaaa");

    numeroBarcos = board->numBoats;

    if (check_free(8, 8, &board->boats[numeroBarcos], board->board) == 1 && (x1 < 8 && y1 < 8) && (x1 > 0 && y1 > 0) && (dir == 'H' || dir == 'V') && typeToSize(type) > 0)
    {
        printf("aaaaaa");
        if (dir == 'H')
        {
            switch (type)
            {
                case 'P': 
                    init_boat(&board->boats[numeroBarcos], 'P', pos, 'H');
                    board->numBoats++;
                    break;
                case 'N': 
                    init_boat(&board->boats[numeroBarcos], 'N', pos, 'H');
                    board->numBoats++;
                    break;
                case 'C':
                    init_boat(&board->boats[numeroBarcos], 'C', pos, 'H');
                    board->numBoats++;
                    break;
                case 'S':
                    init_boat(&board->boats[numeroBarcos], 'S', pos, 'H');
                    board->numBoats++;
                    break;
                default:
                    break;
            }
        }
        else if ( dir == 'V')
        {
            switch (type)
            {
                case 'P': 
                    init_boat(&board->boats[board->numBoats], 'P', pos, 'V');
                    board->numBoats++;
                    break;
                case 'N': 
                    init_boat(&board->boats[board->numBoats], 'N', pos, 'V');
                    board->numBoats++;
                    break;
                case 'C':
                    init_boat(&board->boats[board->numBoats], 'C', pos, 'V');
                    board->numBoats++;
                    break;
                case 'S':
                    init_boat(&board->boats[board->numBoats], 'S', pos, 'V');
                    board->numBoats++;
                    break;
                default:
                    break;
            }
        }
        return 0;
    }
    else if (check_free(8, 8, &board->boats[1], board->board) == 0)
    {
        return -1;
    }
    else if (x1 > 8 || y1 > 8 || x1 < 0 || y1 < 0)
    {
        return -2;
    }
    else if (dir != 'H' || dir != 'V')
    {
        return -3;
    }
    else if (typeToSize(type) == -1)
    {
        return -4;
    }
    return 0;
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
    //Implementar

    return 'O';
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
    //Implementar

    return -3;
}

//int colocaNavio()
int main(void)
{

    Board brd;
    Boat a;
    Position xy;
    init_board(N, M, &brd);
    print_board(N, M, brd.board, 0);

    brd.numBoats = 0;

    xy.x = 1;
    xy.y = 2;

    init_boat(&a, 'C', xy, 'V');

    printf("\n%d\n", check_free(8, 8, &a, brd.board));

    place_boat(2, 1, 'H', 'P', &brd);

    
    
    /**Exemplo de uso da print_board e da place_boat**/
    /**Precisa de as implementar primeiro**/
    //print_board(N, M, brd.board, 0);
    //place_boat(1,3, 'H', 'P', &brd);
    
    return 0;
}
