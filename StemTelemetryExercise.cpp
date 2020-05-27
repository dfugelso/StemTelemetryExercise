// StemTelemetryExercise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
using namespace std;

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>

#include "StemExerciseAlertHandler.h"
#include "ProcessTelemetry.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: StemTelemetryExercise <Telemerty Input Filename>\n";
        exit(0);
    }

    std::cout << "Reading telemetry from " << argv[1] << "\n";

    ifstream file(argv[1], ios_base::in);

    if (!file.is_open())
    {
        cout << "\nInput file: " << argv[1] << " was not found.\n";
        exit(1);
    }

    // create alert handler and telemetry processor
    string telemetryInputLine;
    StemExerciseAlertHandler alertHandler;
    ProcessTelemetry processTelemetry(alertHandler);

    // Process each line
    while (getline(file, telemetryInputLine))
    {
        cout << "Processing line: " << telemetryInputLine << "\n";
        processTelemetry.ProcessTelemetryLine(telemetryInputLine);
    }

    cout << "Press enter to exit!\n";
}


