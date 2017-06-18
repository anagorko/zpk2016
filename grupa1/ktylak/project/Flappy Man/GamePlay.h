#include "bird.h"
#include "pipe.h"

class GamePlay{
    int pipeWidth = 0;
    int pipeHeight = 0;
    int pipeX = 300;
    int pipeGap = 180;
    int pipeHole = 200;
    float scale = 1.3;
    int birdX = 40;
    int birdY = 500 / 3;
    int birdWidth = 0;
    int birdHeight = 0;
    int birdWidthDest = 36;
    int birdHeightDest = 28;

public:
    bool error();
    void remover();
    bool game();
};

