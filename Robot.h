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
        void reset();
        inline const Vector2 GetLastPoint(){return PointRead->back();}
        inline const double TimeElapsedLidar(){return elapsed_lidar.count();}
        inline const double TimeElapsedImu(){return elapsed_imu.count();}
        void UpdatePoseSurMap();
        float yaw_ = 0;
        float yaw_ref = 0;//Réferentiel

    protected:
        Vector2 pose_{0,0};

        Texture2D Image_robot = LoadTexture("Robot.png");
        Rectangle sourceRec = { 0.0f, 0.0f, (float)Image_robot.width, (float)Image_robot.height };
        Vector2 offset = Map::screen /2;
        Rectangle dest = { pose_.x + offset.x, pose_.y + offset.y, 100.0f, (float)Image_robot.height  * 100 / (float)Image_robot.width };
        Vector2 origin = Vector2{ (float)Image_robot.width/ 2.0f, (float)Image_robot.height/ 2.0f };
        Vector2 positionSurMap;
        Vector2 pivot;

        Serveur LidarServ;
        Serveur ImuServ;

        float scale = 4.0f;

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

        chrono::time_point<std::chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>>
        last_lidar = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_lidar ;

        chrono::time_point<std::chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>>
        last_imu= std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_imu ;

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
    workerThreadLidar.detach();
    workerThreadImu   = std::thread(&Robot::ImuRecup, this);
    workerThreadImu.detach();
}

Robot::~Robot() {
    Run = false;
    UnloadTexture(Image_robot);
}
void Robot::Draw() {
    Map::Draw();  // dessine la texture

    BeginMode2D(camera);


    DrawTexturePro(Image_robot, sourceRec, dest, pivot, - yaw_ , WHITE);

    EndMode2D();
}



void Robot::UpdatePoseSurMap(){

    Vector2 mapOffset = screen * scale / 2.0f;
    positionSurMap = (pose_ / scale) + mapOffset;

    float largeurRobot = 100.0f;
    float hauteurRobot = (float)Image_robot.height * largeurRobot / (float)Image_robot.width;

    dest = {
        positionSurMap.x,
        positionSurMap.y,
        largeurRobot,
        hauteurRobot
    };

    pivot = { dest.width / 2.0f, dest.height / 2.0f };
}

void Robot::Update(){

    {
        std::lock_guard<std::mutex> lock(ImuMux);
        if (!ImuMid->empty()) std::swap(ImuRead, ImuMid);

    }
    if(!ImuRead->empty()) {

        elapsed_imu = std::chrono::high_resolution_clock::now() - last_imu;
        last_imu = std::chrono::high_resolution_clock::now();
        yaw_ = ImuRead->back().yaw - yaw_ref;
        ImuRead->clear();

    }

    {
        std::lock_guard<std::mutex> lock(LidarMux);
        if (!Pointtmp->empty()) std::swap(PointRead, Pointtmp);
    }
    if (!PointRead->empty()) {
        Vector2 localPose;
        elapsed_lidar = std::chrono::high_resolution_clock::now() - last_lidar;
        last_lidar = std::chrono::high_resolution_clock::now();
        {
            std::lock_guard<std::mutex> lock(PoseMux);
            localPose = pose_;
        }
        for(auto &p : *PointRead) AddPoint( (RotationPolaireDeg(p, - yaw_ - 90) + localPose) / scale);
        cout << "Point read\n";
        PointRead->clear();}
    UpdatePoseSurMap();
    Map::Update();

};

bool Robot::EventUpdate(){
    return Map::EventUpdate();
}

void Robot::reset(){
    Map::reset();
    yaw_ref = yaw_;
}

void Robot::LidarRecup() {
    int n = 0;
    while (Run.load() && (n = LidarServ.receive()) > 0) {
        cout << "Point receive\n";
        for(int i = 0 ; i < n / sizeof(Point) ; i++){
            Pointwrite->emplace_back(Vector2{point[i].distance,point[i].angle});
        }
        {
            std::lock_guard<std::mutex> lock(LidarMux);
            std::swap(Pointwrite, Pointtmp);
        }
        Pointwrite->clear();

    }
}

void Robot::ImuRecup() {
    int n = 0;
    while (Run.load() && (n = ImuServ.receive()) > 0) {
            {
                std::lock_guard<std::mutex> lock(ImuMux);
                ImuWrite->emplace_back(*imu) ;
                std::swap(ImuWrite,ImuMid);
            }
            float delta_gauche = (float)imu->pasgauche - ancien_pasgauche;
            float delta_droite = (float)imu->pasdroite - ancien_pasdroite;
            ancien_pasgauche   = (float)imu->pasgauche;
            ancien_pasdroite   = (float)imu->pasdroite;
            float norm = ( delta_droite + delta_gauche  ) * coef.load() /2;
            Vector2 dir = {1,0};
            {
            std::lock_guard<std::mutex> lock(PoseMux);
            pose_ += Rotation(dir, imu -> yaw *  DEG2RAD) * norm;
            }

    }
}
