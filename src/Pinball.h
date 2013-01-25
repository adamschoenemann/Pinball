#ifndef PINBALL_H_INCLUDED
#define PINBALL_H_INCLUDED

class FysCircle : public virtual ICircle, public Dynamic, public virtual ICollideable {
	public:

	real radius;
	ofColor color;

	Vector2 getCenter(){
		return getPosition();
	}

	real getRadius(){
		return radius;
	}

	FysCircle(real rad):radius(rad){
		color = ofColor(255, 0, 0);
	}

	FysCircle(const Vector2& pos, const Vector2& vel, real rad):radius(rad){
		setPosition(pos);
		setVelocity(vel);
		setMass(1.0f);
		setDamping(1.0f);
		color = ofColor(255, 0, 0);
	}

	virtual Fysik::Rectangle getAABB(){
		Vector2 pos = getCenter();
		return Fysik::Rectangle(radius, radius, pos, Fysik::Rectangle::EXTENDS);
	}




	void draw(){
		Vector2 pos = getPosition();
		ofPushStyle();
		ofPushMatrix();

		ofTranslate(pos.x, pos.y);
		ofSetColor(color);
		ofCircle(0, 0, radius);

		ofPopStyle();
		ofPopMatrix();
	}

};

class StaticCircle : public FysCircle {
	public:

	StaticCircle(Vector2 pos, real rad):FysCircle(rad){
		setPosition(pos);
		setInverseMass(0);
		setDamping(1.0f);
	}

};

class Pinball : public FysCircle {
	public:

	Pinball(Vector2 pos, real rad):FysCircle(rad){
		setPosition(pos);
		setInverseMass(1.0f);
		setDamping(0.9f);
	}

	int checkBounds(int w, int h){
		Vector2 pos = getPosition();
		real x = pos.x;
		real y = pos.y;
		if(x - radius < 0){
			setPosition(Vector2(radius, getPosition().y));
			setVelocity(getVelocity().componentProduct(Vector2(-1, 1)));
		}
		if(x + radius > w){
			setPosition(Vector2(w - radius, getPosition().y));
			setVelocity(getVelocity().componentProduct(Vector2(-1, 1)));
		}
		if(y - radius < 0){
			setPosition(Vector2(getPosition().x, radius));
			setVelocity(getVelocity().componentProduct(Vector2(1, -1)));
		}
		if(y + radius > h){
			setPosition(Vector2(getPosition().x, h - radius));
			setVelocity(getVelocity().componentProduct(Vector2(1, -1)));
			return 1;
		}
		return 0;
	}

};


#endif // PINBALL_H_INCLUDED
