// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.
template<typename KeyType, typename ValueType>
class MyHash
{
public:
	MyHash(double maxLoadFactor = 0.5);
	~MyHash();
	void reset();
	void associate(const KeyType& key, const ValueType& value);
	int getNumItems() const;
	double getLoadFactor() const;

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyHash(const MyHash&) = delete;
	MyHash& operator=(const MyHash&) = delete;

private:
	//Bucket type which can be used as linked list
	struct Bucket {
		KeyType m_key;
		ValueType m_value;
		Bucket* next;
	};
	//type pointer to pointer to bucket to create dynamic array of Bucket pointers which are essentially
	//a linked list of buckets
	Bucket **m_hashTable;
	double m_maxLoadFactor;//maximum load factor allowed
	double m_numItems;//number of items in hash table
	int m_size;// size of hash table
};

template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
	//assign load factor according to requirements
	if (maxLoadFactor <= 0)
		m_maxLoadFactor = 0.5;
	else if (maxLoadFactor > 2.0)
		m_maxLoadFactor = 2.0;
	else m_maxLoadFactor = maxLoadFactor;
	m_numItems = 0;
	//dynamically allocate array of Bucket pointers
	m_hashTable = new Bucket*[100];
	//assign each pointer to null pointer to start
	for (int i = 0; i < 100; i++)
		m_hashTable[i] = nullptr;
	m_size = 100;
}

template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
	//delete each bucket
	for (int i = 0; i < m_size; i++)
		delete m_hashTable[i];
	delete[] m_hashTable;
}


template <typename KeyType, typename ValueType>
inline
int MyHash<KeyType, ValueType>::getNumItems() const { return m_numItems; } //return number of items in hash table}


template <typename KeyType, typename ValueType>
inline
double MyHash<KeyType, ValueType>::getLoadFactor() const { return (m_numItems / m_size); } // return load factor 

template <typename KeyType, typename ValueType>
inline
void MyHash<KeyType, ValueType>::reset()
{
	//delete each bucket
	for (int i = 0; i < m_size; i++)
		delete m_hashTable[i];
	delete [] m_hashTable; 
	//assign new array of 100 bucket pointers
	m_hashTable = new Bucket*[100];
	//set bucket pointer values to null
	for (int i = 0; i < 100; i++)
		m_hashTable[i] = nullptr;
	m_size = 100; // reset size and number of items
	m_numItems = 0;
}

template <typename KeyType, typename ValueType>
inline
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int hasher(const KeyType& k);  // prototype
	unsigned int h = hasher(key);

	//resize table if its too small
	if (((m_numItems + 1.0) / m_size) > m_maxLoadFactor)
	{
		const int new_size = m_size * 2;
		Bucket** temp = new Bucket*[new_size];
		for (int i = 0; i < m_size; i++)
		{
			temp[i] = m_hashTable[i];
		}
		for (int i = m_size; i < new_size; i++)
		{
			temp[i] = nullptr;
		}
		delete[] m_hashTable;
		m_hashTable = temp;
		m_size = new_size;
	}
	//add item if linked list is empty
	if (m_hashTable[h % m_size] == nullptr) {
		m_hashTable[h%m_size] = new Bucket;
		m_hashTable[h%m_size]->m_value = value;
		m_hashTable[h%m_size]->m_key = key;
		m_hashTable[h%m_size]->next = nullptr;
		m_numItems++;
	}
	else {
		//check if value needs to be updated not added
		Bucket* p = m_hashTable[h % m_size];
		while (p != nullptr) {
			if (p->m_key == key) {
				p->m_value = value;
				return;
			}
			p = p->next;
		}
		//add item to linked list
		Bucket* add = new Bucket;
		add->m_value = value;
		add->m_key = key;
		add->next = m_hashTable[h % m_size];
		m_hashTable[h % m_size] = add;
		m_numItems++;

	}

}

template <typename KeyType, typename ValueType>
inline
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{

	unsigned int hasher(const KeyType& k);  // prototype
	unsigned int h = hasher(key);
	Bucket* p = m_hashTable[h % m_size];
	//if list is empty
	if (p == nullptr)
		return nullptr;
	//iterate through list to find value 
	while (p != nullptr) {
		if (p->m_key == key) {
			return &(p->m_value); //return adress of value
		}
		p = p->next;
	}
	return nullptr;
}
