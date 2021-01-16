- mysql
  - 10亿级数据读写性能优化-sql优化，减少查磁盘次数，热点数据在缓存(https://zhuanlan.zhihu.com/p/65811829)
  - 分片库
  - 一主多从库，读写分离(尽可能读从库)
  - https://cloud.tencent.com/developer/article/1145377 mmap
  
- 分布式数据库, 分布式锁，各种一致性算法(raft等)

- MySQL innodb原理，看官方文档


==>

string GetCurrentDate()
{
    time_t t = time(NULL);
    tm *ptm = gmtime(&t);
    char s[80];
    strftime(s, 80, "%Y%m%d", ptm);
    return string(s);
}

string SignedCrypty::GetRawTxSigned(const vector<string> &args,bool CheckDate)
{
    if(args.size() != 3)
    {
        return "null";
    }
    const string md5_tx = MD5(args[0]).toStr();
    const string md5_pub = MD5(args[1]).toStr(); //PRODUCT_MYZPRODUCT_MYZPRODUCT_MYZPRODUCT_MYZPRODUCT_MYZ
    const string md5_pwd = MD5(args[2]).toStr(); //PRODUCT_MYZPRODUCT_MYZPRODUCT_MYZPRODUCT_MYZPRODUCT_MYZ
    const std::string DataStr = CheckDate?GetCurrentDate():"2018-4-25";
    const string md5_date = MD5(DataStr).toStr();

    char append_char[129];
    for(int i=0;i<32;i++)
    {
        append_char[i] = md5_tx[i];
        append_char[i + 32] = md5_pub[i];
        append_char[i + 64] = md5_pwd[i];
        append_char[i + 96] = md5_date[i];
    }
    append_char[128] = '\0';
    string append_str(append_char);
    return MD5(append_str).toStr();
}