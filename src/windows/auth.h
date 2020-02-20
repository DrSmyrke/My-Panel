#ifndef AUTH_H
#define AUTH_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include "global.h"

class AuthWindow : public QDialog
{
	Q_OBJECT
public:
	struct Data
	{
		QString username;
		QString password;
	};
	explicit AuthWindow(QWidget *parent = 0);
	void setTarget(const QString &target);
	Data getData();
private:
	QLineEdit* m_pUsernameBox;
	QLineEdit* m_pPasswordBox;
	QLabel* m_pTitle;
};

#endif // AUTH_H
