//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <HttpProt.hpp>
#include "ElastFrm.hpp"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TPageControl *PageControl1;
   TTabSheet *TabSheet1;
   TTabSheet *TabSheet2;
   TTabSheet *TabSheet3;
   TLabel *versionLabel;
   THttpCli *HttpCli1;
   TElasticForm *ElasticForm1;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall HttpCli1DocBegin(TObject *Sender);
   void __fastcall HttpCli1DocEnd(TObject *Sender);
   void __fastcall versionLabelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   bool __fastcall TForm1::httpGet(AnsiString URL, char* buffer, int bufsize);
   __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 