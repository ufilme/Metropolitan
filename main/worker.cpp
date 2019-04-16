#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>
#include <stdlib.h>
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
  int lineCounter = 0, lastPos, pos = 0, h = 0, l = 0;
  char lineDel;
  string currentLine, allStations;
  ifstream data("data/train.txt");
  while (getline(data, currentLine)){
    lineCounter++;
    //------------------------------------------
    //Line Load
    pos = currentLine.find("_", pos);
    lines[h].line = currentLine.substr(0, pos);
    cout << lines[h].line << "  \n";
    lastPos = pos;
    //------------------------------------------
    //Stations load
    pos = currentLine.find("_", 0);
    allStations = currentLine.substr(0, pos);
    pos = 0;
    while (linedel != "*"){
        pos = allStations(",", pos);
        if (allStations[pos + 1] == "*"){
          cout << "Stations Load Ended Successfully" << endl;
          break;
        }

    }
    h++;
  }
  data.close();
}

int main(){

  //getting current time for train update
  time_t currentTime = time(NULL);

  load(lines);

}
