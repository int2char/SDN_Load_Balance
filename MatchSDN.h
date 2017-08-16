#include"settings.h"
#include"Kuhn.h"
class MatchSDN {
	private:
		int NUM;
		int MUM;
		int**BinaryGraph;
		int *match1;
		int*match2;
		vector<pair<int, int>> inov;
		map<int, vector<int>> path2tcam;
	public:
		map<int, vector<int>> GetMap(){
			return path2tcam;
		}
		MatchSDN(vector<pair<pair<int, float>, vector<int>>>paths, vector<int>LeftTCAM, map<int, vector<int>>_path2tcam):path2tcam(_path2tcam){
			cout << "left path is:" << paths.size() << endl;
			for (int i = 0; i < paths.size();i++)
			if (!ifpathsdn(paths[i].second))
			{
				PRINTPATH(paths[i].second);

			}
			int totalTCAM = 0;
			for (int i = 0; i < LeftTCAM.size(); i++)
				totalTCAM += LeftTCAM[i];
			NUM = (paths.size()>totalTCAM) ? paths.size() : totalTCAM;
			MUM = paths.size();
			//cout << "num is:" << NUM << endl;
			//cout << "mum is:" << MUM << endl;

			int offset=0;
			for (int i = 0; i < LeftTCAM.size(); i++)
			{
				inov.push_back(make_pair(offset, LeftTCAM[i]));
				offset += LeftTCAM[i];
			}
			BinaryGraph = (int**)calloc(MUM, sizeof(int*));
			for (int i = 0; i < MUM; i++)
				BinaryGraph[i] = (int*)calloc(NUM, sizeof(int));
			for (int i = 0; i < MUM; i++)
				for (int j = 0; j < NUM; j++)
				{
					BinaryGraph[i][j] = -inf;
					if (i == j)
						BinaryGraph[i][j] = 0;
				}
			for (int i = 0; i < paths.size(); i++)
			{
				vector<int>currentpath = paths[i].second;
				for (int j = 0; j < currentpath.size(); j++)
				if (IFISSDN(currentpath[j]))
					{
						int id = GETSDNNUM(currentpath[j]);
						for (int k = inov[id].first; k < inov[id].first + inov[id].second; k++)
						{
							BinaryGraph[i][k] = (int)paths[i].first.second+1;
						}
					}
			}
			match1 = (int*)calloc(MAXN, sizeof(int));
			match2 = (int*)calloc(MAXN, sizeof(int));
			for (int i = 0; i < MUM; i++)
			{
				match1[i] = -1;
			}
			for (int i = 0; i < NUM; i++)
			{
				match2[i] = -1;
			}
			//cout << "mum is:" << MUM << endl;

			cout<<"result is"<<KM(MUM, NUM, BinaryGraph, match1, match2)<<endl;
			/*for (int i = 0; i < MUM; i++)
			if (BinaryGraph[i][match1[i]]>0)
			{
				LeftTCAM[GETSDNNUM(TCAM2SDN(match1[i]))]--;
			}*/

			for (int i = 0; i < MUM; i++)
			{
				vector<int>mm;
				if (BinaryGraph[i][match1[i]]>0)
				{
					LeftTCAM[GETSDNNUM(TCAM2SDN(match1[i]))]--;
					mm.push_back(TCAM2SDN(match1[i]));
					path2tcam[paths[i].first.first] = mm;
				}
			}
			/*cout << "match answer is:" << endl;
			for (int i = 0; i < MUM;i++)
			if (path2tcam.find(paths[i].first.first) != path2tcam.end())
				cout << path2tcam[paths[i].first.first][0] << "¡¡";*/
			cout << endl;
			cout << " left tcam is:" << endl;
			for (int i = 0; i < LeftTCAM.size(); i++)
				cout << LeftTCAM[i] << " ";
			cout << endl;
		};
		int TCAM2SDN(int Tid){
			for (int i = 0; i < inov.size(); i++)
			if (Tid >= inov[i].first&&Tid < inov[i].first + inov[i].second)
				return GETSDN(i);
		
		};
		~MatchSDN(){
			for (int i = 0; i < MUM; i++)
				delete[] BinaryGraph[i];
			delete match1;
			delete match2;
		};
};