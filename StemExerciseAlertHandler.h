#pragma once
#include "IAlertHandler.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>

// Handle alerts fro the Stem telemetry exercise. Just writing the alert to the console.
// Instantiates the virtual function AlertText (string AlertText).
class StemExerciseAlertHandler :
	public IAlertHandler
{
public:
	void AlertText(string AlertText);
};

