#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Hello, welcome to PacMan v2." << endl << \
		"WASD to move." << endl << \
		"C to change map, I to show move info, R to restart game." << endl <<\
		"Ctrl + S to save changed map. " << endl << \
		" " << endl;
	
	showInfo		= TRUE;//	FALSE; //----Сначала отключаем вывод инфы при движении
	changeMapMode	= TRUE;//	FALSE; // ---Выключен мод редактирования карты

	 /*
	//Заполням карту нулями
	saveMap.open("map.txt");
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			//записываем в файл
			saveMap  << "0";
		}
		saveMap << endl;
	}
	cout << "Map saved!" << endl;
	saveMap.close();
	*/

	//--------загрузить карту в массив field
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

	//------------PACMAN	
	pacMan.pacManImage.loadImage("..\\..\\data\\pacman.png");
	pacMan.score = 0;
	
	//-----------Подсчет количества плюшек
	scoreCount = 0;
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			if (field[i][j] == '2'){
				scoreCount++;
			}
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){



}

//--------------------------------------------------------------
void ofApp::draw(){

	//-----------Нарисовать карту каждый кадр
	//----проверяем каждый элемент карты

	sizePixel = 32;		//--- размер "пикселя"
	int scoreNumber = 0;	//--- отображаем каждую плюшку, даем ей свой id
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			//---------Бэкграунд, если номер = 0
			ofSetColor(ofColor::blueViolet);
			ofRect(i * sizePixel, j * sizePixel, sizePixel, sizePixel);

			//----Рисуем стену, если 1
			if (field[i][j] == '1'){				
				ofSetColor(ofColor::black);
				ofRect(i * sizePixel, j * sizePixel, sizePixel, sizePixel);
			}
			//---Положение плюшки
			if (field[i][j] == '2'){
				ofSetColor(ofColor::white);
				scorePoint.push_back(new ScorePoint());
				scorePoint[scoreNumber] -> position.x = i;
				scorePoint[scoreNumber] -> position.y = j;
				scoreNumber++;
				ofRect(i * sizePixel + sizePixel/4, j * sizePixel + sizePixel/4,\
						sizePixel/2, sizePixel/2);
			}
			//------получаем положение пакмена
			if (field[i][j] == '3'){
				pacMan.x = i;
				pacMan.y = j;
			}			
		}
	}
	


	//---Инфа сбоку с кол-вом очков
	ofSetColor(ofColor::black);
	ofDrawBitmapString("SCORE:", 1030, 50);
	ofDrawBitmapString(ofToString(pacMan.score), 1080, 50);




	//----Положение Пакмена
	ofSetColor(255);
	pacMan.pacManImage.draw(pacMan.x * sizePixel, pacMan.y * sizePixel, sizePixel, sizePixel);
	
	//-----------sprite use here
	//pacMan.pacManImage.drawSubsection(130,180,40,40,0,0);
	
	//------------отобразить пиксель, на который наведена мышь
	showChangeMap();


	//------WE HAVE A WINNER!!!!
	if(pacMan.score == scoreCount){
		ofSetColor(ofColor::black);
		ofRect(sizePixel * 12, sizePixel * 15,\
				sizePixel * 8, sizePixel * 2);
		ofSetColor(ofColor::white);
		ofDrawBitmapString("WE HAVE A WINNER!!!", 450, 530);
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//cout << key << endl;

	//***********************************************************************
	//добавить разворот изображения пакмена
	//сделать каждую кнопку движения - функцией для удобной модификации

	//-------Двжение пакмена по нажатию
	//---ВПРАВО
	if(key == 'd'){
		moveRight();
	}
	//---ВВЕРХ
	if(key == 'w'){
		moveUp();
	}
	//---ВЛЕВО
	if(key == 'a'){
		moveLeft();
	}
	//---ВНИЗ
	if(key == 's'){
		moveDown();
	}


	//-----Переключить - показывать ли инфу при движении
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

	//-----Переключить - изменять карту
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

	//-----Рестарт игры
	if (key == 'r'){
		setup();
	}

	if (key == 'e'){
		pacMan.pacManImage.crop(512, 0, 512, 512);
		pacMan.pacManImage.draw(
	}
	if (key == 't'){
		
		pacMan.pacManImage.crop(0, 0, 512, 512);
	}

	//---------Идентично нажатию Ctrl + S
	//-------Пересохранить карту в файл
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
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//----Если включен мод для изменения карты
if (changeMapMode){
	//----получить значения пикселя, который нужно изменить
	int mousePosX = int(floor(mouseX/sizePixel));
	int mousePosY = int(floor(mouseY/sizePixel));
	
	cout << mousePosX << " : " << mousePosY << endl;

	//---изменить пустое место на стенку и наоборот на ЛКМ
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
	
	//---изменить пустое место на плюшку и наоборот на ПКМ
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
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//---------Показывает значения вокруг пакмена на карте из переменной field
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
	int mousePosX = int(floor(mouseX/sizePixel));
	int mousePosY = int(floor(mouseY/sizePixel));


	ofSetColor(180);
	ofRect(mousePosX * sizePixel, mousePosY * sizePixel, sizePixel, sizePixel);
}
}

//---------------------Движение Пакмена вправо
void ofApp::moveRight(){
	int x = pacMan.x;
	int y = pacMan.y;
		
	//----перенос в начало, если край карты
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
		
	//-----показать значение элементов вокруг пакмена
	else ofApp::showAround(x, y);
}

//-----------Движение Пакмена вверх
void ofApp::moveUp(){
	int x = pacMan.x;
	int y = pacMan.y;

	//----перенос в начало, если край карты
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
		
	//-----показать значение элементов вокруг пакмена
	else ofApp::showAround(x, y);	
}


//-----------Движение Пакмена влево
void ofApp::moveLeft(){
	int x = pacMan.x;
	int y = pacMan.y;

	//----перенос в начало, если край карты
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
		
	//-----показать значение элементов вокруг пакмена
	else ofApp::showAround(x, y);
}


//-----------Движение Пакмена вниз
void ofApp::moveDown(){
	int x = pacMan.x;
	int y = pacMan.y;

	//----перенос в начало, если край карты
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
		
		//---Найти какую именно плюшку удалить
		/*
		for (int i = 0; i < scorePoint.size(); i++){
			int pointX = scorePoint[i] -> position.x;
			int pointY = scorePoint[i] -> position.y;

			if (pointX == x && pointY == y+1){
				cout << "EAT! "<< i << endl;
				//перемещаем элемент в конец списка
				swap(scorePoint[i], scorePoint.back());
				//удаляем его
				scorePoint.pop_back();
				pacMan.score++;
				field[x][y]='0';
				field[x][y+1]='3';
			}
		}
		*/
	}
		
	//-----показать значение элементов вокруг пакмена
	else ofApp::showAround(x, y);
}