#ifndef RECCHECK
// For debugging
#include <iostream>
// For remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void recurse(string& current, const string& fixed, string floating, const set<string>& dict, set<string>& results, int index); // generate all possible valid strings
bool validPrefix(const string& current, const set<string>& dict, int index);

// Definition of primary wordle function
// creates a results set, creates a working string equal to in
// calls recursive helper starting at index 0, returns results
set<string> wordle(
    const string& in,
    const string& floating,
    const set<string>& dict)
{
    // Add your code here
    set<string> results;
    string current = in;
    recurse(current, in, floating, dict, results, 0);

    return results;
}

// Define any helper functions here

// fixed letters stay fixed, floating letters appear at least once
// remaining slots are any letters a-z, and word is valid (in dict)
// constantly update floating string for pruning purposes
void recurse(string& current, const string& fixed, string floating, const set<string>& dict, set<string>& results, int index) {
    // pruning
    // if we have fewer slots left than floating letters that need to appear
    // potential word is INVALID
    int remainingSlots = (int)current.size() - index;
    if (remainingSlots < (int)floating.size()) {
        return;
    }

    // base case, if the current index equal to size, we've filled all positions + word is complete
    if (index == (int)current.size()) {
        if ((floating.empty()) && (dict.find(current) != dict.end())) { // check dictionary
            results.insert(current);
        }
        return;
    }

    // fixed slot (green)
    if (fixed[index] != '-') {
        char letter = fixed[index];
        // make copies of the floating letters
        // (seperate branches, seperate ways we deal with the floating letters)
        string nextFloating = floating;
        // was fixed letter listed as a floating letter?
        size_t isFloating = nextFloating.find(letter);
        if (isFloating != string::npos) {
            nextFloating.erase(isFloating, 1);
        }
        // place fixed letter
        char old = current[index];
        current[index] = letter;
        recurse(current, fixed, nextFloating, dict, results, index+1);
        current[index] = old;
        return; // only one branch
    }

    // floating letter case: if remaining slots equal floating string size,
    // ALL remaining slots MUST be filled w floating letters
    // for instructor3 case that takes five billion years
    if (remainingSlots == (int)floating.size()) {
        for (size_t i = 0; i < floating.size(); i++) {
            char letter = floating[i];
            string nextFloating = floating;
            nextFloating.erase(i, 1);
            char old = current[index];
            current[index] = letter;
            recurse(current, fixed, nextFloating, dict, results, index+1);
            current[index] = old; // backtracking undoes any modifications made
        }
        return; // only one branch
    }

    // open slot (gray)
    // try all 26 letters
    for (char letter = 'a'; letter <= 'z'; letter++) {
        string nextFloating = floating;
        // did we place a floating letter?
        size_t isFloating = nextFloating.find(letter);
        if (isFloating != string::npos) {
            nextFloating.erase(isFloating, 1);
        }
        // pruning
        int nextRemaining = current.size() - (index + 1);
        if (nextRemaining < (int)nextFloating.size()) {
            continue;  // impossible, skip early
        }
        char old = current[index];
        current[index] = letter;
        // prefix pruning
        // after placing letter, check if partial word
        // is a prefix for a REAL dict word -> if not, prune
        if (!validPrefix(current, dict, index+1)) {
            current[index] = old;
            continue; // skip branch
        }
        recurse(current, fixed, nextFloating, dict, results, index+1);
        current[index] = old;
    }
}

bool validPrefix(const string& current, const set<string>& dict, int index) {
    string prefix = current.substr(0, index); // get prefix
    auto it = dict.lower_bound(prefix); // find first dictionary word with prefix
    if (it == dict.end()) { // no word has prefix
        return false;
    }
    return it->compare(0, prefix.size(), prefix) == 0;
}