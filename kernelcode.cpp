#include <iostream>
#include <fstream>
using namespace std;

int main(){
// Open the file for CPU information
fstream thisProcFile;
thisProcFile.open ("/proc/sys/kernel/hostname", ios::in);
// Read the file. Note this file only has the host name in it
string line;
getline(thisProcFile, line);
cout << "Hostname: "<<line << endl << endl;
thisProcFile.close();

thisProcFile.open ("/proc/cpuinfo", ios::in);
// Read the file. Note this file only has the host name in it
getline(thisProcFile, line);
cout << line << endl << endl;


getline(thisProcFile, line);
cout << line << endl << endl;


getline(thisProcFile, line);


getline(thisProcFile, line);
cout << line << endl << endl;

getline(thisProcFile, line);
cout << line << endl << endl;
thisProcFile.close();

thisProcFile.open ("/proc/version", ios::in);
// Read the file. Note this file only has the host name in it
getline(thisProcFile, line);
cout <<"Kernel Verison:"<< line << endl << endl;
thisProcFile.close();

thisProcFile.open ("/proc/uptime", ios::in);
// Read the file. Note this file only has the host name in it
double line1;
thisProcFile >> line1;
cout <<"Time since last boot in seconds: " << line1 << endl;
cout << "Time in hours: "<< line1/3600 << endl;

thisProcFile >> line1;
cout <<"Time idle in seconds: " <<line1 << endl;
cout << "Time in hours: "<< line1/3600 << endl<< endl;
thisProcFile.close();

thisProcFile.open ("/proc/meminfo", ios::in);
// Read the file. Note this file only has the host name in it

getline(thisProcFile, line);
cout << line<< endl;

getline(thisProcFile, line);
cout  <<line << endl;
thisProcFile.close();

}
