#pragma once
#include "particle.h"

using namespace std;

class Clock {
    public:
        Clock();

        void setup();
        void draw();
        void update();
        void set24h(bool use24);
    
    private:
        void updateParticles();
        void initParticles();
        void updateTime();

        ofTrueTypeFont font;
        
        bool use24Format = true;
        
        ofVec2f position;
        float textSize;
        int currentMinutes = -1;

        vector< ofPath > letterPaths;
        vector< vector<Particle> > particleGroups;
        vector< vector<Bond> > bonds;

        const int groupSize = 25;
};
