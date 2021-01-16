#include "jsonchecker.h"
#include<json/writer.h>
#include<json/reader.h>
#include<signedcrypty.h>
#include<stringformat.h>
#include"md5.h"
#include"signedcrypty.h"
#include<ilog.h>

const std::string RawTx = "source:::";
const std::string SignedKey = "print:::";

const std::vector<Json::Value> EmptyVector;

char JsonChecker::TransactionSpliter = '@';

std::string JsonChecker::PackJson(const std::vector<Json::Value>& Jsons,const std::string& FingerPrintStr,bool CheckDate)
{
    Json::FastWriter writer;
    Json::Value Trans(Json::arrayValue);

    for(auto Json:Jsons)
    {
        Trans.append(Json);
    }

    const std::string JsonStr =writer.write(Trans);
    const std::vector<std::string> ToPrint ={JsonStr,FingerPrintStr,FingerPrintStr};
    bitstar::common::LogProxy::verbosef("UnpackedJson length=%d",ToPrint[0].length());
    bitstar::common::LogProxy::verbose(ToPrint[0]);
    const std::string Print =SignedCrypty::GetRawTxSigned(ToPrint,CheckDate);
    Json::Value Result;
    Result[RawTx] = Trans;
    Result[SignedKey]=Print;
    const std::string PrintedTrans = writer.write(Result);
//    std::cout<<"JsonStr:"<<JsonStr<<std::endl
//            <<"FingerPrintStr:"<<FingerPrintStr<<std::endl
//           <<"PrintedTrans:"<<PrintedTrans<<std::endl;
    return PrintedTrans;
}
std::vector<Json::Value> JsonChecker::UnpackJson(const std::string& SignedStr,const std::string& FingerPrintStr,std::string& Result,bool CheckDate)
{
    Json::Reader Reader;
    Json::Value Jsons;
    if(!Reader.parse(SignedStr,Jsons,false))
    {
        Result=bitstar::common::StringFormat::strfmt("Json格式解析错误,接收字符长度=%d！",SignedStr.length());
        return EmptyVector;
    }
    if((!Jsons.isObject()) || (!Jsons.isMember(RawTx) || (!Jsons.isMember(SignedKey))))
    {
        Result=bitstar::common::StringFormat::strfmt("Json字段解析错误，没有Print或RawData字段,接收字符长度=%d！",SignedStr.length());
        return EmptyVector;
    }
    Json::Value UnpackJson = Jsons[RawTx];
    const std::string SourcePrint = Jsons.get(SignedKey,"").asCString();
    Json::FastWriter writer;
    std::string JsonStr = writer.write(UnpackJson);
    const std::string UnpackedJson=JsonStr; //bitstar::common::StringFormat::trimEnd(JsonStr,"\n");
    //bitstar::common::LogProxy::verbose("UnpackedJson :"+UnpackedJson);
    const std::vector<std::string> ToPrint ={UnpackedJson,FingerPrintStr,FingerPrintStr};
    bitstar::common::LogProxy::verbosef("UnpackedJson length=%d",ToPrint[0].length());
    bitstar::common::LogProxy::verbose(ToPrint[0]);
    const std::string Print =SignedCrypty::GetRawTxSigned(ToPrint,CheckDate);
//    std::cout<<"SignedStr:"<<SignedStr<<std::endl
//            <<"UnpackJson:"<<UnpackJson<<std::endl
//            <<"FingerPrintStr:"<<FingerPrintStr<<std::endl
//           <<"Result:"<<Result<<std::endl;
    if(Print!=SourcePrint)
    {
        Result="指纹校验错误:"+Print;
        return EmptyVector;
    }
    std::vector<Json::Value> ResultJson;
    for(unsigned int i=0;i<UnpackJson.size();i++)
    {
        ResultJson.push_back(UnpackJson.get(i,""));
    }
    return ResultJson;
}

std::string Md5Vector(const std::vector<std::string> Strings,const std::string& FingerPrintStr,bool CheckDate)
{
    std::vector<std::string> Input(Strings);
    Input.push_back(FingerPrintStr);
    Input.push_back(FingerPrintStr);
    std::string VecString = bitstar::common::StringFormat::Join(Input,JsonChecker::TransactionSpliter);
    if(CheckDate)
    {
        VecString+=GetCurrentDate();
    }
    const string Print = MD5(VecString).toStr();
    return Print;
}
std::string JsonChecker::PackString(const std::vector<std::string>& Strings,const std::string& FingerPrintStr,bool CheckDate)
{
//    Json::Value StrJson(Json::arrayValue);
//    for(const std::string str:Strings)
//    {
//        StrJson.append(str);
//    }
    std::vector<std::string> Input(Strings);
    std::string Print = Md5Vector(Input,FingerPrintStr,CheckDate);
    Input.push_back(Print);
    return bitstar::common::StringFormat::Join(Input,TransactionSpliter);
}

std::vector<std::string> JsonChecker::UnpackString(const std::string& SignedStr,const std::string& FingerPrintStr,std::string& Result,bool CheckDate)
{
     std::vector<std::string> Vect = bitstar::common::StringFormat::split(SignedStr,TransactionSpliter);
     if(Vect.size()<2)
     {
         Result="error->Data Length less than 2...";
         Vect.clear();
         return Vect;
     }
     const std::string Print = bitstar::common::StringFormat::trimEnd(Vect[Vect.size()-1],"\n");
     Vect.pop_back();
     const std::string CheckPrint =Md5Vector(Vect,FingerPrintStr,CheckDate);
     if(CheckPrint == Print)
     {
        return Vect;
     }
     bitstar::common::LogProxy::errorf("Print Check Error: %s != %s",CheckPrint.c_str(),Print.c_str());
     Result="Print Check Failed!";
     Vect.clear();
     return Vect;
}
