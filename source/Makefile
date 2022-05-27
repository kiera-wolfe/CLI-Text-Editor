CC = g++
LIBPATH =

CFLAGS = -O3 -std=c++11 -Wall

all: TextEditor


ECCommandText.o : ECCommandText.cpp  ECCommandText.h
	$(CC)  $(CFLAGS) -c ECCommandText.cpp
	
ECTextViewImp.o : ECTextViewImp.cpp  ECTextViewImp.h
	$(CC)  $(CFLAGS) -c ECTextViewImp.cpp

ECTextEditor.o : ECTextEditor.cpp  ECTextEditor.h  ECObserver.h
	$(CC)  $(CFLAGS) -c ECTextEditor.cpp

ECEditorTest.o : ECEditorTest.cpp
	$(CC)  $(CFLAGS) -c ECEditorTest.cpp

TextEditor: ECCommandText.o ECTextViewImp.o ECTextEditor.o ECEditorTest.o 
	$(CC) -o TextEditor ECCommandText.o ECTextViewImp.o ECTextEditor.o ECEditorTest.o
