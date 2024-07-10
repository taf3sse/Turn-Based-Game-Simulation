#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <cstring>
#include <fstream>
#include <unistd.h>
using namespace std;

int strToint(string strValue)
{
  int intValue = 0;
  stringstream objStringStream;
  objStringStream << strValue;
  objStringStream >> intValue;
  return intValue;
}

string intTostr(int intValue)
{
  ostringstream objStringStream;
  objStringStream << intValue;
  return objStringStream.str();
}

void printMap()
{
  cout << endl;
  ifstream objFile("map.dat", ios::out | ios::binary);
  string line;
  while (getline(objFile, line))
  {
    for (int z = 0; z < line.length(); z++)
    {
      cout << (line[z] - 48) << " ";
    }
    cout << endl;
  }
  objFile.close();
}

void writeFile(int gameMap[][100], int rowLength, int columnLength)
{
  ofstream objFile("map.dat", ios::out | ios::binary);
  for (int z = 0; z < rowLength; z++)
  {
    for (int y = 0; y < columnLength; y++)
    {
      objFile << gameMap[z][y];
    }
    objFile << endl;
  }
  objFile.close();
}

int checkGame(int gameMap[][100], int rowLength, int columnLength)
{
  int countTeam1 = 0, countTeam2 = 0;
  for (int z = 0; z < rowLength; z++)
  {
    for (int y = 0; y < columnLength; y++)
    {
      if (gameMap[z][y] == 0)
      {
        return -1;
      }
      if (gameMap[z][y] == 1)
      {
        countTeam1++;
      }
      else
      {
        countTeam2++;
      }
    }
  }
  if (countTeam1 == countTeam2)
  {
    return 0;
  }
  else if (countTeam1 > countTeam2)
  {
    return 1;
  }
  else
  {
    return 2;
  }
}

bool checkPos(int t1Occupied[][2], int t2Occupied[][2], int lenT1Occupied, int lenT2Occupied, int checkRow, int checkColumn, int gameMap[][100])
{
  // Check if the position is occupied by team 1 or team 2
  if (gameMap[checkRow][checkColumn] != 0)
  {
    return true; // Position already hit
  }

  // Check if the position is occupied by original positions of team 1 or team 2
  for (int z = 0; z < lenT1Occupied; z++)
  {
    if (t1Occupied[z][0] == checkRow && t1Occupied[z][1] == checkColumn)
    {
      return true; // Original position of team 1
    }
  }
  for (int z = 0; z < lenT2Occupied; z++)
  {
    if (t2Occupied[z][0] == checkRow && t2Occupied[z][1] == checkColumn)
    {
      return true; // Original position of team 2
    }
  }

  return false; // Position is not occupied
}

struct strucThreadData
{
  int t1ogPos[100][2] = {};
  int t2ogPos[100][2] = {};
  int t1Size = 0;
  int t2Size = 0;
  int curTurn = 0;
};

int getScore()
{
  ifstream objFile("map.dat", ios::out | ios::binary);
  int gameMap[100][100] = {};
  string line;
  int teamT1Occupied = 0, rowLength = 0, columnLength = 0;
  while (getline(objFile, line))
  {
    if (line != "")
    {
      rowLength++;
      columnLength = line.length();
    }
    for (int z = 0; z < line.length(); z++)
    {
      gameMap[teamT1Occupied][z] = line[z] - 48;
    }
    teamT1Occupied++;
  }
  objFile.close();
  return checkGame(gameMap, rowLength, columnLength);
}

void *game(void *objThreadArgs)
{
  srand((unsigned)time(NULL));
  struct strucThreadData *curThreadData;
  curThreadData = (struct strucThreadData *)objThreadArgs;

  int gameMap[100][100] = {};
  int teamT1Occupied = 0, col1, col2, rowLength = 0, columnLength = 0;
  string line, input;
  ifstream objFile("map.dat", ios::out | ios::binary);
  while (getline(objFile, line))
  {
    if (line != "")
    {
      rowLength++;
      columnLength = line.length();
    }
    for (int z = 0; z < line.length(); z++)
    {
      gameMap[teamT1Occupied][z] = line[z] - 48;
    }
    teamT1Occupied++;
  }
  objFile.close();

  int countComma = 0;
  if (curThreadData->curTurn == 1)
  {
    cout << endl
         << "Enter comma separated co-ordinates for team 1: ";
  }
  else
  {
    cout << endl
         << "Enter comma separated co-ordinates for team 2: ";
  }

  cin >> input;

  for (int z = 0; z < input.length(); z++)
  {
    if (input[z] == ',')
    {
      countComma++;
    }
  }
  if (countComma != 1)
  {
    cout << "There must be only 1 comma separated 2 integer values for co-ordinates !!" << endl;
    pthread_exit(NULL);
  }

  col1 = strToint(input.substr(0, input.find(",")));
  input.erase(0, input.find(",") + 1);
  col2 = strToint(input);

  if (col1 < 0 || col1 >= rowLength || col2 < 0 || col2 >= columnLength)
  {
    cout << "Invalid values for co-ordinates. Please enter the value within the range of map size !!" << endl;
    pthread_exit(NULL);
  }
  if (checkPos(curThreadData->t1ogPos, curThreadData->t2ogPos, curThreadData->t1Size, curThreadData->t2Size, col1, col2, gameMap))
  {
    cout << "Invalid values for co-ordinates. Co-ordinates already occupied for original position !!" << endl;
  }
  else
  {
    if (gameMap[col1][col2] != curThreadData->curTurn)
    {
      if (!checkPos(curThreadData->t1ogPos, curThreadData->t2ogPos, curThreadData->t1Size, curThreadData->t2Size, col1, col2, gameMap))
      {
        gameMap[col1][col2] = curThreadData->curTurn;
        cout << "Missile Successfully Fired !!" << endl;
      }
      else
      {
        cout << "Invalid values for co-ordinates. Co-ordinates already occupied for original position !!" << endl;
      }
    }
    else
    {
      gameMap[col1][col2] = 3 - curThreadData->curTurn;
      cout << "Missile Mis-Fired !!" << endl;
    }

    writeFile(gameMap, rowLength, columnLength);
  }
  curThreadData->curTurn = 3 - curThreadData->curTurn;
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  int t1Size = 0, t2Size = 0, rowLength = 0, columnLength = 0;
  srand((unsigned)time(NULL));
  struct strucThreadData objThreadData;

  if (argc != 5)
  {
    cout << endl
         << "Invalid no. of arguments passed. Please enter 4 integer arguments > 0 !!";
    return 0;
  }
  t1Size = strToint(argv[1]);
  t2Size = strToint(argv[2]);
  rowLength = strToint(argv[3]);
  columnLength = strToint(argv[4]);
  if (t1Size < 1 || t2Size < 1 || rowLength < 1 || columnLength < 1)
  {
    cout << endl
         << "Invalid arguments passed. Please enter 4 integer arguments > 0 !!";
    return 0;
  }
  if ((t1Size + t2Size) > (rowLength * columnLength))
  {
    cout << endl
         << "Invalid arguments passed. Map size cannot be smaller than total size of both teams !!";
    return 0;
  }
  int gameMap[100][100] = {};
  int t1ogPos[100][2] = {};
  int t2ogPos[100][2] = {};
  int teamT1Occupied = 0, teamT2Occupied = 0, xAxis = 0, yAxis = 0;
  while (teamT1Occupied < t1Size)
  {
    xAxis = rand() % rowLength;
    yAxis = rand() % columnLength;
    if (gameMap[xAxis][yAxis] == 0)
    {
      gameMap[xAxis][yAxis] = 1;
      t1ogPos[teamT1Occupied][0] = xAxis;
      t1ogPos[teamT1Occupied][1] = yAxis;
      teamT1Occupied++;
    }
  }
  while (teamT2Occupied < t2Size)
  {
    xAxis = rand() % rowLength;
    yAxis = rand() % columnLength;
    if (gameMap[xAxis][yAxis] == 0)
    {
      gameMap[xAxis][yAxis] = 2;
      t2ogPos[teamT2Occupied][0] = xAxis;
      t2ogPos[teamT2Occupied][1] = yAxis;
      teamT2Occupied++;
    }
  }
  writeFile(gameMap, rowLength, columnLength);
  int curTurn = 1;
  int intScore = getScore();
  while (intScore == -1)
  {
    objThreadData.curTurn = curTurn;
    for (int z = 0; z < t1Size; z++)
    {
      objThreadData.t1ogPos[z][0] = t1ogPos[z][0];
      objThreadData.t1ogPos[z][1] = t1ogPos[z][1];
    }
    for (int z = 0; z < t2Size; z++)
    {
      objThreadData.t2ogPos[z][0] = t2ogPos[z][0];
      objThreadData.t2ogPos[z][1] = t2ogPos[z][1];
    }
    objThreadData.t1Size = t1Size;
    objThreadData.t2Size = t2Size;

    pthread_t myThread;
    pthread_create(&myThread, NULL, game, (void *)&objThreadData);
    pthread_join(myThread, NULL); // Wait for the thread to finish

    curTurn = objThreadData.curTurn;
    intScore = getScore();
  }
  printMap();
  if (intScore == 0)
  {
    cout << endl
         << "Game Is Drawn !!" << endl;
  }
  else if (intScore == 1)
  {
    cout << endl
         << "Team 1 is the Winner !!" << endl;
  }
  else
  {
    cout << endl
         << "Team 2 is the Winner !!" << endl;
  }
  return 0;
}
