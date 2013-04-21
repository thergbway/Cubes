#pragma once
#include <QDebug>

template <typename type, const int size>
class SimpleArray{
private:
	type array[size];
	int elementsUsed;
public:
	SimpleArray(){
		elementsUsed=0;
	};
	void push_back(type t){
		if(elementsUsed >= size){
			while(true)
				qDebug()<<"Critical error: SimpleArray is overloaded. See simpleArray.h. elementsUsed="<<elementsUsed<<"maxSize="<<size;
		}
		array[elementsUsed]=t;
		++elementsUsed;
	}
	int getElementsCount(return elementsUsed);
	type& opertor [] (int index){return array[index]};
};