#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#if defined(_WIN32)
#include <windows.h>
#endif
#define nLines 4
#define nStations 12

using namespace std;

struct display{
  string name, line;
  int time;
} station[nLines * nStations];

void wait(int timer){
  #if defined(_WIN32)
    Sleep(timer);
  #elif defined(__linux__)
    timer /= 1000;
    sleep(timer);
  #endif
}

//This theorically can clear the console in Windows and Linux
void clrscr(){
  #if defined(_WIN32)
    system("cls");
  #elif defined(__linux__)
    system("clear");
  #endif
}

void white(HANDLE color){
  SetConsoleTextAttribute(color, 15);
}

void yellow(HANDLE color){
  SetConsoleTextAttribute(color, 14);
}

void green(HANDLE color){
  SetConsoleTextAttribute(color, 10);
}

void red(HANDLE color){
  SetConsoleTextAttribute(color, 12);
}

void inizializeStruct(int &errorFlag, HANDLE color){
  string line;
  ifstream file ("data/tran.txt");
  int pos = 0, staticPos, i = 0;
  char posChar;
  cout << "[";
  yellow(color);
  cout << "WORKER";
  white(color);
  cout << "] ";
  cout << "Checking File";
  for (int i = 0; i < 3; i++){
    cout << ".";
    usleep(250000);
  }
  wait(500);
  clrscr();
  if (file.is_open()){
    cout << "[";
    green(color);
    cout << "WORKER";
    white(color);
    cout << "] ";
    cout << "Checking Done Successfully\n";
    while (getline (file, line)){
      //inizializing lines
      for (int j = 0; j < nStations; j++){
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
    cout << "[";
    red(color);
    cout << "WORKER";
    white(color);
    cout << "] ";
    red(color);
    cout << "Database File not found\n";
    wait(1000);
    white(color);
    cout << "[";
    red(color);
    cout << "WORKER";
    white(color);
    cout << "] ";
    red(color);
    cout << "Cannot continue\n";
    wait(1000);
    white(color);
    cout << "[";
    red(color);
    cout << "WORKER";
    white(color);
    cout << "] ";
    red(color);
    cout << "Aborting...\n";
    errorFlag = 1;
  }
}

void coolOutput(HANDLE color){
  int k = 3;
  cout << right << setw(6) << "LINE" << setw(20) << "STATION" << setw(6) << "TIME" << endl;
  cout << endl;
  for (int i = 0; i < nStations * nLines; i++){
    SetConsoleTextAttribute(color, k);
    if (station[i].name != "Empty"){
      cout << setw(6) << station[i].line << setw(20) << station[i].name << setw(6) << station[i].time << endl;
      k++;
      if (k == 15){
        k = 3;
      }
    }
  }
}

int main(){
  int errorFlag = 0;
  HANDLE  color;

  color = GetStdHandle(STD_OUTPUT_HANDLE);
  inizializeStruct(errorFlag, color);
  white(color);
  if (errorFlag == 1){
    SetConsoleTextAttribute(color, 12);
  } else {
    //coolOutput(color);
  }
  white(color);
  return 0;
}
