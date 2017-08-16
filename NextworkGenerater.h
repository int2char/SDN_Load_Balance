#ifndef NETWORKGENERATER
#define NETWORKGENERATER
#include "BaseElem.h"
#include<algorithm>
#include<sstream>
#include<queue>
class shorter
{
public:
	bool operator()(shakepath shake1,shakepath shake2){
	
		int len1 = shake1.length;
		int len2 = shake2.length;
		if (shake1.dividenode < 0)len1 += 1000;
		if (shake2.dividenode < 0)len2 += 1000;
		if (len1 >len2)
			return true;
		return false;
	}
};
class NetworkGenerater{
private:
	FileReader reader;
	Graph G;
public:
	GraphVisiter GVistor;
	map<pair<int, int>, vector<int> > nodepath;
	map<pair<int, int>, vector<int> > edgepath;
	map<string, int>pathmark;
	vector<Flow> flows;
	vector<Flow> realflows;
	map<pair<int,int>,vector<shakepath>>PathSet;
	map<int, vector<shakepath>> FLowIDPath;
	int Cnt;
public:
	NetworkGenerater(int _Cnt) :reader(TOPOFILE), G(reader), GVistor(&G),Cnt(_Cnt){
		floyd();
		readflows();
		sort(flows.begin(), flows.end(), cmp);
		/*cout << " checking path is:" << endl;
		PRINTPATH(nodepath[make_pair(16,20)]);
		vector<int> path = GVistor.getNeighbor(17);
		for (int i = 0; i < GVistor.getNeighbor(17).size(); i++)
		if (nodepath.find(make_pair(path[i], 20)) != nodepath.end())
		{
			cout << "erro relly happens!!!";
			PRINTPATH(nodepath[make_pair(path[i],20)]);
		}*/
		/*for (int i = 0; i < TOPK; i++)
		{
			if (nodepath.find(make_pair(flows[i].start, flows[i].end)) == nodepath.end())
				cout << "erro: " << i << endl;
			else
				cout << "right: " << i << endl;
				
		}*/

		//GrabPathset();
		cout << "diversion:" << flows[0].volume << endl;
	}
	NetworkGenerater(double*C) :reader(TOPOFILE), G(reader), GVistor(&G), Cnt(0){
		floyd();
		readflows(C,flows);
		sort(flows.begin(), flows.end(), cmp);
		//GrabPathset();
		//cout << "diversion:" << flows[0].volume <<" "<<flows[1].volume<<" "<<flows[2].volume<<endl;
	}
	NetworkGenerater(double*C,double*CR) :reader(TOPOFILE), G(reader), GVistor(&G), Cnt(0){
		floyd();
		readflows(C,flows);
		readflows(CR, realflows);
		sort(flows.begin(), flows.end(), cmp);
		//GrabPathset();
		//cout << "diversion:" << flows[0].volume <<" "<<flows[1].volume<<" "<<flows[2].volume<<endl;
	}
private:
	    void readflows(){
		pair<int, int> FlowST[FLOWNUM];
		FILE*cont = readfile(FLOWST);
		int s, t;
		char stbuf[100];
		while (fgets(stbuf,99,cont)!=NULL){
			char*stnode = strtok(stbuf, " ");
			s = atoi(stnode);
			t=atoi(strtok(NULL, " "));
			char buf[10000];
			fgets(buf, 10000, cont);
			char*p;
			p = strtok(buf, " ");
			do{
				FlowST[atoi(p)] = make_pair(s, t);
				p = strtok(NULL, " ");
			} while (p != NULL);
		 }
		FILE *data = readfile(FLOWVOLUMW);
			float volume;
			int name = 0;
			char flowbuf[20000];
			while (fgets(flowbuf,19999,data)!=NULL)
			{
				char*p=0;
				p= strtok(flowbuf, " ");
				int i = 0;
				while (i < Cnt){
					p = strtok(NULL, " ");
					i++;
				}
				volume = atof(p);
				if (volume < 0)
					volume = 0;
				//if (PATHSDN(nodepath[make_pair(FlowST[name].first, FlowST[name].second)]))
					flows.push_back(Flow(name, FlowST[name].first, FlowST[name].second,volume));
				name++;
			}

		}
		void readflows(double *C, vector<Flow>&ansflows)
		{
			pair<int, int> FlowST[FLOWNUM];
			FILE*cont = readfile(FLOWST);
			int s, t;
			char stbuf[100];
			while (fgets(stbuf, 99, cont) != NULL){
				char*stnode = strtok(stbuf, " ");
				s = atoi(stnode);
				t = atoi(strtok(NULL, " "));
				char buf[10000];
				fgets(buf, 10000, cont);
				char*p;
				p = strtok(buf, " ");
				do{
					FlowST[atoi(p)] = make_pair(s, t);
					p = strtok(NULL, " ");
				} while (p != NULL);
			}
			int name = 0;
			float volume = 0;
			int check = 0;
			for (name; name < TOPK; name++)
			{
				volume = 0;
				if (C[name]>0)
					volume = C[name];
				ansflows.push_back(Flow(name, FlowST[name].first, FlowST[name].second, volume));
			}
		}
		void readflowsdata(){
			pair<int, int> FlowST[FLOWNUM];
			FILE*cont = readfile(FLOWST);
			int s, t;
			char stbuf[100];
			while (fgets(stbuf, 99, cont) != NULL){
				char*stnode = strtok(stbuf, " ");
				s = atoi(stnode);
				t = atoi(strtok(NULL, " "));
				char buf[10000];
				fgets(buf, 10000, cont);
				char*p;
				p = strtok(buf, " ");
				do{
					FlowST[atoi(p)] = make_pair(s, t);
					p = strtok(NULL, " ");
				} while (p != NULL);
			}
			FILE *data = readfile(FLOWVOLUMW);
			float volume;
			int name = 0;
			while (fscanf(data, "%f", &volume) == 1)
			{
				if (volume < 0)
					volume = 0;
			//if (PATHSDN(nodepath[make_pair(FlowST[name].first, FlowST[name].second)]))
				flows.push_back(Flow(name, FlowST[name].first, FlowST[name].second, volume));
				name++;
			}

		}

		FILE* readfile(char*filename){
				FILE*file;
				if ((file = fopen(filename, "r+")) < 0)
				{
					printf("data file open erro!!!");
					throw exception("open data erro!!!");
				}
				if (file == NULL)
				{
					printf("erro data file is null!!!");
					throw exception("erro data file is null!!!");
				}
				return file;

		}
		void floyd(){
			vector<vector<int> > dist(NODE, vector<int>(NODE, NODE));
			vector<vector<int> > preNode(NODE, vector<int>(NODE, -1));

			for (int i = 0; i < NODE; ++i)
			{
				int src = i, dst;
				dist[src][src] = 0;
				vector<int>Neibor=GVistor.getNeighbor(i);
				for (int k = 0; k < Neibor.size();k++)
				{
					dst = Neibor[k];
					dist[src][dst] = 1;
					//dist[dst][src] = 1;
					preNode[src][dst] = src;
				}
			}
			
			for (int k = 0; k < NODE; ++k)
			{
				for (int i = 0; i < NODE; ++i)
				{
					for (int j = 0; j < NODE; ++j)
					{
						if (dist[i][j] > dist[i][k] + dist[k][j])
						{
							dist[i][j] = dist[i][k] + dist[k][j];
							preNode[i][j] = preNode[k][j];
						}
					}
				}
			}
			int id = 0;
			for (int i = 0; i < NODE; ++i)
			{
				//for(int j = i + 1; j < NodeCnt; ++j)
				for (int j = 0; j < NODE; ++j)
				{
					if (i == j) continue;
					if (preNode[i][j] == -1) continue;
					vector<int> curpath;
					curpath.push_back(j);
					int k = j;
					while (preNode[i][k] != -1)
					{
						curpath.push_back(preNode[i][k]);
						k = preNode[i][k];
					}
					++id;
					reverse(curpath.begin(), curpath.end());
					nodepath.insert(make_pair(make_pair(i, j), curpath));
					map<pair<int, int>, vector<int>>::iterator iter;
					for (iter = nodepath.begin(); iter != nodepath.end(); iter++)
					{
						vector<int> temp;
						for (int i = 0; i < iter->second.size()-1; i++)
						{
							temp.push_back(GVistor.getST2E(make_pair(iter->second[i], iter->second[i + 1])));
						}
						edgepath[iter->first] = temp;
						temp.clear();
					}
				}
			}
			
		}
		void GrabPathset(){
			string name = "";
			int baicnt = 0;
			for (int i = 0; i < TOPK; i++)
			{
					priority_queue<shakepath, vector<shakepath>, shorter> pathque;
					vector<int> initpath = nodepath[make_pair(flows[i].start, flows[i].end)];
					int dividep = CANBEINIT(initpath);
					shakepath shaker(MakeName(initpath, initpath[0], initpath[initpath.size() - 1]), initpath, node2edge(initpath), dividep, initpath[0], initpath[initpath.size() - 1], 1);
					pathque.push(shaker);
					int GENn = 1;
					int iter = 0;
					if(baicnt<BAIK){
						do
						{
							shakepath tempshaker = pathque.top();
							pathque.pop();
							GENn--;
							GENn += Generatemore(tempshaker, pathque);
							pathque.push(tempshaker);
						} while (GENn > 0);
					}
					vector<shakepath> thispaths;
					while (!pathque.empty()){
						thispaths.push_back(pathque.top());
						pathque.pop();
					}
					if (thispaths.size() > 1)
						baicnt++;
					PathSet[make_pair(flows[i].start, flows[i].end)] = thispaths;
					FLowIDPath[i]=thispaths;
			}
			
		}
		int Generatemore(shakepath &path, priority_queue<shakepath, vector<shakepath>, shorter>&queue){
			//cout << "jj:"; PRINTPATH(path.nodepath);
			int shakenode = path.dividenode;
			if (shakenode < 0)
				return -1;
			vector<int>nopath=path.nodepath;
			vector<int>firstpart;
			int i = 0;
			int nextnode = 0;
			while (true){
				nextnode = nopath[i+1];
				firstpart.push_back(nopath[i]);
				if (i==shakenode)
					break;
				i++;
			}	
			vector<int>neibor = GVistor.getNeighbor(nopath[shakenode]);
			//cout << "nextnode"<<nextnode << endl;
			int GENn = 0;
			for (int i = 0; i < neibor.size(); i++)
			{
				vector<int> newpath(firstpart);
				vector<int>secondpart;
				if (neibor[i] != nextnode)
				{
					if (nodepath.find(make_pair(neibor[i], path.end)) != nodepath.end())
					{
						secondpart = nodepath[make_pair(neibor[i], path.end)];
						for (int i = 0; i < secondpart.size(); i++)
							newpath.push_back(secondpart[i]);
						if (!HASCIRCLE(newpath) && newpath[newpath.size() - 1] == path.end)
						{
							GENn++;
							//cout << "generate success!!!" << endl;
							/*for (int i = 0; i < newpath.size(); i++)
							{
							cout << newpath[i] << " ";
							}*/
							int newdivide = -1;
							for (int i = shakenode + 1; i < newpath.size() - 1; i++)
							if (IFISSDN(newpath[i]))
							{
								newdivide = i;
								//cout << "set new divid" << newpath[i] << " ";
							}
								shakepath newshaker = shakepath(MakeName(newpath, path.start, path.end), newpath, node2edge(newpath), newdivide, path.start, path.end, 0);
								newshaker.moretcam = path.moretcam;
								newshaker.moretcam.push_back(GETSDNNUM(newpath[shakenode]));
								//cout << "neibor i in:" << neibor[i] << endl;
								queue.push(newshaker);
							//cout << endl;
						}
					}
				}
			}
			path.dividenode = -1;
			for (int i = shakenode + 1; i < nopath.size() - 1; i++)
			{
				if (IFISSDN(nopath[i]))
				{
					path.dividenode = i;
				}
			}
			return GENn;
			
		}
		string MakeName(vector<int>path,int start,int end){
			stringstream st;
			st << "({" << start << "," <<end << "}:";
			string name = st.str();
			for (int i = 0; i < path.size(); i++)
				if (IFISSDN(path[i]))
				{
					stringstream ss;
					ss << path[i];
					string s1 = ss.str();
					name += "(" + s1 + ",";
					stringstream sf;
					if (i != path.size() - 1)
					{
						sf << path[i + 1];
					}
					else
						sf << -1;
					name += sf.str() + ")" + ",";
				}
			name += ")";
			//cout << " name is :" << name << endl;
			return name;
		}
		vector<int> node2edge(vector<int>path){
			vector<int>result;
			for (int i = 0; i < path.size()-1; i++)
			{
				result.push_back(GVistor.getST2E(make_pair(path[i], path[i + 1])));
			}
			return result;
		}
		static bool cmp(Flow f1, Flow f2)
		{
			if (f1.volume >f2.volume)
				return true;
			return false;
		}

};
#endif