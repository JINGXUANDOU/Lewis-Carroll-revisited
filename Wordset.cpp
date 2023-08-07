#include "Wordset.hpp"
#include <string>
#include <iostream>
#include <map>

using namespace std;

// returns s, as a number in the given base, mod the given modulus
unsigned polynomialHashFunction(const std::string & s, unsigned base, unsigned mod)
{
    const string letters = "abcdefghijklmnopqrstuvwxyz";
    map<char, unsigned> mapping;
    
    for (unsigned i = 0; i < 26; ++i)
        mapping[letters[i]] = i + 1;

    unsigned s_value = 0;

    for(unsigned i = 0; i < s.size(); ++i)
        s_value = s_value * base + mapping.at(s[i]);
    
    return (s_value - mod * (s_value / mod));
}




WordSet::WordSet(unsigned initialCapacity, unsigned evictionThreshold)
{
    T1 = new string[initialCapacity];
    T2 = new string[initialCapacity];
    capacity = initialCapacity;
    evict_threshold = evictionThreshold;
}

WordSet::~WordSet()
{
    delete[] T1;
    delete[] T2;
}

void WordSet::insert(const std::string & s)
{
    while (true){
        string s1 = s;
        unsigned s_value1 = polynomialHashFunction(s1, BASE_H1, capacity);
        unsigned s_value2 = polynomialHashFunction(s1, BASE_H2, capacity);

        if (contains(s))
            return;

        for (unsigned i = 0; i < evict_threshold; i++){
            s_value1 = polynomialHashFunction(s1, BASE_H1, capacity);
            if (T1[s_value1].empty()){
                T1[s_value1] = s1;
                count++;
                return;
            }
            
            swap(s1, T1[s_value1]);
            s_value2 = polynomialHashFunction(s1, BASE_H2, capacity);

            if (T2[s_value2].empty()){
                T2[s_value2] = s1;
                count++;
                return;
            }
            
            swap(s1, T2[s_value2]);

        }
        resize();
    }
    //insert(s1);
}

bool WordSet::contains(const std::string & s) const
{
    unsigned s_value1 = polynomialHashFunction(s, BASE_H1, capacity);
    if (T1[s_value1] == s)
        return true;

    unsigned s_value2 = polynomialHashFunction(s, BASE_H2, capacity);
    if (T2[s_value2] == s){
        return true;
    }

	return false;  // stub, not correct.
}

// return how many distinct strings are in the set
unsigned WordSet::getCount() const
{
	return count;
}

// return how large the underlying array is.
unsigned WordSet::getCapacity() const
{
	return capacity;
}

unsigned WordSet::calu_size(){
    unsigned n = 2 * capacity;
    while (true){
        if (n % 2 == 0 || n % 3 == 0){
            n++;
            continue;
        }
    
        for (unsigned i = 5; i * i<=n; i = i + 6){
            if (n % i == 0 || n % (i + 2) == 0){
                n++;
                continue;
            }
        }
    
        return n;        
    }
}

void WordSet::resize(){
	unsigned new_capacity = calu_size();
	 
	string* new_T1 = new string[new_capacity];
	for (unsigned i = 0; i < capacity; i++) {
        if (T1[i].empty() == false){
		    unsigned h1 = polynomialHashFunction(T1[i], BASE_H1, new_capacity);
		    new_T1[h1] = T1[i];
        }
	}
	delete[] T1;
    T1 = new_T1;
	
	string* new_T2 = new string[new_capacity];
	for (unsigned i = 0; i < capacity; i++) {
        if (T2[i].empty() == false){
		    unsigned h2 = polynomialHashFunction(T2[i], BASE_H2, new_capacity);
		    new_T2[h2] = T2[i];
        }
	}
	delete[] T2;
    T2 = new_T2;
	 
	capacity = new_capacity;
}