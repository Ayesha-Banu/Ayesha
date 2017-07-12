#include "pcasettings.h"

struct PCASettingsData {
    PCASettingsData(QString filename = nullptr) :
        rootName("PCA"),
        fileName(filename),
        xmlFormat(QSettings::InvalidFormat)


    {

    }
    const QString rootName;
    const QString fileName;
    QSettings::Format xmlFormat;
    QHash <QString,QVariant> hash;



};

struct XmlNode : public QObject {
    XmlNode(const QString &name, const QString &text = QString(),  QObject *parent = 0) :
        tagName(name),
        subtext(text),
        QObject(parent)
    {
    }
    QString tagName;
    QString subtext;

};


void PCASettings::init() {
    QString file = "C:/Users/ayesha/QtWorkspace/Ayesha7.xml";
    d = new PCASettingsData(file);
    qDebug("In init");
    d->hash.clear();
    d->xmlFormat  = QSettings::registerFormat("xml",( bool (*)(QIODevice&,  QMap<QString, QVariant>&))( &PCASettings::readSettingsXml),( bool (*)(QIODevice&, const QMap<QString, QVariant>&))( &PCASettings::writeSettingsXml));
    qDebug() <<   d->xmlFormat ;

}

PCASettings::~PCASettings()
{
    d->hash.~QHash();
      delete d;
      d = 0;
}



QHash<QString, QVariant> PCASettings::read_Config_Xml()
{
    // this->create_Config_Xml();

       qDebug() << d->xmlFormat;
       qDebug() << "Ay";
       QSettings settings(d->fileName,(d->xmlFormat),0);
       qDebug() << settings.fileName();

       QFile file;
       if (!file.exists(d->fileName)){

           qDebug() << "file doesnt exist";
           return d->hash;}

       qDebug() << "file is found";

       QStringList keys = settings.allKeys();
       int i;
       int count = keys.count();
       qDebug() << count;
       for(i = 0; i <count; i++){
           qDebug() << settings.value(keys.value(i));

           d->hash.insert( keys.value(i),settings.value(keys.value(i)));}

       return d->hash;
}

int PCASettings::create_Config_Xml()
{
    /** hash for storing default values of PCA settings **/
       QHash< QString, QVariant> hashDef;

       QSettings settings(d->fileName,d->xmlFormat,0);
       qDebug() << settings.fileName();
       QFile file;
       if (!file.exists(d->fileName)) return -1;

       /* ** PCA settings ** */
       hashDef.insert(" ID", "1");
       /**SENSOR1 settings**/
       hashDef.insert("SENSORS/SENSOR1/ID", "1");
       hashDef.insert("SENSORS/SENSOR1/NAME", "EH3150");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/TYPE", "EO");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/ZOOM/MIN_ZOOM", "1");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/ZOOM/MAX_ZOOM", "10");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/ZOOM/ZOOM_NUM_LEVELS", "10");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/ZOOM/TYPE", "O");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/FOV/MIN_FOV", "4.6");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/FOV/MAX_FOV", "53.4");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/FRAME_RATE", "30");
       hashDef.insert("SENSORS/SENSOR1/CAPABILITIES/RESOLUTION", "1280x720");
       /**GIMBAL settings ..???? to be made more detailed**/
       hashDef.insert("GIMBAL/GIMBAL1/ID","1");
       hashDef.insert("GIMBAL/GIMBAL1/CONNECTED_SENSORS","1");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION","R-P");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION/LIMITS/R_MIN","-40");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION/LIMITS/R_MAX","+40");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION/LIMITS/P_MIN","-20");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION/LIMITS/P_MAX","+60");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION/ACTUATION","Brushless");
       hashDef.insert("GIMBAL/GIMBAL1/CAPABILITIES/CONFIGURATION/CONTROLLER","Alexmos");
       /**VEHICLE settings ... ***/
       hashDef.insert("VEHICLE_CONFIG/ID","1");
       hashDef.insert("VEHICLE_CONFIG/FLIGHT_CONTROLLER","Atom");
       hashDef.insert("VEHICLE_CONFIG/PROTOCOL","E-Link");
       hashDef.insert("VEHICLE_CONFIG/TYPE","FW");
       /**COMMUNICATION settings ***/
       hashDef.insert("COMM_CONFIG/PCA_PORT","10001");
       hashDef.insert("COMM_CONFIG/PROTOCOL","Elink");
       hashDef.insert("COMM_CONFIG/AP/IP","192.168.1.150");
       hashDef.insert("COMM_CONFIG/AP/PORT","20001");
       /* * PCA CAPABILITIES ** */
       hashDef.insert("CAPABILITIES/MODES/ANGLE","undefined");
       hashDef.insert("CAPABILITIES/MODES/TRACKING","undefined");
       hashDef.insert("CAPABILITIES/MODES/TRACKING/GEO_LOCK","undefined");
       hashDef.insert("CAPABILITIES/MODES/TRACKING/OBJECT_LOCK","undefined");
       hashDef.insert("CAPABILITIES/MODES/TRACKING/OBJECT_LOCK_AND_FOLLOW","undefined");
       hashDef.insert("CAPABILITIES/MODES/MAPPING","undefined");



       QHashIterator<QString, QVariant> k(hashDef);
       while (k.hasNext())
       {
           k.next();
           settings.setValue(k.key(), k.value());
       }


       settings.sync();
       return 0;




}

bool PCASettings::readSettingsXml(QIODevice &device, QMap<QString, QVariant> &map)
{

    QString rootName = "PCA";
       map.clear();
   //    qDebug() << "In Read SettingS";
       QXmlStreamReader xml(&device);

       QPointer<XmlNode> curNode = 0;

       qDebug() << xml.name().toString();
       while(!xml.atEnd())
       {
           qDebug() << curNode->parent();
           switch(xml.readNext())
           {
           case QXmlStreamReader::StartElement:
               qDebug() << "1" << xml.name().toString();
               if(curNode != 0)
                   //we're already processing the file if there already is a current node
                   curNode = new XmlNode(xml.name().toString(), QString(), curNode);
               else if(xml.name().toString() == rootName)
                   //no current node? this must be the first one: the root
                   curNode = new XmlNode(rootName);
               else
                   return false; // invalid format: first element *must* be root tag

               break;

           case QXmlStreamReader::EndElement:
               qDebug() << "3 In Read case 1";
               //if current node has no parent, that means we just closed the root tag
               //we're done!
               if(!curNode->parent())
               {
                   delete curNode;
                   return true;
               }

               //otherwise, we just closed the current category.
               //on the next loop iteration, we should get either the start of the next category or the closing tag of the parent (either the parent category or the "parent" leaf name)
               else
                   curNode = (XmlNode*) QScopedPointer<XmlNode>(curNode)->parent();

               break;

           case QXmlStreamReader::Characters:
               qDebug() << "2" << xml.text().toString();
               if(!xml.isWhitespace())
                   map[this->fullPath(curNode)] = xml.text().toString();
               break;
           }
       }

       qDebug() << "reached error point";
       //if it gets here, an error occured.
       map.clear();
       return true;


}

bool PCASettings::writeSettingsXml(QIODevice &device, const QMap<QString, QVariant> &map)
{


    XmlNode *root = new XmlNode("PCA");


      /************************************************************/
      //step 1/2: process the structure of the settings map & the keys. the purpose
      //of this step is to put all the keys of one category next to each other.
      //but we do not sort within the category. in this step, we place our results
      //from the QStringList of QMap.keys() into a tree-like structure
      foreach(const QString &unsplitKey, map.keys())
      {
          QStringList segs = unsplitKey.split("/", QString::SkipEmptyParts);
          QString val = map[unsplitKey].toString();

          XmlNode *cur = root;

          //for each segment of the current key, we loop through the branches of
          //our tree looking for appropriate branches/leaves. on the way down to
          //the specific leaf we want, we create & add nodes as needed.

          for(int i = 0; i < segs.length(); i++)
          {
              if(i == segs.length() - 1)
              {
                  //the last segment is a leaf that wasn't previously found.
                  //we don't keep the ref since it becomes a child of the parent
                  new XmlNode(segs[i], val, cur);
              }
              else
              {
                  //search for the node for the current segment. create it as a
                  //child of the current node if it doesn't exist. then we use it
                  //for the next iteration
                  XmlNode *foundItem = 0;
                  foreach(QObject *object, cur->children())
                  {
                      XmlNode *child = (XmlNode*) object;
                      if(0 == QString::compare(child->tagName, segs[i], Qt::CaseInsensitive))
                      {
                          foundItem = child;
                          break;
                      }
                  }

                  if(!foundItem)
                      foundItem = new XmlNode(segs[i], QString(), cur);

                  cur = foundItem;
              }
          }
      }

      QXmlStreamWriter xml(&device);
      xml.setAutoFormatting(true);
      xml.setAutoFormattingIndent(-1);
      xml.writeStartDocument();

      QList<XmlNode*> stack;
      stack << root;

      while(true)
      {
          //see step 1
          XmlNode *cur;
          while((cur = stack.takeLast()) == 0)
          {
              xml.writeEndElement();

              if(stack.isEmpty())
              {
                  xml.writeEndDocument();
                  delete root;
                  return true;
              }
          }

          //see step 2
          xml.writeStartElement(cur->tagName);
          stack << 0; // required to close text-only elements as well as for nodes with children to go back up a level when children are processed.

          //see step 3
          if(cur->children().size() == 0)
              xml.writeCharacters(cur->subtext);
          else
              for(int i = 0; i < cur->children().length(); i++)
                  stack << (XmlNode*) cur->children()[i];

      }

      //should *never* get here
      return false;

}

QString PCASettings::fullPath(const XmlNode *cur) const
{
    QString rootName = "PCA";


       QString path = cur->tagName;

       while((cur = (const XmlNode *) cur->parent()) != 0)
           path.prepend(cur->tagName + "/");

       return path.mid(rootName.size() + 1); // remove root node & trailing slash


}
