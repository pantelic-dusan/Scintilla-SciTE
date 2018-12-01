// Scintilla source code edit control
/** @file LexMBox.cxx
 ** Lexer for MBox.
 **/
// Copyright 2018 - by Dusan Pantelic <dusan.pantelic96@gmail.com> and Randy Kramer <rhkramer@gmail.com>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
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

void SCI_METHOD LexerMBox::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
    LexAccessor styler(pAccess);
    StyleContext scCTX(startPos, lengthDoc, initStyle, styler);

    for (; scCTX.More() ; scCTX.Forward()) {
        switch(scCTX.state) {
            case SCE_MBOX_DEFAULT:
                if (scCTX.Match('(', '*')) {
                    scCTX.SetState(SCE_MBOX_COMMENT);
                    scCTX.Forward();
                    break;
                };
            case SCE_MBOX_COMMENT:
                if (scCTX.Match('*', ')')) {
                    scCTX.Forward();
                    scCTX.ForwardSetState(SCE_MBOX_DEFAULT);
                    break;
                };
        };
    }
    scCTX.Complete();
}

void SCI_METHOD LexerMBox::Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {

}

LexerModule lmMBox(SCLEX_MBOX, LexerMBox::LexerFactoryMBox, "mbox", MBoxWordlistDesc);