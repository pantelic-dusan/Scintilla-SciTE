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

// Map of real line states for current document in editor
std::map<IDocument*, std::map<Sci_Position, char>> documentDataMap;

// Map of line states when processed for current document in editor, this is used for highlight
std::map<IDocument*, std::map<Sci_Position, char>> documentStateMap;

// Great performance emprovement when declaring regex only once
std::regex customKeywordRegex("[^\\s:]+:\\s+.*?\\s+");
std::regex fromKeywordRegex("From\\s+(\\S+|(\".*\")+)\\s+(Mon|Tue|Wed|Thu|Fri|Sat|Sun)\\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\s+([1-9]|0[1-9]|[1-2][0-9]|3[0-1])\\s+([0-1][0-9]|2[0-4]):([0-5][0-9]):([0-5][0-9])\\s+\\d{4}(\\s+.*?)?\\s*");
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
Sci_Position ProcessLines(Sci_Position startPos, Sci_Position lengthDoc, LexAccessor &styler, IDocument *pAccess) {

    std::string lineBuffer;
    Sci_Position currentPos = startPos;
    Sci_Position currentLine = styler.GetLine(startPos);
    Sci_Position endDoc = styler.Length();

    while((currentPos < startPos+lengthDoc || documentDataMap[pAccess][currentLine-1] != SCE_MBOX_FROM) && currentPos <= endDoc ) {

        char c = static_cast<char>(styler.SafeGetCharAt(currentPos++));
        lineBuffer += c;

        if (c == '\n' || currentPos >= endDoc) {
            
            if (lineBuffer.substr(0,5) == "From " && IsFromLine(lineBuffer)) {
                documentDataMap[pAccess][currentLine] = SCE_MBOX_FROM;
            }
            else if (lineBuffer.find(':') != std::string::npos && IsCustomKeywordLine(lineBuffer)) {
                documentDataMap[pAccess][currentLine] = SCE_MBOX_CUSTOM_KEYWORD;
            }
            else if (lineBuffer == "\n" || lineBuffer == "\r\n") {
                documentDataMap[pAccess][currentLine] = SCE_MBOX_BLANK_LINE;
            }
            else if (documentDataMap[pAccess].find(currentLine-1) != documentDataMap[pAccess].end() && (documentDataMap[pAccess][currentLine-1] == SCE_MBOX_CUSTOM_KEYWORD || documentDataMap[pAccess][currentLine-1] == SCE_MBOX_CUSTOM_KEYWORD_VALUE) && IsCustomKeywordValueAcrossLine(lineBuffer)) {
                documentDataMap[pAccess][currentLine] = SCE_MBOX_CUSTOM_KEYWORD_VALUE;
            }
            else {
                    documentDataMap[pAccess][currentLine] = SCE_MBOX_DEFAULT;
            }

            lineBuffer.clear();
            currentLine++;
        } 
    }
    if (currentLine <= styler.GetLine(startPos)) {
        return styler.GetLine(startPos);
    }

    return currentLine-1;
}

// Process lines and states from dataMap and form stateMap wich is used for highlight
void ProcessStates(Sci_Position startLine, Sci_Position endLine, IDocument *pAccess) {

    for (auto data = documentDataMap[pAccess].find(startLine); data != documentDataMap[pAccess].end() && data->first <= endLine; data++) {

        documentStateMap[pAccess][data->first] = SCE_MBOX_DEFAULT;
        
        if (data->first != 0) {
            if (data->second != SCE_MBOX_BLANK_LINE) {
                continue;
            }
        }

        while (data->second == SCE_MBOX_BLANK_LINE) {
            data++;
        }

        Sci_Position begin = data->first;
        Sci_Position end = data->first;
        while (data != documentDataMap[pAccess].end() && data->second != SCE_MBOX_BLANK_LINE) {
            end = data->first;
            data++;
        }
        data--;
        
        if (documentDataMap[pAccess][begin] != SCE_MBOX_FROM) {
            for (Sci_Position i = begin; i <= end; i++) {
                documentStateMap[pAccess][i] = SCE_MBOX_DEFAULT;
            }
            continue;
        }

        bool isAllKeywords = true;
        for (Sci_Position i = begin+1; i <= end; i++) {
            if (documentDataMap[pAccess][i] != SCE_MBOX_CUSTOM_KEYWORD && documentDataMap[pAccess][i] != SCE_MBOX_CUSTOM_KEYWORD_VALUE) {
                isAllKeywords = false;
                break;
            }
        }

        if (!isAllKeywords) {
            for (Sci_Position i = begin; i <= end; i++) {
                documentStateMap[pAccess][i] = SCE_MBOX_DEFAULT;
            }
            continue;
        }

        if (std::next(data) == documentDataMap[pAccess].end() || std::next(data)->second != SCE_MBOX_BLANK_LINE || std::next(data)->first > endLine) {
            for (Sci_Position i = begin; i <= end; i++) {
                documentStateMap[pAccess][i] = SCE_MBOX_DEFAULT;
            }
            continue;
        }

        for (Sci_Position i = begin; i <= end; i++) {
            documentStateMap[pAccess][i] = documentDataMap[pAccess][i];
        } 

    }

}

// Finds position of last valid MBox header from currentLine
Sci_Position FindLastMBoxHeader(Sci_Position currentLine, IDocument *pAccess) {

    while(documentDataMap[pAccess].find(currentLine-1) != documentDataMap[pAccess].end() && documentDataMap[pAccess][currentLine] != SCE_MBOX_FROM ) {
        currentLine--;
    }

    while(documentDataMap[pAccess].find(currentLine-1) != documentDataMap[pAccess].end() && documentDataMap[pAccess][currentLine] != SCE_MBOX_BLANK_LINE ) {
        currentLine--;
    }

    return currentLine;
}

// Set states for characters
void SCI_METHOD LexerMBox::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {

    if (documentDataMap.find(pAccess) == documentDataMap.end() || documentStateMap.find(pAccess) == documentStateMap.end()) {
        std::map<Sci_Position, char> dataMap;
        documentDataMap[pAccess] = dataMap;
        std::map<Sci_Position, char> stateMap;
        documentStateMap[pAccess] = stateMap;
    }

    LexAccessor styler(pAccess);

    Sci_Position endLine = ProcessLines(startPos, lengthDoc , styler, pAccess);
    Sci_Position startLine  = FindLastMBoxHeader(styler.GetLine(startPos), pAccess);

    StyleContext scCTX(styler.LineStart(startLine), styler.LineEnd(endLine)-styler.LineStart(startLine), initStyle, styler);

    ProcessStates(startLine, endLine, pAccess);

    Sci_Position currentLine = startLine;
    
    for (; scCTX.More() ; scCTX.Forward()) {
        if (scCTX.atLineStart) {
            scCTX.SetState(documentStateMap[pAccess][currentLine]);
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
    
    if (documentDataMap.find(pAccess) == documentDataMap.end() || documentStateMap.find(pAccess) == documentStateMap.end()) {
        std::map<Sci_Position, char> dataMap;
        documentDataMap[pAccess] = dataMap;
        std::map<Sci_Position, char> stateMap;
        documentStateMap[pAccess] = stateMap;
    }
    
    LexAccessor styler(pAccess);
    int baseLevel = styler.LevelAt(documentStateMap[pAccess].begin()->first) & SC_FOLDLEVELNUMBERMASK;

    for (auto line = documentStateMap[pAccess].begin(); line != documentStateMap[pAccess].end(); line++) {
        if (line->second == SCE_MBOX_FROM) {
            styler.SetLevel(line->first, baseLevel|SC_FOLDLEVELHEADERFLAG);
        }
        else {
            auto lookahead = std::next(line);
            if (lookahead != documentStateMap[pAccess].end() && lookahead->second == SCE_MBOX_FROM) {
                styler.SetLevel(line->first, baseLevel);
            }
            else {
                styler.SetLevel(line->first, baseLevel+1);
            }
        }
    }
}





LexerModule lmMBox(SCLEX_MBOX, LexerMBox::LexerFactoryMBox, "mbox", MBoxWordlistDesc);