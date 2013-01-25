#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetBackgroundColor(255);
	lastTimeUsed = 0;
	gameEndsAt = gameDuration;
	gameOver = 0;
	font.loadFont("MONACO.TTF", 32);
	bgImg.loadImage("george.jpg");

	startGame();

	ofSetCircleResolution(100);
	ofEnableAlphaBlending();
	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void testApp::update(){
	if(gameOver) return;
	if(ofGetFrameNum() > 5){
		tRemaining = gameEndsAt - ofGetElapsedTimeMillis();
		if(tRemaining < 0){
			gameOver = 1;
			return;
		}

		real dur = ofGetLastFrameTime();
		bool hitGround = pball->checkBounds(ofGetWidth(), ofGetHeight());
		if(hitGround){
			gameOver = 1;
			return;
		}
		// Collision detection
		for(Circles::iterator j = circles.begin(); j != circles.end(); j++){
			if(pball == *j) continue;
			CollisionData data = Intersect::circleCircle(pball, *j); // Circle collision detection
			if(data.isCollision){
				Contact contact;
				contact.penetration = data.interpenetration;
				contact.normal = data.normal;
				contact.restitution = (pball->getRestitution() + (*j)->getRestitution()) / 2;
				contact.add(pball);
				contact.add(*j);
				contacts.push_back(contact);

				// add points
				real mag = pball->getVelocity().magnitude();
				if(mag > 150){
					int k =  mag / 150;
					points += ((*j)->color.getHue() > 0) ? int((*j)->getRestitution() * 10 * k) : 0;
				}
			}
		}
		// resolve contacts
		for(Contacts::iterator i = contacts.begin(); i != contacts.end(); i++){
			(*i).resolve(dur);
		}
		pball->setAcceleration(0, 300.0f); // gravity
		pball->integrate(dur);
		if(contacts.empty() == false) contacts.clear();

	}
}

//--------------------------------------------------------------
void testApp::draw(){
	bgImg.draw(0, 0);
	for(Circles::iterator i = circles.begin(); i != circles.end(); i++){
		(*i)->draw();
	}

	// draw HUD
	{
		ofPushStyle();
		ofSetColor(0, 0, 0, 125);
		ofRect(0, 0, ofGetWidth(), 30);
		ofPopStyle();
	}

	// Draw cooldown
	{

		float width = 100;
		ofPushStyle();
		ofSetColor(0, 255, 255);
		int CDWait = ofGetElapsedTimeMillis() - lastTimeUsed;
		if(CDWait < cooldown){
			width = 100 * CDWait/cooldown;

		}
		ofRect(5, 5, width, 20);
		ofPopStyle();

		ofPushStyle();
		ofNoFill();
		ofSetColor(0);
		ofRect(5, 5, 100, 20);
		ofPopStyle();
	}
	// Draw points
	{
		ofPushStyle();
		ofSetColor(255);
		stringstream ss;
		ss << "Points: " << points;

		ofDrawBitmapString(ss.str(), ofGetWidth() - 100, 20);

		ofPopStyle();
	}

	// Draw time
	{
		stringstream ss;
		ss << "Time remaining: " << tRemaining / 1000;

		ofDrawBitmapString(ss.str(), ofGetWidth() / 2, 20);
	}

	// end game
	if(gameOver) endGame();

}

//--------------------------------------------------------------
void testApp::startGame(){

	points = 0;
	int rad = 30;
	pball = new Pinball(Vector2(ofGetWidth() / 2, rad), rad);

	pball->setVelocity(-20, 20);
	pball->color = ofColor(0, 155, 0);
	pball->setRestitution(1.0f);
	pball->setDamping(0.9);

	circles.push_back(pball);


	int totalRadius = 500;
	int maxRad = 50;
	real totalRest = 14;
	real maxRest = 1.5f;
	while(totalRadius > 0){
		int newRad = ofRandom(10, maxRad);
		totalRadius -= newRad;

		// Place new circle somewhere it doesn't collide with already existing circles
		Vector2 newPos;
		bool collided;
		do {
			collided = false;

			newPos = Vector2(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight() - 100));
			Circle check;
			check.radius = newRad;
			check.center = newPos;
			for(Circles::iterator i = circles.begin(); i != circles.end(); i++){
				CollisionData data = Intersect::circleCircle(&check, *i);
				if(data.isCollision){
					collided = true;
					break;
				}
			}

		} while(collided == true);

		StaticCircle* c = new StaticCircle(newPos, newRad);
		real rest = (totalRest > 0) ? ofRandom(0.2f, maxRest) : 0.2f;
		totalRest -= rest;
		c->setRestitution(rest);
		int hue = (rest > maxRest/2) ? 125 : 0;
		c->color = (ofColor().fromHsb(hue, 255, rest*(255/maxRest)));
		circles.push_back(c);
	}
}

//--------------------------------------------------------------
void testApp::endGame(){
	pball->setInverseMass(0);
	ofPushStyle();
	ofSetColor(255, 0, 0);

	stringstream ss;
	ss << "GAME OVER!\nPOINTS: " << points;
	ofRectangle box = font.getStringBoundingBox(ss.str(), 0, 0);
	font.drawString(ss.str(), box.width / 2, ofGetHeight() / 2);

	ofPopStyle();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(gameOver){
		gameOver = 0;
		gameEndsAt = ofGetElapsedTimeMillis() + gameDuration;
		circles.clear();
		lastTimeUsed = ofGetElapsedTimeMillis();
		startGame();
		return;
	}
	if(ofGetElapsedTimeMillis() - cooldown > lastTimeUsed){
		real str = 500.0f;
		Vector2 vec;
		if(key == 'a'){
			vec.x = -1;
		}
		if(key == 'w'){
			vec.y = -1;
		}
		if(key == 'd'){
			vec.x = 1;
		}
		if(key == 's'){
			vec.y = 1;
		}

		pball->setVelocity(pball->getVelocity() + vec * str);
		lastTimeUsed = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
