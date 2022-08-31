/*
 * Expected.h
 *
 *  Created on: Jun 29, 2022
 *      Author: 16502
 */

#ifndef EXPECTED_H_
#define EXPECTED_H_

template <typename T, typename exception_type>
class Expected {
public:
	Expected(const exception_type& e);
	Expected(const T& in);
	~Expected();
	operator bool(){return expected;}
	bool get(T&, exception_type&);
	void get(T&);
	void get(exception_type&);

private:
	bool expected;
	union {
		exception_type e;
		T val;
	}v;
};

template <typename T, typename exception_type>
Expected<T, exception_type>::Expected(const exception_type& e): expected(false)
{
	v.e = e;
}

template <typename T, typename exception_type>
Expected<T, exception_type>::Expected(const T& in): expected(true){
	v.val = in;
}

//obviously this is super expensive if T is expensive to construct and I throw a lot - sorry
template <typename T, typename exception_type>
bool Expected<T, exception_type>::get(T& t_out, exception_type& e_out){
	if(expected){
		t_out = v.val;
	}else{
		e_out = v.e;
	}
	return expected;
}

template <typename T, typename exception_type>
void Expected<T, exception_type>::get(T& t_out){
	t_out = v.val;
}

template <typename T, typename exception_type>
void Expected<T, exception_type>::get(exception_type& e_out){
	e_out = v.e;
}

#endif /* EXPECTED_H_ */
