#include <iostream>
#include "serveur.h"

int main(){

    init_serveur();
    while(strcmp(buffer,"Q"))run_serv();


    return 0;
}
