/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  czchelot @uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  Winter 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    -
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Exception.h"
#include "ece250.h"

enum bin_state_t { UNOCCUPIED, OCCUPIED, ERASED };

template <typename Type>
class Quadratic_hash_table {
	private:
		int count;
		int power;
		int array_size;
		int mask;
		int numDel;
		Type *array;
		bin_state_t *occupied;

		int hash( Type const & ) const;

	public:
		Quadratic_hash_table( int = 5 );
		~Quadratic_hash_table();
		int size() const;
		int capacity() const;
		double load_factor() const;
		bool empty() const;
		bool member( Type const & ) const;
		Type bin( int ) const;

		void print() const;

		void insert( Type const & );
		bool erase( Type const & );
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Quadratic_hash_table<T> const & );
};

template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
numDel( 0 ),
array( new Type[array_size] ),
occupied( new bin_state_t[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = UNOCCUPIED;
	}
}
template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table(){
	delete[] array;
	delete[] occupied;
}
template <typename Type>
int Quadratic_hash_table<Type>::hash(Type const &obj) const {
	//Returns the integer portion of the object mod the array size using bitwise AND with mask
		return (static_cast<int>(obj) & mask);
}
template <typename Type>
int Quadratic_hash_table<Type>::capacity() const {
	//Returns the max capacity of the hash table
		return array_size;
}
template <typename Type>
int Quadratic_hash_table<Type>::size() const {
	//Returns the number of elements in the hash table
		return count;
}
template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const {
	//Returns the number of occupied bins plus the number of erased bins over the array size
		return ((double)(count+numDel) / array_size);
}

template <typename Type>
bool Quadratic_hash_table<Type>::empty() const {
	//Returns false if there are no elements in the table
	if(count == 0)
		return true;
	return false;
}
template <typename Type>
Type Quadratic_hash_table<Type>::bin(int n) const {
	//Returns the value at a specified bin number
	if(array_size-1 >= n && occupied[n] == OCCUPIED)
		return array[n];
}
template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj) const {
	//Start probing at the has value of the object
	int probe = hash(obj);
	//Set initial offset value to 1
	int offset = 1;
	//Initialize counter for number of iterations
	int counter = 0;

	//Continue looping while on an occupied or erased bin, making sure that you have not counted more times than the array size
	while((occupied[probe] == OCCUPIED || occupied[probe] == ERASED) && counter < array_size)
	{
		//If we find the object in at this bin
		if(occupied[probe] == OCCUPIED && array[probe] == obj)
		{
			//Return true
			return true;
			//Exit loop
			break;
		}
		//If we do not find the object yet, keep iterating quadratically
		probe = probe+offset &mask;
		offset++;
		counter++;
	}
	//If object not in table return false
	return false;
}

template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj){
	//Throw oveflow if table is full
	if(count == array_size)
		throw overflow();

		//If the obj is in the table already, exit as we cannot insert it again
	 if(member(obj))
	 	return;

	//Start probing at the has value
	int probe = hash(obj);
	int offset = 1;

	//If we try to insert into an occupied bin
	while(occupied[probe] == OCCUPIED)
	{
		//Iterate quadratically until an empty bin is found
		probe = probe+offset &mask;
		offset++;
	}
	//Store object in bin
	array[probe] = obj;

	//If we insert into a bin that was previously erased, decrement the number of deleted bins
	if(occupied[probe] == ERASED){
		numDel--;
	}
	//Set bin to occupied
	occupied[probe] = OCCUPIED;
	count++;
	return;
}

template <typename Type>
bool Quadratic_hash_table<Type>::erase(Type const &obj){

	//set probe to start at the hash value of the object
	int probe = hash(obj);
	int offset = 1;
	int counter = 0;

	if(!member(obj)){
		return false;
	}
	//if a collision occurs
	while((occupied[probe] == OCCUPIED || occupied[probe] == ERASED) && counter < array_size)
	{
		if(occupied[probe] == OCCUPIED && array[probe] == obj)
		{
			//set value in bin to 0
			array[probe] = 0;
			//set state to deleted
			occupied[probe] = ERASED;
			//decrease count by 1
			numDel++;
			count--;
			//it means it exists in the hash table so return true
			//std::cout <<*this<<std::endl;
			return true;
		}
		//offset the probe by offset^2
		probe = probe+offset &mask;
		offset++;
	}
	//std::cout <<*this<<std::endl;

	return false;
}

template <typename Type>
void Quadratic_hash_table<Type>::clear() {
	//Loop through table
	for(int i = 0; i < array_size; i++)
	{
		//Assign bin vals to 0
		array[i] = 0;
		//Set states to UNOCCUPIED
		occupied[i] = UNOCCUPIED;
	}
	//Reset count and numDel to 0
	count = 0;
	numDel = 0;
	return;
}

template <typename T>
std::ostream &operator<<( std::ostream &out, Quadratic_hash_table<T> const &hash ) {
	for ( int i = 0; i < hash.capacity(); ++i ) {
		if ( hash.occupied[i] == UNOCCUPIED ) {
			out << "- ";
		} else if ( hash.occupied[i] == ERASED ) {
			out << "x ";
		} else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif
