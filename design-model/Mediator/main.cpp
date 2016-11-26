//测试案例  
int main()  {     
    
    Mediator *mediator = new HouseMediator();  
    Person *person1 = new Renter();    //租房者  
    Person *person2 = new Landlord();  //房东  
    
    mediator->SetA(person1);  
    mediator->SetB(person2);  
    
    person1->SetMediator(mediator);  
    person2->SetMediator(mediator);  
    person1->SendMessage("我想在南京路附近租套房子，价格800元一个月\n");  
    person2->SendMessage("出租房子：南京路100号，70平米，1000元一个月\n");  
    
    delete person1; 
    delete person2; 
    delete mediator;  
    
    return 0;  
}  