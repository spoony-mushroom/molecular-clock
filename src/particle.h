#pragma once
#include "ofMain.h"

class Particle {
public:
    Particle(float x = 0, float y = 0);
    void update(float deltaTime);
    void draw();
    void applyForce(ofVec2f force);
    void attract(const vector<ofPolyline>& shape);
private:
    void reset();
    float mass;
    float damp;

    friend class Bond;
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f acceleration;

    ofVec2f initialPos;
};

class Bond {
public:
    Bond(Particle* p1, Particle* p2);
    void update(const vector<ofPolyline>& shape);
    void draw();
private:
    ofPolyline line;
    float dist;
    Particle* first;
    Particle* second;
};