#pragma once

using namespace std;

#include <chrono>
#include <iostream>
#include <string>

#include "IAlertHandler.h"

using namespace std::chrono;

// Class to handle telemetry input and create alerts if necessary.
// This class detects:
//
// Telemetry input line is text in comma delimited format for:
// Time, kW, V, I, kW-avg, V-avg, I-avg
// 
//    * Time gaps greated than 1.5 seconds between inputs
//    * kW values < 0.0
//    * V values outside range of 480 V + / -5.0 V
//    * I values < 0.0.
//
class ProcessTelemetry
{
public:
	ProcessTelemetry(IAlertHandler& AlertHandler);
	void ProcessTelemetryLine(string TelemetryLine);

private:
	double convertTimeToDouble(string timestamp);
	IAlertHandler& alertHandler;
	unsigned int numberOfLines;
	double timeOfLastEntry;
};

