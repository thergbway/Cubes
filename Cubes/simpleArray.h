#pragma once
#include <QDebug>

template<class T, int size>
class SimpleArray;

template <class T, int size>
class SimpleArray{
private:
	T array[size];
	int elementsUsed;
public:
	SimpleArray(){
		elementsUsed=0;
	};
	T& operator[](int index) {
		if(index >= size){
			qDebug()<<"SimpleArray. Requested"<<index<<"of"<<size-1;
		}
		return array[index];
	};
	void push_back(T t){
		if(elementsUsed >= size){
			while(true)
				qDebug()<<"Critical error: SimpleArray is overloaded. See simpleArray.h. elementsUsed="<<elementsUsed<<"maxSize="<<size;
		}
		array[elementsUsed]=t;
		++elementsUsed;
	};
	int getElementsCount(){return elementsUsed;};
	void goToStart(){elementsUsed=0;};
};