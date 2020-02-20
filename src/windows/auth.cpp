#include "auth.h"
#include <QLayout>
#include <QPushButton>

AuthWindow::AuthWindow(QWidget *parent) : QDialog(parent)
{
	QPushButton* submitB = new QPushButton(tr("Submit"),this);

	m_pTitle = new QLabel( this );
	m_pUsernameBox = new QLineEdit( this );
	m_pPasswordBox = new QLineEdit( this );
		m_pPasswordBox->setEchoMode( QLineEdit::EchoMode::Password );

	QWidget* centrWidget = new QWidget(this);
		QVBoxLayout* vBox = new QVBoxLayout();
		vBox->addWidget( m_pTitle );
			QHBoxLayout* userBox = new QHBoxLayout();
			userBox->addWidget(new QLabel( tr("<b>Username:</b>") ));
			userBox->addWidget(m_pUsernameBox);
		vBox->addLayout(userBox);
			QHBoxLayout* passBox = new QHBoxLayout();
			passBox->addWidget(new QLabel( tr("<b>Password:</b>") ));
			passBox->addWidget(m_pPasswordBox);
		vBox->addLayout(passBox);
			QHBoxLayout* submitBox = new QHBoxLayout();
			submitBox->addWidget(submitB);
		vBox->addLayout(submitBox);
	centrWidget->setLayout(vBox);

	this->setLayout( vBox );
	setFixedSize( 320, 140 );
	setWindowTitle( tr("Authorization") );
	setModal( true );

	connect( submitB, &QPushButton::clicked, this, &AuthWindow::accept);
}

void AuthWindow::setTarget(const QString &target)
{
	QString title = tr( "Auth to: " );
	auto tmp = target.split( "@" );
	if( tmp.size() == 2 ){
		m_pUsernameBox->setText( tmp[0] );
		m_pUsernameBox->setReadOnly( true );
		title += "<b>" + tmp[1] + "</b>";
	}else{
		title += "<b>" + target + "</b>";
	}
	m_pTitle->setText( title );
}

AuthWindow::Data AuthWindow::getData()
{
	Data data;

	data.username = m_pUsernameBox->text();
	data.password = m_pPasswordBox->text();

	return data;
}
