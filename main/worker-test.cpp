#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <iomanip>
#define maxStations 12
#define metroLines 4
#define parkingTime 45
using namespace std;


struct underground{
  string line, terminal, stations[maxStations], last;
  int interval[maxStations-1], number_of_stops, nextTime, diff, retard, parkingFlag;
} lines[metroLines];

//This theorically can clear the console in Windows and Linux
void clrscr(){
    system("@cls||clear");
}

void separator(){
    string underscore;
    for (int i = 0; i < 51; i++){
        underscore += "-";
        cout << "\r" << underscore << flush;
    }
    cout << endl;
}

void stamp(underground lines[]){
  for (int i = 0; i < metroLines; i++){
    cout << lines[i].line << "\t";
    for (int j = 0; j < maxStations; j++){
      if (lines[i].stations[j] != "Empty"){
        cout << lines[i].stations[j] << ",";
      }
    }
    cout << "\n\t";
    for (int j = 0; j < maxStations; j++){
      if (lines[i].stations[j] != "Empty"){
        cout << lines[i].interval[j] << ",";
      }
    }
    cout << endl;
  }
}

void selectionSort(underground lines[]){
  underground tmp;
  for (int j = 0; j < metroLines; j++){
    for (int i = 0; i < metroLines; i++){
        if (lines[j].diff < lines[i].diff){
            tmp = lines[j];
            lines[j] = lines[i];
            lines[i] = tmp;
        }
    }
  }
}

void print(underground lines[], int interStation[]){
  int flag = 0;
  for (int i = 0; i < metroLines; i++){
    if (lines[i].parkingFlag == 1){
      if (flag == 0){
        clrscr();
        cout << right << setw(5) << "LINEA" << setw(20) << "STAZIONE ATTUALE" << setw(20) << "PROSSIMA STAZIONE" << setw(20) << "DIREZIONE" << setw(30) << "TEMPO DI ARRIVO STIMATO\n" << endl;
        flag = 1;
      }
      cout << right << setw(5) << lines[i].line << setw(20) << lines[i].last << setw(20) << lines[i].stations[interStation[i]] << setw(20) << lines[i].terminal << setw(30) << "IN STAZIONE\n" << endl;
      flag = 0;
    } else {
      if (flag == 0){
        cout << right << setw(5) << "LINEA" << setw(20) << "ULTIMA STAZIONE" << setw(20) << "PROSSIMA STAZIONE" << setw(20) << "DIREZIONE" << setw(30) << "TEMPO DI ARRIVO STIMATO\n" << endl;
        flag = 1;
      }
      cout << right << setw(5) << lines[i].line << setw(20) << lines[i].last << setw(20) << lines[i].stations[interStation[i]] << setw(20) << lines[i].terminal << setw(20) << lines[i].diff/3600 << setw(2) << ":" <<setw(2) << lines[i].diff/60 << setw(2) << ":" << setw(2) << lines[i].diff%60 << endl;
    }
  }
}

//This function give us the current time to update the train
int time(){
  int currentTime = time(NULL);
  return currentTime;
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
  //------------------------------------------
  //Loading Container Data
  ifstream data("data/train.txt");
  while (getline(data, currentLine)){
    //------------------------------------------
    //Clearing and initializing var
    lines[h].number_of_stops = 0;
    allStations.clear(); allTimes.clear();
    lastPos = 0; pos = 0; posLastStations = 0; posLastTime = 0; l = 0;
    //------------------------------------------
    //Incrementing line counter of file lines
    lineCounter++;
    //------------------------------------------
    //Checking if array is smaller than the database file
    separator();
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
      separator();
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
        //Incrementing number of stations stops
        if (lines[h].stations[l] != "Empty"){
        lines[h].number_of_stops += 1;
        }
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
  data.close();
  if (warnflag > 0){
    cout << "[WARN] One or more errors occurred" << endl;
    cout << "[WARN] It's recommended to check the files" << endl;
    separator();
    cout << "[WORKER] Closing File" << endl;
    cout << "[WORKER] Anyway... Loading Finished Successfully" << endl;
    separator();
    usleep(2000000);
  } else {
    separator();
    cout << "[WORKER] Closing File" << endl;
    cout << "[WORKER] Loading Finished Successfully" << endl;
    separator();
    usleep(2000000);
  }

}

void last_update_load(underground lines[], int &controlFlag, int interStation[], int interTime[]){
  string last_station;

  fstream data("data/last_stations.txt");
  //------------------------------------------
  //If there is something in the file
  for (int i = 0; i < metroLines; i++){
    if (getline(data, last_station)){
        if (last_station == lines[i].stations[lines[i].number_of_stops - 1]){
          cout << "BISOGNA INVERTIRE" << endl;
          controlFlag = 1;
        } else {
          lines[i].last = last_station;
        }
    } else {  //there is nothing, this mean first start
        lines[i].last = lines[i].stations[interStation[i]];
        interStation[i]++;
        lines[i].terminal = lines[i].stations[lines[i].number_of_stops - 1];
        data << lines[i].last;
    }
    last_station.clear();
  }
}

void parking(underground lines[], int p, int interTime[]){
  int currentTime = time();
  lines[p].nextTime = parkingTime + currentTime;
}

void worker(underground lines[], int &controlFlag, int interStation[], int interTime[]){
  int currentTime;

  currentTime = time();
  //
  for (int i = 0; i < metroLines; i++){
    lines[i].nextTime = lines[i].interval[interTime[i]] + currentTime;
  }

  while(true){
    currentTime = time();
    for (int i = 0; i < metroLines; i++){
        lines[i].diff = lines[i].nextTime - currentTime;
        if (lines[i].nextTime - currentTime == 0){
          if (lines[i].parkingFlag == 0){
            parking(lines, i, interTime);
            lines[i].parkingFlag = 1;
            lines[i].last = lines[i].stations[interStation[i]];
            interStation[i]++;
          } else {
            interTime[i]++;
            lines[i].nextTime = lines[i].interval[interTime[i]] + currentTime;
            lines[i].parkingFlag = 0;
          }
        }
        //if (last_station[i] == lines[i].stations[lines[i].number_of_stops - 1] && controlFlag == 0){
          //INVERTIRE
        //}
    }
    clrscr();
    selectionSort(lines);
    print(lines, interStation);
  }
}

int main(){

  int flag = 0;
  int interTime[metroLines] = {0,0,0,0}, interStation[metroLines] = {0,0,0,0};

  load(lines);

  stamp(lines);

  last_update_load(lines, flag, interTime, interStation);

  worker(lines, flag, interTime, interStation);
}
