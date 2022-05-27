//ECTextEditor.cpp 

#include "ECTextEditor.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//Oserver ****************************************************

ECEditorObserver::ECEditorObserver(ECTextViewImp* view) : 
                    textView(view)
{
    textView->Attach(this);
    listRows.push_back("");
    textView->Show();
}
        
void ECEditorObserver::Update()
{
    vector<string> &dataref = listRows;
    KeyHandler* HandlingChain = new UpArrowHandler(dataref, textView, histCmds);
    HandlingChain->handle();
    textView->InitRows();
    textView->AddRow(" ");
    for(string s : listRows)
    {
        textView->AddRow(s);
    }
}


// Commands ************************************************

void ECInsertCommand::Execute()
{
    doc[pos_y].insert(doc[pos_y].begin() + pos_x, text);
    textView->SetCursorX(pos_x + 1);
}

void ECInsertCommand::UnExecute()
{
    doc[pos_y].erase(doc[pos_y].begin() + pos_x);
    textView->SetCursorX(pos_x);
}

void ECRemoveCommand::Execute()
{
    if(pos_x == 0 && pos_y != 0)
    {
        flag = true;
        textView->SetCursorX(doc[pos_y - 1].size());
        textView->SetCursorY(pos_y - 1);
        doc[pos_y - 1].append(doc[pos_y]);
        doc.erase(doc.begin() + pos_y);
    }
    else
    {
        text = doc[pos_y][pos_x - 1];
        doc[pos_y].erase(doc[pos_y].begin() + pos_x - 1);
        textView->SetCursorX(textView->GetCursorX() - 1);
    }
}

void ECRemoveCommand::UnExecute()
{
    if(flag)
    {
        textView->SetCursorX(0);
        textView->SetCursorY(textView->GetCursorY() + 1);
        string newline = doc[pos_y - 1].substr(prevx);
        doc.insert(doc.begin() + pos_y, newline);
        doc[pos_y - 1] = doc[pos_y - 1].substr(0, prevx);
    }
    else
    {
        doc[pos_y].insert(doc[pos_y].begin() + pos_x - 1, text);
        textView->SetCursorX(pos_x);
    }
}

void ECEnterCommand::Execute()
{
    textView->SetCursorX(0);
    textView->SetCursorY(textView->GetCursorY() + 1);
    string newline = doc[pos_y].substr(pos_x);
    doc.insert(doc.begin() + pos_y + 1, newline);
    doc[pos_y] = doc[pos_y].substr(0, pos_x);
}

void ECEnterCommand::UnExecute()
{
    textView->SetCursorX(doc[pos_y].size());
    textView->SetCursorY(pos_y);
    doc[pos_y].append(doc[pos_y + 1]);
    doc.erase(doc.begin() + pos_y + 1);
}


// Key handlers ********************************************


void DownArrowHandler::handle()
{
    if(PKey == ARROW_DOWN)
    {
        int y_val = textView->GetCursorY() + 1;
        int x_val = textView->GetCursorX();
        if(data.size() == 0)
        {
            y_val = 0;
        }
        else if(y_val >= data.size())
        {
            y_val = data.size() - 1;
            x_val = data[y_val].size();
        }
        else if(x_val > data[y_val].size())
        {
            x_val = data[y_val].size();
        }
        textView->SetCursorX(x_val);
        textView->SetCursorY(y_val);
    }
    else
    {
        nextHandler->handle();
    }
}



void UpArrowHandler::handle()
{
    if(PKey == ARROW_UP)
    {
        int y_val = textView->GetCursorY() - 1;
        int x_val = textView->GetCursorX();
        if(data.size() == 0)
        {
            y_val = 0;
        }
        else if(y_val < 0)
        {
            y_val = 0;
            x_val = 0;
        }
        else if(x_val > data[y_val].size())
        {
            x_val = data[y_val].size();
        }
        textView->SetCursorX(x_val);
        textView->SetCursorY(y_val);
    }
    else
    {
        nextHandler->handle();
    }
}

void LeftArrowHandler::handle()
{
    if(PKey == ARROW_LEFT)
    {
        int y_val = textView->GetCursorY();
        int x_val = textView->GetCursorX() - 1;
        if(data.size() == 0)
        {
            x_val = 0;
        }
        else if(x_val < 0 && y_val == 0)
        {
            y_val = 0;
            x_val = 0;
        }
        else if(x_val < 0 && y_val != 0)
        {
            y_val--;
            x_val = data[y_val].size();
        }
        textView->SetCursorX(x_val);
        textView->SetCursorY(y_val);
    }
    else
    {
        nextHandler->handle();
    }
}

void RightArrowHandler::handle()
{
    if(PKey == ARROW_RIGHT)
    {
        int y_val = textView->GetCursorY();
        int x_val = textView->GetCursorX() + 1;
        if(data.size() == 0)
        {
            x_val = 0;
        }
        else if(x_val > data[y_val].size() && y_val == data.size() - 1)
        {
            x_val = data[y_val].size();
        }
        else if(x_val > data[y_val].size() && y_val < data.size() - 1)
        {
            y_val++;
            x_val = 0;
        }
        textView->SetCursorX(x_val);
        textView->SetCursorY(y_val);
    }
    else
    {
        nextHandler->handle();
    }
}

void TextHandler::handle()
{
    if(PKey >= 32 && PKey <= 126)
    {
        ECInsertCommand * cmd = new ECInsertCommand(data, textView, textView->GetCursorX(), textView->GetCursorY(), static_cast<char>(PKey));
        histCmds.ExecuteCmd(cmd);
    }
    else
    {
        nextHandler->handle();
    }
}

void BackspaceHandler::handle()
{
    if(PKey == BACKSPACE)
    {
        if(textView->GetCursorX() == 0 && textView->GetCursorY() == 0)  return;
        else
        {
            ECRemoveCommand * cmd = new ECRemoveCommand(data, textView, textView->GetCursorX(), textView->GetCursorY());
            histCmds.ExecuteCmd(cmd);
        }
    }
    else
    {
        nextHandler->handle();
    }
}


void EnterHandler::handle()
{
    if(PKey == ENTER)
    {
        ECEnterCommand * cmd = new ECEnterCommand(data, textView, textView->GetCursorX(), textView->GetCursorY());
        histCmds.ExecuteCmd(cmd);
    }
    else
    {
        nextHandler->handle();
    }
}

void UndoHandler::handle()
{
    if(PKey == CTRL_Z)
    {
        histCmds.Undo();
    }
    else
    {
        nextHandler->handle();
    }
}

void RedoHandler::handle()
{
    if(PKey == CTRL_Y)
    {
        histCmds.Redo();
    }
    else
    {
        //nextHandler->handle();
    }
}

