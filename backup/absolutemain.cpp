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
#define nStations 13
#define mainStation "San Diego"

using namespace std;

int totalTime[nLines], normalTime[nLines], reversedTime[nLines];
string firstStation[nLines], lastStation[nLines];

void inizializeToZero()
{
  for (int i = 0; i < nLines; i++)
  {
    totalTime[i] = 0;
    normalTime[i] = 0;
    reversedTime[i] = 0;
  }
}


struct display{
  string name, line;
  int time;
} station[nLines * nStations];

struct readyToHTML
{
  string lastStation;
  string firstStation;
  int hourArriving, minArriving;
  string line;
}htmlReady[8];

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
  int stationCont = 0;
  int mainStationPos[nLines], lineCont = 0;
  string line;
  ifstream file ("data/absolutetrain.txt");
  int pos = 0, staticPos, i = 0, k = 0;
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
              if (j == 0)
              {
                firstStation[stationCont] = station[j + i].name;
                stationCont++;
              }
              if (station[j + i].name == mainStation)
              {
                mainStationPos[lineCont] = j + i;
                lineCont++;
              }
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
                  totalTime[k] += station[j + i].time;
                  if ( j + i < mainStationPos[k])
                  {
                    normalTime[k] += station[j + i].time;
                  }
                  else
                  {
                    reversedTime[k] += station[j + i].time;
                  }
                  staticPos = pos + 1;
                  j++;

              }
              pos++;
            }
        }
      }
      k++;
      i += nStations;
    }
    file.close();
    int k = 0;
    for (int i = 0; i < nLines * nStations - 1; i++)
    {
      cout << i << endl;
      if ((station[i].name != "Empty") && (station[i + 1].name == "Empty"))
      {
        lastStation[k] = station[i].name;
        k++;
      }
    }
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

void workerOutput()
{
  bool isReversed;
  int time[nLines], tmp , reverseTime[nLines], tmpReverse;
  int k = 0, j = 0;
  int initialTimeH, initialTimeM;
  int initialSeconds;
  int finalTimeH, finalTimeM;
  int finalSeconds;
  char c;
  int hour, min, sec;
  cout << "Insert the initial time(HH:MM): ";
  cin >> initialTimeH >> c >> initialTimeM;
  finalTimeH = initialTimeH + 2;
  finalTimeM = initialTimeM;
  initialSeconds = initialTimeH * 60 * 60 + initialTimeM * 60;
  finalSeconds = finalTimeH * 60 * 60 + finalTimeM * 60;

  int cont = 0;

  while (k != nLines)
  {
    time[k] = normalTime[k];
    reverseTime[k] = reversedTime[k];
    if (initialTimeH % 2 == 0)
    {
      isReversed = false;
      if (time[k] < initialTimeM * 60)
      {
        isReversed = true;
        reverseTime[k] += 3600;
        time[k] += 3600;
        cont++;
      }
    }
    else
    {
      isReversed = true;
      if (reversedTime[k] < initialTimeM * 60)
      {
        time[k] += 3600;
        reverseTime[k] += 3600;
        isReversed = false;
        cont++;
      }
    }
    for (int i = initialTimeM * 60; i <= (((finalTimeH - initialTimeH) * 60) * 60) + (finalTimeM * 60); i++)
    {
      if (!isReversed)
      {
        if (i == time[k])
        {
          tmp = time[k];
          hour = time[k]/3600;
    	    time[k] = time[k]%3600;
    	    min = time[k]/60;
    	  //  time[k] = time[k]%60;
    	  //  sec = time[k];
          cout << station[k * nStations].line << " PROVENIENZA: " << firstStation[k] << " DIREZIONE: " << lastStation[k] << " ";
          if (min < 10)
          {
            cout <<hour + initialTimeH << ":0" << min << endl;
          }
          else
          {
            cout <<hour + initialTimeH << ":" << min << endl;
          }
          reverseTime[k] += 3600;
          time[k] = tmp + 3600;
          isReversed = true;
          htmlReady[cont].firstStation = firstStation[k];
          htmlReady[cont].lastStation = lastStation[k];
          htmlReady[cont].hourArriving = hour + initialTimeH;
          htmlReady[cont].minArriving = min;
          htmlReady[cont].line = station[k * nStations].line;
          cont++;
        }
      }
      else
      {
        if (i == reverseTime[k])
        {
          tmpReverse = reverseTime[k];
          hour = reverseTime[k]/3600;
          reverseTime[k] = reverseTime[k]%3600;
          min = reverseTime[k]/60;
          cout << station[k * nStations].line << " PROVENIENZA: " << lastStation[k] << " DIREZIONE: " << firstStation[k] << " ";
          if (min < 10)
          {
            cout <<hour + initialTimeH << ":0" << min << endl;
          }
          else
          {
            cout <<hour + initialTimeH << ":" << min << endl;
          }
          time[k] += 3600;
          reverseTime[k] = tmpReverse + 3600;
          isReversed = false;
          htmlReady[cont].firstStation = lastStation[k];
          htmlReady[cont].lastStation = firstStation[k];
          htmlReady[cont].hourArriving = hour + initialTimeH;
          htmlReady[cont].minArriving = min;
          htmlReady[cont].line = station[k * nStations].line;
          cont++;
        }
      }
    }
    k++;
  }
  cout << "---------------" << endl;
}


int main(){
  inizializeToZero();
  int errorFlag = 0;
  HANDLE  color;

  color = GetStdHandle(STD_OUTPUT_HANDLE);
  inizializeStruct(errorFlag, color);
  white(color);
  if (errorFlag == 1){
    SetConsoleTextAttribute(color, 12);
  } else {
    coolOutput(color);
  }
  white(color);
  workerOutput();

  for (int i = 0; i < 8; i++)
  {
    cout << htmlReady[i].firstStation << endl;
    cout << htmlReady[i].lastStation << endl;
    cout << htmlReady[i].hourArriving << endl;
    cout << htmlReady[i].minArriving << endl;
    cout << htmlReady[i].line << endl;
    cout << endl;
  }

  return 0;
}
