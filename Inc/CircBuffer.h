/*
 * CircBuffer.h
 *
 *  Created on: Jun 29, 2022
 *      Author: 16502
 */

#ifndef INC_CIRCBUFFER_H_
#define INC_CIRCBUFFER_H_

#include <cmath>
#include "Expected.h"

enum class CircBufCodes {
	Buf_Success,
	Buf_Full_error,
	Buf_empty_error,
	Buf_improper_index,
	Buf_out_of_bounds
};


template<typename T, int L>
class CircBuffer {
public:

	using expect_T = Expected<T, CircBufCodes>;

	CircBuffer();
	~CircBuffer();
	expect_T operator [](int);
	expect_T pop_front();
	expect_T pop_back();
	expect_T push(const T&);
	expect_T rotate_buf(const T&);
	int get_size();

private:
	T vals[L]; //should this be a pointer array, wut?
	int arr_start;
	int arr_end;
};

template<typename T, int L>
CircBuffer<T, L>::CircBuffer(): arr_start(0), arr_end(1){

}

template<typename T, int L>
int CircBuffer<T, L>::get_size(){
	return arr_start < arr_end ? arr_end - arr_start : L - arr_start + arr_end - 1;
}

template<typename T, int L>
typename CircBuffer<T,L>::expect_T CircBuffer<T, L>::operator [](const int i){
	if (i > get_size()){
		return expect_T(CircBufCodes::Buf_out_of_bounds);
	}
	return expect_T(vals[(arr_start+i)%L]);
}

template<typename T, int L>
typename CircBuffer<T,L>::expect_T CircBuffer<T, L>::pop_front(){
	if(arr_start == arr_end - 1 || (arr_start == L - 1 && arr_end == 0)){
		return CircBuffer<T,L>::expect_t(CircBufCodes::Buf_empty_error);
	}

	T t_ = vals[arr_end];
	arr_end = arr_end == 0 ? L - 1: arr_end - 1;

	return expect_T(t_);
}

template<typename T, int L>
typename CircBuffer<T,L>::expect_T CircBuffer<T, L>::pop_back(){
	if(arr_start == arr_end - 1 || (arr_start == L - 1 && arr_end == 0)){
		return CircBuffer<T,L>::expect_t(CircBufCodes::Buf_empty_error);
	}

	T t_ = vals[arr_start];
	arr_start = (arr_start + 1)%(L);

	return expect_T(t_);
}

template<typename T, int L>
typename CircBuffer<T,L>::expect_T CircBuffer<T, L>::push(const T& input){
	if(arr_start == arr_end){
		return CircBuffer<T,L>::expect_t(CircBufCodes::Buf_Full_error);
	}

	arr_end = (arr_end + 1)%(L);
	vals[arr_end] = input;

	return CircBuffer<T,L>::expect_T(CircBufCodes::Buf_Success);

}


//array can't be empty to rotate - need to add a check for that
template<typename T, int L>
typename CircBuffer<T,L>::expect_T CircBuffer<T, L>::rotate_buf(const T& input){
	if(arr_start == arr_end - 1 || (arr_start == L - 1 && arr_end == 0)){
		return CircBuffer<T,L>::expect_t(CircBufCodes::Buf_empty_error);
	}

	T t_ = pop_front();
	push(input);
	return t_;
}
#endif /* INC_CIRCBUFFER_H_ */
