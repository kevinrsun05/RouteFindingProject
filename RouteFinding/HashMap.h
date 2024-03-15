//
//  HashMap.h
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#ifndef HashMap_h
#define HashMap_h

#include <string>
#include <vector>
#include <list>
using namespace std;

template <typename T>
class HashMap
{
  public:
    HashMap(double max_load = 0.75); // constructor
    ~HashMap(); // destructor; deletes all of the items in the hashmap
    int size() const; // return the number of associations in the hashmap
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const T& value);
    // Defines the bracket operator for HashMap, so you can use your map like this:
    // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in
    // the hashmap and map it to the default value of type T (0 for builtin types).
    // It returns a reference to the newly created value in the map.
    T& operator[](const std::string& key);
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value within the map.
    const T* find(const std::string& key) const;
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
  private:
    struct nodeType {
        string str;
        T value;
    };
    
    HashMap(const HashMap&);
    HashMap& operator=(const HashMap&);
    
    void makeNewVector();
    double m_maxLoad;
    int m_associations;
    vector<list<nodeType>> m_hashMap;
};

template<typename T>
HashMap<T>::HashMap(double max_load)
: m_maxLoad(max_load), m_associations(0), m_hashMap(10)
{}

template<typename T>
HashMap<T>::~HashMap() {}

template<typename T>
int HashMap<T>::size() const {
    return m_associations;
}

template <typename T>
void HashMap<T>::makeNewVector() {
    vector<list<nodeType>> temp(m_hashMap.size() * 2);
    for (int i = 0; i < m_hashMap.size(); i++) {
        typename list<nodeType>::const_iterator it;
        for (it = m_hashMap[i].begin(); it != m_hashMap[i].end(); it++) {
            size_t h = hash<string>()((*it).str) % temp.size();
            temp[h].push_back(*it);
        }
    }
    m_hashMap = temp;
}

template<typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
    if (find(key) != nullptr) {
        *find(key) = value;
        return;
    }

    if ((m_associations + 1.0) / m_hashMap.size() > m_maxLoad) {
        makeNewVector();
    }
    
    
    nodeType n = nodeType();
    n.str = key;
    n.value = value;
    size_t h = hash<string>()(key) % m_hashMap.size();
    m_hashMap[h].push_back(n);
    m_associations++;
}

template<typename T>
T& HashMap<T>::operator[](const std::string& key) {
    double* num = find(key);
    if (num != nullptr) {
        return *num;
    }
    insert(key, T()); //should be T()
    return *find(key);
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const {
    size_t i = hash<string>()(key) % m_hashMap.size();
    typename list<nodeType>::const_iterator it;
    for (it = m_hashMap[i].begin(); it != m_hashMap[i].end(); it++) {
        if ((*it).str == key) {
            return &((*it).value);
        }
    }
    return nullptr;
}



#endif /* HashMap_h */
