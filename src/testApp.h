#pragma once

#include "ofMain.h"
#include <fysik.hpp>
using namespace Fysik;
#include "Pinball.h"
#include <sstream>


class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void endGame();
		void startGame();

		ofTrueTypeFont font;
		ofImage bgImg;

		typedef list<FysCircle*> Circles;
		Circles circles;

		typedef list<Contact> Contacts;
		Contacts contacts;

		Pinball* pball;

		static const int cooldown = 3000;
		static const int gameDuration = 20000;
		int lastTimeUsed;
		int points;
		int gameEndsAt;
		int tRemaining;
		bool gameOver;

};
