#include "Entity.h"

#include <vector>
#include <algorithm>
#include <iostream>

bool coPlanar(std::vector<glm::vec3>& t1, std::vector<glm::vec3>& t2,glm::vec3 normal) {
    glm::vec3 n1 , n2; 
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            n1 = glm::cross(t1[(j + 1) % 3] - t1[j], t1[(j + 2) % 3] - t1[j]);
            n2 = glm::cross(t1[(j + 1) % 3] - t1[j], t2[i] - t1[j]);
            if (glm::dot(n1, n2) < 0) break; 
        }
        return true;
    }
    return false;
}

bool primitiveCollison(float* a, float* b) {
    std::vector<glm::vec3> triangle1(3, glm::vec3());
    std::vector<glm::vec3> triangle2(3, glm::vec3());
    for (int i = 0; i < 3; i++)
    {
        triangle1[i] = glm::vec3(a[i * 4] / a[i * 4 + 3], a[i * 4 + 1] / a[i * 4 + 3], a[i * 4 + 2] / a[i * 4 + 3]);
        triangle2[i] = glm::vec3(b[i * 4] / b[i * 4 + 3], b[i * 4 + 1] / b[i * 4 + 3], b[i * 4 + 2] / b[i * 4 + 3]);
    }
    //plane equation dot(n,X) + d = 0

    //calculating 1st tiangle's plane
    glm::vec3 n1 = glm::cross(triangle1[1]-triangle1[0],triangle1[2]-triangle1[0]);
    float d1 = glm::dot(-n1,triangle1[0]);

    //calculating 2nd tiangle's plane
    glm::vec3 n2 = glm::cross(triangle2[1] - triangle2[0], triangle2[2] - triangle2[0]);
    float d2 = glm::dot(-n2 , triangle2[0]);

    if (glm::all(glm::lessThan(glm::abs(glm::abs(n1)-glm::abs(n2)), glm::vec3(DBL_EPSILON)))) return coPlanar(triangle1, triangle2, n1);
   
    //Checking if triangle1 intersects 2nd plane (by checking signed distance) 

    std::pair<float, float> dp1[3], dp2[3];
    int tmp1 = 0;
    for (int i = 0; i < 3; i++)
    {
        dp1[i].second = glm::dot(n2, triangle1[i]) + d2;
        if (dp1[i].second == 0)
        {
            return false;
        }
        else if (dp1[i].second > 0) ++tmp1;
        else --tmp1;

    }
    if (tmp1==3||tmp1==-3) return false;
    
    //Checking if triangle2 intersects 1nd plane (by checking signed distance)
    int tmp2 = 0;

    for (int i = 0; i < 3; i++)
    {
        dp2[i].second = glm::dot(n1, triangle2[i]) + d1;
        if (dp2[i].second == 0)
        {
            return false;
        }
        else if (dp2[i].second > 0) ++tmp2;
        else --tmp2;

    }
    if (tmp2 == 3 || tmp2 == -3) return false;
    //Calculate D
    glm::vec3 D = glm::cross(n1,n2);
    //Obtain p values of triangles
    for (int i = 0; i < 3; i++)
    {
        dp1[i].first = glm::dot(D, triangle1[i]);//[maxi];
        dp2[i].first = glm::dot(D, triangle2[i]);//[maxi];
    }

    std::sort(dp1, dp1 + 3, [](const std::pair<float, float>& x,
        const std::pair<float,float>& y)
        {
            return x.second< y.second;
        });
    std::sort(dp2, dp2 + 3, [](const std::pair<float, float>& x,
        const std::pair<float, float>& y)
        {
            return x.second < y.second;
        });
    //Create intervals
    float t1[2],t2[2];
    if (glm::sign(dp1[2].second) == glm::sign(dp1[1].second)) {
        t1[0] = dp1[1].first + (dp1[0].first - dp1[1].first) * (dp1[1].second / (dp1[1].second - dp1[0].second));
        t1[1] = dp1[2].first + (dp1[0].first - dp1[2].first) * (dp1[2].second / (dp1[2].second - dp1[0].second));
    }
    else {
        t1[0] = dp1[1].first + (dp1[2].first - dp1[1].first) * (dp1[1].second / (dp1[1].second - dp1[2].second));
        t1[1] = dp1[0].first + (dp1[2].first - dp1[0].first) * (dp1[0].second / (dp1[0].second - dp1[2].second));
    }
    if (glm::sign(dp2[2].second) == glm::sign(dp2[1].second)) {
        t2[0] = dp2[1].first + (dp2[0].first - dp2[1].first) * (dp2[1].second / (dp2[1].second - dp2[0].second));
        t2[1] = dp2[2].first + (dp2[0].first - dp2[2].first) * (dp2[2].second / (dp2[2].second - dp2[0].second));
    }
    else {
        t2[0] = dp2[1].first + (dp2[2].first - dp2[1].first) * (dp2[1].second / (dp2[1].second - dp2[2].second));
        t2[1] = dp2[0].first + (dp2[2].first - dp2[0].first) * (dp2[0].second / (dp2[0].second - dp2[2].second));
    }
    
    std::sort(t1, t1 + 2);
    std::sort(t2, t2 + 2);

    //Find if intervals intersect

    if ((t2[0] <= t1[0] && t1[0] <= t2[1]) || (t1[0] <= t2[0] && t2[0] <= t1[1]))
    {
        //printf("t1 %f ,%f, %f \n", dp1[0].second, dp1[1].second, dp1[2].second);
        //printf("t2 %f ,%f, %f \n", dp2[0].second, dp2[1].second, dp2[2].second);
        //printf("int1 %f, %f\n", t1[0], t1[1]);
        //printf("int2 %f, %f\n", t2[0], t2[1]);
        //printf("tmp1 = %d, tmp2 = %d \n", tmp1,tmp2);
        return true;
    }
    return false;
}

std::pair<int,int> Entity::isColliding(Entity* e)
{
    //if (!this->isInCollisonRadius(e)) {
    //    return std::pair<int,int>(-1,-1);
    //}
    int i, j;
    for (i = 0; i < this->collisonGeometry->verticesNo/3; i++)
    {
        for (j = 0; j < e->collisonGeometry->verticesNo/3; j++)
        {
            if (primitiveCollison(this->collisionGemoetryCalculated + i * 12, e->collisionGemoetryCalculated + j * 12))
                return std::pair<int, int>(i, j);
        }
    }
    return std::pair<int, int>(-1, -1);
}

std::vector<int> Entity::everyFaceCollsion(Entity* e) {
    std::vector<int> out;
    for (int i = 0; i < this->collisonGeometry->verticesNo / 3; i++)
    {
        for (int j = 0; j < e->collisonGeometry->verticesNo / 3; j++)
        {
            if (primitiveCollison(this->collisionGemoetryCalculated + i * 12, e->collisionGemoetryCalculated + j * 12))
            {
                out.push_back(i);
                out.push_back(j);
            }
        }
    }
    return out;
}


void Entity::moveModel()
{
    int vertNo = this->collisonGeometry->verticesNo;
    glm::vec4 tmp;
    auto vertSource = this->collisonGeometry->coords;
    const auto out = this->collisionGemoetryCalculated;
    for (int i = 0; i < vertNo; i++)
    {
        tmp = glm::vec4(vertSource[i * 4], vertSource[i * 4 + 1], vertSource[i * 4 + 2], vertSource[i * 4 + 3]);
        tmp = this->M * tmp;
        out[i * 4] = tmp.x;
        out[i * 4 + 1] = tmp.y;
        out[i * 4 + 2] = tmp.z;
        out[i * 4 + 3] = tmp.w;
    }
}


bool Entity::isInCollisonRadius(Entity* e)
{
    if (glm::distance(this->center, e->center) <= this->collisonRadius + e->collisonRadius) {
        return true;
    }
    return false;
}

Entity::Entity()
{
    this->center = glm::vec4(0,0,0,1);
    this->rotation = glm::quat(1,0,0,0);
    this->calculateM();
}

void Entity::draw()
{
    this->objectGeometry->draw(sp,getM(),tex);
}

void Entity::drawBoundingBox() {
    this->collisonGeometry->draw(sp , getM(),tex);
}


Entity::Entity(ShaderProgram* _sp,GLuint* _tex, Mesh* _obj, glm::vec3 _center, glm::quat _rot) : Entity(_sp,_tex,_obj, _obj, _center, _rot)
{
}

Entity::Entity(ShaderProgram* _sp, GLuint* _tex, Mesh* _objGeo, Mesh* _objColl,glm::vec3 _center ,glm::quat _rot ): tex(_tex), sp(_sp),center(glm::vec4(_center,1)),rotation(_rot)
{
    this->collisonGeometry = _objColl;
    this->objectGeometry = _objGeo;
    glm::vec4 zero(0, 0, 0, 1);
    glm::vec4 tmp;
    auto coords = this->collisonGeometry->coords;
    float maxD=0;
    for (int i = 0; i < this->collisonGeometry->verticesNo; i++)
    {
        tmp = glm::vec4(coords[i * 4], coords[i * 4 + 1], coords[i * 4 + 2], coords[i * 4 + 3]);
        maxD = glm::max(maxD, glm::distance(zero, tmp));
    }
    collisonRadius = maxD;
    collisionGemoetryCalculated = new float[this->collisonGeometry->verticesNo*4];
    calculateM();
    moveModel();
}

Entity::~Entity()
{
    delete[] collisionGemoetryCalculated;
}

void Entity::calculateM()
{
    this->M = glm::translate(glm::mat4(1.0f), glm::vec3(center));
    this->M = this->M * glm::mat4(rotation);
    this->M = glm::scale(this->M, Scale);
}

glm::mat4 Entity::getM() {
    return M;
}
float* Entity::getCollisonModel() {
    return collisionGemoetryCalculated;
}
void Entity::react(AbstractEngine& ab) {
    ab.react(this);
}