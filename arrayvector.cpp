//cai dat vector bang mang
#include <bits/stdc++.h>
using namespace std;
#ifndef __arrayvector__cpp
#define __arrayvector__cpp
template <class T>
class Vector_reverse_iterator;
template <class T>
class Vector{
	unsigned int cap,num;
	T *buff;
	public:
		typedef T *iterator;
		typedef Vector_reverse_iterator<T> reverse_iterator;
		Vector(){
			cap=0;num=0;buff=NULL;
		}
		Vector(unsigned int C){
			cap=C;
			num=C;
			buff = new T[C];
		}
		Vector(unsigned int n, T x){
			cap = num = n; 	buff = new T[n];
			for(int i=0;i<n;i++) buff[i] = x;
		}
		~Vector(){
			if(buff) delete[] buff;
		}
		void reserve(unsigned int newcap){
			if(newcap < cap) return;
			T *temp = new T[newcap];
			for(int i=0;i< num; i++) temp[i]=buff[i];
			if(buff) delete[] buff;
			buff = temp;
		}
		T back(){
			return buff[num-1];
		}
		unsigned int size() {
			return num;
		}
		bool empty(){
			return num==0;
		}
		void pop_back(){
			if(num) num--;
		}
		void push_back(T x){
			if(cap==num) reserve(cap*2+1); buff[num++] = x;
		}
		T &at(int k){
			if(k<0||k>=num){			
				k=0;
			} 
		return buff[k];

	}
	T &operator[](int k){
		if(k<0||k>=num){			
			k=0;
		} 
		return buff[k];
	}
	void resize(int newsize,T x=0){ // nếu giá trị x != 0 thì tất cả sẽ dc gán giá tri truyền vào đó
		if(newsize>cap) reserve(newsize);
		 num = newsize;
		 for(int i=0;i<num;i++) buff[i]=x;
	}
	void remove(int k){
		if(k<0||k>=num) return;
		for(int i=k+1;i <= num-1; i++){
			buff[i-1] = buff[i];
		}
		num--;
	}
	void insert(int k,T x){
		if(k<0 || k>=num) return;
		if(num==cap){
			reserve(cap*2+1);
		}
		for(int i=num-1;i>=k;i--) buff[i+1] = buff[i];
		buff[k] = x;
		num++; 
	}
	iterator begin(){
		return buff;
	}
	iterator end(){
		return buff+num;
	}
	reverse_iterator rbegin(){
		return buff+(num-1);
	}
	reverse_iterator rend(){
		return buff-1;
	}
};

template <class T>
class Vector_reverse_iterator{
	T *curr;
	public:
		Vector_reverse_iterator(T *buff=NULL){
			curr = buff;
		}
		Vector_reverse_iterator operator++(){
			return --curr;
		}
		T *getCurr(){
			return curr;
		}
		bool operator!=(Vector_reverse_iterator<T> V){
			return curr!=V.getCurr();
		}
		T operator*(){
			return *curr;
		}
};
#endif
