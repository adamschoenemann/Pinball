#ifndef DISPLAY_OBJECT_HPP
#define DISPLAY_OBJECT_HPP

#include "ofMain.h"
#include <vector>


typedef double Scalar;

class ADisplayObject {
	protected:
	Scalar _x, _y, _width, _height;
	ofColor _color;

	public:
	virtual void draw() = 0;

	// Accessors
	Scalar x(){return _x;}
	void x(Scalar x) {_x = x;}

	Scalar y(){return _y;}
	void y(Scalar y) {_y = y;}

	Scalar width() {return _width;}
	void width(Scalar val) {_width = val;}

	Scalar height() {return _height;}
	void height(Scalar val) {_height = val;}

	ofColor color() {return _color;}
	void color(ofColor val) {_color = val;}

	// Constructors
	ADisplayObject(){}
	ADisplayObject(Scalar x, Scalar y, Scalar w, Scalar h, ofColor col){
		this->x(x);
		this->y(y);
		this->width(w);
		this->height(h);
		this->color(col);
	}
};

class DisplayRect : public ADisplayObject{
	public:

	DisplayRect(){}
	DisplayRect(Scalar x, Scalar y, Scalar w, Scalar h, ofColor color)
		:ADisplayObject(x, y, w, h, color){

	}
	virtual void draw(){
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(x(), y());

		ofSetColor(color());
		ofRect(0, 0, width(), height());

		ofPopMatrix();
		ofPopStyle();
	}

};

class DisplayCircle : public ADisplayObject {
	public:
	Scalar radius;
	DisplayCircle(Scalar x, Scalar y, Scalar rad, ofColor color)
		: ADisplayObject(x, y, rad*2, rad*2, color), radius(rad){

	}

	virtual void draw(){
		ofPushStyle();
		ofSetColor(color());
		ofCircle(x(), y(), radius);
		ofPopStyle();
//		cout << x() << "\t" << y() << endl;
	}
};

class Layer {
	private:

	typedef vector<ADisplayObject*> ObjectList;
	ObjectList objects;

	public:

	void add(ADisplayObject* obj){
		objects.push_back(obj);
	}
	void draw(){
		for(ObjectList::iterator i = objects.begin(); i != objects.end(); i++){
			(*i)->draw();
		}
	}
};

namespace drawfuncs {

	class IDrawFunc {

		virtual void operator() = 0;
	}

	class DrawRect : IDrawFunc {
		public:
		Scar x, y, w, h;
		DrawRect(Scalar x, Scalar y, Scalar w, Scalar h): x(x), y(y), w(w), h(h) {}

		virtual void operator(){
			ofRect(x, y, w, h);
		}
	}

}

class Graphics {

	private:

	typedef vector<IDrawFunc*> FuncQueue;
	FuncQueue queue;

	void add(IDrawFunc* func){
		queue.push_back(func);
	}

	public:

	void rect(Scalar x, Scalar y, Scalar w, Scalar h){
		IDrawFunc* func = new DrawRect(x, y, w, h);
		add(func);
	}

};

class ADynamicDisplayObject : ADisplayObject {
	private:

	public:

};


#endif
