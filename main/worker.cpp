#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#define maxStations 12
#define metroLines 4
using namespace std;
// first letter of a station = 17 (for strings = 16)
//frequency 3,7
//metropolitan lines 0,2


struct underground{
  string line, terminal, stations[maxStations];
  int interval[maxStations-1], number_of_stops, retard;
} lines[metroLines];

void load(underground lines[]){
  int lineCounter = 0, lastPos, pos = 0, posLastStations = 0, h = 0, l = 0;
  string lineDel;
  string currentLine, allStations, tmp;
  ifstream data("data/train.txt");
  while (getline(data, currentLine)){
    lineCounter++;
    lastPos = 0;
    pos = 0;
    //------------------------------------------
    //Line Load
    pos = currentLine.find("_", pos);
    cout << pos << endl;
    lines[h].line = currentLine.substr(0, pos);
    cout << lines[h].line << "  \n";
    lastPos = pos + 1;
    cout << lastPos << endl;
    //------------------------------------------
    //Stations load
    pos = currentLine.find("_", lastPos);
    allStations = currentLine.substr(lastPos, pos);
    cout << allStations << endl;
    pos = 0;
    while (lineDel != "*"){
        if (allStations[pos + 1] == '*'){
          cout << "Stations Load Ended Successfully" << endl;
          lineDel = "*";
        } else {
        cout << pos << endl;
        pos = allStations.find(",", posLastStations);
        cout << pos << endl;
        tmp = allStations.substr(posLastStations, pos - posLastStations);
        cout << tmp << "  \n";
        posLastStations = pos + 1;
        l++;
        }
    }
    lineDel = "c";
    h++;
  }
  data.close();
}

int main(){

  //getting current time for train update
  time_t currentTime = time(NULL);

  load(lines);

}
