#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Jogo da velha implementado em C.

Tabuleiro numa Matriz   -> (-1) vazio
                        -> 0 jogador "O"
                        -> 1 jogador "X"
*/

/*Devolve 1 estamos num estado terminal e o jogador "X" ganhou, 0 se estamos num estado terminal e o jogador "O" ganhou,
devolve -1 se o jogo empatou e -2 se não é um estado terminal.*/
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

//Retorna 1 se o jogador ocupava o meio do tabuleiro. Retorna 0 caso contrário.
int meio_ocupado(int jogador, int** tab) {
    if(tab[1][1] == jogador) return jogador;
    return 1 - jogador;
}

//Retorna 1 se o jogador ocupava mais cantos do tabuleiro. Retorna 0 caso contrário.
int dominio_canto(int jogador, int** tab) {
    int cont = 0, cont_opo = 0;

    if(tab[0][0] == jogador) cont++;
    else if(tab[0][0] == (1 - jogador)) cont_opo++;

    if(tab[0][2] == jogador) cont++;
    else if(tab[0][2] == (1 - jogador)) cont_opo++;
    
    if(tab[2][0] == jogador) cont++;
    else if(tab[2][0] == (1 - jogador)) cont_opo++;

    if(tab[2][2] == jogador) cont++;
    else if(tab[2][2] == (1 - jogador)) cont_opo++;

    if(cont > cont_opo) return jogador;
    return 1 - jogador;
}

//Retorna 1 se o jogador "X" vence a partida
int jogador_1(int jogador) {
    return jogador;
}

//Retorna 1 se o jogador "O" vence a partida
int jogador_2(int jogador) {
    return (1-jogador);
}

//Retorna 1 se o jogador tem uma sequência de dois no tabuleiro. Retorna 0 caso contrário
int sequencia_de_dois(int jogador, int** tab) {
    
    //Testa se as linhas têm alguma sequência de dois
    for(int i = 0; i < 3; i++) {
        if(tab[i][0] == jogador) {
            if(tab[i][1] == jogador)
                return 1;
        }
        else if(tab[i][1] == jogador){
            if(tab[i][2] == jogador)
                return 1;
        }
    }

    //Testa se as colunas têm alguma sequência de dois
    for(int j = 0; j < 3; j++) {
        if(tab[0][j] == jogador) {
            if(tab[1][j] == jogador)
                return 1;
        }
        else if(tab[1][j] == jogador){
            if(tab[2][j] == jogador)
                return 1;
        }
    }

    //Testa se as diagonais têm alguma sequência de dois
    for(int i = 0; i < 2; i++) {
        if(tab[i][i] == jogador)
            if(tab[i+1][i+1] == jogador)
                return 1;
        if(tab[i][2-i] == jogador)
            if(tab[i+1][2-(i+1)] == jogador)
                return 1;
    }

    return 0;
}

//Retorna 1 se o jogador tem um h sem traço no tabuleiro. Retorna 0 caso contrário
int h_sem_traco(int jogador, int** tab) {
    
    //Checa h sem traço superior
    if(tab[0][0] == jogador && tab[0][0] == tab[1][0])
        if(tab[0][2] == jogador && tab[0][2] == tab[1][2])
            return 1;

    //Checa h sem traço inferior
    if(tab[1][0] == jogador && tab[1][0] == tab[2][0])
        if(tab[1][2] == jogador && tab[1][2] == tab[2][2])
            return 1;

    //Checa h sem traço esquerdo
    if(tab[0][0] == jogador && tab[0][0] == tab[0][1])
        if(tab[2][0] == jogador && tab[2][0] == tab[2][1])
            return 1;

    //Checa h sem traço direito
    if(tab[0][1] == jogador && tab[0][1] == tab[0][2])
        if(tab[2][1] == jogador && tab[2][1] == tab[2][2])
            return 1;

    return 0;
}

/*Dada uma formatação final do tabuleiro, associa uma cena a essa formatação e gera dois arquivos: um com o tabuleiro e o vetor de predicados
associado e o outro apenas com os vetores de predicados.*/
void constroi_cena(int** tab, int meio, int canto, int jog_1, int jog_2, int seq_dois, int h_sem_traco) {
    FILE* cenas = fopen("cenas.txt", "a");
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(tab[i][j] != -1)
                if(j == 2)
                    fprintf(cenas, " %d", tab[i][j]);
                else
                    fprintf(cenas, " %d |", tab[i][j]);
            else
                if(j == 2)
                    fprintf(cenas, " ");
                else
                    fprintf(cenas, "   |");
        }
        if(i == 1) fprintf(cenas, "   (%d, %d, %d, %d, %d, %d)", meio, canto, jog_1, jog_2, seq_dois, h_sem_traco);
        fprintf(cenas, "\n");
    }
    fprintf(cenas, "\n");
    fclose(cenas);

    cenas = fopen("cenas_sem_tab.txt", "a");
    fprintf(cenas, "(%d, %d, %d, %d, %d, %d)\n", meio, canto, jog_1, jog_2, seq_dois, h_sem_traco);
    fclose(cenas);
}

//Constroi um arquivo para teste de aprendizado. Cada linha contém uma cena de u estado final aleatório do jogo.
void constroi_testes(int meio, int canto, int jog_1, int jog_2, int seq_dois, int h_sem_traco) {
    FILE* cenas = fopen("testes.txt", "a");
    fprintf(cenas, "(%d, %d, %d, %d, %d, %d)\n", meio, canto, jog_1, jog_2, seq_dois, h_sem_traco);
    fclose(cenas);
}

/*Código dedicado a interpretar conjuntamente as quatro relações e fornece features probabilísticas para decidir quem vai ganhar ganhou o jogo.*/

/*Para a primeira implementação, resolvi que a relação alvo seria jogador_1, que decide se o jogador "X" venceu ou não. Para isso, vamos avaliar
a relação de meio com jogador 1, de canto com jogador 1, e de meio e canto juntos com jogador 1. Depois disso, será decidido qual é a probabilidade
de jogador 1 ganhar ou não interpretadas essas features num mapa.*/

//Avalia se, para todo domínio de meio, também vale vitória de jog_1
int meio_e_jog_1() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco; 

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(meio == 1) {
        if(jog_2 == 1)
            return 0;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(meio == 1) {
            if(jog_2 == 1)
                return 0;
        }
    }
    fclose(cenas);

    return 1;
}

//Avalia se, para todo domínio de canto, também vale vitória de jog_1
int canto_e_jog_1() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco; 

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(canto == 1) {
        if(jog_2 == 1)
            return 0;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(canto == 1) {
            if(jog_2 == 1)
                return 0;
        }
    }
    fclose(cenas);

    return 1;
}

//Avalia se, para todo h sem traço, também vale derrota de jog_1
int h_sem_traco_e_not_jog_1() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco; 

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(h_sem_traco == 1) {
        if(jog_1 == 1)
            return 0;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(h_sem_traco == 1) {
            if(jog_1 == 1)
                return 0;
        }
    }
    fclose(cenas);

    return 1;
}

double h_sem_traco_e_not_jog_1_prob() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco;
    int total = 0, contador = 0, total_2 = 1;

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(h_sem_traco == 1) {
        if(jog_1 == 0 && jog_2 == 0)
            contador++;
        total++;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(h_sem_traco == 1) {
            if(jog_1 == 0 && jog_2 == 0)
                contador++;
            total++;
        }
        total_2++;
    }
    fclose(cenas);

    if(total == 0) return -1;

    printf("%d\n", total_2);

    return ((double)contador/(double)total);
}

//Avalia se, para todo domínio de meio e domínio de canto, também vale vitória de jog_1
int meio_canto_e_jog_1() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco; 

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(meio == 1 && canto == 1) {
        if(jog_2 == 1)
            return 0;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(meio == 1 && canto == 1) {
            if(jog_2 == 1)
                return 0;
        }
    }
    fclose(cenas);

    return 1;
}

//Avalia se, para todo domínio de meio e sequência de dois, também vale vitória de jog_1
int meio_seq_dois_e_jog_1() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco; 

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(meio == 1 && seq_dois == 1) {
        if(jog_2 == 1)
            return 0;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(meio == 1 && seq_dois == 1) {
            if(jog_2 == 1)
                return 0;
        }
    }
    fclose(cenas);

    return 1;
}

//Avalia se, para todo domínio de canto e sequência de dois, também vale vitória de jog_1
int canto_seq_dois_e_jog_1() {
    FILE* cenas = fopen("cenas_sem_tab.txt", "r");
    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco; 

    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    if(canto == 1 && seq_dois == 1) {
        if(jog_2 == 1)
            return 0;
    }
    while(f != EOF) {
        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
        if(canto == 1 && seq_dois == 1) {
            if(jog_2 == 1)
                return 0;
        }
    }
    fclose(cenas);

    return 1;
}

/*Função que testa o arquivo teste com as formulação de cláusulas já estabelecida. Imprime a quantidade de acertos que teve em todos os chutes que deu e a
quantidade de acertos sobre todo o espaço de testes.*/
void testa() {
    int t_meio = meio_e_jog_1();
    int t_canto = canto_e_jog_1();
    int t_h_sem_traco = h_sem_traco_e_not_jog_1();
    int t_meio_canto = meio_canto_e_jog_1();
    int t_meio_seq_dois = meio_seq_dois_e_jog_1();
    int t_canto_seq_dois = canto_seq_dois_e_jog_1();
    printf("\n%d %d %d %d %d %d\n", t_meio, t_canto, t_h_sem_traco, t_meio_canto, t_meio_seq_dois, t_canto_seq_dois);

    int meio, canto, jog_1, jog_2, seq_dois, h_sem_traco;

    int chutes = 0, acertos = 0, total = 0;

    FILE* cenas = fopen("testes.txt", "r");
    int f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    while(f != EOF) {
        total++;
        if(t_meio == 1 && meio == 1) {
            chutes++;
            if(jog_1 == 1)
                acertos++;
        }
        else if(t_canto == 1 && canto == 1) {
            chutes++;
            if(jog_1 == 1)
                acertos++;
        }
        else if(t_h_sem_traco == 1 && h_sem_traco == 1) {
            chutes++;
            if(jog_1 == 0)
                acertos++;
        }
        else if(t_meio_canto == 1 && meio == 1 && canto == 1) {
            chutes++;
            if(jog_1 == 1)
                acertos++;
        }
        else if(t_meio_seq_dois == 1 && meio == 1 && seq_dois == 1) {
            chutes++;
            if(jog_1 == 1)
                acertos++;
        }
        else if(t_canto_seq_dois == 1 && canto == 1 && seq_dois == 1) {
            chutes++;
            if(jog_1 == 1)
                acertos++;
        }
        /*else {
            if(!jog_1) {
                chutes++;
                acertos++;
            }
        }*/

        f = fscanf(cenas, "(%d, %d, %d, %d, %d, %d)\n", &meio, &canto, &jog_1, &jog_2, &seq_dois, &h_sem_traco);
    }

    printf("\n ---------- Testando afirmar e acertar ----------- \n");
    if(chutes != 0)
        printf("\n                      %.4f\n", (float)acertos/(float)chutes);
    else
        printf("\n                      0.0000\n");
    printf("\n ------------------------------------------------- \n");

    printf("\n ------ Testando afirmar e acertar no total ------ \n");
    printf("\n                      %.4f\n", (float)acertos/(float)total);
    printf("\n -------------------------------------------------\n");

}

//Testa a resposta do algoritmo para se o jogador deve ganhar ou não a partida a partir do h sem traço do jogador 1

int testa_jogo_a_partir(int jogador, int** tab) {
    int t_meio = meio_e_jog_1();
    int t_canto = canto_e_jog_1();
    int t_h_sem_traco = h_sem_traco_e_not_jog_1();
    int t_meio_canto = meio_canto_e_jog_1();
    int t_meio_seq_dois = meio_seq_dois_e_jog_1();
    int t_canto_seq_dois = canto_seq_dois_e_jog_1();

    int meio = meio_ocupado(jogador, tab);
    int canto = dominio_canto(jogador, tab);
    int seq_dois = sequencia_de_dois(jogador, tab);
    int h_sem_t = h_sem_traco(jogador, tab);

    if(t_meio == 1 && meio == 1) {
        return 1;
    }
    else if(t_canto == 1 && canto == 1) {
        return 1;
    }
    else if(t_h_sem_traco == 1 && h_sem_t == 1) {
        return 0;
    }
    else if(t_meio_canto == 1 && meio == 1 && canto == 1) {
        return 1;
    }
    else if(t_meio_seq_dois == 1 && meio == 1 && seq_dois == 1) {
        return 1;
    }
    else if(t_canto_seq_dois == 1 && canto == 1 && seq_dois == 1) {
        return 1;
    }

    return 0;
}

//Testa a resposta do algoritmo para se o jogador deve ganhar ou não a partida a partir do h sem traço do jogador 1 considerando apenas o predicado h_sem_traco
double testa_jogo_a_partir_h_sem_traco() {
    double t_h_sem_traco = h_sem_traco_e_not_jog_1_prob();

    return t_h_sem_traco;
}

//Simula o jogo a partir de um ponto do tabuleiro para contar probabilidade do jogador 1 vencer a partida a partir dali
void simula_jogo_a_partir(int jogador, int* conta_terminal_aux, int* jog_1_aux, int** tab) {
    int terminal = terminal_state(tab);
    if(terminal != -2) {
        if(terminal == 1 || terminal == -1)
            (*jog_1_aux) += 1;
        (*conta_terminal_aux) += 1;
    }
    else {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(tab[i][j] == -1) {
                    tab[i][j] = jogador;
                    simula_jogo_a_partir(1-jogador, conta_terminal_aux, jog_1_aux, tab);
                    tab[i][j] = -1;
                }
            }
        }
    }
}

/*Simula todas as possibilidades de jogo para o jogo da velha e conta cada um dos predicados relacionados aos estados finais*/
void simula_jogo(int jogador, int* conta_terminal, int* jog_1, int* jog_2, int** tab, int* conta_h, int* total_h) {

    int terminal = terminal_state(tab);
    if(terminal != -2) {
        int arg_1 = 0, arg_2 = 0, arg_3 = 0, arg_4 = 0, arg_5 = 0, arg_6 = 0;
        
        if(meio_ocupado(1, tab)) {
            arg_1 = 1;
        }
        if(dominio_canto(1, tab)) {
            arg_2 = 1;
        }
        if(terminal >= 0) {
            if(jogador_1(terminal)) {
                (*jog_1) += 1;
                arg_3 = 1;
            }
            if(jogador_2(terminal)) {
                (*jog_2) += 1;
                arg_4 = 1;
            }
        }
        if(sequencia_de_dois(1, tab))
            arg_5 = 1;
        if(h_sem_traco(1, tab))
            arg_6 = 1;
        (*conta_terminal) += 1;

        int r = 0;
        if(r == 0)
            constroi_cena(tab, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);
        else if(r == 1)
            constroi_testes(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);

    }

    else {

        //Escolhe jogos ocasionais para testar a eficiência do algoritmo
        //int r = rand()%20000;

        //Testa jogos em que o tabuleiro encontra-se em h sem traço
        int r = 1;
        if(h_sem_traco(1, tab) && jogador == 0) r = 0;
        int jog_1_aux = 0; int conta_terminal_aux = 0;
        if(r == 0) {
            int** tab_aux = malloc(3*sizeof(int*));
            for(int i = 0; i < 3; i++) {
                tab_aux[i] = malloc(3*sizeof(int));
            }
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    tab_aux[i][j] = tab[i][j];
                }
            }
            simula_jogo_a_partir(jogador, &conta_terminal_aux, &jog_1_aux, tab_aux);
            //int aux = testa_jogo_a_partir(jogador, tab);
            printf("\n");
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    printf(" %d ", tab[i][j]);
                }
                printf("\n");
            }
            if(tab[0][0] == -1 && tab[0][1] == -1 && tab[0][2] == 0){
                double aux = testa_jogo_a_partir_h_sem_traco();
                if(aux > 1)
                    printf("Não tivemos exemplos de h se traço dessa vez....\n");
                else
                    printf("\nO programa diz que o jogador 1 não perde com probabilidade %.4lf\n", aux);
            }
            printf("O jogador 1 não perde esse tabuleiro com probabilidade %.4f\n", (float)jog_1_aux/(float)conta_terminal_aux);
            (*conta_h) += jog_1_aux;
            (*total_h) += conta_terminal_aux;
            printf("O jogador 1 não perde com probabilidade %.4f\n", (float)(*conta_h)/(float)(*total_h));
            printf("%d", (*total_h));

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
                    simula_jogo(1-jogador, conta_terminal, jog_1, jog_2, tab, conta_h, total_h);
                    tab[i][j] = -1;
                }
            }
        }
    }
}

int main() {
    
    srand(time(NULL));

    int jogador = 1;
    int conta_terminal = 0;
    int jog_1_vence = 0; int jog_2_vence = 0;

    int** tab = malloc(3*sizeof(int*));
    for(int i = 0; i < 3; i++) {
        tab[i] = malloc(3*sizeof(int));
    }
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            tab[i][j] = -1;
        }
    }

    int conta_h = 0, total_h = 0;
    simula_jogo(jogador, &conta_terminal, &jog_1_vence, &jog_2_vence, tab, &conta_h, &total_h);
    
    printf("\nNúmero de estados terminais: %d\n", conta_terminal);
    printf("Número de vitórias do jogador 1: %d\n", jog_1_vence);
    printf("Número de vitórias do jogador 2: %d\n", jog_2_vence);

    testa();

    double aux = testa_jogo_a_partir_h_sem_traco();
    if(aux > 1)
        printf("Não tivemos exemplos de h se traço dessa vez....\n");
    else
        printf("\nO programa diz que o jogador 1 não perde com probabilidade %.4lf\n", aux);

    //Limpando os arquivos
    ///*
    FILE* arq = fopen("cenas.txt", "w");
    fclose(arq);
    arq = fopen("cenas_sem_tab.txt", "w");
    fclose(arq);
    arq = fopen("testes.txt", "w");
    fclose(arq);
    //*/

    return 0;
}