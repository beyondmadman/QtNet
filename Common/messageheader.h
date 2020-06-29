#ifndef MASSAGEHEADER_H
#define MASSAGEHEADER_H

enum CMDTYPE
{
    CMD_MESSAGE,
    CMD_IMAGE
};

////包头
//struct DataHeader
//{
//    qint64 dataLength;
//    qint64 cmd;
//};

//inline QDataStream& operator<<(QDataStream& out, const DataHeader& header)
//{
//    out << header.dataLength;
//    return out;
//}

//inline QDataStream& operator>>(QDataStream& in,DataHeader& header)
//{
//   in >> header.dataLength;
//   return in;
//}

////文件
////struct NETFile : public DataHeader
////{
////    NETFile()
////    {
////        dataLength = sizeof(NETFile);
////        cmd = CMD_FILE;
////    }
////    QString fileName;      // 文件名称
////};

////消息
//struct NETMessage
//{
//    QByteArray data;          //数据
//};

//inline QDataStream& operator<<(QDataStream& out, const NETMessage& msg)
//{
//    out << msg.data;
//    return out;
//}

//inline QDataStream& operator>>(QDataStream& in,NETMessage& msg)
//{
//   in >> msg.data;
//   return in;
//}

#endif // MASSAGEHEADER_H
