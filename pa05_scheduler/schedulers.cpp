#include "schedulers.h"

//Round Robin scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the first process on that list, if available (i.e., if the list has members)
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // if the process isn't done, add it to the back of the ready queue
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}


int ShortestProcessNext(const int& curTime, const vector<Process>& procList)
{
    //processed id to return
    int idx = -1;
    int curMaxTime = INT32_MAX;

    // only check processes that are not finished
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(curTime >= procList[i].startTime && !procList[i].isDone)
        {
            // pre-emption removal
            if(procList[i].timeScheduled > 0)
            {
                idx = i;
                break;
            }
            
            // choose shortest time among scheduled processes
            if (procList[i].totalTimeNeeded < curMaxTime)
            {   
                curMaxTime = procList[i].totalTimeNeeded;
                idx = i;
            }
        }
    }

    return idx;
}

int ShortestRemainingTime(const int& curTime, const vector<Process>& procList)
{
    //processed id to return
    int idx = -1;
    int remainingTime = INT32_MAX;

    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        // only check processes that are not finished
        if(curTime >= procList[i].startTime && !procList[i].isDone)
        {
            // shose shortest remaining time on all currently schedualed processes
            if (procList[i].totalTimeNeeded - procList[i].timeScheduled < remainingTime)
            {   
                remainingTime = procList[i].totalTimeNeeded - procList[i].timeScheduled;
                idx = i;
            }
        }
    }

    return idx;
}

int HighestResponseRatioNext(const int& curTime, const vector<Process>& procList)
{
    //processed id to return
    int idx = -1;
    float curIndexResponseRatio = -1.0;
    float responseRatio = -1;
    
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        // only check processes that are not finished
        if(curTime >= procList[i].startTime && !procList[i].isDone)
        {
            // pre-emption removal
            if(procList[i].timeScheduled > 0)
            {
                idx = i;
                break;
            }

            curIndexResponseRatio = (((curTime - procList[i].startTime) + procList[i].totalTimeNeeded))/ static_cast<float>(procList[i].totalTimeNeeded);

            if (curIndexResponseRatio > responseRatio)
            {   
                responseRatio = curIndexResponseRatio;
                idx = i;
            }
        }
    }

    return idx;
}

float round(float var) 
{ 
    float value = (int)(var * 100 + .5); 
    return (float)value / 100; 
} 

void StatProcessing(const vector<Process>& procList)
{
    int calcTr = 0;
    float avgTurnaroundTime = 0;
    float avgNormalTurnaroundTime = 0;
    vector<int> turnaroundTime;
    vector<float> normalizedTurnaroundTime;


    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {   
        calcTr = procList[i].timeFinished - procList[i].startTime + 1;
        turnaroundTime.push_back(calcTr);
        normalizedTurnaroundTime.push_back(static_cast<float>(calcTr)/procList[i].totalTimeNeeded);
    }

    cout << "Process: ";
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        cout << procList[i].id << " ";
    }
    cout << endl;

    cout << "Finish Time: ";
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        cout << procList[i].timeFinished + 1 << " ";
    }
    cout << endl;

    cout << "Turnaround Time (Tr): ";
    for(int i = 0, i_end = turnaroundTime.size(); i < i_end; ++i)
    {
        cout << turnaroundTime[i] << " ";
    }
    cout << endl;

    cout << "Normalized Turnaround Time (Tr/Ts): ";
    for(int i = 0, i_end = normalizedTurnaroundTime.size(); i < i_end; ++i)
    {
        cout << round(normalizedTurnaroundTime[i]) << " ";
    }
    cout << endl;

    cout << "Avrage Turnaround Time: ";
    for(int i = 0, i_end = turnaroundTime.size(); i < i_end; ++i)
    {
        avgTurnaroundTime += turnaroundTime[i];
    }
    cout << round(static_cast<float>(avgTurnaroundTime) / turnaroundTime.size()) << endl;

    cout << "Avrage Normalized Turnaround Time: ";
    for(int i = 0, i_end = normalizedTurnaroundTime.size(); i < i_end; ++i)
    {
        avgNormalTurnaroundTime += normalizedTurnaroundTime[i];
    }
    cout << round(static_cast<float>(avgNormalTurnaroundTime) / normalizedTurnaroundTime.size()) << endl;
    
}