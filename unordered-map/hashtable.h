#ifndef HASHTABLE_H
#define HASHTABLE_H

//This class implements a Hash Table using separate-chaining.

//Hashtable will be stored internally as a vector of lists
#include <vector>
#include <list>

//For making data pairs (for key,value)
#include <utility> 

//For file I/O
#include <fstream>

//NOTE: I would normally implement this using a .h and .cpp file, however the requirement
//for this project was to use a .hpp file instead.

// max_prime is used by the given functions (end of .hpp file)
static const unsigned int max_prime = 1301081;

//The default_capacity is used if the initial capacity of the underlying vector of the 
//hash table is zero. 
static const unsigned int default_capacity = 11;

namespace cop4530 {
	template<typename K, typename V>
	class HashTable {
	public:
		HashTable(size_t size = 101); 
		/*
			Creates a hash table, 
			where the size of the vector is set to prime_below(size) (where size is default
			to 101), where prime_below() is a private member function given in-class.
		*/
		~HashTable(); 
		/*
		    Deletes all elements in hash table.
		*/

		bool contains(const K& k) const;
		/*
		    Checks if key k is in the hash table.
		*/

		bool match(const std::pair<K, V>& kv) const;
		/*
		    Checks if key - value pair is in the hash table.
		*/

		bool insert(const std::pair<K, V>& kv);
		/*
		    Adds the key - value pair kv into the hash table.
		    Doesn't add if kv is already in the hash table.
		    If the key is the hash table but with a different value, the value will be
		    updated to the new one with kv.
		    Returns true if kv is inserted or the value is updated.
			    Returns false otherwise (i.e., if kv is in the hash table).
		*/

		bool insert(std::pair<K, V>&& kv);
		/*
		    Move version of insert.
		*/

		bool remove(const K& k);
		/*
		    Deletes the key k and the corresponding value if it is in the hash table.
		    Return true if k is deleted, returns false otherwise (i.e., if key k is not in
		    the hash table).
		*/

		void clear();
		/*
		    Deletes all elements in the hash table
		*/

		bool load(const char* filename);
		/*
		    Loads the content of the file with name filename into the hash table.
		    In the file, each line contains a single pair of keyand value, separated by a
		    white space.
		*/

		void dump() const;
		/*
		    Displays all entries in the hash table.If an entry contains multiple key -
		    value pairs,
		    separated by a semicolon character
		*/

		size_t size() const;
		/*
		    Returns the number of elements in the hash table.
		*/

		bool write_to_file(const char* filename) const;
		/*
		    Writes all elements in the hash table into a file with name -filename-.
		    Similar to the file format in the load function, each line contains a pair of
		    key - value pair, separated by a white space.
		*/

	private:
		void makeEmpty();
		/*
		    Delete all elements in the hash table.The public interface clear() will call
		    this function.
		*/

		void rehash();
		/*
		    Rehash function. Called when the number of elements in the hash table is greater
		    than the size of the vector.
		*/

		size_t myhash(const K& k) const;
		/*
		    Return the index of the vector entry where k should be stored.
		*/
		
		unsigned long prime_below(unsigned long);

		void setPrimes(std::vector<unsigned long>&);
		/*
		    Two helpful functions to determine the proper prime numbers used in setting up
		    the vector size.
			    Whenever you need to set hash table to a new size "sz", call prime_below(sz)
			    to determine the new proper underlying vector size.
		*/
			

		//---DATA---//

		int currentSize;
		/*
		    Amount of elements currently in myHashTable, NOT its total size.
		*/

		std::vector<std::list<std::pair<K, V>>> myHashTable;
		/*
		    The hash table's storage.
		    Vector of list of pairs, each pair contains a key / value pair.
		*/
		
	};

	//Implementation file
	#include "hashtable.hpp"
}
#endif
