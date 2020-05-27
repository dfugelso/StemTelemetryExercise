#pragma once
using namespace std;
#include <string>


// Interface class for abstracting alert handlign from detection.
// Different systems may have different alert mechanisms.
class IAlertHandler
{
public:
    virtual void AlertText(string AlertText) = 0;
};

