#ifndef FLSTATICS
#define FLSTATICS
#include"NextworkGenerater.h"
#include"map"
#include   <fstream> 
class flowstatics
{
private:
	NetworkGenerater&Network;
	map<pair<int,int>,float> linkloads;
public:
	flowstatics(NetworkGenerater&net) :Network(net){};
	void output(){
		for (int i = 0; i < Network.flows.size(); i++)
		{
			vector<int> path = Network.nodepath[make_pair(Network.flows[i].start, Network.flows[i].end)];
			int start, end;
			float volume = Network.flows[i].volume;
			for (int j = 0; j < path.size()-1; j++)
			{
				start = path[j], end = path[j + 1];
				linkloads[make_pair(start, end)] += volume;
			}
		}
		map<pair<int, int>, float>::iterator iter;
		int count = 0;
		ofstream outfile("statics.txt");
		for (iter = linkloads.begin(); iter != linkloads.end(); iter++)
		{
			outfile << (*iter).first.first << " " << (*iter).first.second << " " << (*iter).second << endl;
			outfile << (*iter).first.second << " " << (*iter).first.first << " " << linkloads[make_pair((*iter).first.second, (*iter).first.first)] << endl;
			linkloads.erase(make_pair((*iter).first.second, (*iter).first.first));
		}
	}
};
#endif