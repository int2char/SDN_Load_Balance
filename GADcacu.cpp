#include"BaseElem.h"
#include"NextworkGenerater.h"
#include"GAsearcher.h"
#include"MatchSDN.h"
#include   <fstream> 
#include"mex.h"
#define File "C:\\Users\\kb318zq\\Desktop\\Gsdnvarry.txt"
#include"flowstatics.h"
using namespace std;
int SDNNUM = 12;
int TCAM = 12;
int BAIK = (5 * SDNNUM*TCAM) / 2;
int POP = ((BAIK < 200) ? 500 : ((int)(((BAIK - 200) / 2 + 500) / 100) + 1) * 100);
int KEEP = (POP / 10) * 2;
int CROSS = (POP / 10) * 5;
int MUTA = (POP / 10) * 3;
void setparameter(int sdn, int Tcam){
	SDNNUM = sdn;
	TCAM = Tcam - ROUNUM;
	BAIK = (5 * SDNNUM*TCAM) / 2;
	POP = ((BAIK < 200) ? 500 : ((int)(((BAIK - 200) / 2 + 500) / 100) + 1) * 100);
	KEEP = (POP / 10) * 2;
	CROSS = (POP / 10) * 5;
	MUTA = (POP / 10) * 3;


}
double Unityvalue(NetworkGenerater&Network){
	int EdgeNum = Network.GVistor.getEdgeNum();
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
	return max;
}
/*void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	setparameter(*mxGetPr(prhs[2]), *mxGetPr(prhs[3]));
	cout << BAIK << "¡¡" << TCAM << " " << POP << endl;
	NetworkGenerater network(mxGetPr(prhs[0]), mxGetPr(prhs[1]));
	GAsearcher Gas(network, MAXITER, BESTKEEP, FEASERCH);
	cout << "init value is:" << Gas.Unityvalue() << endl;
	cout << "check value is:" << Gas.CheckValue() << endl;
	cout << "check lest value is:" << Gas.Getlest() << endl;
	if (!Gas.IfSuccess())
	{
		MatchSDN match(Gas.GetleftPath(), Gas.GetleftTCAM(), Gas.GetMap());
		map<int, vector<int>>resultmap = match.GetMap();
		plhs[0] = mxCreateDoubleMatrix(FLOWNUM, resultmap.size(), mxREAL);
		double*C = mxGetPr(plhs[0]);
		for (int i = 0; i < resultmap.size(); i++)
		for (int j = 0; j < FLOWNUM; j++)
		{

			if (j == network.flows[i].name)
				C[i*FLOWNUM + j] = 1;
			else
				C[i*FLOWNUM + j] = 0;
		}
		plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
		double*B = mxGetPr(plhs[1]);
		B[0] = Gas.RealShady();
		cout << "real flow is:" << Gas.RealShady();
		cout << "add in:" << resultmap.size() << endl;
	}
	else{
		plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
		double*B = mxGetPr(plhs[1]);
		B[0] = -1;
		plhs[0] = mxCreateDoubleMatrix(FLOWNUM, 1, mxREAL);
		double*C = mxGetPr(plhs[0]);
		for (int i = 0; i < 200; i++)
		for (int j = 0; j < FLOWNUM; j++)
			C[i*FLOWNUM + j] = 0;
	}
	return;
}*/

int main()
{
	int sdd = 6;
	int tc = 75;
	//for (int gg =0; gg <12; gg++)
	{

		setparameter(6, ROUNUM);
		cout << BAIK << "¡¡" << TCAM << " " << POP << endl;
		ofstream outfile(File, ios::app);
		float valuerel = 0;
		float valuega = 0;
		for (int cnt = 0; cnt < 1; cnt++)
		{
			NetworkGenerater network(cnt);
			flowstatics flos(network);
			flos.output();
			/*GAsearcher Gas(network, MAXITER, BESTKEEP, FEASERCH);
			cout << "init value is:" << Gas.Unityvalue() << endl;
			valuerel += Gas.Unityvalue();
			float gac = Gas.CheckValue();
			cout << "check value is:" << gac << endl;
			cout << "check lest is:" << Gas.Getlest() << endl;
			valuega += gac;
			if (!Gas.IfSuccess())
			{
				MatchSDN match(Gas.GetleftPath(), Gas.GetleftTCAM(), Gas.GetMap());
				map<int, vector<int>>resultmap = match.GetMap();
				cout << "add in size is:" << resultmap.size() << endl;
			}*/
		}
		cout << "finally value is:" << valuega / 10 << " " << valuerel / 10 << endl;
		outfile << SDNNUM << " " << TCAM + ROUNUM << ":" << endl;
		outfile << "finally value is:" << valuega / 10 << " " << valuerel / 10 << endl;
		outfile.close();
	}
	while (true);
}

