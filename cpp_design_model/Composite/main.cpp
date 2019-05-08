int main() {  
    Company *root = new ConcreteCompany("总公司");  
    Company *leaf1=new FinanceDepartment("财务部");  
    Company *leaf2=new HRDepartment("人力资源部");  
    root->Add(leaf1);  
    root->Add(leaf2);  
  
    //分公司A  
    Company *mid1 = new ConcreteCompany("分公司A");  
    Company *leaf3=new FinanceDepartment("财务部");  
    Company *leaf4=new HRDepartment("人力资源部");  
    mid1->Add(leaf3);  
    mid1->Add(leaf4);  
    root->Add(mid1);  
    
    //分公司B  
    Company *mid2=new ConcreteCompany("分公司B");  
    FinanceDepartment *leaf5=new FinanceDepartment("财务部");  
    HRDepartment *leaf6=new HRDepartment("人力资源部");  
    mid2->Add(leaf5);  
    mid2->Add(leaf6);  
    root->Add(mid2);  
    root->Show(0);  
  
    delete leaf1; delete leaf2;  
    delete leaf3; delete leaf4;  
    delete leaf5; delete leaf6;   
    delete mid1; delete mid2;  
    delete root;  
    return 0;  
}  