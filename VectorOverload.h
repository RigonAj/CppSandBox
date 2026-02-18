 #ifndef VECTOROVERLOAD_H
 #define  VECTOROVERLOAD_H
 #include "raylib.h"
#include <cmath>
#include <vector>
#include <array>

//------ADDITION--------
inline Vector2 operator+(const Vector2 &v1 , const Vector2 &v2)
{return Vector2{v1.x + v2.x, v1.y + v2.y} ; }

inline Vector2 operator+(const Vector2 &v1 , float a)
{return Vector2{v1.x + a, v1.y + a} ; }

inline Vector2 operator+(float a, const Vector2 &v1)
{return Vector2{v1.x + a, v1.y + a} ; }

inline Vector2 operator+=(Vector2 &v1, const Vector2 &v2){
    v1 = v1 + v2;
    return v1; }

inline Vector2 operator+=(Vector2 &v1, const float a){
    v1 = v1 + a;
    return v1; }

//------SOUSTRACTION--------
inline Vector2 operator-(const Vector2 &v1 , const Vector2 &v2)
{return Vector2{v1.x - v2.x, v1.y - v2.y} ; }

inline Vector2 operator-(const float a , const Vector2 &v1)
{return Vector2{a - v1.x, a - v1.y} ; }

inline Vector2 operator-(const Vector2 &v1 ,const float a)
{return Vector2{v1.x - a, v1.y - a} ; }

inline Vector2 operator-=(Vector2 &v1, const Vector2 &v2){
    v1 = v1 - v2;
    return v1; }

inline Vector2 operator-=(Vector2 &v1, const float a){
    v1 = v1 - a;
    return v1; }

//------MULTIPLICATION-------
inline Vector2 operator*(const Vector2 &v1 , const Vector2 &v2)
{return Vector2{v1.x * v2.x, v1.y * v2.y} ; }

inline Vector2 operator*(const Vector2 &v1 ,const float coef)
{return Vector2{v1.x * coef, v1.y * coef} ; }

inline Vector2 operator*( const float coef, const Vector2 &v1)
{return Vector2{v1.x * coef, v1.y * coef} ; }

inline Vector2 operator*=(Vector2 &v1, const Vector2 &v2){
    v1 = v1 * v2;
    return v1; }

inline Vector2 operator*=(Vector2 &v1, const float coef){
    v1 = v1 * coef;
    return v1; }

//------DIVISION-------
inline Vector2 operator/(const Vector2 &v1 , const Vector2 &v2)
{return Vector2{v1.x / v2.x, v1.y / v2.y} ; }

inline Vector2 operator/(const Vector2 &v1 ,const float coef)
{return Vector2{v1.x / coef, v1.y / coef} ; }

inline Vector2 operator/( const float coef, const Vector2 &v1)
{return Vector2{coef / v1.x ,  coef / v1.y } ; }

inline Vector2 operator/=(Vector2 &v1, const Vector2 &v2){
    v1 = v1 / v2;
    return v1; }

inline Vector2 operator/=(Vector2 &v1, const float coef){
    v1 = v1 / coef;
    return v1; }

inline Vector2 operator-(const Vector2& v) { return {-v.x, -v.y}; }

//------COMPARAISON-------

inline bool operator==(const Vector2 &v1, const Vector2 &v2){
    const float precision = 0.0001f;
    return (v1.x-v2.x) < precision  && (v1.y-v2.y) < precision ; }

inline bool operator!=(const Vector2 &v1, const Vector2 &v2){
    const float precision = 0.0001f;
    return (v1.x-v2.x) > precision  || (v1.y-v2.y) > precision ; }


//-------FONCTION UTILE-------


inline float Dot(const Vector2 &v1 ,const Vector2 &v2 ) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline float Cross(const Vector2 &v1 ,const Vector2 &v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

inline float Norm(const Vector2 &v) {
    return sqrtf(Dot(v,v));
}

inline Vector2 Normalize(const Vector2 &v) {
    float norm = Norm(v);
    if (norm > 0) return v / norm;
    return { 0, 0 };
}

inline float Distance(const Vector2 &p1, const Vector2 &p2) {
    return Norm(p1 - p2);
}

inline float Angle(const Vector2 &v1, const Vector2 &v2){
    return atan2f(Cross(v1, v2), Dot(v1, v2));
}

/* Reference est le centre de rotation, par défault {0,0} */
inline Vector2& Rotation(Vector2 &v1, const float angle,const Vector2 &Reference = {0,0}){
    const float c= cosf(angle);
    const float s= sinf(angle);

    float dx = v1.x - Reference.x ;
    float dy = v1.y - Reference.y;

    v1.x = (dx * c - dy * s) + Reference.x;
    v1.y = (dx * s + dy * c) + Reference.y;
    return v1;
}

/* Reference est le centre de rotation, par défault {0,0} */
inline void RotatePoints(std::vector<Vector2> &points, const float angle,const Vector2 &Reference = {0,0}){
    const float c= cosf(angle);
    const float s= sinf(angle);

    for(auto &point : points){

        float dx = point.x - Reference.x ;
        float dy = point.y - Reference.y;

        point.x = dx * c - dy * s + Reference.x;
        point.y = dx * s + dy * c + Reference.y;


    }
}

template <size_t N>// permet de laisser compilateur en fonction du code la valeur de N
inline void RotatePoints(std::array<Vector2,N> &points, const float angle,const Vector2 &Reference = {0,0}){
    const float c= cosf(angle);
    const float s= sinf(angle);

    for(auto &point : points){

        float dx = point.x - Reference.x ;
        float dy = point.y - Reference.y;

        point.x = dx * c - dy * s + Reference.x;
        point.y = dx * s + dy * c + Reference.y;
    }
}


#endif
