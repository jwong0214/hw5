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
void recurse(string in, string floating, const set<string>& dict, set<string>& results, size_t index); // generate all possible valid strings

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
  recurse(in, floating, dict, results, 0);
  return results;
}

// Define any helper functions here

// fixed letters stay fixed, floating letters appear at least once
// remaining slots are any letters a-z, and word is valid (in dict)
// constantly update floating string for pruning purposes
void recurse(string in, string floating, const set<string>& dict, set<string>& results, size_t index) {
  // base case, if the current index equal to size, we've filled all positions + word is complete
  if (index == in.size()) {
    if ((floating.empty()) && (dict.count(in) == 1)) { // check dictionary
      results.insert(in);
    }
    return;
  }

  // pruning
  // if we have fewer slots left than floating letters that need to appear
  // potential word is INVALID and we prune
  size_t remainingSlots = 0;
  for (size_t i = index; i < in.size(); i++) {
    if (in[i] == '-') {
      remainingSlots++;
    }
  }
  if (remainingSlots < floating.size()) {
    return;
  }

  // fixed slot (green)
  if (in[index] != '-') {
    recurse(in, floating, dict, results, index+1);
  // open slot (gray), try all 26 characters
  } else {
    for (char letter = 'a'; letter <= 'z'; letter++) {
      in[index] = letter;

      // is character in the floating string?
      // but first, make a copy of the floating string
      // so seperate branches don't affect other branches, 
      // cuz they have seperate ways of dealing with the floating letters
      string nextFloating = floating;
      size_t isFloating = nextFloating.find(letter);
      if (isFloating != string::npos) {
        nextFloating.erase(isFloating, 1);
      }
      recurse(in, nextFloating, dict, results, index+1); // go to next character
      in[index] = '-'; // backtracking undoes any modifications made
    }
  }
}