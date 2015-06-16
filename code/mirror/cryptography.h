#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <QObject>
#include <QCryptographicHash>

class cryptography : public QObject
{
	Q_OBJECT

public:
	cryptography(QObject *parent);
	~cryptography();
	
	static bool Encrypt(QByteArray &dst, const QByteArray &src);
	static bool Decrypt(QByteArray &dst, const QByteArray &src);

private:
	
};

#endif // CRYPTOGRAPHY_H
