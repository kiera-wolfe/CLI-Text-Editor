//ECCommand.h

//
//  ECCommand.h
//  
//
//

#ifndef ECCommandText_h
#define ECCommandText_h

#include <vector>
using namespace std;

// ******************************************************
// Implement command design pattern

class ECCommand
{
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};

// ******************************************************
// Implement command history

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    bool Undo();
    bool Redo();
    void ExecuteCmd( ECCommand *pCmd );
    
private:
    vector<ECCommand*> cmdHistory;
    int posTracker = -1;
};


#endif /* ECCommand_h */
