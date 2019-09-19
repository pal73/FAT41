//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CPort.hpp"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TComboBox *ComboBox1;
        TButton *B1;
        TButton *Button1;
        TComPort *ComPort1;
        TTimer *Timer1;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TButton *Button7;
        TButton *Button8;
        TButton *Button18;
        TComboBox *ComboBox4;
        TButton *Button11;
        TButton *Button12;
        TButton *Button13;
        TComboBox *ComboBox2;
        TComboBox *ComboBox3;
        TButton *Button9;
        TButton *Button10;
        TButton *Button14;
        TButton *Button15;
        TButton *Button16;
        TLabel *Label3;
        TLabel *Label1;
        TLabel *Label2;
        TButton *Button17;
        TButton *Button19;
        TButton *Button20;
        TButton *Button21;
        TButton *Button22;
        TButton *Button23;
        TButton *Button24;
        TButton *Button25;
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall B1Click(TObject *Sender);
        void __fastcall ComPort1RxChar(TObject *Sender, int Count);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
        void __fastcall Button20Click(TObject *Sender);
        void __fastcall Button19Click(TObject *Sender);
        void __fastcall Button21Click(TObject *Sender);
        void __fastcall Button22Click(TObject *Sender);
        void __fastcall Button23Click(TObject *Sender);
        void __fastcall Button24Click(TObject *Sender);
        void __fastcall Button25Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void __fastcall TForm1::uart_out_out(unsigned short len, char data0, char data1, char data2, char data3, char data4, char data5, char data6, char data7);
        //int index_offset (signed int index,signed int offset);
        //char control_check(int index);
        void __fastcall TForm1::uart_in_an(void);
        //AnsiString char2str_h(unsigned char in);
        //AnsiString long2str_h(unsigned long in);
        void __fastcall TForm1::out_adr_blok_to_page(char* ptr);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
