#include "raylib.h"
#include <array>
#include "VectorOverload.h"
using namespace std;
#define SIZE 10000

class Map{
    public:
        Map(Vector2);

        void AddPoint(Vector2);
        void AddPoint(float x,float y);
        void AddPointPolaire(Vector2);
        void AddPointPolaire(float,float);

        void Draw()const;
        void SetTexture(Vector2,Vector2);
        void reset();
        inline void SetScale(float scal){scale = scal;}
        inline float GetScale()const {return scale;}

        float angle = 0;

    protected:
        array<Vector2,SIZE> points{Vector2{0,0}};
        int index = 0;
        RenderTexture2D target;
        Vector2 offset;
        float scale=1;
};
