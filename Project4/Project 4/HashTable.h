//
//  HashTable.h
//  Project4
//
//  Created by Connor Kenny on 3/5/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#ifndef __Project4__HashTable__
#define __Project4__HashTable__

#include <iostream>
template <typename KeyType,	typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity);
    ~HashTable();
    bool isFull() const;
    bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key, ValueType& value);
    
private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* m_next = nullptr;
        Node* m_prev = nullptr;
        bool m_isPermanent;
        Node* m_nextTouched = nullptr;
        Node* m_prevTouched = nullptr;
    };
    Node** m_hashTable;
    Node* m_recentlyTouched;
    Node* m_lastTouched;
    int m_capacity;
    int m_numBuckets;
    int m_numAssociations;
    
    int getBucket(KeyType key) const;
    
				//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
				//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
};

template <typename KeyType,	typename ValueType>
int HashTable<KeyType, ValueType>::getBucket(KeyType key) const                     // return the bucket to which the key belongs
{
    unsigned int computeHash(KeyType);
    return computeHash(key) % m_numBuckets;
}

template <typename KeyType,	typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)
: m_capacity(capacity), m_numBuckets(numBuckets), m_numAssociations(0), m_recentlyTouched(nullptr), m_lastTouched(nullptr)      // Create empty HashTable
{
    m_hashTable = new Node*[m_numBuckets];
    for (int k = 0; k < m_numBuckets; k++)
        m_hashTable[k] = nullptr;
}

template <typename KeyType,	typename ValueType>
HashTable<KeyType, ValueType>::~HashTable()                                          // Deletes each item
{
    for (int k = 0; k < m_numBuckets; k++)
    {
        Node* search = m_hashTable[k];
        while(search != nullptr)
        {
            Node* temp = search->m_next;
            delete search;                                      // delete each node in the list
            search = temp;
        }
    }
    delete [] m_hashTable;                                      // delete the entire Hash table
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const                                  // Returns whether or not the HashTable is full
{
    return m_numAssociations == m_capacity;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent)         // Assign a key/value pair
{
    ValueType dummyValue;
    int bucketNum = getBucket(key);
    Node* search = m_hashTable[bucketNum];
    if (!get(key, dummyValue))
    {
        if (isFull())
            return false;
        else
        {
            Node* itemToBeAdded = new Node;     // Create a new node
            itemToBeAdded->m_next = nullptr;
            itemToBeAdded->m_prev = nullptr;
            itemToBeAdded->m_key = key;
            itemToBeAdded->m_value = value;
            itemToBeAdded->m_isPermanent = permanent;
            
            m_hashTable[bucketNum] = itemToBeAdded;            // Place node in correct spot

            m_hashTable[bucketNum]->m_next = search;

            if (search != nullptr)                      // Adjust the item that was at the front of the list
                search->m_prev = itemToBeAdded;
            
            m_numAssociations++;
        }
    }
    else
    {
        while (search->m_key != key)       // Find the correct node
        {
            search = search->m_next;
        }
        search->m_value = value;
    }
    touch(key);                                 // Make it the most recently touched node
    return true;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType& key, ValueType& value) const
{
    Node* item = m_hashTable[getBucket(key)];               // Go to correct bucket
    while (item != nullptr)
    {
        if (item->m_key == key)
        {
            value = item->m_value;                          // If found node, set value to the node's value
            return true;
        }
        item = item->m_next;
    }
    return false;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key)
{
    Node* item = m_hashTable[getBucket(key)];
    if (item->m_isPermanent)                       // If not permanent, adjust pointers accordingly
        return false;
    
    while (item != nullptr)
    {
        if (item->m_key == key)                 // Find the correct node
            break;
        item = item->m_next;
    }
    
    if (item == nullptr)
        return false;
    
    if (m_lastTouched == item)
        m_lastTouched = m_lastTouched->m_nextTouched;

    if (item->m_prevTouched != nullptr)
        item->m_prevTouched->m_nextTouched = item->m_nextTouched;       // takes item out of linked list to be put at head
    if (item->m_nextTouched != nullptr)
        item->m_nextTouched->m_prevTouched = item->m_prevTouched;
        
    item->m_nextTouched = nullptr;
    item->m_prevTouched = m_recentlyTouched;
    
    
    if (m_lastTouched == nullptr)
        m_lastTouched = item;
        
    
    if (m_recentlyTouched != nullptr)
        m_recentlyTouched->m_nextTouched = item;
    m_recentlyTouched = item;                       // put item at head of recently touched list
        
    return true;
}


template <typename KeyType,	typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value)
{
    if (m_numAssociations == 0 || m_recentlyTouched == nullptr)
        return false;
    Node* search = m_lastTouched;               // start at most recently touched item
    
    key = search->m_key;
    value = search->m_value;                        // set key and value correctly
        
    if (search->m_nextTouched != nullptr)                   // set pointers
        search->m_nextTouched->m_prevTouched = nullptr;
    
    if (search->m_next != nullptr)
        search->m_next->m_prev = search->m_prev;
    
    if (search->m_prev != nullptr)
        search->m_prev->m_next = search->m_next;
    
    bool deletedLastNode = false;
    if (search == m_recentlyTouched)                // discarded last one because it still is m_recentlyTouched
        deletedLastNode = true;
    
    m_numAssociations--;
    
    if (m_lastTouched != nullptr)                       // if not the last item, adjust m_lastTouched
        m_lastTouched = m_lastTouched->m_nextTouched;
    
    delete search;
    m_hashTable[getBucket(key)] = nullptr;        // assign deleted node to nullptr
    
    if (deletedLastNode)
        m_recentlyTouched = nullptr;
    
    return true;
}

#endif /* defined(__Project4__HashTable__) */