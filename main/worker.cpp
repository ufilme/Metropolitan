#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define maxStations 12
#define metroLines 4
using namespace std;


struct underground{
  string line, terminal, stations[maxStations], last;
  int interval[maxStations-1], number_of_stops, retard;
} lines[metroLines];

//This theorically can clear the console in Windows and Linux
void clrscr(){
    system("@cls||clear");
}

//This function give us the current time to update the train
void time(){
  time_t currentTime = time(NULL);
}

void load(underground lines[]){
  int lineCounter = 0, lastPos, pos = 0, posLastStations = 0, posLastTime = 0, h = 0, l = 0, warnflag = 0;
  string lineDel, currentLine, allStations, allTimes;

  cout << "[WORKER] Starting Loading";
  for (int i = 0; i < 3; i++){
    cout << ".";
    usleep(250000);
  }
  cout << "\n[WORKER] Opening File" << endl;
  ifstream data("data/train.txt");
  while (getline(data, currentLine)){
    //------------------------------------------
    //Clearing all tmp variables
    allStations.clear(); allTimes.clear();
    lastPos = 0; pos = 0; posLastStations = 0; posLastTime = 0; l = 0;
    //------------------------------------------
    //Incrementing line counter of file lines
    lineCounter++;
    //------------------------------------------
    //Checking if array is smaller than the database file
    cout << "--------------------------------------------------" << endl;
    if (lineCounter > metroLines){
      cout << "\nTraceback:" << endl;
      usleep(250000);
      warnflag = 1; //setting the warnflag to 1 to get personalized messagge at the end of loading
      cout << "\tIt seems that the database hold more train lines than the size of the array" << endl;
      cout << "\tThis means that not all the lines are probably loaded" << endl;
      cout << "\tCheck the file or correct the array size" << endl;
      usleep(1000000);
      cout << "\nPress any key to continue..." << endl;
      cin.get();  //waiting for keyboard inpunt
      cout << "--------------------------------------------------" << endl;
      break;
    }
    cout << "[WORKER] Reading Line Nr: " << lineCounter << endl;
    usleep(250000);
    //------------------------------------------
    //Loading of train line name
    cout << "[WORKER] Loading Line Name" << endl;
    usleep(250000);
    //------------------------------------------
    //Reading file until the first "_"
    pos = currentLine.find("_", pos);
    //------------------------------------------
    //Extracting string between 0 and start position, that is the position before "_"
    lines[h].line = currentLine.substr(0, pos);
    //------------------------------------------
    //Change lastPos from one position before "_", to it
    lastPos = pos + 1;
    //------------------------------------------
    //Loading of all stations of a train line
    cout << "[WORKER] Starting Loading of All Stations" << endl;
    usleep(250000);
    //------------------------------------------
    //Reading file until the first "_"
    pos = currentLine.find("_", lastPos);
    //------------------------------------------
    //Extracting all stations into a string
    allStations = currentLine.substr(lastPos, pos - lastPos);
    //------------------------------------------
    //Resetting start position to 0
    pos = 0;
    //------------------------------------------
    //Making a while loop to check if all stations are loaded. After the last station in the database, there is a "*" meaning that all is loaded
    //So while check if line delimiter is equal to "*"
    while (lineDel != "*"){
        //------------------------------------------
        //If in the next char array (the string where are all the stations) position there is a "*" the line delimiter is update to stop the while loop
        if (allStations[pos + 1] == '*'){
          cout << "[WORKER] Stations Loading Ended Successfully" << endl;
          usleep(1000000);
          lineDel = "*";
        } else {
        //------------------------------------------
        //Searching in the string for the "," delimiter to determinate a station name
        //This will return a number that then we turn into the lenght of the name
        pos = allStations.find(",", posLastStations);
        //------------------------------------------
        //Extracting the name and putting into struct
        //This start extracting from last stations position (that initially is 0) for the length of the name (that is the subtraction of the position before the "," and the initial position)
        lines[h].stations[l] = allStations.substr(posLastStations, pos - posLastStations);
        //------------------------------------------
        //Change last station position from one position before ",", to it
        //This let us move to the next station
        posLastStations = pos + 1;
        //------------------------------------------
        //Incrementing var l to move into the array of stations
        l++;
        }
    }
    //------------------------------------------
    //Reading again file until "_" because pos change previously
    pos = currentLine.find("_", posLastStations);
    //------------------------------------------
    //Changing line delimiter character to use it again next with the intervals time
    lineDel = "!";
    //------------------------------------------
    //Change lastPos from one position before "_", to it
    lastPos = pos + 1;
    l = 0;
    //------------------------------------------
    //Loading intervals time between the stations
    cout << "[WORKER] Starting Loading of All Intervals Time" << endl;
    usleep(250000);
    //------------------------------------------
    //Reading file until the first "_"
    pos = currentLine.find("_", lastPos);
    //------------------------------------------
    //Extracting all times into a string
    allTimes = currentLine.substr(lastPos, pos - lastPos);
    //Same while as above, only change is the use of stringstream to convert time from string to int to load into the struct
    while (lineDel != "*"){
        if (allTimes[pos + 1] == '*'){
            cout << "[WORKER] Intervals Loading Ended Successfully" << endl;
            usleep(1000000);
            lineDel = "*";
        } else {
          pos = allTimes.find(",", posLastTime);
          stringstream(allTimes.substr(posLastTime, pos - posLastTime)) >> lines[h].interval[l];
          posLastTime = pos + 1;
          l++;
        }
    }
    //Changing line delimiter character to use it again next in the while loop
    lineDel = "!";
    //------------------------------------------
    //Clering last readed database line, just in case
    currentLine.clear();
    //------------------------------------------
    //Incrementing var h to move into the array of train lines
    h++;
  }
  //------------------------------------------
  //Personalized line of error
  if (warnflag > 0){
    cout << "[WARN] One or more errors occurred" << endl;
    cout << "[WARN] It's recommended to check the files" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "[WORKER] Closing File" << endl;
    data.close();
    cout << "[WORKER] Anyway... Loading Finished Successfully" << endl;
    cout << "--------------------------------------------------" << endl;
  } else {
    cout << "--------------------------------------------------" << endl;
    cout << "[WORKER] Closing File" << endl;
    data.close();
    cout << "[WORKER] Loading Finished Successfully" << endl;
    cout << "--------------------------------------------------" << endl;
  }

}

int main(){

  load(lines);

  for (int i = 0; i < metroLines; i++){
    for (int j = 0; j < maxStations; j++){
      if (lines[i].stations[j] != "Empty"){
          cout << lines[i].line << "\t" << lines[i].stations[j] << "\t" << lines[i].interval[j] << endl;
      }
    }
    cout << endl;
  }
}
