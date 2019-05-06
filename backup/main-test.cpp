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
    errorFlag = 1;
    white(color);
  }
  //------------------------------------------
}

void inizializeStruct(int &errorFlag, HANDLE color){
  string line, tmp;
  int pos = 0, staticPos, i = 0, currentLine = 0;
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
    i += nStations;
  }
  file.close();
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
    int time[nLines], tmp , reverseTime[nLines] , tmpReverse, flag = 0;
    int k = 0, j = 0;
    int initialTimeH, initialTimeM;
    int initialSeconds;
    int finalTimeH, finalTimeM;
    int finalSeconds;
    char c;
    int hour, min, sec;
    //while (flag == 0){
      cout << "Insert the initial time(HH:MM): ";
      cin >> initialTimeH >> c >> initialTimeM;
      cout << "Insert the final time(HH:MM): ";
      cin >> finalTimeH >> c >> finalTimeM;
    //  if (c == ':' && initialTimeH >= 0 && initialTimeH <= 24 && initialTimeM >= 0 && initialTimeM <= 59){
      //  if (finalTimeH >= 0 && finalTimeH <= 24 && finalTimeM >= 0 && finalTimeM <= 59){
      //    flag = 1;
    //    }
    //  }
  //  }
    initialSeconds = initialTimeH * 60 * 60 + initialTimeM * 60;
    finalSeconds = finalTimeH * 60 * 60 + finalTimeM * 60;


  while (k != nLines)
  {
    if (initialTimeH % 2 == 0)
    {
      isReversed = false;
    }
    else
    {
      isReversed = true;
    }
    time[k] = normalTime[k];
    reverseTime[k] = reversedTime[k];
    for (int i = initialTimeM * 60; i <= ((finalTimeH - initialTimeH) * 60 * 60 + finalTimeM * 60); i++)
    {
      if (!isReversed)
      {
        if (i == time[k])
        {
          tmp = time[k];
          hour = time[k]/3600;
    	    time[k] = time[k]%3600;
    	    min = time[k]/60;
    	    time[k] = time[k]%60;
    	    sec = time[k];
          cout << station[k * nStations].line << " ";
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
            reverseTime[k] = reverseTime[k]%60;
            sec = reverseTime[k];
            cout << station[k * nStations].line << "r ";
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
          }
      }
    }
    k++;
  }
}


int main(){
  inizializeToZero();
  int errorFlag = 0;
  HANDLE  color = GetStdHandle(STD_OUTPUT_HANDLE);

  checkfiles(errorFlag, color);
  separator();
  inizializeStruct(errorFlag, color);
  white(color);
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
  } else {
    separator();
    workerprint(color);
    cout << "Closing File" << endl;
    workerprint(color);
    cout << "Loading Finished Successfully" << endl;
    separator();
    wait(2000);
    //coolOutput(color);
  }
  white(color);
  cout << "\nPress any key to continue..." << endl;
  cin.get();  //waiting for keyboard inpunt
  clrscr();
  workerOutput();

  return 0;
}
