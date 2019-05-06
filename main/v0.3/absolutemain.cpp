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

//define the station where the programs runs
#define mainStation "San Diego"

using namespace std;

//global variables that i could've put in the main
int totalTime[nLines], normalTime[nLines], reversedTime[nLines];
string firstStation[nLines], lastStation[nLines];

//subroutine that inizialize the global variables to 0
void inizializeToZero()
{
  for (int i = 0; i < nLines; i++)
  {
    totalTime[i] = 0;
    normalTime[i] = 0;
    reversedTime[i] = 0;
  }
}

//struct inizialized with the file
struct display{
  string name, line;
  int time;
} station[nLines * nStations];


//struct with the variables that are going in the html file
struct readyToHTML
{
  string lastStation;
  string firstStation;
  int hourArriving, minArriving;
  string line;
}htmlReady[8];

//subroutine that detect the O.S. and then put to sleep the program for 1 second
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

//colors that we used for the errors
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

//subroutine that inizialize the struct with the file
void inizializeStruct(int &errorFlag, HANDLE color){
  int stationCont = 0;
  int mainStationPos[nLines], lineCont = 0;
  string line;
  //file directory
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
  //check if the file opened correctly
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
              //saves the first station of each line
              if (j == 0)
              {
                firstStation[stationCont] = station[j + i].name;
                stationCont++;
              }
              //saves the position of the stations that we defined
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
                  //time to get from the start to the station where we are
                  if ( j + i < mainStationPos[k])
                  {
                    normalTime[k] += station[j + i].time;
                  }
                  else  //time to get from the end to the station where we are
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
    //saves the last stations of each line
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
  else //some errors messages
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

//an output to check the struct inizialized with the file
//it doesnt output the "Empty" names of course
//it is also colored
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

//finally the main part of the program
//this subroutine takes in input a time
//and outputs each time a train will pass in our station in 2 hours
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
  //the final time, as I said, is 2 hour after the initial so I just add 2 to the hours
  finalTimeH = initialTimeH + 2;
  finalTimeM = initialTimeM;
  //I transform all the times in seconds
  //I dont actually remember if I used those variables
  initialSeconds = initialTimeH * 60 * 60 + initialTimeM * 60;
  finalSeconds = finalTimeH * 60 * 60 + finalTimeM * 60;

  int cont = 0;

  while (k != nLines)
  {
    time[k] = normalTime[k];
    reverseTime[k] = reversedTime[k];
    //each line takes 1 hour to do 1 run and then it does it again but reversed
    //so I check if the inizial time is odd or even and then check the isReversed bool variable
    if (initialTimeH % 2 == 0)
    {
      isReversed = false;
      //I also check if the initial time is greater or lower then the time that the train needs to get to our station
      //if it is greater it adds 1 hours to the time
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
      //same thing as before if the initial time is greater or lower than  the time that the train needs to get to our station
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
      //this checks if it is reversed or not
      if (!isReversed)
      {
        //if the %i% is the same as the time to get to our station it will calculatate hours and mins from the time
        if (i == time[k])
        {
          tmp = time[k];
          hour = time[k]/3600;
    	    time[k] = time[k]%3600;
    	    min = time[k]/60;
          // we dont need the seconds but i leaved there just in case
    	     //  time[k] = time[k]%60;
    	      //  sec = time[k];
          cout << station[k * nStations].line << " coming from: " << firstStation[k] << " going to: " << lastStation[k] << " ";
          //if the min were minus then 10 the output would've been something like 03:3 and not 03:03
          //so I did some correction
          if (min < 10)
          {
            cout <<hour + initialTimeH << ":0" << min << endl;
          }
          else
          {
            cout <<hour + initialTimeH << ":" << min << endl;
          }
          //I add 1 hour to the times
          reverseTime[k] += 3600;
          time[k] = tmp + 3600;
          isReversed = true;
          //This inizialize the struct that will go in the html
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
        //this is literally the same thing as before but with %reverseTime% instead of %time% and other things reversed
        if (i == reverseTime[k])
        {
          tmpReverse = reverseTime[k];
          hour = reverseTime[k]/3600;
          reverseTime[k] = reverseTime[k]%3600;
          min = reverseTime[k]/60;
          cout << station[k * nStations].line << " coming from: " << lastStation[k] << " going to: " << firstStation[k] << " ";
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

  //this checks if the struct is inizialized correctly
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
