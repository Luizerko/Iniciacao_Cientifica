#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Jogo da velha implementado em C.

Tabuleiro numa Matriz   -> (-1) vazio
                        -> 0 jogador "O"
                        -> 1 jogador "X"
*/

/*Devolve 1 se estamos num estado terminal e o jogador "X" ganhou, 0 se estamos num estado terminal e o jogador "O" ganhou,
-1 se o jogo empatou e -2 se não é um estado terminal.*/
int terminal_state(int** tab) {
    
    int ganhou;

    //Checa se alguém ganhou em linha
    for(int i = 0; i < 3; i++) {
        if(tab[i][0] != -1) {
            int val = tab[i][0];
            ganhou = val;
            for(int j = 1; j < 3; j++) {
                if(tab[i][j] != val) ganhou = -2;
            }
            if(ganhou == val) return ganhou;
        }
    }

    //Checa se alguém ganhou em coluna
    for(int j = 0; j < 3; j++) {
        if(tab[0][j] != -1) {
            int val = tab[0][j];
            ganhou = val;
            for(int i = 1; i < 3; i++) {
                if(tab[i][j] != val) ganhou = -2;
            }
            if(ganhou == val) return ganhou;
        }
    }

    //Checa se alguém ganhou em diagonal
    if(tab[0][0] != -1) {
        int val = tab[0][0];
        ganhou = val;
        for(int i = 1; i < 3; i++){
            if(tab[i][i] != val) ganhou = -2;
        }
        if(ganhou == val) return ganhou;
    }
    if(tab[0][2] != -1) {
        int val = tab[0][2];
        ganhou = val;
        for(int i = 1; i < 3; i++){
            if(tab[i][2-i] != val) ganhou = -2;
        }
        if(ganhou == val) return ganhou;
    }

    //Checa se o jogo não tem vencedores e tem espaços vazios - estado não terminal
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(tab[i][j] == -1) return (-2);
        }
    }

    //Jogo empatou
    return -1;

}

//Retorna 1 se o jogador 1 tem um h sem traço no tabuleiro. Retorna 0 caso contrário
int h_sem_traco(int** tab) {

    //Checa h sem traço superior
    if(tab[0][0] == 1 && tab[0][0] == tab[1][0])
        if(tab[0][2] == 1 && tab[0][2] == tab[1][2])
            return 1;

    //Checa h sem traço inferior
    if(tab[1][0] == 1 && tab[1][0] == tab[2][0])
        if(tab[1][2] == 1 && tab[1][2] == tab[2][2])
            return 1;

    //Checa h sem traço esquerdo
    if(tab[0][0] == 1 && tab[0][0] == tab[0][1])
        if(tab[2][0] == 1 && tab[2][0] == tab[2][1])
            return 1;

    //Checa h sem traço direito
    if(tab[0][1] == 1 && tab[0][1] == tab[0][2])
        if(tab[2][1] == 1 && tab[2][1] == tab[2][2])
            return 1;

    return 0;

}

//Devolve a probabilidade de jogador 1 ("X") não perder o jogo dado h sem traço para o mesmo baseado nos exemplos de cenas construídos
double h_sem_traco_e_not_jog_1_prob() {

    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int jog_1, jog_2, h_sem_traco;
    int total = 0, contador = 0;

    int f = fscanf(cenas, "(%d, %d, %d)\n", &jog_1, &jog_2, &h_sem_traco);
    if(h_sem_traco == 1) {
        if(jog_1 == 0 && jog_2 == 0)
            contador++;
        total++;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d)\n", &jog_1, &jog_2, &h_sem_traco);
        if(h_sem_traco == 1) {
            if(jog_1 == 0 && jog_2 == 0)
                contador++;
            total++;
        }
    }

    fclose(cenas);

    if(total == 0) return -1;

    return ((double)contador/(double)total);

}

//Constroi cenas indicando se o jogador 1 ("X") vence ou perde, se o jogador 2 ("O") vence ou perde e se o jogador 1 tinha h sem traço naquele estado terminal
void constroi_cena(int jog_1, int jog_2, int h_sem_traco) {


    FILE* cenas = fopen("cenas_sem_tab.txt", "a+");
    fprintf(cenas, "(%d, %d, %d)\n", jog_1, jog_2, h_sem_traco);
    fclose(cenas);

}

/*Simula o jogo rodando todas as possíveis interações entre os jogadores num tabuleiro de jogo da velha a partir de um certo ponto inicial do tabuleiro
e contando todas as vitórias ou empates do jogador 1 ("X") a partir de tal configuração*/
void simula_jogo_a_partir(int jogador, int** tab, int* conta_terminal_aux, int* jog_1_aux) {

    int terminal = terminal_state(tab);

    if(terminal != -2) {

        //Conta as vitórias ou empates de jogador 1 ("X")
        if(terminal == 1 || terminal == -1)
            (*jog_1_aux) += 1;
        (*conta_terminal_aux) += 1;

    }

    else {

        //Simula todas as possibilidades de jogo
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(tab[i][j] == -1) {
                    tab[i][j] = jogador;
                    simula_jogo_a_partir(1-jogador, tab, conta_terminal_aux, jog_1_aux);
                    tab[i][j] = -1;
                }
            }
        }

    }

}

//Simula o jogo rodando todas as possíveis interações entre os jogadores num tabuleiro de jogo da velha
void simula_jogo(int jogador, int** tab, int* conta_h, int* total_h) {

    int terminal = terminal_state(tab);

    if(terminal != -2) {

        int arg_1 = 0, arg_2 = 0, arg_3 = 0;

        if(terminal >= 0)

            //Jogador 1 ("X") ganhou 
            if(terminal) 
                arg_1 = 1;

            //Jogador 2 ("O") ganhou
            else
                arg_2 = 1;

        //Testa se tabuleiro tem h_sem_traco de jogador 1 ("X")
        if(h_sem_traco(tab))
            arg_3 = 1;

        constroi_cena(arg_1, arg_2, arg_3);

    }

    else {

        //Testa jogos em que o tabuleiro encontra-se em h sem traço para o jogador 1 ("X") e é vez do jogador 2 ("O")
        int r = 1;
        int jog_1_aux = 0; int conta_terminal_aux = 0;

        if(h_sem_traco(tab) && jogador == 0)
            r = 0;

        if(r == 0) {

            //Cria tabuleiro auxiliar
            int** tab_aux = malloc(3*sizeof(int*));
            for(int i = 0; i < 3; i++) {
                tab_aux[i] = malloc(3*sizeof(int));
            }
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    tab_aux[i][j] = tab[i][j];
                }
            }

            /*Simula jogo a partir de um estado de h sem traço para o jogador 1 ("X") na vez do jogador 2 ("O") e incremente contagem total
            de casos de jogador um não perdendo e estados h sem traço para o jogador 1 respectivamente*/
            simula_jogo_a_partir(jogador, tab_aux, &conta_terminal_aux, &jog_1_aux);
            (*conta_h) += jog_1_aux;
            (*total_h) += conta_terminal_aux;

            //Imprime o tabuleiro a ser analisado
            printf("\n");
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    printf(" %d ", tab[i][j]);
                }
                printf("\n");
            }

            printf("O jogador 1 não perde esse tabuleiro com probabilidade %.4f\n", (float)jog_1_aux/(float)conta_terminal_aux);
            printf("O jogador 1 não perde com probabilidade %.4f\n", (float)(*conta_h)/(float)(*total_h));
            printf("O número total de casos analisados com h sem traço é: %d\n", (*total_h));

            for(int i = 0; i < 3; i++) {
                free(tab_aux[i]);
            }
            free(tab_aux);

        }

        //Simula todas as possibilidades de jogo
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(tab[i][j] == -1) {
                    tab[i][j] = jogador;
                    simula_jogo(1-jogador, tab, conta_h, total_h);
                    tab[i][j] = -1;
                }
            }
        }
    }

}

int main() {

    //Constroi a matriz de tabuleiro e inicializa todos as posições em -1 (vazio)
    int** tab = malloc(3*sizeof(int*));
    for(int i = 0; i < 3; i++) {
        tab[i] = malloc(3*sizeof(int));
    }
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            tab[i][j] = -1;
        }
    }

    /*Inicializa a contagem de não derrotas com o tabuleiro em h sem traço para o jogador 1 ("X") e a contagem de configurações de tabuleiro
    em que havia h sem traço para o jogador 1*/
    int conta_h = 0, total_h = 0;

    //Simula o jogo
    simula_jogo(1, tab, &conta_h, &total_h);

    //Testa qual a probabilidade do jogador 1 ("X") ganhar baseado nas cenas que foram obtidas
    double testa = h_sem_traco_e_not_jog_1_prob();

    if(testa == -1)
        printf("Não tivemos exemplos de h se traço dessa vez....\n");
    else
        printf("\nO programa diz que o jogador 1 não perde com probabilidade %.4lf\n", testa);

    return 0;

}