#include <qapplication.h>
#include <qtranslator.h> 
#include <qsettings.h>
#include "mainform.h"

// do splasha
#include <qlabel.h>
#include <qtimer.h>
#include <qdir.h>

#include "config.h"

int
main (int argc, char **argv)
{
  QApplication a (argc, argv);
  QRect screen = QApplication::desktop ()->screenGeometry ();

  QSettings settings;
  QString lang = settings.readEntry ("elinux/lang", "english");

  QDir abs (a.argv ()[0]);
  QString graphDir;
  if (QString (a.argv ()[0]).left (2) == "./")
    graphDir = abs.absPath ();
  else
    graphDir = "/usr/bin/qfaktury";
  //absPath();
  graphDir = graphDir.replace ("bin", "share");
  // qDebug (graphDir);
  QString langDir = graphDir + "/translations/";

  QTranslator qfaktury( 0 );
  qfaktury.load( QString( lang ), langDir  );
  a.installTranslator( &qfaktury );


#ifndef QF_nosplash__
  QLabel splash ("", 0, "splash",
		 Qt::WStyle_NoBorder | Qt::WStyle_Customize | Qt::
		 WStyle_StaysOnTop | Qt::WShowModal);
  splash.resize (380, 450);


  splash.setPixmap (QPixmap (graphDir + "/icons/splash.png"));
  splash.move (screen.center () - QPoint (splash.width () / 2,
					  splash.height () / 2));
  splash.show ();

  QTimer slip, slip2;
#endif
  Form1 w;
  w.move (screen.center () - QPoint (w.width () / 2, w.height () / 2));

#ifndef QF_nosplash__
  QObject::connect (&slip, SIGNAL (timeout ()), &splash, SLOT (close ()));
  slip.start (4000);
  w.hide ();

  QObject::connect (&slip2, SIGNAL (timeout ()), &w, SLOT (show ()));
  slip2.start (3800);
#endif

#ifdef QF_nosplash__
  w.show ();
#endif

  a.connect (&a, SIGNAL (lastWindowClosed ()), &a, SLOT (quit ()));
  return a.exec ();
}
