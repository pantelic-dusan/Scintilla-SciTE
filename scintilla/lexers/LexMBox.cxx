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

std::map<Sci_Position, char> dataMap;
std::map<Sci_Position, char> stateMap;

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


bool IsFromLine(std::string line) {

    std::string fromKeywordRegex("\\s*From\\s+");
    std::string fromSourceRegex("\\s*(\\S+|(\".*\")+)\\s*");
    std::string fromDateRegex("\\s*(Mon|Tue|Wed|Thu|Fri|Sat|Sun)\\s*(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\s+([0-2][1-9]|3[0-1])\\s*([0-1][0-9]|2[0-4]):([0-5][0-9]):([0-5][0-9])\\s*\\d{4}\\s*");
    std::regex r(fromKeywordRegex+fromSourceRegex+fromDateRegex);
    return std::regex_match(line, r);
}


bool IsCustomKeywordLine(std::string line) {

    std::string customKeywordRegex("^[A-Za-z]+:.*?\\s+");

    std::regex r(customKeywordRegex);
    return std::regex_match(line, r);
}

Sci_Position ProcessLines(Sci_Position startPos, Sci_Position lengthDoc, LexAccessor &styler) {

    std::string lineBuffer;
    Sci_Position currentPos = startPos;
    Sci_Position currentLine = styler.GetLine(startPos);
    Sci_Position endDoc = styler.Length();

    while((currentPos < startPos+lengthDoc || dataMap[currentLine-1] != SCE_MBOX_FROM) && currentPos <= endDoc ) {

        char c = static_cast<char>(styler.SafeGetCharAt(currentPos++));
        
        lineBuffer += c;
        //printf("%c", c);

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
            else if (lineBuffer == "\n") {
                if (dataMap.find(currentLine) == dataMap.end()) {
                    dataMap.insert(std::make_pair(currentLine, SCE_MBOX_BLANK_LINE)); 
                }
                else {
                    dataMap[currentLine] = SCE_MBOX_BLANK_LINE;
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
                if (dataMap[i] != SCE_MBOX_CUSTOM_KEYWORD) {
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

Sci_Position FindLastMBoxHeader(Sci_Position currentLine) {

    // currentLine - 1 for not geting negative value if 0 line isn't from
    while(dataMap.find(currentLine-1) != dataMap.end() && dataMap[currentLine] != SCE_MBOX_FROM) {
        currentLine--;
    }

    return currentLine;
}

void SCI_METHOD LexerMBox::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
    LexAccessor styler(pAccess);

    Sci_Position startLine  = FindLastMBoxHeader(styler.GetLine(startPos));
    Sci_Position endLine = ProcessLines(startPos, lengthDoc , styler);;
    
    StyleContext scCTX(styler.LineStart(startLine), styler.LineEnd(endLine)-styler.LineStart(startLine), initStyle, styler);

    ProcessStates();

    Sci_Position currentLine = startLine;

    for (; scCTX.More() ; scCTX.Forward()) {
        if (scCTX.atLineStart) {
            scCTX.SetState(stateMap[currentLine]);
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

