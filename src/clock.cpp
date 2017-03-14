#include <ctime>
#include "ofMain.h"
#include "clock.h"

using namespace std;

Clock::Clock()
: textSize(225), letterPaths(5) {
}

void Clock::setup() {
    font.load("Courier New Bold.ttf", textSize, false, false, true);
    position = ofVec2f(70, (ofGetHeight() + textSize) / 2.f);
    // get the string as paths
    updateTime();

    initParticles();
}

void Clock::draw() {

    ofPushMatrix();
    ofTranslate(position);

    for (auto& group : particleGroups) {
        for (Particle& p : group) {
            p.draw();
        }
    }

    for (int i = 0; i < letterPaths.size(); i++) {
        for (auto& bond : bonds[i]) {
            bond.draw();
        }
    }

    ofPopMatrix();
}

void Clock::initParticles() {
    for (int i = 0; i < letterPaths.size(); i++) {
        particleGroups.push_back(vector<Particle>());
        bonds.push_back(vector<Bond>());
        ofRectangle box = letterPaths[i].getOutline().front().getBoundingBox();
        int numParticles = i == 2 ? 6 : groupSize;
        for (int j = 0; j < numParticles; j++) {
            float x = ofRandom(box.getLeft(), box.getRight());
            float y = ofRandom(box.getTop(), box.getBottom());
            particleGroups[i].push_back(Particle(x, y));
        }
        
        // Particles repel eash other
        for (auto ptr1 = particleGroups[i].begin(); ptr1 != particleGroups[i].end(); ++ptr1) {
            for (auto ptr2 = ptr1 + 1; ptr2 != particleGroups[i].end(); ++ptr2) {
                bonds[i].push_back(Bond(&*ptr1, &*ptr2));
            }
        }
    }
}

void Clock::update() {
    updateTime();
    updateParticles();
}

void Clock::updateTime() {
    int minutes = ofGetMinutes();
    int hours = ofGetHours();

    if (minutes != currentMinutes) {
        char timeChars[6];
        sprintf(timeChars, "%02d.%02d", hours, minutes);
        letterPaths = font.getStringAsPoints(timeChars);

        currentMinutes = minutes;
    }
}

void Clock::updateParticles() {
    float dt = 1.f / 30;
    for (int i = 0; i < letterPaths.size(); i++) {
        // Calculate forces on on particles
        auto lines = letterPaths[i].getOutline();
	lines[0].simplify(5.f);

        for (Bond& b: bonds[i]) {
            b.update(lines);
        }

        for (Particle& p : particleGroups[i]) {
            // boundaries attract the particles
            p.attract(lines);
            p.update(dt);
        }
    }
}



void Clock::set24h(bool use24) {
    use24Format = use24;
}
