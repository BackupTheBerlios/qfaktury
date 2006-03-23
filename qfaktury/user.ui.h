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
#include <qsettings.h>
#include <qtextcodec.h>
#include "version.h"


/* TODO                                                 
  1. move settings to program directory
  2. change main group to elinux
*/




/*!
  * init function read settings
  !*/
void
Form2::init ()
{
  QSettings settings;
  //QDir::setPath ("$HOME/elinux");
  //settings.insertSearchPath( QSettings::Unix, "$HOME/elinux" );// move settings to program directory
  QString localEnc = settings.readEntry ("elinux/localEnc", "ISO 8859-2");
  //settings.setPath("elinux", ".qfaktury");
  QTextCodec::setCodecForCStrings (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForTr (QTextCodec::codecForName (localEnc));
  QTextCodec::setCodecForLocale (QTextCodec::codecForName (localEnc));

  nameEdit->setText (settings.readEntry ("elinux/user/nazwa"));
  placeEdit->setText (settings.readEntry ("elinux/user/miejscowosc"));
  codeEdit->setText (settings.readEntry ("elinux/user/kod"));
  addressEdit->setText (settings.readEntry ("elinux/user/adres"));
  accountEdit->setText (settings.readEntry ("elinux/user/konto"));
  nipEdit->setText (settings.readEntry ("elinux/user/nip"));
  phoneEdit->setText (settings.readEntry ("elinux/user/phone"));
  emailEdit->setText (settings.readEntry ("elinux/user/email"));
  wwwEdit->setText (settings.readEntry ("elinux/user/www"));
}

/*!
  *   save settings
  !*/
void
Form2::okClick ()
{
  QSettings settings;
  //settings.insertSearchPath( QSettings::Unix, "$HOME/elinux" );// move settings to program directory
  //settings.setPath("elinux", ".qfaktury");
  settings.beginGroup ("elinux");
  settings.writeEntry ("user/nazwa", nameEdit->text ());	// zapis String
  settings.writeEntry ("user/miejscowosc", placeEdit->text ());
  settings.writeEntry ("user/kod", codeEdit->text ());
  settings.writeEntry ("user/adres", addressEdit->text ());
  settings.writeEntry ("user/konto", accountEdit->text ());
  settings.writeEntry ("user/nip", nipEdit->text ());
  settings.writeEntry ("user/phone", phoneEdit->text ());
  settings.writeEntry ("user/email", emailEdit->text ());
  settings.writeEntry ("user/www", wwwEdit->text ());
  settings.endGroup ();
  close ();
}
