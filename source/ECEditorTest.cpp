// Test code for editor
#include "ECTextViewImp.h"
#include "ECTextEditor.h"
#include <iostream>

using namespace  std;

int main(int argc, char *argv[])
{
    ECTextViewImp* wndTest = new ECTextViewImp();
    new ECEditorObserver(wndTest);
    return 0;
}
