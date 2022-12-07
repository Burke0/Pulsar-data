/*John Alex Burke
 * prog2.cpp
 * last modified 2/10/19
 * This program processes data from a pulsar data txt file and stores them in 5 parallel arrays.
 It also creates a filtered txt file based on a sigma value threshold, and a dmBins file in which the largest sigma of each dm value is output. 
 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAXSIGNALS=53000;
typedef double doubleArrays[MAXSIGNALS];
typedef int intArrays[MAXSIGNALS];

//function prototypes
int readFile(string&, doubleArrays, doubleArrays, doubleArrays, intArrays, intArrays );
void head(doubleArrays, doubleArrays, doubleArrays, intArrays, intArrays,string);
void tail(doubleArrays, doubleArrays, doubleArrays, intArrays, intArrays,int,string);
void sigmaFilter(doubleArrays, doubleArrays, doubleArrays, intArrays, intArrays,int,string, double);
void dmBins(doubleArrays, doubleArrays,int);

int main() {
	string header;
	doubleArrays DM = {};//setting all values = 0
	doubleArrays sigma= {};
	doubleArrays time= {};
	intArrays sample= {};
	intArrays downfact= {};
	int numOfSignals=0;
	double threshold=16;//this can be changed to create filtered files looking for different sigma values

//function calls
	numOfSignals= readFile(header, DM, sigma, time, sample, downfact);
	head(DM, sigma, time, sample, downfact,header);
	tail(DM, sigma, time, sample, downfact, numOfSignals,header);
	sigmaFilter(DM, sigma, time, sample, downfact, numOfSignals,header,threshold);
	dmBins(DM, sigma,numOfSignals);
	return 0;
}
int readFile(string& head, doubleArrays DM, doubleArrays sigma, doubleArrays time, intArrays sample, intArrays downfact )
{
	/*readFile: reads the file �palfa.txt�, saves the header (first line of the file), and populates
the five parallel arrays with values. It should also return an integer representing the actual
number of signals in the file.
	 */

	int count=0;
	ifstream infile;
	infile.open("palfa.txt");
	if (infile.fail())
		cout<<"file failed to open?"<<endl;

	getline(infile, head);

	while(!infile.eof())
	{
		infile >> DM[count] >> sigma[count] >> time[count] >> sample[count] >> downfact[count];
		count++;
	}

	infile.close();
	return count-1;
}

void head(doubleArrays DM, doubleArrays sigma , doubleArrays time, intArrays sample, intArrays downfact, string header)
{
	cout<<header<<endl;
	for(int i=0; i<20; i++)//displays first 20 signals
		cout<<DM[i]<<" "<<sigma[i]<<" "<< time[i]<<" "<<sample[i]<<" "<<downfact[i]<<" "<<endl;
	
}
void tail(doubleArrays DM, doubleArrays sigma, doubleArrays time, intArrays sample, intArrays downfact, int numSignals, string header)//displays last 20 signals
{
	cout<<endl<<header<<endl;
	for(int i =numSignals-20; i<numSignals; i++)//displays last 20 signals
		cout<<DM[i]<<" "<<sigma[i]<<" "<< time[i]<<" "<<sample[i]<<" "<<downfact[i]<<" "<<endl;
}
void sigmaFilter(doubleArrays DM, doubleArrays sigma, doubleArrays time, intArrays sample, intArrays downfact, int numSignals, string header, double threshold)
{
	
/*� creates a file called �filtered.txt� that contains the header and all signals
with a sigma value higher than a threshold (like 10.0).
 */
 ofstream filtered("filtered.txt");
 filtered<<header<<endl;
 for(int i=0; i<numSignals; i++)
 {
 	if(sigma[i]>threshold)
 	filtered<<DM[i]<<" "<<sigma[i]<<" "<< time[i]<<" "<<sample[i]<<" "<<downfact[i]<<" "<<endl;
 }
 
 filtered.close();
}
void dmBins(doubleArrays DM, doubleArrays sigma, int numSignals)
{
	/*creates a file called �binned.txt� that contains two columns and 13 rows plus a
header. Each row will contain a unique DM and the maximum sigma value recorded for
that DM. There are 12 distinct DMs in �palfa.txt� ranging from 0 to 1.2 in increments of
0.1.*/
	ofstream dmBins("dmBins.txt");
	dmBins<<"DM  MAXSigma"<<endl;
	int current=0;
	for(double u=0.0; u<=1.2; u+=.1)
	{
		double highest=0.0;
			do
			{
				if(sigma[current]>highest)
					highest=sigma[current];
				
				current++;
			}
			while(DM[current]<u+0.1 && current<=numSignals);
		
		dmBins<<u<<" "<<highest<<endl;
	}
	
	
	
	dmBins.close();
}

