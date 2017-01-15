#pragma once
#include "ofMain.h"

#define PIXEL_SIZE 32


class Pacman{
public:
	Pacman(){
	
	};

	ofImage pacManImage;
	
	//----Открытый и закрытый рот пакмена
	ofImage pacManImageOpen, pacManImageClose;
	bool statusMouth;

	//--положение
	float x, y;
	//--Направление пакмена WASD(0123)
	short int direction;

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

		void changeStatusMouth();


		int field[64][64];
		int scoreCount;

		int sizePixel;

		int gameSpeed; //---Количество ходов в секунду. С повышением уровня можно увеличиаь скорость

		bool showInfo;
		bool changeMapMode;



		ofstream saveMap;
		ifstream openMap;

		Pacman pacMan;
		vector<ScorePoint*> scorePoint;

};
