 #ifndef VECTOROVERLOAD_H
 #define  VECTOROVERLOAD_H
 #include "raylib.h"
#include <cmath>

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
    return v1.x == v2.x && v1.y == v2.y; }

inline bool operator!=(const Vector2 &v1, const Vector2 &v2){
    return v1.x != v2.x || v1.y != v2.y; }


//-------FONCTION UTILE-------
inline float Norm(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

inline Vector2 Normalize(Vector2 v) {
    float norm = Norm(v);
    if (norm > 0) return v / norm;
    return { 0, 0 };
}

#endif
