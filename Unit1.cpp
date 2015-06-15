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
#pragma link "FtpCli"
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
   1
   basic model app with:
     Http, version control check and download, tabs, ini
     ftp with example for flares

*/
AnsiString INIfilename= "";
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

   // collect INI file entries, set defaults
   TIniFile *ini;
   if(0==INIfilename.Length()) { // first time
      INIfilename= ChangeFileExt( Application->ExeName, ".INI");
      ini= new TIniFile(INIfilename);
      INIfilename= ini->ReadString("Setup", "INI", INIfilename); // allows changing the INIfilename some day
      delete ini;
   }
   ini = new TIniFile(INIfilename);
/*
   OpenDialog1->FilterIndex= ini->ReadInteger("Setup", "FilterIndex", 2); // default to all
   dataDir= ini->ReadString("Setup", "dataDir", "");
   observerEdit->Text= ini->ReadString("Setup", "Observer", "");
   Label4->Caption= reportDir= ini->ReadString("Setup", "reportDir", "");
   reportFile= ini->ReadString("Setup", "reportFile", "None");
*/
   delete ini;
/* model to be used later for puts
      TIniFile *ini= new TIniFile(INIfilename);
      ini->WriteBool("Setup", "reportByMonth", reportByMonth->Checked);
      delete ini;
*/
}
//---------------------------------------------------------------------------

// GOES flare data for ftp
typedef struct {
   double begin;
   double end;
   char  desc[15];
} flareDetail;
#define FLAREMAX 200
flareDetail flares[FLAREMAX];
short flareCount= 0;








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

// get flare data
AnsiString lastFlare;
short __fastcall TForm1::getFlareData(int yr, int mo, int day)
{
      AnsiString s;
      FILE* fp;
      char buf[256];
      int x;
//eg    ftp://ftp.swpc.noaa.gov/pub/warehouse/2015/2015_events/20150301events.txt
      FtpClient1->HostName= "ftp.swpc.noaa.gov";
      FtpClient1->HostDirName= s.sprintf("pub/warehouse/%04d/%04d_events", yr, yr);
      FtpClient1->HostFileName= s.sprintf("%04d%02d%02devents.txt", yr, mo, day);
      if(s==lastFlare) return flareCount; // did it already
      FtpClient1->LocalFileName= "flare.txt";
      if(FtpClient1->Receive()) {
         fp= fopen("flare.txt", "r");
         flareCount= 0;
         while(fgets(buf, sizeof(buf), fp) && flareCount< FLAREMAX) {
            if(buf[43]=='X' || buf[43]=='F') { // XRA  xray or FLA flare event
               flares[flareCount].begin= ((buf[11]-48)* 600 + (buf[12]-48)*60 + (buf[13]-48)*10 + (buf[14]-48))/ 1440.0;
               flares[flareCount].end= ((buf[18]-48)* 600 + (buf[19]-48)*60 + (buf[20]-48)*10 + (buf[21]-48))/ 1440.0;
               x= 57;
               while(buf[++x]!= ' ');
               buf[x]= 0;
               strcpy(flares[flareCount].desc, &buf[58]);
               flareCount++;
            }
         }
         lastFlare= s;
         fclose(fp);
         remove("flare.txt");  // comment out to leave in dir
      }
      return flareCount;
}
/*   example flare report
:Product: 20150301events.txt
:Created: 2015 Mar 04 0357 UT
:Date: 2015 03 01
# Prepared by the U.S. Dept. of Commerce, NOAA, Space Weather Prediction Center
# Please send comments and suggestions to SWPC.Webmaster@noaa.gov
#
# Missing data: ////
# Updated every 5 minutes.
#                            Edited Events for 2015 Mar 01
#
#Event    Begin    Max       End  Obs  Q  Type  Loc/Frq   Particulars       Reg#
#-------------------------------------------------------------------------------

4080       0158   0210      0232  G15  5   XRA  1-8A      C1.0    1.9E-03   2290
4080       0158   0159      0211  LEA  3   FLA  N19W66    SF                2290

4090       0449   0449      0451  LEA  3   FLA  N19W68    SF                2290

4100       0453   0453      0503  LEA  3   FLA  N19W68    SF                2290
.....
*/
