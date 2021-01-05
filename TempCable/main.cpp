#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

const std::string DEVICEPATH = "/sys/bus/w1/devices/";


float getTemp(std::string& s) {
	float output;
	std::stringstream ss;
	ss << DEVICEPATH << s << "/temperature";
	std::string path = ss.str();
	std::ifstream file(path);
	if (file.good()) {
		std::string line;
		std::getline(file, line);
		output = atof(line.c_str()) / 1000;
		file.close();
	}
	return output;
}

void getSensors(std::vector<std::string>& v) {
	system("sudo modprobe w1-gpio");
	system("sudo modprobe w1-therm");

	DIR* dirp = opendir(DEVICEPATH.c_str());
	struct dirent* dp;
	while ((dp = readdir(dirp)) != nullptr) {
		if (strstr(dp->d_name, "28-00000"))
		{
			v.push_back(dp->d_name);
		}
	}
	closedir(dirp);
}

int main(int argc, char* argv[])
{
	int interval = 5;

	std::vector<std::string> sensors;
	getSensors(sensors);
	printf("\n");
	while (true)
	{
		for (auto sensorID : sensors)
		{
			printf(sensorID.c_str());
			printf("\t");
			printf("temp: %3.3f C", getTemp(sensorID));
			printf("\n");
		}
		printf("\n");


		sleep(interval);
	}

}