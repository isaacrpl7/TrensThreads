#include "trem.h"
#include <QtCore>
#include <mutex>
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
            if (y == 30 && x <270){
                x+=10;
            }else if (x == 270 && y < 150){
//                state[0] = ESPERANDO;
//                if(state[1] != REGIAO_CRITICA)
//                    state[0] = REGIAO_CRITICA;
                mtx[0].lock();
                while(x == 270 && y < 150) {
                    y+=10;
                    emit updateGUI(ID, x,y);
                    msleep(velocidade);
                }
                mtx[0].unlock();
            } else if (x > 140 && y == 150) {
                //state[0] = ANDANDO;
                x-=10;
            } else
                y-=10;
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (y == 30 && x <400){
                x+=10;
            } else if (x == 400 && y < 150)
                y+=10;
            else if (x > 270 && y == 150)
                x-=10;
            else {
                mtx[0].lock();
                while(x == 270 && y > 30) {
                    y-=10;
                    emit updateGUI(ID, x,y);
                    msleep(velocidade);
                }
                mtx[0].unlock();
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (y == 30 && x <530)
                x+=10;
            else if (x == 530 && y < 150)
                y+=10;
            else if (x > 400 && y == 150)
                x-=10;
            else
                y-=10;
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
