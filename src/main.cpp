#include "mainwindow.h"
#include "version.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLockFile>
#include <QMessageBox>
#include <QNetworkProxy>
#include "global.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QLockFile lockFile(QDir::temp().absoluteFilePath("mypanel.lock"));
	/* Пытаемся закрыть Lock File, если попытка безуспешна в течение 100 миллисекунд,
	 * значит уже существует Lock File созданный другим процессом.
	 * Следовательно, выбрасываем предупреждение и закрываем программу
	 * */
	if(!lockFile.tryLock(100)){
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText("Приложение уже запущено.\n"
					   "Разрешено запускать только один экземпляр приложения.");
		msgBox.exec();
		return 1;
	}

	app::conf.version = QString("%1.%2").arg(APP_VER_FIRST).arg(APP_VER_SECOND);

	app::localeName = QLocale::system().name();
	QTranslator translator(&a);
	if(translator.load(app::localeName,"://lang/")) a.installTranslator(&translator);

	app::loadSettings();
	if( !app::parsArgs(argc, argv) ) return 0;

	if( app::conf.proxy.useProxy ){
		QNetworkProxy proxy;
		switch (app::conf.proxy.type) {
			case 0:		proxy.setType(QNetworkProxy::DefaultProxy);		break;
			case 1:		proxy.setType(QNetworkProxy::Socks5Proxy);		break;
			case 2:		proxy.setType(QNetworkProxy::NoProxy);			break;
			case 3:		proxy.setType(QNetworkProxy::HttpProxy);		break;
			case 4:		proxy.setType(QNetworkProxy::HttpCachingProxy);	break;
			case 5:		proxy.setType(QNetworkProxy::FtpCachingProxy);	break;
			default:	proxy.setType(QNetworkProxy::NoProxy);			break;
		}
		proxy.setHostName( app::conf.proxy.hostName );
		proxy.setPort( app::conf.proxy.port );
		proxy.setUser( app::conf.proxy.user );
		proxy.setPassword( app::conf.proxy.password );
		QNetworkProxy::setApplicationProxy(proxy);
	}

	MainWindow w;
	w.show();
    w.run();

	return a.exec();
}
