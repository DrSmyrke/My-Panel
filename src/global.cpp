#include "global.h"
#include <QSettings>
#include <QDateTime>
#include <QProcess>

namespace app {
	Config conf;
	QRect screen;
	QString localeName;

	void loadSettings()
	{
		QSettings settings("MySoft","MyPanel");

		app::conf.sync.personalDir = settings.value("SYNC/personalDir",app::conf.sync.personalDir).toString();
		app::conf.sync.user = settings.value("SYNC/user",app::conf.sync.user).toString();
		app::conf.sync.server = settings.value("SYNC/server",app::conf.sync.server).toString();
		app::conf.sync.workDir = settings.value("SYNC/workDir",app::conf.sync.workDir).toString();
		app::conf.sync.port = settings.value("SYNC/port",app::conf.sync.port).toUInt();

		settings.beginGroup("SYNC_SAVE_DIRS");
		app::conf.sync.saveDirs.clear();
		for(auto elem:settings.childKeys()) app::conf.sync.saveDirs.push_back( settings.value(elem).toString() );
		settings.endGroup();

		settings.beginGroup("BOOKMARKS");
		app::conf.bookmarks.clear();
		for(auto elem:settings.childKeys()){
			auto tmp = settings.value(elem).toString().split("	");
			if( tmp.size() < 4 ) continue;

			Bookmark bm;
			bm.name = tmp[0];
			bm.type = tmp[1];
			bm.path = tmp[2];
			bm.mount = ( tmp[3].toUInt() == 1 )?true:false;
			if( tmp.size() >= 5 ) bm.mountDir = tmp[4];
			if( tmp.size() >= 6 ) bm.mountOnStart = ( tmp[5].toUInt() == 1 )?true:false;
			if( !bm.name.isEmpty() and !bm.type.isEmpty() and !bm.path.isEmpty() ) app::conf.bookmarks.push_back( bm );
		}
		settings.endGroup();




		QSettings settingsProxy("MySoft","PROXY_SETTINGS");

		app::conf.proxy.useProxy	= settingsProxy.value( "PROXY/useProxy", app::conf.proxy.useProxy ).toBool();
		app::conf.proxy.hostName	= settingsProxy.value( "PROXY/hostName", app::conf.proxy.hostName ).toString();
		app::conf.proxy.port		= settingsProxy.value( "PROXY/port", app::conf.proxy.port ).toInt();
		app::conf.proxy.user		= settingsProxy.value( "PROXY/user", app::conf.proxy.user ).toString();
		app::conf.proxy.password	= settingsProxy.value( "PROXY/password", app::conf.proxy.password ).toString();
		app::conf.proxy.type		= settingsProxy.value( "PROXY/type", app::conf.proxy.type ).toInt();
	}

	void saveSettings()
	{
		uint16_t i = 0;

		QSettings settings("MySoft","MyPanel");
		settings.clear();

		settings.setValue("SYNC/personalDir",app::conf.sync.personalDir);
		settings.setValue("SYNC/user",app::conf.sync.user);
		settings.setValue("SYNC/server",app::conf.sync.server);
		settings.setValue("SYNC/workDir",app::conf.sync.workDir);
		settings.setValue("SYNC/port",app::conf.sync.port);

		i = 0;
		for(auto elem:app::conf.sync.saveDirs){
			settings.setValue("SYNC_SAVE_DIRS/" + QString::number(i),elem);
			i++;
		}

		i = 0;
		for(auto elem:app::conf.bookmarks){
			QString mount = ( elem.mount )?"1":"0";
			QString mountOnStart = ( elem.mountOnStart )?"1":"0";
			QString str;
			if( !elem.mountDir.isEmpty() and elem.mount ){
				str = elem.name + "	" + elem.type + "	" + elem.path + "	" + mount + "	" + elem.mountDir + "	" + mountOnStart;
			}else{
				str = elem.name + "	" + elem.type + "	" + elem.path + "	" + mount ;
			}
			settings.setValue("BOOKMARKS/" + QString::number(i),str);
			i++;
		}




		QSettings settingsProxy("MySoft","PROXY_SETTINGS");
		settingsProxy.clear();
		settingsProxy.setValue( "PROXY/useProxy", app::conf.proxy.useProxy );
		settingsProxy.setValue( "PROXY/hostName", app::conf.proxy.hostName );
		settingsProxy.setValue( "PROXY/port", app::conf.proxy.port );
		settingsProxy.setValue( "PROXY/user", app::conf.proxy.user );
		settingsProxy.setValue( "PROXY/password", app::conf.proxy.password );
		settingsProxy.setValue( "PROXY/type", app::conf.proxy.type );

		QStringList list;
		list.append( "0-DefaultProxy" );
		list.append( "1-Socks5Proxy" );
		list.append( "2-NoProxy" );
		list.append( "3-HttpProxy" );
		list.append( "4-HttpCachingProxy" );
		list.append( "4-FtpCachingProxy" );
		settingsProxy.setValue( "PROXY/typeAvailable", list.join( " | " ) );
	}

	bool parsArgs(int argc, char *argv[])
	{
		bool ret = true;
		for(int i=0;i<argc;i++){
			if(QString(argv[i]).indexOf("-")==0){
				if(QString(argv[i]) == "--help" or QString(argv[1]) == "-h"){
					printf("Usage: %s [OPTIONS]\n"
							"  -l <FILE>    log file\n"
							"  -v    Verbose output\n"
							"  --version	print current version\n"
							"\n", argv[0]);
					ret = false;
				}
				if(QString(argv[i]) == "-l") app::conf.logFile = QString(argv[++i]);
				if(QString(argv[i]) == "-v") app::conf.verbose = true;
				if(QString(argv[i]) == "--version"){
					printf( "%s\n", app::conf.version.toUtf8().data() );
					ret = false;
				}
			}
		}
		return ret;
	}

	void setLog(const uint8_t logLevel, const QString &mess)
	{
		if(app::conf.logLevel < logLevel or app::conf.logLevel == 0) return;

		QDateTime dt = QDateTime::currentDateTime();
		QString str = dt.toString("yyyy.MM.dd [hh:mm:ss] ") + mess + "\n";

		if( app::conf.verbose ){
			printf( "%s", str.toUtf8().data() );
			fflush( stdout );
		}

		if( app::conf.logFile.isEmpty() ) return;
		FILE* f;
		f = fopen(app::conf.logFile.toUtf8().data(),"a+");
		fwrite(str.toUtf8().data(),str.length(),1,f);
		fclose(f);
	}

}
