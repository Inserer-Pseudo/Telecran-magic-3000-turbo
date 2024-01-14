/*
void startPress(){
    Appuie.start();
}

void stopPress(){
    Appuie.stop();

    auto DureeAppuie = std::chrono::duration<float>(Appuie.elapsed_time()).count();
    if (DureeAppuie >= 0.2 && DureeAppuie < 3.0){
        CmdEfface=false;
        CmdSuivant=true;
    } else if(DureeAppuie >= 3.0){
        CmdEfface=true;
        CmdSuivant=false;
    }

    Appuie.reset();
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

printf("Etat calibration : %s\n", CmdSuivant ? "true" : "false");

*/