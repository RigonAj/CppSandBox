#pragma once
#include "map.h"
#include "serveur.h"
#include <raylib.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

struct Point {
    float distance=0;
    float pange=0;
    float angle=0;
};
class Robot:public Map{

    public:
        Robot(Vector2,const char*,uint16_t);
        ~Robot();
        void OdometryWorker();
        void Draw();
        void Update();
        bool EventUpdate();

    protected:
        Vector2 pose_;
        float yaw_;

        Serveur LidarServ;
        Serveur ImuServ;

        std::mutex stateMutex;
        std::atomic<bool> Run{true};
        std::thread workerThread;
        Point *point;

        vector<Vector2> Point1, Point2, Point3;
        vector<Vector2> *Pointwrite,*Pointtmp , *PointRead;
};

Robot::Robot(Vector2 screen,const char* ip, uint16_t port):Map(screen){
    LidarServ.init(ip,port);
    ImuServ.init(ip, (port + 1) );
    workerThread = std::thread(&Robot::OdometryWorker, this);
    point = (Point*)LidarServ.GetData();
    Point1.reserve(1000); Point2.reserve(1000); Point3.reserve(1000);
    Pointwrite = &Point1;
    Pointtmp = &Point2;
    PointRead = &Point3;


}

Robot::~Robot() {
    Run = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void Robot::Draw(){
    Map::Draw();
};

void Robot::Update(){
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        if (!Pointtmp->empty()) std::swap(PointRead, Pointtmp);
    }
    if (!PointRead->empty()) {
        for(const auto &p : *PointRead) {
            AddPointPolaire(p);
        }
        PointRead->clear();}
    Map::Update();
};

bool Robot::EventUpdate(){
    return Map::EventUpdate();
}

void Robot::OdometryWorker() {
    int n = 0;
    while (Run) {
        if( (n = LidarServ.receive()) > 0){
            for(int i = 0 ; i < n / sizeof(Point) ; i++){
                Pointwrite->emplace_back(Vector2{point[i].distance,point[i].angle});
            }
            {
                std::lock_guard<std::mutex> lock(stateMutex);
                std::swap(Pointwrite, Pointtmp);
            }
            Pointwrite->clear();
        }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
