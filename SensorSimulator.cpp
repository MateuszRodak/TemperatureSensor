#include "Thermostat.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

# define PI          3.141592653589793238462643383279502884L

Thermostat::Thermostat()
{
	diviceNumber = 0;
	histereze = 1;
	heaterOn = false;
	nowWh = time(0);
	desireTemp = 22;
	temperature=0;
	light=0;
	rainEnd = 0;
	srand(nowWh);
}

void Thermostat::UpdateCondition()
{
	struct tm  unpacked;
	errno_t error = localtime_s(&unpacked, &nowWh);
	int minuteDay = unpacked.tm_hour * 60 + unpacked.tm_min;
	double t = minuteDay / (24 * 60.0);
	double s = sin((t-8/24.0) * 2 * PI);
	temperature= 15 + s * 10;
	{
		double b = rand();
		b = b / RAND_MAX - 0.5;//-0,5 - 0,5
		temperature += b * 10;
	}
	light = s*100;
	if (light > 0)//day
	{
		double b = rand();
		b = b / RAND_MAX - 0.5;//-0,5 - 0,5
		b *= 20;//-10 -10
		light += b;
	}
	/*if (rainEnd == 0)
	{
		double b = rand();
		b = b / RAND_MAX;
		if (b > 0.8)
		{
			b = rand();
			b = b / RAND_MAX;
			b *= (60 * 6);
			rainEnd = nowWh + b;
		}
	}
	else if (rainEnd < nowWh)
	{
		light -= 0.1;
		temperature -= 5;
	}
	else
		rainEnd = 0;*/
	if (light < 0)
		light = 0;
}


void Thermostat::UpdateHeater()
{
	if (temperature < (desireTemp - histereze))
		heaterOn = true;
	else if (temperature> ( histereze+ desireTemp))
		heaterOn = false;
}
	
std::string Thermostat::getEvent(time_t wh)
{
	nowWh = wh;
	UpdateCondition();
	UpdateHeater();
	std::stringstream out;

	out << "{"
		<< '"' << "Num" << '"' << ": " << diviceNumber << ','
		<< '"' << "time" << '"' << ": " << wh << ','
		<< '"' << "Temp" << '"' << ": " << std::fixed << std::setprecision(2) << temperature << ','
		<< '"' << "Light" << '"' << ": " << light << ','
		<< '"' << "dTemp" << '"' << ": " << desireTemp << ','
		<< '"' << "Stan" << '"' << ": " << heaterOn
		<< "}";

	return out.str();
}