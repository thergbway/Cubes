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
	void pushUpNormal(){//добавить 3d вектор-нормаль вверх (три значения)
		push_back(0);
		push_back(1);
		push_back(0);
	};	
	void pushDownNormal(){//добавить 3d вектор-нормаль вниз (три значения)
		push_back(0);
		push_back(-1);
		push_back(0);
	};
	void pushBackNormal(){//добавить 3d вектор-нормаль назад (три значения)
		push_back(0);
		push_back(0);
		push_back(-1);
	};
	void pushFrontNormal(){//добавить 3d вектор-нормаль вперед (три значения)
		push_back(0);
		push_back(0);
		push_back(1);
	};
	void pushLeftNormal(){//добавить 3d вектор-нормаль влево (три значения)
		push_back(-1);
		push_back(0);
		push_back(0);
	};
	void pushRightNormal(){//добавить 3d вектор-нормаль вправо (три значения)
		push_back(1);
		push_back(0);
		push_back(0);
	};
	int getElementsCount(){return elementsUsed;};
	void goToStart(){elementsUsed=0;};
};