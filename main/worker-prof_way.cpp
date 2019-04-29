#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined(_WIN32)
#include <windows.h>
#endif
#define maxStations 12
#define metroLines 4
using namespace std;


struct underground{
  string line, terminal, station, last;
  int interval, number_of_stops;
} stations[maxStations];

void wait(int timer){
  #if defined(_WIN32)
    Sleep(timer);
  #elif defined(__linux__)
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

void separator(){
    string underscore;
    for (int i = 0; i < 51; i++){
        underscore += "-";
        cout << "\r" << underscore << flush;
        usleep(1);
    }
    cout << endl;
}

//This function give us the current time to update the train
void timet(int currentTime){
  currentTime = time(NULL);
}

void load(underground stations[]){
  int lineCounter = 0, lastPos, pos = 0, posLine = 0, posLastStations = 0, posLastTime = 0, h = 0, l = 0, warnflag = 0;
  string lineDel, currentLine, allStations, allTimes;

  ifstream data("data/train-test.txt");
  while(getline(data, currentLine)){
    allStations.clear(); allTimes.clear();
    lastPos = 0; pos = 0; posLine = 0; posLastStations = 0; posLastTime = 0; l = 0;

    pos = currentLine.find("_", pos);
    lastPos = pos + 1;
    pos = currentLine.find("_", lastPos);
    allStations = currentLine.substr(lastPos, pos - lastPos);
    pos = 0;
    while (lineDel != "*"){
        if (allStations[pos + 1] == '*'){
          //cout << "[WORKER] Stations Loading Ended Successfully" << endl;
          wait(500);
          lineDel = "*";
        } else {
          pos = allStations.find(",", posLastStations);
          stations[l].station = allStations.substr(posLastStations, pos - posLastStations);
          posLine = currentLine.find("_", posLine);
          stations[l].line = currentLine.substr(0, posLine);
          posLine = 0;
          if (stations[l].station != "Empty"){
            stations[l].number_of_stops += 1;
          }
        posLastStations = pos + 1;
        l++;
        }
    }
    pos = currentLine.find("_", posLastStations);
    lineDel = "!";
    pos = currentLine.find("_", lastPos);
    lastPos = pos + 1;
    l = 0;
    allTimes = currentLine.substr(lastPos, pos - lastPos);
    while (lineDel != "*"){
        if (allTimes[pos + 1] == '*'){
            //cout << "[WORKER] Intervals Loading Ended Successfully" << endl;
            usleep(1000000);
            lineDel = "*";
        } else {
          pos = allTimes.find(",", posLastTime);
          stringstream(allTimes.substr(posLastTime, pos - posLastTime)) >> stations[l].interval;
          posLastTime = pos + 1;
          l++;
        }
        cout << stations[l].line << stations[l].station << stations[l].interval << endl;
    }
    lineDel = "!";
    currentLine.clear();
  }

}

int main(){

  int time, currentTime;
  string choice;

  load(stations);
}
