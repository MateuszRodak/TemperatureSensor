
#include <iostream>
#include "Thermostat.h"
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

int ile_Paczek = 0;

int main()
{
    std::cout << "OTO PROGRAM C++ DO ODBIERANIA TEMP Z TERMOSTATU\n";

    static std::string oneEvent;
    static std::string allEvents;

    Thermostat thermostat1;
    thermostat1.SetHistereze(1);
    thermostat1.SetDesireTemperature(20);


    int buff;
    std::cout << "NUMER URZADZENIA:\n";
    std::cin >> buff;
    thermostat1.SetDiviceNumber(buff);

    std::cout << "ILE WYGENEROWANYCH PACZEK DANYCH MA ZOSTAC WYSLANYCH NA SERWER\n";
    std::cin >> buff;
    ile_Paczek = buff;

    std::cout << "ODCZYTYWANIE TEMPERATURY \n";

    thermostat1.getEvent(time(0));

    allEvents = "[";
  
    for (int i = 0; i < ile_Paczek; i++)
    {
        oneEvent = thermostat1.getNextEvent(15);
        allEvents = allEvents + oneEvent;
        if (i < ile_Paczek - 1) { allEvents += ","; }
        std::cout << oneEvent << std::endl;
    }
    allEvents += "]";
    std::cout << allEvents << std::endl;

    std::cout << "PRZED CURLEM \n";

    CURL* curl;
    CURLcode res;

    static const char* postThisJson = allEvents.c_str();

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://PAGENAME/DBsavingData.php");

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "username");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "password");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postThisJson);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postThisJson));

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    std::cout << "PO CURLU \n";
    std::cout << "KONIEC PROGRAMU \n";
    return 0;
}
