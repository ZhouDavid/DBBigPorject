#include <iostream>
#include <stdio.h>
#include <time.h>
#include "RouteReader.h"
using namespace std;

int main(int argc, char **argv) {
    clock_t start,end;
    start=clock();
    RouteReader reader(argv[1]);
    vector<pair<int,int>> result;  //两个相似的route_id
    vector<pair<int,int>> candidate;
    reader.buildMap();
    reader.filt(candidate);
    reader.verify(candidate,result);
    int can_size = candidate.size();
    int res_size = result.size();
    cout<<"cansize="<<can_size<<"res_size="<<res_size<<endl;
    ofstream fout("E:\\DBBigProject\\result.txt");
    for(int i  =0;i<result.size();i++){
        fout<<result[i].first<<' '<<result[i].second<<endl;
    }
    fout.close();
    end=clock();
    cout<<"spend time:"<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
    return 0;
}