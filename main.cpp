#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

//Struct declarations
struct adaptor{
	int joltRating;
	vector<int> nextAdaptorJoltage;
};

//Function declarations
string getInput(const string &path);

long long recursiveCount(const vector<adaptor>& adaptorList, const adaptor& myAdaptor, long long dp[]);

int getLargestJolt(vector<int> inputVector);

//Constant declarations & globals
const int MAX_INCREASE =3;

int main()
{
	//Variable declaration
	string filename = "input.txt";
	string input;
	vector<int> joltRatings;
	vector<adaptor> adaptorList;
	int currentJolt = 0;
	int nextJolt = 0;
	int largestJolt = 0;
	int jolt1 = 0;
	int jolt2 = 0;
	int jolt3 = 0;
	int adaptorsComplete = 0;
	uint64_t possiblePaths = 0;

	//Obtain input
	ifstream ifs(filename);
	while(getline(ifs, input))
	{
		joltRatings.push_back(static_cast<int>(stoi(input)));
	}

	sort(joltRatings.begin(), joltRatings.end());

	//Obtain largestJolt
	largestJolt=getLargestJolt(joltRatings);

	long long dp[177];
	for(int i=0; i<177; ++i)
	{
		dp[i]=0;
	}

	//count joltages (this completes part 1)
	while(adaptorsComplete <= joltRatings.size())
	{
		nextJolt = largestJolt;
		for(int i=0; i<joltRatings.size(); ++i)
		{
			if(joltRatings[i]>currentJolt && joltRatings[i]<nextJolt)
			{
				nextJolt = joltRatings[i];
			}
		}
		switch (nextJolt - currentJolt){
			case 1:
				++jolt1;
				break;
			case 2:
				++jolt2;
				break;
			case 3:
				++jolt3;
				break;
			default:
				++jolt3;
				break;
		}
		currentJolt = nextJolt;
		++adaptorsComplete;
	}

	//Part 2
	adaptor outlet;
	outlet.joltRating = 0;
	for(int i=0; i<joltRatings.size(); ++i)
	{
		for(int j=1; j<4; ++j)
		{
			if(joltRatings[i]==j)
			{
				outlet.nextAdaptorJoltage.push_back(joltRatings[i]);
			}
		}
	}
	adaptorList.push_back(outlet);

	for(int i=0; i<joltRatings.size(); ++i)
	{
		adaptor myAdaptor;
		myAdaptor.joltRating = joltRatings[i];

		for(int k=0; k<joltRatings.size(); ++k)
		{
			for(int j=1; j<4; ++j)
			{
				if(joltRatings[k]==joltRatings[i]+j)
				{
					myAdaptor.nextAdaptorJoltage.push_back(joltRatings[i]+j);
				}
			}
		}
		if(joltRatings[i]==largestJolt)
		{
			myAdaptor.nextAdaptorJoltage.push_back(joltRatings[i]+3);
		}

		adaptorList.push_back(myAdaptor);
	}

	cout << recursiveCount(adaptorList, adaptorList[0], dp) << endl;

	//Testing
	cout << largestJolt << endl;
	cout << jolt1 << " " << jolt2 << " " << jolt3 << endl;
	cout << jolt1 * jolt3 << " " << joltRatings.size() << endl;

	for(int i=0; i<adaptorList.size(); ++i)
	{
		cout << "Adaptor List Value: " << adaptorList[i].joltRating << " -> ";
		for(int j=0; j<adaptorList[i].nextAdaptorJoltage.size(); ++j)
		{
			cout << adaptorList[i].nextAdaptorJoltage[j] << " ";
		}
		cout << endl;
	}

	cout << dp[0] << endl;
	cout << dp[1] << endl;
	cout << dp[2] << endl;

	return 0;
}

//Obtain largest jolt rating
int getLargestJolt(vector<int> joltRatings)
{
	int largestJolt = 0;
	for(int i=0; i<joltRatings.size(); ++i)
	{
		if(joltRatings[i]>largestJolt)
		{
			largestJolt=joltRatings[i];
		}
	}
	return largestJolt;
}

//Recursive function to count number of ways to use the adaptors
long long recursiveCount(const vector<adaptor>& adaptorList, const adaptor& myAdaptor, long long dp[])
{
	int aLI = -1;

	if(myAdaptor.joltRating == 176)
	{
		dp[myAdaptor.joltRating] = 1;
		return dp[myAdaptor.joltRating];
	}

	for(int i=0; i<myAdaptor.nextAdaptorJoltage.size(); ++i)
	{	
		if(dp[myAdaptor.nextAdaptorJoltage[i]]==0) //if the next adaptor has unknown path amount, find out
		{
			for(int j=0; j<adaptorList.size(); ++j)
			{
				if(adaptorList[j].joltRating == myAdaptor.nextAdaptorJoltage[i])
				{
					aLI = j;
				}
			}

			dp[myAdaptor.joltRating] += recursiveCount(adaptorList, adaptorList[aLI], dp);
		}
		else if(dp[myAdaptor.nextAdaptorJoltage[i]] != 0) //if the next adaptor has a known path amount, add it to current path amount
		{
			dp[myAdaptor.joltRating] += dp[myAdaptor.nextAdaptorJoltage[i]];
		}
	}
	return dp[myAdaptor.joltRating];
}

/* Not actually used, typed for learning */
string getInput(const string &path)
{
	ifstream ifs(path);
	if (!ifs.is_open())
	{
		cerr << "Could not open the file -'" << path << "'" << endl;
		exit(EXIT_FAILURE);
	}

	return string((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
}