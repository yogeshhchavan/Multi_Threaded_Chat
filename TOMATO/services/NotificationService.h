#ifndef NOTIFICATION_SERVICE_H
#define NOTIFICATION_SERVICE_H

#include<iostream>
#include "../models/Order.h"
using namespace std;

class NotificationService {
public:
    static void notify(Order* order) {
        cout<< "\nNotification : New "<<order->getType()<<"order places!"<<endl;
        cout<<"________________________________________________________"<<endl;
        cout<<"Order ID: "<<order->getOrderId()<<endl;
        cout<<"Customer: "<<order->getUser()->getName()<<endl;
        cout<<"Restaurant: "<<order->getRestaurant()->getName()<<endl;
        cout<<"Items Ordered: "<<endl;

        const vector<MenuItem>& items = order->getItems();
        for(const auto& item : items){
            cout<<" - "<<item.getName()<<" ($"<<item.getPrice()<<")"<<endl;
        }

        cout<<"Total : $"<<order->getTotal()<<endl;
        cout<<"Scheduled For: "<<order->getScheduled()<<endl;
        cout<<"Payment Done :"<<endl;
        cout<<"________________________________________________________\n"<<endl;   

    }

};
#endif // NOTIFICATION_SERVICE_H