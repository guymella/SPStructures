//------------------------------------------------------------------------------
//  ArrayTest.h
//  Test Array class.
//------------------------------------------------------------------------------

#include "Structures/Containers/SparseArray.h"
#include <assert.h>
#include <algorithm>
#define CHECK assert

bool testSparseArray() {
	// create empty array
	SparseArray<int> array0;
	//CHECK(array0.GetMinGrow() == ORYOL_CONTAINER_DEFAULT_MIN_GROW);
	//CHECK(array0.GetMaxGrow() == ORYOL_CONTAINER_DEFAULT_MAX_GROW);
	CHECK(array0.Size() == 0);
	//CHECK(array0.Capacity() == 0);
	CHECK(array0.Empty());
	//CHECK(array0.Spare() == 0);

	// copy-construct from empty array
	SparseArray<int> array1(array0);
	//CHECK(array1.GetMinGrow() == ORYOL_CONTAINER_DEFAULT_MIN_GROW);
	//CHECK(array1.GetMaxGrow() == ORYOL_CONTAINER_DEFAULT_MAX_GROW);
	CHECK(array1.Size() == 0);
	//CHECK(array1.Capacity() == 0);
	CHECK(array1.Empty());
	//CHECK(array1.Spare() == 0);

	// copy-assign empty array
	SparseArray<int> array2 = array0;
	//CHECK(array1.GetMinGrow() == ORYOL_CONTAINER_DEFAULT_MIN_GROW);
	//CHECK(array1.GetMaxGrow() == ORYOL_CONTAINER_DEFAULT_MAX_GROW);
	CHECK(array1.Size() == 0);
	//CHECK(array1.Capacity() == 0);
	CHECK(array1.Empty());
	//CHECK(array1.Spare() == 0);

	// add some elements
	const int one = 1;
	CHECK(array0.PushBack(0) == 0);
	CHECK(array0.PushBack(one) == one);
	CHECK(array0.PushBack(2) == 2);
	CHECK(array0.Size() == 3);
	//CHECK(array0.Capacity() == ORYOL_CONTAINER_DEFAULT_MIN_GROW);
	CHECK(!array0.Empty());
	//CHECK(array0.Spare() == (ORYOL_CONTAINER_DEFAULT_MIN_GROW - 3));
	CHECK(array0[0] == 0);
	CHECK(array0[1] == 1);
	CHECK(array0[2] == 2);
	CHECK(array0.Front() == 0);
	CHECK(array0.Back() == 2);

	// copy-construct from non-empty array
	SparseArray<int> array3(array0);
	CHECK(array3.Size() == 3);
	//CHECK(array3.Capacity() == 3);
	CHECK(!array3.Empty());
	//CHECK(array3.Spare() == 0);
	CHECK(array3[0] == 0);
	CHECK(array3[1] == 1);
	CHECK(array3[2] == 2);
	CHECK(array3.Front() == 0);
	CHECK(array3.Back() == 2);

	// copy-assign from non-empty array
	array2 = array0;
	CHECK(array2.Size() == 3);
	//CHECK(array2.Capacity() == 3);
	CHECK(!array2.Empty());
	//CHECK(array2.Spare() == 0);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 2);
	CHECK(array2.Front() == 0);
	CHECK(array2.Back() == 2);

	// move-assign
	array2 = std::move(array0);
	CHECK(array0.Size() == 0);
	CHECK(array2.Size() == 3);
	//CHECK(array2.Capacity() == ORYOL_CONTAINER_DEFAULT_MIN_GROW);
	CHECK(!array2.Empty());
	//CHECK(array2.Spare() == (ORYOL_CONTAINER_DEFAULT_MIN_GROW - 3));
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 2);
	CHECK(array2.Front() == 0);
	CHECK(array2.Back() == 2);

	// test add
	array2.PushBack(3);
	CHECK(array2.Size() == 4);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 2);
	CHECK(array2[3] == 3);

	// check element copy-assignment and move-assignment
	array2[0] = 2;
	CHECK(array2[0] == 2);
	array2[0] = 0;

	// insert element
	array2.Insert(0, 5);
	CHECK(array2.Size() == 5);
	CHECK(array2[0] == 5);
	CHECK(array2[1] == 0);
	CHECK(array2[2] == 1);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);
	// append with insert (this is valid)
	array2.Insert(5, 6);
	CHECK(array2.Size() == 6);
	CHECK(array2[0] == 5);
	CHECK(array2[1] == 0);
	CHECK(array2[2] == 1);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);
	CHECK(array2[5] == 6);
	array2.Insert(2, 7);
	CHECK(array2.Size() == 7);
	CHECK(array2[0] == 5);
	CHECK(array2[1] == 0);
	CHECK(array2[2] == 7);
	CHECK(array2[3] == 1);
	CHECK(array2[4] == 2);
	CHECK(array2[5] == 3);
	CHECK(array2[6] == 6);
	array2.Insert(4, 8);
	CHECK(array2.Size() == 8);
	CHECK(array2[0] == 5);
	CHECK(array2[1] == 0);
	CHECK(array2[2] == 7);
	CHECK(array2[3] == 1);
	CHECK(array2[4] == 8);
	CHECK(array2[5] == 2);
	CHECK(array2[6] == 3);
	CHECK(array2[7] == 6);

	// erase, keep order
	array2.Erase(0);
	CHECK(array2.Size() == 7);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 7);
	CHECK(array2[2] == 1);
	CHECK(array2[3] == 8);
	CHECK(array2[5] == 3);
	CHECK(array2[6] == 6);
	array2.Erase(6);
	CHECK(array2.Size() == 6);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 7);
	CHECK(array2[2] == 1);
	CHECK(array2[3] == 8);
	CHECK(array2[4] == 2);
	CHECK(array2[5] == 3);
	array2.Erase(1);
	CHECK(array2.Size() == 5);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 8);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);

	// check erase swap
	//array2.Trim();
	CHECK(array2.Size() == 5);
	//CHECK(array2.Capacity() == 5);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 8);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);
	array2.PushBack(4);
	array2.PushBack(5);
	CHECK(array2.Size() == 7);
	//CHECK(array2.Capacity() == (5 + ORYOL_CONTAINER_DEFAULT_MIN_GROW));
	//array2.Trim();
	CHECK(array2.Size() == 7);
	//CHECK(array2.Capacity() == 7);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 8);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);
	CHECK(array2[5] == 4);
	CHECK(array2[6] == 5);
	array2.EraseSwap(2);
	CHECK(array2.Size() == 6);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 5);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);
	CHECK(array2[5] == 4);
	array2.EraseSwap(3);
	CHECK(array2.Size() == 5);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 1);
	CHECK(array2[2] == 5);
	CHECK(array2[3] == 4);
	CHECK(array2[4] == 3);
	array2.EraseSwapBack(1);
	CHECK(array2.Size() == 4);
	CHECK(array2[0] == 0);
	CHECK(array2[1] == 3);
	CHECK(array2[2] == 5);
	CHECK(array2[3] == 4);
	array2.EraseSwapFront(2);
	CHECK(array2.Size() == 3);
	CHECK(array2[0] == 3);
	CHECK(array2[1] == 0);
	CHECK(array2[2] == 4);

	array2.Clear();
	CHECK(array2.Size() == 0);
	//CHECK(array2.Capacity() == 7);
	CHECK(array2.begin() == array2.end());
	for (int i = 0; i < 16; i++) {
		array2.PushBack(i);
	}
	for (int* p = array2.begin(), i = 0; p != array2.end(); p++, i++) {
		CHECK(*p == i);
	}
	array2.Clear();
	for (int* p = array2.begin(); p != array2.end(); p++) {
		CHECK(false);
	}

	// check for for(:) works
	for (int i = 0; i < 16; i++) {
		array2.PushBack(i);
	}
	for (int x : array2) {
		CHECK(array2[x] == x);
	}

	// test some algorithms stuff
	array2.Clear();
	array2.PushBack(1);
	array2.PushBack(3);
	array2.PushBack(5);
	array2.PushBack(7);
	array2.PushBack(9);
	array2.PushBack(11);
	bool allOdd = std::all_of(array2.begin(), array2.end(), [](int i) { return i % 2 == 1; });
	CHECK(allOdd);
	bool anyEven = std::any_of(array2.begin(), array2.end(), [](int i) { return i % 2 == 0; });
	CHECK(!anyEven);
	array2[2] = 2;
	allOdd = std::all_of(array2.begin(), array2.end(), [](int i) { return i % 2 == 1; });
	CHECK(!allOdd);
	anyEven = std::any_of(array2.begin(), array2.end(), [](int i) { return i % 2 == 0; });
	CHECK(anyEven);
	std::reverse(array2.begin(), array2.end());
	CHECK(array2[0] == 11);
	CHECK(array2[1] == 9);
	CHECK(array2[2] == 7);
	CHECK(array2[3] == 2);
	CHECK(array2[4] == 3);
	CHECK(array2[5] == 1);
	std::sort(array2.begin(), array2.end());
	CHECK(array2[0] == 1);
	CHECK(array2[1] == 2);
	CHECK(array2[2] == 3);
	CHECK(array2[3] == 7);
	CHECK(array2[4] == 9);
	CHECK(array2[5] == 11);

	// FindIndexLinear
	//CHECK(array2.FindIndexLinear(3) == 2);
	//CHECK(array2.FindIndexLinear(7, 0, 3) == InvalidIndex);
	//CHECK(array2.FindIndexLinear(7, 3, InvalidIndex) == 3);
	//CHECK(array2.FindIndexLinear(11) == 5);

	// reverse sort
	std::sort(array2.begin(), array2.end(), [](int a, int b) {
		return b < a;
		});
	CHECK(array2[0] == 11);
	CHECK(array2[1] == 9);
	CHECK(array2[2] == 7);
	CHECK(array2[3] == 3);
	CHECK(array2[4] == 2);
	CHECK(array2[5] == 1);

	// construct from initializer list
	/*Array<String> array4({ "Bla", "Blub", "Blob", "Blubber" });
	CHECK(array4.Size() == 4);
	CHECK(array4[0] == "Bla");
	CHECK(array4[1] == "Blub");
	CHECK(array4[2] == "Blob");
	CHECK(array4[3] == "Blubber");*/

	// test emplace
	/*Array<KeyValuePair<int, String>> array5;
	array5.PushBack(1, "Bla");
	array5.PushBack(2, "Blub");
	CHECK(array5.Size() == 2);
	CHECK(array5[0].Key() == 1);
	CHECK(array5[0].Value() == "Bla");
	CHECK(array5[1].Key() == 2);
	CHECK(array5[1].Value() == String("Blub"));*/

	// pop back and front
	SparseArray<int> popArray;
	for (int i = 0; i < 10; i++) {
		popArray.PushBack(i);
	}
	CHECK(popArray.Size() == 10);

	CHECK(popArray.PopBack() == 9);
	CHECK(popArray.Size() == 9);
	CHECK(popArray.Back() == 8);

	CHECK(popArray.PopFront() == 0);
	CHECK(popArray.Size() == 8);
	CHECK(popArray.Front() == 1);

	CHECK(popArray.PopBack() == 8);
	CHECK(popArray.Size() == 7);
	CHECK(popArray.Back() == 7);

	CHECK(popArray.PopFront() == 1);
	CHECK(popArray.Size() == 6);
	CHECK(popArray.Front() == 2);

	// EraseRange
	SparseArray<int> array6;
	for (int i = 0; i < 16; i++) {
		array6.PushBack(i);
	}
	CHECK(array6.Size() == 16);
	array6.EraseRange(0, 3);
	CHECK(array6.Size() == 13);
	CHECK(array6[0] == 3);
	CHECK(array6[1] == 4);
	array6.EraseRange(2, 3);
	CHECK(array6.Size() == 10);
	CHECK(array6[0] == 3);
	CHECK(array6[1] == 4);
	CHECK(array6[2] == 8);
	CHECK(array6[3] == 9);
	array6.EraseRange(7, 3);
	CHECK(array6.Size() == 7);
	CHECK(array6[0] == 3);
	CHECK(array6[1] == 4);
	CHECK(array6[2] == 8);
	CHECK(array6[3] == 9);
	CHECK(array6[6] == 12);

	//// MakeSlice
	//SparseArray<int> array7 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//Slice<int> slice = array7.MakeSlice(3, 4);
	//CHECK(slice.Size() == 4);
	//CHECK(slice.Offset() == 3);
	//CHECK(slice[0] == 4);
	//CHECK(slice[1] == 5);
	//CHECK(slice[2] == 6);
	//CHECK(slice[3] == 7);
	//CHECK(slice.begin() == &array7[3]);
	//CHECK(slice.end() == &array7[7]);
	//slice[0] = 123;
	//CHECK(array7[3] == 123);

	//// array view over full array
	//slice = array7.MakeSlice();
	//CHECK(slice.Size() == 10);
	//CHECK(slice.Offset() == 0);
	//CHECK(slice[0] == 1);
	//CHECK(slice[9] == 10);

	////test Slice offset operators
	//slice = array7.MakeSlice(2, 2);
	//CHECK(slice.Size() == 2);
	//CHECK(slice.Offset() == 2);
	//CHECK(slice[0] == 3);
	//CHECK(slice[1] == 123);
	//Slice<int> sl1 = slice++;
	//CHECK(sl1[0] == 3);
	//CHECK(sl1[1] == 123);
	//CHECK(slice[0] == 123);
	//CHECK(slice[1] == 5);
	//Slice<int> sl2 = ++slice;
	//CHECK(sl2[0] == 5);
	//CHECK(sl2[1] == 6);
	//CHECK(sl1 + 2 == sl2);
	//sl1 += 2;
	//CHECK(sl1 == sl2);



	//// MakesmartSlice
	//Array<int> array8 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//SmartSlice<int> smartslice = array8.MakeSmartSlice(3, 4);
	//CHECK(smartslice.Size() == 4);
	//CHECK(smartslice.Offset() == 3);
	//CHECK(smartslice[0] == 4);
	//CHECK(smartslice[1] == 5);
	//CHECK(smartslice[2] == 6);
	//CHECK(smartslice[3] == 7);
	//CHECK(smartslice.begin() == &array8[3]);
	//CHECK(smartslice.end() == &array8[7]);
	//smartslice[0] = 123;
	//CHECK(array8[3] == 123);
	//smartslice.PushBack(124);
	//CHECK(smartslice.Size() == 5);
	//CHECK(smartslice.Offset() == 3);
	//CHECK(array8.Size() == 11);
	//CHECK(smartslice[0] == 123);
	//CHECK(smartslice[1] == 5);
	//CHECK(smartslice[2] == 6);
	//CHECK(smartslice[3] == 7);
	//CHECK(smartslice[4] == 124);
	//CHECK(smartslice.begin() == &array8[3]);
	//CHECK(smartslice.end() == &array8[8]);
	
	// fixed capacity
	//Array<int> array8;
	//array8.SetFixedCapacity(128);
	//CHECK(array8.Size() == 0);
	//CHECK(array8.Capacity() == 128);
	//CHECK(array8.GetMinGrow() == 0);
	//CHECK(array8.GetMaxGrow() == 0);


	//Check Sparse
	
	SparseArray<int> array9;
	for (int i = 0; i < 16; i++) {
		array9.PushBack(i);
	}
	CHECK(array9.Size() == 16);
	CHECK(!array9.Exists(20));
	CHECK(array9[20] == 0);
	CHECK(array9.Size() == 21);
	CHECK(array9[22] = 123);
	CHECK(array9.Size() == 23);
	CHECK(array9.Exists(20));
	CHECK(!array9.Exists(21));
	CHECK(array9.Exists(22));
	CHECK(*array9.Exists(20) == 0);
	CHECK(*array9.Exists(22)==123);

	//check shift
	array9[20] = 122;
	array9.PushBack(124);
	array9.PushBack(125);
	array9.ShiftRange(12, 10, -2);

	CHECK(array9[9] == 9);
	CHECK(array9[10] == 12);
	CHECK(array9[11] == 13);
	CHECK(array9[12] == 14);
	CHECK(array9[13] == 15);
	CHECK(!array9.Exists(14));
	CHECK(!array9.Exists(15));
	CHECK(*array9.Exists(18) == 122);
	CHECK(!array9.Exists(19));
	CHECK(!array9.Exists(20));
	CHECK(!array9.Exists(21));
	CHECK(*array9.Exists(22) == 123);
	CHECK(*array9.Exists(23) == 124);
	CHECK(*array9.Exists(24) == 125);
	CHECK(!array9.Exists(25));

	array9.ShiftRange(12, 11, 2);

	CHECK(array9[9] == 9);
	CHECK(array9[10] == 12);
	CHECK(array9[11] == 13);
	CHECK(!array9.Exists(12));
	CHECK(!array9.Exists(13));
	CHECK(array9[14] == 14);
	CHECK(array9[15] == 15);
	CHECK(!array9.Exists(18));
	CHECK(*array9.Exists(20) == 122);
	CHECK(!array9.Exists(21));
	CHECK(!array9.Exists(22));
	CHECK(!array9.Exists(23));
	CHECK(*array9.Exists(24) == 123);
	CHECK(!array9.Exists(25));

	array9.ShiftRange(20, 10, 100);

	CHECK(array9.Size() == 125);
	CHECK(array9[9] == 9);
	CHECK(array9[10] == 12);
	CHECK(array9[11] == 13);
	CHECK(!array9.Exists(12));
	CHECK(!array9.Exists(13));
	CHECK(array9[14] == 14);
	CHECK(array9[15] == 15);
	CHECK(!array9.Exists(18));
	CHECK(!array9.Exists(20));
	CHECK(!array9.Exists(21));
	CHECK(!array9.Exists(22));
	CHECK(!array9.Exists(23));
	CHECK(!array9.Exists(24));
	CHECK(*array9.Exists(120) == 122);
	CHECK(!array9.Exists(121));
	CHECK(!array9.Exists(122));
	CHECK(!array9.Exists(123));
	CHECK(*array9.Exists(124) == 123);
	CHECK(array9.Back() == 123);

	return true;
}