## Basic structure homework
Implement object with name “MyObject” which is parent of kernel_kobj.    
Object should include linked_list structure.    
This object should contain sysfs attribute with name “list”.    
On read form attribute “list” it should show content of the objects linked list.    
On write to attribute “list” it should add new string to the objects linked list.    
!! Do not forget properly free all the resources during rmmod.    
