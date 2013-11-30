//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Copyright (c) 2010-2013 Cecilio Salmeron. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice, this
//      list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright notice, this
//      list of conditions and the following disclaimer in the documentation and/or
//      other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// For any comment, suggestion or feature request, please contact the manager of
// the project at cecilios@users.sourceforge.net
//---------------------------------------------------------------------------------------

#include <UnitTest++.h>
#include <sstream>
#include "lomse_config.h"

//classes related to these tests
#include "lomse_text_splitter.h"
#include "lomse_injectors.h"
#include "lomse_document.h"
#include "lomse_internal_model.h"
#include "lomse_im_factory.h"
#include "lomse_engrouters.h"

#include "utf8.h"

using namespace UnitTest;
using namespace std;
using namespace lomse;


//---------------------------------------------------------------------------------------
// access to protected members
class MyDefaultTextSplitter : public DefaultTextSplitter
{
public:
    MyDefaultTextSplitter(ImoTextItem* pText, LibraryScope& libraryScope)
        : DefaultTextSplitter(pText, libraryScope)
    {
    }

    std::vector<LUnits>& my_get_glyph_widths() { return m_glyphWidths; }
    const wstring& my_get_glyphs() { return m_glyphs; }

};

//---------------------------------------------------------------------------------------
class DefaultTextSplitterTestFixture
{
public:
    LibraryScope m_libraryScope;

    DefaultTextSplitterTestFixture()     //SetUp fixture
        : m_libraryScope(cout)
    {
        m_libraryScope.set_default_fonts_path(TESTLIB_FONTS_PATH);
    }

    ~DefaultTextSplitterTestFixture()    //TearDown fixture
    {
    }

    ImoTextItem* prepare_test(Document& doc, const string& data)
    {
        doc.create_empty();
        ImoParagraph* pPara = doc.add_paragraph();
        return pPara->add_text_item(data);
    }

    string to_str(const wstring& wtext)
    {
        string utf8result;
        utf8::utf32to8(wtext.begin(), wtext.end(), back_inserter(utf8result));
        return utf8result;
    }
};


SUITE(DefaultTextSplitterTest)
{

    TEST_FIXTURE(DefaultTextSplitterTestFixture, text_measured)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "This is a paragraph");

        MyDefaultTextSplitter splitter(pText, m_libraryScope);

        std::vector<LUnits>& widths = splitter.my_get_glyph_widths();
        CHECK( widths.size() == 19 );
        CHECK( splitter.more_text() == true );

//        //to prepare some texts
//        LUnits widthFirst = 0.0f;
//        for (int i=0; i < 6; ++i)
//            widthFirst += widths[i];
//        cout << "First chunk width = " << widthFirst << endl;
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, text_measured_non_latin)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "This is Chinese 音乐老师");

        MyDefaultTextSplitter splitter(pText, m_libraryScope);

        std::vector<LUnits>& widths = splitter.my_get_glyph_widths();
        CHECK( widths.size() == 20 );
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, first_chunk)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "This is a paragraph");
        DefaultTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(1000.0f);

        CHECK( pEngr != NULL );
        WordEngrouter* pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( pEngrouter->get_text() == L"This" );
//        cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
//        cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == true );

        delete pEngr;
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, next_chunk)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "This is a paragraph");
        DefaultTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(1000.0f);
        delete pEngr;
        pEngr = splitter.get_next_text_engrouter(6000.0f);

        CHECK( pEngr != NULL );
        WordEngrouter* pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( pEngrouter->get_text() == L"is a paragraph" );
//        cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
//        cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == false );

        delete pEngr;
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, no_more_chunks)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "This is a paragraph");
        DefaultTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(1000.0f);
        delete pEngr;
        pEngr = splitter.get_next_text_engrouter(6000.0f);
        delete pEngr;
        CHECK( splitter.more_text() == false );

        pEngr = splitter.get_next_text_engrouter(6000.0f);
        CHECK( pEngr != NULL );
        NullEngrouter* pEngrouter = dynamic_cast<NullEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( splitter.more_text() == false );

        delete pEngr;
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, empty_text)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "");
        DefaultTextSplitter splitter(pText, m_libraryScope);

        CHECK( splitter.more_text() == false );

        Engrouter* pEngr = splitter.get_next_text_engrouter(1000.0f);
        CHECK( pEngr != NULL );
        NullEngrouter* pEngrouter = dynamic_cast<NullEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( splitter.more_text() == false );

        delete pEngr;
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, first_chunk_several_spaces)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "This   is a paragraph");
        DefaultTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(1000.0f);

        CHECK( pEngr != NULL );
        WordEngrouter* pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( pEngrouter->get_text() == L"This" );
//        cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
//        cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == true );
        delete pEngr;

        pEngr = splitter.get_next_text_engrouter(6000.0f);

        CHECK( pEngr != NULL );
        pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( pEngrouter->get_text() == L"is a paragraph" );
//        cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
//        cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == false );

        delete pEngr;
    }

    TEST_FIXTURE(DefaultTextSplitterTestFixture, first_just_before_space)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "Hello world!");
        DefaultTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(1000.0f);

        CHECK( pEngr != NULL );
        WordEngrouter* pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( pEngrouter->get_text() == L"Hello" );
//        cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
//        cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == true );
        delete pEngr;
    }

};


//=======================================================================================
// Chinese text splitter tests
//=======================================================================================

//---------------------------------------------------------------------------------------
// access to protected members
class MyChineseTextSplitter : public ChineseTextSplitter
{
public:
    MyChineseTextSplitter(ImoTextItem* pText, LibraryScope& libraryScope)
        : ChineseTextSplitter(pText, libraryScope)
    {
    }

    std::vector<LUnits>& my_get_glyph_widths() { return m_glyphWidths; }
    const wstring& my_get_glyphs() { return m_glyphs; }

};

//---------------------------------------------------------------------------------------
class ChineseTextSplitterTestFixture
{
public:
    LibraryScope m_libraryScope;

    ChineseTextSplitterTestFixture()     //SetUp fixture
        : m_libraryScope(cout)
    {
        m_libraryScope.set_default_fonts_path(TESTLIB_FONTS_PATH);
    }

    ~ChineseTextSplitterTestFixture()    //TearDown fixture
    {
    }

    ImoTextItem* prepare_test(Document& doc, const string& data)
    {
        doc.create_empty();
        ImoStyle* pStyle = doc.create_style("chinese");
        pStyle->font_file("wqy-zenhei.ttc");
        ImoParagraph* pPara = doc.add_paragraph();
        return pPara->add_text_item(data, pStyle);
    }

    string to_str(const wstring& wtext)
    {
        string utf8result;
        utf8::utf32to8(wtext.begin(), wtext.end(), back_inserter(utf8result));
        return utf8result;
    }
};


SUITE(ChineseTextSplitterTest)
{

    TEST_FIXTURE(ChineseTextSplitterTestFixture, text_measured)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "编辑名称，缩写，MIDI设置和其他特性");

        MyChineseTextSplitter splitter(pText, m_libraryScope);

        std::vector<LUnits>& widths = splitter.my_get_glyph_widths();
        CHECK( widths.size() == 19 );
        CHECK( splitter.more_text() == true );

        ////to prepare some texts
        //LUnits widthFirst = 0.0f;
        //for (int i=0; i < 6; ++i)
        //    widthFirst += widths[i];
        //cout << "First chunk width = " << widthFirst << endl;       //2540.0f
    }

    TEST_FIXTURE(ChineseTextSplitterTestFixture, first_chunk)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "编辑名称，缩写，MIDI设置和其他特性");
        ChineseTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(2600.0f);

        CHECK( pEngr != NULL );
        WordEngrouter* pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( to_str( pEngrouter->get_text() ) == "编辑名称，缩" );
            //CHECK( pEngrouter->get_text() == L"编辑名称，缩" );  <-- this fails in Windows. why?
        //cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
        //cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == true );

        delete pEngr;
    }

    TEST_FIXTURE(ChineseTextSplitterTestFixture, next_chunk)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "编辑名称，缩写，MIDI设置和其他特性");
        ChineseTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(2600.0f);
        CHECK( splitter.more_text() == true );
        delete pEngr;

        pEngr = splitter.get_next_text_engrouter(10000.0f);
        CHECK( pEngr != NULL );
        WordEngrouter* pEngrouter = dynamic_cast<WordEngrouter*>( pEngr );
        CHECK( pEngrouter != NULL );
        CHECK( to_str( pEngrouter->get_text() ) == "写，MIDI设置和其他特性" );
        //cout << "chunk = '" << to_str( pEngrouter->get_text() ) << "'" << endl;
        //cout << "size = " << pEngrouter->get_width() << endl;
        CHECK( splitter.more_text() == false );

        delete pEngr;
    }

    TEST_FIXTURE(ChineseTextSplitterTestFixture, no_more_chunks)
    {
        Document doc(m_libraryScope);
        ImoTextItem* pText = prepare_test(doc, "编辑名称，缩写，MIDI设置和其他特性");
        ChineseTextSplitter splitter(pText, m_libraryScope);

        Engrouter* pEngr = splitter.get_next_text_engrouter(2600.0f);
        delete pEngr;
        pEngr = splitter.get_next_text_engrouter(10000.0f);
        delete pEngr;
        CHECK( splitter.more_text() == false );

        pEngr = splitter.get_next_text_engrouter(5000.0f);
        CHECK( pEngr == NULL );
        CHECK( splitter.more_text() == false );
        delete pEngr;
    }

}

