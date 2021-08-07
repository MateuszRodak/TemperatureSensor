#pragma once
#include<string>
/**
* Simulate thermostate device, and generate status frames:
* unix timeStamp : temperature (in C degree) :  light level (0-100) : desireTemp  (in C degree) : state of heater (0 or 1);
**/
class Thermostat
{
	time_t nowWh;
	int diviceNumber;
	double temperature;
	double light;
	time_t rainEnd;
	void UpdateCondition();
	double desireTemp;
	double histereze;
	bool heaterOn;
	void UpdateHeater();

public:
	Thermostat();
	/**
	* Set histereze of thermostate in Celcius degress
	**/
	void SetHistereze(double level = 1)
	{
		level = fabs(level);
		if(level>0.1)
			histereze = level;
	}
	/**
	* Set thermostate desire value in Celcius degress
	**/
	void SetDesireTemperature(double temp)
	{
		desireTemp = temp;
	}
	/**
	* generate new frame for @param wh timestamp
	**/
	std::string getEvent(time_t wh = 0);
	/**
	* generate new frame for next timestamp with customable step
	**/
	std::string getNextEvent(int timeStepMin = 15)
	{
		return getEvent(nowWh + timeStepMin*60);
	}

	void SetDiviceNumber(int number)
	{
		diviceNumber = number;
	}


};

