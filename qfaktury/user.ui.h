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
  QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("ISO8859-2"));

  QSettings settings;
  nameEdit->setText (settings.readEntry ("przelewy/user/nazwa"));
  placeEdit->setText (settings.readEntry ("przelewy/user/miejscowosc"));
  codeEdit->setText (settings.readEntry ("przelewy/user/kod"));
  addressEdit->setText (settings.readEntry ("przelewy/user/adres"));
  accountEdit->setText (settings.readEntry ("przelewy/user/konto"));
  nipEdit->setText (settings.readEntry ("przelewy/user/nip"));
  phoneEdit->setText (settings.readEntry ("przelewy/user/phone"));
  emailEdit->setText (settings.readEntry ("przelewy/user/email"));
  wwwEdit->setText (settings.readEntry ("przelewy/user/www"));
}

/*!
  *   save settings
  !*/
void
Form2::okClick ()
{
  QSettings settings;
  settings.beginGroup ("przelewy");
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
