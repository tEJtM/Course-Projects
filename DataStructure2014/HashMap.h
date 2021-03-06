/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
	public:
    class Entry
    {
	public:
        K key;
        V value;
        Entry(const K &k, const V &v)
        {
            key = k;
            value = v;
        }

        K getKey() const
        {
            return key;
        }

        V getValue() const
        {
            return value;
        }
    };
    static const int RANGE = 99971;
    LinkedList<Entry> *list;
    int siz;
	public:
    class Iterator
    {
	    HashMap<K, V, H> * HM;
	    int hashP, visited;
	    typename LinkedList<Entry>::Iterator itr;
public:
        /**
         * TODO Returns true if the iteration has more elements.
         */
        Iterator(HashMap<K, V, H> * HM) : HM(HM), itr(HM->list[0].iterator()) {visited = 0; hashP = 0;}
        bool hasNext()
	{
		return visited < HM->siz;
	}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next()
	{
		if(visited == HM->siz) throw ElementNotExist();
		else
		{
			visited ++;
			if(itr.hasNext()) return itr.next();
			else
			{
				for(hashP++; !HM->list[hashP].size(); hashP++);
				new (&itr) typename LinkedList<Entry>::Iterator(HM->list[hashP].iterator());
				return itr.next();
			}
		}
	}
    };

    /**
     * TODO Constructs an empty hash map.
     */
    HashMap()
    {
	    siz = 0;
	    list = new LinkedList<Entry>[RANGE];
    }

    /**
     * TODO Destructor
     */
    ~HashMap()
    {

    }

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x)
    {
	    for(int i = 0; i < RANGE; i++) list[i] = x.list[i];
	    siz = x.siz;
    }

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x)
    {
	    list = new LinkedList<Entry>[RANGE];
	    *this = x;
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator()
    {
	    return Iterator(this);
    }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear()
    {
	    siz = 0;
	    for(int i = 0; i < RANGE; i++) list[i].clear();
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    static int hash(int x)
    {
	    return (x % RANGE + RANGE) % RANGE;
    }
    bool containsKey(const K &key)
    {
	    for(typename LinkedList<Entry>::Iterator itr = list[hash(H().hashCode(key))].iterator(); itr.hasNext(); )
		    if(itr.next().key == key) return true;
	    return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value)
    {
	    for(Iterator itr = iterator(); itr.hasNext(); )
		    if(itr.next().value == value)
			    return true;
	    return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key)
    {
	    for(typename LinkedList<Entry>::Iterator itr = list[hash(H().hashCode(key))].iterator(); itr.hasNext(); )
	    {
		    const Entry & E = itr.next();
		    if(E.key == key) return E.value;
	    }
	    throw ElementNotExist();
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const
    {
	    return !siz;
    }

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value)
    {
	    for(typename LinkedList<Entry>::Iterator itr = list[hash(H().hashCode(key))].iterator(); itr.hasNext(); )
		    if(itr.next().key == key)
		    {
	//		    cout << key << endl;
			    itr.remove();
			    siz--;
			    break;
		    }
	    //cout << key << endl;
	    siz++;
	    list[hash(H().hashCode(key))].add(Entry(key, value));
	    
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key)
    {
	    for(typename LinkedList<Entry>::Iterator itr = list[hash(H().hashCode(key))].iterator(); itr.hasNext(); )
		    if(itr.next().key == key)
		    {
			    itr.remove();
			    siz--;
			    return;
		    }
	    throw ElementNotExist();
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const
    {
	    return siz;
    }
    friend Iterator;
};

#endif
