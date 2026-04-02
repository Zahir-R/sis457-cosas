#pragma once

#include "CoreMinimal.h"

template <typename T>
class Nodo {
public:
	T Element;
	Nodo<T>* Next;
	Nodo(T Value) : Element(Value), Next(nullptr) {}
};

template <typename T>
class MyList {
private:
	Nodo<T>* Head;
	int32 Size;

public:
	MyList() : Head(nullptr), Size(0) {}
	~MyList() { Empty(); }

	void Insert(T Val)
	{
		Nodo<T>* NewNode = new Nodo<T>(Val);
		if (Head == nullptr) {
			Head = NewNode;
		}
		else {
			Nodo<T>* Temp = Head;
			while (Temp->Next) {
				Temp = Temp->Next;
			}
			Temp->Next = NewNode;
		}
		Size++;
	}

	void Empty()
	{
		while (Head)
		{
			Nodo<T>* Temp = Head;
			Head = Head->Next;
			delete Temp;
		}
		Size = 0;
	}

	int32 GetSize() const { return Size; }
	Nodo<T>* GetHead() const { return Head; }
};