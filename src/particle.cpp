#include "particle.h"

using namespace std;

Particle::Particle(float x, float y)
: mass(1.f)
, damp(1.f)
, position(x, y)
, initialPos(x,y) {

}

void Particle::update(float dt) {
    float t = ofGetElapsedTimef();
    float noiseX = ofSignedNoise(2 * position.x, 2 * position.y, t);
    float noiseY = ofSignedNoise(2 * position.x, 2 * position.y, t+1);
    applyForce(50 * ofVec2f(noiseX, noiseY));

    float dt2 = dt * dt;

    auto dampingAcc = -velocity * damp; 
    auto v2 = velocity + dt * (dampingAcc + acceleration);
    position += dt * velocity + 0.5f * dt2 * acceleration;
    velocity = v2;

    if (position.length() > 10000) {
        reset();
    }
}

void Particle::draw() {
    acceleration = ofVec2f(0, 0);
    ofSetColor(255, 255, 255);
    ofDrawCircle(position, 5);
}

void Particle::reset() {
    position = initialPos;
    velocity.set(0, 0);
    acceleration.set(0, 0);
}

void Particle::applyForce(ofVec2f force) {
    acceleration += force / mass;
}

void Particle::attract(const vector<ofPolyline>& shape) {
    auto shapePtr = shape.begin();
    if (!shapePtr->inside(position)) {
        ofVec2f closestPt = shapePtr->getClosestPoint(position);
        ofVec2f dir = closestPt - position;
        float dist = max(dir.length(), 20.f);
        ofVec2f force = 16 * dist * dir / dist;
        applyForce(force);
    }

    for ( ; shapePtr != shape.end(); ++shapePtr) {
        if (shapePtr->inside(position)) {
            ofVec2f closestPt = shapePtr->getClosestPoint(position);
            ofVec2f dir = closestPt - position;
            float dist = max(dir.length(), 20.f); 
            ofVec2f force = 0.1 * dist * dir / dist;
            applyForce(force);
        }
    }
}

Bond::Bond(Particle* p1, Particle* p2)
: first(p1), second(p2) {
    vector<ofPoint> pts;
    pts.push_back(first->position);
    pts.push_back(second->position);
    line = ofPolyline(pts);
}

void Bond::update(const vector<ofPolyline>& shape) {
    bool active = false;
    auto midPoint = (first->position + second->position) * 0.5f;
    if (shape.front().inside(midPoint)) {
        active = true;
    }
    else if (midPoint.squareDistance(shape.front().getClosestPoint(midPoint)) < 40.f) {
        active = true;
    }

    if (active) {
        auto dir = first->position - second->position;
        dist = first->position.squareDistance(second->position);
	dist = max(dist, 20.f);
        auto force = 10000 / dist * dir.getNormalized();
        first->applyForce(force);
        second->applyForce(-force);
    }
    else {
        dist = -1;
    }

    auto& vertices = line.getVertices();
    vertices[0].set(first->position);
    vertices[1].set(second->position);
}

void Bond::draw() {
    int alpha = max(0, min(255, (int)(300000.f / dist)));
    //if (alpha < 75) alpha /= dist;
    ofSetLineWidth(2.f);
    ofSetColor(255, 255, 255, alpha);
    line.draw();
}
