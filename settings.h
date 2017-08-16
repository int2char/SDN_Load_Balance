#ifndef SETTINGS
#define SETTINGS
#include<map>
#include<vector>
#include"BaseElem.h"
#include<iostream>
//sdn number&sdn node vector;
extern int SDNNUM;
static int node[12] = { 1,6,2,9,15,16,12,0,8,11,18,19 };
static int tcam[12] = { 12, 22, 32, 42, 52, 62, 72, 82, 92, 102, 112, 122 };
//graph node number
#define NODE 23
//reading file flow number
#define FLOWNUM 5788
//max divid route(unuse)
#define MAXROUTE 5
//top large flow number
#define TOPK 5788
#define ROUNUM 78
//tcam limit for sdn
extern int TCAM;//#define TCAM (tcam[SETING])
//#define BAIK (((5*SDNNUM*TCAM)/2))
extern int BAIK;
//link capacity
#define LINLC 2000000
//GA serch population
extern int POP;//#define POP ((BAIK<200)?500:((int)(((BAIK-200)/2+500)/100)+1)*100)
//GA pop divation ,notice:KEEP+CROSS+MUTA=POP
extern int KEEP;//#define KEEP ((POP/10)*2)
extern int CROSS;//#define CROSS ((POP/10)*5)
extern int MUTA;//#define MUTA ((POP/10)*3)
//GA iter settings 
#define MAXITER 1000
#define FEASERCH 100
#define BESTKEEP 15
//search presicion,to end a slow convergency;
#define PRESICION 0.00001
//file root

#define TOPOFILE "D:\\BaiduYunDownload\\ksdnpath\\ksdnpath\\ksdnpath\\dataineed\\Topology_Geant.txt"
#define FLOWVOLUMW "D:\\BaiduYunDownload\\ksdnpath\\ksdnpath\\ksdnpath\\dataineed\\Geant_MultiPrefixs_TM.txt"
#define FLOWST "D:\\BaiduYunDownload\\ksdnpath\\ksdnpath\\ksdnpath\\dataineed\\Geant_MultiPrefixs_TM_Cotents.txt"
//some useful function
#define GETSDNNUM(id) getsdnnum(id)
#define GETSDN(sdn) getsdn(sdn)
#define IFISSDN(id) ifissdn(id)
#define HASCIRCLE(p) ifcircle(p)
#define PATHSDN(p) ifpathsdn(p)
#define CANBEINIT(p) ifcanbeinit(p) 
#define PRINTPATH(p) printpath(p)
//#define CANROTATE(p) ifcanberotate(p)
using namespace std;
static int getsdnnum(int id)
{
	for (int i = 0; i < SDNNUM; i++)
	if (node[i] == id)
		return i;

}
static int getsdn(int sdn){
	return node[sdn];
}
static bool ifissdn(int id)
{
	for (int j = 0; j < SDNNUM; j++){ if (node[j] == id)return true; } return false;
}
static bool ifcircle(vector<int> path){
	map<int, int> pamap;
	for (int i = 0; i < path.size(); i++)
	{
		if (pamap.find(path[i]) == pamap.end())
			pamap.insert(make_pair(path[i], 1));
		else
			return true;
	}
	return false;
}
static bool ifpathsdn(vector<int> path){
	for (int i = 0; i < path.size(); i++)
	if (IFISSDN(path[i]))
	{
		return true;
	}
	return false;
}
static int ifcanbeinit(vector<int> path){
	for (int i = 0; i < path.size() - 1; i++)
	{
		if (IFISSDN(path[i]))
			return i;
	}
	return -1;
}
static void printpath(vector<int>path){
	for (int i = 0; i < path.size(); i++)
		cout << path[i] << " ";
	cout << endl;
}
#endif