#include <qdom.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qsettings.h>

#include "zaokr.h"


class dane
{
public:
  dane ()
  {
  }
  dane (QString c1, QString c2, QString c3)
  {
    code = c1;
    if (code == "")
      code = "-";
    curr = c2;
    if (curr == "")
      curr = "-";
    pkwiu = c3;
    if (pkwiu == "")
      pkwiu = "-";
  }
  QString codeX () const
  {
    return code;
  }
  QString currX () const
  {
    return curr;
  }
  QString pkwiuX () const
  {
    return pkwiu;
  }
private:
    QString code;
  QString curr;
  QString pkwiu;
};

typedef QMap < QString, dane > daneList;
	// EmployeeMap map;
daneList listaTowary2;
daneList listaUslugi2;

QStringList listaTowary;
QStringList listaUslugi;
QString vat;

void
towList::init ()
{
  QSettings settings;
  QString localEnc = settings.readEntry ("elinux/localEnc", "ISO 8859-2");
  QTextCodec::setCodecForCStrings (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForTr (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForLocale (QTextCodec::codecForName (localEnc));
  ret = "";
  QDir tmp;
  progDir = tmp.homeDirPath () + "/elinux";
  listaTowary.clear ();
  listaUslugi.clear ();
  listaTowary2.clear ();
  listaUslugi2.clear ();
  readTow (progDir);
  listView1->clear ();
  fillLv (0);
  nameEdit->setEnabled (FALSE);
}

void
towList::readTow (QString progDir)
{
  QDomDocument doc ("towary");
  QDomElement root;
  QDomElement towar;
  QDomElement usluga;
  QString code, curr, pkwiu;



  QFile file (progDir + "/towary.xml");
  if (!file.open (IO_ReadOnly))
    {
      // qDebug ("file doesn't exists");
      return;
    }
  else
    {
      QTextStream stream (&file);
      if (!doc.setContent (stream.read ()))
	{
	  // qDebug ("can not set content ");
	  file.close ();
	  return;
	}
      else
	{
	  root = doc.documentElement ();
	  towar = root.firstChild ().toElement ();
	  usluga = root.lastChild ().toElement ();
	}
      QString text, idx;

      for (QDomNode n = towar.firstChild (); !n.isNull ();
	   n = n.nextSibling ())
	{
	  idx = n.toElement ().attribute ("idx");
	  listaTowary.append (idx);

	  text = n.toElement ().attribute ("name");
	  listaTowary.append (text);

	  code = n.toElement ().attribute ("code");
	  curr = n.toElement ().attribute ("curr");
	  pkwiu = n.toElement ().attribute ("pkwiu");
	  listaTowary2[idx] = dane (code, curr, pkwiu);

	}

      for (QDomNode n = usluga.firstChild (); !n.isNull ();
	   n = n.nextSibling ())
	{
	  idx = n.toElement ().attribute ("idx");
	  listaUslugi.append (idx);

	  text = n.toElement ().attribute ("name");
	  listaUslugi.append (text);

	  code = n.toElement ().attribute ("code");
	  curr = n.toElement ().attribute ("curr");
	  pkwiu = n.toElement ().attribute ("pkwiu");
	  listaUslugi2[idx] = dane (code, curr, pkwiu);
	}
    }
}



void
towList::doAccept ()
{
  if (countEdit->text () == "")
    {
      QMessageBox::information (this, tr("QFaktury"), tr("Podaj ilo��"),
				QMessageBox::Ok);
      return;
    }

  if (selectedItem != "")
    {
// |Index|Nazwa|Kod|Ilo��|Jm|cena jednostkowa|Kwota netto|vat|kwota brutto|
      if (comboBox1->currentItem () == 0)
	{
	  /*
	     int x = listaTowary.findIndex(selectedItem);
	     // qDebug( "%d", x );
	     // qDebug( selectedItem );
	     // qDebug( "code" + listaTowary2[id].codeX() ); 
	     // qDebug( "curr" + listaTowary2[id].currX() );
	     // qDebug( "pkwiu" + listaTowary2[id].pkwiuX() );
	   */
	  ret =
	    selectedItem + "|" + listaTowary2[id].codeX () + "|" +
	    listaTowary2[id].pkwiuX () + "|" + countEdit->text () + "|" +
	    listaTowary2[id].currX () + "|" +
	    QString::number (rabatSpin->value ()) + "|" + cenaEdit->text () +
	    "|" + nettoLabel->text () + "|" + vat + "|" +
	    bruttoLabel->text ();
	}
      if (comboBox1->currentItem () == 1)
	{
	  // int x = listaUslugi.findIndex(selectedItem);
	  // // qDebug( "%d", x );
	  ret =
	    selectedItem + "|" + listaUslugi2[id].codeX () + "|" +
	    listaUslugi2[id].pkwiuX () + "|" + countEdit->text () + "|" +
	    listaUslugi2[id].currX () + "|" +
	    QString::number (rabatSpin->value ()) + "|" + cenaEdit->text () +
	    "|" + nettoLabel->text () + "|" + vat + "|" +
	    bruttoLabel->text ();

	}
      accept ();
    }
  else
    {
      QMessageBox::information (this, tr("QFaktury"), tr("Wska� towar"),
				QMessageBox::Ok);
    }
}

void
towList::comboBox1Changed (int x)
{
  // qDebug (__FUNCTION__);
  selectedItem = "";
  countEdit->setText ("");
  rabatSpin->setValue (0);
  cenaEdit->setText ("");
  nettoLabel->setText ("");
  bruttoLabel->setText ("");
  nameEdit->setText ("");
  nameEdit->setEnabled (FALSE);


  listView1->clear ();
  fillLv (x);
}

void
towList::calcNetto ()
{
  QString rabat1 = QString::number (rabatSpin->value ());
  if (rabat1.length () == 1)
    rabat1 = "0.0" + rabat1;
  else
    rabat1 = "0." + rabat1;

  double rabat2 =
    getPrice2 (countEdit->text (), cenaEdit->text ()) * rabat1.toDouble ();
  double netto2 = getPrice2 (countEdit->text (), cenaEdit->text ()) - rabat2;
  double brutto2 = getPriceBrutto (netto2, vat);
  bruttoLabel->setText (fixStr (QString::number (brutto2)));
  nettoLabel->setText (fixStr (QString::number (netto2)));
}


void
towList::fillLv (int x)
{
  bool next = true;
  QListViewItem *tmp = NULL;
  switch (x)
    {
    case 0:
      for (uint i = 0; i < listaTowary.count (); ++i)
	{
	  if (next)
	    {
	      tmp = new QListViewItem (listView1, listaTowary[i]);
	      next = false;
	    }
	  else
	    {
	      tmp->setText (1, listaTowary[i]);
	      next = true;
	    }

	}
      break;
    case 1:
      for (uint i = 0; i < listaUslugi.count (); ++i)
	{
	  if (next)
	    {
	      tmp = new QListViewItem (listView1, listaUslugi[i]);
	      next = false;
	    }
	  else
	    {
	      tmp->setText (1, listaUslugi[i]);
	      next = true;
	    }

	}
      break;

    }
}

void
towList::lv1selChanged (QListViewItem * item)
{
  if (item->isSelected ())
    {
      readNettos (item->text (0));
      id = item->text (0);
      selectedItem = item->text (1);
      calcNetto ();
      nameEdit->setEnabled (TRUE);
      nameEdit->setText (item->text (1));
    }
  else
    {
      nameEdit->setEnabled (FALSE);
    }
}

void
towList::readNettos (QString index)
{
  QDomDocument doc ("towary");
  QDomElement root;
  QDomElement towar;
  QDomElement usluga;

  QFile file (progDir + "/towary.xml");
  if (!file.open (IO_ReadOnly))
    {
      // qDebug ("file doesn't exists");
      return;
    }
  else
    {
      QTextStream stream (&file);
      if (!doc.setContent (stream.read ()))
	{
	  // qDebug ("can not set content ");
	  file.close ();
	  return;
	}
      else
	{
	  root = doc.documentElement ();
	  towar = root.firstChild ().toElement ();
	  usluga = root.lastChild ().toElement ();
	}
      QString text;

      if (comboBox1->currentItem () == 0)
	{
	  for (QDomNode n = towar.firstChild (); !n.isNull ();
	       n = n.nextSibling ())
	    {

	      text = n.toElement ().attribute ("idx");
	      if (text == index)
		{
		  nettos[0] = n.toElement ().attribute ("netto1");
		  nettos[1] = n.toElement ().attribute ("netto2");
		  nettos[2] = n.toElement ().attribute ("netto3");
		  nettos[3] = n.toElement ().attribute ("netto4");
		  cenaEdit->setText (nettos[0]);
		  spinBox2->setValue (1);
		  vat = n.toElement ().attribute ("vat");
		}
	    }
	}

      if (comboBox1->currentItem () == 1)
	{

	  for (QDomNode n = usluga.firstChild (); !n.isNull ();
	       n = n.nextSibling ())
	    {
	      text = n.toElement ().attribute ("idx");
	      if (text == index)
		{
		  nettos[0] = n.toElement ().attribute ("netto1");
		  nettos[1] = n.toElement ().attribute ("netto2");
		  nettos[2] = n.toElement ().attribute ("netto3");
		  nettos[3] = n.toElement ().attribute ("netto4");
		  cenaEdit->setText (nettos[0]);
		  spinBox2->setValue (1);
		  vat = n.toElement ().attribute ("vat");
		}
	    }
	}
    }


}


void
towList::spinChanged (int a)
{
  // // qDebug (__FUNCTION__);
  cenaEdit->setText (nettos[a - 1]);
  calcNetto ();
}


void
towList::setSelItemText ()
{
  selectedItem = nameEdit->text ();
}
