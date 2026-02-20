#pragma once
#include "VectorOverload.h"
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

struct ImuData {
    float yaw;
    int pasgauche;
    int pasdroite;
};

class Robot:public Map{

    public:
        Robot(Vector2,const char*,uint16_t);
        ~Robot();
        void LidarRecup();
        void ImuRecup();
        void Draw();
        void Update();
        bool EventUpdate();

    protected:
        Vector2 pose_{0,0};
        float yaw_ = 0;

        Serveur LidarServ;
        Serveur ImuServ;

        std::mutex LidarMux;
        std::mutex ImuMux;
        std::mutex PoseMux;
        std::atomic<bool> Run{true};
        std::atomic<float> coef{1.0f};

        std::thread workerThreadLidar;
        std::thread workerThreadImu;

        Point *point;
        vector<Vector2> Point1, Point2, Point3;
        vector<Vector2> *Pointwrite,*Pointtmp , *PointRead;

        ImuData *imu;
        vector<ImuData> Imu1, Imu2, Imu3;
        vector<ImuData> *ImuWrite,*ImuMid , *ImuRead;
        float ancien_pasgauche = 0;
        float ancien_pasdroite = 0;
};

// Dans le constructeur, initialiser les pointeurs IMU
Robot::Robot(Vector2 screen, const char* ip, uint16_t port) : Map(screen) {
    LidarServ.init(ip, port);
    ImuServ.init(ip, (port + 1));

    point = (Point*)LidarServ.GetData();
    imu   = (ImuData*)ImuServ.GetData();


    Point1.reserve(1000); Point2.reserve(1000); Point3.reserve(1000);
    Pointwrite = &Point1;
    Pointtmp   = &Point2;
    PointRead  = &Point3;

    Imu1.reserve(1000); Imu2.reserve(1000); Imu3.reserve(1000);
    ImuWrite = &Imu1;
    ImuMid   = &Imu2;
    ImuRead  = &Imu3;


    workerThreadLidar = std::thread(&Robot::LidarRecup, this);
    workerThreadImu   = std::thread(&Robot::ImuRecup, this);
}

Robot::~Robot() {
    Run = false;
    if (workerThreadLidar.joinable()) {
        workerThreadLidar.join();
    }
    if (workerThreadImu.joinable()) {
        workerThreadImu.join();
    }
}

void Robot::Draw(){
    Map::Draw();
};

void Robot::Update(){
    {
        std::lock_guard<std::mutex> lock(ImuMux);
        if (!ImuMid->empty()) std::swap(ImuRead, ImuMid);

    }
    if(!ImuRead->empty()) {
        yaw_ = ImuRead->back().yaw;
        ImuRead->clear();

    }

    {
        std::lock_guard<std::mutex> lock(LidarMux);
        if (!Pointtmp->empty()) std::swap(PointRead, Pointtmp);
    }
    if (!PointRead->empty()) {
        Vector2 localPose;
        {
            std::lock_guard<std::mutex> lock(PoseMux);
            localPose = pose_;
        }
        for(auto &p : *PointRead) AddPoint( RotationPolaire(p, yaw_) + localPose);

        PointRead->clear();}

    Map::Update();
};

bool Robot::EventUpdate(){
    return Map::EventUpdate();
}

void Robot::LidarRecup() {
    int n = 0;
    while (Run.load()) {
        if( (n = LidarServ.receive()) > 0){
            for(int i = 0 ; i < n / sizeof(Point) ; i++){
                Pointwrite->emplace_back(Vector2{point[i].distance,point[i].angle});
            }
            {
                std::lock_guard<std::mutex> lock(LidarMux);
                std::swap(Pointwrite, Pointtmp);
            }
            Pointwrite->clear();
        }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Robot::ImuRecup() {
    int n = 0;
    while (Run.load()) {
        if( (n = ImuServ.receive()) > 0){
            {
                std::lock_guard<std::mutex> lock(ImuMux);
                ImuWrite->emplace_back(*imu) ;
                std::swap(ImuWrite,ImuMid);
            }
            float delta_gauche = (float)imu->pasgauche - ancien_pasgauche;
            float delta_droite = (float)imu->pasdroite - ancien_pasdroite;
            ancien_pasgauche = (float)imu->pasgauche;
            ancien_pasdroite = (float)imu->pasdroite;
            float norm = ( delta_droite + delta_gauche  ) * coef.load() /2;
            Vector2 dir = {1,0};
            {
            std::lock_guard<std::mutex> lock(PoseMux);
            pose_ += Rotation(dir, imu -> yaw *  DEG2RAD) * norm;
            }
        }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
