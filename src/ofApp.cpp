#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Hello, welcome to PacMan v2." << endl << \
		"WASD to move." << endl << \
		"C to change map, I to show move info, R to restart game." << endl <<\
		"Ctrl + S to save changed map. " << endl << \
		" " << endl;
	
	showInfo		= FALSE; //----������� ��������� ����� ���� ��� ��������
	changeMapMode	= FALSE; // ---�������� ��� �������������� �����
	
	ghost1.scoreOnWay = FALSE;
	ghost1.direction  = 3;

	gameSpeed		=  3; //----���������� �������� � �������.
	pacMan.direction = 3; //----����������� ��������
	pacMan.score	=  0; //---����������� ���������� �����
	pacMan.lives	=  3; //--����������� ���������� ������

	ghost1.testLives = &gameSpeed;


	loadMapToField(); //--------��������� ����� � ������ field

	//------------PACMAN	
	//---����� ��������
	pacMan.pacManImage.loadImage("..\\..\\data\\pacmanSprite.png");
	//--����� � �������� ����
	pacMan.pacManImageOpen.cropFrom(pacMan.pacManImage, 0, 0, 512, 512);
	//--����� � �������� ����
	pacMan.pacManImageClose.cropFrom(pacMan.pacManImage, 512, 0, 512, 512);
	changeStatusMouth();
	
	
	//-----------������� ���������� ������
	scoreCount = 0;
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			if (field[i][j] == '2'){
				scoreCount++;
			}
		}
	}
	scoreInGame = scoreCount;

	//---����������
	ghost1.ghostImage.loadImage("..\\..\\data\\ghost1.png");

}

//--------------------------------------------------------------
void ofApp::update(){



}

//--------------------------------------------------------------
void ofApp::draw(){

	//-----------���������� ����� ������ ����
	//----��������� ������ ������� �����

	PIXEL_SIZE;	//--- ������ "�������"
	int scoreNumber = 0;	//--- ���������� ������ ������, ���� �� ���� id
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			//---------���������, ���� ����� = 0
			ofSetColor(ofColor::blueViolet);
			ofRect(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);

			//----������ �����, ���� 1
			if (field[i][j] == '1'){				
				ofSetColor(ofColor::black);
				ofRect(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
			}
			//---��������� ������
			if (field[i][j] == '2'){
				ofSetColor(ofColor::white);
				scorePoint.push_back(new ScorePoint());
				scorePoint[scoreNumber] -> position.x = i;
				scorePoint[scoreNumber] -> position.y = j;
				scoreNumber++;
				ofRect(i * PIXEL_SIZE + PIXEL_SIZE/4, j * PIXEL_SIZE + PIXEL_SIZE/4,\
						PIXEL_SIZE/2, PIXEL_SIZE/2);
			}
			//------�������� ��������� �������
			if (field[i][j] == '3'){
				pacMan.x = i;
				pacMan.y = j;
			}			
			//-----��������� ����������
			if (field[i][j] == '4'){
				ghost1.x = i;
				ghost1.y = j;
			}
		}
	}
	


	//---���� ����� � ���-��� �����
	ofSetColor(ofColor::black);
	ofDrawBitmapString("SCORE:", 1030, 50);
	ofDrawBitmapString(ofToString(pacMan.score), 1080, 50);

	//----���������� ������������ ���������� �����, � ������������, ���� ��������� ������
	showHighScore();

	ofSetColor(ofColor::black);
	ofDrawBitmapString("LIVES:", 1030, 120);
	ofDrawBitmapString(ofToString(pacMan.lives), 1080, 120);


	//----��������� �������
	ofSetColor(255);
	pacMan.pacManImage.draw(pacMan.x * PIXEL_SIZE, pacMan.y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
	
	//---��������� ��������
	ofSetColor(255);
	ghost1.ghostImage.draw(ghost1.x * PIXEL_SIZE, ghost1.y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
	


	//------------���������� �������, �� ������� �������� ����
	showChangeMap();


	//-------------------------����� � ��� ��������
	//--�������� ����� ��� �������� �������
	int time = ofGetElapsedTimeMillis();	

	if(time > (1000/gameSpeed)){
		//----�������� �������
		switch (pacMan.direction){
		case 0: moveUp(); break;
		case 1: moveLeft(); break;
		case 2: moveDown(); break;
		case 3: moveRight(); break;	
		}
		//----�������� ��������
		switch (ghost1.direction){
		case 0: ghostMoveUp(); break;
		case 1: ghostMoveLeft(); break;
		case 2: ghostMoveDown(); break;
		case 3: ghostMoveRight(); break;	
		}

		//---����� �������� ������� � ����� ������
		ofResetElapsedTimeCounter();
	}


	//------WE HAVE A WINNER!!!!
	if(pacMan.score >= scoreCount){
		ofSetColor(ofColor::black);
		ofRect(PIXEL_SIZE * 12, PIXEL_SIZE * 15,\
				PIXEL_SIZE * 8, PIXEL_SIZE * 2);
		ofSetColor(ofColor::white);
		ofDrawBitmapString("WE HAVE A WINNER!!!", 450, 530);

		//---������� �� ����� �������
		//---����������� ��������, ��������� ����������� ������� �� �������, ����������� ���������� ����������� �����
		
		gameSpeed		+= 1;
		pacMan.direction = 3;
		scoreCount		+= scoreInGame;

		//---�������� ����������� �������
		pacMan.pacManImage.loadImage("..\\..\\data\\pacmanSprite.png");
		pacMan.pacManImageOpen.cropFrom(pacMan.pacManImage, 0, 0, 512, 512);
		pacMan.pacManImageClose.cropFrom(pacMan.pacManImage, 512, 0, 512, 512);
		changeStatusMouth();

		//---��������� ����� � ������,
		loadMapToField();
	}


	//--------------���������� ������ ���������
	if (pacMan.lives == 0){
		ofSetColor(ofColor::black);
		ofRect(PIXEL_SIZE * 12, PIXEL_SIZE * 15,\
				PIXEL_SIZE * 8, PIXEL_SIZE * 2);
		ofSetColor(ofColor::red);
		ofDrawBitmapString("GAME    OVER", 470, 510);
		ofDrawBitmapString("Your score: ", 450, 530);
		ofDrawBitmapString(ofToString(pacMan.score), 580, 530);
		//-----����������� ����������� - ��������� �����������
		pacMan.direction = 4;
		ghost1.direction = 4;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//cout << key << endl;

	//***********************************************************************
	//�������� �������� ����������� �������
	//������� ������ ������ �������� - �������� ��� ������� �����������

	//-------����� ����������� ������� �� �������
	//--------� ����� ����������� �����������
	//**********************************************************************************
	//*****�������� ����� �����������, ����� ������ �� ������� �������� � ��������
			// � ������ pacManImage

	//---�����
	if(key == 'w'){		
		if(pacMan.direction==1) pacMan.pacManImageOpen.rotate90(45); 
		if(pacMan.direction==2) pacMan.pacManImageOpen.rotate90(90);
		if(pacMan.direction==3) pacMan.pacManImageOpen.rotate90(-45);
		
		if(pacMan.direction==1) pacMan.pacManImageClose.rotate90(45); 
		if(pacMan.direction==2) pacMan.pacManImageClose.rotate90(90);
		if(pacMan.direction==3) pacMan.pacManImageClose.rotate90(-45);
		pacMan.direction = 0;
		
	}
	//---�����
	if(key == 'a'){
		if(pacMan.direction==0) pacMan.pacManImageOpen.rotate90(-45); 
		if(pacMan.direction==2) pacMan.pacManImageOpen.rotate90(45);
		if(pacMan.direction==3) pacMan.pacManImageOpen.rotate90(90);
		
		if(pacMan.direction==0) pacMan.pacManImageClose.rotate90(-45); 
		if(pacMan.direction==2) pacMan.pacManImageClose.rotate90(45);
		if(pacMan.direction==3) pacMan.pacManImageClose.rotate90(90);
		pacMan.direction = 1;
		
	}
	//---����
	if(key == 's'){
		if(pacMan.direction==0) pacMan.pacManImageOpen.rotate90(90); 
		if(pacMan.direction==1) pacMan.pacManImageOpen.rotate90(-45);
		if(pacMan.direction==3) pacMan.pacManImageOpen.rotate90(45);
		
		if(pacMan.direction==0) pacMan.pacManImageClose.rotate90(90); 
		if(pacMan.direction==1) pacMan.pacManImageClose.rotate90(-45);
		if(pacMan.direction==3) pacMan.pacManImageClose.rotate90(45);
		pacMan.direction = 2;
		
	}
	//---������
	if(key == 'd'){
		if(pacMan.direction==0) pacMan.pacManImageOpen.rotate90(45); 
		if(pacMan.direction==1) pacMan.pacManImageOpen.rotate90(90);
		if(pacMan.direction==2) pacMan.pacManImageOpen.rotate90(-45);
		
		if(pacMan.direction==0) pacMan.pacManImageClose.rotate90(45); 
		if(pacMan.direction==1) pacMan.pacManImageClose.rotate90(90);
		if(pacMan.direction==2) pacMan.pacManImageClose.rotate90(-45);
		pacMan.direction = 3;
		//moveRight();
	}

	//-----����������� - ���������� �� ���� ��� ��������
	if (key == 'i'){
		if (showInfo == TRUE){
			showInfo = FALSE;
			cout << "OFF info" << endl;
		}
		else {
			showInfo = TRUE;
			cout << "ON info" << endl;
		}
	}

	//-----����������� - �������� �����
	if (key == 'c'){
		if (changeMapMode == TRUE){
			changeMapMode = FALSE;
			cout << "OFF change Map mode" << endl;
		}
		else {
			changeMapMode = TRUE;
			cout << "ON change Map Mode" << endl;
		}
	}

	//-----������� ����
	if (key == 'r'){
		setup();
	}


	//---------��������� ������� Ctrl + S
	//-------������������� ����� � ����
	if (key == 19){

		saveMap.open("map.txt");

		for (int i = 0; i < 32; i++){
			for (int j = 0; j < 32; j++){
				saveMap << field[i][j] - 48;
			}
			saveMap << endl;
		}
		saveMap.close();
		cout << "Map Saved!" << endl;
	}

	//-- �������� ����������� ��������
	if (key=='i'){
		ghost1.direction = 0;
	}
	if (key=='j'){
		ghost1.direction = 1;
	}
	if (key=='k'){
		ghost1.direction = 2;
	}
	if (key=='l'){
		ghost1.direction = 3;
	}

	//-������������ ��������� ����� �������� ��������
	if (key == 'e'){
		*ghost1.testLives += 2;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//----���� ������� ��� ��� ��������� �����
if (changeMapMode){
	//----�������� �������� �������, ������� ����� ��������
	int mousePosX = int(floor(mouseX/PIXEL_SIZE));
	int mousePosY = int(floor(mouseY/PIXEL_SIZE));
	
	cout << mousePosX << " : " << mousePosY << endl;

	//---�������� ������ ����� �� ������ � �������� �� ���
	if (button == 0){
		if (field[mousePosX][mousePosY] == '0'){
			field[mousePosX][mousePosY] = '1';
			cout << "0 to 1" << endl;
		}
		else if (field[mousePosX][mousePosY] == '1'){
			field[mousePosX][mousePosY] = '0';
			cout << "1 to 0" << endl;
		} 
	}
	
	//---�������� ������ ����� �� ������ � �������� �� ���
	if (button == 2){
		if (field[mousePosX][mousePosY] == '0'){
			field[mousePosX][mousePosY] = '2';
			cout << "0 to 2" << endl;
		}
		else if (field[mousePosX][mousePosY] == '2'){
			field[mousePosX][mousePosY] = '0';
			cout << "2 to 0" << endl;
		} 
	}
}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}


//---------���������� �������� ������ ������� �� ����� �� ���������� field
void ofApp::showAround(int x, int y){
if (showInfo){	
	cout << x << ":" << y << endl;
	cout << "----" << field[x][y-1]-48 << "----" << endl;
	cout << "-" << field[x-1][y]-48 << "--" <<\
		field[x][y]-48 << "--" << field[x+1][y]-48 << "-" << endl;
	cout << "----" << field[x][y+1]-48 << "----" << endl;
	cout << "***********" << endl;
}
}

//--------
void ofApp::showChangeMap(){
if (changeMapMode){
	int mousePosX = int(floor(mouseX/PIXEL_SIZE));
	int mousePosY = int(floor(mouseY/PIXEL_SIZE));


	ofSetColor(180);
	ofRect(mousePosX * PIXEL_SIZE, mousePosY * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
}
}

//---------------------�������� ������� ������
void ofApp::moveRight(){
	//--�������� ����������� ���
	changeStatusMouth();

	int x = pacMan.x;
	int y = pacMan.y;
		
	//----������� � ������, ���� ���� �����
	if ((x+1 == 32)){
		field[x][y]='0';
		field[0][y]='3';				
	}
	else if (field[x+1][y] == '0'){
		field[x][y]='0';
		field[x+1][y]='3';
		ofApp::showAround(x+1, y);			
	}
	else if (field[x+1][y] == '2'){
		pacMan.score++;
		field[x][y]='0';
		field[x+1][y]='3';
		ofApp::showAround(x+1, y);
	}
		
	//-----�������� �������� ��������� ������ �������
	else ofApp::showAround(x, y);
}

//-----------�������� ������� �����
void ofApp::moveUp(){
	changeStatusMouth();

	int x = pacMan.x;
	int y = pacMan.y;

	//----������� � ������, ���� ���� �����
	if ((y == 0)){
		field[x][y]='0';
		field[x][31]='3';				
	}
	else if (field[x][y-1] == '0'){
		field[x][y]='0';
		field[x][y-1]='3';
		ofApp::showAround(x, y-1);
	}
	else if (field[x][y-1] == '2'){
		pacMan.score++;
		field[x][y]='0';
		field[x][y-1]='3';
		ofApp::showAround(x, y-1);
	}
		
	//-----�������� �������� ��������� ������ �������
	else ofApp::showAround(x, y);	
}


//-----------�������� ������� �����
void ofApp::moveLeft(){
	changeStatusMouth();

	int x = pacMan.x;
	int y = pacMan.y;

	//----������� � ������, ���� ���� �����
	if ((x == 0)){
		field[x][y]='0';
		field[31][y]='3';
	}
	else if (field[x-1][y] == '0'){
		field[x][y]='0';
		field[x-1][y]='3';
		ofApp::showAround(x-1, y);
	}
	else if (field[x-1][y] == '2'){
		pacMan.score++;
		field[x][y]='0';
		field[x-1][y]='3';
		ofApp::showAround(x-1, y);
	}
		
	//-----�������� �������� ��������� ������ �������
	else ofApp::showAround(x, y);
}


//-----------�������� ������� ����
void ofApp::moveDown(){
	changeStatusMouth();

	int x = pacMan.x;
	int y = pacMan.y;

	//----������� � ������, ���� ���� �����
	if ((y+1 == 32)){
		field[x][y]='0';
		field[x][0]='3';				
	}
	else if (field[x][y+1] == '0'){
		field[x][y]='0';
		field[x][y+1]='3';
		ofApp::showAround(x, y+1);
	}
	else if (field[x][y+1] == '2'){
		pacMan.score++;
		field[x][y]='0';
		field[x][y+1]='3';
		ofApp::showAround(x, y+1);
		
		//---����� ����� ������ ������ �������
		/*
		for (int i = 0; i < scorePoint.size(); i++){
			int pointX = scorePoint[i] -> position.x;
			int pointY = scorePoint[i] -> position.y;

			if (pointX == x && pointY == y+1){
				cout << "EAT! "<< i << endl;
				//���������� ������� � ����� ������
				swap(scorePoint[i], scorePoint.back());
				//������� ���
				scorePoint.pop_back();
				pacMan.score++;
				field[x][y]='0';
				field[x][y+1]='3';
			}
		}
		*/
	}
		
	//-----�������� �������� ��������� ������ �������
	else ofApp::showAround(x, y);
}

//-----�������� ������ ��� �������
void ofApp::changeStatusMouth(){
	//--���� ��� ������ - �������
	if (pacMan.statusMouth == 1){
		pacMan.statusMouth = 0;
		pacMan.pacManImage.clone(pacMan.pacManImageClose);
	}
	else {
		pacMan.statusMouth = 1;
		pacMan.pacManImage.clone(pacMan.pacManImageOpen);
	}

}

//--------��������� ����� � ������ field
void ofApp::loadMapToField(){
	openMap.open("map.txt");
	for (int i = 0; i < 64; i++){
		std::string line;
			getline(openMap, line);	
			for (int j = 0; j < 64; j++){
				field[i][j] = line[j];
			//cout << line[j];
		}
			//cout << endl;
	}
	openMap.close();

}

//----�������� �� ����� ����. �����. ���� ������ ������ - ���������� � ����
void ofApp::showHighScore(){
	//----�������� ����. ����
	openMap.open("highscore.hs");
	std::string line;
	getline(openMap, line);
	highscore = atoi(line.c_str()); //---������� string to int
	openMap.close();

	if (pacMan.score >= highscore){
		//---������������, ���� ����� ������ ��� ����.�����
		highscore = pacMan.score;
		saveMap.open("highscore.hs");
		saveMap << highscore;
		saveMap.close();
	}
	ofDrawBitmapString("HIGH \nSCORE:", 1030, 80);
	ofDrawBitmapString(ofToString(highscore), 1080, 93);

}

//---������� �����, ��������� �� ������
//----������������, ���� ����� ��� �������!!!
void ofApp::resetMapToZero(){
	//�������� ����� ������
	saveMap.open("map.txt");
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			//���������� � ����
			saveMap  << "0";
		}
		saveMap << endl;
	}
	cout << "Map saved!" << endl;
	saveMap.close();
}

void ofApp::gameOver(){
	cout << "GAME OVER!!!" << endl;
	cout << "Your score: " << pacMan.score << endl;
	//---��������� ���������� �� �����
	field[ghost1.x][ghost1.y] = '0';
	field[14][13]		= '4'; ghost1.x = 14; ghost1.y = 13;
	ghost1.direction	= 3;
	
	field[pacMan.x][pacMan.y] = '0';
	field[14][24]		=  '3'; pacMan.x = 14; pacMan.y = 13;
	//pacMan.direction	=  3;
	pacMan.lives		-= 1; //----����� 1 �����
	//loadMapToField();	

}





//-------------------------------------------------------------GHOST
void ofApp::ghostMoveRight(){
	int x = ghost1.x;
	int y = ghost1.y;
		
	//----������� � ������, ���� ���� �����
	if ((x+1 == 32)){
		field[x][y]='0';
		field[0][y]='4';				
	}
	else if (field[x+1][y] == '0'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x+1][y]='4';
		}else{
			field[x][y]='0';
			field[x+1][y]='4';
		}		

		ghost1.scoreOnWay = FALSE;
		ghostChangeDirection(x+1, y);
	}
	else if (field[x+1][y] == '2'){
		
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x+1][y]='4';
		}else{
			field[x][y]='0';
			field[x+1][y]='4';
		}

		ghost1.scoreOnWay = TRUE;
		ghostChangeDirection(x+1, y);
	}
	else if (field[x+1][y] == '3'){
		gameOver();
	}
	else{
		ghostChangeDirection(x, y);
	}

}

void ofApp::ghostMoveUp(){
	int x = ghost1.x;
	int y = ghost1.y;
		
	//----������� � ������, ���� ���� �����
	if (x == 0){
		field[x][y]='0';
		field[x][31]='4';				
	}
	else if (field[x][y-1] == '0'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x][y-1]='4';
		}else{
			field[x][y]='0';
			field[x][y-1]='4';
		}

		ghost1.scoreOnWay = FALSE;
		ghostChangeDirection(x, y-1);
	}
	else if (field[x][y-1] == '2'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x][y-1]='4';
		}else{
			field[x][y]='0';
			field[x][y-1]='4';
		}

		ghost1.scoreOnWay =TRUE;
		ghostChangeDirection(x, y-1);
	}
	else if (field[x][y-1] == '3'){
		gameOver();
	}else{
		ghostChangeDirection(x, y);
	}

}

void ofApp::ghostMoveLeft(){
	int x = ghost1.x;
	int y = ghost1.y;
		
	//----������� � ������, ���� ���� �����
	if (x == 0){
		field[x][y]='0';
		field[31][y]='4';				
	}
	else if (field[x-1][y] == '0'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x-1][y]='4';
		}else{
			field[x][y]='0';
			field[x-1][y]='4';	
		}

		ghost1.scoreOnWay = FALSE;
		ghostChangeDirection(x-1, y);
	}
	else if (field[x-1][y] == '2'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x-1][y]='4';
		}else{
			field[x][y]='0';
			field[x-1][y]='4';	
		}

		ghost1.scoreOnWay = TRUE;
		ghostChangeDirection(x-1, y);
	}
	else if (field[x-1][y] == '3'){
		gameOver();
	}
	else{
		ghostChangeDirection(x, y);
	}

}

void ofApp::ghostMoveDown(){
	int x = ghost1.x;
	int y = ghost1.y;
		
	//----������� � ������, ���� ���� �����
	if ((x+1 == 32)){
		field[x][y]='0';
		field[x][0]='4';				
	}
	else if (field[x][y+1] == '0'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x][y+1]='4';		
		}else{
			field[x][y]='0';
			field[x][y+1]='4';		
		}

		ghost1.scoreOnWay = FALSE;
		ghostChangeDirection(x, y+1);
	}
	else if (field[x][y+1] == '2'){
		if (ghost1.scoreOnWay){
			field[x][y]='2';
			field[x][y+1]='4';
		}else{
			field[x][y]='0';
			field[x][y+1]='4';		
		}

		ghost1.scoreOnWay = TRUE;
		ghostChangeDirection(x, y+1);
	}
	else if (field[x][y+1] == '3'){
		gameOver();
	}else{	
		ghostChangeDirection(x, y);
	}
}

void ofApp::ghostChangeDirection(int x, int y){
	//---������� ������ ��������
	int up = field[x][y-1] - 48;
	int left = field[x-1][y] - 48;
	int down = field[x][y+1] - 48;
	int right = field[x+1][y] - 48;

	int solution = ofRandom(2);
	//cout << solution << endl;
	/*cout << ghost1.direction << endl;
	
	cout << x << ":" << y << endl;
	cout << "----" << up << "----" << endl;
	cout << "-" << left << "--" <<\
		field[x][y]-48 << "--" << right << "-" << endl;
	cout << "----" <<down << "----" << endl;
	cout << "***********" << endl;
	*/

	//----��������� ��������� �������� �� ������������
	if (left != 1){
		if(up != 1){
			//--����
			if (right == 1 && down == 1){
				if (x > pacMan.x){
					if (y > pacMan.y)	ghost1.direction = 0;
					if (y <= pacMan.y)	ghost1.direction = 1;
				}
				else if(y < pacMan.y){
					ghost1.direction = 1;
				}else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 1; break;
					}
				}
			}
			//---����������� UpLeftDown
			else if(right == 1 && down != 1){
				//---������� ��� ������
				//--���� ����� ������� ��� � ������
				if(x >= pacMan.x){
					if (y > pacMan.y)	ghost1.direction = 0;
					if (y < pacMan.y)	ghost1.direction = 2;
				}

				//---���� ������ - ��������
				else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
			//---����������� UpLeftRight
			else if(right != 1 && down == 1){
				//--���� ���� �������� ��� � ���� � ���
				if (y >= pacMan.y){
					if (x > pacMan.x)	ghost1.direction = 1;
					if (x < pacMan.x)	ghost1.direction = 3;
					//---���� ���������� �����
					else				ghost1.direction = 0;
				}
				
				else{
					switch(solution){
					case 0: ghost1.direction = 1; break;
					case 1: ghost1.direction = 3; break;
					}
				}
			}
		}
		else if(down != 1){
			//--����
			if (right == 1 && up == 1){
				if (x > pacMan.x && y > pacMan.y){
					ghost1.direction = 1;
				}
				else if(y < pacMan.y){
					ghost1.direction = 2;	
				}else{
				switch(solution){
					case 0: ghost1.direction = 1; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
			//---����������� UpLeftDown
			else if(right == 1 && up != 1){
				//---������� ��� ������
				//--���� �����
				if(x >= pacMan.x){
					ghost1.direction = 1;
				}
				//---���� ������ - ��������
				else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
			//--����������� LeftDownRight
			else if(up == 1 && right != 1){
				//--���� ���� �������
				if (y < pacMan.y){
					if (x > pacMan.x)	ghost1.direction = 1;
					if (x < pacMan.x)	ghost1.direction = 3;
					//---���� ���������� ����
					else				ghost1.direction = 2;
				}
				//----���� �� �������������� ����� � ��������
				else if (y = pacMan.y){
					if (x > pacMan.x)	ghost1.direction = 1;
					if (x < pacMan.x)	ghost1.direction = 3;
				}
				else{
					switch(solution){
					case 0: ghost1.direction = 1; break;
					case 1: ghost1.direction = 3; break;
					}

				}
			}
		}
	}
	//--------------------------------------------------
	else if(right != 1){
		if (up != 1){
			//--����
			if (down == 1 && left == 1){
				if(y > pacMan.y){
					ghost1.direction = 0;
				}
				else if(x < pacMan.x){
					ghost1.direction = 3;
				}else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 3; break;
					}
				}
			}
			//---����������� UpDownRight
			else if(left == 1 && down != 1){
				//---������� ��� ������
				if (x < pacMan.x){
					//---���� ���� ��������
					if (y < pacMan.y)	ghost1.direction = 2;
					//--���� ����
					if (y > pacMan.y)	ghost1.direction = 0;
					//--���� �� �������������� �����
					else				ghost1.direction = 3;
				}
				//---���� �� ������������ �����
				else if (x = pacMan.x){
					if (y > pacMan.y) ghost1.direction = 0;
					if (y < pacMan.y) ghost1.direction = 2;
				}
				//---���� ������ �����
				else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
		}
		else if (down != 1){
			//----����
			if (up == 1 && left == 1){
				if (x < pacMan.x){
					ghost1.direction = 3;
				}
				else if(y < pacMan.y){
					ghost1.direction = 2;
				}else{
					switch(solution){
					case 0: ghost1.direction = 2; break;
					case 1: ghost1.direction = 3; break;
					}
				}
			}
		}
	}
	//-------------------------------------�������� ��� 4 �������
	if(up != 1 && down != 1 && left != 1 && right != 1) {
		//----������� ��������� �������
		if (x > pacMan.x){
			if (y > pacMan.y) {
				switch(solution){
				case 0: ghost1.direction = 0; break;
				case 1: ghost1.direction = 1; break;
				}
			}
			else{
				switch(solution){
				case 0: ghost1.direction = 1; break;
				case 1: ghost1.direction = 2; break;
				}
			}
		}
		else{
			if (y > pacMan.y) {
				switch(solution){
				case 0: ghost1.direction = 0; break;
				case 1: ghost1.direction = 3; break;
				}
			}
			else{
				switch(solution){
				case 0: ghost1.direction = 2; break;
				case 1: ghost1.direction = 3; break;
				}
			}
		}
	}






	/*
	//----���� ������ �����
	if (x >= pacMan.x){
		//----������ ����
		if (y >= pacMan.y){
			cout << "left up" << endl;
			//----���������� � ����� ������ ����
			int solution = ofRandom(2);
			//---������ ������ ��������
			if (left == 1 && up != 1) ghost1.direction = 0;
			//--������ ����� ��������
			if (left != 1 && up == 1) ghost1.direction = 1;
			//---������ � ����� �������� - �������� ��������� �������
			if (left != 1 && up != 1){
				switch(solution){
				case 0: ghost1.direction = 0; break;
				case 1: ghost1.direction = 1; break;
				}
			}
			//---������ � ����� ������, ��������� � ������ �������
			if (left == 1 && up == 1){
				switch(solution){
				case 0: ghost1.direction = 2; break;
				case 1: ghost1.direction = 3; break;
				}
			}
		}
		//--������ ����
		else if (y < pacMan.y){
			cout << "left down" << endl;
			//---���������� � ������ ����� ����
			int solution = ofRandom(2);
			//--������ ����� ��������
			if (left == 1 && down != 1) ghost1.direction = 2;
			//--������ ����� ��������
			if (left != 1 && down == 1) ghost1.direction = 1;
			//---����� � ����� �������� - �������� ��������� �������
			if (left != 1 && down != 1){
				switch(solution){
				case 0: ghost1.direction = 1; break;
				case 1: ghost1.direction = 2; break;
				}
			}
			//---����� � ����� ������, ��������� � ������ �������
			if (left == 1 && down == 1){
				switch(solution){
				case 0: ghost1.direction = 0; break;
				case 1: ghost1.direction = 3; break;
				}
			}
		}
	}*/

}


void Ghost::moves(int test[64][64]){
	test[1][1] = '1';

}