/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qdatetime.h>
#include <qdir.h>
#include <qprocess.h>
#include <qapplication.h>
#include <qsettings.h>
#include <qdom.h>
#include <qmessagebox.h>
#include <qtextcodec.h>


#include "kontlist.h"
#include "towlist.h"
#include "chamount.h"

#include "version.h"

#include "zaokr.h"
#include "slownie.h"


#include "config.h"


// QDate liabDate;
int citem;
// bool editKor;


void
korForm::init ()
{
  // invoice fra1;
  // dodac do settingsow!!

  tableTow->setColumnWidth (0, 0);	//index
  tableTow->setColumnWidth (1, 90);	// nazwa
  tableTow->setColumnWidth (2, 35);	// kod
  tableTow->setColumnWidth (3, 35);	// kod
  tableTow->setColumnWidth (4, 40);	// ilosc
  tableTow->setColumnWidth (5, 30);	// jm
  tableTow->setColumnWidth (6, 30);	// rabat
  tableTow->setColumnWidth (7, 60);	// cena jedn.
  tableTow->setColumnWidth (8, 60);	// netto
  tableTow->setColumnWidth (9, 30);	// vat
  tableTow->setColumnWidth (10, 50);	// netto *  (wzor vat)
  // tableTow->setColumnWidth(5, 90);
  // sellingDate->setDate (QDate::currentDate ());
  sellingDate2->setDate (QDate::currentDate ());
  liabDate->setDate (QDate::currentDate ());


  QDir abs (qApp->argv ()[0]);
  if (QString (qApp->argv ()[0]).left (2) == "./")
    templDir = abs.absPath ();
  else
    templDir = "/usr/bin/qfaktury";
  //absPath();
  templDir = templDir.replace ("bin", "share");
  templDir = templDir + "/templates/";

//  QString tmp;
  QSettings settings;
/*
   tmp = settings.readEntry ("elinux/faktury/korNr");

  QStringList one1 = QStringList::split ("/", tmp);
  int nr = one1[0].toInt () + 1;
  lastInvoice =
    QString::number (nr) + "/" + QDate::currentDate ().toString ("MM/yyyy");
  korNr->setText (lastInvoice);
  */

  QString localEnc = settings.readEntry ("elinux/localEnc", "ISO 8859-2");
  QTextCodec::setCodecForCStrings (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForTr (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForLocale (QTextCodec::codecForName (localEnc));

  platCombo->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/payments")),
		      -1);

// currCombo->insertStringList( QStringList::split("|",settings.readEntry("elinux/faktury/waluty")), -1); 
  // reasonCombo->clear();
  reasonCombo->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/pkorekty")),
		      -1);

  // if (settings.readEntry ("elinux/faktury/addText") != "" )
  additEdit->setText (settings.readEntry ("elinux/faktury/addText"));
  // editKor = false;
}


void
korForm::readData (QString fraFile)
{
  qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
  backBtn->setEnabled (false);	// do this another way
  korNr->setEnabled (false);
  setCaption ( tr("Edytuje korekt�") );
  // editKor = true;
  fName = fraFile;

  // here we would read all data from one xml file to the this window
  QDomDocument doc ("faktury");
  QDomElement root;
  QDomElement nabywca;
  QDomElement towary;
  QDomElement towaryOld;

  fName = fraFile;
  QFile file (progDir2 + "/faktury/" + fraFile);
  // qDebug( file.name() );
  if (!file.open (IO_ReadOnly))
    {
      qDebug ("file doesn't exists");
      return;
    }
  else
    {
      QTextStream stream (&file);
      if (!doc.setContent (stream.read ()))
	{
	  file.close ();
	  // return;
	}
    }

  root = doc.documentElement ();
  frNr->setText (root.attribute ("frnr"));
  korNr->setText (root.attribute ("nr"));
  sellingDate->
    setDate (QDate::fromString (root.attribute ("data.sprzed"), Qt::ISODate));
  sellingDate2->
    setDate (QDate::
	     fromString (root.attribute ("data.korekty"), Qt::ISODate));

  QDomNode tmp;
  tmp = root.firstChild ();
  tmp = tmp.toElement ().nextSibling ();	// nabywca
  nabywca = tmp.toElement ();
  kontrName->setText (nabywca.attribute ("nazwa") + "," +
		      nabywca.attribute ("miasto") + "," +
		      nabywca.attribute ("ulica") + ", " + tr("NIP: ") +
		      nabywca.attribute ("nip"));
  kontrName->setCursorPosition (1);

  tmp = tmp.toElement ().nextSibling ();	// towary
  towary = tmp.toElement ();
  rabatValue->setValue (towary.attribute ("goods::rabat").toInt ());
  int towCount = towary.attribute ("goods::count").toInt ();
  int i = 0;
  QDomElement towar;
  towar = towary.firstChild ().toElement ();
  // qDebug( towar.attribute("Nazwa")  );
  for (i = 0; i < towCount; ++i)
    {
      tableTow->insertRows (tableTow->numRows (), 1);
      tableTow->setText (tableTow->numRows () - 1, 0, QString::number (tableTow->numRows ()));	// lp
      tableTow->setText (tableTow->numRows () - 1, 1, towar.attribute ("Nazwa"));	// nazwa
      tableTow->setText (tableTow->numRows () - 1, 2, towar.attribute ("Kod"));	// kod
      tableTow->setText (tableTow->numRows () - 1, 3, towar.attribute ("PKWiU"));	// pkwiu
      tableTow->setText (tableTow->numRows () - 1, 4, towar.attribute ("Ilosc"));	// ilosc
      tableTow->setText (tableTow->numRows () - 1, 5, towar.attribute ("Jm."));	// jm
      tableTow->setText (tableTow->numRows () - 1, 6, towar.attribute ("Rabat"));	// rabat
      tableTow->setText (tableTow->numRows () - 1, 7, towar.attribute ("Cena_jdn."));	// cena jdn.
      tableTow->setText (tableTow->numRows () - 1, 8, towar.attribute ("Wartosc_Netto"));	// netto
      tableTow->setText (tableTow->numRows () - 1, 9, towar.attribute ("Stawka_VAT"));	// vat
      tableTow->setText (tableTow->numRows () - 1, 10, towar.attribute ("Wartosc_Brutto"));	// brutto
      towar = towar.nextSibling ().toElement ();
    }

  tmp = tmp.toElement ().nextSibling ();
  towaryOld = tmp.toElement ();
  // qDebug(  towaryOld.tagName() ); 
  QString tmp1;
  towCount = towaryOld.attribute ("goods::count").toInt ();
  // qDebug( towaryOld.attribute ("goods::count") );
  i = 0;
  towary = towaryOld.firstChild ().toElement ();

  oldNetto = 0;
  oldVat = 0;
  oldBrutto = 0;

  for (i = 0; i < towCount; ++i)
    {
      tmp1 = QString::number (i);
      tmp1 += "|" + towary.attribute ("Nazwa");
      tmp1 += "|" + towary.attribute ("Kod");
      tmp1 += "|" + towary.attribute ("PKWiU");
      tmp1 += "|" + towary.attribute ("Ilosc");
      tmp1 += "|" + towary.attribute ("Jm.");
      tmp1 += "|" + towary.attribute ("Rabat");
      tmp1 += "|" + towary.attribute ("Cena_jdn.");
      tmp1 += "|" + towary.attribute ("Wartosc_Netto");
      oldNetto +=
	towary.attribute ("Wartosc_Netto").replace (",", ".").toDouble ();
      tmp1 += "|" + towary.attribute ("Stawka_VAT");
      oldVat += towary.attribute ("Kwota_Vat").replace (",", ".").toDouble ();
      tmp1 += "|" + towary.attribute ("Kwota_Vat");
      tmp1 += "|" + towary.attribute ("Wartosc_Brutto");
      oldBrutto +=
	towary.attribute ("Wartosc_Brutto").replace (",", ".").toDouble ();
      towary = towary.nextSibling ().toElement ();
      towaryPKor[i] = tmp1;
    }


  tmp = tmp.toElement ().nextSibling ();
  QDomElement additional = tmp.toElement ();
  additEdit->setText (additional.attribute ("text"));

  platCombo->setCurrentItem (additional.attribute ("forma.plat").toInt ());

  reasonCombo->setCurrentItem (additional.attribute ("przyczyna").toInt ());
  paymFry->setText (platCombo->currentText ());
  liabDate->
    setDate (QDate::
	     fromString (additional.attribute ("liabDate"), Qt::ISODate));
  citem = additional.attribute ("waluta").toInt ();
  QSettings settings;
  // currCombo->setCurrentItem (additional.attribute ("waluta").toInt ());
  QStringList waluty =
    QStringList::split ("|", settings.readEntry ("elinux/faktury/waluty"));
  currency = waluty[additional.attribute ("waluta").toInt ()];
  // qDebug(currency); qFatal("dsadsa");
  fraValLbl->
    setText (fixStr (QString::number (oldBrutto).replace (",", ".")));


  lastInvoice += "/" + QDate::currentDate ().toString ("dd");

  if (!settings.readBoolEntry ("elinux/faktury/edit"))
    {
      korNr->setEnabled (FALSE);
      backBtn->setEnabled (FALSE);
      sellingDate2->setEnabled (FALSE);
      tableTow->setEnabled (FALSE);
      rabatValue->setEnabled (FALSE);
      platCombo->setEnabled (FALSE);
      liabDate->setEnabled (FALSE);
      reasonCombo->setEnabled (FALSE);
      additEdit->setEnabled (FALSE);
      addTw->setEnabled (FALSE);
      rmTow->setEnabled (FALSE);
      editTw->setEnabled (FALSE);
      saveBtn->setEnabled (FALSE);
      if (rabatValue->value () == 0)
	{
	  constRab->setChecked (true);
	}
      constRab->setEnabled (FALSE);
    }
  else
    {
      korNr->setEnabled (TRUE);
      backBtn->setEnabled (TRUE);
      sellingDate2->setEnabled (TRUE);
      tableTow->setEnabled (TRUE);
      if (rabatValue->value () == 0)
	{
	  constRab->setChecked (FALSE);
	  rabatValue->setEnabled (FALSE);
	}
      else
	{
	  constRab->setChecked (TRUE);
	  rabatValue->setEnabled (TRUE);
	}
      platCombo->setEnabled (TRUE);
      liabDate->setEnabled (TRUE);
      reasonCombo->setEnabled (TRUE);
      additEdit->setEnabled (TRUE);
      addTw->setEnabled (TRUE);
      rmTow->setEnabled (TRUE);
      editTw->setEnabled (TRUE);
      saveBtn->setEnabled (TRUE);
      constRab->setEnabled (TRUE);
    }


  countRabat ();
  countSum ();


}

void
korForm::readDataNewKor (QString fraFile)
{
  qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
  // here we would read all data from one xml file to the this window
  QDomDocument doc ("faktury");
  QDomElement root;
  QDomElement nabywca;
  QDomElement towary;

  backBtnClick ();

  fName = "";			// fraFile;
  QFile file (progDir2 + "/faktury/" + fraFile);
  // qDebug( file.name() );
  if (!file.open (IO_ReadOnly))
    {
      qDebug ("file doesn't exists");
      return;
    }
  else
    {
      QTextStream stream (&file);
      if (!doc.setContent (stream.read ()))
	{
	  file.close ();
	  // return;
	}
    }

  root = doc.documentElement ();
  frNr->setText (root.attribute ("nr"));
  sellingDate->
    setDate (QDate::fromString (root.attribute ("data.sprzed"), Qt::ISODate));

  QDomNode tmp;
  tmp = root.firstChild ();
  tmp = tmp.toElement ().nextSibling ();	// nabywca
  nabywca = tmp.toElement ();
  kontrName->setText (nabywca.attribute ("nazwa") + "," +
		      nabywca.attribute ("miasto") + "," +
		      nabywca.attribute ("ulica") + ", " + tr("NIP: ") +
		      nabywca.attribute ("nip"));
  kontrName->setCursorPosition (1);

  tmp = tmp.toElement ().nextSibling ();	// towary
  towary = tmp.toElement ();
  rabatValue->setValue (towary.attribute ("goods::rabat").toInt ());
  int towCount = towary.attribute ("goods::count").toInt ();
  int i = 0;
  QDomElement towar;
  towar = towary.firstChild ().toElement ();
  // qDebug( towar.attribute("Nazwa")  );
  QString tmp1;
  oldNetto = 0;
  oldVat = 0;
  oldBrutto = 0;
  for (i = 0; i < towCount; ++i)
    {
      tableTow->insertRows (tableTow->numRows (), 1);
      tableTow->setText (tableTow->numRows () - 1, 0, QString::number (tableTow->numRows ()));	// lp
      tmp1 = QString::number (i);

      tableTow->setText (tableTow->numRows () - 1, 1, towar.attribute ("Nazwa"));	// nazwa
      tmp1 += "|" + towar.attribute ("Nazwa");

      tableTow->setText (tableTow->numRows () - 1, 2,
			 towar.attribute ("Kod")); // kod
      tmp1 += "|" + towar.attribute ("Kod");

      tableTow->setText (tableTow->numRows () - 1, 3, towar.attribute ("PKWiU"));	// pkwiu
      tmp1 += "|" + towar.attribute ("PKWiU");

      tableTow->setText (tableTow->numRows () - 1, 4, towar.attribute ("Ilosc"));	// ilosc
      tmp1 += "|" + towar.attribute ("Ilosc");

      tableTow->setText (tableTow->numRows () - 1, 5,
			 towar.attribute ("Jm.")); // jm
      tmp1 += "|" + towar.attribute ("Jm.");

      tableTow->setText (tableTow->numRows () - 1, 6, towar.attribute ("Rabat"));	// jm
      tmp1 += "|" + towar.attribute ("Rabat");

      tableTow->setText (tableTow->numRows () - 1, 7, towar.attribute ("Cena_jdn."));	// cena jdn.
      tmp1 += "|" + towar.attribute ("Cena_jdn.");

      tableTow->setText (tableTow->numRows () - 1, 8, towar.attribute ("Wartosc_Netto"));	// netto
      tmp1 += "|" + towar.attribute ("Wartosc_Netto");
      oldNetto +=
	towar.attribute ("Wartosc_Netto").replace (",", ".").toDouble ();

      tableTow->setText (tableTow->numRows () - 1, 9, towar.attribute ("Stawka_VAT"));	// vat
      tmp1 += "|" + towar.attribute ("Stawka_VAT");
      oldVat += towar.attribute ("Stawka_VAT").replace (",", ".").toDouble ();


      tableTow->setText (tableTow->numRows () - 1, 10, towar.attribute ("Wartosc_Brutto"));	// brutto
      tmp1 += "|" + towar.attribute ("Kwota_Vat");
      tmp1 += "|" + towar.attribute ("Wartosc_Brutto");
      oldBrutto +=
	towar.attribute ("Wartosc_Brutto").replace (",", ".").toDouble ();

      towar = towar.nextSibling ().toElement ();
      towaryPKor[i] = tmp1;
    }
  tmp = tmp.toElement ().nextSibling ();
  QDomElement additional = tmp.toElement ();
  additEdit->setText (additional.attribute ("text"));

  platCombo->setCurrentItem (additional.attribute ("forma.plat").toInt ());
  paymFry->setText (platCombo->currentText ());
  liabDate->setDate (QDate::currentDate ());
  /*
  liabDate->
    setDate (QDate::
	     fromString (additional.attribute ("liabDate"), Qt::ISODate));
  */
  citem = additional.attribute ("waluta").toInt ();
  QSettings settings;
  // currCombo->setCurrentItem (additional.attribute ("waluta").toInt ());
  QStringList waluty =
    QStringList::split ("|", settings.readEntry ("elinux/faktury/waluty"));
  currency = waluty[additional.attribute ("waluta").toInt ()];
  // qDebug(currency); qFatal("dsadsa");
  fraValLbl->
    setText (fixStr (QString::number (oldBrutto).replace (",", ".")));

  if (settings.readBoolEntry ("elinux/faktury/editSymbol"))
    {
      korNr->setEnabled (FALSE);
      backBtn->setEnabled (FALSE);
    }
  else
    {
      korNr->setEnabled (TRUE);
      backBtn->setEnabled (TRUE);
    }


  countRabat ();
  countSum ();
}

void
korForm::getKontrahent ()
{
  kontList *klWindow =
    new kontList (this, "", FALSE, Qt::WStyle_NoBorder | Qt::WShowModal);
  if (klWindow->exec () == QDialog::Accepted)
    {
      kontrName->setText (klWindow->ret);
      kontrName->setCursorPosition (1);
    }
}

void
korForm::addTow ()
{
  towList *twWindow =
    new towList (this, "", FALSE, Qt::WStyle_NoBorder | Qt::WShowModal);
  if (twWindow->exec () == QDialog::Accepted)
    {
      // qDebug( twWindow->ret );
      // DAR|100|0,12|22|14.64
      // twarers|21398edwa|45|szt.|15,45|22|848.205
      tableTow->insertRows (tableTow->numRows (), 1);
      QStringList row = QStringList::split ("|", twWindow->ret);
      tableTow->setText (tableTow->numRows () - 1, 0, QString::number (tableTow->numRows ()));	// lp
      tableTow->setText (tableTow->numRows () - 1, 1, row[0]);	// nazwa
      tableTow->setText (tableTow->numRows () - 1, 2, row[1]);	// kod
      tableTow->setText (tableTow->numRows () - 1, 3, row[2]);	// pkwiu
      tableTow->setText (tableTow->numRows () - 1, 4, row[3]);	// ilosc
      tableTow->setText (tableTow->numRows () - 1, 5, row[4]);	// jm
      tableTow->setText (tableTow->numRows () - 1, 6, row[5]);	// Rabat
      tableTow->setText (tableTow->numRows () - 1, 7, row[6]);	// cena jdn.
      tableTow->setText (tableTow->numRows () - 1, 8, row[7]);	// netto
      tableTow->setText (tableTow->numRows () - 1, 9, row[8]);	// vat
      tableTow->setText (tableTow->numRows () - 1, 10, row[9]);	// brutto
    }
  countRabat ();
  countSum ();
}


void
korForm::countRabat ()
{

  double kwota = 0;
  double cenajdn = 0;
  double rabat = 0;
  priceBRabN = 0;
  QString rabat1;
  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      if (constRab->isChecked ())
	tableTow->setText (i, 6, QString::number (rabatValue->value ()));
      rabat1 = tableTow->text (i, 6);
      if (rabat1.length () == 1)
	rabat1 = "0.0" + rabat1;
      else
	rabat1 = "0." + rabat1;

      cenajdn = tableTow->text (i, 7).replace (",", ".").toDouble ();
      kwota = cenajdn * tableTow->text (i, 4).replace (",", ".").toInt ();
      rabat = kwota * rabat1.toDouble ();
      tableTow->setText (i, 8, fixStr (QString::number (kwota - rabat)));
// netto
      priceBRabN = priceBRabN + kwota;
      // qDebug(tableTow->text(i, 6).replace(",", "."));
    }

  double vat = 0;
  double netto = 0;
  QString vat1;
  kwota = 0;
  cenajdn = 0;
  rabat = 0;
  priceBRab = 0;		// priceBRabN
/*    
    rabat1 = QString::number( rabatValue->value());
    if (rabat1.length() == 1) rabat1 = "1.0" + rabat1;
     else rabat1 = "1." + rabat1;			
*/
  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      rabat1 = tableTow->text (i, 6);
      if (rabat1.length () == 1)
	rabat1 = "0.0" + rabat1;
      else
	rabat1 = "0." + rabat1;

      vat1 = tableTow->text (i, 9).replace (",", ".");
      // QString::number( rabatValue->value());
      if (vat1.length () == 1)
	vat1 = "1.0" + vat1;
      else
	vat1 = "1." + vat1;

      vat = vat1.toDouble ();
      if (vat == 0)
	vat = 1;

      netto = tableTow->text (i, 8).replace (",", ".").toDouble ();	// afterrab.
//************************************************************************//
      cenajdn = tableTow->text (i, 7).replace (",", ".").toDouble ();
      kwota = cenajdn * tableTow->text (i, 4).replace (",", ".").toInt ();
      rabat = kwota * rabat1.toDouble ();
      // kwota - rabat; // jedn. netto
      priceBRab = priceBRab + (vat * (kwota));
//************************************************************************//
      kwota = vat * netto;
      // qDebug(QString::number(kwota));
      tableTow->setText (i, 10, fixStr (QString::number (kwota)));
      // fixStr(
      // qDebug(tableTow->text(i, 6).replace(",", "."));
    }

}


void
korForm::countSum ()
{
  // count netto sum
  double kwota = 0;
  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      kwota += tableTow->text (i, 8).replace (",", ".").toDouble ();
      // qDebug(tableTow->text(i, 6).replace(",", "."));
    }
  snetto = fixStr (QString::number (kwota));

  // count brutto sum
  kwota = 0;
  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      kwota += tableTow->text (i, 10).replace (",", ".").toDouble ();
      // qDebug(tableTow->text(i, 6).replace(",", "."));
    }
  sbrutto = fixStr (QString::number (kwota));

  // count rabat
  QString rabat1 = QString::number (rabatValue->value ());
  if (rabat1.length () == 1)
    rabat1 = "1.0" + rabat1;
  else
    rabat1 = "1." + rabat1;
  // qDebug( "rabat " +  rabat1 );
  // double rabatVal = rabat1.toDouble();
  // double rabatBr = sbrutto->text().toDouble();
  // double przedRab = rabatVal * rabatBr;
  // qDebug("przed rabatem1: " + QString::number(priceBRab));
  // qDebug("przed rabatem2: " + QString::number(przedRab));
  sRabat = fixStr (QString::number (priceBRab - sbrutto.toDouble ()).
		   replace (".", ","));
  // setting labels text

  fvKor->setText (sbrutto);
  diffLabel->
    setText (fixStr
	     (QString::
	      number ((oldBrutto -
		       sbrutto.replace (",", ".").toDouble ()) * -1)));
}

void
korForm::rabatChange ()
{
  countRabat ();
  countSum ();
}

void
korForm::delTowar ()
{
  tableTow->removeRow (tableTow->currentRow ());
  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      tableTow->setText (i, 0, QString::number (i + 1));
    }
  countRabat ();
  countSum ();
}

void
korForm::editTowar ()
{
  // we can only modify amount
  chAmount *changeAmount =
    new chAmount (this, "", FALSE, Qt::WStyle_NoBorder | Qt::WShowModal);
  changeAmount->nameTow->setText (tableTow->
				  text (tableTow->currentRow (), 1));
  changeAmount->codeTow->setText (tableTow->
				  text (tableTow->currentRow (), 2));
  changeAmount->spinAmount->setValue (tableTow->
				      text (tableTow->currentRow (),
					    4).toInt ());
  changeAmount->spinRabat->setValue (tableTow->
				     text (tableTow->currentRow (),
					   6).toInt ());
  if (changeAmount->exec () == QDialog::Accepted)
    {
      // kontrName->setText( klWindow->ret );
      tableTow->setText (tableTow->currentRow (), 4,
			 QString::number (changeAmount->spinAmount->
					  value ()));
      tableTow->setText (tableTow->currentRow (), 6,
			 QString::number (changeAmount->spinRabat->value ()));
    }
  countRabat ();
  countSum ();
}


void
korForm::makeInvoiceHeadar ()
{

  QSettings settings;
  QString localEnc = settings.readEntry ("elinux/localEnc", "ISO 8859-2");

  fraStrList += "<html><head>";

  fraStrList += tr("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=" + localEnc + "\"/>");
  // or maybe sth like --> read encoding from a settings

  fraStrList += "<meta name=\"creator\" value=\"http://www.e-linux.pl\"/>";
  fraStrList += "</head>";
  fraStrList += "<title>" + tr("______Korekta______") + "</title>";
  fraStrList += "<style type=\"text/css\"><!-- ";
  // qDebug( templDir  );

  QFile file (templDir + "style.css");
  if (file.open (IO_ReadOnly))
    {
      QTextStream stream (&file);
      QString line;
      while (!stream.atEnd ())
	{
	  line = stream.readLine ();
	  fraStrList += line;
	}
      file.close ();
    }

  fraStrList += "--></style>";
  fraStrList += "<body>";
  fraStrList +=
    "<table width=\"700\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">";
  //  class=\"page_break\" ------>>>> think about this
  fraStrList += "<tr comment=\"headar\"><td>";
  fraStrList += "<table comment=\"headar table\" width=\"100%\" border=\"0\">";
  fraStrList += "<tr>";
  fraStrList += "<td colspan=\"2\" width=\"60%\" align=\"left\" valign=\"center\" class=\"podpisy\">";
  // logo code
  // eof logo
  QString logo = settings.readEntry ("elinux/faktury/logo");
  if (logo != "")
    {
      fraStrList +=
	"<img src=\"" + logo + "\"  width=\"200\" height=\"100\">";
    }
  else
    {
      fraStrList += tr("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Piecz�� wystawcy");
    }
  // in case when we don't have a logo :(

  fraStrList += "</td>";
  fraStrList += "<td>";

#ifdef QF_base__
  fraStrList += "<h2>" + tr("FAKTURA VAT<br>koryguj�ca<br>");
  fraStrList += tr("NR: ") + korNr->text () + "<br></h2>";
#endif

#ifdef QF_vatmp__
  fraStrList += "<h2>" + tr("FAKTURA VAT MP<br>koryguj�ca<br>");
  fraStrList += tr("NR: ") + korNr->text () + "<br></h2>";
#endif

#ifdef QF_noVAT__
  fraStrList += "<h2>" tr("Rachunek<br>koryguj�cy<br>");
  fraStrList += tr("NR: ") + korNr->text () + "<br></h2>";
#endif

  fraStrList += "<h5>" + tr("Data wystawienia: ") + sellingDate2->date ().toString ("yyyy-MM-dd") + "<br>";
  // fraStrList +=
  // QDate::currentDate ().toString ("yyyy-MM-dd")
  //   "Data sprzeda�y: " + sellingDate->date ().toString ("yyyy-MM-dd") +
  //    "<br></h5>";
  fraStrList += "</td>";
  fraStrList += "<td width=\"20\">&nbsp;</td>";
  fraStrList += "</tr>";
  fraStrList += "<tr>";
  fraStrList += "<td colspan=\"3\" align=\"right\" valign=\"top\">";
  fraStrList += "<br>" + tr("ORYGINA�/KOPIA") + "<br>";
  fraStrList += "</td>";
  fraStrList += "<td width=\"20\">&nbsp;</td>";
  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}

void
korForm::makeInvoiceBody ()
{
  fraStrList += "<tr comment=\"body\"><td>";
  fraStrList += "<table width=\"100%\" border=\"0\">";
  fraStrList += "<tr>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td width=\"48%\"  valign=\"top\"> ";
  fraStrList += "<h4>" + tr("Sprzedawca:") + "</h4>";
  QSettings settings;
  fraStrList += "<h5>" + settings.readEntry ("elinux/user/nazwa") + "<br>";
  fraStrList +=
    settings.readEntry ("elinux/user/kod") + " " +
    settings.readEntry ("elinux/user/miejscowosc") + "<br>";
  fraStrList += settings.readEntry ("elinux/user/adres") + "<br>";	// "ul." + 
  fraStrList += tr("NIP: ") + settings.readEntry ("elinux/user/nip") + "<br>";
  fraStrList +=
    tr("Nr konta: ") + settings.readEntry ("elinux/user/konto").replace ("-",
								       " ") +
    "<br>";
  if (settings.readEntry ("elinux/user/phone") != "")
    fraStrList +=
      tr ("Telefon: ") + settings.readEntry ("elinux/user/phone") + "<br>";
  if (settings.readEntry ("elinux/user/email") != "")
    fraStrList +=
      tr ("E-mail: ") + settings.readEntry ("elinux/user/email") + "<br>";
  if (settings.readEntry ("elinux/user/www") != "")
    fraStrList +=
      tr ("WWW: ") + settings.readEntry ("elinux/user/www") + "<br>";


  fraStrList += "</h5>";
  fraStrList += "</td>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td width=\"48%\"  valign=\"top\">";
  fraStrList += "<h4>" + tr("Nabywca:") + "</h4>";
  fraStrList += "<h5>" + kontrName->text ().replace (",", "<br>") + "<br>";
  fraStrList += "</h5>";
  fraStrList += "</td>";
  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}

void
korForm::makeInvoiceCorr ()
{
  fraStrList += "<tr comment=\"corr\"><td>";
  fraStrList += "<table width=\"100%\" border=\"0\">";
  fraStrList += "<tr>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td width=\"48%\"> ";

#ifdef QF_base__
  fraStrList += "<h5>" + tr("Nr faktury VAT: ") + frNr->text ();
  fraStrList += "<br>" + tr("Data wystawienia: ") + sellingDate->date ().toString ("yyyy-MM-dd"); 
  fraStrList += "<br>" + tr("Data sprzeda�y: ") + sellingDate->date ().toString ("yyyy-MM-dd");
  fraStrList += "<br>" + tr("Forma p�atno�ci faktury: ") + paymFry->text () + "</h5>";
#endif

#ifdef QF_vatmp__
  fraStrList += "<h5>" + tr("Nr faktury VAT: ") + frNr->text ();
  fraStrList += "<br>" + tr("Data wystawienia: ") + sellingDate->date ().toString ("yyyy-MM-dd"); 
  fraStrList += "<br>" + tr("Data sprzeda�y: ") + sellingDate->date ().toString ("yyyy-MM-dd");
  fraStrList += "<br>" + tr("Forma p�atno�ci faktury: ") + paymFry->text () + "</h5>";
#endif

#ifdef QF_noVAT__
  fraStrList += "<h5>" + tr("Nr rachunku: ") + frNr->text ();
  fraStrList += "<br>" + tr("Data wystawienia: ") + sellingDate->date ().toString ("yyyy-MM-dd");	// 
  fraStrList += "<br>" + tr("Data sprzeda�y: ") + sellingDate->date ().toString ("yyyy-MM-dd");
  fraStrList += "<br>" + tr("Forma p�atno�ci rachunku: ") + paymFry->text () + "</h5>";
#endif
  fraStrList += "</td>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td width=\"48%\" valign=\"bottom\">";
  fraStrList += "<h5>" + tr("Pow�d korekty: ") + reasonCombo->currentText () + "<br>";
  fraStrList += tr("Termin rozliczenia: ") + liabDate->date ().toString ("yyyy-MM-dd") + "<br>";
  fraStrList += "</h5>";
  fraStrList += "</td>";
  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}


void
korForm::makeInvoiceGoods2 ()
{
  fraStrList += "<tr comment=\"goods\" align=\"center\"><td>";

#ifdef QF_base__
  fraStrList += "<h4>" + tr("Pozycje na fakturze przed korekt�: ") + "</h4>";
#endif

#ifdef QF_vatmp__
  fraStrList += "<h4>" + tr("Pozycje na fakturze przed korekt�: ") + "</h4>";
#endif

#ifdef QF_noVAT__
  fraStrList += "<h4>" + tr("Pozycje na rachunku przed korekt�: ") + "</h4>";
#endif

  fraStrList += "<table width=\"100%\" border=\"1\" class=\"goods\">";
  fraStrList += "<tr class=\"towaryN\">";

  QSettings settings;
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Lp"))
    fraStrList += "<td width=\"20\"  align=\"center\"> " + tr("Lp.") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Nazwa"))
    fraStrList += "<td width=\"120\" align=\"center\"> " + tr("Nazwa") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Kod"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Kod") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/pkwiu"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("PKWiU") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/ilosc"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Ilo��") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/jm"))
    fraStrList += "<td width=\"20\" align=\"center\"> " + tr("Jm.") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/cenajedn"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Cena jdn.") + "</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/wartnetto")  )
//  fraStrList += "<td width=\"60\" align=\"center\">Warto�� Netto</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatperc")  )
//  fraStrList += "<td width=\"20\" align=\"center\">Rabat %</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatval")  )
//  fraStrList += "<td width=\"20\" align=\"center\">Rabat Warto��</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/nettoafter"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Netto") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatval"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Stawka VAT") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatprice"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Kwota Vat") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/bruttoval"))
    fraStrList += "<td width=\"60\" align=\"center\"> " + tr("Warto�� Brutto") + "</td>";
  fraStrList += "</tr>";
  int max = towaryPKor.size ();
  for (int i = 0; i < max; ++i)
    {
      // double vatPrice = tableTow->text(i, 9).replace(",", ".").toDouble() -
      tableTow->text (i, 6).replace (",", ".").toDouble ();
      QStringList rowTxt = QStringList::split ("|", towaryPKor[i]);
      fraStrList += "<tr class=\"towaryList\">";
// lp, nazwa, kod, ilosc, jm, cena jm., netto, vat, brutto
// if ( settings.readBoolEntry("elinux/faktury_pozycje/Lp")   )
      /*
         tmp1 = QString::number (i);
         tmp1 += "|" + towary.attribute ("Nazwa");
         tmp1 += "|" + towary.attribute ("Kod");
         tmp1 += "|" + towary.attribute ("PKWiU");
         tmp1 += "|" + towary.attribute ("Ilosc");
         tmp1 += "|" + towary.attribute ("Jm.");
         tmp1 += "|" + towary.attribute ("Rabat");
         tmp1 += "|" + towary.attribute ("Cena_jdn.");
         tmp1 += "|" + towary.attribute ("Wartosc_Netto");
         tmp1 += "|" + towary.attribute ("Stawka_VAT");
         tmp1 += "|" + towary.attribute ("Kwota_Vat");
         tmp1 += "|" + towary.attribute ("Wartosc_Brutto");
         // towary = towary.nextSibling ().toElement ();
       */

      fraStrList += "<td>&nbsp;" + QString::number (i + 1) + "</td>";

      if (settings.readBoolEntry ("elinux/faktury_pozycje/Nazwa"))
	fraStrList += "<td>&nbsp;" + rowTxt[1] + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/Kod"))
	fraStrList += "<td>&nbsp;" + rowTxt[2] + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/pkwiu"))
	fraStrList += "<td>&nbsp;" + rowTxt[3] + "</td>";
// pkwiu
      if (settings.readBoolEntry ("elinux/faktury_pozycje/ilosc"))
	fraStrList += "<td>&nbsp;" + rowTxt[4] + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/jm"))
	fraStrList += "<td>&nbsp;" + rowTxt[5] + "</td>";

      if (settings.readBoolEntry ("elinux/faktury_pozycje/cenajedn"))
	fraStrList += "<td>&nbsp;" + rowTxt[7] + "</td>";
/*   
      double cenajdn = tableTow->text (i, 6).replace (",", ".").toDouble ();
      double kwota =
	cenajdn * tableTow->text (i, 4).replace (",", ".").toInt ();
if ( settings.readBoolEntry("elinux/faktury_pozycje/wartnetto")  )
      fraStrList += "<td>&nbsp;" + fixStr (QString::number (kwota)) + "</td>";
// netto
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatperc")  )
      fraStrList += "<td>&nbsp;" + QString::number (rabatValue->value ()) + "%
</td>";	// rabat
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatval")  )
      fraStrList += "<td>&nbsp;" + fixStr (QString::number (kwota -
tableTow->text (i, 7).replace (",", ".").toDouble ())) + " </td>";	// rabat
value
      */
      if (settings.readBoolEntry ("elinux/faktury_pozycje/nettoafter"))
	fraStrList += "<td>&nbsp;" + rowTxt[8] + "</td>";	// netto po rab
      if (settings.readBoolEntry ("elinux/faktury_pozycje/vatval"))
	fraStrList += "<td>&nbsp;" + rowTxt[9] + "%</td>";
      /*
         double vatPrice = tableTow->text (i, 9).replace (",",
         ".").toDouble () -
         tableTow->text (i,
         7).replace (",", ".").toDouble ();
       */
      if (settings.readBoolEntry ("elinux/faktury_pozycje/vatprice"))
	fraStrList += "<td>&nbsp;" + rowTxt[10] + "</td>";
      /*
         if ( settings.readBoolEntry("elinux/faktury_pozycje/vatprice")  )
         fraStrList +=
         "<td>&nbsp;" + fixStr (QString::number (vatPrice)) + "</td>"; */
// if ( settings.readBoolEntry("elinux/faktury_pozycje/bruttoval")  )
      fraStrList += "<td>&nbsp;" + rowTxt[11] + "</td>";
      fraStrList += "</tr>";
    }

  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}

void
korForm::makeInvoiceSumm2 ()
{
//   double vatPrice = sbrutto.replace (",",  ".").toDouble () -  snetto.replace(",", ".").toDouble ();
  fraStrList += "<tr comment=\"razem\" align=\"center\"><td>";
  fraStrList += "<table width=\"100%\" border=\"0\">";
  fraStrList += "<tr class=\"stawki\">";

  QSettings settings;
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Lp"))
    fraStrList += "<td width=\"20\"  align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Nazwa"))
    fraStrList += "<td width=\"120\" align=\"center\">&nbsp;</td>";
//if ( settings.readBoolEntry("elinux/faktury_pozycje/Kod")  )
//  fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/pkwiu")  )
//  fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/ilosc"))
    fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/jm")  )
//  fraStrList += "<td width=\"20\" align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/cenajedn"))
    fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/wartnetto"))
    fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
/*  
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatperc")  )
  fraStrList += "<td width=\"20\" align=\"center\">&nbsp;</td>";
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatval")  )
  fraStrList += "<td width=\"20\" align=\"center\">&nbsp;</td>";
*/
/*
if ( settings.readBoolEntry("elinux/faktury_pozycje/nettoafter")  )
if ( settings.readBoolEntry("elinux/faktury_pozycje/vatval")  )
if ( settings.readBoolEntry("elinux/faktury_pozycje/vatprice")  )
if ( settings.readBoolEntry("elinux/faktury_pozycje/bruttoval")  )
*/

  fraStrList += "<td width=\"140\">&nbsp;" + tr("Razem:") + "</td>";
  fraStrList += "<td width=\"60\">&nbsp;" + fixStr (QString::number (oldNetto)) + "</td>";	// netto
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatval"))
    fraStrList += "<td width=\"60\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatprice"))
    fraStrList += "<td width=\"60\">&nbsp;" + fixStr (QString::number (oldVat)) + "</td>";	// vat
  fraStrList += "<td width=\"60\">&nbsp;" + fixStr (QString::number (oldBrutto)) + "</td>";	// brutto



  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}


void
korForm::makeInvoiceGoods ()
{
  fraStrList += "<tr comment=\"goods\" align=\"center\"><td>";

#ifdef QF_base__
  fraStrList += "<h4>" + tr("Pozycje na fakturze po korekcie: ") + "</h4>";
#endif

#ifdef QF_vatmp__
  fraStrList += "<h4>" + tr("Pozycje na fakturze po korekcie: ") + "</h4>";
#endif


#ifdef QF_noVAT__
  fraStrList += "<h4>" + tr("Pozycje na rachunku po korekcie: ") + "</h4>";
#endif
  fraStrList += "<table width=\"100%\" border=\"1\" class=\"goods\">";
  fraStrList += "<tr class=\"towaryN\">";

  QSettings settings;
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Lp"))
    fraStrList += "<td width=\"20\"  align=\"center\">" + tr("Lp.") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Nazwa"))
    fraStrList += "<td width=\"120\" align=\"center\">" + tr("Nazwa") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Kod"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Kod") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/pkwiu"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("PKWiU") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/ilosc"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Ilo��") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/jm"))
    fraStrList += "<td width=\"20\" align=\"center\">" + tr("Jm.") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/cenajedn"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Cena jdn.") + "</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/wartnetto")  )
//  fraStrList += "<td width=\"60\" align=\"center\">Warto�� Netto</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatperc")  )
//  fraStrList += "<td width=\"20\" align=\"center\">Rabat %</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatval")  )
//  fraStrList += "<td width=\"20\" align=\"center\">Rabat Warto��</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/nettoafter"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Netto") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatval"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Stawka VAT") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatprice"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Kwota Vat") + "</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/bruttoval"))
    fraStrList += "<td width=\"60\" align=\"center\">" + tr("Warto�� Brutto") + "</td>";
  fraStrList += "</tr>";

  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      // double vatPrice = tableTow->text(i, 9).replace(",", ".").toDouble() -
      tableTow->text (i, 6).replace (",", ".").toDouble ();
      fraStrList += "<tr class=\"towaryList\">";
// lp, nazwa, kod, ilosc, jm, cena jm., netto, vat, brutto
      if (settings.readBoolEntry ("elinux/faktury_pozycje/Lp"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 0) + "</td>";

      if (settings.readBoolEntry ("elinux/faktury_pozycje/Nazwa"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 1) + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/Kod"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 2) + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/pkwiu"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 3) + "</td>";
// pkwiu
      if (settings.readBoolEntry ("elinux/faktury_pozycje/ilosc"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 4) + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/jm"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 5) + "</td>";
// rabat
//      fraStrList += "<td>&nbsp;" + tableTow->text (i, 6) + "</td>";
      if (settings.readBoolEntry ("elinux/faktury_pozycje/cenajedn"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 7) + "</td>";
/*   
      double cenajdn = tableTow->text (i, 6).replace (",", ".").toDouble ();
      double kwota =
	cenajdn * tableTow->text (i, 4).replace (",", ".").toInt ();
if ( settings.readBoolEntry("elinux/faktury_pozycje/wartnetto")  )
      fraStrList += "<td>&nbsp;" + fixStr (QString::number (kwota)) + "</td>";
// netto
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatperc")  )
      fraStrList += "<td>&nbsp;" + QString::number (rabatValue->value ()) + "%
</td>";	// rabat
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatval")  )
      fraStrList += "<td>&nbsp;" + fixStr (QString::number (kwota -
tableTow->text (i, 7).replace (",", ".").toDouble ())) + " </td>";	// rabat
value
      */
      if (settings.readBoolEntry ("elinux/faktury_pozycje/nettoafter"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 8) + "</td>";	// netto po rab
      if (settings.readBoolEntry ("elinux/faktury_pozycje/vatval"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 9) + "%</td>";

      double vatPrice =
	tableTow->text (i, 10).replace (",",
					".").toDouble () - tableTow->text (i,
									   8).
	replace (",", ".").toDouble ();

      if (settings.readBoolEntry ("elinux/faktury_pozycje/vatprice"))
	fraStrList +=
	  "<td>&nbsp;" +
	  fixStr (QString::number (vatPrice).replace (",", ".")) + "</td>";
      /*
         if ( settings.readBoolEntry("elinux/faktury_pozycje/vatprice")  )
         fraStrList +=
         "<td>&nbsp;" + fixStr (QString::number (vatPrice)) + "</td>"; */
      if (settings.readBoolEntry ("elinux/faktury_pozycje/bruttoval"))
	fraStrList += "<td>&nbsp;" + tableTow->text (i, 10) + "</td>";
      fraStrList += "</tr>";
    }

  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}

void
korForm::makeInvoiceSumm ()
{
  double vatPrice = sbrutto.replace (",", ".").toDouble () -
    snetto.replace (",", ".").toDouble ();
  fraStrList += "<tr comment=\"razem\" align=\"center\"><td>";
  fraStrList += "<table width=\"100%\" border=\"0\">";
  fraStrList += "<tr class=\"stawki\">";

  QSettings settings;
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Lp"))
    fraStrList += "<td width=\"20\"  align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/Nazwa"))
    fraStrList += "<td width=\"120\" align=\"center\">&nbsp;</td>";
//if ( settings.readBoolEntry("elinux/faktury_pozycje/Kod")  )
//  fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/pkwiu")  )
//  fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/ilosc"))
    fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
// if ( settings.readBoolEntry("elinux/faktury_pozycje/jm")  )
//  fraStrList += "<td width=\"20\" align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/cenajedn"))
    fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
  if (settings.readBoolEntry ("elinux/faktury_pozycje/wartnetto"))
    fraStrList += "<td width=\"60\" align=\"center\">&nbsp;</td>";
/*  
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatperc")  )
  fraStrList += "<td width=\"20\" align=\"center\">&nbsp;</td>";
if ( settings.readBoolEntry("elinux/faktury_pozycje/rabatval")  )
  fraStrList += "<td width=\"20\" align=\"center\">&nbsp;</td>";
*/
/*
if ( settings.readBoolEntry("elinux/faktury_pozycje/nettoafter")  )
if ( settings.readBoolEntry("elinux/faktury_pozycje/vatval")  )
if ( settings.readBoolEntry("elinux/faktury_pozycje/vatprice")  )
if ( settings.readBoolEntry("elinux/faktury_pozycje/bruttoval")  )
*/

  fraStrList += "<td width=\"140\">&nbsp;" + tr("Razem:") + "</td>";
  fraStrList += "<td width=\"60\">&nbsp;" + snetto + "</td>";	// netto
  if (settings.readBoolEntry ("elinux/faktury_pozycje/vatval"))
    fraStrList += "<td width=\"60\">&nbsp;</td>";

#ifdef QF_base__
  fraStrList += "<td width=\"60\">&nbsp;" + fixStr (QString::number (vatPrice)) + "</td>";	// vat
#endif

#ifdef QF_vatmp__
  fraStrList += "<td width=\"60\">&nbsp;" + fixStr (QString::number (vatPrice)) + "</td>";	// vat
#endif

  fraStrList += "<td width=\"60\">&nbsp;" + sbrutto + "</td>";	// brutto

  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}

void
korForm::makeInvoiceSummAll ()
{

  QSettings settings;

  fraStrList += "<tr comment=\"podsumowanie\"><td>";
  fraStrList += "<table width=\"100%\" border=\"0\">";
  fraStrList += "<tr>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td width=\"48%\"> ";
  double endVal = diffLabel->text ().replace (",", ".").toDouble ();

  if (endVal > 1)
    fraStrList +=
      "<h4>" + tr("Do zap�aty: ") + diffLabel->text () + " " + currency + "</h4>";
  else
    fraStrList +=
      "<h4>" + tr("Do zwrotu: ") + fixStr (QString::number (endVal * -1)) + " " +
      currency + "</h4>";

  fraStrList +=
    "<h5>" + tr("s�ownie:") + slownie (diffLabel->text (), currency) + "</h5>";
  fraStrList += "<h5>" + tr("forma p�atno�ci: ") + platCombo->currentText () + "<br>";
  fraStrList += tr("termin p�atno�ci: ") + liabDate->date ().toString ("yyyy-MM-dd") + "<br>";

  QString paym1 = settings.readEntry ("elinux/faktury/paym1");

  if (paym1.left (3) == platCombo->currentText ().left (3))
    fraStrList += "<b>" + tr("Zap�acono got�wk�") + "</b> <br>";

  fraStrList += "</h5>";
  fraStrList += "<h4>" + additEdit->text () + "</h4>";

  fraStrList += "</td>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td width=\"48%\" valign=\"top\">";
  fraStrList += "<table width=\"80%\" class=\"stawki\" border=\"0\">";
#ifdef QF_base__
  fraStrList += "<tr>";
  fraStrList +=
    "<td colspan=\"4\"><hr width=\"100%\" noshade=\"noshade\" color=\"black\"/></td>";
  fraStrList += "</tr>";
  fraStrList += "<tr>";
  fraStrList += "<td colspan=\"4\"><h5>" + tr("Og�em stawkami:") + "&nbsp;</h5></td>";
// Og�em stawkami:
  fraStrList += "</tr>";
  fraStrList += getStawkami ();

  fraStrList += "<tr>";
  fraStrList += "<td>&nbsp;</td>";	// netto
  fraStrList += "<td>&nbsp;</td>";	// stawka
  fraStrList += "<td>&nbsp;</td>";	// podatek
  fraStrList += "<td>&nbsp;</td>";	// brutto
  fraStrList += "</tr>";
#endif

#ifdef QF_vatmp__
  fraStrList += "<tr>";
  fraStrList +=
    "<td colspan=\"4\"><hr width=\"100%\" noshade=\"noshade\" color=\"black\"/></td>";
  fraStrList += "</tr>";
  fraStrList += "<tr>";
  fraStrList += "<td colspan=\"4\"><h5>" + tr("Og�em stawkami:") + "&nbsp;</h5></td>";
// Og�em stawkami:
  fraStrList += "</tr>";
  fraStrList += getStawkami ();

  fraStrList += "<tr>";
  fraStrList += "<td>&nbsp;</td>";	// netto
  fraStrList += "<td>&nbsp;</td>";	// stawka
  fraStrList += "<td>&nbsp;</td>";	// podatek
  fraStrList += "<td>&nbsp;</td>";	// brutto
  fraStrList += "</tr>";
#endif

  fraStrList += "</table>";
  fraStrList += "</td>";
  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
}


QString
korForm::getStawkami ()
{
  QStringList out;
  QSettings settings;
  QStringList
    stawki =
    QStringList::split ("|", settings.readEntry ("elinux/faktury/stawki"));
  QMap < int, double >stawkiNetto;
  QMap < int, double >stawkiVat;
  QMap < int, double >stawkiBrutto;
  // every currency in int value is equeal vat currncy

  int ssize = stawki.size ();
  // qDebug( "%d", ssize ); 

  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      for (int y = 0; y < ssize; ++y)
	{
	  if (stawki[y] == tableTow->text (i, 9))
	    {
	      stawkiNetto[y] +=
		tableTow->text (i, 8).replace (",", ".").toDouble ();
	      stawkiBrutto[y] +=
		tableTow->text (i, 10).replace (",", ".").toDouble ();
	      stawkiVat[y] = stawkiBrutto[y] - stawkiNetto[y];
	    }
	  else
	    {
	      stawkiNetto[y] += 0;
	      stawkiBrutto[y] += 0;
	      stawkiVat[y] += 0;

	    }
	}
    }

  for (int y = 0; y < ssize; ++y)
    {
      out += "<tr>";
      out += "<td>" + fixStr (QString::number (stawkiNetto[y])) + "</td>"; // netto
      out += "<td>" + stawki[y] + "</td>";	// stawka
      out += "<td>" + fixStr (QString::number (stawkiVat[y])) + "</td>"; // podatek
      out += "<td>" + fixStr (QString::number (stawkiBrutto[y])) + "</td>"; // brutto
      out += "</tr>";

    }
  return out.join (" ");

}


void
korForm::makeInvoiceFooter ()
{
  fraStrList += "<tr comment=\"podpis\" align=\"center\"><td>";
  fraStrList += "<br><br><br><br>";
  fraStrList += "<table width=\"80%\" border=\"0\">";
  fraStrList += "<tr>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td witdh=\"48%\" align=\"center\"> ";
  fraStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
  fraStrList += "</td>";
  fraStrList += "<td witdh=\"60\">&nbsp;</td>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td witdh=\"48%\" align=\"center\"> ";
  fraStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
  fraStrList += "</td>";
  fraStrList += "</tr>";
  fraStrList += "<tr class=\"podpisy\">";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td witdh=\"48%\" align=\"center\"> ";

#ifdef QF_base__
  fraStrList += tr("Imi� i nazwisko osoby upowa�nionej do wystawienia faktury VAT");
#endif

#ifdef QF_vatmp__
  fraStrList += tr("Imi� i nazwisko osoby upowa�nionej do wystawienia faktury VAT");
#endif


#ifdef QF_noVAT__
  fraStrList += tr("Imi� i nazwisko osoby upowa�nionej do wystawienia rachunku");
#endif

  fraStrList += "</td>";
  fraStrList += "<td witdh=\"60\">&nbsp;</td>";
  fraStrList += "<td witdh=\"20\">&nbsp;</td>";
  fraStrList += "<td witdh=\"48%\" align=\"center\"> ";

#ifdef QF_base__
  fraStrList += tr("Imi� i nazwisko osoby upowa�nionej do odbioru faktury VAT");
#endif

#ifdef QF_vatmp__
  fraStrList += tr("Imi� i nazwisko osoby upowa�nionej do odbioru faktury VAT");
#endif


#ifdef QF_noVAT__
  fraStrList += tr("Imi� i nazwisko osoby upowa�nionej do odbioru rachunku");
#endif

  fraStrList += "</td>";
  fraStrList += "</tr>";
  fraStrList += "</table>";
  fraStrList += "</td></tr>";
  fraStrList += "</table>";
  fraStrList += "<tr comment=\"comment\" align=\"left\"><td>";
  fraStrList += "</td></tr>";
  fraStrList += "</table>";
  fraStrList += "<h6>QFaktury e-linux.pl" + tr(" wersja ");
  fraStrList += version;
  fraStrList += "<h6>";
  fraStrList += "</body>";
  fraStrList += "</html>";
}

void
korForm::makeInvoice ()
{
  if (kontrName->text () == "")
    {
      QMessageBox::information (this, "QFaktury", tr("Wybierz kontrahenta"),
				QMessageBox::Ok);
      return;
    }

  if (tableTow->numRows () == 0)
    {
      QMessageBox::information (this, "QFaktury", tr("Nie ma towar�w"),
				QMessageBox::Ok);
      return;
    }

  fraStrList = "";

  makeInvoiceHeadar ();
  makeInvoiceBody ();
  makeInvoiceCorr ();
  makeInvoiceGoods2 ();
  makeInvoiceSumm2 ();
  makeInvoiceGoods ();
  makeInvoiceSumm ();
  makeInvoiceSummAll ();
  makeInvoiceFooter ();

  QFile file ("/tmp/invoice.html");
  if (file.open (IO_WriteOnly))
    {
      QTextStream stream (&file);
      for (QStringList::Iterator it = fraStrList.begin ();
	   it != fraStrList.end (); ++it)
	stream << *it << "\n";
      file.close ();
    }


  QSettings settings;
  QStringList args;
  if (settings.readBoolEntry ("elinux/default_browser", TRUE))
    {
      // firts we check is KDE working
      args += "kfmclient";
      args += "exec";
      args += "/tmp/invoice.html";
      QProcess cmd (args);
      if (!cmd.start ())
	{
	  // if not we use GNOME2 api
	  args.clear ();
	  args += "gnome-open";
	  args += "/tmp/invoice.html";
	  QProcess cmd2 (args);
	  cmd2.start ();
	}
    }
  else
    {
      args += settings.readEntry ("elinux/browser_name", "mozilla");
      args += "/tmp/invoice.html";
      QProcess cmd3 (args);
      cmd3.start ();
    }
}


void
korForm::saveInvoice ()
{
 /*!
   BUG!!
	gdy dodajemy korekte, a w tym dniu nie bylo jeszcze dodane nic
   to nazwa pliku jest zle wyliczana... rozniaca 0/1 i korekta nie
   moze byc zaczytana... mozna to zaradzic tymczasowo przez przycisk
   zastosuj... ale powinno sie ten problem rozwiazac. 
  */

  if (kontrName->text () == "")
    {
      QMessageBox::information (this, "QFaktury", tr("Wybierz kontrahenta"),
				QMessageBox::Ok);
      return;
    }

  if (tableTow->numRows () == 0)
    {
      QMessageBox::information (this, "QFaktury", tr("Nie ma towar�w"),
				QMessageBox::Ok);
      return;
    }

  QDomDocument doc ("faktury");
  QDomElement root;
  QString fileName = fName;
  int pNumber = 0;
  QFile file;
  if (fileName == "")
    {
      fileName = QDate::currentDate ().toString ("yyyy-MM-dd");
      pNumber = 0;
      file.setName (progDir2 + "/faktury/h" + fileName + "_" +
		    threePlaces(pNumber) + ".xml");
      // pNumber += 1;
      ret = "h" + fileName + "_" + threePlaces(pNumber) + ".xml" + "|";

      while (file.exists ())
	{
	  file.setName (progDir2 + "/faktury/h" + fileName + "_" +
			threePlaces(pNumber) + ".xml");
	  ret =
	    "h" + fileName + "_" + threePlaces(pNumber) + ".xml" + "|";
	  pNumber += 1;
	}
      qDebug ("2");
    }
  else
    {
      file.setName (progDir2 + "/faktury/" + fileName);
      ret = fileName + "|";
      qDebug ("1");
    }
//   qDebug( "dasa" + progDir2 + "/faktury/h" + fileName + "_" + QString::number(pNumber) + ".xml" );



  // qDebug( "adsad %s", file.name() ); 
  // if (!file.open (IO_ReadOnly)) {

  root = doc.createElement ("korekta");
  root.setAttribute ("nr", korNr->text ());
  root.setAttribute ("frnr", frNr->text ());
  ret += korNr->text () + "|";
  root.setAttribute ("data.wyst", QDate::currentDate ().toString ("yyyy-MM-dd"));
  root.setAttribute ("data.korekty", sellingDate2->date ().toString ("yyyy-MM-dd"));
  ret += QDate::currentDate ().toString ("yyyy-MM-dd") + "|";
  root.setAttribute ("data.sprzed", sellingDate->date ().toString ("yyyy-MM-dd"));

  QSettings settings1;
  settings1.beginGroup ("elinux");

  ret += tr("korekta") + "|";
  root.setAttribute ("type", tr("korekta"));
  settings1.writeEntry ("faktury/korNr", korNr->text ());

  settings1.endGroup ();

  doc.appendChild (root);

  QDomElement sprzedawca;
  sprzedawca = doc.createElement ("sprzedawca");
  QSettings settings;
  sprzedawca.setAttribute ("nazwa", settings.readEntry ("elinux/user/nazwa"));
  sprzedawca.setAttribute ("kod", settings.readEntry ("elinux/user/kod"));
  sprzedawca.setAttribute ("miasto", settings.readEntry ("elinux/user/miejscowosc"));
  sprzedawca.setAttribute ("ulica", settings.readEntry ("elinux/user/adres"));
  sprzedawca.setAttribute ("nip", settings.readEntry ("elinux/user/nip"));
  sprzedawca.setAttribute ("konto", settings.readEntry ("elinux/user/konto").
			   replace (" ", "-"));
  sprzedawca.setAttribute ("telefon", settings.readEntry ("elinux/user/phone"));
  sprzedawca.setAttribute ("email", settings.readEntry ("elinux/user/email"));
  sprzedawca.setAttribute ("www", settings.readEntry ("elinux/user/www"));

  root.appendChild (sprzedawca);

  QDomElement nabywca;
  nabywca = doc.createElement ("nabywca");
  QStringList kht = QStringList::split (",", kontrName->text ());
  /* here can be bug, if kontrahent name would be with commas
     so what, block this
   */
  nabywca.setAttribute ("nazwa", kht[0]);
  ret += kht[0] + "|";
  nabywca.setAttribute ("miasto", kht[1]);
  nabywca.setAttribute ("ulica", kht[2]);

  nabywca.setAttribute ("nip", kht[3].replace (" ", "").replace (tr("NIP:"), ""));
  ret += kht[3].simplifyWhiteSpace ();
  // nabywca.setAttribute ("nip", kht[3].replace (" ", "").replace("NIP:", "NIP:")); 
  // .replace (" ", "").replace ("NIP:", ""));
  // ret += kht[3].replace (" ", "").replace("NIP:", "NIP: ");
  root.appendChild (nabywca);

// sellingDate2

  QDomElement towary;
  QDomElement goods;
  goods = doc.createElement ("goods");
  goods.setAttribute ("goods::rabat", QString::number (rabatValue->value ()));

  for (int i = 0; i < tableTow->numRows (); ++i)
    {
      towary = doc.createElement ("towar:" + tableTow->text (i, 0));
      goods.setAttribute ("goods::count", QString::number (i + 1));
      towary.setAttribute ("Lp.", tableTow->text (i, 0));
      towary.setAttribute ("Nazwa", tableTow->text (i, 1));
      towary.setAttribute ("Kod", tableTow->text (i, 2));
      towary.setAttribute ("PKWiU", tableTow->text (i, 3));
      towary.setAttribute ("Ilosc", tableTow->text (i, 4));
      towary.setAttribute ("Jm.", tableTow->text (i, 5));
      if (!constRab->isChecked ())
	{
	  towary.setAttribute ("Rabat", tableTow->text (i, 6));
	}
      else
	{
	  towary.setAttribute ("Rabat",
			       QString::number (rabatValue->value ()));
// rabat
	}
      // towary.setAttribute ("Rabat", tableTow->text (i, 6));
      towary.setAttribute ("Cena_jdn.", tableTow->text (i, 7));
      double cenajdn = tableTow->text (i, 7).replace (",", ".").toDouble ();
      double kwota =
	cenajdn * tableTow->text (i, 4).replace (",", ".").toInt ();

      towary.setAttribute ("Wartosc_Netto", fixStr (QString::number (kwota)));
// netto
      // towary.setAttribute ("Rabat", QString::number (rabatValue->value ()));
// rabat
      towary.setAttribute ("Netto_po_rabacie", tableTow->text (i, 8));
      towary.setAttribute ("Stawka_VAT", tableTow->text (i, 9));
      double vatPrice = tableTow->text (i, 10).replace (",",
							".").toDouble () -
	tableTow->text (i, 8).replace (",", ".").toDouble ();

      towary.setAttribute ("Kwota_Vat", fixStr (QString::number (vatPrice)));
      towary.setAttribute ("Wartosc_Brutto", tableTow->text (i, 9));
      goods.appendChild (towary);

    }
  root.appendChild (goods);

  QDomElement goodsRoot;
  QDomElement goodsOld;
  goodsRoot = doc.createElement ("goodsOldRoot");

  int y = towaryPKor.count ();
  QStringList tmp;
  for (int i = 0; i < y; ++i)
    {
      tmp = QStringList::split ("|", towaryPKor[i]);
      goodsOld = doc.createElement ("goodOld:" + tmp[0]);
      goodsRoot.setAttribute ("goods::count", QString::number (i + 1));
      goodsOld.setAttribute ("Lp.", QString::number (i + 1));
      goodsOld.setAttribute ("Nazwa", tmp[1]);
      goodsOld.setAttribute ("Kod", tmp[2]);
      goodsOld.setAttribute ("PKWiU", tmp[3]);
      goodsOld.setAttribute ("Ilosc", tmp[4]);
      goodsOld.setAttribute ("Jm.", tmp[5]);
      goodsOld.setAttribute ("Rabat", tmp[6]);
      goodsOld.setAttribute ("Cena_jdn.", tmp[7]);
      goodsOld.setAttribute ("Wartosc_Netto", tmp[8]);
      goodsOld.setAttribute ("Stawka_VAT", tmp[9]);
      goodsOld.setAttribute ("Kwota_Vat", tmp[10]);
      goodsOld.setAttribute ("Wartosc_Brutto", tmp[11]);
      goodsRoot.appendChild (goodsOld);
    }


  root.appendChild (goodsRoot);

  QDomElement addinfo;
  addinfo = doc.createElement ("addinfo");
  addinfo.setAttribute ("text", additEdit->text ());
  addinfo.setAttribute ("forma.plat",
			QString::number (platCombo->currentItem ()));
  addinfo.setAttribute ("przyczyna",
			QString::number (reasonCombo->currentItem ()));
//  qDebug( "moux" );
  addinfo.setAttribute ("liabDate",
			sellingDate->date ().toString ("yyyy-MM-dd"));

  // liabDate->date().toString ("yyyy-MM-dd")
  addinfo.setAttribute ("waluta", QString::number (citem));
//                      QString::number (currCombo->currentItem ()));
  root.appendChild (addinfo);

  QString xml = doc.toString ();
  file.close ();
  file.open (IO_WriteOnly);
  QTextStream ts (&file);
  ts << xml;
  // qDebug ( "ret" + ret );
  file.close ();
  accept ();
}

QString
korForm::numbersCount (int in, int x)
{
  QString tmp2, tmp = QString::number (in);
  tmp2 = "";
  int incr = x - tmp.length ();
  for (int i = 0; i < incr; ++i)
    tmp2 += "0";
  return tmp2 + tmp;
}

void
korForm::backBtnClick ()
{
  QString tmp;
  QSettings settings;

  QString prefix, sufix;
//  int numbers;

  tmp = settings.readEntry ("elinux/faktury/korNr");
  prefix = settings.readEntry ("elinux/faktury/prefix");
  sufix = settings.readEntry ("elinux/faktury/sufix");

  QStringList one1 = QStringList::split ("/", tmp);
  one1[0] = one1[0].remove (prefix);
  one1[0] = one1[0].remove (sufix);
  // qDebug( one1[0] );
  int nr = one1[0].toInt () + 1;
  lastInvoice =
    prefix + numbersCount (nr,
			   settings.
			   readNumEntry ("elinux/faktury/chars_in_symbol"));

  if (settings.readBoolEntry ("elinux/faktury/day"))
    lastInvoice += "/" + QDate::currentDate ().toString ("dd");

  if (settings.readBoolEntry ("elinux/faktury/month"))
    lastInvoice += "/" + QDate::currentDate ().toString ("MM");

  if (settings.readBoolEntry ("elinux/faktury/year"))
    {
      if (!settings.readBoolEntry ("elinux/faktury/shortYear"))
	lastInvoice += "/" + QDate::currentDate ().toString ("yy");
      else
	lastInvoice += "/" + QDate::currentDate ().toString ("yyyy");
    }

  lastInvoice += sufix;
  korNr->setText (lastInvoice);
}


/*
void korForm::backBtnClick()
{
  QString tmp;
  QSettings settings;

  if (caption ().right (3) == "VAT")
    {
      tmp = settings.readEntry ("elinux/faktury/fvat");
    }
  else
    {
      tmp = settings.readEntry ("elinux/faktury/fpro");
    }
   tmp = settings.readEntry ("elinux/faktury/korNr");

  QStringList one1 = QStringList::split ("/", tmp);
  int nr = one1[0].toInt () + 1;
  lastInvoice =
    QString::number (nr) + "/" + QDate::currentDate ().toString ("MM/yyyy");
  korNr->setText (lastInvoice);
}
*/

void
korForm::constRabClick ()
{
  if (constRab->isChecked ())
    {
      rabatValue->setEnabled (true);
    }
  else
    {
      rabatValue->setEnabled (false);
      rabatValue->setValue (0);	// is this make sense
    }
}
