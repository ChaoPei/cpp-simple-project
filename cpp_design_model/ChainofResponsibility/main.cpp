int main()  {     
    Manager *general = new ThirdManager(NULL, "A"); //设置上级，总经理没有上级  
    Manager *majordomo = new SecondManager(general, "B"); //设置上级  
    Manager *common = new FirstManager(majordomo, "C"); //设置上级  
    common->DealWithRequest("D",300);   //员工D要求加薪  
    common->DealWithRequest("E", 600);  
    common->DealWithRequest("F", 1000);  
    delete common; 
    delete majordomo; 
    delete general;  
    return 0;  
}  