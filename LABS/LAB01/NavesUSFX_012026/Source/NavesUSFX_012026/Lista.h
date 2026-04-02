#pragma once
#include "CoreMinimal.h"

template<typename T>
class TCustomList {
private:
	T* Elements;
	int32 Size;
	int32 Capacity;

public:
	TCustomList(int32 capacity = 20) : Size(-1), Capacity(capacity) {
		Elements = new T[Capacity];
	}
	~TCustomList() {
		delete[] Elements;
	}

	void Add(T Element) {
		if (Size < Capacity - 1) {
			Elements[++Size] = Element;
		}
	}

	T Get(int32 Index) {
		if (Index >= 0 && Index <= Size) {
			return Elements[Index];
		}
		return T(); // Valor predeterminado
	}

	int32 Num() { return Size + 1; }

	void Empty() { Size = -1; }
};