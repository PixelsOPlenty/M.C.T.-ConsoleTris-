//ConsoleTris
#include<iostream>
#include<chrono>
#include<cstdlib>
#include<string>
#include<Windows.h>


const int GAMETILESX = 10, GAMETILESY = 20, STARTINGDIFFICULTY = 60, FASTDROPSPEED = 2;
bool frameTick = false, spawnBlock, right, rightHeld, left, leftHeld, wouldClip, spinRight, spinRightHeld, spinLeft, spinLeftHeld, hardDrop, hardDropHeld, fastDrop, fastDropHeld, gameOver;
const double FRAMEDELAY = (1.0 / 60.0);
int gameTicksTillDrop, currentDifficulty, currentBlock, nextBlock, moveHorizontal, moveVertical, fastDropTicks, lineRotationState = 0, score, linesKilled, level, totalLinesCleared, nextLevel;
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

int main()
{
    std::cout << "\nWelcome to PixelsOPlenty's M.C.T. 'ConsoleTris' Version 1.1! :D\n\nIf you find any bugs please message me!\n\nEnter any character and then press'Enter' to begin.\n";

    std::cin >> boardImage;

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



    while (!gameOver)
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

        if (frameTick)
        {
            fastDropTicks--;
            gameTicksTillDrop--;

            if (spawnBlock)
            {
                createBlock();
                checkBlocks(0, 0);
                if (wouldClip)
                {
                    gameOver = true;
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


    }





    std::cout << "Program Closed, Game Over!\nPress Enter To Quit." << std::endl;
    std::cin >> boardImage;
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

    currentDifficulty = STARTINGDIFFICULTY;
    gameTicksTillDrop = currentDifficulty;
    spawnBlock = true;
    nextBlock = rand() % 7;
    wouldClip = false;
    right = false;
    left = false;
    score = 0;
    totalLinesCleared = 0;
    level = 1;
    nextLevel = 20;

}

void drawBoard()
{
    boardImage = "\n\n\n\n\n\n\n\n\n\n\t###################################\n\t#";


    for (int y = GAMETILESY - 1; y >= 0; y--)
    {
        for (int x = 0; x < GAMETILESX; x++)
        {
            if (gameBoard[x][y] == true)
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
            else if (x == block[0] && y == block[1] || x == block[2] && y == block[3] || x == block[4] && y == block[5]|| x == block[6] && y == block[7])
            {
                boardImage += "[]";
            }
            else if (x == dropShadow[0] && y == dropShadow[1] || x == dropShadow[2] && y == dropShadow[3] || x == dropShadow[4] && y == dropShadow[5] || x == dropShadow[6] && y == dropShadow[7])
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
        boardImage += "#\n\t#";
    }

    boardImage += "##################################\n\n\n\n";

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

void checkRL ()
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

//Note add block rotation type 2 with other line peice movements for more responsive controls???