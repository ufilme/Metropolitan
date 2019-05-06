#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <iomanip>
#define maxStations 12
#define nMetroLines 4
// first letter of a station = 17 (for strings = 16)
//frequency 3,7
//metropolitan lines 0,2

using namespace std;

time_t currentTime = time(NULL);

struct metropolitan
{
  string line;
  string station[maxStations];
  int frequency[maxStations];
}metroLines[nMetroLines];

struct outputDisplay
{
  string station;
  int secondsLeft;
  int frequency;
  string line;
}display[maxStations * nMetroLines];


void initializeFile(metropolitan metroLines[])
{
  for (int i = 0; i < nMetroLines; i++)
  {
    for (int j = 0; j < maxStations; j++)
    {
      metroLines[i].station[j] = "EMPTY";
      metroLines[i].frequency[j] = -1;
    }
  }
  string fileLine;
  ifstream file ("Fermate.txt");
  if (!file.is_open())
  {
    cout << "Error 404 file not found\n";
    exit;
  }
  int cont;
  for (int i = 0; i < nMetroLines; i++)
  {
    char tmp = 'x';
    int posFirstLetter = 15;
    int posLastLetter;
    bool stationTrue_timeFalse = true;
    char tmp2 = 'x';
    cont = 0;
    file.clear();
    file.seekg(0,std::ios::beg);
    while (getline (file, fileLine))
    {
      if (cont == i)
      {
        metroLines[i].line = fileLine.substr(0,2);
        //metroLines[i].frequency = stoi(fileLine.substr(3,7));
        int j = posFirstLetter;
        int k = 0;
        while (tmp != '*')
        {
          tmp = fileLine[j];
          tmp2 = fileLine[j + 1];
          if (tmp == ',' && tmp2 != tmp)
          {
            posFirstLetter = j;
          }
          if (tmp2 == ',' && tmp != tmp2)
          {
            posLastLetter = j;
            if (stationTrue_timeFalse)
            {
              metroLines[i].station[k] = fileLine.substr(posFirstLetter + 1, posLastLetter - posFirstLetter);
              k++;
              stationTrue_timeFalse = false;
            }
            else
            {
              stationTrue_timeFalse = true;
              metroLines[i].frequency[k - 1] = atoi(fileLine.substr(posFirstLetter + 1, posLastLetter - posFirstLetter).c_str());
            }
          }
          j++;
        }
      }
      cont++;
    }
  }
  file.close();
}

void displayInitialize (outputDisplay display[], metropolitan metroLines[])
{
  int i = 0;
  for (int j = 0; j < nMetroLines; j++)
  {
    display[i].line = metroLines[j].line;
    for (int k = 0; k < maxStations; k++)
    {
        display[i].station = metroLines[j].station[k];
        display[i].frequency = metroLines[j].frequency[k];
        display[i].line = metroLines[j].line;
        i++;
    }
  }
}

void displayOnAction(outputDisplay display[])
{
  while(true)
  {

  }
}

int main()
{
  initializeFile(metroLines);
  /*for (int i = 0; i < nMetroLines; i++)
  {
    cout << metroLines[i].line << endl;
    for (int j = 0; j < maxStations; j++)
    {
      if (metroLines[i].station[j] != "EMPTY" && metroLines[i].frequency[j] != -1)
      {
        cout << metroLines[i].station[j] << endl;
        cout << metroLines[i].frequency[j] << endl;
      }
    }
  }*/
  displayInitialize(display, metroLines);
  cout <<left<< setw(8) << "LINE" << setw(25) << "STATION" << setw(10) << "TIME" << endl;
  for (int i = 0; i < nMetroLines * maxStations; i++)
  {
      if (display[i].station != "EMPTY")
        {
          cout <<left<<setw(8) << display[i].line << setw(25) << display[i].station << setw(10) << display[i].frequency << endl;
        }
  }
  return 0;
}
//21600 6 hours in seconds
//14400 4 hours in seconds
//86400 1 day in seconds
