#pragma once

#include "ofMain.h"

class Pacman{
public:
	Pacman(){
	
	};

	ofImage pacManImage;
	//--положение
	float x, y;

	int score;


};

class ScorePoint{

public:
	ScorePoint(){
	
	};
	
	ofVec2f position;


};

class ofApp : public ofBaseApp{
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

		//-----my functions 
		void showAround(int x, int y);
		void showChangeMap();
		
		void moveRight();
		void moveUp();
		void moveLeft();
		void moveDown();

		int field[64][64];
		int scoreCount;

		int sizePixel;

		bool showInfo;
		bool changeMapMode;

		ofstream saveMap;
		ifstream openMap;

		Pacman pacMan;
		vector<ScorePoint*> scorePoint;

};
