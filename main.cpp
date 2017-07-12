#include <PCASettings.h>
#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QtCore/QtCore>
#include <QtXml/QtXml>
#include <QString>
#include <iostream>
#include "stdio.h"



int main(int argc, char *argv[])
{
    QApplication b(argc, argv);

   PCASettings *a ;

PCASettings *pcaset = new  PCASettings;
 QString xmlFile = "C:/Users/ayesha/QtWorkspace/Ayesha7.xml" ;
 QHash< QString, QVariant> hash;
        pcaset->init();


 hash = pcaset->read_Config_Xml();




  qDebug() << "hash read value:" <<hash.value("COMM_CONFIG/PROTOCOL").toString() ;
}




