#ifndef BASEELEM
#define BASEELEM
#include<map>
#include<vector>
#include "stdio.h";
#include "stdlib.h"
#include"settings.h"
#include<iostream>
using namespace std;
/*Base node : normal router:include route table ,port table
class BaseNode{
	private:
		int NodeNum;
		map<int,int> Routable;
		map<int, int> Node2Port;
	public:
		virtual int getTCAM(){
			return MAXTCAM;
		};
		virtual string getNodeType(){
			return "BaseNode";
		};
		int getNext(int num)
		{
			return Routable[num];
		};
		int getPort(int neighbor)
		{
			return Node2Port[neighbor];
		};
		BaseNode(int num,map<int,int>_Routmap) :NodeNum(num),Routable(_Routmap){
			map<int, int>::iterator iter;
			int i = 0;
			for (iter = Routable.begin(); iter != Routable.end(); iter++)
				Node2Port[i++] = iter->second;
		};

};*/
/*SDNnode with extr TACM attr than Basenode
class SDNNode:public BaseNode{
	int TCAMnum;
	public:
		int getTCAM(){
			return TCAMnum;
		}
		string getNodeType(){
			return "SDNnode";
		}
		SDNNode(int num,map<int,int>_Routable) :BaseNode(num,_Routable){};
};*/
/*link class represent each link*/
class Link{
	public:
		int start; int end;
		float capacity;
	public:
		Link(int s, int e, float cap) :start(s), end(e),capacity(cap){};
	
};
/*ways to gernerate the Graph should inherient from this class*/
class GraphIniter{
	friend class Graph;
	public:
		virtual void GraphInit(vector<Link>&,vector<int>*,map<pair<int,int>,int>&)=0;
};
/*generate graph from a file*/
class FileReader:public GraphIniter {
	private:
		string filename;
		FILE* file;
	public:
		FileReader(string _name){
			filename = _name;
			if ((file = fopen(filename.c_str(), "r+")) < 0)
			{
				printf("Graph file open erro!!!");
				throw exception("open Graph erro!!!");
			}
			if (file == NULL)
			{
				printf("erro graph file is null!!!");
				throw exception("erro graph file is null!!!");
			}
	}	
		virtual void GraphInit(vector<Link>&edge, vector<int>* neibor,map<pair<int, int>, int>&ST2E){
			int s=0, t=0;
			int i = 0;
			if (file == NULL)
				printf("erro no file open");
			while (fscanf(file, "%d %d", &s, &t) == 2){
				edge.push_back(Link(s, t, LINLC));
				edge.push_back(Link(t,s , LINLC));
				neibor[s].push_back(t);
				neibor[t].push_back(s);
				ST2E[make_pair(s, t)] = i;
				ST2E[make_pair(t, s)] = i+1;
				i+=2;
			}	
	};
	~FileReader(){
		if (file > 0)
			fclose(file);
	}
};
/*Graph class,include links & neigbor nodes of each node*/
class Graph{
	friend class GraphVisiter;
	private :
		vector<Link> Edges;
		vector<int> Neighbor[NODE];
		map<pair<int, int>, int> ST2E;
	public:
		Graph(GraphIniter &_initer):Edges(0, Link(0, 0, 0)){
			_initer.GraphInit(Edges,Neighbor,ST2E);
		};
		~Graph(){
		}
};
/*only this class can visit the Graph,(for data safety)*/
class GraphVisiter{
	Graph *G;
	public:
		GraphVisiter(Graph *_G){
			G = _G;
		}
		Link getEdge(int num){
			return G->Edges[num];
		}
		vector<int> getNeighbor(int num){
			if (num >= NODE)
			{ 
				printf("erro id out of range in getNeibor!!!\n");
				throw exception("id out of range in getNeibor!!!");
			}
			return G->Neighbor[num];
		};
		int getST2E(pair<int, int> pk)
		{
			return G->ST2E[pk];
		};
		int getEdgeNum(){
			return	G->Edges.size();
		};
	
	
};
class Flow{
public:
	int name;
	int start; int end;
	float volume;
public:
	Flow(int _name,int s, int e, float cap) :name(_name),start(s), end(e),volume(cap){};

};
class shakepath{
	public:
		string id;
		int oring;
		int dividenode;
		vector<int> moretcam;
		vector<int>nodepath;
		vector<int>edgepath;
		int start, end;
		int length;
		shakepath(string _id, vector<int> vec,vector<int>evec,int divid,int _start,int _end,int _orang) :id(_id),nodepath(vec),edgepath(evec),dividenode(divid),start(_start),end(_end),length(nodepath.size()),oring(_orang){};
};
#endif // BASEELEMENT
