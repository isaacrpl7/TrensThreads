#include "trem.h"
#include <QtCore>
#include <mutex>
#include <iostream>
#define ANDANDO 0
#define ESPERANDO 1
#define REGIAO_CRITICA 2

std::mutex mtx[7];
int state[7];
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
                if(state[0] != REGIAO_CRITICA && state[0] != ESPERANDO)
                    state[0] = ANDANDO;
                if(state[0] != ESPERANDO){
                    x+=10;
                }
                if (x > 249 && y == 30 && state[0] != REGIAO_CRITICA){
                    state[0] = ESPERANDO;
                    //std::cout << "Trem 1 esperando..." << std::endl;
                }
                if(state[0] == ESPERANDO) {
                    mtx[0].lock();
                    state[0] = REGIAO_CRITICA;
                    //std::cout << "Trem 1 entrou na regiao critica" << std::endl;
                }
            }else if (x == 270 && y < 150){
                if(state[0] == REGIAO_CRITICA){
                    y+=10;
                    if(x == 270 && y == 150) {
                        state[0] = ANDANDO;
                        //std::cout << "Trem 1 saiu na regiao critica" << std::endl;
                        mtx[0].unlock();
                    }
                }
            } else if (x > 140 && y == 150) {
                x-=10;
            } else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (y == 30 && x <400){ //Entrando na regiao de espera e logo apos critica a sua direita
                //inicializando o estado
                if(state[1] != REGIAO_CRITICA && state[1] != ESPERANDO)
                    state[1] = ANDANDO;
                //andando (caso nao esteja em espera)
                if(state[1] != ESPERANDO){
                    x+=10;
                }
                //escolhendo o lugar onde espera (verificando se nao esta na regiao critica)
                if (x > 379 && y == 30 && state[1] != REGIAO_CRITICA){
                    state[1] = ESPERANDO;
                    //std::cout << "Trem 2 esperando..." << std::endl;
                }
                //entrando na regiao critica
                if(state[1] == ESPERANDO) {
                    mtx[1].lock();//usa mutex para verificar se pode entrar na regiao critica
                    state[1] = REGIAO_CRITICA;
                    //std::cout << "Trem 2 entrou na regiao critica de sua direita" << std::endl;
                }
            } else if (x == 400 && y < 150){ //regiao critica
                if(state[1] == REGIAO_CRITICA){
                    y+=10;
                    if(x == 400 && y == 150) {//ao chegar no ultimo lugar da regiao critica, mudar estado e liberar mutex
                        state[1] = ANDANDO;
                        //std::cout << "Trem 2 saiu na regiao critica de sua direita" << std::endl;
                        mtx[1].unlock();
                    }
                }
            } else if (x > 270 && y == 150) {//preparando para entrar na espera
                if(state[1] != ESPERANDO)
                    x-=10;
                if (x < 291 && y == 150 && state[1] != REGIAO_CRITICA){
                    state[1] = ESPERANDO;
                    //std::cout << "Trem 2 esperando..." << std::endl;
                }
                if(state[1] == ESPERANDO) {
                    mtx[0].lock();
                    //std::cout << "Trem 2 entrou na regiao critica de sua esquerda" << std::endl;
                    state[1] = REGIAO_CRITICA;
                }
            } else { //Regiao critica
                if(state[1] == REGIAO_CRITICA){
                    y-=10;
                    if (y == 30 && x <400){
                        mtx[0].unlock();
                        state[1] = ANDANDO;
                        //std::cout << "Trem 2 saiu da regiao critica de sua esquerda" << std::endl;
                    }
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
                if(state[2] != ESPERANDO)
                    x-=10;
                if (x < 421 && y == 150 && state[2] != REGIAO_CRITICA){
                    state[2] = ESPERANDO;
                    //std::cout << "Trem 3 esperando..." << std::endl;
                }
                if(state[2] == ESPERANDO) {
                    mtx[1].lock();
                    //std::cout << "Trem 3 entrou na regiao critica" << std::endl;
                    state[2] = REGIAO_CRITICA;
                }
            } else {
                if(state[2] == REGIAO_CRITICA){
                    y-=10;
                    if (y == 30 && x < 421){
                        mtx[1].unlock();
                        state[2] = ANDANDO;
                        //std::cout << "Trem 3 saiu da regiao critica" << std::endl;
                    }
                }
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (y == 150 && x <330)
                x+=10;
            else if (x == 330 && y < 270)
                y+=10;
            else if (x > 200 && y == 270)
                x-=10;
            else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (y == 150 && x <460)
                x+=10;
            else if (x == 460 && y < 270)
                y+=10;
            else if (x > 330 && y == 270)
                x-=10;
            else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}
