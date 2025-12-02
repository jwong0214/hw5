#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Add prototypes for any helper functions here
// sched[nDays][dailyNeed]
bool backtrack (const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<size_t>& shiftsUsed, size_t slotIndex);

// Add your implementation of schedule() and other helper functions here
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    // sched n x d vector of vectors with d workers scheduled to work on each n days
    // avail n x k vector of vectors with avaliability of k workers for each n days
    // prepare output matrix nDays x dailyNeed
    size_t nDays = avail.size();
    size_t kWorkers = avail[0].size();
    sched.resize(nDays, vector<Worker_T>(dailyNeed, INVALID_ID));

    // track number of shifts assigned to each worker
    vector<size_t> shiftsUsed(kWorkers, 0);

    // start backtracking at slot 0 (day=0, position=0)
    return backtrack(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0);
}

bool backtrack(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, vector<size_t>& shiftsUsed, size_t slotIndex) {
    size_t nDays = avail.size();
    size_t kWorkers = avail[0].size();
    size_t totalSlots = nDays * dailyNeed;

    // base case: when positions in schedule have been filled
    if (slotIndex == totalSlots) {
        return true;
    }

    // convert matrix into flat indexes of the day and position of that day
    size_t day = slotIndex / dailyNeed;
    size_t pos = slotIndex % dailyNeed;

    // try assigning each worker (DFS)
    for (size_t w = 0; w < kWorkers; w++) {
        // pruning (kinda)
        // worker is avaliable on this day + shifts worked doesn't exceed max shifts
        if ((!avail[day][w]) || (shiftsUsed[w] >= maxShifts)) {
            continue;
        }
        // worker is not already scheduled on the day
        if (find(sched[day].begin(), sched[day].end(), w) != sched[day].end()) {
            continue;
        }
        // if passes prev if statements, then choose worker w
        sched[day][pos] = w;
        shiftsUsed[w]++;
        
        // recurse to the next slot
        if (backtrack(avail, dailyNeed, maxShifts, sched, shiftsUsed, slotIndex+1)) {
            return true; // found solution
        }

        // if there's no further solution when worker w is assigned to the day, backtrack
        sched[day][pos] = INVALID_ID;
        shiftsUsed[w]--;
    }

    return false; // no workers AT ALL fit so we FAIL
}