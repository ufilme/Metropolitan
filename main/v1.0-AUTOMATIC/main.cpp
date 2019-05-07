#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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
void inizializeToZero(){
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
struct readyToHTML{
  string lastStation;
  string firstStation;
  int hourArriving, minArriving;
  string line;
  string direction;
}htmlReady[nLines*2];

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

//------------------------------------------
//Colors
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
//------------------------------------------

//------------------------------------------
//Line separator
void separator(){
    string underscore;
    for (int i = 0; i < 51; i++){
        underscore += "-";
        cout << "\r" << underscore << flush;
    }
    cout << endl;
}
//------------------------------------------

void workerprint(HANDLE color){
  cout << left << setw(2) << "[";
  green(color);
  cout << left << setw(7) << "WORKER";
  white(color);
  cout << "] ";
}

void checkfiles(int &errorFlag, HANDLE color){
  string tmp;
  ifstream file ("data/train.txt");
  //------------------------------------------
  //Checking File textes
  for (int i = 0; i < 8; i++){
    if (tmp.length() == 3){
      tmp.clear();
    }
    workerprint(color);
    cout << "Starting Checking Files" << endl;
    if (i == 0){
      wait(2000);
    }
    cout << left << setw(2) << "[";
    if (i % 2 != 0){
      yellow(color);
    } else {
      white(color);
    }
    cout << left << setw(7) << "WORKER";
    white(color);
    cout << "] ";
    cout << "Checking File";
    tmp += ".";
    cout << tmp;
    wait(150);
    clrscr();
  }
  tmp += ".";
  workerprint(color);
  cout << "Starting Checking Files" << endl;
  cout << left << setw(2) << "[";
  cout << left << setw(7) << "WORKER";
  white(color);
  cout << "] ";
  cout << "Checking File" << tmp << endl;
  //------------------------------------------
  if (file.is_open()){
    workerprint(color);
    cout << "Checking Done Successfully\n";
    white(color);
  } else {
    cout << left << setw(2) << "[";
    red(color);
    cout << left << setw(7) << "ERROR";
    white(color);
    cout << "] ";
    red(color);
    cout << "Database File Not Found\n";
    wait(1000);
    errorFlag = 404;
    white(color);
  }
  //------------------------------------------
}

//subroutine that inizialize the struct with the file
void inizializeStruct(int &errorFlag, HANDLE color){
  int stationCont = 0, currentLine = 0, flag = 0;
  int mainStationPos[nLines], lineCont = 0;
  string line;
  int pos = 0, staticPos, i = 0, k = 0;
  char posChar;

  cout << left << setw(2) << "[";
  cout << left << setw(7) << "WORKER";
  cout << "] Starting Loading";
  for (int i = 0; i < 3; i++){
    cout << ".";
    wait(150);
  }
  cout << endl;
  workerprint(color);
  cout << "Reading File" << endl;
  ifstream file ("data/train.txt");
  while (getline (file, line)){
    //------------------------------------------
    //Incrementing line counter of file lines
    currentLine++;
    //------------------------------------------
    //Checking if array is smaller than the database file
    separator();
    if (currentLine > nLines){
      red(color);
      cout << "\nTraceback:" << endl;
      wait(250);
      errorFlag = 1; //setting the errorFlag to 1 to get personalized messagge at the end of loading
      cout << "\tIt seems that the database hold more train lines than the size of the array" << endl;
      cout << "\tThis means that not all the lines are probably loaded" << endl;
      cout << "\tCheck the file or correct the array size" << endl;
      wait(1000);
      white(color);
      cout << "\nPress any key to continue..." << endl;
      cin.get();  //waiting for keyboard inpunt
      separator();
      break;
    }
    workerprint(color);
    cout << "Reading Line Nr: " << currentLine << endl;
    wait(200);
    workerprint(color);
    cout << "Loading Line Name" << endl;
    wait(50);
    //inizializing lines
    for (int j = 0; j < nStations; j++){
      pos = line.find('_');
      station[j + i].line = line.substr(0, pos);
    }
    workerprint(color);
    cout << "Starting Loading of All Stations" << endl;
    wait(200);
    //inizialiazing stations
    for (int j = 0; j < nStations; j++){
      posChar = line[pos];
      staticPos = pos + 1;
      while (posChar != '*'){
        posChar = line[pos];
        if (posChar == ','){
          station[j + i].name = line.substr(staticPos, pos - staticPos);
          //saves the first station of each line
          if (j == 0){
            firstStation[stationCont] = station[j + i].name;
            stationCont++;
          }
          //saves the position of the stations that we defined
          if (station[j + i].name == mainStation){
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
        workerprint(color);
        cout << "Stations Loading Ended Successfully" << endl;
        wait(50);
        workerprint(color);
        cout << "Starting Loading of All Intervals Time" << endl;
        wait(200);
        //inizializing times
        for (int j = 0; j < nStations; j++){
          staticPos = pos;
          while (posChar != '*'){
            posChar = line[pos];
            if (posChar == ','){
              stringstream(line.substr(staticPos, pos - staticPos)) >> station[j + i].time;
              totalTime[k] += station[j + i].time;
              //time to get from the start to the station where we are
              if ( j + i < mainStationPos[k]){
                normalTime[k] += station[j + i].time;
              } else { //time to get from the end to the station where we are
                reversedTime[k] += station[j + i].time;
              }
              staticPos = pos + 1;
              j++;
            }
            pos++;
          }
        }
        workerprint(color);
        cout << "Intervals Loading Ended Successfully" << endl;
        wait(50);
      }
      k++;
      i += nStations;
    }
    file.close();
    k = 0;
    //saves the last stations of each line
    for (int i = 0; i < nLines * nStations - 1; i++)
    {
      if ((station[i].name != "Empty") && (station[i + 1].name == "Empty"))
      {
        lastStation[k] = station[i].name;
        k++;
      }
    }
  }

//finally the main part of the program
//this subroutine takes in input a time
//and outputs each time a train will pass in our station in 2 hours
void workerOutput(int &initialTimeH, int &initialTimeM){
  bool isReversed;
  int time[nLines], tmp , reverseTime[nLines], tmpReverse;
  int k = 0, j = 0;
  int initialSeconds;
  int finalTimeH, finalTimeM;
  int finalSeconds;
  char c;
  int hour, min, sec;
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
        //cont++;
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
        //cont++;
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
          htmlReady[cont].direction = "RIGHT";
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
          htmlReady[cont].direction = "LEFT";
          cont++;
        }
      }
    }
    k++;
  }
}

void htmlparser(){
  fstream html("html/index.html");
  string tab = "      ";
  html.seekg(498, ios::beg);
  for(int i = 0; i < 1000; i++){
    html << " " << endl;
  }
  html.seekg(498, ios::beg);
  for (int i = 0; i < 8; i++){
    html << "    <div class='data'>" << endl;
    html << tab << "<p class='linefeed'>" << htmlReady[i].line << "</p>" << endl;
    html << tab << "<p class='linefeed'>" << htmlReady[i].direction << "</p>" << endl;
    html << tab << "<p class='linefeed'>" << htmlReady[i].firstStation << "</p>" << endl;
    html << tab << "<p class='linefeed'>" << htmlReady[i].lastStation << "</p>" << endl;
    if (htmlReady[i].hourArriving < 10 && htmlReady[i].minArriving < 10){
      html << tab << "<p class='linefeed'>0" << htmlReady[i].hourArriving << ":0" << htmlReady[i].minArriving << "</p>" << endl;
    } else if (htmlReady[i].minArriving < 10){
      html << tab << "<p class='linefeed'>" << htmlReady[i].hourArriving << ":0" << htmlReady[i].minArriving << "</p>" << endl;
    } else if (htmlReady[i].hourArriving < 10){
      html << tab << "<p class='linefeed'>0" << htmlReady[i].hourArriving << ":" << htmlReady[i].minArriving << "</p>" << endl;
    } else {
      html << tab << "<p class='linefeed'>" << htmlReady[i].hourArriving << ":" << htmlReady[i].minArriving << "</p>" << endl;
    }
    html << "    </div>" << endl;
  }
  html << "  </body>" << endl;
  html << "</html>" << endl;
}

void getCurrentTime(int &initialTimeH, int &initialTimeM){
  string midnight, tmp;
  int intertime;
  time_t currentTime;

  currentTime = time(NULL);
  midnight = ctime(&currentTime);

  midnight = midnight.substr(11,8);
  cout << midnight << endl;

  tmp += midnight[0];
  tmp += midnight[1];
  stringstream(tmp) >> intertime;
  initialTimeH = intertime;
  tmp.clear();

  tmp += midnight[3];
  tmp += midnight[4];
  stringstream(tmp) >> intertime;
  initialTimeM = intertime;
  tmp.clear();

}

int main(){
  int errorFlag = 0, updateTime = 5, controlFlag = 0, exitFlag = 0, flag = 0;
  int initialTimeH, initialTimeM;
  string input;
  HANDLE  color = GetStdHandle(STD_OUTPUT_HANDLE);

  inizializeToZero();
  checkfiles(errorFlag, color);
  if (errorFlag == 404){
    cout << left << setw(2) << "[";
    red(color);
    cout << left << setw(7) << "ERROR";
    white(color);
    cout << "]";
    red(color);
    cout <<" Fatal Error" << endl;
    white(color);
    cout << left << setw(2) << "[";
    red(color);
    cout << left << setw(7) << "ERROR";
    white(color);
    cout << "]";
    red(color);
    cout << " Impossible to Continue" << endl;
    white(color);
    cout << "\nPress any key to close..." << endl;
    cin.get();
    exit(EXIT_FAILURE);
  }
  inizializeStruct(errorFlag, color);
  if (errorFlag == 1){
    cout << left << setw(2) << "[";
    yellow(color);
    cout << left << setw(7) << "WARN";
    white(color);
    cout << "] One or more errors occurred" << endl;
    cout << left << setw(2) << "[";
    yellow(color);
    cout << left << setw(7) << "WARN";
    white(color);
    cout << "] It's recommended to check the files" << endl;
    separator();
    workerprint(color);
    cout << "Closing File" << endl;
    workerprint(color);
    cout << "Anyway... Loading Finished" << endl;
    separator();
    wait(2000);
  } else {
    separator();
    workerprint(color);
    cout << "Closing File" << endl;
    workerprint(color);
    cout << "Loading Finished Successfully" << endl;
    separator();
    wait(2000);
  }
  white(color);

  clrscr();

  getCurrentTime(initialTimeH, initialTimeM);

  system("D:/Utente/Desktop/inf/c++/Metropolitan/main/v1.0-AUTOMATIC/html/index.html");

  while (controlFlag == 0){

    workerOutput(initialTimeH, initialTimeM);

    htmlparser();

    updateTime = 5;
    while (exitFlag == 0){
      cout << "Running..." << endl;
      cout << "Next update in: " << updateTime << endl;
      cout << "To exit hold SPACE" << endl;
      if (GetKeyState(VK_SPACE)){
        exitFlag = 1;
        controlFlag = 1;
      }
      wait(1000);
      clrscr();
      updateTime--;
      if (updateTime == 0){
        getCurrentTime(initialTimeH, initialTimeM);
        workerOutput(initialTimeH, initialTimeM);
        htmlparser();
        updateTime = 5;
      }
    }
  }

  return 0;
}
