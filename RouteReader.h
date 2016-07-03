//
// Created by Zhou Jianyu on 2016/6/22.
//

#ifndef BIGPROJECT_ROUTEREADER_H
#define BIGPROJECT_ROUTEREADER_H
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <math.h>

#define MAX_X_NUM 100
#define MAX_Y_NUM 100
#define ROUTE_NUM 20000
#define R 6370996.81
#define pi 3.1416
using namespace std;


class Position{
public:
    int route_id;
    int pid;
    double x;   //经度
    double y;   //纬度
    Position(int rid,int pid,double x,double y):route_id(rid),pid(pid),x(x),y(y){}
};
class RouteReader{
public:
    double xmax,ymax,xmin,ymin;
    double delx,dely;
    string filename;
    int counter[ROUTE_NUM];
    unordered_map<int,vector<Position>> routes;  //建立route 索引
    RouteReader(char* filename);
    ~RouteReader();

    void abstractPos(string record, int pid,unordered_map<int,vector<Position>>&);
    double calDistance(const Position& p1,const Position& p2);
    void filt(vector<pair<int,int>>& candidate);
    void verify(vector<pair<int,int>>& candidate,vector<pair<int,int>>& result);
    void buildMap();
    bool findNearPoint(const Position& p,int iter_time);
    void clear_counter();
    bool is_sim(int r1,int r2);
};













#endif //BIGPROJECT_ROUTEREADER_H
