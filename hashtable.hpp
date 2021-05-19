//Implementation file for hashtable.h

/*
    Initializes current size to 0.

    Steps through elements of each list in vector and initializes
    to new [key type] and new [value type].
*/
template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size) {
    currentSize = 0;

    for (auto& currentList : myHashTable) {
        for (auto& currentPair : currentList) {
            K newK{};
            V newV{};
            currentPair.first  = newK;
            currentPair.second = newV;
        }
    }
}

/*
    Calls the public function makeEmpty, C++ deallocates the rest automatically.
*/
template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    makeEmpty();
}

/*
    Runs hash function on key and gets reference to list at that index in the vector.
    Then steps through the list and if a matching key is found, returns true, else returns false.
*/
template <typename K, typename V>
bool HashTable<K, V>::contains(const K& k) const {
    //whichList is the list our key would hash to.
    //myhash returns the hash value of k.
    auto& whichList = myHashTable[myhash(k)];
    auto  itr       = whichList.begin();

    //Step through whichList using itr.
    while (itr != whichList.end()) {
        //If true, key has been found.
        if ((*itr).first == k) {
            return true;
        }
        itr++;
    }
    //If here, key was not found.
    return false;
}

/*
    Same logic as contains() but with extra if-condition for returning true
    When matching key found, checks if value matches as well.
*/
template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V>& kv) const {
    auto& whichList = myHashTable[myhash(kv.first)];
    auto itr        = whichList.begin();

    while (itr != whichList.end()) {
        if ((*itr).first == kv.first) {
            //Extra condition, if true then values match.
            if ((*itr).second == kv.second) {
                return true;
            }
        }
        itr++;
    }
    return false;
}

/*
    Gets the hash value of the key and uses it to get corresponding list
    If list size is greater than 0, checks for repeats with same logic as match()
        DOES NOT CALL CONTAINS/MATCH because we may modify the key-value pair if key is
        found but value does not match.
*/
template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V>& kv) {
    auto& whichList = myHashTable[myhash(kv.first)];

    if (whichList.size() != 0) {
        for (auto& currentPair : whichList) {
            if (currentPair.first == kv.first) {
                if (currentPair.second == kv.second) {
                    return false;
                }
                currentPair.second = std::move(kv.second);
                return true;
            }
        }
    }
    //If this line is reached, key wasn't found
    //so we add it to the list

    whichList.push_back(kv);
    currentSize++;

    //Rehash hash table if we have more elements in
    //a single list than indices in the vector.
    if (currentSize > myHashTable.size()) {
        rehash();
    }
    return true;
}

/*
    Same logic as above function, but we move the data instead of copying
*/
template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V>&& kv) {
    auto& whichList = myHashTable[myhash(kv.first)];

    if (whichList.size() != 0) {
        for (auto& currentPair : whichList) {
            if (currentPair.first == kv.first) {
                if (currentPair.second == kv.second) {
                    return false;
                }
                currentPair.second = std::move(kv.second);
                return true;
            }
        }
    }

    //This is where the move happens
    whichList.push_back(std::move(kv));
    if (++currentSize > myHashTable.size()) {
        rehash();
    }
    return true;
}

/*
    Same logic as match(), when the key is found the corresponding
    key-value pair is erased from the list.
    Using an iterator allows us to easily use the provided .erase function
    for lists.
*/
template <typename K, typename V>
bool HashTable<K, V>::remove(const K& k) {
    auto& whichList = myHashTable[myhash(k)];
    auto itr = whichList.begin();

    while (itr != whichList.end()) {
        if ((*itr).first == k) {
            //Where the erasing occurs
            whichList.erase(itr);
            currentSize--;
            return true;
        }
        itr++;
    }
    return false;
}

/*
    Public wrapper of makeEmpty.
    No real purpose since no extra code is given,
    but was part of the assignment.
*/
template <typename K, typename V>
void HashTable<K, V>::clear() {
    makeEmpty();
}

/*
    currentSize getter.
*/
template <typename K, typename V>
size_t HashTable<K, V>::size() const {
    return currentSize;
}

/*
    Gets file named -filename-, expects input in the form of:
    key value
    key value
    key value

    Reads each key,value pair in and uses insert() for each.
*/
template <typename K, typename V>
bool HashTable<K, V>::load(const char* filename) {
    //Get the file
    std::ifstream file;
    std::string filenameString = filename;
    file.open(filenameString);

    //To-be-used for temporarily storing each pair
    K currentK;
    V currentV;

	if(file.is_open()){	
 	   std::pair <K, V> currentKV;
 	   while (file >> currentK) {
 	       if (file >> currentV) {
 	           currentKV.first  = currentK;
 	           currentKV.second = currentV;
 	           insert(currentKV);
 	       }
 	   }
	    return true;
	}
    return false;
}

/*
    We are stepping through each Hashtable element and outputting each as
    n Key Value
    where n is the pair's place in the hash table (from 1 to currentSize)
*/
template <typename K, typename V>
void HashTable<K, V>::dump() const {
    std::cout << std::endl;
    std::pair<K, V> currentKV;

    int n = 0;

    while (n < myHashTable.size()) {
        std::cout << "v[" << n << "]" << ": ";
        auto currentList = myHashTable[n];

        while (!currentList.empty()) {
            currentKV = currentList.back();
            currentList.pop_back();
            std::cout << currentKV.first;
            std::cout << " ";
            std::cout << currentKV.second;

            if (!currentList.empty()) {
                std::cout << ":";
            }
        }
        std::cout << std::endl;
        n++;
    }
}

/*
    Identical to console output dump(), but outputting to the sent in file
    instead of console.
*/
template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char* filename) const {
    std::ofstream file;
    std::string filenameString = filename;
    file.open(filename);
    
    int n = 0;
    std::pair<K, V> currentKV;
    while (n < myHashTable.size()) {
        auto currentList = myHashTable[n];
        while (!currentList.empty()) {
            currentKV = currentList.back();
            currentList.pop_back();
            file << currentKV.first;
            file << " ";
            file << currentKV.second;
            file << std::endl;
        }
        n++;
    }
    return true;
}

/*
    Runs .clear on each list in the vector.
*/
template <typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (auto& thisList : myHashTable) {
        thisList.clear();
    }
    currentSize = 0;
}

/*
    Creates new HashTable object with twice the size
    sent in to constructor, then runs insert() on it with
    each value from the current HashTable, 
*/
template <typename K, typename V>
void HashTable<K, V>::rehash() {
    HashTable<K, V> newTable(myHashTable.size() * 2);

    int n;
    n = 0;

    while (n < myHashTable.size()) {
        if (!myHashTable[n].empty()) {

            auto& currentList = myHashTable[n];

            if (currentList.size() >= 1) {
                for (auto& x : currentList) {
                    newTable.insert(x);
                }
            }
        }

        n++;
    }

    //No need for a deep copy implementation, handled by vector class which
    //deep copies each element (the lists) as well.
    myHashTable = newTable;

}






//*****FUNCTIONS GIVEN IN-CLASS FOR HASHING*******


template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K& k) const {
    static std::hash<K> hf;
    return hf(k) % myHashTable.size();
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
        std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector <unsigned long> v(n + 1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
            return n;
        --n;
    }

    return 2;
}


//Sets all prime number indexes to 1. Called by method prime_below(n)  
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for (i = 2; i * i < n; ++i)
    {
        if (vprimes[i] == 1)
            for (j = i + i; j < n; j += i)
                vprimes[j] = 0;
    }
}


