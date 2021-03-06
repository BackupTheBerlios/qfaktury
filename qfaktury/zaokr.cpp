#include "zaokr.h"

/*!
  Question: what currency separator is used in other countries?
@todo
  ask someone!!
*/


/*!
  This round digits to the 2 places after commas.
  You should never use this, see fixstr
  */
QString
doRound (QString in)
{
  QString tmp = in;
  QString reszta = tmp.remove (0, tmp.find (",") + 1);
  if (reszta.length () > 2)
    {
      tmp = in;
      QString calk = tmp.remove (tmp.find (","), reszta.length () + 1);	// liczba calk
      // int last = reszta.right(1).toInt();
      // first 
      // if we  have 0,12321321321
      // the wy modify this to: 0,123
      // second
      // roud (depending on last digit >5 or < 5)
      reszta = "0." + reszta;
      double dOriginal = reszta.toDouble ();
      double iTemp = 0;
      //  qDebug(QString::number(last));
      // if (last < 5)
      iTemp = floor ((dOriginal + .005) * 100);
      /*
         if (last > 5)
         iTemp = floor((dOriginal - .005) * 100);
       */
      dOriginal = iTemp / 100;
      // qDebug("!!!" + reszta + " | | " + calk );
      QString out = QString::number (calk.toInt () + dOriginal);
// in = in.replace(",", ".");
      return out;
    }
  else
    {
      return in;
    }
}

QString
addZeros (QString in)
{
  // ie. 0, then return 0,00
  // qDebug("? 1 " + in);
  QString sth = in;
  QString reszta = sth.remove (0, sth.find (",") + 1);
  // qDebug( reszta + " " + in );
  if (reszta.length () == 1)
    {
      if (in.find (',', 0) == -1)
	{
	  return in + ",00";
	}
      return in + "0";
    }
  return in;
}

float
doRound2 (float a, bool round)
{
  return ((float) ((int) (a * 100 + (round ? 0.5 : 0)))) / 100;
}

/*!
  This round or just add two zeros
  Use this function when need to rouind sth.
  */
QString
fixStr (QString in)
{
  in = in.replace (".", ",");
  QString tmp = in;
  if (tmp.find (',', 0) == -1)
    {
      return tmp + ",00";
    }
  return addZeros (doRound (in).replace (".", ","));
}


/*!
 This calculate brutto  
  */
float
getPrice (QString count, QString netto, QString vat)
{
  int cnt = count.toInt ();
  float nt = netto.replace (",", ".").toFloat ();
  float tmpVt = vat.toFloat ();
  float vt = (tmpVt / 100) + 1;
  return (nt * vt) * cnt;
}

/*!
 This calculate brutto  
  */
float
getPriceBrutto (double netto, QString vat)
{
  float nt = netto;		// .replace (",", ".").toFloat ();
  float tmpVt = vat.toFloat ();
  float vt = (tmpVt / 100) + 1;
  return nt * vt;
}


/*!
 This calculate netto
  */
float
getPrice2 (QString count, QString netto)
{
  int cnt = count.toInt ();
  float nt = netto.replace (",", ".").toFloat ();
  return nt * cnt;
}

/*!
 This returns number as three chars numbers
  */
QString 
threePlaces(int in)
{
  QString tmp;
  tmp = QString::number(in);
  // uint length () const 
  if ( tmp.length() == 1 ) return "00" + tmp;
  if ( tmp.length() == 2 ) return "0" + tmp;
  return tmp;
}