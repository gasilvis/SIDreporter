//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HttpProt"
#pragma link "ElastFrm"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

#define Version 1
// when you change this, update gasilvis.com/SIDrep/SIDreplog.php which should return this value
/*
*/
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   TStream *DataIn;
   char cp[10000];
   AnsiString s;
   int cver;
   // set window header
   Form1->Caption= "SID Reporter Application, "+ s.sprintf("version %i", Version);

   // get current version information
   if(httpGet("http://www.gasilvis.com/SID/SIDReplog.php", cp, sizeof(cp))) {
      sscanf(cp, "%d", &cver);
      if(cver > Version) {
         versionLabel->Tag= 1; // set in properties if you always want it downloadable
         versionLabel->Font->Color= clBlue;
         versionLabel->Caption= s.sprintf("Click here to download version %i", cver);
      } else {
         versionLabel->Caption= s.sprintf("%i is the latest version of the SID Reporter", cver);
      }
   }
}
//---------------------------------------------------------------------------










// http get
bool __fastcall TForm1::httpGet(AnsiString URL, char* buffer, int bufsize)
{
   TStream *DataIn;
   // simple encoding: replace ' ' with '+'
   while(URL.Pos(" ")) URL[URL.Pos(" ")]= '+';
   HttpCli1->URL        = URL;
   HttpCli1->RcvdStream = NULL;
   char altbuffer[100];
   char* buf;
   if(buffer==NULL) { // return not expected
      buf= altbuffer;
      bufsize= sizeof(buffer);
   } else { buf= buffer; }
   try {
      HttpCli1->Get();
      DataIn = new TFileStream(Form1->HttpCli1->DocName, fmOpenRead);
      DataIn->ReadBuffer(buf, min(bufsize, DataIn->Size));
      delete DataIn;
      remove(HttpCli1->DocName.c_str());
      return true;
   } __except (TRUE) {
//      Form1->Memo4->Lines->Add("GET Failed !");
//      Form1->Memo4->Lines->Add("StatusCode   = " + IntToStr(Form1->HttpCli1->StatusCode));
//      Form1->Memo4->Lines->Add("ReasonPhrase = " + Form1->HttpCli1->ReasonPhrase);
      return false;
   }
}
void __fastcall TForm1::HttpCli1DocBegin(TObject *Sender)
{
    //Memo4->Lines->Add(HttpCli1->ContentType + " => " + HttpCli1->DocName);
    //Memo4->Lines->Add("Document = " + HttpCli1->DocName);
    HttpCli1->RcvdStream = new TFileStream(HttpCli1->DocName, fmCreate);
}

void __fastcall TForm1::HttpCli1DocEnd(TObject *Sender)
{
    if (HttpCli1->RcvdStream) {
        delete HttpCli1->RcvdStream;
        HttpCli1->RcvdStream = NULL;
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::versionLabelClick(TObject *Sender)
{
    if(versionLabel->Tag) {
       ShellExecute(Handle,"open", "https://github.com/gasilvis/SIDreporter/raw/master/SIDreporter.exe",0,0,SW_SHOW);
    }
}
//---------------------------------------------------------------------------

