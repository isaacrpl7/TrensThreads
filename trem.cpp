#include "trem.h"
#include <QtCore>
#include <mutex>
#include <iostream>
#define ANDANDO 0
#define ESPERANDO 1

std::mutex mtx[7];
int state[7];
int trem5PrimeiraPassagem=1;
int trem4PrimeiraPassagem=1;
//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 100;
}

void Trem::setVelocidade(int posicao){
    this->velocidade = posicao;
}

//Função a ser executada após executar trem->START
void Trem::run(){
    while(true){
        switch(ID){
        case 1:     //Trem 1
            if (y == 30 && x <270){//preparando para entrar na espera
                if(state[0] != ESPERANDO)
                    state[0] = ANDANDO;
                if(state[0] == ANDANDO){
                    x+=10;
                }
                if (x == 250 && y == 30){
                    state[0] = ESPERANDO;
                    //std::cout << "Trem 1 esperando..." << std::endl;
                    mtx[0].lock();
                    state[0] = ANDANDO;
                    //std::cout << "Trem 1 entrou na regiao critica" << std::endl;
                }
            }else if (x == 270 && y < 150){
                if(state[0] == ANDANDO)
                    y+=10;
                if (x == 270 && y == 130){
                    state[0] = ESPERANDO;
                    //std::cout << "Trem 1 esperando..." << std::endl;
                    mtx[5].lock();
                    state[0] = ANDANDO;
                    //std::cout << "Trem 1 entrou na regiao critica numero 6" << std::endl;
                }
                if(x == 270 && y == 150) {
                    //std::cout << "Trem 1 saiu na regiao critica" << std::endl;
                    mtx[0].unlock();
                }
            } else if (x > 140 && y == 150) {
                x-=10;
                if(x == 180 && y == 150) {
                    //std::cout << "Trem 1 saiu na regiao critica" << std::endl;
                    mtx[5].unlock();
                }
            } else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (y == 30 && x <400){ //Entrando na regiao de espera e logo apos critica a sua direita
                //inicializando o estado
                if(state[1] != ESPERANDO)
                    state[1] = ANDANDO;
                //andando (caso nao esteja em espera)
                if(state[1] == ANDANDO){
                    x+=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 380 && y == 30){
                    state[1] = ESPERANDO;
                    mtx[1].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[1] = ANDANDO;
                    //std::cout << "Trem 2 entrou na regiao critica de sua direita" << std::endl;
                }
                if (x == 290){
                    mtx[0].unlock();
                    //std::cout << "Trem 2 saiu da regiao critica de sua esquerda" << std::endl;
                }
            } else if (x == 400 && y < 150){ //regiao critica
                if(state[1] == ANDANDO) {
                    y+=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 400 && y == 130){
                    state[1] = ESPERANDO;
                    std::cout << "Trem 2 entrou em espera do T4..." << std::endl; // PARA EVITAR DEADLOCK ESPERA OS DOIS SAIREM
                    mtx[4].lock();//usa mutex para verificar se pode entrar na regiao critica (espera regiao 5)
                    std::cout << "Trem 2 entrou em espera do T5..." << std::endl;
                    mtx[3].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[1] = ANDANDO;
                    std::cout << "Trem 2 andando pela regiao 4..." << std::endl;
                    //std::cout << "Trem 2 entrou na regiao critica 4" << std::endl;
                }
                if(x == 400 && y == 150) {//ao chegar no ultimo lugar da 2 regiao critica, mudar estado e liberar mutex
                    //std::cout << "Trem 2 saiu na regiao critica de sua direita" << std::endl;
                    mtx[1].unlock();
                }
            } else if (x > 270 && y == 150) {//preparando para entrar na espera
                if(state[1] == ANDANDO)
                    x-=10;
                if (x == 290 && y == 150){
                    state[1] = ESPERANDO;
                    //std::cout << "Trem 2 esperando..." << std::endl;
                    mtx[0].lock();
                    //std::cout << "Trem 2 entrou na regiao critica de sua esquerda" << std::endl;
                    state[1] = ANDANDO;
                }
                if(x == 310 && y == 150) {//ao chegar no ultimo lugar da 2 regiao critica, mudar estado e liberar mutex
                    //std::cout << "Trem 2 saiu da 4 regiao critica" << std::endl;
                    mtx[3].unlock();
                }
            } else { //Regiao critica
                y-=10;
                if(x == 270 && y == 130) {//ao chegar no ultimo lugar da 2 regiao critica, mudar estado e liberar mutex
                    std::cout << "Trem 2 saiu da 5 regiao critica" << std::endl;
                    mtx[4].unlock();
                }
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (y == 30 && x <530){
                x+=10;
            }else if (x == 530 && y < 150)
                y+=10;
            else if (x > 400 && y == 150){ //preparando para entrar em espera
                if(state[2] == ANDANDO)
                    x-=10;
                if (x == 470){//entrando na regiao critica, mas em espera (EVITANDO DEADLOCK)
                    state[2] = ESPERANDO;
                    //std::cout << "Trem 3 esperando o T2..." << std::endl; //O TREM 3 VAI ESPERAR O TREM DOIS PRIMEIRO, PARA DEPOIS
                    mtx[1].lock();                                        //CERTIFICAR DE QUE O TREM 5 PASSOU E EVITAR O DEADLOCK
                    //std::cout << "Trem 3 pode entrar na regiao critica 2" << std::endl;

                    //std::cout << "Trem 3 em espera do T5..." << std::endl;
                    mtx[2].lock();
                    state[2] = ANDANDO;
                    //std::cout << "Trem 3 andando pela regiao 3..." << std::endl;
                }
            } else {
                y-=10;
                if (y == 30 && x < 421){
                    mtx[1].unlock();
                    //std::cout << "Trem 3 saiu da regiao critica 2" << std::endl;
                }
                if (y == 130 && x == 400){
                    mtx[2].unlock();
                    //std::cout << "Trem 3 saiu da regiao critica 3" << std::endl;
                }
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (y == 150 && x <330){
                //inicializando o estado
                if(state[3] != ESPERANDO) {
                    state[3] = ANDANDO;
                }
                if(trem4PrimeiraPassagem && x == 200)//lockar na primeira passagem
                    mtx[5].lock();
                //andando (caso nao esteja em espera)
                if(state[3] == ANDANDO){
                    x+=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 250 && y == 150){
                    state[3] = ESPERANDO;
                    std::cout << "Trem 4 em espera do T5" << std::endl;
                    mtx[6].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[3] = ANDANDO;
                    std::cout << "Trem 4 andando pela regiao 7" << std::endl;
                }
                if (x == 290 && y == 150){
                    mtx[5].unlock();
                    std::cout << "Trem 4 saiu da regiao critica 6" << std::endl;
                    trem4PrimeiraPassagem = 0;
                }
            } else if (x == 330 && y < 270) {
                y+=10;
                if (x == 330 && y == 270){
                    mtx[6].unlock();
                    //std::cout << "Trem 4 saiu da regiao critica 7" << std::endl;
                }
                if (y == 160){
                    mtx[4].unlock();
                    std::cout << "Trem 4 saiu da regiao critica 5" << std::endl;
                }
            } else if (x > 200 && y == 270)
                x-=10;
            else {
                if(state[3] == ANDANDO)
                    y-=10;
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 200 && y == 170){
                    state[3] = ESPERANDO;

                    std::cout << "Trem 4 em espera do T2" << std::endl;
                    mtx[4].lock();//(esperar regiao 5)

                    std::cout << "Trem 4 em espera do T1" << std::endl;
                    mtx[5].lock();//usa mutex para verificar se pode entrar na 6 regiao critica
                    state[3] = ANDANDO;
                    std::cout << "Trem 4 andando pela regiao 6" << std::endl;
                }
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (y == 150 && x <460) {
                //inicializando o estado
                if(state[4] != ESPERANDO) {
                    state[4] = ANDANDO;
                }
                if(trem5PrimeiraPassagem && x == 330)//lockar na primeira passagem
                    mtx[3].lock();
                //andando (caso nao esteja em espera)
                if(state[4] == ANDANDO){
                    x+=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 380 && y == 150){
                    state[4] = ESPERANDO;
                    //std::cout << "Trem 5 em espera do T3" << std::endl;
                    mtx[2].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[4] = ANDANDO;
                    //std::cout << "Trem 5 andando pela regiao 3" << std::endl;
                    //std::cout << "Trem 5 entrou na regiao critica 3" << std::endl;
                }
                if (x == 420 && y == 150){
                    mtx[3].unlock();
                    //std::cout << "Trem 5 saiu da regiao critica 4" << std::endl;
                    trem5PrimeiraPassagem = 0;
                }
            } else if (x == 460 && y < 270) {
                y+=10;
                if (x == 460 && y == 170){
                    mtx[2].unlock();
                    //std::cout << "Trem 5 saiu da regiao critica 3" << std::endl;
                }
            } else if (x > 330 && y == 270) {
                if(state[4] == ANDANDO) {
                    x-=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 350 && y == 270){
                    state[4] = ESPERANDO;
                    std::cout << "Trem 5 em espera do T4" << std::endl;
                    mtx[6].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[4] = ANDANDO;
                    std::cout << "Trem 5 andando pela regiao 7" << std::endl;
                }
            } else {
                //andando (caso nao esteja em espera)
                if(state[4] == ANDANDO){
                    y-=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x == 330 && y == 170){
                    state[4] = ESPERANDO;
                    mtx[3].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[4] = ANDANDO;
                    //std::cout << "Trem 5 entrou na regiao critica 4" << std::endl;
                }
                if (x == 330 && y == 150){
                    mtx[6].unlock();
                    //std::cout << "Trem 5 saiu da regiao critica 7" << std::endl;
                }
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}
