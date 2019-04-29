#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>
#define nLines 4
#define nStations 12

using namespace std;

struct display
{
  string name;
  string line;
  int time;
}station[nLines * nStations];

void inizializeStruct()
{
  string line;
  ifstream file ("data/train.txt");
  int pos = 0, staticPos, i = 0;
  char posChar;
  if (file.is_open())
  {
    while (getline (file, line))
    {
      //inizializing lines
      for (int j = 0; j < nStations; j++)
      {
        pos = line.find('_');
        station[j + i].line = line.substr(0, pos);
      }
      //inizialiazing stations
      for (int j = 0; j < nStations; j++)
      {
        posChar = line[pos];
        staticPos = pos + 1;
        while (posChar != '*')
        {
          posChar = line[pos];
          if (posChar == ',')
          {
              station[j + i].name = line.substr(staticPos, pos - staticPos);
              staticPos = pos + 1;
              j++;
          }
          pos++;
        }
        pos++;
        posChar = line[pos];
        //inizializing times
        for (int j = 0; j < nStations; j++)
        {
            staticPos = pos;
            while (posChar != '*')
            {
              posChar = line[pos];
              if (posChar == ',')
              {
                  stringstream(line.substr(staticPos, pos - staticPos)) >> station[j + i].time;
                  staticPos = pos + 1;
                  j++;
              }
              pos++;
            }
        }
      }
      i += nStations;
    }
    file.close();
  }
  else
  {
    cout << "Error file not found" << endl;
  }
}

void coolOutput()
{
  HANDLE  hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  int k = 3;
  cout <<left<< setw(6) << "LINE" << setw(20) << "STATION" << setw(6) << "TIME" << endl;
  for (int i = 0; i < nStations * nLines; i++)
  {
    FlushConsoleInputBuffer(hConsole);
    SetConsoleTextAttribute(hConsole, k);
    if (station[i].name != "Empty")
    {
      cout << setw(6) << station[i].line << setw(20) << station[i].name << setw(6) << station[i].time << endl;
      k++;
      if (k == 15)
      {
        k = 3;
      }
    }
  }
}

int main()
{
  HANDLE  hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  inizializeStruct();
  coolOutput();
  SetConsoleTextAttribute(hConsole, 15);
  return 0;
}
