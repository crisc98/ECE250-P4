/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  uwuserid @uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter|Spring|Fall) 201N
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
		static_cast<int>(obj) & mask;
}
template <typename Type>
int Quadratic_hash_table<Type>::capacity() const {
		return array_size;
}
template <typename Type>
int Quadratic_hash_table<Type>::size() const {
		return count;
}
template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const {
		return ((count+numDel) / array_size);
}

template <typename Type>
bool Quadratic_hash_table<Type>::empty() const {
	if(array_size == 0)
		return true;
	return false;
}
template <typename Type>
Type Quadratic_hash_table<Type>::bin(int n) const {
	if(array_size-1 >= n && occupied[n] == OCCUPIED)
		return array[n];
}
template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj) const {
	//set probe to be hash function 1's result
	int probe = hash(obj);
	int offset = 1;
	//initiate counter that will keep track of number of iterations through table
	int counter = 0;

	//while seeing an occupied bin and counter has not reached its limit
	while(occupied[probe] == OCCUPIED && counter < array_size)
	{
		//if element is what we're looking for
		if(array[probe] == obj)
		{
			//it means it exists in the hash table so return true
			return true;
			//break out of while loop
			break;
		}
		//if element is not what we're looking for, keep iterating quadraticaly
		probe +=(offset*offset);
		offset++;
		//increase counter by one
		counter++;
	}
	//if object is not in hash table return false
	return false;
}

template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj){
	//if hash table is full throw overflow exception
	if(count == array_size)
		throw overflow();

	if(member(obj))
		return;

	//set probe to start at the hash value of the object
	int probe = hash(obj);
	int offset = 1;
	//if a collision occurs
	while(occupied[probe] == OCCUPIED)
	{
		//offset the probe by offset^2
		probe += (offset*offset);
		offset++;
	}
	//store object in hash table
	array[probe] = obj;
	//set state of that bin to occupied
	occupied[probe] = OCCUPIED;
	//increase count by 1
	count++;
	return;
}

template <typename Type>
bool Quadratic_hash_table<Type>::erase(Type const &obj){
	if(count == array_size)
		throw overflow();

	//set probe to start at the hash value of the object
	int probe = hash(obj);
	int offset = 1;
	int counter = 0;
	//if a collision occurs
	while(occupied[probe] == OCCUPIED && counter < array_size)
	{
		if(array[probe] == obj)
		{
			//set value in bin to 0
			array[probe] = 0;
			//set state to deleted
			occupied[probe] = ERASED;
			//decrease count by 1
			count--;
			//it means it exists in the hash table so return true
			return true;
		}
		//offset the probe by offset^2
		probe += (offset*offset);
		offset++;
	}
	
	return false;
}

template <typename Type>
void Quadratic_hash_table<Type>::clear() {
	//traverse through entire hash table
	for(int i = 0; i < array_size; i++)
	{
		//set values to 0
		array[i] = 0;
		//set states to UNOCCUPIED
		occupied[i] = UNOCCUPIED;
	}
	//set count to 0
	count = 0;
	return;
}
// Your implementation here

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
