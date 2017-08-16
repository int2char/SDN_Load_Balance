#ifndef GASEARCHER
#define GASEARCHER
#include"NextworkGenerater.h"
#include "windows.h"
#include"time.h"
static unsigned long int nexta = 1;
void srand(unsigned int seed){
	nexta = seed;
}
int rand()
{
	nexta = nexta * 1103515245 + 12345;
	return (int)(nexta / 65536) % 32768;
}
class GAsearcher {
private:
	NetworkGenerater Network;
	vector<pair<double, double> > initload;
	vector<int> initTCAMload;
	vector<pair<double, double> > edgeload;
	vector<pair<int, vector<shakepath>>> bearable;
	vector<pair<double, vector<int>>> chormes;
	vector<vector<int>> BestPath;
	vector<vector<int>> BestEdgePath;

	vector<int> LeftTCAM;
	vector<int>taskleft;
	map<int, vector<int>> path2tcam;
	int IfFeasible;
	int EdgeNum;
	int MaxIter;
	int FeasibleNUM;
	int ConvergeIter;
	double BestValue;
public:
	GAsearcher(NetworkGenerater _network, int max, int converge, int feasilbe) :Network(_network), MaxIter(max), ConvergeIter(converge), initTCAMload(SDNNUM, 0), BestPath(TOPK, vector<int>()), BestEdgePath(TOPK, vector<int>()), FeasibleNUM(feasilbe){
		IfFeasible = 1;
		BestValue = 1.7e+18;
		GAbegin();
		playgold();
		float pres = PRESICION;
		int convergekeep = 0;
		for (int i = 0; i < MaxIter; i++)
		{
			srand(111);
			double newvalue = fitness();
			if (newvalue < BestValue - pres)
			{
				IfFeasible = 0;
				Set_console_color(10);
				cout << newvalue << endl;
				BestValue = newvalue;
				if (newvalue < 15)
					pres = 0.0001;

				convergekeep = 0;
				Set_console_color(7);

			}
			else
			{
				cout << BestValue << endl;
				if (IfFeasible > 0)
					IfFeasible++;

			}
			convergekeep++;
			if (convergekeep > ConvergeIter&&IfFeasible == 0)
			{
				break;
			}
			if (IfFeasible > FeasibleNUM)
			{
				cout << endl;
				cout << "no feasible answer find..." << endl;
				break;
			}
			Crossover();
			randmutation();
		}
		GrabBest();
	};
	map<int, vector<int>>GetMap(){
		return path2tcam;
	}//return to match sdn
	vector < pair<pair<int, float>, vector<int>>> GetleftPath()//return to mathsdn
	{
		vector < pair<pair<int, float>, vector<int>>> vec;
		for (int i = 0; i < taskleft.size(); i++)
		{
			if (PATHSDN(BestPath[taskleft[i]]))
				vec.push_back(make_pair(make_pair(taskleft[i], Network.flows[taskleft[i]].volume), BestPath[taskleft[i]]));
		}
		return vec;
	};
	vector<int> GetleftTCAM(){
		return LeftTCAM;
	}//return to muchsdn
	bool IfSuccess(){
		if (IfFeasible == 0)
			return false;
		return true;
	}
	double GetBestValue(){
		return BestValue;
	}
	double Unityvalue(){
		vector<double> eload(EdgeNum, 0);
		for (int i = 0; i < TOPK; i++)
		{
			vector<int>path = Network.edgepath[make_pair(Network.flows[i].start, Network.flows[i].end)];
			for (int j = 0; j < path.size(); j++)
				eload[path[j]] += Network.flows[i].volume;
		}
		float max = 0;
		for (int i = 0; i < EdgeNum; i++)
		if (eload[i]>max)
			max = eload[i];
		cout << "max load is:" << max << endl;
		double value = 0;
		for (int i = 0; i < eload.size(); i++)
		{
			float newvalue = eload[i] / LINLC;
			if (newvalue>value)
				value = newvalue;
			//value += GetUa(eload[i], LINLC);
		}
		return value;
	}
	double Getlest(){
		cout << "edge num:" << EdgeNum << endl;
		vector<double> eload(EdgeNum, 0);
		int checknum = 0;
		for (int i = 0; i < TOPK; i++)
		{
			vector<shakepath>shav = Network.FLowIDPath[i];
			if (shav.size() <= 1)
			{
				checknum++;
				vector<int> npath = shav[0].edgepath;
				for (int j = 0; j < npath.size(); j++)
				{
					eload[npath[j]] += Network.flows[i].volume;
				}
			}
		}
		cout << "check num is:" << checknum << endl;
		double value = 0;
		for (int i = 0; i < eload.size(); i++)
		{

			float newvalue = eload[i] / LINLC;
			if (newvalue>value)
				value = newvalue;
			//value += GetUa(eload[i], LINLC);
		}
		return value;

	}
	double RealShady(){
		int k = 0;
		vector<double> eload(EdgeNum, 0);
		double rrload = 0;
		for (int i = 0; i < TOPK; i++)
		{
			rrload += Network.flows[i].volume;
			for (int j = 0; j < BestEdgePath[i].size(); j++)
			{
				eload[BestEdgePath[i][j]] += Network.realflows[Network.flows[i].name].volume;
			}
		}
		double value = 0;
		cout << endl;
		double totalload = 0;
		for (int i = 0; i < eload.size(); i++)
		{
			float newvalue = eload[i] / LINLC;
			if (newvalue>value)
				value = newvalue;
		}
		return value;
	}
	double CheckValue(){
		int k = 0;
		vector<double> eload(EdgeNum, 0);
		double rrload = 0;
		for (int i = 0; i < TOPK; i++)
		{
			rrload += Network.flows[i].volume;
			for (int j = 0; j < BestEdgePath[i].size(); j++)
			{
				eload[BestEdgePath[i][j]] += Network.flows[i].volume;
			}
		}
		double value = 0;
		/*for (int i = 0; i < eload.size(); i++)
		value += GetUa(eload[i], LINLC);*/
		cout << endl;
		double totalload = 0;
		for (int i = 0; i < eload.size(); i++)
		{

			//totalload += eload[i];
			float newvalue = eload[i] / LINLC;
			if (newvalue>value)
				value = newvalue;
			//value += GetUa(eload[i], LINLC);
		}
		//cout <<"total load is:"<<rrload<<" "<<totalload << endl;
		return value;
	}
	void CheckTCAMResult(){
		int k = 0;
		vector<int>lefttcam;
		for (int j = 0; j < SDNNUM; j++)
			lefttcam.push_back(TCAM);
		for (int i = 0; i < TOPK; i++)
		{
			Flow flow = Network.flows[i];
			vector<shakepath>shav = Network.FLowIDPath[i];//PathSet[make_pair(Network.flows[i].start, Network.flows[i].end)];
			if (shav.size() <= 1)
				continue;
			else
			{
				vector<int>current = chormes[0].second;
				vector<int>cupath = bearable[k].second[current[k]].nodepath;
				int end = cupath[cupath.size() - 1];
				for (int j = 0; j < cupath.size() - 1; j++)
				{
					if (IFISSDN(cupath[j]))
					{
						if (cupath[j + 1] != Network.nodepath[make_pair(cupath[j], end)][1])
							lefttcam[GETSDNNUM(cupath[j])]--;
					}
				}
				k++;
			}

		}
		cout << "check tcam result is:" << endl;
		for (int h = 0; h < SDNNUM; h++)
			cout << lefttcam[h] << " ";
	}
private:
	double GetUa(double volume, double capacity)
	{
		double basevalue = 0;
		double basex = 0;
		double ldca = volume;
		double B[5] = { 0.333333333333, 0.6666666666667, 0.9, 1.0, 1.1 };
		double X[6] = { 1, 3, 10, 70, 500, 5000 };
		if (ldca < B[0] * capacity)
		{
			return (X[0] * ldca) / capacity;
		}
		basex += B[0] * capacity;
		basevalue += B[0] * capacity*X[0];
		if (ldca < B[1] * capacity)
		{
			return (X[1] * (ldca - basex) + basevalue) / capacity;
		}
		basevalue += X[1] * (B[1] - B[0])*capacity;
		basex += (B[1] - B[0])*capacity;
		if (ldca < B[2] * capacity)
		{
			return (X[2] * (ldca - basex) + basevalue) / capacity;
		}
		basevalue += X[2] * (B[2] - B[1])*capacity;
		basex += (B[2] - B[1])*capacity;
		if (ldca < B[3] * capacity)
		{
			return(X[3] * (ldca - basex) + basevalue) / capacity;
		}
		basevalue += (X[3] * (B[3] - B[2])*capacity) / capacity;
		basex += (B[3] - B[2])*capacity;
		if (ldca < B[4] * capacity)
		{
			return (X[4] * (ldca - basex) + basevalue) / capacity;
		}
		basevalue += X[4] * (B[4] - B[3])*capacity;
		basex += (B[4] - B[3])*capacity;
		if (ldca>B[4] * capacity)
			return (X[5] * (ldca - basex) + basevalue) / capacity;


	}
	void GAbegin(){
		cout << endl;
		EdgeNum = Network.GVistor.getEdgeNum();
		for (int i = 0; i < EdgeNum; i++)
		{
			initload.push_back(make_pair(0, LINLC));
		}
		for (int i = 0; i < TOPK; i++)
		{
			float volume = Network.flows[i].volume;
			vector<shakepath>shav = Network.FLowIDPath[i];
			if (shav.size() <= 1)
			{
				vector<int> edgepath = shav[0].edgepath;
				for (int j = 0; j < edgepath.size(); j++)
					initload[edgepath[j]].first += volume;
			}
			else
			{
				bearable.push_back(make_pair(i, shav));
			}
		}
	}
	void playgold(){
		for (int i = 0; i < POP; i++)
		{
			vector<int>temp;
			for (int j = 0; j < bearable.size(); j++)
				temp.push_back(rand() % bearable[j].second.size());
			chormes.push_back(make_pair(0, temp));

		}
	}
	double fitness(){
		for (int i = 0; i <chormes.size(); i++)
		{
			vector<int> tcamuse;
			for (int j = 0; j < SDNNUM; j++)
				tcamuse.push_back(0);
			vector<int>current = chormes[i].second;
			double fitvalue = 0;
			vector<pair<double, double> > load(EdgeNum, make_pair(0, 0));
			for (int j = 0; j < current.size(); j++)
			{
				vector<int >cpath = bearable[j].second[current[j]].edgepath;
				vector<int >more = bearable[j].second[current[j]].moretcam;
				for (int k = 0; k < more.size(); k++)
				{
					int tt = more[k];
					tcamuse[more[k]]++;
				}
				int flowid = bearable[j].first;
				for (int k = 0; k < cpath.size(); k++)
					load[cpath[k]].first += Network.flows[flowid].volume;
			}
			for (int j = 0; j < load.size(); j++)
			{
				load[j].first += initload[j].first;
				load[j].second += initload[j].second;
			}
			int totalover = 0;
			for (int j = 0; j < SDNNUM; j++)
			if (tcamuse[j]>TCAM)
				totalover += (tcamuse[j] - TCAM);
			fitvalue += totalover*1.7e+18;
			tcamuse.clear();
			double value = 0;
			for (int j = 0; j < load.size(); j++)
			{
				double newvalue = load[j].first / LINLC;
				if (newvalue>value)
					value = newvalue;
			}
			fitvalue += value;
			//for (int j = 0; j < load.size(); j++)
				//fitvalue += GetUa(load[j].first, LINLC);
			// cout <<"this is"<< fitvalue << " ";
			chormes[i].first = fitvalue;
		}
		sort(chormes.begin(), chormes.end(), Ccmp);
		return  chormes[0].first;
	}
	void Crossover(){
		vector<vector<int>> newcross;
		for (int i = 0; i < CROSS + MUTA; i += 2){
			int fatherid = rand() % KEEP;
			int montherid = rand() % (CROSS + KEEP);
			vector<int> eleml;
			vector<int> elemr;
			for (int j = 0; j<bearable.size(); j++)
			{
				int dividor = rand() % 10;
				if (dividor <5)
				{
					eleml.push_back(chormes[fatherid].second[j]);
					elemr.push_back(chormes[montherid].second[j]);
				}
				else
				{
					elemr.push_back(chormes[fatherid].second[j]);
					eleml.push_back(chormes[montherid].second[j]);

				}
			}
			newcross.push_back(eleml);
			newcross.push_back(elemr);
		}
		for (int i = KEEP; i < POP; i++)
		{
			chormes[i].second = newcross[i - KEEP];
		}
		newcross.clear();
	}
	void randmutation(){
		int offset = rand() % (CROSS + MUTA) + KEEP;
		int position = (bearable.size()>0) ? rand() % bearable.size() : -1;
		if (position < 0)
			return;
		int flag = 0;
		chormes[offset].second[position] = rand() % (bearable[position].second.size());
	}
	void GrabBest(){
		int k = 0;
		for (int j = 0; j < SDNNUM; j++)
			LeftTCAM.push_back(TCAM);

		for (int i = 0; i < TOPK; i++)
		{
			vector<shakepath>shav = Network.FLowIDPath[i];
			if (shav.size() <= 1)
			{
				BestPath[i] = shav[0].nodepath;
				BestEdgePath[i] = shav[0].edgepath;

				taskleft.push_back(i);
			}
			else
			{
				vector<int>current = chormes[0].second;
				BestPath[i] = bearable[k].second[current[k]].nodepath;
				BestEdgePath[i] = bearable[k].second[current[k]].edgepath;
				vector<int >more = bearable[k].second[current[k]].moretcam;
				if (more.size() == 0)
					taskleft.push_back(i);
				else
					path2tcam[i] = more;
				for (int h = 0; h < more.size(); h++)
				{
					int tt = more[h];
					LeftTCAM[more[h]]--;
				}
				k++;
			}
		}
		cout << "left tcam is:" << endl;
		for (int i = 0; i < LeftTCAM.size(); i++)
			cout << LeftTCAM[i] << " ";
		cout << endl;
	}
	void Set_console_color(double color_index)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
	}
	static bool Ccmp(pair<double, vector<int>> a, pair<double, vector<int>> b){
		if (a.first<b.first)
			return true;
		return false;
	}
};
#endif