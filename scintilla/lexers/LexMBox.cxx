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

bool IsFromLine(Sci_Position currentLine, LexAccessor &styler) {
    std::string line;
    Sci_Position startPos = styler.LineStart(currentLine);
    Sci_Position docLines = styler.GetLine(styler.Length() - 1);
    Sci_Position endPos;
    if ( docLines == currentLine ) {
        endPos = styler.Length();
    } else {
        endPos = styler.LineStart(currentLine + 1) - 1;
    }
    Sci_Position currentPos = startPos;
    while (currentPos < endPos) {
       line += static_cast<char>(styler.SafeGetCharAt(currentPos));
       currentPos += 1;
    }
    std::string fromKeywordRegex("\\s*From\\s+");
    std::string fromSourceRegex("\\s*(\\S+|(\".*\")+)\\s*");
    std::string fromDateRegex("\\s*(Mon|Tue|Wed|Thu|Fri|Sat|Sun)\\s*(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\s+([0-2][1-9]|3[0-1])\\s*([0-1][0-9]|2[0-4]):([0-5][0-9]):([0-5][0-9])\\s*\\d{4}\\s*");

    std::regex r(fromKeywordRegex+fromSourceRegex+fromDateRegex);
    return std::regex_match(line, r);
}

bool IsDateLine(Sci_Position currentLine, LexAccessor &styler) {
    std::string line;
    Sci_Position startPos = styler.LineStart(currentLine);
    Sci_Position docLines = styler.GetLine(styler.Length() - 1);
    Sci_Position endPos;
    if ( docLines == currentLine ) {
        endPos = styler.Length();
    } else {
        endPos = styler.LineStart(currentLine + 1) - 1;
    }
    Sci_Position currentPos = startPos;
    while (currentPos < endPos) {
       line += static_cast<char>(styler.SafeGetCharAt(currentPos));
       currentPos += 1;
    }
    std::string dateKeywordRegex("\\s*Date:\\s+");
    std::string dateDateRegex("\\s*(0[1-9]|1[0-2])/([0-2][0-9]|3[0-1])/(\\d{2})\\s+([0-1][0-9]|2[0-4]):([0-5][0-9])\\s+(am|pm)\\s*");

    std::regex r(dateKeywordRegex+dateDateRegex);
    return std::regex_match(line, r);
}

bool IsSubjectLine(Sci_Position currentLine, LexAccessor &styler) {
    std::string line;
    Sci_Position startPos = styler.LineStart(currentLine);
    Sci_Position docLines = styler.GetLine(styler.Length() - 1);
    Sci_Position endPos;
    if ( docLines == currentLine ) {
        endPos = styler.Length();
    } else {
        endPos = styler.LineStart(currentLine + 1) - 1;
    }
    Sci_Position currentPos = startPos;
    while (currentPos < endPos) {
       line += static_cast<char>(styler.SafeGetCharAt(currentPos));
       currentPos += 1;
    }
    std::string subjectKeywordRegex("\\s*Subject:\\s+");
    std::string subjectTextRegex("\\s*.*?\\s*");

    std::regex r(subjectKeywordRegex+subjectTextRegex);
    return std::regex_match(line, r);
}

void SCI_METHOD LexerMBox::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
    LexAccessor styler(pAccess);
    StyleContext scCTX(startPos, lengthDoc, initStyle, styler);
    Sci_Position currentLine = styler.GetLine(startPos) ;
    for (; scCTX.More() ; scCTX.Forward()) {
        if (scCTX.atLineStart) {
            scCTX.SetState(SCE_MBOX_DEFAULT);
        }
        if(scCTX.ch == '\n') {
            currentLine += 1;
            continue;
        }
        switch(scCTX.state) {
            case SCE_MBOX_DEFAULT:
                if (scCTX.atLineStart && IsFromLine(currentLine, styler)) {
                    scCTX.SetState(SCE_MBOX_FROM_LINE);
                    break;
                }
                if (scCTX.atLineStart && IsDateLine(currentLine, styler)) {
                    scCTX.SetState(SCE_MBOX_DATE_LINE);
                    break;
                }
                if (scCTX.atLineStart && IsSubjectLine(currentLine, styler)) {
                    scCTX.SetState(SCE_MBOX_SUBJECT_LINE);
                    break;
                }
                break;
            case SCE_MBOX_FROM_LINE:
                if (scCTX.Match('m', ' ')) {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_FROM_TEXT);
                }
                if (scCTX.Match(' ', '\"')) {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_FROM_QUOTED_TEXT);
                }
                break;
            case SCE_MBOX_FROM_TEXT:
                if (scCTX.ch == ' ') {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_FROM_TIME);
                }
                break;
            case SCE_MBOX_FROM_QUOTED_TEXT:
                if (scCTX.Match('\"', ' ')) {
                    scCTX.Forward();
                    scCTX.ForwardSetState(SCE_MBOX_FROM_TIME);
                }
                break;
            case SCE_MBOX_FROM_TIME:
                if (scCTX.atLineEnd) {
                    scCTX.ForwardSetState(SCE_MBOX_DEFAULT);
                }
                break;
            case SCE_MBOX_DATE_LINE:
                if (scCTX.Match(':', ' ')) {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_DATE_DATE);
                }
                break;
            case SCE_MBOX_DATE_DATE:
                if (scCTX.atLineEnd) {
                    scCTX.ForwardSetState(SCE_MBOX_DEFAULT);
                }
                break;
            case SCE_MBOX_SUBJECT_LINE:
                if (scCTX.Match(':', ' ')) {
                    scCTX.Forward();
                    scCTX.SetState(SCE_MBOX_SUBJECT_TEXT);
                }
                break;
            case SCE_MBOX_SUBJECT_TEXT:
                if (scCTX.atLineEnd) {
                    scCTX.ForwardSetState(SCE_MBOX_DEFAULT);
                }
                break;
        };

    }
    scCTX.Complete();
}

void SCI_METHOD LexerMBox::Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {

}

LexerModule lmMBox(SCLEX_MBOX, LexerMBox::LexerFactoryMBox, "mbox", MBoxWordlistDesc);

