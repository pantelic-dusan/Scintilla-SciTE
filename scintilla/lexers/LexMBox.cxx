// Scintilla source code edit control
/** @file LexMBox.cxx
 ** Lexer for MBox.
 **/
// Copyright 2018 - by Dusan Pantelic <dusan.pantelic96@gmail.com> and Randy Kramer <rhkramer@gmail.com>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <regex>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

#ifndef SCI_NAMESPACE
using namespace Scintilla;
#endif

static const char *const MBoxWordlistDesc[] = {
    0
};

// Map of real line states 
std::map<Sci_Position, char> dataMap;

// Map of line states when processed, this is used for highlight
std::map<Sci_Position, char> stateMap;

// Great performance emprovement when declaring regex only once
std::regex customKeywordRegex("^\\S+:.*?\\s+");
std::regex fromKeywordRegex("From\\s+(\\S+|(\".*\")+)\\s+(Mon|Tue|Wed|Thu|Fri|Sat|Sun)\\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\s+([0-2][1-9]|3[0-1])\\s+([0-1][0-9]|2[0-4]):([0-5][0-9]):([0-5][0-9])\\s+\\d{4}\\s*");
std::regex customKeywordValueAcrossLines("^\\s+\\S+.*?\\s*");


// Initializing Lexer class
class LexerMBox : public ILexer4 {
public:
    LexerMBox() {

    }

    virtual ~LexerMBox() {

    }

    int SCI_METHOD Version() const {
        return lvRelease4;
    }

    void SCI_METHOD Release() {
        delete this;
    }

    const char * SCI_METHOD PropertyNames() {
        return NULL;
    }
    
    int SCI_METHOD PropertyType(const char *name) {
        return -1;
    }

    const char * SCI_METHOD DescribeProperty(const char *name) {
        return NULL;
    }

    Sci_Position SCI_METHOD PropertySet(const char *key, const char *val) {
        return -1;
    }

    const char * SCI_METHOD DescribeWordListSets() {
        return NULL;
    }

    Sci_Position SCI_METHOD WordListSet(int n, const char *wl) {
        return -1;
    }

    void SCI_METHOD Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess);

    void SCI_METHOD Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess);

    void * SCI_METHOD PrivateCall(int operation, void *pointer) {
        return NULL;
    }

    int SCI_METHOD LineEndTypesSupported() {
        return -1;
    }

    int SCI_METHOD AllocateSubStyles(int styleBase, int numberStyles) {
        return -1;
    }

    int SCI_METHOD SubStylesStart(int styleBase) {
        return -1;
    }

	int SCI_METHOD SubStylesLength(int styleBase) {
        return -1;
    }

	int SCI_METHOD StyleFromSubStyle(int subStyle) {
        return -1;
    }

	int SCI_METHOD PrimaryStyleFromStyle(int style) {
        return -1;
    }

    void SCI_METHOD FreeSubStyles() {
        
    }

	void SCI_METHOD SetIdentifiers(int style, const char *identifiers) {
        
    }

	int SCI_METHOD DistanceToSecondaryStyles() {
        return -1;
    }

	const char * SCI_METHOD GetSubStyleBases() {
        return NULL;
    }

	int SCI_METHOD NamedStyles() {
        return -1;
    }

	const char * SCI_METHOD NameOfStyle(int style) {
        return NULL;
    }

	const char * SCI_METHOD TagsOfStyle(int style) {
        return NULL;
    }

	const char * SCI_METHOD DescriptionOfStyle(int style) {
        return NULL;
    }

    static ILexer4 *LexerFactoryMBox() {
        return new LexerMBox();
    }

};

// Checks with regex if string is in valid format for FROM line
bool IsFromLine(std::string line) {

    return std::regex_match(line, fromKeywordRegex);
}

// Checks with regex if string is valid custom keyword line in format <keyword>: <value>
bool IsCustomKeywordLine(std::string line) {

    return std::regex_match(line, customKeywordRegex);
}

bool IsCustomKeywordValueAcrossLine(std::string line) {

    return std::regex_match(line, customKeywordValueAcrossLines);
}

// Process text and for each line add state in dataMap 
Sci_Position ProcessLines(Sci_Position startPos, Sci_Position lengthDoc, LexAccessor &styler) {

    std::string lineBuffer;
    Sci_Position currentPos = startPos;
    Sci_Position currentLine = styler.GetLine(startPos);
    Sci_Position endDoc = styler.Length();

    while((currentPos < startPos+lengthDoc || dataMap[currentLine-1] != SCE_MBOX_FROM) && currentPos <= endDoc ) {

        char c = static_cast<char>(styler.SafeGetCharAt(currentPos++));
        lineBuffer += c;

        if (c == '\n') {
            
            if (lineBuffer.substr(0,5) == "From " && IsFromLine(lineBuffer)) {
                if (dataMap.find(currentLine) == dataMap.end()) {
                        dataMap.insert(std::make_pair(currentLine, SCE_MBOX_FROM)); 
                }
                else {
                    dataMap[currentLine] = SCE_MBOX_FROM;
                }
            }
            else if (lineBuffer.find(':') != std::string::npos && IsCustomKeywordLine(lineBuffer)) {
                
                if (dataMap.find(currentLine) == dataMap.end()) {
                    dataMap.insert(std::make_pair(currentLine, SCE_MBOX_CUSTOM_KEYWORD)); 
                }
                else {
                    dataMap[currentLine] = SCE_MBOX_CUSTOM_KEYWORD;
                }
            }
            else if (lineBuffer == "\n" || lineBuffer == "\r\n") {
                if (dataMap.find(currentLine) == dataMap.end()) {
                    dataMap.insert(std::make_pair(currentLine, SCE_MBOX_BLANK_LINE)); 
                }
                else {
                    dataMap[currentLine] = SCE_MBOX_BLANK_LINE;
                }
            }
            else if (dataMap.find(currentLine-1) != dataMap.end() && (dataMap[currentLine-1] == SCE_MBOX_CUSTOM_KEYWORD || dataMap[currentLine-1] == SCE_MBOX_CUSTOM_KEYWORD_VALUE) && IsCustomKeywordValueAcrossLine(lineBuffer)) {
                if (dataMap.find(currentLine) == dataMap.end()) {
                    dataMap.insert(std::make_pair(currentLine, SCE_MBOX_CUSTOM_KEYWORD_VALUE)); 
                }
                else {
                    dataMap[currentLine] = SCE_MBOX_CUSTOM_KEYWORD_VALUE;
                }
            }
            else {
                if (dataMap.find(currentLine) == dataMap.end()) {
                    dataMap.insert(std::make_pair(currentLine, SCE_MBOX_DEFAULT)); 
                }
                else {
                    dataMap[currentLine] = SCE_MBOX_DEFAULT;
                }
            }

            lineBuffer.clear();
            currentLine++;
        } 
    }

    return currentLine-1;
}

// Process lines and states from dataMap and form stateMap wich is used for highlight
void ProcessStates(void) {

    for (auto data = dataMap.begin(); data != dataMap.end(); data++) {

        if (stateMap.find(data->first) == stateMap.end()) {
            stateMap.insert(std::make_pair(data->first, data->second)); 
        }
        else {
            stateMap[data->first] = data->second;
        }

        if (data->second != SCE_MBOX_DEFAULT) {
            Sci_Position begin = data->first;
            Sci_Position end = data->first;
            while (data != dataMap.end() && data->second != SCE_MBOX_DEFAULT) {
                end = data->first;
                data++;
            }
            data--;

            if (dataMap[begin] != SCE_MBOX_BLANK_LINE && begin != 0 ) {
                for (Sci_Position i = begin; i <= end; i++) {
                    if (stateMap.find(i) == stateMap.end()) {
                        stateMap.insert(std::make_pair(i, SCE_MBOX_DEFAULT)); 
                    }
                    else {
                        stateMap[i] = SCE_MBOX_DEFAULT;
                    }
                } 
                if (data != dataMap.end())  {
                    continue;
                }
                else {
                    break;
                }
            }

            Sci_Position new_begin = begin;

            if (dataMap[begin] == SCE_MBOX_BLANK_LINE) {
                while (dataMap[new_begin] == SCE_MBOX_BLANK_LINE) {
                    new_begin++;
                }
            }

            if (dataMap[new_begin] != SCE_MBOX_FROM  ) {
                for (Sci_Position i = begin; i <= end; i++) {
                    if (stateMap.find(i) == stateMap.end()) {
                        stateMap.insert(std::make_pair(i, SCE_MBOX_DEFAULT)); 
                    }
                    else {
                        stateMap[i] = SCE_MBOX_DEFAULT;
                    }
                } 
                if (data != dataMap.end())  {
                    continue;
                }
                else {
                    break;
                }
            }

            if (dataMap[end] != SCE_MBOX_BLANK_LINE ) {
                for (Sci_Position i = begin; i <= end; i++) {
                    if (stateMap.find(i) == stateMap.end()) {
                        stateMap.insert(std::make_pair(i, SCE_MBOX_DEFAULT)); 
                    }
                    else {
                        stateMap[i] = SCE_MBOX_DEFAULT;
                    }
                } 
                if (data != dataMap.end())  {
                    continue;
                }
                else {
                    break;
                }
            }

            Sci_Position new_end = end;
            while (dataMap[new_end] == SCE_MBOX_BLANK_LINE) {
                new_end--;
            }

            bool isAllKeywords = true;
            for (Sci_Position i = new_begin+1; i <= new_end; i++) {
                if (dataMap[i] != SCE_MBOX_CUSTOM_KEYWORD && dataMap[i] != SCE_MBOX_CUSTOM_KEYWORD_VALUE) {
                    isAllKeywords = false;
                    break;
                }
            }

            if (!isAllKeywords) {
                for (Sci_Position i = begin; i <= end; i++) {
                    if (stateMap.find(i) == stateMap.end()) {
                        stateMap.insert(std::make_pair(i, SCE_MBOX_DEFAULT)); 
                    }
                    else {
                        stateMap[i] = SCE_MBOX_DEFAULT;
                    }
                } 
                if (data != dataMap.end())  {
                    continue;
                }
                else {
                    break;
                }
            }

            for (Sci_Position i = begin; i <= end; i++) {
                if (stateMap.find(i) == stateMap.end()) {
                    stateMap.insert(std::make_pair(i, dataMap[i])); 
                }
                else {
                    stateMap[i] = dataMap[i];
                }
                if (data != dataMap.end())  {
                    continue;
                }
                else {
                    break;
                }
            } 

        }
    }
}

// Finds position of last valid MBox header from currentLine
Sci_Position FindLastMBoxHeader(Sci_Position currentLine) {

    // currentLine - 1 for disable geting negative value if 0 line isn't from
    while(dataMap.find(currentLine-1) != dataMap.end() && dataMap[currentLine] != SCE_MBOX_FROM) {
        currentLine--;
    }

    return currentLine;
}

// Set states for characters
void SCI_METHOD LexerMBox::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
    
    LexAccessor styler(pAccess);

    Sci_Position startLine  = FindLastMBoxHeader(styler.GetLine(startPos));
    Sci_Position endLine = ProcessLines(startPos, lengthDoc , styler);
    
    StyleContext scCTX(styler.LineStart(startLine), styler.LineEnd(endLine)-styler.LineStart(startLine), initStyle, styler);

    ProcessStates();

    Sci_Position currentLine = startLine;

    for (; scCTX.More() ; scCTX.Forward()) {
        if (scCTX.atLineStart) {
            scCTX.SetState(stateMap[currentLine]);
            printf("%d %d %d\n", currentLine, stateMap[currentLine], dataMap[currentLine]);
        }
        if(scCTX.ch == '\n') {
            currentLine += 1;
            continue;
        }
        switch(scCTX.state) {
            case SCE_MBOX_FROM:
                if (scCTX.Match('m', ' ')) {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_FROM_VALUE);
                }
                break;
            case SCE_MBOX_CUSTOM_KEYWORD:
                if (scCTX.Match(':', ' ')) {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_CUSTOM_KEYWORD_VALUE);
                }
                break;
        };

    }
    scCTX.Complete();
}

void SCI_METHOD LexerMBox::Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {

}

LexerModule lmMBox(SCLEX_MBOX, LexerMBox::LexerFactoryMBox, "mbox", MBoxWordlistDesc);

