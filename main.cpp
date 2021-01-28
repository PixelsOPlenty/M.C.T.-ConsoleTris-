//ConsoleTris
#include<iostream>
#include<chrono>
#include<cstdlib>
#include<string>
#include<Windows.h>
#include<vector>

const int GAMETILESX = 10, GAMETILESY = 20, FASTDROPSPEED = 3;
bool frameTick = false, spawnBlock, right, rightHeld, left, leftHeld, wouldClip, spinRight, spinRightHeld, spinLeft, spinLeftHeld, hardDrop, hardDropHeld, fastDrop, fastDropHeld, quitGame;
const double FRAMEDELAY = (1.0 / 60.0);
int gameTicksTillDrop, currentDifficulty, currentBlock, nextBlock, moveHorizontal, moveVertical, fastDropTicks, lineRotationState = 0, score, linesKilled, level, totalLinesCleared, nextLevel, menuItem, gameState = 2, bgCycler = 0, tabs = 0, spaces = 0, endLines = 0, startLines = 0, difficultyType = 1, gameOverScreenCounter;
double timer = 0.0;
std::string boardImage;

int block[8];
int dropShadow[8];
int lineFixer[8];
int blockDataHold[8];

int square[8] = { 0,0,0,1,1,0,1,1 }; //0
int line[8] = { 1,1,0,1,2,1,3,1 }; //1
int lBlock[8] = { 1,1,0,0,0,1,2,1 }; //2
int reverseLBlock[8] = { 1,1,0,1,2,0,2,1 }; //3
int tBlock[8] = { 1,1,0,1,1,0,2,1 }; //4
int squigly[8] = { 1,1,0,0,1,0,2,1 }; //5
int reverseSquigly[8] = { 1,0,0,1,1,1,2,0 }; //6
int nullBlock[8] = { 0,0,0,0,0,0,0,0 }; //OhNo!!!

bool gameBoard[GAMETILESX][GAMETILESY];

char menuDrawing[33][20];

void clearBoard();

void drawBoard();

void checkControls();

void createBlock();

void checkRL();

void checkBlocks(int x, int y);

void buildBlock(int blockType[8], int x, int y);

void moveBlock(int x, int y);

void spawnDropShadow();

void moveShadow(int y);

void checkRotation();

void rotationType1();

void placeBlock();

void clearLines();

void rotationType2();

void buildLineFixer();

void lineRotationLine();

void updateLine();

void checkLevel();

void blockPopOut1();

void blockPopOut2();

void drawMenuScreen();

void clearMenuDrawing();

void createBGBlock();

void drawMCTLogo();

void checkMenuControls();

void clearControls();

void drawControlScreen();

void checkOptionsControls();

void drawOptionsScreen();

char numToChar(int numby, int charPoint);

class bgPiece
{
    int bgBlock[12];
    int bgLayer, bgCycles;



    void bgBuildBlock(int passedBlock[8], int x);
public:
    bgPiece();
    void bgMovePiece();
    bool checkOB();
    void drawBGBlock(int lToDraw);
};
bgPiece::bgPiece()
{

    bgLayer = rand() % 4;

    switch (bgLayer)
    {
    case 0:
        bgCycles = 6;
        break;
    case 1:
        bgCycles = 12;
        break;
    case 2:
        bgCycles = 25;
        break;
    case 3:
        bgCycles = 45;
        break;
    }

    switch (rand() % 7)
    {
    case 0:
        bgBuildBlock(square, rand() % 36 - 3);
        break;
    case 1:
        bgBuildBlock(line, rand() % 40 - 7);
        break;
    case 2:
        bgBuildBlock(lBlock, rand() % 38 - 5);
        break;
    case 3:
        bgBuildBlock(reverseLBlock, rand() % 38 - 5);
        break;
    case 4:
        bgBuildBlock(tBlock, rand() % 38 - 5);
        break;
    case 5:
        bgBuildBlock(squigly, rand() % 38 - 5);
        break;
    case 6:
        bgBuildBlock(reverseSquigly, rand() % 38 - 5);
        break;
    }

}
void bgPiece::bgMovePiece()
{
    bgCycles -= 1;

    if (bgCycles == 0)
    {
        switch (bgLayer)
        {
        case 0:
            bgCycles = 6;
            break;
        case 1:
            bgCycles = 12;
            break;
        case 2:
            bgCycles = 25;
            break;
        case 3:
            bgCycles = 45;
            break;
        }

        bgBlock[2] -= 1;
        bgBlock[5] -= 1;
        bgBlock[8] -= 1;
        bgBlock[11] -= 1;

    }

}
void bgPiece::bgBuildBlock(int passedBlock[8], int x)
{
    bgBlock[0] = (passedBlock[0] * 2) + x;
    bgBlock[1] = ((passedBlock[0] * 2) + 1) + x;
    bgBlock[2] = passedBlock[1] + 19;

    bgBlock[3] = (passedBlock[2] * 2) + x;
    bgBlock[4] = ((passedBlock[2] * 2) + 1) + x;
    bgBlock[5] = passedBlock[3] + 19;

    bgBlock[6] = (passedBlock[4] * 2) + x;
    bgBlock[7] = ((passedBlock[4] * 2) + 1) + x;
    bgBlock[8] = passedBlock[5] + 19;

    bgBlock[9] = (passedBlock[6] * 2) + x;
    bgBlock[10] = ((passedBlock[6] * 2) + 1) + x;
    bgBlock[11] = passedBlock[7] + 19;
}
bool bgPiece::checkOB()
{
    if (bgBlock[11] < -3)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void bgPiece::drawBGBlock(int lToDraw)
{
    if (lToDraw == bgLayer)
    {
        if (bgBlock[2] < 20 && bgBlock[2] >= 0)
        {
            if (bgBlock[0] < 33 && bgBlock[0] >= 0)
            {
                menuDrawing[bgBlock[0]][bgBlock[2]] = '[';
            }
            if (bgBlock[1] < 33 && bgBlock[1] >= 0)
            {
                menuDrawing[bgBlock[1]][bgBlock[2]] = ']';
            }
        }
        if (bgBlock[5] < 20 && bgBlock[5] >= 0)
        {
            if (bgBlock[3] < 33 && bgBlock[3] >= 0)
            {
                menuDrawing[bgBlock[3]][bgBlock[5]] = '[';
            }
            if (bgBlock[4] < 33 && bgBlock[4] >= 0)
            {
                menuDrawing[bgBlock[4]][bgBlock[5]] = ']';
            }
        }
        if (bgBlock[8] < 20 && bgBlock[8] >= 0)
        {
            if (bgBlock[6] < 33 && bgBlock[6] >= 0)
            {
                menuDrawing[bgBlock[6]][bgBlock[8]] = '[';
            }
            if (bgBlock[7] < 33 && bgBlock[7] >= 0)
            {
                menuDrawing[bgBlock[7]][bgBlock[8]] = ']';
            }
        }
        if (bgBlock[11] < 20 && bgBlock[11] >= 0)
        {
            if (bgBlock[9] < 33 && bgBlock[9] >= 0)
            {
                menuDrawing[bgBlock[9]][bgBlock[11]] = '[';
            }
            if (bgBlock[10] < 33 && bgBlock[10] >= 0)
            {
                menuDrawing[bgBlock[10]][bgBlock[11]] = ']';
            }
        }

    }
}

std::vector<bgPiece> bgBlockVector;

int main()
{
    //std::cout << "\nWelcome to PixelsOPlenty's M.C.T. 'ConsoleTris' Version 2.1! :D\n\nIf you find any bugs please message me!\n\nEnter any character and then press'Enter' to begin.\n";

    //std::cin >> boardImage;

    /*
    block[0] = 4;
    block[1] = 5;
    block[2] = 5;
    block[3] = 5;
    block[4] = 4;
    block[5] = 6;
    block[6] = 5;
    block[7] = 6;
    */

    srand(time(NULL));

    clearBoard();

    drawBoard();

    frameTick = false;


    auto lastFrame = std::chrono::high_resolution_clock::now();
    auto thisFrame = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> deltaTime = thisFrame - lastFrame;



    while (!quitGame)
    {
        frameTick = false;

        thisFrame = std::chrono::high_resolution_clock::now();
        deltaTime = thisFrame - lastFrame;
        lastFrame = thisFrame;

        timer += deltaTime.count();

        if (timer >= FRAMEDELAY)
        {
            frameTick = true;

            timer -= FRAMEDELAY;

            while (timer >= FRAMEDELAY)
            {
                timer -= FRAMEDELAY;
            }

        }

        checkControls();

        /*        if (GetAsyncKeyState((unsigned short)'R') & 0x8000)
                {
                    spawnBlock = true;
                }*/

        if (frameTick && gameState == 1)
        {
            fastDropTicks--;
            gameTicksTillDrop--;

            if (spawnBlock)
            {
                createBlock();
                checkBlocks(0, 0);
                if (wouldClip)
                {
                    gameState = 4;
                    placeBlock();
                    clearControls();
                }
            }

            checkRotation();
            checkRL();
            checkRotation();
            checkRL();

            if (hardDrop)
            {
                wouldClip = false;
                for (int i = 0; !wouldClip; i--)
                {
                    checkBlocks(0, i);

                    if (wouldClip)
                    {
                        i++;
                        moveBlock(0, i);
                    }
                }
                gameTicksTillDrop = 0;
                fastDropTicks = 0;

            }


            if (gameTicksTillDrop < 1 && !fastDrop || fastDropTicks < 1 && fastDrop || currentDifficulty < FASTDROPSPEED && gameTicksTillDrop < 1)
            {

                checkBlocks(0, -1);
                if (wouldClip == true)
                {
                    checkRotation();
                    checkRL();
                    checkRotation();
                    checkRL();
                    placeBlock();
                    spawnBlock = true;
                    fastDropHeld = true;
                    clearLines();


                }
                else
                {
                    moveBlock(0, -1);
                    checkRotation();
                    checkRL();
                    checkRotation();
                    checkRL();
                }


            }
            if (gameTicksTillDrop < 1)
            {
                gameTicksTillDrop = currentDifficulty + 1;
            }
            if (fastDropTicks < 1)
            {
                fastDropTicks = FASTDROPSPEED + 1;
            }


            left = false;
            right = false;
            spawnDropShadow();
            drawBoard();
            std::cout << boardImage;
            wouldClip = false;
            fastDrop = false;
            spinRight = false;
            spinLeft = false;
            hardDrop = false;
            //Mercy Spawn Frames
            if (spawnBlock)
            {
                gameTicksTillDrop += 16;
            }

        }
        if (frameTick && gameState == 3)
        {
            clearMenuDrawing();

            checkOptionsControls();

            if (bgCycler < 0)
            {
                createBGBlock();
                bgCycler = 50;
            }
            else
            {
                bgCycler--;
            }

            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(3);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(2);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(1);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(0);
                bgBlockVector[i].bgMovePiece();

                if (bgBlockVector[i].checkOB())
                {
                    bgBlockVector.erase(bgBlockVector.begin() + i);
                    i--;
                }
            }

            drawOptionsScreen();

            drawMenuScreen();
            std::cout << boardImage;
        }
        if (frameTick && gameState == 2)
        {
            clearMenuDrawing();

            if (bgCycler < 0)
            {
                createBGBlock();
                bgCycler = 50;
            }
            else
            {
                bgCycler--;
            }


            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(3);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(2);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(1);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(0);
                bgBlockVector[i].bgMovePiece();

                if (bgBlockVector[i].checkOB())
                {
                    bgBlockVector.erase(bgBlockVector.begin() + i);
                    i--;
                }
            }

            if (right)
            {
                clearControls();
                gameState = 0;
            }


            drawControlScreen();
            drawMenuScreen();
            std::cout << boardImage;

        }
        if (frameTick && gameState == 0)
        {
            clearMenuDrawing();

            checkMenuControls();

            if (bgCycler < 0)
            {
                createBGBlock();
                bgCycler = 50;
            }
            else
            {
                bgCycler--;
            }


            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(3);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(2);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(1);
            }
            for (int i = 0; i < bgBlockVector.size(); i++)
            {
                bgBlockVector[i].drawBGBlock(0);
                bgBlockVector[i].bgMovePiece();

                if (bgBlockVector[i].checkOB())
                {
                    bgBlockVector.erase(bgBlockVector.begin() + i);
                    i--;
                }
            }

            drawMCTLogo();
            drawMenuScreen();
            std::cout << boardImage;
        }
        if (frameTick && gameState == 4)
        {

            if (gameOverScreenCounter < 160)
            {
                gameOverScreenCounter++;
            }
            else if (left || right || hardDrop || fastDrop || spinLeft || spinRight)
            {
                gameState = 0;
            }

            drawBoard();
            std::cout << boardImage;

            clearControls();

        }


    }



    //std::cout << "Program Closed, Game Over!\nPress Enter To Quit." << std::endl;
    //std::cin >> boardImage;
    return 0;
}

void clearBoard()
{
    for (int i = 0; i < GAMETILESX; i++)
    {
        for (int i2 = 0; i2 < GAMETILESY; i2++)
        {
            gameBoard[i][i2] = false;
        }
    }


    gameTicksTillDrop = currentDifficulty;
    spawnBlock = true;
    nextBlock = rand() % 7;
    wouldClip = false;

    gameOverScreenCounter = 0;
    score = 0;
    totalLinesCleared = 0;
    //Need to Reset the Level depending on gamemode.
    bgBlockVector.clear();
    bgCycler = 0;
    menuItem = 0;
    clearControls();
    checkLevel();
}

void drawBoard()
{
    boardImage = "\n";
    for (int i = 0; i < startLines; i++)
    {
        boardImage += "\n";
    }
    for (int i = 0; i < tabs; i++)
    {
        boardImage += "\t";
    }
    for (int i = 0; i < spaces; i++)
    {
        boardImage += " ";
    }
    boardImage += "###################################\n";
    for (int i = 0; i < tabs; i++)
    {
        boardImage += "\t";
    }
    for (int i = 0; i < spaces; i++)
    {
        boardImage += " ";
    }
    boardImage += "#";


    for (int y = GAMETILESY - 1; y >= 0; y--)
    {
        for (int x = 0; x < GAMETILESX; x++)
        {
            if (gameOverScreenCounter == 160 && y == 2)
            {
                boardImage += "==";
            }
            else if (gameOverScreenCounter == 160 && y == 1)
            {
                if (x == 0)
                {
                    boardImage += "  Press Any Key to  ";
                }

            }
            else if (gameOverScreenCounter == 160 && y == 0)
            {
                if (x == 0)
                {
                    boardImage += " Return to the Menu ";
                }

            }
            else if (y < ((gameOverScreenCounter - 30) / 5))
            {

                if (gameOverScreenCounter == 160 && y == 16 && x > 0 && x < 9)
                {
                    boardImage += "==";
                }
                else if (gameOverScreenCounter == 160 && y == 12 && x > 0 && x < 9)
                {
                    boardImage += "==";
                }
                else if (gameOverScreenCounter == 160 && (y == 15 || y == 13) && x > 1 && x < 8)
                {
                    boardImage += "  ";
                }
                else if ((gameOverScreenCounter == 160 && y == 14 && x == 2))
                {
                    boardImage += " G";
                }
                else if ((gameOverScreenCounter == 160 && y == 14 && x == 3))
                {
                    boardImage += "AM";
                }
                else if ((gameOverScreenCounter == 160 && y == 14 && x == 4))
                {
                    boardImage += "E ";
                }
                else if ((gameOverScreenCounter == 160 && y == 14 && x == 5))
                {
                    boardImage += " O";
                }
                else if ((gameOverScreenCounter == 160 && y == 14 && x == 6))
                {
                    boardImage += "VE";
                }
                else if ((gameOverScreenCounter == 160 && y == 14 && x == 7))
                {
                    boardImage += "R ";
                }
                else if ((gameOverScreenCounter == 160 && (y == 14 || y == 13 || y == 15) && x == 8))
                {
                    boardImage += "| ";
                }
                else if ((gameOverScreenCounter == 160 && (y == 14 || y == 13 || y == 15) && x == 1))
                {
                    boardImage += " |";
                }
                else
                {
                    boardImage += "[]";
                }



            }
            else if (gameBoard[x][y] == true)
            {
                boardImage += "[]";
            }
            /*else if (x == lineFixer[0] && y == lineFixer[1] || x == lineFixer[2] && y == lineFixer[3] || x == lineFixer[4] && y == lineFixer[5] || x == lineFixer[6] && y == lineFixer[7])
            {
                if (lineRotationState == 0)
                {
                    boardImage += "00";
                }
                else if (lineRotationState == 1)
                {
                    boardImage += "11";
                }
                else if (lineRotationState == 2)
                {
                    boardImage += "22";
                }
                else if (lineRotationState == 3)
                {
                    boardImage += "33";
                }
                else
                {
                    boardImage += "XX";
                }
            }*/
            else if (x == block[0] && y == block[1] || x == block[2] && y == block[3] || x == block[4] && y == block[5] || x == block[6] && y == block[7])
            {
                boardImage += "[]";
            }
            else if (gameState == 1 && (x == dropShadow[0] && y == dropShadow[1] || x == dropShadow[2] && y == dropShadow[3] || x == dropShadow[4] && y == dropShadow[5] || x == dropShadow[6] && y == dropShadow[7]))
            {
                boardImage += "::";
            }
            else
            {
                boardImage += "  ";
            }

        }
        if (y == 19)
        {
            boardImage += "# NEXT BLOCK ";
        }
        else if (y == 18)
        {
            boardImage += "#------------";
        }
        else if (y == 17)
        {
            boardImage += "#            ";
        }
        else if (y == 16)
        {
            switch (nextBlock)
            {
            case 0:
                boardImage += "#    [][]    ";
                break;
            case 1:
                boardImage += "#  [][][][]  ";
                break;
            case 2:
                boardImage += "#  [][][]    ";
                break;
            case 3:
                boardImage += "#  [][][]    ";
                break;
            case 4:
                boardImage += "#  [][][]    ";
                break;
            case 6:
                boardImage += "#  [][]      ";
                break;
            case 5:
                boardImage += "#    [][]    ";
                break;
            default:
                boardImage += "#            ";
                break;
            }
        }
        else if (y == 15)
        {
            switch (nextBlock)
            {
            case 0:
                boardImage += "#    [][]    ";
                break;
            case 1:
                boardImage += "#            ";
                break;
            case 2:
                boardImage += "#  []        ";
                break;
            case 3:
                boardImage += "#      []    ";
                break;
            case 4:
                boardImage += "#    []      ";
                break;
            case 6:
                boardImage += "#    [][]    ";
                break;
            case 5:
                boardImage += "#  [][]      ";
                break;
            default:
                boardImage += "#            ";
                break;
            }
        }
        if (y == 14)
        {
            boardImage += "#            ";
        }
        if (y == 13)
        {
            boardImage += "#############";
        }
        if (y == 8)
        {
            if (level > 9)
            {
                boardImage += "#  LEVEL: ";
                boardImage += std::to_string(level);
                boardImage += " ";
            }
            else
            {
                boardImage += "#  LEVEL: ";
                boardImage += std::to_string(level);
                boardImage += "  ";
            }
        }
        if (y == 9)
        {
            boardImage += "#############";
        }
        if (y == 12)
        {
            boardImage += "#   SCORE    ";
        }
        if (y == 11)
        {
            boardImage += "#------------";
        }
        if (y == 10)
        {
            if (score < 10)
            {
                boardImage += "#     ";
                boardImage += std::to_string(score);
                boardImage += "      ";
            }
            else if (score < 100)
            {
                boardImage += "#     ";
                boardImage += std::to_string(score);
                boardImage += "     ";
            }
            else if (score < 1000)
            {
                boardImage += "#    ";
                boardImage += std::to_string(score);
                boardImage += "     ";
            }
            else if (score < 10000)
            {
                boardImage += "#    ";
                boardImage += std::to_string(score);
                boardImage += "    ";
            }
            else if (score < 100000)
            {
                boardImage += "#   ";
                boardImage += std::to_string(score);
                boardImage += "    ";
            }
            else if (score < 1000000)
            {
                boardImage += "#   ";
                boardImage += std::to_string(score);
                boardImage += "   ";
            }
            else if (score < 10000000)
            {
                boardImage += "#  ";
                boardImage += std::to_string(score);
                boardImage += "   ";
            }
            else if (score < 100000000)
            {
                boardImage += "#  ";
                boardImage += std::to_string(score);
                boardImage += "  ";
            }
            else if (score < 1000000000)
            {
                boardImage += "# ";
                boardImage += std::to_string(score);
                boardImage += "  ";
            }
            else if (score < 10000000000)
            {
                boardImage += "# ";
                boardImage += std::to_string(score);
                boardImage += " ";
            }
        }
        if (y == 7)
        {
            boardImage += "#############";
        }
        if (y == 6)
        {
            boardImage += "#   LINES    ";
        }
        if (y == 5)
        {
            boardImage += "#  CLEARED   ";
        }
        if (y == 4)
        {
            boardImage += "#------------";
        }
        if (y == 3)
        {
            if (totalLinesCleared < 10)
            {
                boardImage += "#     ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "      ";
            }
            else if (totalLinesCleared < 100)
            {
                boardImage += "#     ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "     ";
            }
            else if (totalLinesCleared < 1000)
            {
                boardImage += "#    ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "     ";
            }
            else if (totalLinesCleared < 10000)
            {
                boardImage += "#    ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "    ";
            }
            else if (totalLinesCleared < 100000)
            {
                boardImage += "#   ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "    ";
            }
            else if (totalLinesCleared < 1000000)
            {
                boardImage += "#   ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "   ";
            }
            else if (totalLinesCleared < 10000000)
            {
                boardImage += "#  ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "   ";
            }
            else if (totalLinesCleared < 100000000)
            {
                boardImage += "#  ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "  ";
            }
            else if (totalLinesCleared < 1000000000)
            {
                boardImage += "# ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += "  ";
            }
            else if (totalLinesCleared < 10000000000)
            {
                boardImage += "# ";
                boardImage += std::to_string(totalLinesCleared);
                boardImage += " ";
            }

        }
        else if (y == 2)
        {
            boardImage += "#    NEXT    ";
        }
        else if (y == 1)
        {
            boardImage += "#  LEVEL AT  ";
        }
        else if (y == 0)
        {
            if (nextLevel < 10)
            {
                boardImage += "#     ";
                boardImage += std::to_string(nextLevel);
                boardImage += "      ";
            }
            else if (nextLevel < 100)
            {
                boardImage += "#     ";
                boardImage += std::to_string(nextLevel);
                boardImage += "     ";
            }
            else if (nextLevel < 1000)
            {
                boardImage += "#    ";
                boardImage += std::to_string(nextLevel);
                boardImage += "     ";
            }
            else if (nextLevel < 10000)
            {
                boardImage += "#    ";
                boardImage += std::to_string(nextLevel);
                boardImage += "    ";
            }
            else if (nextLevel < 100000)
            {
                boardImage += "#   ";
                boardImage += std::to_string(nextLevel);
                boardImage += "    ";
            }
            else if (nextLevel < 1000000)
            {
                boardImage += "#   ";
                boardImage += std::to_string(nextLevel);
                boardImage += "   ";
            }
            else if (nextLevel < 10000000)
            {
                boardImage += "#  ";
                boardImage += std::to_string(nextLevel);
                boardImage += "   ";
            }
            else if (nextLevel < 100000000)
            {
                boardImage += "#  ";
                boardImage += std::to_string(nextLevel);
                boardImage += "  ";
            }
            else if (nextLevel < 1000000000)
            {
                boardImage += "# ";
                boardImage += std::to_string(nextLevel);
                boardImage += "  ";
            }
            else if (nextLevel < 10000000000)
            {
                boardImage += "# ";
                boardImage += std::to_string(nextLevel);
                boardImage += " ";
            }
        }
        boardImage += "#\n";
        for (int i = 0; i < tabs; i++)
        {
            boardImage += "\t";
        }
        for (int i = 0; i < spaces; i++)
        {
            boardImage += " ";
        }
        boardImage += "#";
    }

    boardImage += "##################################";
    for (int i = 0; i < endLines; i++)
    {
        boardImage += "\n";
    }

}

void checkControls()
{
    if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
    {
        if (!leftHeld)
        {
            left = true;
            leftHeld = true;

            //std::cout << "AGONY!";
        }
    }
    else
    {
        leftHeld = false;
    }

    if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
    {
        if (!rightHeld)
        {
            right = true;
            rightHeld = true;

            //std::cout << "PAIN!";
        }
    }
    else
    {
        rightHeld = false;
    }

    if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
    {
        if (!spinLeftHeld)
        {
            spinLeft = true;
            spinLeftHeld = true;

            //std::cout << "F***!";
        }
    }
    else
    {
        spinLeftHeld = false;
    }

    if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
    {
        if (!spinRightHeld)
        {
            spinRight = true;
            spinRightHeld = true;

            //std::cout << "DANGIT!";
        }
    }
    else
    {
        spinRightHeld = false;
    }

    if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
    {
        if (!hardDropHeld)
        {
            hardDrop = true;
            hardDropHeld = true;

            //std::cout << "Ouch!";
        }
    }
    else
    {
        hardDropHeld = false;
    }

    if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
    {
        if (!fastDropHeld)
        {
            fastDrop = true;
            //std::cout << "DEAR LORD!";
        }
    }
    else
    {
        fastDropHeld = false;
    }

}

void checkBlocks(int x, int y)
{
    wouldClip = false;

    if (block[0] + x > GAMETILESX - 1 || block[0] + x < 0 || block[2] + x > GAMETILESX - 1 || block[2] + x < 0 || block[4] + x > GAMETILESX - 1 || block[4] + x < 0 || block[6] + x > GAMETILESX - 1 || block[6] + x < 0 || block[1] + y > GAMETILESY - 1 || block[1] + y < 0 || block[3] + y > GAMETILESY - 1 || block[3] + y < 0 || block[5] + y > GAMETILESY - 1 || block[5] + y < 0 || block[7] + y > GAMETILESY - 1 || block[7] + y < 0 || gameBoard[block[0] + x][block[1] + y] || gameBoard[block[2] + x][block[3] + y] || gameBoard[block[4] + x][block[5] + y] || gameBoard[block[6] + x][block[7] + y])
    {
        wouldClip = true;
    }

}

void checkRL()
{
    if (right && left)
    {
        right = false;
        left = false;
        leftHeld = false;
        rightHeld = false;
    }


    if (right)
    {
        checkBlocks(1, 0);
        if (!wouldClip)
        {
            moveBlock(1, 0);

            right = false;
        }
    }

    if (left)
    {
        checkBlocks(-1, 0);
        if (!wouldClip)
        {
            moveBlock(-1, 0);

            left = false;
        }
    }

}

void createBlock()
{

    currentBlock = nextBlock;
    nextBlock = rand() % 7;
    spawnBlock = false;

    switch (currentBlock)
    {
    case 0:
        buildBlock(square, 4, 18);
        break;
    case 1:
        buildBlock(line, 3, 18);
        buildLineFixer();
        lineRotationState = 0;
        break;
    case 2:
        buildBlock(lBlock, 3, 18);
        break;
    case 3:
        buildBlock(reverseLBlock, 3, 18);
        break;
    case 4:
        buildBlock(tBlock, 3, 18);
        break;
    case 5:
        buildBlock(squigly, 3, 18);
        break;
    case 6:
        buildBlock(reverseSquigly, 3, 18);
        break;
    default:
        buildBlock(nullBlock, 0, 19);
        break;

    }

}
void buildBlock(int blockType[8], int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        block[i] = blockType[i];
        if (i == 0 || i == 2 || i == 4 || i == 6)
        {
            block[i] += x;
        }
        else
        {
            block[i] += y;
        }
    }

}
void moveBlock(int x, int y)
{
    block[0] += x;
    block[2] += x;
    block[4] += x;
    block[6] += x;

    block[1] += y;
    block[3] += y;
    block[5] += y;
    block[7] += y;

    if (currentBlock == 1)
    {
        lineFixer[0] += x;
        lineFixer[2] += x;
        lineFixer[4] += x;
        lineFixer[6] += x;

        lineFixer[1] += y;
        lineFixer[3] += y;
        lineFixer[5] += y;
        lineFixer[7] += y;
    }



}
void spawnDropShadow()
{
    wouldClip = false;
    for (int i = 0; !wouldClip; i--)
    {
        checkBlocks(0, i);

        if (wouldClip)
        {
            i++;
            moveShadow(i);
        }
    }




}

void moveShadow(int y)
{
    dropShadow[0] = block[0];
    dropShadow[2] = block[2];
    dropShadow[4] = block[4];
    dropShadow[6] = block[6];

    dropShadow[1] = block[1] + y;
    dropShadow[3] = block[3] + y;
    dropShadow[5] = block[5] + y;
    dropShadow[7] = block[7] + y;
}

void checkRotation()
{

    if (currentBlock > 1)
    {
        rotationType1();
    }
    else if (currentBlock == 0)
    {
        spinRight = false;
        spinLeft = false;
    }
    else if (currentBlock == 1)
    {
        rotationType2();
    }

}

void rotationType1()
{
    wouldClip = false;

    for (int i = 0; i < 8; i++)
    {
        blockDataHold[i] = block[i];
    }


    if (spinRight && spinLeft)
    {
        spinRight = false;
        spinLeft = false;
        spinRightHeld = false;
        spinLeftHeld = false;
    }
    else if (spinRight)
    {
        for (int i = 2; i < 7; i += 2)
        {
            if (block[i + 1] > block[1])
            {

                if (block[i] > block[0])
                {
                    block[i + 1] += -2;
                }
                else if (block[i] == block[0])
                {
                    block[i + 1] += -1;
                    block[i] += 1;
                }
                else if (block[i] < block[0])
                {
                    block[i] += 2;
                }

            }
            else if (block[i + 1] < block[1])
            {

                if (block[i] > block[0])
                {
                    block[i] += -2;
                }
                else if (block[i] == block[0])
                {
                    block[i + 1] += 1;
                    block[i] += -1;
                }
                else if (block[i] < block[0])
                {
                    block[i + 1] += 2;
                }

            }
            else if (block[i + 1] == block[1])
            {
                if (block[i] > block[0])
                {
                    block[i] += -1;
                    block[i + 1] += -1;
                }
                else if (block[i] < block[0])
                {
                    block[i] += 1;
                    block[i + 1] += 1;
                }
            }

        }

        blockPopOut1();
    }
    else if (spinLeft)
    {
        for (int i = 2; i < 7; i += 2)
        {
            if (block[i + 1] > block[1])
            {

                if (block[i] > block[0])
                {
                    block[i] += -2;
                }
                else if (block[i] == block[0])
                {
                    block[i + 1] += -1;
                    block[i] += -1;
                }
                else if (block[i] < block[0])
                {
                    block[i + 1] += -2;
                }

            }
            else if (block[i + 1] < block[1])
            {

                if (block[i] > block[0])
                {
                    block[i + 1] += 2;
                }
                else if (block[i] == block[0])
                {
                    block[i + 1] += 1;
                    block[i] += 1;
                }
                else if (block[i] < block[0])
                {
                    block[i] += 2;
                }

            }
            else if (block[i + 1] == block[1])
            {
                if (block[i] > block[0])
                {
                    block[i] += -1;
                    block[i + 1] += 1;
                }
                else if (block[i] < block[0])
                {
                    block[i] += 1;
                    block[i + 1] += -1;
                }
            }

        }

        blockPopOut1();
    }

    if (wouldClip)
    {
        for (int i = 0; i < 8; i++)
        {
            block[i] = blockDataHold[i];
        }
    }
}

void placeBlock()
{
    for (int i = 0; i < 7; i += 2)
    {
        gameBoard[block[i]][block[i + 1]] = true;
    }
}

void clearLines()
{
    linesKilled = 0;
    for (int y = 0; y < GAMETILESY; y++)
    {
        if (gameBoard[0][y] && gameBoard[1][y] && gameBoard[2][y] && gameBoard[3][y] && gameBoard[4][y] && gameBoard[5][y] && gameBoard[6][y] && gameBoard[7][y] && gameBoard[8][y] && gameBoard[9][y])
        {
            linesKilled += 1;
            totalLinesCleared += 1;
            for (int x = 0; x < GAMETILESX; x++)
            {
                gameBoard[x][y] = false;
            }

            for (int z = y; z < GAMETILESY - 1; z++)
            {

                for (int x = 0; x < GAMETILESX; x++)
                {
                    gameBoard[x][z] = gameBoard[x][z + 1];
                }
            }

            for (int x = 0; x < GAMETILESX; x++)
            {
                gameBoard[x][GAMETILESY - 1] = false;
            }


            y--;
        }
    }

    if (difficultyType == 0)
    {
        if (linesKilled == 1)
        {
            score += 100;
            checkLevel();
        }
        if (linesKilled == 2)
        {
            score += 250;
            checkLevel();
        }
        if (linesKilled == 3)
        {
            score += 400;
            checkLevel();
        }
        if (linesKilled > 3)
        {
            score += 600;
            checkLevel();
        }
    }
    else if (difficultyType == 1)
    {
        if (linesKilled == 1)
        {
            score += 40 * level;
            checkLevel();
        }
        if (linesKilled == 2)
        {
            score += 100 * level;
            checkLevel();
        }
        if (linesKilled == 3)
        {
            score += 300 * level;
            checkLevel();
        }
        if (linesKilled > 3)
        {
            score += 1200 * level;
            checkLevel();
        }
    }
}

void buildLineFixer()
{
    for (int i = 0; i < 8; i++)
    {
        lineFixer[i] = square[i];
        if (i == 0 || i == 2 || i == 4 || i == 6)
        {
            lineFixer[i] += 4;
        }
        else
        {
            lineFixer[i] += 18;
        }
    }
}

void rotationType2()
{
    wouldClip = false;

    for (int i = 0; i < 8; i++)
    {
        blockDataHold[i] = block[i];
    }

    if (spinRight && spinLeft)
    {
        spinLeft = false;
        spinRight = false;
        spinRightHeld = false;
        spinLeftHeld = false;
    }
    else if (spinRight)
    {
        lineRotationState += 1;
        lineRotationLine();
        updateLine();
        blockPopOut1();
        blockPopOut2();
    }
    else if (spinLeft)
    {
        lineRotationState -= 1;
        lineRotationLine();
        updateLine();
        blockPopOut1();
        blockPopOut2();
    }



    if (wouldClip)
    {
        for (int i = 0; i < 8; i++)
        {
            block[i] = blockDataHold[i];
        }
        if (spinLeft)
        {
            lineRotationState += 1;
            lineRotationLine();
        }
        if (spinRight)
        {
            lineRotationState -= 1;
            lineRotationLine();
        }

    }
    else
    {
        spinRight = false;
        spinLeft = false;
    }


}
void lineRotationLine()
{
    if (lineRotationState > 3)
    {
        lineRotationState = 0;
    }
    else if (lineRotationState < 0)
    {
        lineRotationState = 3;
    }
}

void updateLine()
{
    if (lineRotationState == 0)
    {
        block[0] = lineFixer[2];
        block[1] = lineFixer[3];
        block[2] = lineFixer[6];
        block[3] = lineFixer[7];

        block[4] = block[0] - 1;
        block[5] = block[1];
        block[6] = block[2] + 1;
        block[7] = block[3];
    }
    else if (lineRotationState == 1)
    {
        block[0] = lineFixer[4];
        block[1] = lineFixer[5];
        block[2] = lineFixer[6];
        block[3] = lineFixer[7];

        block[4] = block[0];
        block[5] = block[1] - 1;
        block[6] = block[2];
        block[7] = block[3] + 1;
    }
    else if (lineRotationState == 2)
    {
        block[0] = lineFixer[0];
        block[1] = lineFixer[1];
        block[2] = lineFixer[4];
        block[3] = lineFixer[5];

        block[4] = block[0] - 1;
        block[5] = block[1];
        block[6] = block[2] + 1;
        block[7] = block[3];
    }
    else if (lineRotationState == 3)
    {
        block[0] = lineFixer[2];
        block[1] = lineFixer[3];
        block[2] = lineFixer[0];
        block[3] = lineFixer[1];

        block[4] = block[0];
        block[5] = block[1] + 1;
        block[6] = block[2];
        block[7] = block[3] - 1;
    }
}
void checkLevel()
{
    if (difficultyType == 0)
    {
        if (totalLinesCleared < 20)
        {
            level = 1;
            currentDifficulty = 60;
            nextLevel = 20;
        }
        else if (totalLinesCleared < 40)
        {
            level = 2;
            currentDifficulty = 40;
            nextLevel = 40;
        }
        else if (totalLinesCleared < 60)
        {
            level = 3;
            currentDifficulty = 30;
            nextLevel = 60;
        }
        else if (totalLinesCleared < 100)
        {
            level = 4;
            currentDifficulty = 20;
            nextLevel = 100;
        }
        else if (totalLinesCleared < 140)
        {
            level = 5;
            currentDifficulty = 15;
            nextLevel = 140;
        }
        else if (totalLinesCleared < 200)
        {
            level = 6;
            currentDifficulty = 12;
            nextLevel = 200;
        }
        else if (totalLinesCleared < 300)
        {
            level = 7;
            currentDifficulty = 10;
            nextLevel = 300;
        }
        else if (totalLinesCleared < 400)
        {
            level = 8;
            currentDifficulty = 8;
            nextLevel = 400;
        }
        else if (totalLinesCleared < 500)
        {
            level = 9;
            currentDifficulty = 6;
            nextLevel = 500;
        }
        else
        {
            level = 10;
            currentDifficulty = 5;
            nextLevel = 0;

        }
    }
    if (difficultyType == 1)
    {
        if (totalLinesCleared < 10)
        {
            level = 1;
            currentDifficulty = 53;
            nextLevel = 10;
        }
        else if (totalLinesCleared < 20)
        {
            level = 2;
            currentDifficulty = 49;
            nextLevel = 20;
        }
        else if (totalLinesCleared < 30)
        {
            level = 3;
            currentDifficulty = 45;
            nextLevel = 30;
        }
        else if (totalLinesCleared < 40)
        {
            level = 4;
            currentDifficulty = 41;
            nextLevel = 40;
        }
        else if (totalLinesCleared < 50)
        {
            level = 5;
            currentDifficulty = 37;
            nextLevel = 50;
        }
        else if (totalLinesCleared < 60)
        {
            level = 6;
            currentDifficulty = 33;
            nextLevel = 60;
        }
        else if (totalLinesCleared < 70)
        {
            level = 7;
            currentDifficulty = 28;
            nextLevel = 70;
        }
        else if (totalLinesCleared < 80)
        {
            level = 8;
            currentDifficulty = 22;
            nextLevel = 80;
        }
        else if (totalLinesCleared < 90)
        {
            level = 9;
            currentDifficulty = 17;
            nextLevel = 90;
        }
        else if (totalLinesCleared < 100)
        {
            level = 10;
            currentDifficulty = 11;
            nextLevel = 100;
        }
        else if (totalLinesCleared < 110)
        {
            level = 11;
            currentDifficulty = 10;
            nextLevel = 110;
        }
        else if (totalLinesCleared < 120)
        {
            level = 12;
            currentDifficulty = 9;
            nextLevel = 120;
        }
        else if (totalLinesCleared < 130)
        {
            level = 13;
            currentDifficulty = 8;
            nextLevel = 130;
        }
        else if (totalLinesCleared < 140)
        {
            level = 14;
            currentDifficulty = 7;
            nextLevel = 140;
        }
        else if (totalLinesCleared < 150)
        {
            level = 15;
            currentDifficulty = 6;
            nextLevel = 150;
        }
        else if (totalLinesCleared < 160)
        {
            level = 16;
            currentDifficulty = 6;
            nextLevel = 160;
        }
        else if (totalLinesCleared < 170)
        {
            level = 17;
            currentDifficulty = 5;
            nextLevel = 170;
        }
        else if (totalLinesCleared < 180)
        {
            level = 18;
            currentDifficulty = 5;
            nextLevel = 180;
        }
        else if (totalLinesCleared < 190)
        {
            level = 19;
            currentDifficulty = 4;
            nextLevel = 190;
        }
        else if (totalLinesCleared < 200)
        {
            level = 20;
            currentDifficulty = 4;
            nextLevel = 200;
        }
        else if (totalLinesCleared < 210)
        {
            level = 21;
            currentDifficulty = 3;
            nextLevel = 000;
        }

    }
}
void blockPopOut1()
{

    checkBlocks(0, 0);

    if (wouldClip && spinRight)
    {
        checkBlocks(1, 0);

        if (!wouldClip)
        {
            moveBlock(1, 0);
        }
    }
    else if (wouldClip && spinLeft)
    {
        checkBlocks(-1, 0);

        if (!wouldClip)
        {
            moveBlock(-1, 0);
        }
    }

    if (wouldClip && spinRight)
    {
        checkBlocks(-1, 0);

        if (!wouldClip)
        {
            moveBlock(-1, 0);
        }
    }
    else if (wouldClip && spinLeft)
    {
        checkBlocks(1, 0);

        if (!wouldClip)
        {
            moveBlock(1, 0);
        }
    }

    if (wouldClip)
    {
        checkBlocks(0, 1);

        if (!wouldClip)
        {
            moveBlock(0, 1);
        }
    }

    if (wouldClip)
    {
        checkBlocks(0, -1);

        if (!wouldClip)
        {
            moveBlock(0, -1);
        }
    }

    if (!wouldClip)
    {
        spinRight = false;
        spinLeft = false;
    }
}

void blockPopOut2()
{
    if (spinRight)
    {
        lineRotationState -= 1;
    }
    else if (spinLeft)
    {
        lineRotationState += 1;
    }
    lineRotationLine();


    if (wouldClip && lineRotationState == 0)
    {
        checkBlocks(0, 2);

        if (!wouldClip)
        {
            moveBlock(0, 2);
        }
    }
    else if (wouldClip && lineRotationState == 2)
    {
        checkBlocks(0, -2);

        if (!wouldClip)
        {
            moveBlock(0, -2);
        }
    }
    else if (wouldClip && lineRotationState == 1)
    {
        checkBlocks(2, 0);

        if (!wouldClip)
        {
            moveBlock(2, 0);
        }
    }
    else if (wouldClip && lineRotationState == 3)
    {
        checkBlocks(-2, 0);

        if (!wouldClip)
        {
            moveBlock(-2, 0);
        }
    }

    if (spinRight)
    {
        lineRotationState += 1;
    }
    else if (spinLeft)
    {
        lineRotationState -= 1;
    }
    lineRotationLine();

    if (!wouldClip)
    {
        spinRight = false;
        spinLeft = false;
    }
}

void drawMenuScreen()
{
    boardImage = "\n";
    for (int i = 0; i < startLines; i++)
    {
        boardImage += "\n";
    }
    for (int i = 0; i < tabs; i++)
    {
        boardImage += "\t";
    }
    for (int i = 0; i < spaces; i++)
    {
        boardImage += " ";
    }
    boardImage += "###################################\n";
    for (int i = 0; i < tabs; i++)
    {
        boardImage += "\t";
    }
    for (int i = 0; i < spaces; i++)
    {
        boardImage += " ";
    }
    boardImage += "#";
    for (int y = 19; y >= 0; y--)
    {
        for (int x = 0; x < 33; x++)
        {
            boardImage += menuDrawing[x][y];
        }
        boardImage += "#\n";
        for (int i = 0; i < tabs; i++)
        {
            boardImage += "\t";
        }
        for (int i = 0; i < spaces; i++)
        {
            boardImage += " ";
        }
        boardImage += "#";
    }
    boardImage += "##################################";
    for (int i = 0; i < endLines; i++)
    {
        boardImage += "\n";
    }
}
//Note add block rotation type 2 with other line peice movements for more responsive controls???

void clearMenuDrawing()
{
    for (int x = 0; x < 33; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            menuDrawing[x][y] = ' ';
        }
    }
}

void createBGBlock()
{
    bgPiece tempy;
    bgBlockVector.push_back(tempy);
}

void drawMCTLogo()
{
    for (int x = 6; x < 28; x++)
    {
        for (int y = 19; y > 9; y--)
        {
            menuDrawing[x][y] = ' ';
        }
    }

    menuDrawing[8][18] = '@';
    menuDrawing[8][17] = '@';
    menuDrawing[8][16] = '@';
    menuDrawing[8][15] = '@';
    menuDrawing[8][14] = '@';
    menuDrawing[8][13] = '@';
    menuDrawing[9][18] = '@';
    menuDrawing[10][17] = '@';
    menuDrawing[10][16] = '@';
    menuDrawing[11][18] = '@';
    menuDrawing[12][18] = '@';
    menuDrawing[12][17] = '@';
    menuDrawing[12][16] = '@';
    menuDrawing[12][15] = '@';
    menuDrawing[12][14] = '@';
    menuDrawing[12][13] = '@';

    menuDrawing[14][17] = '@';
    menuDrawing[15][18] = '@';
    menuDrawing[15][17] = '@';
    menuDrawing[16][18] = '@';
    menuDrawing[17][18] = '@';
    menuDrawing[18][17] = '@';
    menuDrawing[14][16] = '@';
    menuDrawing[14][15] = '@';
    menuDrawing[14][14] = '@';
    menuDrawing[15][14] = '@';
    menuDrawing[15][13] = '@';
    menuDrawing[16][13] = '@';
    menuDrawing[17][13] = '@';
    menuDrawing[18][14] = '@';

    menuDrawing[20][18] = '@';
    menuDrawing[21][18] = '@';
    menuDrawing[22][18] = '@';
    menuDrawing[22][17] = '@';
    menuDrawing[22][16] = '@';
    menuDrawing[22][15] = '@';
    menuDrawing[22][14] = '@';
    menuDrawing[22][13] = '@';
    menuDrawing[23][18] = '@';
    menuDrawing[24][18] = '@';

    menuDrawing[10][11] = '=';
    menuDrawing[11][11] = '=';
    menuDrawing[12][11] = '=';
    menuDrawing[13][11] = '=';
    menuDrawing[14][11] = '=';
    menuDrawing[15][11] = '=';
    menuDrawing[16][11] = '=';
    menuDrawing[17][11] = '=';
    menuDrawing[18][11] = '=';
    menuDrawing[19][11] = '=';
    menuDrawing[20][11] = '=';
    menuDrawing[21][11] = '=';
    menuDrawing[22][11] = '=';

    menuDrawing[11][10] = 'V';
    menuDrawing[12][10] = 'E';
    menuDrawing[13][10] = 'R';
    menuDrawing[14][10] = 'S';
    menuDrawing[15][10] = 'I';
    menuDrawing[16][10] = 'O';
    menuDrawing[17][10] = 'N';
    menuDrawing[19][10] = '2';
    menuDrawing[20][10] = '.';
    menuDrawing[21][10] = '2';

    menuDrawing[10][9] = '=';
    menuDrawing[11][9] = '=';
    menuDrawing[12][9] = '=';
    menuDrawing[13][9] = '=';
    menuDrawing[14][9] = '=';
    menuDrawing[15][9] = '=';
    menuDrawing[16][9] = '=';
    menuDrawing[17][9] = '=';
    menuDrawing[18][9] = '=';
    menuDrawing[19][9] = '=';
    menuDrawing[20][9] = '=';
    menuDrawing[21][9] = '=';
    menuDrawing[22][9] = '=';

    menuDrawing[23][11] = '|';
    menuDrawing[23][10] = '|';
    menuDrawing[23][9] = '|';
    menuDrawing[9][11] = '|';
    menuDrawing[9][10] = '|';
    menuDrawing[9][9] = '|';

    menuDrawing[24][11] = '=';
    menuDrawing[8][11] = '=';
    menuDrawing[24][9] = '=';
    menuDrawing[8][9] = '=';
    menuDrawing[25][11] = '=';
    menuDrawing[7][11] = '=';
    menuDrawing[25][9] = '=';
    menuDrawing[7][9] = '=';

    menuDrawing[6][9] = '|';
    menuDrawing[6][10] = '|';
    menuDrawing[6][11] = '|';
    menuDrawing[26][9] = '|';
    menuDrawing[26][10] = '|';
    menuDrawing[26][11] = '|';

    menuDrawing[26][12] = '|';
    menuDrawing[26][13] = '|';
    menuDrawing[26][14] = '|';
    menuDrawing[26][15] = '|';
    menuDrawing[26][16] = '|';
    menuDrawing[26][17] = '|';
    menuDrawing[26][18] = '|';
    menuDrawing[26][19] = '|';

    menuDrawing[6][12] = '|';
    menuDrawing[6][13] = '|';
    menuDrawing[6][14] = '|';
    menuDrawing[6][15] = '|';
    menuDrawing[6][16] = '|';
    menuDrawing[6][17] = '|';
    menuDrawing[6][18] = '|';
    menuDrawing[6][19] = '|';

    menuDrawing[7][19] = '=';
    menuDrawing[8][19] = '=';
    menuDrawing[9][19] = '=';
    menuDrawing[10][19] = '=';
    menuDrawing[11][19] = '=';
    menuDrawing[12][19] = '=';
    menuDrawing[13][19] = '=';
    menuDrawing[14][19] = '=';
    menuDrawing[15][19] = '=';
    menuDrawing[16][19] = '=';
    menuDrawing[17][19] = '=';
    menuDrawing[18][19] = '=';
    menuDrawing[19][19] = '=';
    menuDrawing[20][19] = '=';
    menuDrawing[21][19] = '=';
    menuDrawing[22][19] = '=';
    menuDrawing[23][19] = '=';
    menuDrawing[24][19] = '=';
    menuDrawing[25][19] = '=';

    menuDrawing[27][9] = '=';
    menuDrawing[5][9] = '=';
    menuDrawing[28][9] = '=';
    menuDrawing[4][9] = '=';
    menuDrawing[29][9] = '=';
    menuDrawing[3][9] = '=';
    menuDrawing[30][9] = '=';
    menuDrawing[2][9] = '=';
    menuDrawing[31][9] = '=';
    menuDrawing[1][9] = '=';
    menuDrawing[32][9] = '=';
    menuDrawing[0][9] = '=';

    menuDrawing[27][11] = '=';
    menuDrawing[5][11] = '=';
    menuDrawing[28][11] = '=';
    menuDrawing[4][11] = '=';
    menuDrawing[29][11] = '=';
    menuDrawing[3][11] = '=';
    menuDrawing[30][11] = '=';
    menuDrawing[2][11] = '=';
    menuDrawing[31][11] = '=';
    menuDrawing[1][11] = '=';
    menuDrawing[32][11] = '=';
    menuDrawing[0][11] = '=';

    menuDrawing[27][19] = '=';
    menuDrawing[5][19] = '=';
    menuDrawing[28][19] = '=';
    menuDrawing[4][19] = '=';
    menuDrawing[29][19] = '=';
    menuDrawing[3][19] = '=';
    menuDrawing[30][19] = '=';
    menuDrawing[2][19] = '=';
    menuDrawing[31][19] = '=';
    menuDrawing[1][19] = '=';
    menuDrawing[32][19] = '=';
    menuDrawing[0][19] = '=';

    menuDrawing[12][7] = 'P';
    menuDrawing[13][7] = 'L';
    menuDrawing[14][7] = 'A';
    menuDrawing[15][7] = 'Y';
    menuDrawing[16][7] = ' ';
    menuDrawing[17][7] = 'G';
    menuDrawing[18][7] = 'A';
    menuDrawing[19][7] = 'M';
    menuDrawing[20][7] = 'E';
    if (menuItem == 0)
    {
        menuDrawing[11][7] = '(';
        menuDrawing[21][7] = ')';
    }

    menuDrawing[10][5] = 'V';
    menuDrawing[11][5] = 'E';
    menuDrawing[12][5] = 'I';
    menuDrawing[13][5] = 'W';
    menuDrawing[14][5] = ' ';
    menuDrawing[15][5] = 'C';
    menuDrawing[16][5] = 'O';
    menuDrawing[17][5] = 'N';
    menuDrawing[18][5] = 'T';
    menuDrawing[19][5] = 'R';
    menuDrawing[20][5] = 'O';
    menuDrawing[21][5] = 'L';
    menuDrawing[22][5] = 'S';
    if (menuItem == 1)
    {
        menuDrawing[9][5] = '(';
        menuDrawing[23][5] = ')';
    }

    menuDrawing[13][3] = 'O';
    menuDrawing[14][3] = 'P';
    menuDrawing[15][3] = 'T';
    menuDrawing[16][3] = 'I';
    menuDrawing[17][3] = 'O';
    menuDrawing[18][3] = 'N';
    menuDrawing[19][3] = 'S';
    if (menuItem == 2)
    {
        menuDrawing[12][3] = '(';
        menuDrawing[20][3] = ')';
    }

    menuDrawing[12][1] = 'Q';
    menuDrawing[13][1] = 'U';
    menuDrawing[14][1] = 'I';
    menuDrawing[15][1] = 'T';
    menuDrawing[16][1] = ' ';
    menuDrawing[17][1] = 'G';
    menuDrawing[18][1] = 'A';
    menuDrawing[19][1] = 'M';
    menuDrawing[20][1] = 'E';
    if (menuItem == 3)
    {
        menuDrawing[11][1] = '(';
        menuDrawing[21][1] = ')';
    }

}

void checkMenuControls()
{
    if (fastDrop == true)
    {
        menuItem++;
        fastDrop = false;
        fastDropHeld = true;
    }
    else if (hardDrop == true)
    {
        menuItem--;
        hardDrop = false;
    }
    if (menuItem > 3)
    {
        menuItem = 0;
    }
    else if (menuItem < 0)
    {
        menuItem = 3;
    }

    if (right && menuItem == 3)
    {
        quitGame = true;
    }
    if (right && menuItem == 0)
    {
        clearBoard();
        gameState = 1;
    }
    if (right && menuItem == 1)
    {
        gameState = 2;
        clearControls();
    }
    if (right && menuItem == 2)
    {
        gameState = 3;
        clearControls();
        menuItem = 0;
    }

    clearControls();
}

void clearControls()
{
    right = false;
    left = false;
    hardDrop = false;
    fastDrop = false;
    spinRight = false;
    spinLeft = false;
}

void drawControlScreen()
{
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 21; x++)
        {
            menuDrawing[x][y] = ' ';
        }
        menuDrawing[21][y] = '|';
    }

    menuDrawing[3][18] = '[';
    menuDrawing[4][18] = 'D';
    menuDrawing[5][18] = ']';
    menuDrawing[6][18] = '=';
    menuDrawing[7][18] = 'R';
    menuDrawing[8][18] = 'I';
    menuDrawing[9][18] = 'G';
    menuDrawing[10][18] = 'H';
    menuDrawing[11][18] = 'T';

    menuDrawing[3][16] = '[';
    menuDrawing[4][16] = 'A';
    menuDrawing[5][16] = ']';
    menuDrawing[6][16] = '=';
    menuDrawing[7][16] = 'L';
    menuDrawing[8][16] = 'E';
    menuDrawing[9][16] = 'F';
    menuDrawing[10][16] = 'T';

    menuDrawing[3][14] = '[';
    menuDrawing[4][14] = 'S';
    menuDrawing[5][14] = ']';
    menuDrawing[6][14] = '=';
    menuDrawing[7][14] = 'F';
    menuDrawing[8][14] = 'A';
    menuDrawing[9][14] = 'S';
    menuDrawing[10][14] = 'T';
    menuDrawing[11][14] = ' ';
    menuDrawing[12][14] = 'D';
    menuDrawing[13][14] = 'R';
    menuDrawing[14][14] = 'O';
    menuDrawing[15][14] = 'P';

    menuDrawing[3][12] = '[';
    menuDrawing[4][12] = 'W';
    menuDrawing[5][12] = ']';
    menuDrawing[6][12] = '=';
    menuDrawing[7][12] = 'H';
    menuDrawing[8][12] = 'A';
    menuDrawing[9][12] = 'R';
    menuDrawing[10][12] = 'D';
    menuDrawing[11][12] = ' ';
    menuDrawing[12][12] = 'D';
    menuDrawing[13][12] = 'R';
    menuDrawing[14][12] = 'O';
    menuDrawing[15][12] = 'P';

    menuDrawing[3][10] = '[';
    menuDrawing[4][10] = 'E';
    menuDrawing[5][10] = ']';
    menuDrawing[6][10] = '=';
    menuDrawing[7][10] = 'S';
    menuDrawing[8][10] = 'P';
    menuDrawing[9][10] = 'I';
    menuDrawing[10][10] = 'N';
    menuDrawing[11][10] = ' ';
    menuDrawing[12][10] = 'R';
    menuDrawing[13][10] = 'I';
    menuDrawing[14][10] = 'G';
    menuDrawing[15][10] = 'H';
    menuDrawing[16][10] = 'T';

    menuDrawing[3][8] = '[';
    menuDrawing[4][8] = 'Q';
    menuDrawing[5][8] = ']';
    menuDrawing[6][8] = '=';
    menuDrawing[7][8] = 'S';
    menuDrawing[8][8] = 'P';
    menuDrawing[9][8] = 'I';
    menuDrawing[10][8] = 'N';
    menuDrawing[11][8] = ' ';
    menuDrawing[12][8] = 'L';
    menuDrawing[13][8] = 'E';
    menuDrawing[14][8] = 'F';
    menuDrawing[15][8] = 'T';

    menuDrawing[3][6] = 'P';
    menuDrawing[4][6] = 'R';
    menuDrawing[5][6] = 'E';
    menuDrawing[6][6] = 'S';
    menuDrawing[7][6] = 'S';
    menuDrawing[8][6] = ' ';
    menuDrawing[9][6] = 'R';
    menuDrawing[10][6] = 'I';
    menuDrawing[11][6] = 'G';
    menuDrawing[12][6] = 'H';
    menuDrawing[13][6] = 'T';
    menuDrawing[14][6] = ' ';
    menuDrawing[15][6] = 'A';
    menuDrawing[16][6] = 'S';

    menuDrawing[3][5] = 'Y';
    menuDrawing[4][5] = 'O';
    menuDrawing[5][5] = 'U';
    menuDrawing[6][5] = ' ';
    menuDrawing[7][5] = 'W';
    menuDrawing[8][5] = 'O';
    menuDrawing[9][5] = 'U';
    menuDrawing[10][5] = 'L';
    menuDrawing[11][5] = 'D';
    menuDrawing[12][5] = ' ';
    menuDrawing[13][5] = 'E';
    menuDrawing[14][5] = 'N';
    menuDrawing[15][5] = 'T';
    menuDrawing[16][5] = 'E';
    menuDrawing[17][5] = 'R';
    menuDrawing[18][5] = '.';

    menuDrawing[3][3] = 'P';
    menuDrawing[4][3] = 'R';
    menuDrawing[5][3] = 'E';
    menuDrawing[6][3] = 'S';
    menuDrawing[7][3] = 'S';
    menuDrawing[8][3] = ' ';
    menuDrawing[9][3] = 'R';
    menuDrawing[10][3] = 'I';
    menuDrawing[11][3] = 'G';
    menuDrawing[12][3] = 'H';
    menuDrawing[13][3] = 'T';
    menuDrawing[14][3] = ' ';
    menuDrawing[15][3] = 'N';
    menuDrawing[16][3] = 'O';
    menuDrawing[17][3] = 'W';

    menuDrawing[3][2] = 'T';
    menuDrawing[4][2] = 'O';
    menuDrawing[5][2] = ' ';
    menuDrawing[6][2] = 'G';
    menuDrawing[7][2] = 'O';
    menuDrawing[8][2] = ' ';
    menuDrawing[9][2] = 'T';
    menuDrawing[10][2] = 'O';
    menuDrawing[11][2] = ' ';
    menuDrawing[12][2] = 'T';
    menuDrawing[13][2] = 'H';
    menuDrawing[14][2] = 'E';

    menuDrawing[4][1] = 'M';
    menuDrawing[5][1] = 'A';
    menuDrawing[6][1] = 'I';
    menuDrawing[7][1] = 'N';
    menuDrawing[8][1] = ' ';
    menuDrawing[9][1] = 'M';
    menuDrawing[10][1] = 'E';
    menuDrawing[11][1] = 'N';
    menuDrawing[12][1] = 'U';
    menuDrawing[13][1] = '.';

}

void checkOptionsControls()
{
    if (fastDrop == true)
    {
        menuItem++;
        fastDrop = false;
        fastDropHeld = true;
    }
    else if (hardDrop == true)
    {
        menuItem--;
        hardDrop = false;
    }
    if (menuItem > 5)
    {
        menuItem = 0;
    }
    else if (menuItem < 0)
    {
        menuItem = 5;
    }

    if (menuItem == 5 && right)
    {
        clearControls();
        menuItem = 2;
        gameState = 0;
    }

    if (menuItem == 0 && right)
    {
        tabs++;
    }
    if (menuItem == 0 && left && tabs > 0)
    {
        tabs--;
    }
    if (menuItem == 1 && right)
    {
        spaces++;
    }
    if (menuItem == 1 && left && spaces > 0)
    {
        spaces--;
    }
    if (menuItem == 2 && right)
    {
        startLines++;
    }
    if (menuItem == 2 && left && startLines > 0)
    {
        startLines--;
    }
    if (menuItem == 3 && right)
    {
        endLines++;
    }
    if (menuItem == 3 && left && endLines > 0)
    {
        endLines--;
    }
    if (menuItem == 4 && right)
    {
        difficultyType = 1;
    }
    else if (menuItem == 4 && left)
    {
        difficultyType = 0;
    }

    clearControls();
}

void drawOptionsScreen()
{
    menuDrawing[25][5] = '|';
    menuDrawing[7][5] = '|';

    for (int y = 0; y < 20; y++)
    {
        for (int x = 7; x < 26; x++)
        {
            menuDrawing[x][y] = ' ';
        }

        menuDrawing[25][y] = '|';
        menuDrawing[7][y] = '|';
    }



    menuDrawing[12][17] = 'T';
    menuDrawing[13][17] = 'A';
    menuDrawing[14][17] = 'B';
    menuDrawing[15][17] = 'S';
    menuDrawing[17][17] = 'O';
    menuDrawing[18][17] = 'V';
    menuDrawing[19][17] = 'E';
    menuDrawing[20][17] = 'R';

    menuDrawing[14][16] = '[';
    menuDrawing[15][16] = numToChar(tabs, 3);
    menuDrawing[16][16] = numToChar(tabs, 2);
    menuDrawing[17][16] = numToChar(tabs, 1);
    menuDrawing[18][16] = ']';

    if (menuItem == 0)
    {
        menuDrawing[18][16] = '[';
        menuDrawing[14][16] = ']';
        menuDrawing[13][16] = '(';
        menuDrawing[19][16] = ')';
        menuDrawing[21][17] = ')';
        menuDrawing[11][17] = '(';
    }

    menuDrawing[11][14] = 'S';
    menuDrawing[12][14] = 'P';
    menuDrawing[13][14] = 'A';
    menuDrawing[14][14] = 'C';
    menuDrawing[15][14] = 'E';
    menuDrawing[16][14] = 'S';
    menuDrawing[18][14] = 'O';
    menuDrawing[19][14] = 'V';
    menuDrawing[20][14] = 'E';
    menuDrawing[21][14] = 'R';

    menuDrawing[14][13] = '[';
    menuDrawing[15][13] = numToChar(spaces, 3);
    menuDrawing[16][13] = numToChar(spaces, 2);
    menuDrawing[17][13] = numToChar(spaces, 1);
    menuDrawing[18][13] = ']';

    if (menuItem == 1)
    {
        menuDrawing[18][13] = '[';
        menuDrawing[14][13] = ']';
        menuDrawing[13][13] = '(';
        menuDrawing[19][13] = ')';
        menuDrawing[10][14] = '(';
        menuDrawing[22][14] = ')';
    }

    menuDrawing[11][11] = 'L';
    menuDrawing[12][11] = 'I';
    menuDrawing[13][11] = 'N';
    menuDrawing[14][11] = 'E';
    menuDrawing[15][11] = 'S';
    menuDrawing[18][11] = 'D';
    menuDrawing[19][11] = 'O';
    menuDrawing[20][11] = 'W';
    menuDrawing[21][11] = 'N';

    menuDrawing[14][10] = '[';
    menuDrawing[15][10] = numToChar(startLines, 3);
    menuDrawing[16][10] = numToChar(startLines, 2);
    menuDrawing[17][10] = numToChar(startLines, 1);
    menuDrawing[18][10] = ']';

    if (menuItem == 2)
    {
        menuDrawing[18][10] = '[';
        menuDrawing[14][10] = ']';
        menuDrawing[13][10] = '(';
        menuDrawing[19][10] = ')';
        menuDrawing[10][11] = '(';
        menuDrawing[22][11] = ')';
    }

    menuDrawing[12][8] = 'L';
    menuDrawing[13][8] = 'I';
    menuDrawing[14][8] = 'N';
    menuDrawing[15][8] = 'E';
    menuDrawing[16][8] = 'S';
    menuDrawing[19][8] = 'U';
    menuDrawing[20][8] = 'P';

    menuDrawing[14][7] = '[';
    menuDrawing[15][7] = numToChar(endLines, 3);
    menuDrawing[16][7] = numToChar(endLines, 2);
    menuDrawing[17][7] = numToChar(endLines, 1);
    menuDrawing[18][7] = ']';

    if (menuItem == 3)
    {
        menuDrawing[18][7] = '[';
        menuDrawing[14][7] = ']';
        menuDrawing[13][7] = '(';
        menuDrawing[19][7] = ')';
        menuDrawing[11][8] = '(';
        menuDrawing[21][8] = ')';
    }

    menuDrawing[12][5] = 'G';
    menuDrawing[13][5] = 'A';
    menuDrawing[14][5] = 'M';
    menuDrawing[15][5] = 'E';
    menuDrawing[17][5] = 'T';
    menuDrawing[18][5] = 'Y';
    menuDrawing[19][5] = 'P';
    menuDrawing[20][5] = 'E';

    menuDrawing[10][4] = 'M';
    menuDrawing[11][4] = 'C';
    menuDrawing[12][4] = 'T';
    menuDrawing[13][4] = 'V';
    menuDrawing[14][4] = '1';

    menuDrawing[17][4] = 'T';
    menuDrawing[18][4] = 'E';
    menuDrawing[19][4] = 'T';
    menuDrawing[20][4] = 'R';
    menuDrawing[21][4] = 'I';
    menuDrawing[22][4] = 'S';

    if (menuItem == 4)
    {
        menuDrawing[11][5] = '(';
        menuDrawing[21][5] = ')';

        if (difficultyType == 0)
        {
            menuDrawing[8][4] = '(';
            menuDrawing[16][4] = ')';
            menuDrawing[9][4] = ']';
            menuDrawing[15][4] = '[';
        }
        if (difficultyType == 1)
        {
            menuDrawing[15][4] = '(';
            menuDrawing[24][4] = ')';
            menuDrawing[16][4] = ']';
            menuDrawing[23][4] = '[';
        }
    }
    else
    {
        if (difficultyType == 0)
        {
            menuDrawing[9][4] = '[';
            menuDrawing[15][4] = ']';
        }
        if (difficultyType == 1)
        {
            menuDrawing[16][4] = '[';
            menuDrawing[23][4] = ']';
        }
    }

    menuDrawing[12][2] = 'M';
    menuDrawing[13][2] = 'A';
    menuDrawing[14][2] = 'I';
    menuDrawing[15][2] = 'N';
    menuDrawing[17][2] = 'M';
    menuDrawing[18][2] = 'E';
    menuDrawing[19][2] = 'N';
    menuDrawing[20][2] = 'U';
    if (menuItem == 5)
    {
        menuDrawing[11][2] = '(';
        menuDrawing[21][2] = ')';
    }

}

char numToChar(int numby, int charPoint)
{

    if (charPoint == 1)
    {
        charPoint = 1;
        if (numby == 100)
        {
            numby -= 100;
        }
        if (numby > 100)
        {
            numby = numby % 100;
        }
        if (numby == 10)
        {
            numby -= 10;
        }
        else if (numby > 10)
        {
            numby = numby % 10;
        }

    }
    else if (charPoint == 2)
    {
        charPoint = 10;
        if (numby == 100)
        {
            numby -= 100;
        }
        else if (numby > 100)
        {
            numby = numby % 100;
        }
    }
    else if (charPoint == 3)
    {
        charPoint = 100;
    }



    switch (numby / charPoint)
    {
    case 0:
        return '0';
        break;
    case 1:
        return '1';
        break;
    case 2:
        return '2';
        break;
    case 3:
        return '3';
        break;
    case 4:
        return '4';
        break;
    case 5:
        return '5';
        break;
    case 6:
        return '6';
        break;
    case 7:
        return '7';
        break;
    case 8:
        return '8';
        break;
    case 9:
        return '9';
        break;
    default:
        return '?';
        break;
    }


}
