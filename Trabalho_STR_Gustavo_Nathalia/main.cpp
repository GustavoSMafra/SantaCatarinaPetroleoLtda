/*
    Acadêmicos: Gustavo da Silva Mafra e Nathália Adriana de Oliveira
    Trabalho: Monitoramento de sensores
    Disciplina: Sistemas em Tempo Real
    Professor: Felipe Viel
*/

#include <iostream>
#include <windows.h>
#include <chrono>
#include <time.h>
#include <conio.h>

using namespace std;

void Sensor400(int sensores[3][6], int pc){
    Sleep(21); // REQUISIÇÃO + RESPOSTA + TEMPO DE GERAÇÃO DA AMOSTRA
    sensores[pc][0] =  rand()%10 + 1; // Gás
    sensores[pc][1] =  rand()%10 + 1; // Petróleo
    return;
}

void Sensor800(int sensores[3][6], int pc){
    Sleep(41); // REQUISIÇÃO + RESPOSTA + TEMPO DE GERAÇÃO DA AMOSTRA
    sensores[pc][2] =  rand()%10 + 1; // Gás
    sensores[pc][3] =  rand()%10 + 1; // Petróleo
    return;
}

void Sensor1200(int sensores[3][6], int pc){
    Sleep(61); // REQUISIÇÃO + RESPOSTA + TEMPO DE GERAÇÃO DA AMOSTRA
    sensores[pc][4] =  rand()%10 + 1; // Gás
    sensores[pc][5] =  rand()%10 + 1; // Petróleo
    return;
}

void Supervisor(int sensores[3][6], int contramedida[3], int pc){
    bool oil = false;
    bool gas = false;

    int status = rand()%10;
    if(status == 0){
        Sleep(300);
        contramedida[pc] = 4;

    } else {
        for(int i = 0; i<6; i++){
            if (i % 2 == 0 && sensores[pc][i] == 1 && gas == false){
                gas = true;
            }
            else if (i % 2 != 0 && sensores[pc][i] == 1 && oil == false){
                oil = true;
            }
        }

        if(gas == true && oil == true){
            contramedida[pc] = 1;
            Sleep(200);
        }
        else if(gas == true){
            contramedida[pc] = 2;
            Sleep(200);
        }
        else if(oil == true){
            contramedida[pc] = 3;
            Sleep(200);
        }
        else{
            contramedida[pc] = 0;
        }
    }
    return;
}

void Display(int sensores[3][6], int contramedida[3], int time[4]){
    for(int i = 0; i<3; i++){
        cout <<"\nPoço["<< i+1 << "]-> "<< endl;
        for(int j=0; j<6; j++){
            cout <<"Sensor["<< j+1 << "]-> ";
            if(sensores[i][j] == 0){
                cout<<" - - - - - - - "<< endl;
            } else if(sensores[i][j] == 1){
                cout<<" Pressão instável"<< endl;
            } else {
                cout<<" Pressão estável"<< endl;
            }
        }
        cout << "Tempo de resposta " << time[i]<< " ms"<<endl;
        if(contramedida[i] == 1){
            cout << "Realizou uma contramedida na tubulação de gás e petróleo, durante 200 ms"<<endl;
        }
        if(contramedida[i] == 2){
            cout << "Realizou uma contramedida na tubulação de gás,  durante 200 ms"<<endl;
        }
        if(contramedida[i] == 3){
            cout << "Realizou uma contramedida na tubulação de petróleo,  durante 200 ms"<<endl;
        }
        if(contramedida[i] == 4){
            cout << "Problema de mal funcionamento no poço, foi necessário desligar o mesmo em 300 ms"<<endl;
        }
    }

    cout<< "\nTempo de atualização do display " << time[3] << " ms" << endl;
    return;
}

int main() {
    setlocale(LC_ALL, "portuguese");
    bool work = true;

    srand(time(NULL));

    int tempo[4] = {};
    int sensores[3][6] = {};
    int contramedida[3] = {};


    while(work == true){

        system("cls");

        auto t_Display = chrono::high_resolution_clock::now(); // COMEÇA O TIMER DO DISPLAY

        auto t_P1 = chrono::high_resolution_clock::now(); // COMEÇA O TIMER
        Sensor400(sensores, 0); // VERIFICA SENSOR 1
        Sensor800(sensores, 0); // VERIFICA SENSOR 2
        Sensor1200(sensores, 0); // VERIFICA SENSOR 3
        Supervisor(sensores, contramedida, 0); // SUPERVISOR
        auto t_P1End = chrono::high_resolution_clock::now() - t_P1; // FIM DO TIME
        tempo[0] = chrono::duration_cast<chrono::milliseconds>(t_P1End).count(); // ARMAZENA O VALOR EM MS

        auto t_P2 = chrono::high_resolution_clock::now(); // COMEÇA O TIMER
        Sensor400(sensores, 1); // VERIFICA SENSOR 1
        Sensor800(sensores, 1); // VERIFICA SENSOR 2
        Sensor1200(sensores, 1); // VERIFICA SENSOR 3
        Supervisor(sensores, contramedida, 1); // SUPERVISOR
        auto t_P2End = chrono::high_resolution_clock::now() - t_P2; // FIM DO TIME
        tempo[1]  = chrono::duration_cast<chrono::milliseconds>(t_P2End).count(); // ARMAZENA O VALOR EM MS

        auto t_P3 = chrono::high_resolution_clock::now(); // COMEÇA O TIMER
        Sensor400(sensores, 2); // VERIFICA SENSOR 1
        Sensor800(sensores, 2); // VERIFICA SENSOR 2
        Sensor1200(sensores, 2); // VERIFICA SENSOR 3
        Supervisor(sensores, contramedida, 2); // SUPERVISOR
        auto t_P3End = chrono::high_resolution_clock::now() - t_P3; // FIM DO TIME
        tempo[2]  = chrono::duration_cast<chrono::milliseconds>(t_P3End).count(); // ARMAZENA O VALOR EM MS

        auto t_DisplayEnd = chrono::high_resolution_clock::now() - t_Display; // FINALIZA O TIMER DO DISPLAY
        tempo[3]  = chrono::duration_cast<chrono::milliseconds>(t_DisplayEnd).count(); // ARMAZENA O VALOR EM MS

        //DEMOSTRA OS VALORES OBTIDOS PARA O USUÁRIO
        Display(sensores, contramedida, tempo);

        cout <<"\n\nPressione esc para encerrar o processo, qualquer outro botão para continuar mais um ciclo"<< endl;
        int button = getch();
        if (button == 27){
            cout << endl;
            work = false;
        }
    }
    return 0;
}
