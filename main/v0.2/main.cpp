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

void calc(){

}

int main(){
  int errorFlag = 0;
  HANDLE  color;
  color = GetStdHandle(STD_OUTPUT_HANDLE);
  white(color);

  checkfiles(errorFlag, color);
  separator();
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

  calc();
  return 0;
}
