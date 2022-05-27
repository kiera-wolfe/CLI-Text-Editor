//ECCommand.cpp

//
//  ECCommand.cpp
//  
//
//

#include "ECCommandText.h"
#include <iostream>
using namespace std;

// ******************************************************
// Implement command history

ECCommandHistory :: ECCommandHistory() {}

ECCommandHistory :: ~ECCommandHistory()
{
  cmdHistory.clear();
}


bool ECCommandHistory::Undo()
{
    if(posTracker < 0)
        return false;
    cmdHistory[posTracker]->UnExecute();
    posTracker--;
    return true;
}

bool ECCommandHistory::Redo()
{
    if(posTracker == cmdHistory.size() - 1)
        return false;
    posTracker++;
    cmdHistory[posTracker]->Execute();
    return true;
}

void ECCommandHistory::ExecuteCmd( ECCommand *pCmd )
{
    if(posTracker < cmdHistory.size() - 1)
    {
        int size = cmdHistory.size();
        for(int i=posTracker + 1; i<size; i++)
        {
            cmdHistory.pop_back();
        }
    }
    posTracker++;
    cmdHistory.push_back(pCmd);
    pCmd->Execute();
}
