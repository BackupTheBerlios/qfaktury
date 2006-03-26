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
#include <qtextcodec.h>
#include <qsettings.h>
#include <qfiledialog.h>


#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qimage.h>
#include <qpicture.h>

#include <qdatetime.h>
#include <qdir.h>
#include <qdom.h>
#include <qfont.h>

#include <qmessagebox.h>
#include <qapplication.h>

// #include "preview.h"




void
Form7::init ()
{
  QSettings settings;
  QString localEnc = settings.readEntry ("elinux/localEnc", "ISO 8859-2");
  QTextCodec::setCodecForCStrings (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForTr (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForLocale (QTextCodec::codecForName (localEnc));
  tablePos->setColumnWidth (0, 300);
  getLanguages();
  getEncodings();
  readSettings ();
}

void
Form7::apply ()
{
  saveSettings ();
  zastButton->setEnabled (false);
}

void
Form7::getLanguages()
{
 QDir abs (qApp->argv ()[0]);
 QString graphDir;
 if (QString (qApp->argv ()[0]).left (2) == "./")
   graphDir = abs.absPath ();
 else
   graphDir = "/usr/bin/qfaktury";
  graphDir = graphDir.replace ("bin", "share");
  graphDir = graphDir + "/translations/";
  QDir allFiles;
  allFiles.setPath ( graphDir );
  allFiles.setFilter (QDir::Files);
  allFiles.setNameFilter ("*.qm");
  QStringList pliczki = allFiles.entryList ();
  int i, max = pliczki.count ();
  for (i = 0; i < max; ++i)
    {
     langList->insertItem( pliczki[i].remove(".qm", FALSE) );
    }

}

void
Form7::getEncodings()
{
 QTextCodec *codec;
 int i;
 for (i = 0; (codec = QTextCodec::codecForIndex(i)); i++)
        codecList->insertItem( codec->name() );
}


void
Form7::okButtonClick ()
{
  saveSettings ();
  accept ();
}

QString Form7::getAll (QListBox * lb)
{
  QString
    tmp;
  int
    selNr,
    posCount = lb->count ();
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      tmp += lb->text (selNr);
      tmp += "|";
    }
  return tmp;
}

void
Form7::saveSettings ()
{
  QSettings settings;

  settings.beginGroup ("elinux");
  settings.writeEntry ("default_browser", cbBrowser->isChecked ());
  settings.writeEntry ("browser_name", editBrName->text ());
  settings.writeEntry ("lang", langList->currentText() );
  settings.writeEntry ("localEnc", codecList->currentText() );
  settings.endGroup ();

  settings.beginGroup ("elinux/faktury");
  settings.writeEntry ("firstrun", "nie");
  settings.writeEntry ("logo", logoEdit->text ());
  settings.writeEntry ("jednostki", getAll (currlBox));
  settings.writeEntry ("stawki", getAll (vatlBox).remove ("%"));
  settings.writeEntry ("waluty", getAll (currencylBox));
  settings.writeEntry ("pkorekty", getAll (korlBox));
  settings.writeEntry ("payments", getAll (paymlBox));	// uwaga!! get first
  settings.writeEntry ("paym1", paymlBox->text (0));

  settings.writeEntry ("addText", additText->text ());

  // settings.writeEntry ("fiskal", checkBNetto->isChecked ());
  settings.writeEntry ("prefix", prefixEdit->text ());
  settings.writeEntry ("sufix", sufixEdit->text ());
  settings.writeEntry ("day", cbDay->isChecked ());
  settings.writeEntry ("month", cbMonth->isChecked ());
  settings.writeEntry ("year", cbYear->isChecked ());
  settings.writeEntry ("edit", cbEdit->isChecked ());
  settings.writeEntry ("editSymbol", cbSmbEdit->isChecked ());
  settings.writeEntry ("shortYear", shortYear->isChecked ());
  settings.writeEntry ("chars_in_symbol", spbNumb->value ());

  settings.endGroup ();

  settings.beginGroup ("elinux/faktury_pozycje");
  QCheckTableItem *cb;
  cb = static_cast < QCheckTableItem * >(tablePos->item (0, 0));
  settings.writeEntry ("Lp", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (1, 0));
  settings.writeEntry ("Nazwa", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (2, 0));
  settings.writeEntry ("Kod", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (3, 0));
  settings.writeEntry ("pkwiu", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (4, 0));
  settings.writeEntry ("ilosc", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (5, 0));
  settings.writeEntry ("jm", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (6, 0));
  settings.writeEntry ("cenajedn", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (7, 0));
  settings.writeEntry ("wartnetto", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (8, 0));
  settings.writeEntry ("rabatperc", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (9, 0));
  settings.writeEntry ("rabatval", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (10, 0));
  settings.writeEntry ("nettoafter", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (11, 0));
  settings.writeEntry ("vatval", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (12, 0));
  settings.writeEntry ("vatprice", cb->isChecked ());
  cb = static_cast < QCheckTableItem * >(tablePos->item (13, 0));
  settings.writeEntry ("bruttoval", cb->isChecked ());
  settings.endGroup ();
}

void
Form7::readSettings ()
{
  QSettings settings;

  //  settings.writeEntry ("firstrun", "nie");
  logoEdit->setText (settings.readEntry ("elinux/faktury/logo"));
  currlBox->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/jednostki")),
		      -1);
  vatlBox->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/stawki")),
		      -1);
  currencylBox->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/waluty")),
		      -1);
  paymlBox->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/payments")),
		      -1);
  korlBox->
    insertStringList (QStringList::
		      split ("|",
			     settings.readEntry ("elinux/faktury/pkorekty")),
		      -1);

  /*  
     settings.writeEntry ("payments",   getAll(paymlBox) ); // uwaga!! get first
     settings.writeEntry ("paym1",   getAll(paymlBox)[0] );   
   */
  QCheckTableItem *cbItem1 = new QCheckTableItem (tablePos, tr("Lp.") );
  tablePos->setItem (0, 0, cbItem1);
  cbItem1->setChecked (settings.readBoolEntry ("elinux/faktury_pozycje/Lp"));

  QCheckTableItem *cbItem2 = new QCheckTableItem (tablePos, tr("Nazwa") );
  tablePos->setItem (1, 0, cbItem2);
  cbItem2->setChecked (settings.
		       readBoolEntry ("elinux/faktury_pozycje/Nazwa"));

  QCheckTableItem *cbItem3 = new QCheckTableItem (tablePos, tr("Kod") );
  tablePos->setItem (2, 0, cbItem3);
  cbItem3->setChecked (settings.readBoolEntry ("elinux/faktury_pozycje/Kod"));

  QCheckTableItem *cbItem4 = new QCheckTableItem (tablePos, tr("PKWiU") );
  tablePos->setItem (3, 0, cbItem4);
  cbItem4->setChecked (settings.
		       readBoolEntry ("elinux/faktury_pozycje/pkwiu"));

  QCheckTableItem *cbItem5 = new QCheckTableItem (tablePos, tr("Ilo¶æ") );
  tablePos->setItem (4, 0, cbItem5);
  cbItem5->setChecked (settings.
		       readBoolEntry ("elinux/faktury_pozycje/ilosc"));

  QCheckTableItem *cbItem6 = new QCheckTableItem (tablePos, tr("Jm.") );
  tablePos->setItem (5, 0, cbItem6);
  cbItem6->setChecked (settings.readBoolEntry ("elinux/faktury_pozycje/jm"));

  QCheckTableItem *cbItem7 = new QCheckTableItem (tablePos, tr("Cena jdn.") );
  tablePos->setItem (6, 0, cbItem7);
  cbItem7->setChecked (settings.
		       readBoolEntry ("elinux/faktury_pozycje/cenajedn"));

  QCheckTableItem *cbItem8 = new QCheckTableItem (tablePos, tr("Warto¶æ Netto") );
  tablePos->setItem (7, 0, cbItem8);
  cbItem8->setChecked (settings.
		       readBoolEntry ("elinux/faktury_pozycje/wartnetto"));

  QCheckTableItem *cbItem9 = new QCheckTableItem (tablePos, tr("Rabat %") );
  tablePos->setItem (8, 0, cbItem9);
  cbItem9->setChecked (settings.
		       readBoolEntry ("elinux/faktury_pozycje/rabatperc"));

  QCheckTableItem *cbItem10 = new QCheckTableItem (tablePos, tr("Rabat Warto¶æ") );
  tablePos->setItem (9, 0, cbItem10);
  cbItem10->setChecked (settings.
			readBoolEntry ("elinux/faktury_pozycje/rabatval"));

  QCheckTableItem *cbItem11 =
    new QCheckTableItem (tablePos, tr("Netto po rabacie") );
  tablePos->setItem (10, 0, cbItem11);
  cbItem11->setChecked (settings.
			readBoolEntry ("elinux/faktury_pozycje/nettoafter"));

  QCheckTableItem *cbItem12 = new QCheckTableItem (tablePos, tr("Stawka VAT") );
  tablePos->setItem (11, 0, cbItem12);
  cbItem12->setChecked (settings.
			readBoolEntry ("elinux/faktury_pozycje/vatval"));

  QCheckTableItem *cbItem13 = new QCheckTableItem (tablePos, tr("Kwota Vat") );
  tablePos->setItem (12, 0, cbItem13);
  cbItem13->setChecked (settings.
			readBoolEntry ("elinux/faktury_pozycje/vatprice"));

  QCheckTableItem *cbItem14 =
    new QCheckTableItem (tablePos, tr("Warto¶æ Brutto") );
  tablePos->setItem (13, 0, cbItem14);
  cbItem14->setChecked (settings.
			readBoolEntry ("elinux/faktury_pozycje/bruttoval"));

  prefixEdit->setText (settings.readEntry ("elinux/faktury/prefix"));
  sufixEdit->setText (settings.readEntry ("elinux/faktury/sufix"));

  // if (settings.readEntry ("elinux/faktury/addText") != "" )
  additText->setText (settings.readEntry ("elinux/faktury/addText"));

  // checkBNetto->setChecked (settings.readBoolEntry ("elinux/faktury/fiskal"));
  cbDay->setChecked (settings.readBoolEntry ("elinux/faktury/day"));
  cbMonth->setChecked (settings.readBoolEntry ("elinux/faktury/month"));
  cbYear->setChecked (settings.readBoolEntry ("elinux/faktury/year"));
  shortYear->setChecked (settings.readBoolEntry ("elinux/faktury/shortYear"));
  cbEdit->setChecked (settings.readBoolEntry ("elinux/faktury/edit"));
  cbBrowser->setChecked (settings.
			 readBoolEntry ("elinux/default_browser", TRUE));
  editBrName->setText (settings.readEntry ("elinux/browser_name"));
  if (!cbBrowser->isChecked ())
    editBrName->setEnabled (TRUE);


  cbSmbEdit->setChecked (settings.
			 readBoolEntry ("elinux/faktury/editSymbol"));
  spbNumb->setValue (settings.
		     readNumEntry ("elinux/faktury/chars_in_symbol"));

 codecList->setCurrentText( settings.readEntry ("elinux/localEnc", "ISO 8859-2") );


 langList->setCurrentText( settings.readEntry ("elinux/lang", "polski") );
 
}


void
Form7::addLogoBtnClick ()
{
  // qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  QString ofn = QFileDialog::getOpenFileName ("/home",
					      tr("Obrazki (*.jpg *.png)"),
					      this,
					      tr("Wybierz logo"),
					      tr("Wybierz plik do wstawienia jako logo"));

  QFile f (ofn);
  if (!f.open (IO_ReadOnly))
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Nie mo¿na dodaæ pliku. Plik tylko do odczytu."),
				QMessageBox::Ok);
      return;
    }


  logoEdit->setText (ofn);
  zastButton->setEnabled (true);
  f.close ();
}


void
Form7::currAddBtnClick ()
{
  //    qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  if (currEdit->text () != "")
    {
      currlBox->insertItem (currEdit->text (), -1);
      currEdit->clear ();
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Nie mo¿na dodaæ. Pole jest puste."),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::currDelBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = currlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (currlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      currlBox->removeItem (selNr);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby usuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}

void
Form7::korAddBtnClick ()
{
  //    qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  if (korEdit->text () != "")
    {
      korlBox->insertItem (korEdit->text (), -1);
      korEdit->clear ();
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Nie mo¿na dodaæ. Pole jest puste."),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}



void
Form7::korDelBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = korlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (korlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      korlBox->removeItem (selNr);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby usuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::vatAddBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  if (vatEdit->text () != "")
    {
      vatlBox->insertItem (vatEdit->text (), -1);
      vatEdit->clear ();
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Nie mo¿na dodaæ. Pole jest puste."),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::vatDelBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = vatlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (vatlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      vatlBox->removeItem (selNr);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby usuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::vatUpBtnClick ()
{
  int selNr, posCount = vatlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (vatlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      QString pmitem = vatlBox->text (selNr);
      vatlBox->removeItem (selNr);
      vatlBox->insertItem (pmitem, selNr - 1);
      vatlBox->setSelected (selNr - 1, true);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby przesuwaæ"),
				QMessageBox::Ok);
    }

  zastButton->setEnabled (true);

}

void
Form7::vatDownBtnClick ()
{
  int selNr, posCount = vatlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (vatlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      QString pmitem = vatlBox->text (selNr);
      vatlBox->removeItem (selNr);
      vatlBox->insertItem (pmitem, selNr + 1);
      vatlBox->setSelected (selNr + 1, true);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby przesuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}

void
Form7::currencyAddBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  if (currencyEdit->text () != "")
    {
      currencylBox->insertItem (currencyEdit->text (), -1);
      currencyEdit->clear ();
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Nie mo¿na dodaæ. Pole jest puste."),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::currencyDelBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = currencylBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (currencylBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      currencylBox->removeItem (selNr);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby usuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::paymUpBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = paymlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (paymlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      QString pmitem = paymlBox->text (selNr);
      paymlBox->removeItem (selNr);
      paymlBox->insertItem (pmitem, selNr - 1);
      paymlBox->setSelected (selNr - 1, true);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby przesuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::paymDownBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = paymlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (paymlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      QString pmitem = paymlBox->text (selNr);
      paymlBox->removeItem (selNr);
      paymlBox->insertItem (pmitem, selNr + 1);
      paymlBox->setSelected (selNr + 1, true);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby przesuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}


void
Form7::paymDelBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  int selNr, posCount = paymlBox->count ();
  bool hasSel = false;
  for (selNr = 0; selNr < posCount; ++selNr)
    {
      if (paymlBox->isSelected (selNr))
	{
	  hasSel = true;
	  break;
	}
    }

  if (hasSel)
    {
      paymlBox->removeItem (selNr);
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Musisz co¶ zaznaczyæ, ¿eby usuwaæ"),
				QMessageBox::Ok);
    }
  zastButton->setEnabled (true);

}

void
Form7::zastBtnEnable ()
{
  zastButton->setEnabled (true);
}

void
Form7::paymAddBtnClick ()
{
  //  qDebug( "%s, %s, %d", __FUNCTION__ , __FILE__, __LINE__);
  if (paymEdit->text () != "")
    {
      paymlBox->insertItem (paymEdit->text (), -1);
      paymEdit->clear ();
    }
  else
    {
      QMessageBox::information (this, tr("Uwaga!!"),
				tr("Nie mo¿na dodaæ. Pole jest puste."),
				QMessageBox::Ok);
    }

  zastButton->setEnabled (true);
}


void
Form7::changeBrowser ()
{
  if (cbBrowser->isChecked ())
    editBrName->setEnabled (FALSE);
  else
    editBrName->setEnabled (TRUE);

  zastBtnEnable ();
}


void
Form7::defTextBtn ()
{
  additText->setText ( tr("towar odebra³em zgodnie z faktur±") );
}
