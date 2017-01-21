#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Hello, welcome to PacMan v2." << endl << \
		"WASD to move." << endl << \
		"C to change map, I to show move info, R to restart game." << endl <<\
		"Ctrl + S to save changed map. " << endl << \
		" " << endl;
	
	showInfo		= FALSE; //----Сначала отключаем вывод инфы при движении
	changeMapMode	= FALSE; // ---Выключен мод редактирования карты
	
	ghost1.scoreOnWay = FALSE;
	ghost1.direction  = 3;

	gameSpeed		=  3; //----Количество движений в секунду.
	pacMan.direction = 3; //----Изначальное движение
	pacMan.score	=  0; //---Изначальное количество очков
	pacMan.lives	=  3; //--Изначальное количество жизней

	ghost1.testLives = &gameSpeed;


	loadMapToField(); //--------загрузить карту в массив field

	//------------PACMAN	
	//---Общая картинка
	pacMan.pacManImage.loadImage("..\\..\\data\\pacmanSprite.png");
	//--кусок с открытым ртом
	pacMan.pacManImageOpen.cropFrom(pacMan.pacManImage, 0, 0, 512, 512);
	//--кусок с закрытым ртом
	pacMan.pacManImageClose.cropFrom(pacMan.pacManImage, 512, 0, 512, 512);
	changeStatusMouth();
	
	
	//-----------Подсчет количества плюшек
	scoreCount = 0;
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			if (field[i][j] == '2'){
				scoreCount++;
			}
		}
	}
	scoreInGame = scoreCount;

	//---Приведение
	ghost1.ghostImage.loadImage("..\\..\\data\\ghost1.png");

}

//--------------------------------------------------------------
void ofApp::update(){



}

//--------------------------------------------------------------
void ofApp::draw(){

	//-----------Нарисовать карту каждый кадр
	//----проверяем каждый элемент карты

	PIXEL_SIZE;	//--- размер "пикселя"
	int scoreNumber = 0;	//--- отображаем каждую плюшку, даем ей свой id
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			//---------Бэкграунд, если номер = 0
			ofSetColor(ofColor::blueViolet);
			ofRect(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);

			//----Рисуем стену, если 1
			if (field[i][j] == '1'){				
				ofSetColor(ofColor::black);
				ofRect(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
			}
			//---Положение плюшки
			if (field[i][j] == '2'){
				ofSetColor(ofColor::white);
				scorePoint.push_back(new ScorePoint());
				scorePoint[scoreNumber] -> position.x = i;
				scorePoint[scoreNumber] -> position.y = j;
				scoreNumber++;
				ofRect(i * PIXEL_SIZE + PIXEL_SIZE/4, j * PIXEL_SIZE + PIXEL_SIZE/4,\
						PIXEL_SIZE/2, PIXEL_SIZE/2);
			}
			//------получаем положение пакмена
			if (field[i][j] == '3'){
				pacMan.x = i;
				pacMan.y = j;
			}			
			//-----Положение привидения
			if (field[i][j] == '4'){
				ghost1.x = i;
				ghost1.y = j;
			}
		}
	}
	


	//---Инфа сбоку с кол-вом очков
	ofSetColor(ofColor::black);
	ofDrawBitmapString("SCORE:", 1030, 50);
	ofDrawBitmapString(ofToString(pacMan.score), 1080, 50);

	//----Отобразить максимальное количество очков, и перезаписать, если набранных больше
	showHighScore();

	ofSetColor(ofColor::black);
	ofDrawBitmapString("LIVES:", 1030, 120);
	ofDrawBitmapString(ofToString(pacMan.lives), 1080, 120);


	//----Положение Пакмена
	ofSetColor(255);
	pacMan.pacManImage.draw(pacMan.x * PIXEL_SIZE, pacMan.y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
	
	//---Положение призрака
	ofSetColor(255);
	ghost1.ghostImage.draw(ghost1.x * PIXEL_SIZE, ghost1.y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
	


	//------------отобразить пиксель, на который наведена мышь
	showChangeMap();


	//-------------------------Время и шаг движения
	//--Отчетное время для движения пакмана
	int time = ofGetElapsedTimeMillis();	

	if(time > (1000/gameSpeed)){
		//----движение пакмена
		switch (pacMan.direction){
		case 0: moveUp(); break;
		case 1: moveLeft(); break;
		case 2: moveDown(); break;
		case 3: moveRight(); break;	
		}
		//----движение призрака
		switch (ghost1.direction){
		case 0: ghostMoveUp(); break;
		case 1: ghostMoveLeft(); break;
		case 2: ghostMoveDown(); break;
		case 3: ghostMoveRight(); break;	
		}

		//---Сброс счетчика времени и новый отсчет
		ofResetElapsedTimeCounter();
	}


	//------WE HAVE A WINNER!!!!
	if(pacMan.score >= scoreCount){
		ofSetColor(ofColor::black);
		ofRect(PIXEL_SIZE * 12, PIXEL_SIZE * 15,\
				PIXEL_SIZE * 8, PIXEL_SIZE * 2);
		ofSetColor(ofColor::white);
		ofDrawBitmapString("WE HAVE A WINNER!!!", 450, 530);

		//---Переход на новый уровень
		//---увеличиваем скорость, назначаем направление пакмена по дефолту, увеличиваем количество необходимых очков
		
		gameSpeed		+= 1;
		pacMan.direction = 3;
		scoreCount		+= scoreInGame;

		//---обновить изображение пакмена
		pacMan.pacManImage.loadImage("..\\..\\data\\pacmanSprite.png");
		pacMan.pacManImageOpen.cropFrom(pacMan.pacManImage, 0, 0, 512, 512);
		pacMan.pacManImageClose.cropFrom(pacMan.pacManImage, 512, 0, 512, 512);
		changeStatusMouth();

		//---загружаем карту с начала,
		loadMapToField();
	}


	//--------------Количество жизней исчерпано
	if (pacMan.lives == 0){
		ofSetColor(ofColor::black);
		ofRect(PIXEL_SIZE * 12, PIXEL_SIZE * 15,\
				PIXEL_SIZE * 8, PIXEL_SIZE * 2);
		ofSetColor(ofColor::red);
		ofDrawBitmapString("GAME    OVER", 470, 510);
		ofDrawBitmapString("Your score: ", 450, 530);
		ofDrawBitmapString(ofToString(pacMan.score), 580, 530);
		//-----Неизвестное направление - персонажи остановятся
		pacMan.direction = 4;
		ghost1.direction = 4;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//cout << key << endl;

	//***********************************************************************
	//добавить разворот изображения пакмена
	//сделать каждую кнопку движения - функцией для удобной модификации

	//-------Смена направления пакмена по нажатию
	//--------И смена направления изображения
	//**********************************************************************************
	//*****Изменить смену изображения, чтобы менять не отделно открытый и закрытый
			// а только pacManImage

	//---ВВЕРХ
	if(key == 'w'){		
		if(pacMan.direction==1) pacMan.pacManImageOpen.rotate90(45); 
		if(pacMan.direction==2) pacMan.pacManImageOpen.rotate90(90);
		if(pacMan.direction==3) pacMan.pacManImageOpen.rotate90(-45);
		
		if(pacMan.direction==1) pacMan.pacManImageClose.rotate90(45); 
		if(pacMan.direction==2) pacMan.pacManImageClose.rotate90(90);
		if(pacMan.direction==3) pacMan.pacManImageClose.rotate90(-45);
		pacMan.direction = 0;
		
	}
	//---ВЛЕВО
	if(key == 'a'){
		if(pacMan.direction==0) pacMan.pacManImageOpen.rotate90(-45); 
		if(pacMan.direction==2) pacMan.pacManImageOpen.rotate90(45);
		if(pacMan.direction==3) pacMan.pacManImageOpen.rotate90(90);
		
		if(pacMan.direction==0) pacMan.pacManImageClose.rotate90(-45); 
		if(pacMan.direction==2) pacMan.pacManImageClose.rotate90(45);
		if(pacMan.direction==3) pacMan.pacManImageClose.rotate90(90);
		pacMan.direction = 1;
		
	}
	//---ВНИЗ
	if(key == 's'){
		if(pacMan.direction==0) pacMan.pacManImageOpen.rotate90(90); 
		if(pacMan.direction==1) pacMan.pacManImageOpen.rotate90(-45);
		if(pacMan.direction==3) pacMan.pacManImageOpen.rotate90(45);
		
		if(pacMan.direction==0) pacMan.pacManImageClose.rotate90(90); 
		if(pacMan.direction==1) pacMan.pacManImageClose.rotate90(-45);
		if(pacMan.direction==3) pacMan.pacManImageClose.rotate90(45);
		pacMan.direction = 2;
		
	}
	//---ВПРАВО
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

	//-- Изменить направление призрака
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

	//-Используется указатель чтобы увличить скорость
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
//----Если включен мод для изменения карты
if (changeMapMode){
	//----получить значения пикселя, который нужно изменить
	int mousePosX = int(floor(mouseX/PIXEL_SIZE));
	int mousePosY = int(floor(mouseY/PIXEL_SIZE));
	
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
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}


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
	int mousePosX = int(floor(mouseX/PIXEL_SIZE));
	int mousePosY = int(floor(mouseY/PIXEL_SIZE));


	ofSetColor(180);
	ofRect(mousePosX * PIXEL_SIZE, mousePosY * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
}
}

//---------------------Движение Пакмена вправо
void ofApp::moveRight(){
	//--Изменить изображение рта
	changeStatusMouth();

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
	changeStatusMouth();

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
	changeStatusMouth();

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
	changeStatusMouth();

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

//-----Изменить статус рта пакмена
void ofApp::changeStatusMouth(){
	//--если рот открыт - закрыть
	if (pacMan.statusMouth == 1){
		pacMan.statusMouth = 0;
		pacMan.pacManImage.clone(pacMan.pacManImageClose);
	}
	else {
		pacMan.statusMouth = 1;
		pacMan.pacManImage.clone(pacMan.pacManImageOpen);
	}

}

//--------загрузить карту в массив field
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

//----Загрузка из файла макс. очков. Если набрал больше - перезапись в файл
void ofApp::showHighScore(){
	//----получить макс. очки
	openMap.open("highscore.hs");
	std::string line;
	getline(openMap, line);
	highscore = atoi(line.c_str()); //---перевод string to int
	openMap.close();

	if (pacMan.score >= highscore){
		//---перезаписать, если очков больше чем макс.очков
		highscore = pacMan.score;
		saveMap.open("highscore.hs");
		saveMap << highscore;
		saveMap.close();
	}
	ofDrawBitmapString("HIGH \nSCORE:", 1030, 80);
	ofDrawBitmapString(ofToString(highscore), 1080, 93);

}

//---Очищает карту, заполняет ее нулями
//----ПОЛЬЗОВАТЬСЯ, ЕСЛИ НУЖНО ВСЕ СРОСИТЬ!!!
void ofApp::resetMapToZero(){
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
}

void ofApp::gameOver(){
	cout << "GAME OVER!!!" << endl;
	cout << "Your score: " << pacMan.score << endl;
	//---поставить персонажей на место
	field[ghost1.x][ghost1.y] = '0';
	field[14][13]		= '4'; ghost1.x = 14; ghost1.y = 13;
	ghost1.direction	= 3;
	
	field[pacMan.x][pacMan.y] = '0';
	field[14][24]		=  '3'; pacMan.x = 14; pacMan.y = 13;
	//pacMan.direction	=  3;
	pacMan.lives		-= 1; //----Минус 1 жизнь
	//loadMapToField();	

}





//-------------------------------------------------------------GHOST
void ofApp::ghostMoveRight(){
	int x = ghost1.x;
	int y = ghost1.y;
		
	//----перенос в начало, если край карты
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
		
	//----перенос в начало, если край карты
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
		
	//----перенос в начало, если край карты
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
		
	//----перенос в начало, если край карты
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
	//---позиции вокруг призрака
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

	//----проверять положение призрака на перекрестках
	if (left != 1){
		if(up != 1){
			//--угол
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
			//---Перекресток UpLeftDown
			else if(right == 1 && down != 1){
				//---Оценить где пакмен
				//--если левее пакмена или в линиюс
				if(x >= pacMan.x){
					if (y > pacMan.y)	ghost1.direction = 0;
					if (y < pacMan.y)	ghost1.direction = 2;
				}

				//---если правее - рандомно
				else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
			//---Перекресток UpLeftRight
			else if(right != 1 && down == 1){
				//--есди выше призрака или в лнию с ним
				if (y >= pacMan.y){
					if (x > pacMan.x)	ghost1.direction = 1;
					if (x < pacMan.x)	ghost1.direction = 3;
					//---если вертикално вверх
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
			//--угол
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
			//---Перекресток UpLeftDown
			else if(right == 1 && up != 1){
				//---Оценить где пакмен
				//--если левее
				if(x >= pacMan.x){
					ghost1.direction = 1;
				}
				//---если правее - рандомно
				else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
			//--перекресток LeftDownRight
			else if(up == 1 && right != 1){
				//--Если выше пакмена
				if (y < pacMan.y){
					if (x > pacMan.x)	ghost1.direction = 1;
					if (x < pacMan.x)	ghost1.direction = 3;
					//---если вертикално вниз
					else				ghost1.direction = 2;
				}
				//----Если по горизонтальной линии с пакменом
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
			//--угол
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
			//---перекресток UpDownRight
			else if(left == 1 && down != 1){
				//---Оценить где пакмен
				if (x < pacMan.x){
					//---если выше призрака
					if (y < pacMan.y)	ghost1.direction = 2;
					//--если ниже
					if (y > pacMan.y)	ghost1.direction = 0;
					//--если по горизонтальной линии
					else				ghost1.direction = 3;
				}
				//---Если по вертикальной линии
				else if (x = pacMan.x){
					if (y > pacMan.y) ghost1.direction = 0;
					if (y < pacMan.y) ghost1.direction = 2;
				}
				//---если пакмен левее
				else{
					switch(solution){
					case 0: ghost1.direction = 0; break;
					case 1: ghost1.direction = 2; break;
					}
				}
			}
		}
		else if (down != 1){
			//----угол
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
	//-------------------------------------свободны все 4 стороны
	if(up != 1 && down != 1 && left != 1 && right != 1) {
		//----оценить положение пакмена
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
	//----Если пакмен левее
	if (x >= pacMan.x){
		//----Пакмен выше
		if (y >= pacMan.y){
			cout << "left up" << endl;
			//----Стремиться в левый вехний угол
			int solution = ofRandom(2);
			//---только сверху свободно
			if (left == 1 && up != 1) ghost1.direction = 0;
			//--только слева свободно
			if (left != 1 && up == 1) ghost1.direction = 1;
			//---сверху и слева свободно - выбираем случайную сторону
			if (left != 1 && up != 1){
				switch(solution){
				case 0: ghost1.direction = 0; break;
				case 1: ghost1.direction = 1; break;
				}
			}
			//---сверху и слева занято, двигаться в другую сторону
			if (left == 1 && up == 1){
				switch(solution){
				case 0: ghost1.direction = 2; break;
				case 1: ghost1.direction = 3; break;
				}
			}
		}
		//--Пакман ниже
		else if (y < pacMan.y){
			cout << "left down" << endl;
			//---стремиться в нижний левый угол
			int solution = ofRandom(2);
			//--только снизу свободно
			if (left == 1 && down != 1) ghost1.direction = 2;
			//--только слева свободно
			if (left != 1 && down == 1) ghost1.direction = 1;
			//---снизу и слева свободно - выбираем случайную сторону
			if (left != 1 && down != 1){
				switch(solution){
				case 0: ghost1.direction = 1; break;
				case 1: ghost1.direction = 2; break;
				}
			}
			//---снизу и слева занято, двигаться в другую сторону
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