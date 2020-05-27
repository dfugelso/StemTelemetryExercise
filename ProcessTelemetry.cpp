#include "ProcessTelemetry.h"

#include <atltime.h>
#include <iomanip>
#include <sstream>
#include <vector>

// Gap time between telementry to create an alert
const double kAlertTimeGap = 1.5;    

// Minimum kW value
const double kMinkW = 0;

// Valid range for volatage
const double kMinV = -5;
const double kMaxV = 480;

// Minimum current value
const double kMinI = 0;

/// <summary> 
/// Instantiate a the process telemetry class. Set time to epoch and number of lines
/// read to zero. Store the alert handler.
/// </summary>
/// <Input>
/// <AlertHandler cref="IalertHandler" processes alerts>
/// </Input>
ProcessTelemetry::ProcessTelemetry(IAlertHandler& AlertHandler) : alertHandler(AlertHandler)
{
	numberOfLines = 0;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	timeOfLastEntry = 0;
}

// I'd add property handlers for all the elements here. But not for an exercise.

/// <summary> 
/// Process a line of telemetry and look for amamolies in the line.
/// Telemetry input line is text in comma delimited format for:
/// Time, kW, V, I, kW-avg, V-avg, I-avg (Note - from read me. Given file doesn't have the averages.)
/// 
///    * Time gaps greated than 1.5 seconds between inputs
///    * kW values < 0.0
///    * V values outside range of 480 V + / -5.0 V
///    * I values < 0.0.
///
/// And,
///
///    * Poorly formed input.
///
/// </summary>
/// <Input>
/// <TelemetryLine cref="string" telemetry input of form <time>, <kW>, <V>, <I>, <kW-avg>, <V-avg>, <I-avg>
/// </Input>
void ProcessTelemetry::ProcessTelemetryLine(string TelemetryLine)
{
	double telemetryTime;
	double kW;
	double V;
	double I;

	// parse telemetry line
	try {
		vector<string> telemetryValues;
		stringstream line(TelemetryLine);

		while (line.good()) {
			string substr;
			getline(line, substr, ',');
			telemetryValues.push_back(substr);
		}

		// convert date/time to double
		telemetryTime = convertTimeToDouble(telemetryValues[0]);

		// kV, V, I
		kW = stod(telemetryValues[1]);
		V = stod(telemetryValues[2]);
		I = stod(telemetryValues[3]);

	}
	catch (const std::exception & e)
	{
		alertHandler.AlertText(e.what());
	}

	// Check for time gap greater than 1.5 seconds
	if (telemetryTime - timeOfLastEntry > kAlertTimeGap&& timeOfLastEntry != 0)
	{
		alertHandler.AlertText(TelemetryLine + " :: HAS TIME GAP.");
	}
	timeOfLastEntry = telemetryTime;

	// Check kw, V, I
	if (kW < kMinkW)
	{
		alertHandler.AlertText(TelemetryLine + " :: kW HAS INVALID VALUE.");
	}

	if (V < kMinV || V > kMaxV)
	{
		alertHandler.AlertText(TelemetryLine + " :: VOLTAGE HAS INVALID VALUE.");
	}

	if (I < kMinI)
	{
		alertHandler.AlertText(TelemetryLine + " :: CURRENT HAS INVALID VALUE.");
	}
}

// Converts date string to number of seconds from epoch with milliseconds as the fractional part.
// Lots can go wrong with time; generally I'd use a thrid party library (boost, Noda Time)
// to handle all the nuances here. Since this is a Windows program,
// this doesn't use striptime. This is fairly naive.
double ProcessTelemetry::convertTimeToDouble (string timestamp)
{
	int yy, month, dd, hh, mm; 
	double secondsAndMilliseconds;

	sscanf_s(timestamp.c_str(), "%4d - %d - %d %d:%d : %lf", &yy, &month, &dd, &hh, &mm, &secondsAndMilliseconds);

	struct tm timeStamp = { 0 };
	timeStamp.tm_year = yy - 1900;
	timeStamp.tm_mon = month - 1;
	timeStamp.tm_mday = dd;
	timeStamp.tm_hour = hh;
	timeStamp.tm_min = mm;
	timeStamp.tm_sec = (int)floor(secondsAndMilliseconds);
	timeStamp.tm_isdst = -1;
	int time = (int)mktime(&timeStamp);

	// Add in the milliseconds
	return (double)time + secondsAndMilliseconds - timeStamp.tm_sec;
}
