//ECTextEditor.h

#ifndef ECTextEditor_h
#define ECTextEditor_h

#include "ECTextViewImp.h"
#include "ECCommandText.h"
#include <string>
#include <vector>

using namespace std;

//Observer *********************************************

class ECEditorObserver : public ECObserver
{
    public:
        ECEditorObserver(ECTextViewImp* view);
        void Update();
    
    private:
        ECTextViewImp* textView;
        vector<string> listRows;
        ECCommandHistory histCmds;
};


//Commands ********************************************

class ECInsertCommand : public ECCommand
{
public:
    ECInsertCommand(vector<string> &d, ECTextViewImp* tv, int px, int py, const char t) : doc(d), textView(tv), pos_x(px), pos_y(py), text(t) {};
    virtual ~ECInsertCommand() {}
    void Execute() override;
    void UnExecute() override;
private:
    vector<string> &doc;
    ECTextViewImp* textView;
    int pos_x, pos_y;
    const char text;
};

class ECRemoveCommand : public ECCommand
{
public:
    ECRemoveCommand(vector<string> &d, ECTextViewImp* tv, int px, int py) : doc(d), textView(tv), pos_x(px), pos_y(py) {};
    virtual ~ECRemoveCommand() {}
    void Execute() override;
    void UnExecute() override;
private:
    vector<string> &doc;
    ECTextViewImp* textView;
    int pos_x, pos_y;
    char text;
    int prevx = doc[pos_y - 1].size();
    bool flag = false;
};

class ECEnterCommand : public ECCommand
{
public:
    ECEnterCommand(vector<string> &d, ECTextViewImp* tv, int px, int py) : doc(d), textView(tv), pos_x(px), pos_y(py) {};
    virtual ~ECEnterCommand() {}
    void Execute() override;
    void UnExecute() override;
private:
    vector<string> &doc;
    ECTextViewImp* textView;
    int pos_x, pos_y;
};


//Key handlers ******************************************

class KeyHandler
{
    public:
        KeyHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : data(d), textView(tv), histCmds(h) {};
        virtual void handle() = 0;
    protected:
        vector<string> &data;
        ECTextViewImp* textView;
        int PKey = textView->GetPressedKey();
        ECCommandHistory& histCmds;
};

class RedoHandler : public KeyHandler
{
    public:
        RedoHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    // private:
    //     KeyHandler* nextHandler = new RedoHandler(data, textView, histCmds);
};

class UndoHandler : public KeyHandler
{
    public:
        UndoHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new RedoHandler(data, textView, histCmds);
};

class EnterHandler : public KeyHandler
{
    public:
        EnterHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new UndoHandler(data, textView, histCmds);
};

class BackspaceHandler : public KeyHandler
{
    public:
        BackspaceHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new EnterHandler(data, textView, histCmds);
};

class TextHandler : public KeyHandler
{
    public:
        TextHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new BackspaceHandler(data, textView, histCmds);
};

class RightArrowHandler : public KeyHandler
{
    public:
        RightArrowHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new TextHandler(data, textView, histCmds);
};

class LeftArrowHandler : public KeyHandler
{
    public:
        LeftArrowHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new RightArrowHandler(data, textView, histCmds);
};

class DownArrowHandler : public KeyHandler
{
    public:
        DownArrowHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new LeftArrowHandler(data, textView, histCmds);
};


class UpArrowHandler : public KeyHandler
{
    public:
        UpArrowHandler(vector<string> &d, ECTextViewImp* tv, ECCommandHistory &h) : KeyHandler(d, tv, h) {};
        void handle();
    private:
        KeyHandler* nextHandler = new DownArrowHandler(data, textView, histCmds);
};


#endif
