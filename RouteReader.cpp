//
// Created by Zhou Jianyu on 2016/6/22.
//
#include "RouteReader.h"
vector<int>Map[MAX_X_NUM][MAX_Y_NUM];

RouteReader::RouteReader(char* filename){
    xmax = ymax = 0;
    xmin = ymin = 1000;
    delx = dely = 0;
    this->filename = string(filename);
    ifstream fin(filename);
    if(!fin){cout<<"fail to open file"<<endl;return;}
    string record;
    int pid = 0;
    while(getline(fin,record)){
        abstractPos(record,pid++,routes);
    }
    delx = (xmax-xmin)/MAX_X_NUM;
    dely = (ymax-ymin)/MAX_Y_NUM;
}
RouteReader::~RouteReader(){}
void RouteReader::abstractPos(string record, int pid,unordered_map<int,vector<Position>>& routes){
    int id;
    double x,y;
    sscanf(record.c_str(),"%d-%lf-%lf",&id,&x,&y);
    if(xmin>x) xmin = x;
    if(ymin>y) ymin = y;
    if(xmax<x) xmax = x;
    if(ymax<y) ymax = y;
    routes[id].push_back(Position(id,pid,x,y));
}


double RouteReader::calDistance(const Position& p1,const Position& p2){
    double lat1 = p1.y,lat2 = p2.y;
    double lng1 = p1.x,lng2 = p2.x;
    return (R*acos(cos(lat1*pi/180 )*cos(lat2*pi/180)*cos(lng1*pi/180 -lng2*pi/180)+
             sin(lat1*pi/180 )*sin(lat2*pi/180)));
}

void RouteReader::filt(vector<pair<int,int>>& candidate){
    int point_num = 0;
    int route_num = routes.size();
    for(int i = 1;i<=route_num;i++){
        clear_counter();
        point_num = routes[i].size();
        for(int j = 0;j<point_num;j++){
            if(!findNearPoint(routes[i][j],j+1))break;
        }
        for(int k = 1;k<=route_num;k++){
            if(counter[k] == point_num){
                if(k!=i){
                    candidate.push_back(pair<int,int>(i,k));
                }
            }
        }
    }
}
bool RouteReader::findNearPoint(const Position& p,int iter_time){
    int xpos = floor((p.x-xmin)/delx);
    int ypos = floor((p.y-ymin)/dely);
    if(xpos >= MAX_X_NUM) xpos = MAX_X_NUM-1;
    if(ypos >= MAX_Y_NUM) ypos = MAX_Y_NUM-1;
    int xleft = xpos-1>=0?xpos-1:0;
    int xright = xpos+1<MAX_X_NUM?xpos+1:MAX_X_NUM-1;
    int yleft = ypos-1>=0?ypos-1:0;
    int yright = ypos+1<MAX_Y_NUM?ypos+1:MAX_Y_NUM-1;
    for(int i = xleft;i<=xright;i++){
        for(int j = yleft;j<=yright;j++){
            for(int m =0;m<Map[i][j].size();m++){
                counter[Map[i][j][m]]++;
            }
        }
    }

    for(int i = 0;i<Map[xpos][ypos].size();i++){
        if(counter[Map[xpos][ypos][i]] == iter_time) return true;
    }
    return false;
}

void RouteReader::clear_counter(){
    for(int i = 0;i<ROUTE_NUM;i++){
        counter[i]=0;
    }
}

void RouteReader::verify(vector<pair<int,int>>& candidate,vector<pair<int,int>>& result){
    for(int i =0 ;i<candidate.size();i++){
        int r1 = candidate[i].first;
        int r2 = candidate[i].second;
        if(is_sim(r1,r2)){
            result.push_back(pair<int,int>(r1,r2));
        }
    }
}

void RouteReader::buildMap(){
    for(int i = 1;i<=routes.size();i++){
        for(int j = 0;j<routes[i].size();j++){
            int xpos = 0;
            int ypos = 0;
            xpos = floor((routes[i][j].x-xmin)/delx);
            ypos = floor((routes[i][j].y-xmin)/delx);
            if(xpos>=MAX_X_NUM)xpos = MAX_X_NUM-1;
            if(ypos>=MAX_Y_NUM)ypos = MAX_Y_NUM-1;
            Map[xpos][ypos].push_back(routes[i][j].route_id);
        }
    }
}

bool RouteReader::is_sim(int r1,int r2){
    bool find;
    for(int i = 0;i<routes[r1].size();i++){
        find = false;
        for(int j = 0;j<routes[r2].size();j++){
            if(calDistance(routes[r1][i],routes[r2][j])<=100){
                find = true;
                break;
            }
        }
        if(find == false) return false;
    }
    return true;
}